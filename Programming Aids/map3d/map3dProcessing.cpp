/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       map3dProcessing.cpp                                                     */
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
#include <math.h>

/*------------------------------------ map3dOutputResult ---*/
void map3dOutputResult(Map3dData * xx,
                       RangeData * result)
{
    if (result->fDollarsPresent || result->fDoubleDollarsPresent || result->fSingleDollarsPresent)
    {
        long     outputCount = result->fOutputCount + (2 * result->fDollarsPresent) +
                               (2 * result->fDoubleDollarsPresent);
        t_atom * inWalker = result->fOutput;
        t_atom * tempList = GET_BYTES(outputCount, t_atom);

        if (tempList)
        {
            t_atom * outWalker = tempList;

            for (long ii = 0; ii < result->fOutputCount; ++ii)
            {
                if (A_SYM == inWalker->a_type)
                {
                    if (inWalker->a_w.w_sym == gDollarSymbol)
                    {
                        /* Output previous input */
                        if (xx->fPreviousInput[0].fIsFloat)
                        {
                            atom_setfloat(outWalker, getFOIFloat(xx->fPreviousInput[0]));
                        }
                        else
                        {
                            atom_setlong(outWalker, getFOILong(xx->fPreviousInput[0]));
                        }
                        if (xx->fPreviousInput[1].fIsFloat)
                        {
                            atom_setfloat(outWalker + 1, getFOIFloat(xx->fPreviousInput[1]));
                        }
                        else
                        {
                            atom_setlong(outWalker + 1, getFOILong(xx->fPreviousInput[1]));
                        }
                        if (xx->fPreviousInput[2].fIsFloat)
                        {
                            atom_setfloat(outWalker + 2, getFOIFloat(xx->fPreviousInput[2]));
                        }
                        else
                        {
                            atom_setlong(outWalker + 2, getFOILong(xx->fPreviousInput[2]));
                        }
                        outWalker += 3;
                    }
                    else if (inWalker->a_w.w_sym == gDoubleDollarSymbol)
                    {
                        /* Output offset of previous input */
                        if (result->fLeftRightDontCare)
                        {
                            if (xx->fPreviousInput[0].fIsFloat)
                            {
                                atom_setfloat(outWalker, getFOIFloat(xx->fPreviousInput[0]));
                            }
                            else
                            {
                                atom_setlong(outWalker, getFOILong(xx->fPreviousInput[0]));
                            }
                        }
                        else if (MatchFloat == result->fLeft.fKind)
                        {
                            atom_setfloat(outWalker, getFOIValue(xx->fPreviousInput[0]) -
                                       getFOIValue(result->fLeft.fValue));
                        }
                        else if (MatchInteger == result->fLeft.fKind)
                        {
                            if (xx->fPreviousInput[0].fIsFloat)
                            {
                                atom_setfloat(outWalker, getFOIValue(xx->fPreviousInput[0]) -
                                           getFOIValue(result->fLeft.fValue));
                            }
                            else
                            {
                                atom_setlong(outWalker, getFOILong(xx->fPreviousInput[0]) -
                                          getFOILong(result->fLeft.fValue));
                            }
                        }
                        else if (xx->fPreviousInput[0].fIsFloat)
                        {
                            atom_setfloat(outWalker, getFOIFloat(xx->fPreviousInput[0]));
                        }
                        else
                        {
                            atom_setlong(outWalker, getFOILong(xx->fPreviousInput[0]));
                        }
                        if (result->fBottomTopDontCare)
                        {
                            if (xx->fPreviousInput[1].fIsFloat)
                            {
                                atom_setfloat(outWalker + 1, getFOIFloat(xx->fPreviousInput[1]));
                            }
                            else
                            {
                                atom_setlong(outWalker + 1, getFOILong(xx->fPreviousInput[1]));
                            }
                        }
                        else if (MatchFloat == result->fBottom.fKind)
                        {
                            atom_setfloat(outWalker + 1, getFOIValue(xx->fPreviousInput[1]) -
                                       getFOIValue(result->fBottom.fValue));
                        }
                        else if (MatchInteger == result->fBottom.fKind)
                        {
                            if (xx->fPreviousInput[1].fIsFloat)
                            {
                                atom_setfloat(outWalker + 1, getFOIValue(xx->fPreviousInput[1]) -
                                           getFOIValue(result->fBottom.fValue));
                            }
                            else
                            {
                                atom_setlong(outWalker + 1, getFOILong(xx->fPreviousInput[1]) -
                                          getFOILong(result->fBottom.fValue));
                            }
                        }
                        else if (xx->fPreviousInput[1].fIsFloat)
                        {
                            atom_setfloat(outWalker + 1, getFOIFloat(xx->fPreviousInput[1]));
                        }
                        else
                        {
                            atom_setlong(outWalker + 1, getFOILong(xx->fPreviousInput[1]));
                        }
                        if (result->fForwardBackDontCare)
                        {
                            if (xx->fPreviousInput[2].fIsFloat)
                            {
                                atom_setfloat(outWalker + 2, getFOIFloat(xx->fPreviousInput[2]));
                            }
                            else
                            {
                                atom_setlong(outWalker + 2, getFOILong(xx->fPreviousInput[2]));
                            }
                        }
                        else if (MatchFloat == result->fForward.fKind)
                        {
                            atom_setfloat(outWalker + 2, getFOIValue(xx->fPreviousInput[2]) -
                                       getFOIValue(result->fForward.fValue));
                        }
                        else if (MatchInteger == result->fForward.fKind)
                        {
                            if (xx->fPreviousInput[2].fIsFloat)
                            {
                                atom_setfloat(outWalker + 2, getFOIValue(xx->fPreviousInput[2]) -
                                           getFOIValue(result->fForward.fValue));
                            }
                            else
                            {
                                atom_setlong(outWalker + 2, getFOILong(xx->fPreviousInput[2]) -
                                          getFOILong(result->fBottom.fValue));
                            }
                        }
                        else if (xx->fPreviousInput[2].fIsFloat)
                        {
                            atom_setfloat(outWalker + 2, getFOIFloat(xx->fPreviousInput[2]));
                        }
                        else
                        {
                            atom_setlong(outWalker + 2, getFOILong(xx->fPreviousInput[2]));
                        }
                        outWalker += 3;
                    }
                    else if (inWalker->a_w.w_sym == gDollarXSymbol)
                    {
                        /* Output previous input */
                        if (xx->fPreviousInput[0].fIsFloat)
                        {
                            atom_setfloat(outWalker, getFOIFloat(xx->fPreviousInput[0]));
                        }
                        else
                        {
                            atom_setlong(outWalker, getFOILong(xx->fPreviousInput[0]));
                        }
                        ++outWalker;
                    }
                    else if (inWalker->a_w.w_sym == gDollarYSymbol)
                    {
                        /* Output previous input */
                        if (xx->fPreviousInput[1].fIsFloat)
                        {
                            atom_setfloat(outWalker, getFOIFloat(xx->fPreviousInput[1]));
                        }
                        else
                        {
                            atom_setlong(outWalker, getFOILong(xx->fPreviousInput[1]));
                        }
                        ++outWalker;
                    }
                    else if (inWalker->a_w.w_sym == gDollarZSymbol)
                    {
                        /* Output previous input */
                        if (xx->fPreviousInput[2].fIsFloat)
                        {
                            atom_setfloat(outWalker, getFOIFloat(xx->fPreviousInput[2]));
                        }
                        else
                        {
                            atom_setlong(outWalker, getFOILong(xx->fPreviousInput[2]));
                        }
                        ++outWalker;
                    }
                    else if (inWalker->a_w.w_sym == gDoubleDollarXSymbol)
                    {
                        /* Output offset of previous input */
                        if (result->fLeftRightDontCare)
                        {
                            if (xx->fPreviousInput[0].fIsFloat)
                            {
                                atom_setfloat(outWalker, getFOIFloat(xx->fPreviousInput[0]));
                            }
                            else
                            {
                                atom_setlong(outWalker, getFOILong(xx->fPreviousInput[0]));
                            }
                        }
                        else if (MatchFloat == result->fLeft.fKind)
                        {
                            atom_setfloat(outWalker, getFOIValue(xx->fPreviousInput[0]) -
                                       getFOIValue(result->fLeft.fValue));
                        }
                        else if (MatchInteger == result->fLeft.fKind)
                        {
                            if (xx->fPreviousInput[0].fIsFloat)
                            {
                                atom_setfloat(outWalker, getFOIValue(xx->fPreviousInput[0]) -
                                           getFOIValue(result->fLeft.fValue));
                            }
                            else
                            {
                                atom_setlong(outWalker, getFOILong(xx->fPreviousInput[0]) -
                                          getFOILong(result->fLeft.fValue));
                            }
                        }
                        else if (xx->fPreviousInput[0].fIsFloat)
                        {
                            atom_setfloat(outWalker, getFOIFloat(xx->fPreviousInput[0]));
                        }
                        else
                        {
                            atom_setlong(outWalker, getFOILong(xx->fPreviousInput[0]));
                        }
                        ++outWalker;
                    }
                    else if (inWalker->a_w.w_sym == gDoubleDollarYSymbol)
                    {
                        /* Output offset of previous input */
                        if (result->fBottomTopDontCare)
                        {
                            if (xx->fPreviousInput[1].fIsFloat)
                            {
                                atom_setfloat(outWalker, getFOIFloat(xx->fPreviousInput[1]));
                            }
                            else
                            {
                                atom_setlong(outWalker, getFOILong(xx->fPreviousInput[1]));
                            }
                        }
                        else if (MatchFloat == result->fBottom.fKind)
                        {
                            atom_setfloat(outWalker, getFOIValue(xx->fPreviousInput[1]) -
                                       getFOIValue(result->fBottom.fValue));
                        }
                        else if (MatchInteger == result->fBottom.fKind)
                        {
                            if (xx->fPreviousInput[1].fIsFloat)
                            {
                                atom_setfloat(outWalker, getFOIValue(xx->fPreviousInput[1]) -
                                           getFOIValue(result->fBottom.fValue));
                            }
                            else
                            {
                                atom_setlong(outWalker, getFOILong(xx->fPreviousInput[1]) -
                                          getFOILong(result->fBottom.fValue));
                            }
                        }
                        else if (xx->fPreviousInput[1].fIsFloat)
                        {
                            atom_setfloat(outWalker, getFOIFloat(xx->fPreviousInput[1]));
                        }
                        else
                        {
                            atom_setlong(outWalker, getFOILong(xx->fPreviousInput[1]));
                        }
                        ++outWalker;
                    }
                    else if (inWalker->a_w.w_sym == gDoubleDollarZSymbol)
                    {
                        /* Output offset of previous input */
                        if (result->fForwardBackDontCare)
                        {
                            if (xx->fPreviousInput[2].fIsFloat)
                            {
                                atom_setfloat(outWalker, getFOIFloat(xx->fPreviousInput[2]));
                            }
                            else
                            {
                                atom_setlong(outWalker, getFOILong(xx->fPreviousInput[2]));
                            }
                        }
                        else if (MatchFloat == result->fForward.fKind)
                        {
                            atom_setfloat(outWalker, getFOIValue(xx->fPreviousInput[2]) -
                                       getFOIValue(result->fForward.fValue));
                        }
                        else if (MatchInteger == result->fForward.fKind)
                        {
                            if (xx->fPreviousInput[2].fIsFloat)
                            {
                                atom_setfloat(outWalker, getFOIValue(xx->fPreviousInput[2]) -
                                           getFOIValue(result->fForward.fValue));
                            }
                            else
                            {
                                atom_setlong(outWalker, getFOILong(xx->fPreviousInput[2]) -
                                          getFOILong(result->fBottom.fValue));
                            }
                        }
                        else if (xx->fPreviousInput[2].fIsFloat)
                        {
                            atom_setfloat(outWalker, getFOIFloat(xx->fPreviousInput[2]));
                        }
                        else
                        {
                            atom_setlong(outWalker, getFOILong(xx->fPreviousInput[2]));
                        }
                        ++outWalker;
                    }
                    else
                    {
                        *outWalker = *inWalker;
                        ++outWalker;
                    }
                }
                else if ((A_DOLLAR == inWalker->a_type) || (A_DOLLSYM == inWalker->a_type))
                {
                    /* Output previous input */
                    if (xx->fPreviousInput[0].fIsFloat)
                    {
                        atom_setfloat(outWalker, getFOIFloat(xx->fPreviousInput[0]));
                    }
                    else
                    {
                        atom_setlong(outWalker, getFOILong(xx->fPreviousInput[0]));
                    }
                    if (xx->fPreviousInput[1].fIsFloat)
                    {
                        atom_setfloat(outWalker + 1, getFOIFloat(xx->fPreviousInput[1]));
                    }
                    else
                    {
                        atom_setlong(outWalker + 1, getFOILong(xx->fPreviousInput[1]));
                    }
                    if (xx->fPreviousInput[2].fIsFloat)
                    {
                        atom_setfloat(outWalker + 2, getFOIFloat(xx->fPreviousInput[2]));
                    }
                    else
                    {
                        atom_setlong(outWalker + 2, getFOILong(xx->fPreviousInput[2]));
                    }
                    outWalker += 3;
                }
                else
                {
                    *outWalker = *inWalker;
                    ++outWalker;
                }
                ++inWalker;
            }
            outlet_anything(xx->fResultOut, gResultSymbol, outputCount, tempList);
            FREE_BYTES(tempList);
        }
    }
    else
    {
        outlet_anything(xx->fResultOut, gResultSymbol, result->fOutputCount, result->fOutput);
    }
} // map3dOutputResult

/*------------------------------------ map3dProcessData ---*/
void map3dProcessData(Map3dData *            xx,
                      const FloatOrInteger & xCoord,
                      const FloatOrInteger & yCoord,
                      const FloatOrInteger & zCoord)
{
    RangeData * walker = xx->fFirstRange;

    for ( ; walker; walker = walker->fNext)
    {
        RangeElement toTest = walker->fLeft;
        bool         okSoFar;

        if (walker->fLeftRightDontCare)
        {
            okSoFar = true;
        }
        else if (xCoord.fIsFloat)
        {
            double inValue = getFOIFloat(xCoord);

            switch (toTest.fKind)
            {
                case MatchInfinity:
                    okSoFar = true;
                    break;

                case MatchFloat:
                    if (toTest.fIsClosed)
                    {
                        okSoFar = (inValue >= getFOIFloat(toTest.fValue));
                    }
                    else
                    {
                        okSoFar = (inValue > getFOIFloat(toTest.fValue));
                    }
                    break;

                case MatchInteger:
                    if (toTest.fIsClosed)
                    {
                        okSoFar = (inValue >= getFOILong(toTest.fValue));
                    }
                    else
                    {
                        okSoFar = (inValue > getFOILong(toTest.fValue));
                    }
                    break;

                default:
                    okSoFar = false;
                    break;
                    
            }
            if (okSoFar)
            {
                toTest = walker->fRight;

                switch (toTest.fKind)
                {
                    case MatchInfinity:
                        okSoFar = true;
                        break;

                    case MatchFloat:
                        if (toTest.fIsClosed)
                        {
                            okSoFar = (inValue <= getFOIFloat(toTest.fValue));
                        }
                        else
                        {
                            okSoFar = (inValue < getFOIFloat(toTest.fValue));
                        }
                        break;

                    case MatchInteger:
                        if (toTest.fIsClosed)
                        {
                            okSoFar = (inValue <= getFOILong(toTest.fValue));
                        }
                        else
                        {
                            okSoFar = (inValue < getFOILong(toTest.fValue));
                        }
                        break;

                    default:
                        okSoFar = false;
                        break;
                        
                }
            }
        }
        else
        {
            long inValue = getFOILong(xCoord);

            switch (toTest.fKind)
            {
                case MatchInfinity:
                    okSoFar = true;
                    break;

                case MatchFloat:
                    if (toTest.fIsClosed)
                    {
                        okSoFar = (inValue >= getFOIFloat(toTest.fValue));
                    }
                    else
                    {
                        okSoFar = (inValue > getFOIFloat(toTest.fValue));
                    }
                    break;

                case MatchInteger:
                    if (toTest.fIsClosed)
                    {
                        okSoFar = (inValue >= getFOILong(toTest.fValue));
                    }
                    else
                    {
                        okSoFar = (inValue > getFOILong(toTest.fValue));
                    }
                    break;

                default:
                    okSoFar = false;
                    break;
                    
            }
            if (okSoFar)
            {
                toTest = walker->fRight;
                switch (toTest.fKind)
                {
                    case MatchInfinity:
                        okSoFar = true;
                        break;

                    case MatchFloat:
                        if (toTest.fIsClosed)
                        {
                            okSoFar = (inValue <= getFOIFloat(toTest.fValue));
                        }
                        else
                        {
                            okSoFar = (inValue < getFOIFloat(toTest.fValue));
                        }
                        break;

                    case MatchInteger:
                        if (toTest.fIsClosed)
                        {
                            okSoFar = (inValue <= getFOILong(toTest.fValue));
                        }
                        else
                        {
                            okSoFar = (inValue < getFOILong(toTest.fValue));
                        }
                        break;

                    default:
                        okSoFar = false;
                        break;
                        
                }
            }
        }
        if (! walker->fBottomTopDontCare)
        {
            if (yCoord.fIsFloat)
            {
                double inValue = getFOIFloat(yCoord);

                if (okSoFar)
                {
                    toTest = walker->fBottom;
                    switch (toTest.fKind)
                    {
                        case MatchInfinity:
                            okSoFar = true;
                            break;

                        case MatchFloat:
                            if (toTest.fIsClosed)
                            {
                                okSoFar = (inValue >= getFOIFloat(toTest.fValue));
                            }
                            else
                            {
                                okSoFar = (inValue > getFOIFloat(toTest.fValue));
                            }
                            break;

                        case MatchInteger:
                            if (toTest.fIsClosed)
                            {
                                okSoFar = (inValue >= getFOILong(toTest.fValue));
                            }
                            else
                            {
                                okSoFar = (inValue > getFOILong(toTest.fValue));
                            }
                            break;

                        default:
                            okSoFar = false;
                            break;
                            
                    }
                }
                if (okSoFar)
                {
                    toTest = walker->fTop;
                    switch (toTest.fKind)
                    {
                        case MatchInfinity:
                            okSoFar = true;
                            break;

                        case MatchFloat:
                            if (toTest.fIsClosed)
                            {
                                okSoFar = (inValue <= getFOIFloat(toTest.fValue));
                            }
                            else
                            {
                                okSoFar = (inValue < getFOIFloat(toTest.fValue));
                            }
                            break;

                        case MatchInteger:
                            if (toTest.fIsClosed)
                            {
                                okSoFar = (inValue <= getFOILong(toTest.fValue));
                            }
                            else
                            {
                                okSoFar = (inValue < getFOILong(toTest.fValue));
                            }
                            break;

                        default:
                            okSoFar = false;
                            break;
                            
                    }
                }
            }
            else
            {
                long inValue = getFOILong(yCoord);

                if (okSoFar)
                {
                    toTest = walker->fBottom;
                    switch (toTest.fKind)
                    {
                        case MatchInfinity:
                            okSoFar = true;
                            break;

                        case MatchFloat:
                            if (toTest.fIsClosed)
                            {
                                okSoFar = (inValue >= getFOIFloat(toTest.fValue));
                            }
                            else
                            {
                                okSoFar = (inValue > getFOIFloat(toTest.fValue));
                            }
                            break;

                        case MatchInteger:
                            if (toTest.fIsClosed)
                            {
                                okSoFar = (inValue >= getFOILong(toTest.fValue));
                            }
                            else
                            {
                                okSoFar = (inValue > getFOILong(toTest.fValue));
                            }
                            break;

                        default:
                            okSoFar = false;
                            break;
                            
                    }
                }
                if (okSoFar)
                {
                    toTest = walker->fTop;
                    switch (toTest.fKind)
                    {
                        case MatchInfinity:
                            okSoFar = true;
                            break;

                        case MatchFloat:
                            if (toTest.fIsClosed)
                            {
                                okSoFar = (inValue <= getFOIFloat(toTest.fValue));
                            }
                            else
                            {
                                okSoFar = (inValue < getFOIFloat(toTest.fValue));
                            }
                            break;

                        case MatchInteger:
                            if (toTest.fIsClosed)
                            {
                                okSoFar = (inValue <= getFOILong(toTest.fValue));
                            }
                            else
                            {
                                okSoFar = (inValue < getFOILong(toTest.fValue));
                            }
                            break;

                        default:
                            okSoFar = false;
                            break;
                            
                    }
                }
            }
        }
        if (! walker->fForwardBackDontCare)
        {
            if (zCoord.fIsFloat)
            {
                double inValue = getFOIFloat(zCoord);

                if (okSoFar)
                {
                    toTest = walker->fForward;
                    switch (toTest.fKind)
                    {
                        case MatchInfinity:
                            okSoFar = true;
                            break;

                        case MatchFloat:
                            if (toTest.fIsClosed)
                            {
                                okSoFar = (inValue >= getFOIFloat(toTest.fValue));
                            }
                            else
                            {
                                okSoFar = (inValue > getFOIFloat(toTest.fValue));
                            }
                            break;

                        case MatchInteger:
                            if (toTest.fIsClosed)
                            {
                                okSoFar = (inValue >= getFOILong(toTest.fValue));
                            }
                            else
                            {
                                okSoFar = (inValue > getFOILong(toTest.fValue));
                            }
                            break;

                        default:
                            okSoFar = false;
                            break;
                            
                    }
                }
                if (okSoFar)
                {
                    toTest = walker->fBack;
                    switch (toTest.fKind)
                    {
                        case MatchInfinity:
                            okSoFar = true;
                            break;

                        case MatchFloat:
                            if (toTest.fIsClosed)
                            {
                                okSoFar = (inValue <= getFOIFloat(toTest.fValue));
                            }
                            else
                            {
                                okSoFar = (inValue < getFOIFloat(toTest.fValue));
                            }
                            break;

                        case MatchInteger:
                            if (toTest.fIsClosed)
                            {
                                okSoFar = (inValue <= getFOILong(toTest.fValue));
                            }
                            else
                            {
                                okSoFar = (inValue < getFOILong(toTest.fValue));
                            }
                            break;

                        default:
                            okSoFar = false;
                            break;
                            
                    }
                }
            }
            else
            {
                long inValue = getFOILong(zCoord);

                if (okSoFar)
                {
                    toTest = walker->fForward;
                    switch (toTest.fKind)
                    {
                        case MatchInfinity:
                            okSoFar = true;
                            break;

                        case MatchFloat:
                            if (toTest.fIsClosed)
                            {
                                okSoFar = (inValue >= getFOIFloat(toTest.fValue));
                            }
                            else
                            {
                                okSoFar = (inValue > getFOIFloat(toTest.fValue));
                            }
                            break;

                        case MatchInteger:
                            if (toTest.fIsClosed)
                            {
                                okSoFar = (inValue >= getFOILong(toTest.fValue));
                            }
                            else
                            {
                                okSoFar = (inValue > getFOILong(toTest.fValue));
                            }
                            break;

                        default:
                            okSoFar = false;
                            break;
                            
                    }
                }
                if (okSoFar)
                {
                    toTest = walker->fBack;
                    switch (toTest.fKind)
                    {
                        case MatchInfinity:
                            okSoFar = true;
                            break;

                        case MatchFloat:
                            if (toTest.fIsClosed)
                            {
                                okSoFar = (inValue <= getFOIFloat(toTest.fValue));
                            }
                            else
                            {
                                okSoFar = (inValue < getFOIFloat(toTest.fValue));
                            }
                            break;

                        case MatchInteger:
                            if (toTest.fIsClosed)
                            {
                                okSoFar = (inValue <= getFOILong(toTest.fValue));
                            }
                            else
                            {
                                okSoFar = (inValue < getFOILong(toTest.fValue));
                            }
                            break;

                        default:
                            okSoFar = false;
                            break;
                            
                    }
                }
            }
        }
        if (okSoFar)
        {
            break;
        }
        
    }
    xx->fPreviousResult = walker;
    if (walker)
    {
        // We found a match!
        xx->fPreviousInput[0] = xCoord;
        xx->fPreviousInput[1] = yCoord;
        xx->fPreviousInput[2] = zCoord;
        map3dOutputResult(xx, walker);
    }
} // map3dProcessData
