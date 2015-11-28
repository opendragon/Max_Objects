/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       map1dProcessing.cpp                                                     */
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
#include <math.h>

/*------------------------------------ map1dOutputResult ---*/
void map1dOutputResult(Map1dData * xx,
                       RangeData * result)
{
    if (result->fDollarsPresent || result->fDoubleDollarsPresent)
    {
        long     outputCount = result->fOutputCount;
        t_atom * inWalker = result->fOutput;
        t_atom * tempList = GET_BYTES(outputCount, t_atom);

        if (tempList)
        {
            t_atom * outWalker = tempList;

            for (long ii = 0; ii < result->fOutputCount; ++ii)
            {
                if (A_SYM == inWalker->a_type)
                {
                    if ((inWalker->a_w.w_sym == gDollarSymbol) ||
                        (inWalker->a_w.w_sym == gDollarXSymbol))
                    {
                        /* Output previous input */
                        if (xx->fPreviousInput.fIsFloat)
                        {
                            atom_setfloat(outWalker, getFOIFloat(xx->fPreviousInput));
                        }
                        else
                        {
                            atom_setlong(outWalker, getFOILong(xx->fPreviousInput));
                        }
                    }
                    else if ((inWalker->a_w.w_sym == gDoubleDollarSymbol) ||
                             (inWalker->a_w.w_sym == gDoubleDollarXSymbol))
                    {
                        /* Output offset of previous input */
                        if (result->fLowerUpperDontCare)
                        {
                            if (xx->fPreviousInput.fIsFloat)
                            {
                                atom_setfloat(outWalker, getFOIFloat(xx->fPreviousInput));
                            }
                            else
                            {
                                atom_setlong(outWalker, getFOILong(xx->fPreviousInput));
                            }
                        }
                        if (MatchFloat == result->fLower.fKind)
                        {
                            atom_setfloat(outWalker, getFOIValue(xx->fPreviousInput) -
                                       getFOIValue(result->fLower.fValue));
                        }
                        else if (MatchInteger == result->fLower.fKind)
                        {
                            if (xx->fPreviousInput.fIsFloat)
                            {
                                atom_setfloat(outWalker, getFOIValue(xx->fPreviousInput) -
                                           getFOIValue(result->fLower.fValue));
                            }
                            else
                            {
                                atom_setlong(outWalker, getFOILong(xx->fPreviousInput) -
                                          getFOILong(result->fLower.fValue));
                            }
                        }
                        else if (xx->fPreviousInput.fIsFloat)
                        {
                            atom_setfloat(outWalker, getFOIFloat(xx->fPreviousInput));
                        }
                        else
                        {
                            atom_setlong(outWalker, getFOILong(xx->fPreviousInput));
                        }
                    }
                    else
                    {
                        *outWalker = *inWalker;
                    }
                }
                else if ((A_DOLLAR == inWalker->a_type) || (A_DOLLSYM == inWalker->a_type))
                {
                    /* Output previous input */
                    if (xx->fPreviousInput.fIsFloat)
                    {
                        atom_setfloat(outWalker, getFOIFloat(xx->fPreviousInput));
                    }
                    else
                    {
                        atom_setlong(outWalker, getFOILong(xx->fPreviousInput));
                    }
                }
                else
                {
                    *outWalker = *inWalker;
                }
                ++outWalker;
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
} // map1dOutputResult

/*------------------------------------ map1dProcessData ---*/
void map1dProcessData(Map1dData *            xx,
                      const FloatOrInteger & input)
{
    RangeData * walker = xx->fFirstRange;

    if (input.fIsFloat)
    {
        double inValue = getFOIFloat(input);

        for ( ; walker; walker = walker->fNext)
        {
            RangeElement toTest = walker->fLower;
            bool         okSoFar;

            if (walker->fLowerUpperDontCare)
            {
                break;
            }

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
                toTest = walker->fUpper;
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
            if (okSoFar)
            {
                break;
            }
        }
    }
    else
    {
        long inValue = getFOILong(input);

        for ( ; walker; walker = walker->fNext)
        {
            RangeElement toTest = walker->fLower;
            bool         okSoFar;

            if (walker->fLowerUpperDontCare)
            {
                break;
            }

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
                toTest = walker->fUpper;
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
            if (okSoFar)
            {
                break;
            }
            
        }
    }
    xx->fPreviousResult = walker;
    if (walker)
    {
        // We found a match!
        xx->fPreviousInput = input;
        map1dOutputResult(xx, walker);
    }
} // map1dProcessData
