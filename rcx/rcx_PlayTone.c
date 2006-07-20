/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       rcx_PlayTone.c                                                          */
/*                                                                                      */
/*  Contains:   The routine cmd_PlayTone().                                             */
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
/*  Created:    2002/03/31                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "rcx.h"

/*------------------------------------ cmd_PlayTone ---*/
Pvoid cmd_PlayTone
  (RcxControlPtr xx,
   long          frequency,
   long          duration)
{
#if (! __powerc)
 #pragma unused(frequency, duration)
#endif /* not __powerc */
  EnterCallback();
  if (xx)
  {
#if __powerc
    if (rcxSynchronize(xx))
    {
      uchar playToneCommand[] = { RCX_PLAY_TONE_CMD, 0, 0, 0 };

      playToneCommand[1] = static_cast<uchar>(frequency & 0x0FF);
      playToneCommand[2] = static_cast<uchar>((frequency >> 8) & 0x0FF);
      playToneCommand[3] = static_cast<uchar>(duration);
      if (rcxSendCommand(xx, playToneCommand, sizeof(playToneCommand),
                          RCX_PLAY_TONE_REPLY, true))
        outlet_bang(xx->fCommandComplete);
      else
        outlet_bang(xx->fErrorBangOut);
    }
    else
      outlet_bang(xx->fErrorBangOut);
#endif /* __powerc */
  }
  ExitMaxMessageHandler()
} /* cmd_PlayTone */
