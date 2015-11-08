/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       map3d_Dump.cpp                                                          */
/*                                                                                      */
/*  Contains:   The routine cmd_Dump().                                                 */
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
/*  Created:    2001/05/02                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "map3d.h"

/*------------------------------------ cmd_Dump ---*/
void cmd_Dump(Map3dData * xx)
{
    if (xx)
    {
        RangeData * walker = xx->fFirstRange;

        for (long ii = 1; walker; walker = walker->fNext, ++ii)
        {
            short    leftRight = static_cast<short>(walker->fLeftRightDontCare ? 1 : 4);
            short    bottomTop = static_cast<short>(walker->fBottomTopDontCare ? 1 : 4);
            short    forwardBack = static_cast<short>(walker->fForwardBackDontCare ? 1 : 4);
            short    outSize = static_cast<short>(walker->fOutputCount + leftRight + bottomTop + forwardBack + 1);
            t_atom * newList = GETBYTES(outSize, Atom);
            t_atom * nextPos;

            if (newList)
            {
                SETLONG(newList, ii);
                nextPos = newList + 1;
                if (walker->fLeftRightDontCare)
                {
                    SETSYM(nextPos, gAsteriskSymbol);
                }
                else
                {
                    SETSYM(nextPos, walker->fLeft.fIsClosed ? gOpenSquareSymbol : gOpenRoundSymbol);
                    if (MatchInfinity == walker->fLeft.fKind)
                    {
                        SETSYM(nextPos + 1, gNegInfSymbol1);
                    }
                    else if (MatchFloat == walker->fLeft.fKind)
                    {
                        SETFLOAT(nextPos + 1, getFOIFloat(walker->fLeft.fValue));
                    }
                    else
                    {
                        SETLONG(nextPos + 1, getFOILong(walker->fLeft.fValue));
                    }
                    if (MatchInfinity == walker->fRight.fKind)
                    {
                        SETSYM(nextPos + 2, gPosInfSymbol1);
                    }
                    else if (MatchFloat == walker->fRight.fKind)
                    {
                        SETFLOAT(nextPos + 2, getFOIFloat(walker->fRight.fValue));
                    }
                    else
                    {
                        SETLONG(nextPos + 2, getFOILong(walker->fRight.fValue));
                    }
                    SETSYM(nextPos + 3, walker->fRight.fIsClosed ? gCloseSquareSymbol : gCloseRoundSymbol);
                }
                nextPos += leftRight;
                if (walker->fBottomTopDontCare)
                {
                    SETSYM(nextPos, gAsteriskSymbol);
                }
                else
                {
                    SETSYM(nextPos, walker->fBottom.fIsClosed ? gOpenSquareSymbol : gOpenRoundSymbol);
                    if (MatchInfinity == walker->fBottom.fKind)
                    {
                        SETSYM(nextPos + 1, gNegInfSymbol1);
                    }
                    else if (MatchFloat == walker->fBottom.fKind)
                    {
                        SETFLOAT(nextPos + 1, getFOIFloat(walker->fBottom.fValue));
                    }
                    else
                    {
                        SETLONG(nextPos + 1, getFOILong(walker->fBottom.fValue));
                    }
                    if (MatchInfinity == walker->fTop.fKind)
                    {
                        SETSYM(nextPos + 2, gPosInfSymbol1);
                    }
                    else if (MatchFloat == walker->fTop.fKind)
                    {
                        SETFLOAT(nextPos + 2, getFOIFloat(walker->fTop.fValue));
                    }
                    else
                    {
                        SETLONG(nextPos + 2, getFOILong(walker->fTop.fValue));
                    }
                    SETSYM(nextPos + 3, walker->fTop.fIsClosed ? gCloseSquareSymbol : gCloseRoundSymbol);
                }
                nextPos += bottomTop;

                if (walker->fForwardBackDontCare)
                {
                    SETSYM(nextPos, gAsteriskSymbol);
                }
                else
                {
                    SETSYM(nextPos, walker->fForward.fIsClosed ? gOpenSquareSymbol : gOpenRoundSymbol);
                    if (MatchInfinity == walker->fForward.fKind)
                    {
                        SETSYM(nextPos + 1, gNegInfSymbol1);
                    }
                    else if (MatchFloat == walker->fForward.fKind)
                    {
                        SETFLOAT(nextPos + 1, getFOIFloat(walker->fForward.fValue));
                    }
                    else
                    {
                        SETLONG(nextPos + 1, getFOILong(walker->fForward.fValue));
                    }
                    if (MatchInfinity == walker->fBack.fKind)
                    {
                        SETSYM(nextPos + 2, gPosInfSymbol1);
                    }
                    else if (MatchFloat == walker->fBack.fKind)
                    {
                        SETFLOAT(nextPos + 2, getFOIFloat(walker->fBack.fValue));
                    }
                    else
                    {
                        SETLONG(nextPos + 2, getFOILong(walker->fBack.fValue));
                    }
                    SETSYM(nextPos + 3, walker->fBack.fIsClosed ? gCloseSquareSymbol : gCloseRoundSymbol);
                }
                nextPos += forwardBack;
                for (short jj = 0; jj < walker->fOutputCount; ++jj)
                {
                    *(nextPos + jj) = *(walker->fOutput + jj);
                }
                outlet_anything(xx->fResultOut, gRangeSymbol, outSize, newList);
                FREEBYTES(newList, outSize);
            }
        }
    }
} // cmd_Dump