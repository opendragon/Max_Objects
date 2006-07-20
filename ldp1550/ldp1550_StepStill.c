/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       ldp1550_StepStill.c                                                     */
/*                                                                                      */
/*  Contains:   The routine cmd_StepStill().                                            */
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

/*------------------------------------ cmd_StepStill ---*/
Pvoid cmd_StepStill
  (LdpControlPtr xx,
   PSymbol       mode)
{
  LdpCommandCode aCommand = LDP_no_command;

  EnterCallback();
  if (xx->fFirst)
  {
    LOG_ERROR_1(OUTPUT_PREFIX "attempt to send 'step&still' command while busy")
    outlet_bang(xx->fErrorBangOut);
  }   
  else if ((mode == gEmptySymbol) || (mode == gFwdSymbol))
    aCommand = LDP_STEP_STILL_FWD;
  else if (mode == gRevSymbol)
    aCommand = LDP_STEP_STILL_REV;
  else
  {
    LOG_ERROR_1(OUTPUT_PREFIX "bad argument to command 'step&still'")
    outlet_bang(xx->fErrorBangOut);
  }
  if (aCommand != LDP_no_command)
  {
    short prevLock = lockout_set(1);

    if (ldpCheckPoolSpace(xx, 3))
    {
      ldpInitCommands(xx);
      ldpAddCommand(xx, aCommand, LDP_AWAITING_ACK);
      ldpAddCommand(xx, LDP_signal_accepted, LDP_not_waiting);
      ldpAddCommand(xx, LDP_signal_done, LDP_not_waiting);
      ldpSendCommand(xx);
    }
    else
    {
      LOG_ERROR_1(OUTPUT_PREFIX "no room for 'step&still' command")
      outlet_bang(xx->fErrorBangOut);
    }
    lockout_set(prevLock);
  }
  ExitMaxMessageHandler()
} /* cmd_StepStill */
