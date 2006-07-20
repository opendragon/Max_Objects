/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       udpPortNotifier.c                                                       */
/*                                                                                      */
/*  Contains:   The routine tcpServerNotifier().                                        */
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
/*  Created:    2005/07/19                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "udpPort.h"

/*------------------------------------ udpPortNotifier ---*/
pascal void udpPortNotifier
  (Pvoid       context,
   OTEventCode code,
   OTResult    result,
   Pvoid       cookie)
{
#if OPEN_TRANSPORT_SUPPORTED
 #pragma unused(cookie, result)
#else /* not OPEN_TRANSPORT_SUPPORTED */
 #pragma unused(code, result, cookie)
#endif /* not OPEN_TRANSPORT_SUPPORTED */
  UdpObjectPtr xx = static_cast<UdpObjectPtr>(context);

  EnterCallback();
  if (xx)
  {
#if OPEN_TRANSPORT_SUPPORTED
    OSStatus	result = kOTNoError;
    bool			doErrorBang = false;

 #if defined(BE_VERBOSE)
  #if SYSLOG_AVAILABLE
    Syslog(SYSLOG_LEVEL, OUTPUT_PREFIX "notifier code: 0x%lx = %s", long(code),
            mapEventToString(code));
  #endif /* SYSLOG_AVAILABLE */
    if (xx->fVerbose)
      LOG_POST_3(OUTPUT_PREFIX "notifier code: 0x%lx = %s", long(code),
                  mapEventToString(code))
 #endif /* BE_VERBOSE */
    switch (code)
    {
      case T_BINDCOMPLETE:
        setObjectState(xx, TCP_OBJECT_BOUND);
        break;

      case T_DATA:
        doErrorBang = (! makeReceiveBufferAvailable(xx));
        break;

      case T_UNBINDCOMPLETE:
        setObjectState(xx, TCP_OBJECT_UNBOUND);
        break;

			case T_UDERR:
	      WRAP_OT_CALL(xx, result, "OTRcvUDErr", OTRcvUDErr(xx->fEndpoint, NULL_PTR))
				doErrorBang = true;
				break;
				
    }
    if (doErrorBang)
      signalError(xx);            
#endif /* OPEN_TRANSPORT_SUPPORTED */
  }
  ExitCodeResource();
} /* udpPortNotifier */
