/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       ldp1550_PlayTill.c                                                      */
/*                                                                                      */
/*  Contains:   The routine cmd_PlayTill().                                             */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 1996 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    1996/09/21                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "ldp1550.h"

/*------------------------------------ cmd_PlayTill ---*/
Pvoid cmd_PlayTill
  (LdpControlPtr xx,
   long          position,
   PSymbol       mode,
   long          stepFactor)
{
  long           maxPosition;
  LdpCommandCode aCommand = LDP_no_command;
  short          speed = LDP_PLAY_FWD;

  EnterCallback();
  if (xx->fFirst)
  {
    LOG_ERROR_1(OUTPUT_PREFIX "attempt to send 'playtill' command while busy")
    outlet_bang(xx->fErrorBangOut);
  }
  else
  {
    maxPosition = ((xx->fMode == LDP_CHAPTER_MODE) ? MAX_CHAPTER_NUMBER :
                                                      MAX_FRAME_NUMBER);
    if ((position <= 0) || (position >= maxPosition) || (stepFactor < 0) ||
        (stepFactor > MAX_STEP_FACTOR))
    {
      LOG_ERROR_1(OUTPUT_PREFIX "bad argument to command 'playtill'")
      outlet_bang(xx->fErrorBangOut);
    }
    else if ((mode != gEmptySymbol) && (mode != gFwdSymbol) && (mode != gFastSymbol) &&
              (mode != gStepSymbol) && (mode != gSlowSymbol))
    {
      LOG_ERROR_1(OUTPUT_PREFIX "bad mode for command 'playtill'")
      outlet_bang(xx->fErrorBangOut);
    }
    else
    {
      aCommand = LDP_REPEAT;
      if (! stepFactor)
        stepFactor = 1;
      if (mode == gFastSymbol)
        speed = LDP_PLAY_F_FWD;
      else if (mode == gSlowSymbol)
        speed = LDP_PLAY_S_FWD;
      else if (mode == gStepSymbol)
        speed = LDP_STEP_FWD;
    }
    if (aCommand != LDP_no_command)
    {
      short prevLock = lockout_set(1), numCommands = 12;

      if (xx->fMode == LDP_FRAME_MODE)
        numCommands += 3;
      if (mode == gStepSymbol)
        numCommands += 4;
      if (ldpCheckPoolSpace(xx, numCommands))
      {
        ldpInitCommands(xx);
        ldpAddCommand(xx, aCommand, LDP_AWAITING_ACK);
        /* Set the target frame */
        if (xx->fMode == LDP_FRAME_MODE)
        {
          ldpAddCommand(xx, static_cast<LdpCommandCode>(((position / 10000) % 10) + '0'),
                        LDP_AWAITING_ACK);
          ldpAddCommand(xx, static_cast<LdpCommandCode>(((position / 1000) % 10) + '0'),
                        LDP_AWAITING_ACK);
          ldpAddCommand(xx, static_cast<LdpCommandCode>(((position / 100) % 10) + '0'),
                        LDP_AWAITING_ACK);
        }
        ldpAddCommand(xx, static_cast<LdpCommandCode>(((position / 10) % 10) + '0'),
                      LDP_AWAITING_ACK);
        ldpAddCommand(xx, static_cast<LdpCommandCode>((position % 10) + '0'),
                      LDP_AWAITING_ACK);
        /* Set the play mode */
        ldpAddCommand(xx, static_cast<LdpCommandCode>(speed), LDP_AWAITING_ACK);
        ldpAddCommand(xx, LDP_ENTER, LDP_AWAITING_ACK);
        /* Set the repeat count */
        ldpAddCommand(xx, static_cast<LdpCommandCode>('0'), LDP_AWAITING_ACK);
        ldpAddCommand(xx, static_cast<LdpCommandCode>('1'), LDP_AWAITING_ACK);
        ldpAddCommand(xx, LDP_ENTER, LDP_AWAITING_ACK);
        /* If step mode, send the step factor */
        if (mode == gStepSymbol)
        {
          ldpAddCommand(xx, static_cast<LdpCommandCode>(((stepFactor / 100) % 10) + '0'),
                        LDP_AWAITING_ACK);
          ldpAddCommand(xx, static_cast<LdpCommandCode>(((stepFactor / 10) % 10) + '0'),
                        LDP_AWAITING_ACK);
          ldpAddCommand(xx, static_cast<LdpCommandCode>((stepFactor % 10) + '0'),
                        LDP_AWAITING_ACK);
          ldpAddCommand(xx, LDP_ENTER, LDP_AWAITING_ACK);
        }
        ldpAddCommand(xx, LDP_signal_accepted, LDP_not_waiting);
        ldpAddCommand(xx, LDP_can_interrupt, LDP_AWAITING_COMPLETE);
        ldpAddCommand(xx, LDP_signal_done, LDP_not_waiting);
        ldpSendCommand(xx);
      }
      else
      {
        LOG_ERROR_1(OUTPUT_PREFIX "no room for 'playtill' command")
        outlet_bang(xx->fErrorBangOut);
      }
      lockout_set(prevLock);
    }
  }
  ExitMaxMessageHandler()
} /* cmd_PlayTill */
