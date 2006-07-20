/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       tcpClient.c                                                             */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the tcpClient module.     */
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
/*  Created:    1998/09/23                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#define IS_MAIN /* */
#include "tcpClient.h"
#include "displayAboutBox.h"
#include "loadAboutBoxData.h"
#include "loadOtherSegments.h"
#include "reportVersion.h"

/* Forward references: */
Pvoid tcpClientCreate
  (PSymbol	ipAddress,
   long			port,
   long			numBuffers);

Pvoid tcpClientFree
  (TcpObjectPtr	xx);

/*------------------------------------ main ---*/
void main
  (Pfptr ff)
{
  EnterCodeResource();
  PrepareCallback();
  FNS = ff;   /* Connect up the function macros. */
  if (isOpenTransportAvailable(OUR_NAME))
  {
    /* Allocate class memory and set up class. */
    setup(reinterpret_cast<t_messlist**>(&gClass),
          reinterpret_cast<method>(tcpClientCreate),
          reinterpret_cast<method>(tcpClientFree), short(sizeof(TcpObjectData)),
          reinterpret_cast<method>(0L), A_DEFSYM, A_DEFLONG, A_DEFLONG, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Anything), "anything", A_GIMME, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Assist), "assist", A_CANT, A_NOTHING);
    addbang(reinterpret_cast<method>(cmd_Status));
    addmess(reinterpret_cast<method>(cmd_Connect), "connect", A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Disconnect), "disconnect", A_NOTHING);
    addfloat(reinterpret_cast<method>(cmd_Float));
    addmess(reinterpret_cast<method>(cmd_Info), "info", A_NOTHING);
    addint(reinterpret_cast<method>(cmd_Int));
    addmess(reinterpret_cast<method>(cmd_List), "list", A_GIMME, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Mode), "mode", A_SYM, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Port), "port", A_LONG, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Self), "self", A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Send), "send", A_GIMME, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Server), "server", A_SYM, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Status), "status", A_NOTHING);
#if defined(BE_VERBOSE)
    addmess(reinterpret_cast<method>(cmd_Verbose), "verbose", A_DEFSYM, A_NOTHING);
#endif /* BE_VERBOSE */
    /* Add object class to New Object List */
    finder_addclass("Devices", OUR_NAME);
    /* Make sure that our string list resource is present */
    rescopy('STR#', OUR_RES_NUMB);
    loadAboutBoxData();
    gDollarSymbol = gensym("$");
    gEmptySymbol = gensym("");
    gMaxSymbol = gensym("max");
    gOffSymbol = gensym("off");
    gOnSymbol = gensym("on");
    gRawSymbol = gensym("raw");
    gReplySymbol = gensym("reply");
    gSelfSymbol = gensym("self");
    gStatusSymbol = gensym("status");
    setUpStateSymbols();
    gVersionNumber = reportVersion(OUR_NAME);
    loadOtherSegments();
  }
#if (! OPEN_TRANSPORT_SUPPORTED)
  error(OUTPUT_PREFIX "not supported on Max/68K");
#endif /* not OPEN_TRANSPORT_SUPPORTED */
  ExitCodeResource();
} /* main */

#define CHECK_A_BYTE(endChar, sumPtr)    \
{\
  for (accum = -1; okSoFar; )\
  {\
    a_char = *nextChar++;\
    if (a_char == endChar)\
    {\
      if (accum < 0)\
        okSoFar = false;\
      break;\
      \
    }\
    if ((a_char >= '0') && (a_char <= '9'))\
    {\
      if (accum < 0)\
        accum = short(a_char - '0');\
      else\
      {\
        accum *= 10;\
        accum += (a_char - '0');\
        if (accum > 255)\
          okSoFar = false;\
      }\
    }\
    else\
      okSoFar = false;\
  }\
  if (okSoFar)\
    sumPtr = accum;\
}

/*------------------------------------ checkIpString ---*/
bool checkIpString
  (PSymbol	ipAddress,
   short &  byte_0,
   short &  byte_1,
   short &  byte_2,
   short &  byte_3)
{
  bool okSoFar = true;

  if (ipAddress != gEmptySymbol)
  {
    Ptr   nextChar = ipAddress->s_name;
    short accum;
    char  a_char;

    /* Get first byte. */
    CHECK_A_BYTE('.', byte_0)
    CHECK_A_BYTE('.', byte_1)
    CHECK_A_BYTE('.', byte_2)
    CHECK_A_BYTE(0, byte_3)
  }
  return okSoFar;
} /* checkIpString */

/*------------------------------------ tcpClientCreate ---*/
Pvoid tcpClientCreate
  (PSymbol ipAddress,
   long    port,
   long    numBuffers)
{
  TcpObjectPtr xx;

  EnterCallback();
  LOG_ENTER()
  xx = static_cast<TcpObjectPtr>(newobject(gClass));
  if (xx)
  {
    bool                okSoFar = true;
    short               byte_0 = DEFAULT_IP_ADDR_0, byte_1 = DEFAULT_IP_ADDR_1;
    short               byte_2 = DEFAULT_IP_ADDR_2, byte_3 = DEFAULT_IP_ADDR_3;
#if OPEN_TRANSPORT_SUPPORTED
    OSStatus            result;
    OTConfigurationRef  this_config;
#endif /* OPEN_TRANSPORT_SUPPORTED */

#if defined(BE_VERBOSE)
    xx->fVerbose = false;
#endif /* BE_VERBOSE */
    presetObjectPointers(xx);
    if ((port < 0) || (port > MAX_PORT) || (numBuffers < 0) ||
        (! checkIpString(ipAddress, byte_0, byte_1, byte_2, byte_3)))
    {
      LOG_ERROR_1(OUTPUT_PREFIX "invalid parameters for device")
      okSoFar = false;
    }
    if (numBuffers == 0)
      numBuffers = NUM_RX_BUFFERS;
    if (okSoFar)
      okSoFar = initObject(OUR_NAME, xx, port, numBuffers);
#if OPEN_TRANSPORT_SUPPORTED
 #if defined(COMPILE_FOR_CATS)
    xx->fDataNotifier = NewOTNotifyUPP(tcpClientNotifier);
 #endif /* COMPILE_FOR_CATS */
    if (okSoFar)
    {
      xx->fAccessControl = acquireOpenTransport(OUR_NAME, static_cast<ushort>(port),
                                                false);
      if (! xx->fAccessControl)
        okSoFar = false;
    }
    /* Get the desired configuration */
    if (okSoFar)
    {
      this_config = OTCreateConfiguration(kTCPName);
      if ((this_config == kOTInvalidConfigurationPtr) ||
          (this_config == kOTNoMemoryConfigurationPtr))
      {
        LOG_ERROR_1(OUTPUT_PREFIX "unable to obtain an OT configuration")
        okSoFar = false;
      }
    }
    /* Set up the endpoint */
    if (okSoFar)
    {
      TEndpointInfo info;

 #if defined(COMPILE_FOR_CATS)
      xx->fEndpoint = OTOpenEndpointInContext(this_config, 0, &info, &result,
                                              xx->fAccessControl->fContext);
 #else /* not COMPILE_FOR_CATS */
      xx->fEndpoint = OTOpenEndpoint(this_config, 0, &info, &result);
 #endif /* not COMPILE_FOR_CATS */
      if (result == kOTNoError)
      {
        xx->fServiceType = info.servtype;
        xx->fAddressSize = short(info.addr);
      }
      else
      {
        REPORT_ERROR(OUTPUT_PREFIX "unable to obtain an endpoint (%ld = %s)", result)
        okSoFar = false;
      }
    }
    if (okSoFar)
    {
      WRAP_OT_CALL(xx, result, "OTSetAsynchronous", OTSetAsynchronous(xx->fEndpoint))
      if (result != kOTNoError)
      {
        REPORT_ERROR(OUTPUT_PREFIX "OTSetAsynchronous failed (%ld = %s)", result)
        reportEndpointState(OUR_NAME, xx);
        okSoFar = false;
      }
    }
    if (okSoFar)
    {
      WRAP_OT_CALL(xx, result, "OTSetBlocking", OTSetBlocking(xx->fEndpoint))
      if (result != kOTNoError)
      {
        REPORT_ERROR(OUTPUT_PREFIX "OTSetBlocking failed (%ld = %s)", result)
        reportEndpointState(OUR_NAME, xx);
        okSoFar = false;
      }
    }
    if (okSoFar)
    {
 #if defined(COMPILE_FOR_CATS)
      WRAP_OT_CALL(xx, result, "OTInstallNotifier", OTInstallNotifier(xx->fEndpoint,
                                                                      xx->fDataNotifier,
                                                                      xx))
 #else /* not COMPILE_FOR_CATS */
      WRAP_OT_CALL(xx, result, "OTInstallNotifier", OTInstallNotifier(xx->fEndpoint,
                                                                      tcpClientNotifier,
                                                                      xx))
 #endif /* not COMPILE_FOR_CATS */
      if (result != kOTNoError)
      {
        REPORT_ERROR(OUTPUT_PREFIX "OTInstallNotifier failed (%ld = %s)", result)
        reportEndpointState(OUR_NAME, xx);
        okSoFar = false;
      }
    }
    if (okSoFar)
      okSoFar = tcpClientSetServer(xx, byte_0, byte_1, byte_2, byte_3, false);
    if (okSoFar)
    {
      WRAP_OT_CALL(xx, result, "OTBind", OTBind(xx->fEndpoint, NULL_PTR, NULL_PTR))
      if (result != kOTNoError)
      {
        REPORT_ERROR(OUTPUT_PREFIX "OTBind failed (%ld = %s)", result)
        reportEndpointState(OUR_NAME, xx);
        okSoFar = false;
      }
    }
#endif /* OPEN_TRANSPORT_SUPPORTED */
    if (! okSoFar)
    {
      freeobject(reinterpret_cast<PObject>(xx));
      xx = NULL_PTR;
    }
  }
  ExitCodeResource();
  return xx;
} /* tcpClientCreate */

/*------------------------------------ tcpClientFree ---*/
Pvoid tcpClientFree
  (TcpObjectPtr xx)
{
  EnterCallback();
  if (xx)
  {
#if OPEN_TRANSPORT_SUPPORTED
    OSStatus result;

    xx->fClosing = true;
    if (xx->fEndpoint != kOTInvalidEndpointRef)
    {
      switch (xx->fState)
      {
        case TCP_OBJECT_CONNECTED:
          tcpClientDisconnect(xx, true);
          /* Fall through */

        case TCP_OBJECT_CONNECTING:
          /* Fall through */

        case TCP_OBJECT_BOUND:
          WRAP_OT_CALL(xx, result, "OTUnbind", OTUnbind(xx->fEndpoint))
          if (result != kOTNoError)
          {
            REPORT_ERROR(OUTPUT_PREFIX "OTUnbind failed (%ld = %s)", result)
		        reportEndpointState(OUR_NAME, xx);
          }
          /* Fall through */

        case TCP_OBJECT_UNBOUND:
          WRAP_OT_CALL(xx, result, "OTCloseProvider", OTCloseProvider(xx->fEndpoint))
          if (result != kOTNoError)
          {
            REPORT_ERROR(OUTPUT_PREFIX "OTCloseProvider failed (%ld = %s)", result)
		        reportEndpointState(OUR_NAME, xx);
          }
          break;

      }
    }
 #if defined(COMPILE_FOR_CATS)
    DisposeOTNotifyUPP(xx->fDataNotifier);
 #endif /* COMPILE_FOR_CATS */
#endif /* OPEN_TRANSPORT_SUPPORTED */
    releaseObjectMemory(OUR_NAME, xx);
#if OPEN_TRANSPORT_SUPPORTED
    relinquishOpenTransport(OUR_NAME, &xx->fAccessControl);
#endif /* OPEN_TRANSPORT_SUPPORTED */
  }
  LOG_EXIT()
  ExitMaxMessageHandler()
} /* tcpClientFree */

/*------------------------------------ tcpClientSetPort ---*/
bool tcpClientSetPort
  (TcpObjectPtr xx,
   const bool		bangOnError)
{
  bool okSoFar = true;

  if (xx)
  {
#if OPEN_TRANSPORT_SUPPORTED
 #if SYSLOG_AVAILABLE
    Syslog(SYSLOG_LEVEL, OUTPUT_PREFIX "entering tcpClientSetPort");
 #endif /* SYSLOG_AVAILABLE */
    switch (xx->fState)
    {
      case TCP_OBJECT_UNBOUND:
        break;

      case TCP_OBJECT_BOUND:
        break;

      case TCP_OBJECT_CONNECTING:
        LOG_ERROR_1(OUTPUT_PREFIX "connecting")
        okSoFar = false;
        break;

      case TCP_OBJECT_CONNECTED:
        LOG_ERROR_1(OUTPUT_PREFIX "connection is active")
        okSoFar = false;
        break;

    }
#endif /* OPEN_TRANSPORT_SUPPORTED */
    if ((! okSoFar) && bangOnError)
      signalError(xx);
#if (OPEN_TRANSPORT_SUPPORTED && SYSLOG_AVAILABLE)
    Syslog(SYSLOG_LEVEL, OUTPUT_PREFIX "exiting tcpClientSetPort");
#endif /* OPEN_TRANSPORT_SUPPORTED and SYSLOG_AVAILABLE */
  }
  return true;
} /* tcpClientSetPort */

/*------------------------------------ tcpClientSetServer ---*/
bool tcpClientSetServer
  (TcpObjectPtr xx,
   const short	byte_0,
   const short  byte_1,
   const short  byte_2,
   const short  byte_3,
   const bool   bangOnError)
{
#if (! OPEN_TRANSPORT_SUPPORTED)
 #pragma unused(byte_0, byte_1, byte_2, byte_3, bangOnError)
#endif /* not OPEN_TRANSPORT_SUPPORTED */
  bool okSoFar = true;

  if (xx)
  {
#if OPEN_TRANSPORT_SUPPORTED
 #if SYSLOG_AVAILABLE
    Syslog(SYSLOG_LEVEL, OUTPUT_PREFIX "entering tcpClientSetHost");
 #endif /* SYSLOG_AVAILABLE */
    switch (xx->fState)
    {
      case TCP_OBJECT_UNBOUND:
        break;

      case TCP_OBJECT_BOUND:
        break;

      case TCP_OBJECT_CONNECTING:
        LOG_ERROR_1(OUTPUT_PREFIX "connecting")
        okSoFar = false;
        break;

      case TCP_OBJECT_CONNECTED:
        LOG_ERROR_1(OUTPUT_PREFIX "connection is active")
        okSoFar = false;
        break;

    }
    if (okSoFar)
      xx->fServerAddress = static_cast<ulong>(((byte_0 & 0x0FF) << 24) |
                                              ((byte_1 & 0x0FF) << 16) |
                                              ((byte_2 & 0x0FF) << 8) |
                                              (byte_3 & 0x0FF));
    if ((! okSoFar) && bangOnError)
      signalError(xx);
 #if SYSLOG_AVAILABLE
    Syslog(SYSLOG_LEVEL, OUTPUT_PREFIX "exiting tcpClientSetHost");
 #endif /* SYSLOG_AVAILABLE */
#endif /* OPEN_TRANSPORT_SUPPORTED */
  }
  return okSoFar;
} /* tcpClientSetServer */

/*------------------------------------ tcpClientConnect ---*/
bool tcpClientConnect
  (TcpObjectPtr xx)
{
  bool okSoFar = true;

  if (xx)
  {
#if OPEN_TRANSPORT_SUPPORTED
 #if SYSLOG_AVAILABLE
    Syslog(SYSLOG_LEVEL, OUTPUT_PREFIX "entering tcpClientConnect");
 #endif /* SYSLOG_AVAILABLE */
    switch (xx->fState)
    {
      case TCP_OBJECT_UNBOUND:
        LOG_ERROR_1(OUTPUT_PREFIX "not bound")
        okSoFar = false;
        break;

      case TCP_OBJECT_BOUND:
        break;

      case TCP_OBJECT_CONNECTING:
        LOG_ERROR_1(OUTPUT_PREFIX "already connecting")
        okSoFar = false;
        break;

      case TCP_OBJECT_CONNECTED:
        LOG_ERROR_1(OUTPUT_PREFIX "already connected")
        okSoFar = false;
        break;

    }
    if (okSoFar)
    {
      OSStatus    result;
      TCall       snd_call;
      InetAddress in_addr;

      setObjectState(xx, TCP_OBJECT_CONNECTING);
      OTInitInetAddress(&in_addr, xx->fServerPort, xx->fServerAddress);
      OTMemzero(&snd_call, sizeof(snd_call));
      snd_call.addr.len = sizeof(in_addr);              
      snd_call.addr.buf = reinterpret_cast<Puchar>(&in_addr);
      WRAP_OT_CALL(xx, result, "OTConnect", OTConnect(xx->fEndpoint, &snd_call, NULL_PTR))
      if (result != kOTNoDataErr)
      {
        REPORT_ERROR(OUTPUT_PREFIX "OTConnect failed (%ld = %s)", result)
        reportEndpointState(OUR_NAME, xx);
        okSoFar = false;
      }
    }
    if (! okSoFar)
      signalError(xx);
 #if SYSLOG_AVAILABLE
    Syslog(SYSLOG_LEVEL, OUTPUT_PREFIX "exiting tcpClientConnect");
 #endif /* SYSLOG_AVAILABLE */
#endif /* OPEN_TRANSPORT_SUPPORTED */
  }
  return okSoFar;
} /* tcpClientConnect */

/*------------------------------------ tcpClientDisconnect ---*/
bool tcpClientDisconnect
  (TcpObjectPtr xx,
   const bool		forced)
{
#if (! OPEN_TRANSPORT_SUPPORTED)
 #pragma unused(forced)
#endif /* not OPEN_TRANSPORT_SUPPORTED */
  bool okSoFar = true;

  if (xx)
  {
#if OPEN_TRANSPORT_SUPPORTED
 #if SYSLOG_AVAILABLE
    Syslog(SYSLOG_LEVEL, OUTPUT_PREFIX "entering tcpClientDisconnect");
 #endif /* SYSLOG_AVAILABLE */
    switch (xx->fState)
    {
      case TCP_OBJECT_UNBOUND:
        LOG_ERROR_1(OUTPUT_PREFIX "not bound")
        okSoFar = false;
        break;

      case TCP_OBJECT_BOUND:
        LOG_ERROR_1(OUTPUT_PREFIX "not connected")
        okSoFar = false;
        break;

      case TCP_OBJECT_CONNECTING:
        LOG_ERROR_1(OUTPUT_PREFIX "connecting")
        okSoFar = false;
        break;

      case TCP_OBJECT_CONNECTED:
        break;

    }
    if (okSoFar || forced)
    {
      OSStatus result;

      if ((! forced) && ((xx->fServiceType == T_COTS_ORD) ||
          (xx->fServiceType == T_TRANS_ORD)))
      {
        WRAP_OT_CALL(xx, result, "OTSndOrderlyDisconnect",
                      OTSndOrderlyDisconnect(xx->fEndpoint))
        if (result == kOTNoDataErr)
          setObjectState(xx, TCP_OBJECT_DISCONNECTING);
        else
        {
          REPORT_ERROR(OUTPUT_PREFIX "OTSndOrderlyDisconnect failed (%ld = %s)", result)
	        reportEndpointState(OUR_NAME, xx);
          okSoFar = false;
        }
      }
      else
      {
        TCall       snd_call;
        InetAddress in_addr;

        OTInitInetAddress(&in_addr, xx->fServerPort, xx->fServerAddress);
        OTMemzero(&snd_call, sizeof(snd_call));
        snd_call.addr.len = sizeof(in_addr);              
        snd_call.addr.buf = reinterpret_cast<Puchar>(&in_addr);
        WRAP_OT_CALL(xx, result, "OTSndDisconnect", OTSndDisconnect(xx->fEndpoint,
                                                                    &snd_call))
        if (result == kOTNoError)
          setObjectState(xx, TCP_OBJECT_BOUND);
        else
        {
          REPORT_ERROR(OUTPUT_PREFIX "OTSndDisconnect failed (%ld = %s)", result)
	        reportEndpointState(OUR_NAME, xx);
          okSoFar = false;
        }
      }
    }
    if (! okSoFar)
      signalError(xx);
 #if SYSLOG_AVAILABLE
    Syslog(SYSLOG_LEVEL, OUTPUT_PREFIX "exiting tcpClientDisconnect");
 #endif /* SYSLOG_AVAILABLE */
#endif /* OPEN_TRANSPORT_SUPPORTED */
  }
  return okSoFar;
} /* tcpClientDisconnect */

StandardInfoRoutine(TcpObjectPtr)