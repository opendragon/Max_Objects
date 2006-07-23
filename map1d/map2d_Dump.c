/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       map2d_Dump.c                                                            */
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

#include "map2d.h"

/*------------------------------------ cmd_Dump ---*/
Pvoid cmd_Dump
  (Map2dPtr xx)
{
  EnterCallback();
  if (xx)
  {
    RangeDataPtr walker = xx->fFirstRange;

    for (long ii = 1; walker; walker = walker->fNext, ++ii)
	  {
	  	short	leftRight = short(walker->fLeftRightDontCare ? 1 : 4);
	  	short	bottomTop = short(walker->fBottomTopDontCare ? 1 : 4);
      short outSize = short(walker->fOutputCount + leftRight + bottomTop + 1);
      PAtom newList = GETBYTES(outSize, Atom), nextPos;

      if (newList)
      {
        SETLONG(newList, ii);
        nextPos = newList + 1;
        if (walker->fLeftRightDontCare)
	        SETSYM(nextPos, gAsteriskSymbol);
        else
        {
	        SETSYM(nextPos, walker->fLeft.fIsClosed ? gOpenSquareSymbol :
	                                                      gOpenRoundSymbol);
	        if (walker->fLeft.fKind == MatchInfinity)
	          SETSYM(nextPos + 1, gNegInfSymbol1);
	        else if (walker->fLeft.fKind == MatchFloat)
	          SETFLOAT(nextPos + 1, getFOIFloat(walker->fLeft.fValue));
	        else
	          SETLONG(nextPos + 1, getFOILong(walker->fLeft.fValue));
	        if (walker->fRight.fKind == MatchInfinity)
	          SETSYM(nextPos + 2, gPosInfSymbol1);
	        else if (walker->fRight.fKind == MatchFloat)
	          SETFLOAT(nextPos + 2, getFOIFloat(walker->fRight.fValue));
	        else
	          SETLONG(nextPos + 2, getFOILong(walker->fRight.fValue));
	        SETSYM(nextPos + 3, walker->fRight.fIsClosed ? gCloseSquareSymbol :
	                                                        gCloseRoundSymbol);
        }
        nextPos += leftRight;
        if (walker->fBottomTopDontCare)
	        SETSYM(nextPos, gAsteriskSymbol);
        else
        {       
	        SETSYM(nextPos, walker->fBottom.fIsClosed ? gOpenSquareSymbol :
	                                                        gOpenRoundSymbol);
	        if (walker->fBottom.fKind == MatchInfinity)
	          SETSYM(nextPos + 1, gNegInfSymbol1);
	        else if (walker->fBottom.fKind == MatchFloat)
	          SETFLOAT(nextPos + 1, getFOIFloat(walker->fBottom.fValue));
	        else
	          SETLONG(nextPos + 1, getFOILong(walker->fBottom.fValue));
	        if (walker->fTop.fKind == MatchInfinity)
	          SETSYM(nextPos + 2, gPosInfSymbol1);
	        else if (walker->fTop.fKind == MatchFloat)
	          SETFLOAT(nextPos + 2, getFOIFloat(walker->fTop.fValue));
	        else
	          SETLONG(nextPos + 2, getFOILong(walker->fTop.fValue));
	        SETSYM(nextPos + 3, walker->fTop.fIsClosed ? gCloseSquareSymbol :
	                                                      gCloseRoundSymbol);
        }
				nextPos += bottomTop;
        for (short jj = 0; jj < walker->fOutputCount; ++jj)
          *(nextPos + jj) = *(walker->fOutput + jj);
        outlet_anything(xx->fResultOut, gRangeSymbol, outSize, newList);
        FREEBYTES(newList, outSize)
      }
    }
  }
  ExitMaxMessageHandler()
} /* cmd_Dump */
