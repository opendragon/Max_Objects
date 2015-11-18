/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       map1dLoadFile.cpp                                                       */
/*                                                                                      */
/*  Contains:   The low-level support routines for the map1d module.                    */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2000 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2000/11/14                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "map1d.h"

/*------------------------------------ map1dClearRangeList ---*/
void map1dClearRangeList(Map1dData * xx)
{
    if (xx)
    {
        while (xx->fFirstRange)
        {
            RangeData * walker = xx->fFirstRange;

            xx->fFirstRange = walker->fNext;
            FREEBYTES(walker->fOutput, walker->fOutputCount)
            FREEBYTES(walker, 1)
        }
        xx->fPreviousResult = xx->fLastRange = NULL_PTR;
        xx->fRangeCount = 0;
    }
} // map1dClearRangeList
/*------------------------------------ map1dGetNextAtomInBuffer ---*/
static bool map1dGetNextAtomInBuffer(Map1dData * xx,
                                     t_atom &    value)
{
    t_atom skipper;
    bool   result = true;

    for ( ; result; )
    {
        result = (! binbuf_getatom(xx->fBuffer, &xx->fBufferTypeOffset, &xx->fBufferStuffOffset, &value));
        if (result && (A_SYM == value.a_type))
        {
            if (kCommentCharacter == value.a_w.w_sym->s_name[0])
            {
                /* skip a comment */
                for ( ; result; )
                {
                    result = (! binbuf_getatom(xx->fBuffer, &xx->fBufferTypeOffset, &xx->fBufferStuffOffset, &skipper));
                    if (A_SEMI == skipper.a_type)
                    {
                        break;
                    }
                }
                /* At this point, we've read the trailing semicolon, so we need to get the next atom. */
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
} // map1dGetNextAtomInBuffer
/*------------------------------------ map1dReportUnexpected ---*/
static void map1dReportUnexpected(Map1dData * xx,
                                  t_atom &    what)
{
    switch (what.a_type)
    {
        case A_FLOAT:
            LOG_ERROR_2(xx, OUTPUT_PREFIX "unexpected floating point (%g)", static_cast<double>(what.a_w.w_float))
            break;

        case A_LONG:
            LOG_ERROR_2(xx, OUTPUT_PREFIX "unexpected long (%ld)", what.a_w.w_long)
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
            LOG_ERROR_1(xx, OUTPUT_PREFIX "unexpected dollar")
            break;

        default:
            LOG_ERROR_2(xx, OUTPUT_PREFIX "unexpected atom, type=%d", static_cast<int>(what.a_type))
            break;
    }
} // map1dReportUnexpected
/*------------------------------------ map1dCollectARange ---*/
static bool map1dCollectARange(Map1dData * xx)
{
    t_atom     holder;
    bool       result = map1dGetNextAtomInBuffer(xx, holder);
    bool       lowerClosed = false;
    bool       upperClosed = false;
    bool       lowerUpperDontCare = false;
    float      lowerFloatValue;
    float      upperFloatValue;
    long       lowerIntValue;
    long       upperIntValue;
    Category   lowerMatch = MatchUnknown;
    Category   upperMatch = MatchUnknown;
    short      outputCount = 0;
    t_binbuf * collector = NULL_PTR;
    short      dollarsPresent = 0;
    short      doubleDollarsPresent = 0;

    if (result)
    {
        if (A_SYM == holder.a_type)
        {
            if (holder.a_w.w_sym == gAsteriskSymbol)
            {
                lowerUpperDontCare = true;
            }
            else if (holder.a_w.w_sym == gOpenSquareSymbol)
            {
                lowerClosed = true;
            }
            else if (holder.a_w.w_sym != gOpenRoundSymbol)
            {
                map1dReportUnexpected(xx, holder);
                result = false;
            }
        }
        else
        {
            map1dReportUnexpected(xx, holder);
            result = false;
        }
    }
    // Get lower value
    if (result && (! lowerUpperDontCare))
    {
        result = map1dGetNextAtomInBuffer(xx, holder);
        if (result)
        {
            switch (holder.a_type)
            {
                case A_SYM:
                    if ((holder.a_w.w_sym == gNegInfSymbol1) || (holder.a_w.w_sym == gNegInfSymbol2))
                    {
                        lowerMatch = MatchInfinity;
                    }
                    else
                    {
                        map1dReportUnexpected(xx, holder);
                        result = false;
                    }
                    break;

                case A_FLOAT:
                    lowerMatch = MatchFloat;
                    lowerFloatValue = holder.a_w.w_float;
                    break;

                case A_LONG:
                    lowerMatch = MatchInteger;
                    lowerIntValue = holder.a_w.w_long;
                    break;

                default:
                    map1dReportUnexpected(xx, holder);
                    result = false;
                    break;
            }
        }
    }
    // Get upper value
    if (result && (! lowerUpperDontCare))
    {
        result = map1dGetNextAtomInBuffer(xx, holder);
        if (result)
        {
            switch (holder.a_type)
            {
                case A_SYM:
                    if ((holder.a_w.w_sym == gPosInfSymbol1) || (holder.a_w.w_sym == gPosInfSymbol2) ||
                        (holder.a_w.w_sym == gPosInfSymbol3) || (holder.a_w.w_sym == gPosInfSymbol4))
                    {
                        upperMatch = MatchInfinity;
                    }
                    else
                    {
                        map1dReportUnexpected(xx, holder);
                        result = false;
                    }
                    break;

                case A_FLOAT:
                    upperMatch = MatchFloat;
                    upperFloatValue = holder.a_w.w_float;
                    break;

                case A_LONG:
                    upperMatch = MatchInteger;
                    upperIntValue = holder.a_w.w_long;
                    break;

                default:
                    map1dReportUnexpected(xx, holder);
                    result = false;
                    break;
            }
        }
    }
    // Get upper bracket
    if (result && (! lowerUpperDontCare))
    {
        result = map1dGetNextAtomInBuffer(xx, holder);
        if (A_SYM == holder.a_type)
        {
            if (holder.a_w.w_sym == gCloseSquareSymbol)
            {
                upperClosed = true;
            }
            else if (holder.a_w.w_sym != gCloseRoundSymbol)
            {
                map1dReportUnexpected(xx, holder);
                result = false;
            }
        }
        else
        {
            map1dReportUnexpected(xx, holder);
            result = false;
        }
    }
    // Collect the return values
    if (result)
    {
        result = map1dGetNextAtomInBuffer(xx, holder);
        collector = static_cast<t_binbuf *>(binbuf_new());

        /* Collect the output values, terminated by a semicolon: */
        if (result && collector)
        {
            for ( ; holder.a_type != A_SEMI; )
            {
                /* We have a value; attach it. */
                binbuf_append(collector, NULL_PTR, 1, &holder);
                ++outputCount;
                if (A_SYM == holder.a_type)
                {
                    if ((holder.a_w.w_sym == gDollarSymbol) || (holder.a_w.w_sym == gDollarXSymbol))
                    {
                        ++dollarsPresent;
                    }
                    else if ((holder.a_w.w_sym == gDoubleDollarSymbol) || (holder.a_w.w_sym == gDoubleDollarXSymbol))
                    {
                        ++doubleDollarsPresent;
                    }
                }
                else if (A_DOLLAR == holder.a_type)
                {
                    ++dollarsPresent;
                }
                result = map1dGetNextAtomInBuffer(xx, holder);
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
        RangeData * newData = GETBYTES(1, RangeData);

        if (newData)
        {
            newData->fNext = NULL_PTR;
            newData->fOutputCount = outputCount;
            newData->fDollarsPresent = dollarsPresent;
            newData->fDoubleDollarsPresent = doubleDollarsPresent;
            newData->fLowerUpperDontCare = lowerUpperDontCare;
            if (! lowerUpperDontCare)
            {
                if (MatchFloat == lowerMatch)
                {
                    setFOI2Float(newData->fLower.fValue, lowerFloatValue);
                }
                else if (MatchInteger == lowerMatch)
                {
                    setFOI2Integer(newData->fLower.fValue, lowerIntValue);
                }
                if (MatchFloat == upperMatch)
                {
                    setFOI2Float(newData->fUpper.fValue, upperFloatValue);
                }
                else if (MatchInteger == upperMatch)
                {
                    setFOI2Integer(newData->fUpper.fValue, upperIntValue);
                }
                newData->fLower.fKind = lowerMatch;
                newData->fUpper.fKind = upperMatch;
                newData->fLower.fIsClosed = lowerClosed;
                newData->fUpper.fIsClosed = upperClosed;
            }
            if (outputCount)
            {
                long     tyOffset = 0;
                long     stOffset = 0;
                t_atom * vector = GETBYTES(newData->fOutputCount, t_atom);

                newData->fOutput = vector;
                for (short ii = 0; ii < newData->fOutputCount; ++ii, ++vector)
                {
                    if (binbuf_getatom(collector, &tyOffset, &stOffset, vector))
                    {
                        break;
                    }
                }
            }
            else
            {
                newData->fOutput = NULL_PTR;
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
} // map1dCollectARange
/*------------------------------------ map1dLoadRangeList ---*/
bool map1dLoadRangeList(Map1dData * xx,
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
            map1dClearRangeList(xx);
            xx->fBufferTypeOffset = 0;
            xx->fBufferStuffOffset = 0;
            for ( ; map1dCollectARange(xx); )
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
} // map1dLoadRangeList
/*------------------------------------ map1dGetNextAtomInList ---*/
static bool map1dGetNextAtomInList(short &     offset,
                                   const short numAtoms,
                                   t_atom *    inList,
                                   t_atom &    result)
{
    bool okSoFar = false;

    if (offset < numAtoms)
    {
        okSoFar = true;
        result = *(inList + offset);
    }
    else if (offset == numAtoms)
    {
        okSoFar = true;
        result.a_type = A_SEMI;
    }
    ++offset;
    return okSoFar;
} // map1dGetNextAtomInList
/*------------------------------------ map1dConvertListToRange ---*/
RangeData * map1dConvertListToRange(Map1dData * xx,
                                    const short offset,
                                    const short numAtoms,
                                    t_atom *    inList)
{
    short       nextAtom = offset;
    t_atom      holder;
    bool        result = map1dGetNextAtomInList(nextAtom, numAtoms, inList, holder);
    bool        lowerClosed = false;
    bool        upperClosed = false;
    bool        lowerUpperDontCare = false;
    float       lowerFloatValue;
    float       upperFloatValue;
    long        lowerIntValue;
    long        upperIntValue;
    Category    lowerMatch = MatchUnknown;
    Category    upperMatch = MatchUnknown;
    short       outputCount = 0;
    t_binbuf *  collector = NULL_PTR;
    short       dollarsPresent = 0;
    short       doubleDollarsPresent = 0;
    RangeData * newData = NULL_PTR;

    if (result)
    {
        if (A_SYM == holder.a_type)
        {
            if (holder.a_w.w_sym == gAsteriskSymbol)
            {
                lowerUpperDontCare = true;
            }
            else if (holder.a_w.w_sym == gOpenSquareSymbol)
            {
                lowerClosed = true;
            }
            else if (holder.a_w.w_sym != gOpenRoundSymbol)
            {
                map1dReportUnexpected(xx, holder);
                result = false;
            }
        }
        else
        {
            map1dReportUnexpected(xx, holder);
            result = false;
        }
    }
    // Get lower value
    if (result && (! lowerUpperDontCare))
    {
        result = map1dGetNextAtomInList(nextAtom, numAtoms, inList, holder);
        if (result)
        {
            switch (holder.a_type)
            {
                case A_SYM:
                    if ((holder.a_w.w_sym == gNegInfSymbol1) || (holder.a_w.w_sym == gNegInfSymbol2))
                    {
                        lowerMatch = MatchInfinity;
                    }
                    else
                    {
                        map1dReportUnexpected(xx, holder);
                        result = false;
                    }
                    break;

                case A_FLOAT:
                    lowerMatch = MatchFloat;
                    lowerFloatValue = holder.a_w.w_float;
                    break;

                case A_LONG:
                    lowerMatch = MatchInteger;
                    lowerIntValue = holder.a_w.w_long;
                    break;

                default:
                    map1dReportUnexpected(xx, holder);
                    result = false;
                    break;
            }
        }
    }
    // Get upper value
    if (result && (! lowerUpperDontCare))
    {
        result = map1dGetNextAtomInList(nextAtom, numAtoms, inList, holder);
        if (result)
        {
            switch (holder.a_type)
            {
                case A_SYM:
                    if ((holder.a_w.w_sym == gPosInfSymbol1) || (holder.a_w.w_sym == gPosInfSymbol2) ||
                        (holder.a_w.w_sym == gPosInfSymbol3) || (holder.a_w.w_sym == gPosInfSymbol4))
                    {
                        upperMatch = MatchInfinity;
                    }
                    else
                    {
                        map1dReportUnexpected(xx, holder);
                        result = false;
                    }
                    break;

                case A_FLOAT:
                    upperMatch = MatchFloat;
                    upperFloatValue = holder.a_w.w_float;
                    break;

                case A_LONG:
                    upperMatch = MatchInteger;
                    upperIntValue = holder.a_w.w_long;
                    break;

                default:
                    map1dReportUnexpected(xx, holder);
                    result = false;
                    break;
            }
        }
    }
    // Get upper bracket
    if (result && (! lowerUpperDontCare))
    {
        result = map1dGetNextAtomInList(nextAtom, numAtoms, inList, holder);
        if (A_SYM == holder.a_type)
        {
            if (holder.a_w.w_sym == gCloseSquareSymbol)
            {
                upperClosed = true;
            }
            else if (holder.a_w.w_sym != gCloseRoundSymbol)
            {
                map1dReportUnexpected(xx, holder);
                result = false;
            }
        }
        else
        {
            map1dReportUnexpected(xx, holder);
            result = false;
        }
    }
    // Collect the return values
    if (result)
    {
        result = map1dGetNextAtomInList(nextAtom, numAtoms, inList, holder);
        collector = static_cast<t_binbuf *>(binbuf_new());

        /* Collect the output values, terminated by a semicolon: */
        if (result && collector)
        {
            for ( ; holder.a_type != A_SEMI; )
            {
                /* We have a value; attach it. */
                binbuf_append(collector, NULL_PTR, 1, &holder);
                ++outputCount;
                if (A_SYM == holder.a_type)
                {
                    if ((holder.a_w.w_sym == gDollarSymbol) || (holder.a_w.w_sym == gDollarXSymbol))
                    {
                        ++dollarsPresent;
                    }
                    else if ((holder.a_w.w_sym == gDoubleDollarSymbol) || (holder.a_w.w_sym == gDoubleDollarXSymbol))
                    {
                        ++doubleDollarsPresent;
                    }
                }
                else if (A_DOLLAR == holder.a_type)
                {
                    ++dollarsPresent;
                }
                result = map1dGetNextAtomInList(nextAtom, numAtoms, inList, holder);
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
        newData = GETBYTES(1, RangeData);
        if (newData)
        {
            newData->fNext = NULL_PTR;
            newData->fOutputCount = outputCount;
            newData->fDollarsPresent = dollarsPresent;
            newData->fDoubleDollarsPresent = doubleDollarsPresent;
            newData->fLowerUpperDontCare = lowerUpperDontCare;
            if (! lowerUpperDontCare)
            {
                if (MatchFloat == lowerMatch)
                {
                    setFOI2Float(newData->fLower.fValue, lowerFloatValue);
                }
                else if (MatchInteger == lowerMatch)
                {
                    setFOI2Integer(newData->fLower.fValue, lowerIntValue);
                }
                if (MatchFloat == upperMatch)
                {
                    setFOI2Float(newData->fUpper.fValue, upperFloatValue);
                }
                else if (MatchInteger == upperMatch)
                {
                    setFOI2Integer(newData->fUpper.fValue, upperIntValue);
                }
                newData->fLower.fKind = lowerMatch;
                newData->fUpper.fKind = upperMatch;
                newData->fLower.fIsClosed = lowerClosed;
                newData->fUpper.fIsClosed = upperClosed;
            }
            if (outputCount)
            {
                long     tyOffset = 0;
                long     stOffset = 0;
                t_atom * vector = GETBYTES(newData->fOutputCount, t_atom);

                newData->fOutput = vector;
                for (short ii = 0; ii < newData->fOutputCount; ++ii, ++vector)
                {
                    if (binbuf_getatom(collector, &tyOffset, &stOffset, vector))
                    {
                        break;
                    }
                }
            }
            else
            {
                newData->fOutput = NULL_PTR;
            }
        }
    }
    if (collector)
    {
        freeobject(reinterpret_cast<t_object *>(collector));
    }
    return newData;
} // map1dConvertListToRange
