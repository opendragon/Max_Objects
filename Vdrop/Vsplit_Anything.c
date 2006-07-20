/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       Vsplit_Anything.c                                                       */
/*                                                                                      */
/*  Contains:   The routine cmd_Anything().                                             */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2005 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2005/07/23                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "Vsplit.h"

/*------------------------------------ cmd_Anything ---*/
Pvoid cmd_Anything
  (VObjectPtr xx,
   PSymbol    message,
   short      argc,
   PAtom      argv)
{
  EnterCallback();
  if (xx)
  {
    short leftCount = 0, rightCount = 0;
    PAtom	leftAtoms = NULL_PTR, rightAtoms = NULL_PTR;

    clearPrevious(xx);
		if (xx->fHowMany > 0)
		{
			if (argc >= xx->fHowMany)
			{
				// We have a split
				leftCount = xx->fHowMany;
				rightCount = short(argc + 1 - leftCount);
				leftAtoms = GETBYTES(leftCount, Atom);
				rightAtoms = GETBYTES(rightCount, Atom);
        SETSYM(leftAtoms, message);
        if (leftCount > 1)
          memcpy(leftAtoms + 1, argv, (leftCount - 1) * sizeof(Atom));
				memcpy(rightAtoms, argv + leftCount - 1, rightCount * sizeof(Atom));
			}
			else
			{
				// Everything goes to the left
				leftCount = short(argc + 1);
				leftAtoms = GETBYTES(leftCount, Atom);
        SETSYM(leftAtoms, message);
        memcpy(leftAtoms + 1, argv, argc * sizeof(Atom));		
			}
		}
		else if (xx->fHowMany < 0)
		{
			if (argc >= (- xx->fHowMany))
			{
				// We have a split
				rightCount = short(- xx->fHowMany);
				leftCount = short(argc + 1 - rightCount);
				leftAtoms = GETBYTES(leftCount, Atom);
				rightAtoms = GETBYTES(rightCount, Atom);
        SETSYM(leftAtoms, message);
        if (leftCount > 1)
          memcpy(leftAtoms + 1, argv, (leftCount - 1) * sizeof(Atom));
				memcpy(rightAtoms, argv + leftCount - 1, rightCount * sizeof(Atom));			
			}
			else
			{
				// Everything goes to the right
				rightCount = short(argc + 1);
				rightAtoms = GETBYTES(rightCount, Atom);
        SETSYM(rightAtoms, message);
        memcpy(rightAtoms + 1, argv, argc * sizeof(Atom));		
			}
		}
    xx->fPreviousList = leftAtoms;
    xx->fPreviousLength = leftCount;
    xx->fPreviousRightList = rightAtoms;
    xx->fPreviousRightLength = rightCount;
  	if (xx->fPreviousRightList)
			genericListOutput(xx->fRightResultOut, xx->fPreviousRightLength, xx->fPreviousRightList);
    if (xx->fPreviousList)
      genericListOutput(xx->fResultOut, xx->fPreviousLength, xx->fPreviousList);
  }
  ExitMaxMessageHandler()
} /* cmd_Anything */
