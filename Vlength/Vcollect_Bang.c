/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       Vcollect_Bang.c                                                         */
/*                                                                                      */
/*  Contains:   The routine cmd_Bang().                                                 */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2002 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2002/08/27                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "Vcollect.h"

/*------------------------------------ cmd_Bang ---*/
Pvoid cmd_Bang
  (VcollectPtr xx)
{
  EnterCallback();
  if (xx)
  {
    switch (xx->fInletNumber)
    {
      case 0:
      case 1:
        /* Left  and right inlet */
        if (xx->fTotalAtoms > 0)
        {
          /* Build up a new list */
          PAtom pile = GETBYTES(xx->fTotalAtoms, Atom);

          if (pile)
          {
            PAtom           outWalker = pile;
            CollectChunkPtr inWalker = xx->fFirstChunk;
            short           toGo = xx->fTotalAtoms, thisMove = CHUNK_SIZE;

            while (inWalker && (toGo > 0))
            {
              if (toGo < CHUNK_SIZE)
                thisMove = toGo;
              memcpy(outWalker, &inWalker->fAtoms, thisMove * sizeof(Atom));
              inWalker = inWalker->fNext;
              outWalker += CHUNK_SIZE;
              toGo -= CHUNK_SIZE;
            }
            genericListOutput(xx->fResultOut, xx->fTotalAtoms, pile);
            FREEBYTES(pile, xx->fTotalAtoms)
          }
        }
        break;

      default:
        LOG_ERROR_2(OUTPUT_PREFIX "unexpected port (%ld) seen", xx->fInletNumber)
        break;

    }
  }
  ExitMaxMessageHandler()
} /* cmd_Bang */
