/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       x10_SetClock.c                                                          */
/*                                                                                      */
/*  Contains:   The routine cmd_SetClock().                                             */
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

#include "x10.h"

/*------------------------------------ cmd_SetClock ---*/
Pvoid cmd_SetClock
  (X10ControlPtr xx)
{
  bool okSoFar = true;

  EnterCallback();
  switch (xx->fKind)
  {
    case X10KindCM11:
      if (xx->fMinorState != x10CM11MinorIdle)
        okSoFar = false;
      else
        x10CM11SendClockInfo(xx, false, true);
      break;

    case X10KindCP290:
      if (xx->fMinorState != x10CP290MinorIdle)
        okSoFar = false;
      else
      {
        DateTimeRec now;

        GetTime(&now);
        *xx->fOutCmd = x10CP290SetClock;
        *xx->fOutArea = static_cast<uchar>(now.minute);
        *(xx->fOutArea + 1) = static_cast<uchar>(now.hour);
        *(xx->fOutArea + 2) = static_cast<uchar>(kCP290DateTimeDayToBits[now.dayOfWeek]);
        x10CP290CalcSum(xx, 0, 3);
        x10CP290SendBuffer(xx, 4);
        xx->fMajorState = x10CP290MajorIdle;
        outlet_bang(xx->fCommandComplete);
      }
      break;

  }
  if (! okSoFar)
  {
    LOG_ERROR_1(OUTPUT_PREFIX "attempt to send command while awaiting response.")
    outlet_bang(xx->fErrorBangOut);
  }
  ExitMaxMessageHandler()
} /* cmd_SetClock */
