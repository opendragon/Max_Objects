/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       tcpMultiServer_Listen.c                                                 */
/*                                                                                      */
/*  Contains:   The routine cmd_Listen().                                               */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2000 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2000/10/14                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "tcpMultiServer.h"

/*------------------------------------ cmd_Listen ---*/
Pvoid cmd_Listen
  (TcpMultiServerPtr xx,
   PSymbol           onOff)
{
  bool okSoFar = true;

  EnterCallback();
  REPORT_MAX_MESSAGE("listen")
  if (xx)
  {
#if OPEN_TRANSPORT_SUPPORTED
    if ((onOff != gOffSymbol) && (onOff != gOnSymbol))
    {
      LOG_ERROR_2(OUTPUT_PREFIX "invalid mode '%s'", onOff->s_name)
      okSoFar = false;
    }
    if (okSoFar)
    {
      switch (xx->fState)
      {
        case TCP_OBJECT_UNBOUND:
          LOG_ERROR_1(OUTPUT_PREFIX "not bound")
          okSoFar = false;
          break;

        case TCP_OBJECT_BOUND:
          if (onOff == gOffSymbol)
          {
            LOG_ERROR_1(OUTPUT_PREFIX "not listening")
            okSoFar = false;
          }
          break;

        case TCP_OBJECT_LISTENING:
          if (onOff == gOnSymbol)
          {
            LOG_ERROR_1(OUTPUT_PREFIX "already listening")
            okSoFar = false;
          }
          break;

        default:
          LOG_ERROR_3(OUTPUT_PREFIX "unexpected state (%ld = %s)", long(xx->fState),
                      mapStateToSymbol(xx->fState)->s_name)
          okSoFar = false;
          break;

      }
    }
#endif /* OPEN_TRANSPORT_SUPPORTED */
    if (okSoFar)
    {       
      if (onOff == gOffSymbol)
        setObjectState(xx, TCP_OBJECT_BOUND);
      else if (onOff == gOnSymbol)
        setObjectState(xx, TCP_OBJECT_LISTENING);
    }
    else
      signalError(xx);
  }
  ExitMaxMessageHandler()
} /* cmd_Listen */