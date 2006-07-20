/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       serialX_Bang.c                                                          */
/*                                                                                      */
/*  Contains:   The routine cmd_Bang().                                                 */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 1998 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    1998/06/21                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "serialX.h"

/*------------------------------------ cmd_Bang ---*/
Pvoid cmd_Bang
  (SerialXControlPtr xx)
{
  long          inCount, nextVal;
  ParamBlockRec receiveParamBlock;
  OSErr         err;
  Handle        holdBuffer = NULL_HDL;
  Ptr           nextChar = NULL_PTR;
  SerStaRec     serStatus;
  short         jj = 0;
  static bool   sawBreak = false;

  EnterCallback();
#if defined(COMPILE_FOR_CATS)
//TBD
#else /* not COMPILE_FOR_CATS */
  SerStatus(xx->fInRefNum, &serStatus);
  if ((serStatus.cumErrs & breakErr) != 0)
    sawBreak = true;
  inCount = 0;
  err = SerGetBuf(xx->fInRefNum, &inCount);
#endif /* not COMPILE_FOR_CATS */
  if ((err == noErr) && (inCount > 0))
  {
    if (inCount < LOCBUFFER_SIZE)
      nextChar = xx->fLocalBuffer;
    else
    {
      holdBuffer = NewHandle(inCount);
      if (holdBuffer)
      {
        HLock(holdBuffer);
        nextChar = *holdBuffer;
      }
    }
    if (nextChar)
    {
      receiveParamBlock.ioParam.ioRefNum = xx->fInRefNum;
      receiveParamBlock.ioParam.ioBuffer = nextChar;
      receiveParamBlock.ioParam.ioReqCount = inCount;
      receiveParamBlock.ioParam.ioCompletion = NULL_PTR;
      receiveParamBlock.ioParam.ioVRefNum = 0;
      receiveParamBlock.ioParam.ioPosMode = 0;
      if (PBRead(&receiveParamBlock, false) == noErr)
      {
        /* send the received characters out, one at a time or in chunks */
        for (long ii = 0; ii < inCount; ii++)
        {
          nextVal = *nextChar++;
          if (sawBreak && (! nextVal))
          {
            sawBreak = false;
            if (! outlet_bang(xx->fBreakSeen))
              break;

          }
          else if (xx->fChunked)
          {
            SETLONG(xx->fChunkData + jj, nextVal);
            if (++jj == xx->fChunkSize)
            {
              if (! outlet_list(xx->fCharOut, NULL_PTR, xx->fChunkSize, xx->fChunkData))
              break;

              jj = 0;
            }
          }
          else if (! outlet_int(xx->fCharOut, nextVal))
            break;

        }
      }
      if (xx->fChunked && (jj != 0))
        /* Post any remaining data. */
        outlet_list(xx->fCharOut, NULL_PTR, jj, xx->fChunkData);
      if (holdBuffer)
      {           
        HUnlock(holdBuffer);
        DisposeHandle(holdBuffer);
      }           
    }
  }
  ExitMaxMessageHandler()
} /* cmd_Bang */
