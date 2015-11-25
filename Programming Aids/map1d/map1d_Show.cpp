/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       map1d_Show.cpp                                                          */
/*                                                                                      */
/*  Contains:   The routine cmd_Show().                                                 */
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
/*  Created:    2001/05/01                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "map1d.h"

/*------------------------------------ cmd_Show ---*/
SHOW_HEADER(Map1dData)
{
    if (xx && (0 < num) && (num <= xx->fRangeCount))
    {
        t_atom_long ii = 1;
        RangeData * walker = xx->fFirstRange;

        for ( ; walker && (ii < num); ++ii)
        {
            walker = walker->fNext;
        }
        if (walker)
        {
            short    lowerUpper = static_cast<short>(walker->fLowerUpperDontCare ? 1 : 4);
            short    outSize = static_cast<short>(walker->fOutputCount + lowerUpper + 1);
            t_atom * newList = GET_BYTES(outSize, t_atom);

            if (newList)
            {
                A_SETLONG(newList, ii);
                if (walker->fLowerUpperDontCare)
                {
                    A_SETSYM(newList + 1, gAsteriskSymbol);
                }
                else
                {
                    A_SETSYM(newList + 1, walker->fLower.fIsClosed ? gOpenSquareSymbol :
                             gOpenRoundSymbol);
                    if (MatchInfinity == walker->fLower.fKind)
                    {
                        A_SETSYM(newList + 2, gNegInfSymbol1);
                    }
                    else if (MatchFloat == walker->fLower.fKind)
                    {
                        A_SETFLOAT(newList + 2, getFOIFloat(walker->fLower.fValue));
                    }
                    else
                    {
                        A_SETLONG(newList + 2, getFOILong(walker->fLower.fValue));
                    }
                    if (MatchInfinity == walker->fUpper.fKind)
                    {
                        A_SETSYM(newList + 3, gPosInfSymbol1);
                    }
                    else if (MatchFloat == walker->fUpper.fKind)
                    {
                        A_SETFLOAT(newList + 3, getFOIFloat(walker->fUpper.fValue));
                    }
                    else
                    {
                        A_SETLONG(newList + 3, getFOILong(walker->fUpper.fValue));
                    }
                    A_SETSYM(newList + 4, walker->fUpper.fIsClosed ? gCloseSquareSymbol :
                             gCloseRoundSymbol);
                }
                for (ii = 0; ii < walker->fOutputCount; ++ii)
                {
                    *(newList + ii + lowerUpper + 1) = *(walker->fOutput + ii);
                }
                outlet_anything(xx->fResultOut, gRangeSymbol, outSize, newList);
                FREE_BYTES(newList);
            }
        }
    }
} // cmd_Show
