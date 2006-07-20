/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       map1d_Dump.c                                                            */
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

#include "map1d.h"

/*------------------------------------ cmd_Dump ---*/
Pvoid cmd_Dump
  (Map1dPtr xx)
{
  EnterCallback();
  if (xx)
  {
    RangeDataPtr walker = xx->fFirstRange;

    for (long ii = 1; walker; walker = walker->fNext, ii++)
    {
      short	lowerUpper = short(walker->fLowerUpperDontCare ? 1 : 4);
      short outSize = short(walker->fOutputCount + lowerUpper + 1);
      PAtom newList = GETBYTES(outSize, Atom);

      if (newList)
      {
        SETLONG(newList, ii);
        if (walker->fLowerUpperDontCare)
	        SETSYM(newList + 1, gAsteriskSymbol);
        else
        {
	        SETSYM(newList + 1, walker->fLower.fIsClosed ? gOpenSquareSymbol :
	                                                        gOpenRoundSymbol);
	        if (walker->fLower.fKind == MatchInfinity)
	          SETSYM(newList + 2, gNegInfSymbol1);
	        else if (walker->fLower.fKind == MatchFloat)
	          SETFLOAT(newList + 2, getFOIFloat(walker->fLower.fValue));
	        else
	          SETLONG(newList + 2, getFOILong(walker->fLower.fValue));
	        if (walker->fUpper.fKind == MatchInfinity)
	          SETSYM(newList + 3, gPosInfSymbol1);
	        else if (walker->fUpper.fKind == MatchFloat)
	          SETFLOAT(newList + 3, getFOIFloat(walker->fUpper.fValue));
	        else
	          SETLONG(newList + 3, getFOILong(walker->fUpper.fValue));
	        SETSYM(newList + 4, walker->fUpper.fIsClosed ? gCloseSquareSymbol :
	                                                        gCloseRoundSymbol);
        }
        for (short jj = 0; jj < walker->fOutputCount; jj++)
          *(newList + jj + lowerUpper + 1) = *(walker->fOutput + jj);
        outlet_anything(xx->fResultOut, gRangeSymbol, outSize, newList);
        FREEBYTES(newList, outSize)
      }
    }
  }
  ExitMaxMessageHandler()
} /* cmd_Dump */
