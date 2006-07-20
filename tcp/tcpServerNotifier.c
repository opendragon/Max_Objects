/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       tcpServerNotifier.c                                                     */
/*                                                                                      */
/*  Contains:   The routine tcpServerNotifier().                                        */
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
/*  Created:    1998/08/26                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "tcpServer.h"

/*------------------------------------ processListenEvent ---*/
static void processListenEvent
  (TcpObjectPtr xx)
{
  if (xx)
  {
#if OPEN_TRANSPORT_SUPPORTED
    TCall       call;
    InetAddress caddr;
    OTResult    look_result;
    //OTLink *    acceptor_link;
    //EPInfo *    acceptor;
    OSStatus    result;

    call.addr.maxlen = sizeof(caddr);
    call.addr.buf = reinterpret_cast<Puchar>(&caddr);
    call.opt.maxlen = 0;
    call.opt.buf = NULL_PTR;
    call.udata.maxlen = 0;
    call.udata.buf = NULL_PTR;
    WRAP_OT_CALL(xx, result, "OTListen", OTListen(xx->fEndpoint, &call))
    if (result == kOTNoDataErr)
      return;

    if (result != kOTNoError)
    {
      look_result = OTLook(xx->fEndpoint);
      if ((result == kOTLookErr) && (look_result == T_DISCONNECT))
      {
        //          DoRcvDisconnect(xx);
      }
      else
      {
        //          DBAlert2("Notifier: T_LISTEN - OTListen error %d look_result %x",
        //                  result, look_result);
      }
      return; 

    }
    WRAP_OT_CALL(xx, result, "OTAccept", OTAccept(xx->fEndpoint, xx->fEndpoint, &call))
    if (result != kOTNoError)
    {
      look_result = OTLook(xx->fEndpoint);
      if ((result == kOTLookErr) && (look_result == T_DISCONNECT))
      {
        //          DoRcvDisconnect(xx);
      }
      else
      {
        //          DBAlert2("Notifier: T_LISTEN - OTAccept error %d look_result %x",
        //                      result, look_result);
      }
    }
#endif /* OPEN_TRANSPORT_SUPPORTED */
  }
} /* processListenEvent */

/*------------------------------------ tcpServerNotifier ---*/
pascal void tcpServerNotifier
  (Pvoid       context,
   OTEventCode code,
   OTResult    result,
   Pvoid       cookie)
{
#if OPEN_TRANSPORT_SUPPORTED
 #pragma unused(result, cookie)
#else /* not OPEN_TRANSPORT_SUPPORTED */
 #pragma unused(code, result, cookie)
#endif /* not OPEN_TRANSPORT_SUPPORTED */
  TcpObjectPtr xx = static_cast<TcpObjectPtr>(context);

  EnterCallback();
  if (xx)
  {
#if OPEN_TRANSPORT_SUPPORTED
    OSStatus err;
    //OTResult endpointState;
    TDiscon  discon;
    bool     do_error_bang = false;

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
      case T_ACCEPTCOMPLETE:
        setObjectState(xx, TCP_OBJECT_CONNECTED);
        break;

      case T_BINDCOMPLETE:
        setObjectState(xx, TCP_OBJECT_BOUND);
        break;

      case T_CONNECT:
        break;

      case T_DATA:
        do_error_bang = (! makeReceiveBufferAvailable(xx));
        break;

      case T_DISCONNECT:
        WRAP_OT_CALL(xx, err, "OTRcvDisconnect", OTRcvDisconnect(xx->fEndpoint, &discon))
        if (err == kOTNoError)
        {
 #if defined(BE_VERBOSE)
          if (xx->fVerbose)
            LOG_POST_2(OUTPUT_PREFIX "disconnect reason (%ld)",
                        long(E2OSStatus(discon.reason)))
 #endif /* BE_VERBOSE */
        }
        else
        {
          if (err == kOTNoDisconnectErr)
            break;

          REPORT_ERROR(OUTPUT_PREFIX "OTRcvDisconnect failed (%ld = %s)", err)
	        reportEndpointState(OUR_NAME, xx);
          do_error_bang = true;
        }
        setObjectState(xx, TCP_OBJECT_LISTENING);
        break;

      case T_DISCONNECTCOMPLETE:
        setObjectState(xx, TCP_OBJECT_LISTENING);
        break;

      case T_EXDATA:
        break;

      case T_GODATA:
        break;

      case T_GOEXDATA:
        break;

      case T_LISTEN:
        if (xx->fState == TCP_OBJECT_LISTENING)
          processListenEvent(xx);
        else
        {
          LOG_ERROR_1(OUTPUT_PREFIX "Listen request received while not listening")
          do_error_bang = true;
        }
        break;

      case T_MEMORYRELEASED:
        break;

      case T_ORDREL:
        WRAP_OT_CALL(xx, err, "OTRcvOrderlyDisconnect",
                      OTRcvOrderlyDisconnect(xx->fEndpoint))
        if (err == kOTNoError)
        {
          //
        }
        else
        {
          if (err == kOTNoDisconnectErr)
            break;

          REPORT_ERROR(OUTPUT_PREFIX "OTRcvOrderlyDisconnect failed (%ld = %s)", err)
	        reportEndpointState(OUR_NAME, xx);
          do_error_bang = true;
        }
        if (xx->fState != TCP_OBJECT_DISCONNECTING)
        {
          WRAP_OT_CALL(xx, err, "OTSndOrderlyDisconnect",
          OTSndOrderlyDisconnect(xx->fEndpoint))
          if (err != kOTNoError)
          {
            REPORT_ERROR(OUTPUT_PREFIX "OTSndOrderlyDisconnect failed (%ld = %s)", err)
		        reportEndpointState(OUR_NAME, xx);
            do_error_bang = true;
          }
        }
        setObjectState(xx, TCP_OBJECT_BOUND);
        break;

      case T_PASSCON:
        break;

      case T_UNBINDCOMPLETE:
        setObjectState(xx, TCP_OBJECT_UNBOUND);
        break;

      default:
        break;

    }
    if (do_error_bang)
      signalError(xx);
#endif /* OPEN_TRANSPORT_SUPPORTED */       
  }
  ExitCodeResource();
} /* tcpServerNotifier */
