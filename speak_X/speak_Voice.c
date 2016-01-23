/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       speak_Voice.c                                                           */
/*                                                                                      */
/*  Contains:   The routine cmd_Voice().                                                */
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
/*  Created:    2001/04/03                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "speak.h"

/*------------------------------------ cmd_Voice ---*/
Pvoid
cmd_Voice(SpeakPtr xx,
          long     newVoice)
{
  EnterCallback();
  if (xx && xx->fSpeaker)
  {
    short voiceCount;

    CountVoices(&voiceCount);
    if ((newVoice > 0) && (newVoice <= voiceCount))
    {
      if (newVoice != xx->fVoiceNumber)
      {
        if (xx->fSpeaker)
        {
          if (xx->fPaused)
          {
            xx->fActive = xx->fPaused = false;
            StopSpeechAt(xx->fSpeaker, kImmediate);
            outlet_bang(xx->fDoneOut);
          }
          else if (xx->fActive)
          {
            LOG_POST_1(OUTPUT_PREFIX "previous speech not finished")
            xx->fActive = false;
            StopSpeechAt(xx->fSpeaker, kImmediate);
            outlet_bang(xx->fDoneOut);
          }
              SetSpeechInfo(xx->fSpeaker, soSpeechDoneCallBack, NULL_PTR);
          DisposeSpeechChannel(xx->fSpeaker);
          xx->fSpeaker = NULL_PTR;
        }
        xx->fVoiceNumber = short(newVoice);
        if ((GetIndVoice(xx->fVoiceNumber, &xx->fVoice) != noErr) ||
              (NewSpeechChannel(&xx->fVoice, &xx->fSpeaker) != noErr))
          LOG_ERROR_1(OUTPUT_PREFIX "problem selecting new voice")
        if ((SetSpeechInfo(xx->fSpeaker, soRefCon, xx) != noErr) ||
              (SetSpeechInfo(xx->fSpeaker, soSpeechDoneCallBack,
                              xx->fCompletionRoutine) != noErr))
          LOG_ERROR_1(OUTPUT_PREFIX "unable to set speech parameters")
      }
    }
    else
      LOG_ERROR_2(OUTPUT_PREFIX "invalid voice number (%ld)", newVoice)
  }
  ExitMaxMessageHandler()
} /* cmd_Voice */
