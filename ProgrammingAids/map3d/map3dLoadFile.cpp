/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       map3dLoadFile.cpp                                                       */
/*                                                                                      */
/*  Contains:   The low-level support routines for the map3d module.                    */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2001 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
/*                                                                                      */
/*              All rights reserved. Redistribution and use in source and binary forms, */
/*              with or without modification, are permitted provided that the following */
/*              conditions are met:                                                     */
/*                * Redistributions of source code must retain the above copyright      */
/*                  notice, this list of conditions and the following disclaimer.       */
/*                * Redistributions in binary form must reproduce the above copyright   */
/*                  notice, this list of conditions and the following disclaimer in the */
/*                  documentation and/or other materials provided with the              */
/*                  distribution.                                                       */
/*                * Neither the name of the copyright holders nor the names of its      */
/*                  contributors may be used to endorse or promote products derived     */
/*                  from this software without specific prior written permission.       */
/*                                                                                      */
/*              THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS     */
/*              "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT       */
/*              LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A */
/*              PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT      */
/*              OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,   */
/*              SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT        */
/*              LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,   */
/*              DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY   */
/*              THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT     */
/*              (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE   */
/*              OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.    */
/*                                                                                      */
/*  Created:    2001/04/07                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "map3d.h"

/*------------------------------------ map3dClearRangeList ---*/
void
map3dClearRangeList(Map3dData * xx)
{
    if (xx)
    {
        while (xx->fFirstRange)
        {
            RangeData * walker = xx->fFirstRange;

            xx->fFirstRange = walker->fNext;
            FREE_BYTES(walker->fOutput);
            FREE_BYTES(walker);
        }
        xx->fPreviousResult = xx->fLastRange = NULL;
        xx->fRangeCount = 0;
    }
} // map3dClearRangeList

/*------------------------------------ map3dGetNextAtomInBuffer ---*/
static bool
map3dGetNextAtomInBuffer(Map3dData * xx,
                         t_atom &    value)
{
    t_atom skipper;
    bool   result = true;

    for ( ; result; )
    {
        result = (! binbuf_getatom(xx->fBuffer, &xx->fBufferTypeOffset, &xx->fBufferStuffOffset,
                                   &value));
        if (result && (A_SYM == value.a_type))
        {
            if (kCommentCharacter == value.a_w.w_sym->s_name[0])
            {
                /* skip a comment */
                for ( ; result; )
                {
                    result = (! binbuf_getatom(xx->fBuffer, &xx->fBufferTypeOffset,
                                               &xx->fBufferStuffOffset, &skipper));
                    if (A_SEMI == skipper.a_type)
                    {
                        break;
                    }
                }
                /* At this point, we've read the trailing semicolon, so we need to get the next
                 atom. */
            }
            else
            {
                /* We've seen a symbol other than '#' */
                break;
            }
        }
        else
        {
            /* We've seen something other than a symbol, so we can leave... */
            break;
        }
    }
    return result;
} // map3dGetNextAtomInBuffer

/*------------------------------------ map3dReportUnexpected ---*/
static void
map3dReportUnexpected(Map3dData * xx,
                      t_atom &    what)
{
    switch (what.a_type)
    {
        case A_FLOAT:
            LOG_ERROR_2(xx, OUTPUT_PREFIX "unexpected floating point (%g)",
                        static_cast<double>(what.a_w.w_float))
            break;

        case A_LONG:
            LOG_ERROR_2(xx, OUTPUT_PREFIX "unexpected long (" LONG_FORMAT ")", what.a_w.w_long)
            break;

        case A_SYM:
            LOG_ERROR_2(xx, OUTPUT_PREFIX "unexpected symbol (%s)", what.a_w.w_sym->s_name)
            break;

        case A_SEMI:
            LOG_ERROR_1(xx, OUTPUT_PREFIX "unexpected semicolon")
            break;

        case A_COMMA:
            LOG_ERROR_1(xx, OUTPUT_PREFIX "unexpected comma")
            break;

        case A_DOLLAR:
        case A_DOLLSYM:
            LOG_ERROR_1(xx, OUTPUT_PREFIX "unexpected dollar")
            break;

        default:
            LOG_ERROR_2(xx, OUTPUT_PREFIX "unexpected atom, type=%d", static_cast<int>(what.a_type))
            break;
            
    }
} // map3dReportUnexpected

/*------------------------------------ map3dCollectARange ---*/
static bool
map3dCollectARange(Map3dData * xx)
{
    t_atom      holder;
    bool        result = map3dGetNextAtomInBuffer(xx, holder);
    bool        backClosed = false;
    bool        bottomClosed = false;
    bool        forwardClosed = false;
    bool        leftClosed = false;
    bool        rightClosed = false;
    bool        topClosed = false;
    bool        bottomTopDontCare = false;
    bool        leftRightDontCare = false;
    bool        forwardBackDontCare = false;
    double      backFloatValue;
    double      bottomFloatValue;
    double      forwardFloatValue;
    double      leftFloatValue;
    double      rightFloatValue;
    double      topFloatValue;
    t_atom_long backIntValue;
    t_atom_long bottomIntValue;
    t_atom_long forwardIntValue;
    t_atom_long leftIntValue;
    t_atom_long rightIntValue;
    t_atom_long topIntValue;
    Category    backMatch = MatchUnknown;
    Category    bottomMatch = MatchUnknown;
    Category    forwardMatch = MatchUnknown;
    Category    leftMatch = MatchUnknown;
    Category    rightMatch = MatchUnknown;
    Category    topMatch = MatchUnknown;
    long        outputCount = 0;
    t_binbuf *  collector = NULL;
    long        dollarsPresent = 0;
    long        doubleDollarsPresent = 0;
    long        singleDollarsPresent = 0;

    // Get the left/right pair
    if (result)
    {
        if (A_SYM == holder.a_type)
        {
            if (holder.a_w.w_sym == gAsteriskSymbol)
            {
                leftRightDontCare = true;
            }
            else if (holder.a_w.w_sym == gOpenSquareSymbol)
            {
                leftClosed = true;
            }
            else if (holder.a_w.w_sym != gOpenRoundSymbol)
            {
                map3dReportUnexpected(xx, holder);
                result = false;
            }
        }
        else
        {
            map3dReportUnexpected(xx, holder);
            result = false;
        }
    }
    // Get left value
    if (result && (! leftRightDontCare))
    {
        result = map3dGetNextAtomInBuffer(xx, holder);
        if (result)
        {
            switch (holder.a_type)
            {
                case A_SYM:
                    if ((holder.a_w.w_sym == gNegInfSymbol1) ||
                        (holder.a_w.w_sym == gNegInfSymbol2))
                    {
                        leftMatch = MatchInfinity;
                    }
                    else
                    {
                        map3dReportUnexpected(xx, holder);
                        result = false;
                    }
                    break;

                case A_FLOAT:
                    leftMatch = MatchFloat;
                    leftFloatValue = holder.a_w.w_float;
                    break;

                case A_LONG:
                    leftMatch = MatchInteger;
                    leftIntValue = holder.a_w.w_long;
                    break;

                default:
                    map3dReportUnexpected(xx, holder);
                    result = false;
                    break;
                    
            }
        }
    }
    // Get right value
    if (result && (! leftRightDontCare))
    {
        result = map3dGetNextAtomInBuffer(xx, holder);
        if (result)
        {
            switch (holder.a_type)
            {
                case A_SYM:
                    if ((holder.a_w.w_sym == gPosInfSymbol1) ||
                        (holder.a_w.w_sym == gPosInfSymbol2) ||
                        (holder.a_w.w_sym == gPosInfSymbol3) ||
                        (holder.a_w.w_sym == gPosInfSymbol4))
                    {
                        rightMatch = MatchInfinity;
                    }
                    else
                    {
                        map3dReportUnexpected(xx, holder);
                        result = false;
                    }
                    break;

                case A_FLOAT:
                    rightMatch = MatchFloat;
                    rightFloatValue = holder.a_w.w_float;
                    break;

                case A_LONG:
                    rightMatch = MatchInteger;
                    rightIntValue = holder.a_w.w_long;
                    break;

                default:
                    map3dReportUnexpected(xx, holder);
                    result = false;
                    break;
                    
            }
        }
    }
    // Get right bracket
    if (result && (! leftRightDontCare))
    {
        result = map3dGetNextAtomInBuffer(xx, holder);
        if (A_SYM == holder.a_type)
        {
            if (holder.a_w.w_sym == gCloseSquareSymbol)
            {
                rightClosed = true;
            }
            else if (holder.a_w.w_sym != gCloseRoundSymbol)
            {
                map3dReportUnexpected(xx, holder);
                result = false;
            }
        }
        else
        {
            map3dReportUnexpected(xx, holder);
            result = false;
        }
    }
    // Get the bottom/top pair
    if (result)
    {
        result = map3dGetNextAtomInBuffer(xx, holder);
        if (A_SYM == holder.a_type)
        {
            if (holder.a_w.w_sym == gAsteriskSymbol)
            {
                bottomTopDontCare = true;
            }
            else if (holder.a_w.w_sym == gOpenSquareSymbol)
            {
                bottomClosed = true;
            }
            else if (holder.a_w.w_sym != gOpenRoundSymbol)
            {
                map3dReportUnexpected(xx, holder);
                result = false;
            }
        }
        else
        {
            map3dReportUnexpected(xx, holder);
            result = false;
        }
    }
    // Get bottom value
    if (result && (! bottomTopDontCare))
    {
        result = map3dGetNextAtomInBuffer(xx, holder);
        if (result)
        {
            switch (holder.a_type)
            {
                case A_SYM:
                    if ((holder.a_w.w_sym == gNegInfSymbol1) ||
                        (holder.a_w.w_sym == gNegInfSymbol2))
                    {
                        bottomMatch = MatchInfinity;
                    }
                    else
                    {
                        map3dReportUnexpected(xx, holder);
                        result = false;
                    }
                    break;

                case A_FLOAT:
                    bottomMatch = MatchFloat;
                    bottomFloatValue = holder.a_w.w_float;
                    break;

                case A_LONG:
                    bottomMatch = MatchInteger;
                    bottomIntValue = holder.a_w.w_long;
                    break;

                default:
                    map3dReportUnexpected(xx, holder);
                    result = false;
                    break;
                    
            }
        }
    }
    // Get top value
    if (result && (! bottomTopDontCare))
    {
        result = map3dGetNextAtomInBuffer(xx, holder);
        if (result)
        {
            switch (holder.a_type)
            {
                case A_SYM:
                    if ((holder.a_w.w_sym == gPosInfSymbol1) ||
                        (holder.a_w.w_sym == gPosInfSymbol2) ||
                        (holder.a_w.w_sym == gPosInfSymbol3) ||
                        (holder.a_w.w_sym == gPosInfSymbol4))
                    {
                        topMatch = MatchInfinity;
                    }
                    else
                    {
                        map3dReportUnexpected(xx, holder);
                        result = false;
                    }
                    break;

                case A_FLOAT:
                    topMatch = MatchFloat;
                    topFloatValue = holder.a_w.w_float;
                    break;

                case A_LONG:
                    topMatch = MatchInteger;
                    topIntValue = holder.a_w.w_long;
                    break;

                default:
                    map3dReportUnexpected(xx, holder);
                    result = false;
                    break;
                    
            }
        }
    }
    // Get top bracket
    if (result && (! bottomTopDontCare))
    {
        result = map3dGetNextAtomInBuffer(xx, holder);
        if (A_SYM == holder.a_type)
        {
            if (holder.a_w.w_sym == gCloseSquareSymbol)
            {
                topClosed = true;
            }
            else if (holder.a_w.w_sym != gCloseRoundSymbol)
            {
                map3dReportUnexpected(xx, holder);
                result = false;
            }
        }
        else
        {
            map3dReportUnexpected(xx, holder);
            result = false;
        }
    }
    // Get the forward/back pair
    if (result)
    {
        result = map3dGetNextAtomInBuffer(xx, holder);
        if (A_SYM == holder.a_type)
        {
            if (holder.a_w.w_sym == gAsteriskSymbol)
            {
                forwardBackDontCare = true;
            }
            else if (holder.a_w.w_sym == gOpenSquareSymbol)
            {
                forwardClosed = true;
            }
            else if (holder.a_w.w_sym != gOpenRoundSymbol)
            {
                map3dReportUnexpected(xx, holder);
                result = false;
            }
        }
        else
        {
            map3dReportUnexpected(xx, holder);
            result = false;
        }
    }
    // Get forward value
    if (result && (! forwardBackDontCare))
    {
        result = map3dGetNextAtomInBuffer(xx, holder);
        if (result)
        {
            switch (holder.a_type)
            {
                case A_SYM:
                    if ((holder.a_w.w_sym == gNegInfSymbol1) ||
                        (holder.a_w.w_sym == gNegInfSymbol2))
                    {
                        forwardMatch = MatchInfinity;
                    }
                    else
                    {
                        map3dReportUnexpected(xx, holder);
                        result = false;
                    }
                    break;

                case A_FLOAT:
                    forwardMatch = MatchFloat;
                    forwardFloatValue = holder.a_w.w_float;
                    break;

                case A_LONG:
                    forwardMatch = MatchInteger;
                    forwardIntValue = holder.a_w.w_long;
                    break;

                default:
                    map3dReportUnexpected(xx, holder);
                    result = false;
                    break;
                    
            }
        }
    }
    // Get back value
    if (result && (! forwardBackDontCare))
    {
        result = map3dGetNextAtomInBuffer(xx, holder);
        if (result)
        {
            switch (holder.a_type)
            {
                case A_SYM:
                    if ((holder.a_w.w_sym == gPosInfSymbol1) ||
                        (holder.a_w.w_sym == gPosInfSymbol2) ||
                        (holder.a_w.w_sym == gPosInfSymbol3) ||
                        (holder.a_w.w_sym == gPosInfSymbol4))
                    {
                        backMatch = MatchInfinity;
                    }
                    else
                    {
                        map3dReportUnexpected(xx, holder);
                        result = false;
                    }
                    break;

                case A_FLOAT:
                    backMatch = MatchFloat;
                    backFloatValue = holder.a_w.w_float;
                    break;

                case A_LONG:
                    backMatch = MatchInteger;
                    backIntValue = holder.a_w.w_long;
                    break;

                default:
                    map3dReportUnexpected(xx, holder);
                    result = false;
                    break;
                    
            }
        }
    }
    // Get back bracket
    if (result && (! forwardBackDontCare))
    {
        result = map3dGetNextAtomInBuffer(xx, holder);
        if (A_SYM == holder.a_type)
        {
            if (holder.a_w.w_sym == gCloseSquareSymbol)
            {
                backClosed = true;
            }
            else if (holder.a_w.w_sym != gCloseRoundSymbol)
            {
                map3dReportUnexpected(xx, holder);
                result = false;
            }
        }
        else
        {
            map3dReportUnexpected(xx, holder);
            result = false;
        }
    }
    // Collect the return values
    if (result)
    {
        result = map3dGetNextAtomInBuffer(xx, holder);
        collector = static_cast<t_binbuf *>(binbuf_new());

        /* Collect the output values, terminated by a semicolon: */
        if (result && collector)
        {
            for ( ; holder.a_type != A_SEMI; )
            {
                /* We have a value; attach it. */
                binbuf_append(collector, NULL, 1, &holder);
                ++outputCount;
                if (A_SYM == holder.a_type)
                {
                    if (holder.a_w.w_sym == gDollarSymbol)
                    {
                        ++dollarsPresent;
                    }
                    else if (holder.a_w.w_sym == gDoubleDollarSymbol)
                    {
                        ++doubleDollarsPresent;
                    }
                    else if ((holder.a_w.w_sym == gDollarXSymbol) ||
                             (holder.a_w.w_sym == gDollarYSymbol) ||
                             (holder.a_w.w_sym == gDollarZSymbol) ||
                             (holder.a_w.w_sym == gDoubleDollarXSymbol) ||
                             (holder.a_w.w_sym == gDoubleDollarYSymbol) ||
                             (holder.a_w.w_sym == gDoubleDollarZSymbol))
                    {
                        ++singleDollarsPresent;
                    }
                }
                else if ((A_DOLLAR == holder.a_type) || (A_DOLLSYM == holder.a_type))
                {
                    ++dollarsPresent;
                }
                result = map3dGetNextAtomInBuffer(xx, holder);
                if (! result)
                {
                    break;
                }
                
            }
        }
    }
    // Assemble the information and attach to the list
    if (result)
    {
        RangeData * newData = GET_BYTES(1, RangeData);

        if (newData)
        {
            newData->fNext = NULL;
            newData->fOutputCount = outputCount;
            newData->fDollarsPresent = dollarsPresent;
            newData->fDoubleDollarsPresent = doubleDollarsPresent;
            newData->fSingleDollarsPresent = singleDollarsPresent;
            newData->fBottomTopDontCare = bottomTopDontCare;
            newData->fLeftRightDontCare = leftRightDontCare;
            newData->fForwardBackDontCare = forwardBackDontCare;
            if (! bottomTopDontCare)
            {
                if (MatchFloat == bottomMatch)
                {
                    setFOI2Float(newData->fBottom.fValue, bottomFloatValue);
                }
                else if (MatchInteger == bottomMatch)
                {
                    setFOI2Integer(newData->fBottom.fValue, bottomIntValue);
                }
                if (MatchFloat == topMatch)
                {
                    setFOI2Float(newData->fTop.fValue, topFloatValue);
                }
                else if (MatchInteger == topMatch)
                {
                    setFOI2Integer(newData->fTop.fValue, topIntValue);
                }
                newData->fBottom.fKind = bottomMatch;
                newData->fTop.fKind = topMatch;
                newData->fBottom.fIsClosed = bottomClosed;
                newData->fTop.fIsClosed = topClosed;
            }
            if (! leftRightDontCare)
            {
                if (MatchFloat == leftMatch)
                {
                    setFOI2Float(newData->fLeft.fValue, leftFloatValue);
                }
                else if (MatchInteger == leftMatch)
                {
                    setFOI2Integer(newData->fLeft.fValue, leftIntValue);
                }
                if (MatchFloat == rightMatch)
                {
                    setFOI2Float(newData->fRight.fValue, rightFloatValue);
                }
                else if (MatchInteger == rightMatch)
                {
                    setFOI2Integer(newData->fRight.fValue, rightIntValue);
                }
                newData->fLeft.fKind = leftMatch;
                newData->fRight.fKind = rightMatch;
                newData->fLeft.fIsClosed = leftClosed;
                newData->fRight.fIsClosed = rightClosed;
            }
            if (! forwardBackDontCare)
            {
                if (MatchFloat == backMatch)
                {
                    setFOI2Float(newData->fBack.fValue, backFloatValue);
                }
                else if (MatchInteger == backMatch)
                {
                    setFOI2Integer(newData->fBack.fValue, backIntValue);
                }
                if (MatchFloat == forwardMatch)
                {
                    setFOI2Float(newData->fForward.fValue, forwardFloatValue);
                }
                else if (MatchInteger == forwardMatch)
                {
                    setFOI2Integer(newData->fForward.fValue, forwardIntValue);
                }
                newData->fBack.fKind = backMatch;
                newData->fForward.fKind = forwardMatch;
                newData->fBack.fIsClosed = backClosed;
                newData->fForward.fIsClosed = forwardClosed;
            }
            if (outputCount)
            {
                long     tyOffset = 0;
                long     stOffset = 0;
                t_atom * vector = GET_BYTES(newData->fOutputCount, t_atom);

                newData->fOutput = vector;
                for (long ii = 0; ii < newData->fOutputCount; ++ii, ++vector)
                {
                    if (binbuf_getatom(collector, &tyOffset, &stOffset, vector))
                    {
                        break;
                    }
                    
                }
            }
            else
            {
                newData->fOutput = NULL;
            }
            if (xx->fLastRange)
            {
                xx->fLastRange->fNext = newData;
            }
            xx->fLastRange = newData;
            if (! xx->fFirstRange)
            {
                xx->fFirstRange = newData;
            }
        }
    }
    if (collector)
    {
        freeobject(reinterpret_cast<t_object *>(collector));
    }
    return result;
} // map3dCollectARange

/*------------------------------------ map3dLoadRangeList ---*/
bool
map3dLoadRangeList(Map3dData * xx,
                   t_symbol *  fileName)
{
    bool         result = false;
    t_filehandle fileRef;

    if (path_opensysfile(fileName->s_name, path_getdefault(), &fileRef, PATH_READ_PERM))
    {
        LOG_ERROR_2(xx, OUTPUT_PREFIX "problem opening file '%s'", fileName->s_name)
    }
    else
    {
        t_handle fileContents = sysmem_newhandle(0);

        if (sysfile_readtextfile(fileRef, fileContents, 0, TEXT_LB_NATIVE))
        {
            LOG_ERROR_2(xx, OUTPUT_PREFIX "problem reading file '%s'", fileName->s_name)
        }
        else if (binbuf_text(xx->fBuffer, fileContents, sysmem_handlesize(fileContents)))
        {
            post("problem processing");
            LOG_ERROR_2(xx, OUTPUT_PREFIX "problem converting file '%s'", fileName->s_name)
        }
        else
        {
            map3dClearRangeList(xx);
            xx->fBufferTypeOffset = 0;
            xx->fBufferStuffOffset = 0;
            for ( ; map3dCollectARange(xx); )
            {
                ++xx->fRangeCount;
                if (xx->fVerbose)
                {
                    LOG_POST_1(xx, OUTPUT_PREFIX "read range specification")
                }
            }
            result = true;
        }
        sysfile_close(fileRef);
        sysmem_freehandle(fileContents);
    }
    return result;
} // map3dLoadRangeList

/*------------------------------------ map3dGetNextAtomInList ---*/
static bool
map3dGetNextAtomInList(long &     offset,
                       const long numAtoms,
                       t_atom *   inList,
                       t_atom &   result)
{
    bool okSoFar = false;

    if (numAtoms > offset)
    {
        okSoFar = true;
        result = *(inList + offset);
    }
    else if (numAtoms == offset)
    {
        okSoFar = true;
        result.a_type = A_SEMI;
    }
    ++offset;
    return okSoFar;
} // map3dGetNextAtomInList

/*------------------------------------ map3dConvertListToRange ---*/
RangeData *
map3dConvertListToRange(Map3dData * xx,
                        const long  offset,
                        const long  numAtoms,
                        t_atom *    inList)
{
    long        nextAtom = offset;
    t_atom      holder;
    bool        result = map3dGetNextAtomInList(nextAtom, numAtoms, inList, holder);
    bool        backClosed = false;
    bool        bottomClosed = false;
    bool        forwardClosed = false;
    bool        leftClosed = false;
    bool        rightClosed = false;
    bool        topClosed = false;
    bool        bottomTopDontCare = false;
    bool        leftRightDontCare = false;
    bool        forwardBackDontCare = false;
    float       backFloatValue;
    float       bottomFloatValue;
    float       forwardFloatValue;
    float       leftFloatValue;
    float       rightFloatValue;
    float       topFloatValue;
    long        backIntValue;
    long        bottomIntValue;
    long        forwardIntValue;
    long        leftIntValue;
    long        rightIntValue;
    long        topIntValue;
    Category    backMatch = MatchUnknown;
    Category    bottomMatch = MatchUnknown;
    Category    forwardMatch = MatchUnknown;
    Category    leftMatch = MatchUnknown;
    Category    rightMatch = MatchUnknown;
    Category    topMatch = MatchUnknown;
    long        outputCount = 0;
    t_binbuf *  collector = NULL;
    long        dollarsPresent = 0;
    long        doubleDollarsPresent = 0;
    long        singleDollarsPresent = 0;
    RangeData * newData = NULL;

    // Get the left/right pair
    if (result)
    {
        if (A_SYM == holder.a_type)
        {
            if (holder.a_w.w_sym == gAsteriskSymbol)
            {
                leftRightDontCare = true;
            }
            else if (holder.a_w.w_sym == gOpenSquareSymbol)
            {
                leftClosed = true;
            }
            else if (holder.a_w.w_sym != gOpenRoundSymbol)
            {
                map3dReportUnexpected(xx, holder);
                result = false;
            }
        }
        else
        {
            map3dReportUnexpected(xx, holder);
            result = false;
        }
    }
    // Get left value
    if (result && (! leftRightDontCare))
    {
        result = map3dGetNextAtomInList(nextAtom, numAtoms, inList, holder);
        if (result)
        {
            switch (holder.a_type)
            {
                case A_SYM:
                    if ((holder.a_w.w_sym == gNegInfSymbol1) ||
                        (holder.a_w.w_sym == gNegInfSymbol2))
                    {
                        leftMatch = MatchInfinity;
                    }
                    else
                    {
                        map3dReportUnexpected(xx, holder);
                        result = false;
                    }
                    break;

                case A_FLOAT:
                    leftMatch = MatchFloat;
                    leftFloatValue = holder.a_w.w_float;
                    break;

                case A_LONG:
                    leftMatch = MatchInteger;
                    leftIntValue = holder.a_w.w_long;
                    break;

                default:
                    map3dReportUnexpected(xx, holder);
                    result = false;
                    break;
                    
            }
        }
    }
    // Get right value
    if (result && (! leftRightDontCare))
    {
        result = map3dGetNextAtomInList(nextAtom, numAtoms, inList, holder);
        if (result)
        {
            switch (holder.a_type)
            {
                case A_SYM:
                    if ((holder.a_w.w_sym == gPosInfSymbol1) ||
                        (holder.a_w.w_sym == gPosInfSymbol2) ||
                        (holder.a_w.w_sym == gPosInfSymbol3) ||
                        (holder.a_w.w_sym == gPosInfSymbol4))
                    {
                        rightMatch = MatchInfinity;
                    }
                    else
                    {
                        map3dReportUnexpected(xx, holder);
                        result = false;
                    }
                    break;

                case A_FLOAT:
                    rightMatch = MatchFloat;
                    rightFloatValue = holder.a_w.w_float;
                    break;

                case A_LONG:
                    rightMatch = MatchInteger;
                    rightIntValue = holder.a_w.w_long;
                    break;

                default:
                    map3dReportUnexpected(xx, holder);
                    result = false;
                    break;
                    
            }
        }
    }
    // Get right bracket
    if (result && (! leftRightDontCare))
    {
        result = map3dGetNextAtomInList(nextAtom, numAtoms, inList, holder);
        if (A_SYM == holder.a_type)
        {
            if (holder.a_w.w_sym == gCloseSquareSymbol)
            {
                rightClosed = true;
            }
            else if (holder.a_w.w_sym != gCloseRoundSymbol)
            {
                map3dReportUnexpected(xx, holder);
                result = false;
            }
        }
        else
        {
            map3dReportUnexpected(xx, holder);
            result = false;
        }
    }
    // Get the bottom/top pair
    if (result)
    {
        result = map3dGetNextAtomInList(nextAtom, numAtoms, inList, holder);
        if (A_SYM == holder.a_type)
        {
            if (holder.a_w.w_sym == gAsteriskSymbol)
            {
                bottomTopDontCare = true;
            }
            else if (holder.a_w.w_sym == gOpenSquareSymbol)
            {
                bottomClosed = true;
            }
            else if (holder.a_w.w_sym != gOpenRoundSymbol)
            {
                map3dReportUnexpected(xx, holder);
                result = false;
            }
        }
        else
        {
            map3dReportUnexpected(xx, holder);
            result = false;
        }
    }
    // Get bottom value
    if (result && (! bottomTopDontCare))
    {
        result = map3dGetNextAtomInList(nextAtom, numAtoms, inList, holder);
        if (result)
        {
            switch (holder.a_type)
            {
                case A_SYM:
                    if ((holder.a_w.w_sym == gNegInfSymbol1) ||
                        (holder.a_w.w_sym == gNegInfSymbol2))
                    {
                        bottomMatch = MatchInfinity;
                    }
                    else
                    {
                        map3dReportUnexpected(xx, holder);
                        result = false;
                    }
                    break;

                case A_FLOAT:
                    bottomMatch = MatchFloat;
                    bottomFloatValue = holder.a_w.w_float;
                    break;

                case A_LONG:
                    bottomMatch = MatchInteger;
                    bottomIntValue = holder.a_w.w_long;
                    break;

                default:
                    map3dReportUnexpected(xx, holder);
                    result = false;
                    break;
                    
            }
        }
    }
    // Get top value
    if (result && (! bottomTopDontCare))
    {
        result = map3dGetNextAtomInList(nextAtom, numAtoms, inList, holder);
        if (result)
        {
            switch (holder.a_type)
            {
                case A_SYM:
                    if ((holder.a_w.w_sym == gPosInfSymbol1) ||
                        (holder.a_w.w_sym == gPosInfSymbol2) ||
                        (holder.a_w.w_sym == gPosInfSymbol3) ||
                        (holder.a_w.w_sym == gPosInfSymbol4))
                    {
                        topMatch = MatchInfinity;
                    }
                    else
                    {
                        map3dReportUnexpected(xx, holder);
                        result = false;
                    }
                    break;

                case A_FLOAT:
                    topMatch = MatchFloat;
                    topFloatValue = holder.a_w.w_float;
                    break;

                case A_LONG:
                    topMatch = MatchInteger;
                    topIntValue = holder.a_w.w_long;
                    break;

                default:
                    map3dReportUnexpected(xx, holder);
                    result = false;
                    break;
                    
            }
        }
    }
    // Get top bracket
    if (result && (! bottomTopDontCare))
    {
        result = map3dGetNextAtomInList(nextAtom, numAtoms, inList, holder);
        if (A_SYM == holder.a_type)
        {
            if (holder.a_w.w_sym == gCloseSquareSymbol)
            {
                topClosed = true;
            }
            else if (holder.a_w.w_sym != gCloseRoundSymbol)
            {
                map3dReportUnexpected(xx, holder);
                result = false;
            }
        }
        else
        {
            map3dReportUnexpected(xx, holder);
            result = false;
        }
    }
    // Get the forward/back pair
    if (result)
    {
        result = map3dGetNextAtomInList(nextAtom, numAtoms, inList, holder);
        if (A_SYM == holder.a_type)
        {
            if (holder.a_w.w_sym == gAsteriskSymbol)
            {
                forwardBackDontCare = true;
            }
            else if (holder.a_w.w_sym == gOpenSquareSymbol)
            {
                forwardClosed = true;
            }
            else if (holder.a_w.w_sym != gOpenRoundSymbol)
            {
                map3dReportUnexpected(xx, holder);
                result = false;
            }
        }
        else
        {
            map3dReportUnexpected(xx, holder);
            result = false;
        }
    }
    // Get forward value
    if (result && (! forwardBackDontCare))
    {
        result = map3dGetNextAtomInList(nextAtom, numAtoms, inList, holder);
        if (result)
        {
            switch (holder.a_type)
            {
                case A_SYM:
                    if ((holder.a_w.w_sym == gNegInfSymbol1) ||
                        (holder.a_w.w_sym == gNegInfSymbol2))
                    {
                        forwardMatch = MatchInfinity;
                    }
                    else
                    {
                        map3dReportUnexpected(xx, holder);
                        result = false;
                    }
                    break;

                case A_FLOAT:
                    forwardMatch = MatchFloat;
                    forwardFloatValue = holder.a_w.w_float;
                    break;

                case A_LONG:
                    forwardMatch = MatchInteger;
                    forwardIntValue = holder.a_w.w_long;
                    break;

                default:
                    map3dReportUnexpected(xx, holder);
                    result = false;
                    break;
                    
            }
        }
    }
    // Get back value
    if (result && (! forwardBackDontCare))
    {
        result = map3dGetNextAtomInList(nextAtom, numAtoms, inList, holder);
        if (result)
        {
            switch (holder.a_type)
            {
                case A_SYM:
                    if ((holder.a_w.w_sym == gPosInfSymbol1) ||
                        (holder.a_w.w_sym == gPosInfSymbol2) ||
                        (holder.a_w.w_sym == gPosInfSymbol3) ||
                        (holder.a_w.w_sym == gPosInfSymbol4))
                    {
                        backMatch = MatchInfinity;
                    }
                    else
                    {
                        map3dReportUnexpected(xx, holder);
                        result = false;
                    }
                    break;

                case A_FLOAT:
                    backMatch = MatchFloat;
                    backFloatValue = holder.a_w.w_float;
                    break;

                case A_LONG:
                    backMatch = MatchInteger;
                    backIntValue = holder.a_w.w_long;
                    break;

                default:
                    map3dReportUnexpected(xx, holder);
                    result = false;
                    break;
                    
            }
        }
    }
    // Get back bracket
    if (result && (! forwardBackDontCare))
    {
        result = map3dGetNextAtomInList(nextAtom, numAtoms, inList, holder);
        if (A_SYM == holder.a_type)
        {
            if (holder.a_w.w_sym == gCloseSquareSymbol)
            {
                backClosed = true;
            }
            else if (holder.a_w.w_sym != gCloseRoundSymbol)
            {
                map3dReportUnexpected(xx, holder);
                result = false;
            }
        }
        else
        {
            map3dReportUnexpected(xx, holder);
            result = false;
        }
    }
    // Collect the return values
    if (result)
    {
        result = map3dGetNextAtomInList(nextAtom, numAtoms, inList, holder);
        collector = static_cast<t_binbuf *>(binbuf_new());

        /* Collect the output values, terminated by a semicolon: */
        if (result && collector)
        {
            for ( ; holder.a_type != A_SEMI; )
            {
                /* We have a value; attach it. */
                binbuf_append(collector, NULL, 1, &holder);
                ++outputCount;
                if (A_SYM == holder.a_type)
                {
                    if (holder.a_w.w_sym == gDollarSymbol)
                    {
                        ++dollarsPresent;
                    }
                    else if (holder.a_w.w_sym == gDoubleDollarSymbol)
                    {
                        ++doubleDollarsPresent;
                    }
                    else if ((holder.a_w.w_sym == gDollarXSymbol) ||
                             (holder.a_w.w_sym == gDollarYSymbol) ||
                             (holder.a_w.w_sym == gDollarZSymbol) ||
                             (holder.a_w.w_sym == gDoubleDollarXSymbol) ||
                             (holder.a_w.w_sym == gDoubleDollarYSymbol) ||
                             (holder.a_w.w_sym == gDoubleDollarZSymbol))
                    {
                        ++singleDollarsPresent;
                    }
                }
                else if ((A_DOLLAR == holder.a_type) || (A_DOLLSYM == holder.a_type))
                {
                    ++dollarsPresent;
                }
                result = map3dGetNextAtomInList(nextAtom, numAtoms, inList, holder);
                if (! result)
                {
                    break;
                }
                
            }
        }
    }
    // Assemble the information and attach to the list
    if (result)
    {
        newData = GET_BYTES(1, RangeData);
        if (newData)
        {
            newData->fNext = NULL;
            newData->fOutputCount = outputCount;
            newData->fDollarsPresent = dollarsPresent;
            newData->fDoubleDollarsPresent = doubleDollarsPresent;
            newData->fSingleDollarsPresent = singleDollarsPresent;
            newData->fBottomTopDontCare = bottomTopDontCare;
            newData->fLeftRightDontCare = leftRightDontCare;
            newData->fForwardBackDontCare = forwardBackDontCare;
            if (! bottomTopDontCare)
            {
                if (MatchFloat == bottomMatch)
                {
                    setFOI2Float(newData->fBottom.fValue, bottomFloatValue);
                }
                else if (MatchInteger == bottomMatch)
                {
                    setFOI2Integer(newData->fBottom.fValue, bottomIntValue);
                }
                if (MatchFloat == topMatch)
                {
                    setFOI2Float(newData->fTop.fValue, topFloatValue);
                }
                else if (MatchInteger == topMatch)
                {
                    setFOI2Integer(newData->fTop.fValue, topIntValue);
                }
                newData->fBottom.fKind = bottomMatch;
                newData->fTop.fKind = topMatch;
                newData->fBottom.fIsClosed = bottomClosed;
                newData->fTop.fIsClosed = topClosed;
            }
            if (! leftRightDontCare)
            {
                if (MatchFloat == leftMatch)
                {
                    setFOI2Float(newData->fLeft.fValue, leftFloatValue);
                }
                else if (MatchInteger == leftMatch)
                {
                    setFOI2Integer(newData->fLeft.fValue, leftIntValue);
                }
                if (MatchFloat == rightMatch)
                {
                    setFOI2Float(newData->fRight.fValue, rightFloatValue);
                }
                else if (MatchInteger == rightMatch)
                {
                    setFOI2Integer(newData->fRight.fValue, rightIntValue);
                }
                newData->fLeft.fKind = leftMatch;
                newData->fRight.fKind = rightMatch;
                newData->fLeft.fIsClosed = leftClosed;
                newData->fRight.fIsClosed = rightClosed;
            }
            if (! forwardBackDontCare)
            {
                if (MatchFloat == backMatch)
                {
                    setFOI2Float(newData->fBack.fValue, backFloatValue);
                }
                else if (MatchInteger == backMatch)
                {
                    setFOI2Integer(newData->fBack.fValue, backIntValue);
                }
                if (MatchFloat == forwardMatch)
                {
                    setFOI2Float(newData->fForward.fValue, forwardFloatValue);
                }
                else if (MatchInteger == forwardMatch)
                {
                    setFOI2Integer(newData->fForward.fValue, forwardIntValue);
                }
                newData->fBack.fKind = backMatch;
                newData->fForward.fKind = forwardMatch;
                newData->fBack.fIsClosed = backClosed;
                newData->fForward.fIsClosed = forwardClosed;
            }
            if (outputCount)
            {
                long     tyOffset = 0;
                long     stOffset = 0;
                t_atom * vector = GET_BYTES(newData->fOutputCount, t_atom);

                newData->fOutput = vector;
                for (long ii = 0; ii < newData->fOutputCount; ++ii, ++vector)
                {
                    if (binbuf_getatom(collector, &tyOffset, &stOffset, vector))
                    {
                        break;
                    }
                    
                }
            }
            else
            {
                newData->fOutput = NULL;
            }
        }
    }
    if (collector)
    {
        freeobject(reinterpret_cast<t_object *>(collector));
    }
    return newData;
} // map3dConvertListToRange
