/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       bqt_Border.c                                                            */
/*                                                                                      */
/*  Contains:   The routine cmd_Border().                                               */
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
/*  Created:    1998/10/24                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "bqt.h"

/*------------------------------------ deferred_Border ---*/
static Pvoid deferred_Border
  (QtPtr xx,
   long  num)
{
  EnterCallback();
  if (xx->fCurrentMovie)
  {
    bool request = (num != 0);

    if (xx->fBorderState != request)
    {
      GrafPtr oldPort;

      xx->fBorderState = request;
      xx->fVariant.fBox.b_outline = request;
      oldPort = patcher_setport(xx->fVariant.fBox.b_patcher);
      if (oldPort)
      {
        if (! box_nodraw(reinterpret_cast<PBox>(xx)))
          box_erase(reinterpret_cast<PBox>(xx));
        SetPort(oldPort);
      }
    }
  }
  else
  {
    LOG_ERROR_1(OUTPUT_PREFIX "no movies loaded")
    SignalError(xx);
  }
  ExitMaxMessageHandler()
} /* deferred_Border */

/*------------------------------------ cmd_Border ---*/
Pvoid cmd_Border
  (QtPtr xx,
   long  num)
{
  EnterCallback();
#if defined(TRACE_MESSAGES)
  LOG_POST_1("Border")
#endif /* TRACE_MESSAGES */
  defer(xx, reinterpret_cast<method>(deferred_Border),
        reinterpret_cast<PSymbol>(num), 0, NULL_PTR);
  ExitMaxMessageHandler()
} /* cmd_Border */
