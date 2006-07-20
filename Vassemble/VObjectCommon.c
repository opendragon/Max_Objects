/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       VObjectCommon.c                                                         */
/*                                                                                      */
/*  Contains:   Shared routines.                                                        */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2003 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2003/06/16                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "VObjectCommon.h"

/*------------------------------------ checkInput ---*/
bool checkInput
  (Qchar				name,
   const short  argc,
   PAtom  			argv)
{
  bool  okSoFar = true;

  for (short ii = 0; okSoFar && (ii < argc); ii++)
  {
    switch (argv[ii].a_type)
    {
      case A_LONG:
        break;

      case A_SYM:
        LOG_ERROR_3("%ssymbol (%s) in input list", name, argv[ii].a_w.w_sym->s_name)
        okSoFar = false;
        break;

      case A_FLOAT:
        LOG_ERROR_3("%sfloating point value (%g) in input list", name,
                    double(argv[ii].a_w.w_float))
        okSoFar = false;
        break;

      default:
        LOG_ERROR_3("%sinput of an unknown type (%hd) seen", name, argv[ii].a_type)
        okSoFar = false;
        break;

    }
  }
  return okSoFar;
} /* checkInput */

/*------------------------------------ clearPrevious ---*/
void clearPrevious
  (VObjectPtr xx)
{
  FREEBYTES(xx->fPreviousList, xx->fPreviousLength)  
  for (ChunkPtr aChunk = xx->fChunkList; aChunk; )
  {
    ChunkPtr  bChunk = aChunk->fNext;
    
    FREEBYTES(aChunk, 1)
    aChunk = bChunk;
  }
  xx->fChunkList = xx->fLastChunk = NULL_PTR;
  xx->fPreviousLength = 0;
} /* clearPrevious */

/*------------------------------------ isSeparator ---*/
bool isSeparator
	(VObjectPtr	xx,
	 const long	value)
{
	for (short index = 0; index < xx->fHowMany; index++)
	{
		if (xx->fSeparators[index] == value)
			return true;
			
	}
	return false;
} /* isSeparator */

/*------------------------------------ setupSeparators ---*/
bool setupSeparators
	(VObjectPtr xx,
	 const long	separator1,
   const long	separator2,
   const long	separator3,
   const long	separator4,
   const long	separator5)
{
	short numSeps = -1;

	if (separator5 > 0)
	  numSeps = 5;
	else if (separator4 > 0)
	  numSeps = 4;
	else if (separator3 > 0)
	  numSeps = 3;
	else if (separator2 > 0)
	  numSeps = 2;
	else if (separator1 > 0)
	  numSeps = 1;
	switch (numSeps)
	{
	  case 5:
	    xx->fSeparators[4] = separator5;
	    // fall through
	    
	  case 4:
	    xx->fSeparators[3] = separator4;
	    // fall through
	    
	  case 3:
	    xx->fSeparators[2] = separator3;
	    // fall through
	    
	  case 2:
	    xx->fSeparators[1] = separator2;
	    // fall through
	    
	  case 1:
	    xx->fSeparators[0] = separator1;
	    break;
	    
	  default:
	    return false;
	    
	}
	for (short index = 0; index < numSeps; index++)
	{
	  if (xx->fSeparators[index] == 0)
	    return false;

	}
	xx->fHowMany = numSeps;
	return true;
} /* setupSeparators */
