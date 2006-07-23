/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       udpPort.c                                                               */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the udpPort module.       */
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

#define IS_MAIN /* */
#include "udpPort.h"
#include "displayAboutBox.h"
#include "loadAboutBoxData.h"
#include "loadOtherSegments.h"
#include "reportVersion.h"

/* Forward references: */
Pvoid udpPortCreate
  (long port,
   long numBuffers);

Pvoid udpPortFree
  (UdpObjectPtr xx);

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
          reinterpret_cast<method>(udpPortCreate),
          reinterpret_cast<method>(udpPortFree), short(sizeof(UdpObjectData)),
          reinterpret_cast<method>(0L), A_DEFLONG, A_DEFLONG, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Anything), "anything", A_GIMME, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Assist), "assist", A_CANT, A_NOTHING);
    addbang(reinterpret_cast<method>(cmd_Status));
    addfloat(reinterpret_cast<method>(cmd_Float));
    addmess(reinterpret_cast<method>(cmd_Info), "info", A_NOTHING);
    addint(reinterpret_cast<method>(cmd_Int));
    addmess(reinterpret_cast<method>(cmd_List), "list", A_GIMME, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Mode), "mode", A_SYM, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Port), "port", A_LONG, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Self), "self", A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Send), "send", A_GIMME, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_SendTo), "sendTo", A_SYM, A_LONG, A_NOTHING);
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
    gFromSymbol = gensym("from");
    gMaxSymbol = gensym("max");
    gOffSymbol = gensym("off");
    gOnSymbol = gensym("on");
    gRawSymbol = gensym("raw");
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

/*------------------------------------ describeEndpointState ---*/
Ptr describeEndpointState
  (EndpointRef anEndpoint)
{
#if (! OPEN_TRANSPORT_SUPPORTED)
 #pragma unused(anEndpoint)
#endif /* not OPEN_TRANSPORT_SUPPORTED */
#if OPEN_TRANSPORT_SUPPORTED
  if (anEndpoint != kOTInvalidEndpointRef)
  {
    OTResult result = OTGetEndpointState(anEndpoint);

    switch (result)
    {
      case T_UNINIT:
        return "Uninitialized";

      case T_UNBND:
        return "Unbound";

      case T_IDLE:
        return "Idle";

      case T_OUTCON:
        return "Outgoing connection request";

      case T_INCON:
        return "Incoming connection request";

      case T_DATAXFER:
        return "Data transfer";

      case T_OUTREL:
        return "Outgoing release request";

      case T_INREL:
        return "Incoming release request";

      default:
        return "Unknown";

    }
  }
#endif /* OPEN_TRANSPORT_SUPPORTED */
  return "Invalid";
} /* describeEndpointState */

/*------------------------------------ processErrorQueue ---*/
static Pvoid processErrorQueue
  (UdpObjectPtr xx)
{
  EnterCallback();
  if (xx)
  {
    short prev_lock = lockout_set(1);

    outlet_bang(xx->fErrorBangOut);
    lockout_set(prev_lock);
    evnum_incr();
  }
  ExitMaxMessageHandler()
} /* processErrorQueue */

/*------------------------------------ initObject ---*/
bool initObject
  (UdpObjectPtr xx,
   const long   port,
   const long   numBuffers)
{
  bool okSoFar = true;

  if (xx)
  {
#if (OPEN_TRANSPORT_SUPPORTED && SYSLOG_AVAILABLE)
    Syslog(SYSLOG_LEVEL, OUTPUT_PREFIX "entering initObject");
#endif /* OPEN_TRANSPORT_SUPPORTED and SYSLOG_AVAILABLE */
    xx->fSelfPort = static_cast<ushort>(port ? port : DEFAULT_PORT);
    xx->fPartnerAddress = 0L;
    xx->fPartnerPort = 0;
    xx->fPartnerKnown = false;
    xx->fErrorBangOut = static_cast<POutlet>(bangout(xx));
    xx->fResultOut = static_cast<POutlet>(outlet_new(xx, NULL_PTR));
    setObjectState(xx, TCP_OBJECT_UNBOUND);
    xx->fEndpoint = kOTInvalidEndpointRef;
    xx->fErrorQueue = static_cast<PQelem>(qelem_new(xx,
                                            reinterpret_cast<method>(processErrorQueue)));
    xx->fRebindQueue = static_cast<PQelem>(qelem_new(xx,
                                            reinterpret_cast<method>(processRebindQueue)));
    xx->fReceiveQueue = static_cast<PQelem>(qelem_new(xx,
                                          reinterpret_cast<method>(processReceiveQueue)));
    xx->fBufferBase = reinterpret_cast<DataBufferHdl>(newhandle(long(BUFF_MEMORY_TO_ALLOC *
                                                                (numBuffers + 2))));
    if (xx->fBufferBase)
    {
      HLock(reinterpret_cast<Handle>(xx->fBufferBase));
      xx->fSendBuffer = *xx->fBufferBase;
      xx->fReceiveBuffer = reinterpret_cast<DataBufferPtr>(ADD_TO_ADDRESS(xx->fSendBuffer,
                                                                    BUFF_MEMORY_TO_ALLOC));
    }
    xx->fLinkBase = reinterpret_cast<UdpBufferLinkHdl>(newhandle(long(sizeof(UdpBufferLinkData) *
                                                                      numBuffers)));
    if (xx->fLinkBase)
    {
      DataBufferPtr    this_buffer =
                              reinterpret_cast<DataBufferPtr>(ADD_TO_ADDRESS(xx->fReceiveBuffer,
                                                                    BUFF_MEMORY_TO_ALLOC));
      UdpBufferLinkPtr prev_link = NULL_PTR, this_link = NULL_PTR;

      HLock(reinterpret_cast<Handle>(xx->fLinkBase));
      xx->fPoolHead = *xx->fLinkBase;
      this_link = xx->fPoolHead;
      for (long link_count = 0; link_count < numBuffers; ++link_count)
      {
        this_link->fPrevious = prev_link;
        this_link->fData = this_buffer;
        this_buffer = reinterpret_cast<DataBufferPtr>(ADD_TO_ADDRESS(this_buffer,
                                                                BUFF_MEMORY_TO_ALLOC));
        this_link->fNext = NULL_PTR;
        if (prev_link)
          prev_link->fNext = this_link;
        prev_link = this_link;
        this_link = reinterpret_cast<UdpBufferLinkPtr>(ADD_TO_ADDRESS(this_link,
                                                              sizeof(UdpBufferLinkData)));
      }
      xx->fPoolTail = prev_link;           
    }
    xx->fClosing = xx->fRawMode = false;
    if (! (xx->fResultOut && xx->fErrorBangOut && xx->fErrorQueue && xx->fBufferBase &&
            xx->fRebindQueue && xx->fReceiveQueue && xx->fLinkBase))
    {
      LOG_ERROR_1(OUTPUT_PREFIX "unable to create port or buffer for object")
      okSoFar = false;
    }
  }
#if (OPEN_TRANSPORT_SUPPORTED && SYSLOG_AVAILABLE)
  Syslog(SYSLOG_LEVEL, OUTPUT_PREFIX "exiting initObject");
#endif /* OPEN_TRANSPORT_SUPPORTED and SYSLOG_AVAILABLE */
  return okSoFar;
} /* initObject */

/*------------------------------------ mapEventToString ---*/
Ptr mapEventToString
  (const OTEventCode	code)
{
  if (IsOTPrivateEvent(code))
    return "Unknown private event";

  if (IsOTCompleteEvent(code))
  {
    switch (code)
    {
      case T_BINDCOMPLETE:
        return "T_BINDCOMPLETE";

    	case T_UNBINDCOMPLETE:
        return "T_UNBINDCOMPLETE";

      case T_ACCEPTCOMPLETE:
        return "T_ACCEPTCOMPLETE";

      case T_REPLYCOMPLETE:
        return "T_REPLYCOMPLETE";

      case T_DISCONNECTCOMPLETE:
        return "T_DISCONNECTCOMPLETE";

      case T_OPTMGMTCOMPLETE:
        return "T_OPTMGMTCOMPLETE";

      case T_OPENCOMPLETE:
        return "T_OPENCOMPLETE";

      case T_GETPROTADDRCOMPLETE:
        return "T_GETPROTADDRCOMPLETE";

      case T_RESOLVEADDRCOMPLETE:
        return "T_RESOLVEADDRCOMPLETE";

      case T_GETINFOCOMPLETE:
        return "T_GETINFOCOMPLETE";

      case T_SYNCCOMPLETE:
        return "T_SYNCCOMPLETE";

      case T_MEMORYRELEASED:
        return "T_MEMORYRELEASED";

      case T_REGNAMECOMPLETE:
        return "T_REGNAMECOMPLETE";

      case T_DELNAMECOMPLETE:
        return "T_DELNAMECOMPLETE";

      case T_LKUPNAMECOMPLETE:
        return "T_LKUPNAMECOMPLETE";

      case T_LKUPNAMERESULT:
        return "T_LKUPNAMERESULT";

      case kOTSyncIdleEvent:
        return "kOTSyncIdleEvent";

      default:
        return "Unknown completion event";

    }
  }   
  if (IsOTProtocolEvent(code))
  {
    switch (code)
    {
      case kOTProviderIsDisconnected:
        return "kOTProviderIsDisconnected";

      case kOTProviderIsReconnected:
        return "kOTProviderIsReconnected";

      default:
        return "Unknown protocol event";

    }
  }
  if (IsOTSignalEvent(code))
    return "Unknown signal event";

  if (IsOTStreamEvent(code))
  {
    switch (code)
    {
      case kGetmsgEvent:
        return "kGetmsgEvent";

      case kStreamReadEvent:
        return "kStreamReadEvent";

      case kStreamWriteEvent:
        return "kStreamWriteEvent";

      case kStreamIoctlEvent:
        return "kStreamIoctlEvent";

      case kStreamOpenEvent:
        return "kStreamOpenEvent";

      case kPollEvent:
        return "kPollEvent";

      default:
        return "Unknown stream event";

    }
  }
  switch (code)
  {
    case T_LISTEN:
      return "T_LISTEN";

    case T_CONNECT:
      return "T_CONNECT";

    case T_DATA:
      return "T_DATA";

    case T_EXDATA:
      return "T_EXDATA";

    case T_DISCONNECT:
      return "T_DISCONNECT";

    case T_ERROR:
      return "T_ERROR";

    case T_UDERR:
      return "T_UDERR";

    case T_ORDREL:
      return "T_ORDREL";

    case T_GODATA:
      return "T_GODATA";

    case T_GOEXDATA:
      return "T_GOEXDATA";

    case T_REQUEST:
      return "T_REQUEST";

    case T_REPLY:
      return "T_REPLY";

    case T_PASSCON:
      return "T_PASSCON";

    case T_RESET:
      return "T_RESET";

    default:
      return "Unknown or miscellaneous event";

  }
} /* mapEventToString */

/*------------------------------------ mapStateToSymbol ---*/
PSymbol mapStateToSymbol
  (const UdpObjectState	aState)
{
  switch (aState)
  {
    case TCP_OBJECT_UNBOUND:
      return gUnboundSymbol;

    case TCP_OBJECT_BOUND:
      return gBoundSymbol;

    default:
      return gUnknownSymbol;

  }
} /* mapStateToSymbol */

/*------------------------------------ presetObjectPointers ---*/
void presetObjectPointers
  (UdpObjectPtr xx)
{
  if (xx)
  {
    xx->fErrorBangOut = xx->fResultOut = NULL_PTR;
    xx->fErrorQueue = xx->fRebindQueue = xx->fReceiveQueue = NULL_PTR;
    xx->fSendBuffer = xx->fReceiveBuffer = NULL_PTR;
    xx->fAccessControl = NULL_PTR;
    xx->fBufferBase = NULL_HDL;
    xx->fLinkBase = NULL_HDL;
    xx->fEndpoint = kOTInvalidEndpointRef;
    xx->fSelfName = NULL_PTR;
    xx->fReceiveHead = xx->fReceiveTail = xx->fPoolHead = xx->fPoolTail = NULL_PTR;
  }
} /* presetObjectPointers */

/*------------------------------------ releaseObjectMemory ---*/
void releaseObjectMemory
  (UdpObjectPtr xx)
{
  if (xx)
  {
#if (OPEN_TRANSPORT_SUPPORTED && SYSLOG_AVAILABLE)
    Syslog(SYSLOG_LEVEL, OUTPUT_PREFIX "entering releaseObjectMemory");
#endif /* OPEN_TRANSPORT_SUPPORTED and SYSLOG_AVAILABLE */
    if (xx->fErrorQueue)
    {
      qelem_unset(xx->fErrorQueue);
      qelem_free(xx->fErrorQueue);
      xx->fErrorQueue = NULL_PTR;
    }
    if (xx->fReceiveQueue)
    {
      qelem_unset(xx->fReceiveQueue);
      qelem_free(xx->fReceiveQueue);
      xx->fReceiveQueue = NULL_PTR;
    }
    if (xx->fRebindQueue)
    {
      qelem_unset(xx->fRebindQueue);
      qelem_free(xx->fRebindQueue);
      xx->fRebindQueue = NULL_PTR;
    }
    xx->fReceiveBuffer = xx->fSendBuffer = NULL_PTR;
    if (xx->fBufferBase)
    {
      HUnlock(reinterpret_cast<Handle>(xx->fBufferBase));
      disposhandle(reinterpret_cast<Handle>(xx->fBufferBase));
      xx->fBufferBase = NULL_HDL;
    }
    xx->fReceiveHead = xx->fReceiveTail = xx->fPoolHead = xx->fPoolTail = NULL_PTR;
    if (xx->fLinkBase)
    {
      HUnlock(reinterpret_cast<Handle>(xx->fLinkBase));
      disposhandle(reinterpret_cast<Handle>(xx->fLinkBase));
      xx->fLinkBase = NULL_HDL;
    }
#if (OPEN_TRANSPORT_SUPPORTED && SYSLOG_AVAILABLE)
    Syslog(SYSLOG_LEVEL, OUTPUT_PREFIX "exiting releaseObjectMemory");
#endif /* OPEN_TRANSPORT_SUPPORTED and SYSLOG_AVAILABLE */
  }
} /* releaseObjectMemory */

/*------------------------------------ reportEndpointState ---*/
void reportEndpointState
  (UdpObjectPtr	xx)
{
  if (xx)
  	LOG_ERROR_2(OUTPUT_PREFIX "Endpoint state: %s", describeEndpointState(xx->fEndpoint));
} /* reportEndpointState */

/*------------------------------------ setUpStateSymbols ---*/
void setUpStateSymbols
  (void)
{
  gBoundSymbol = gensym("bound");
  gUnboundSymbol = gensym("unbound");
  gUnknownSymbol = gensym("unknown");
} /* setUpStateSymbols */

/*------------------------------------ udpPortCreate ---*/
Pvoid udpPortCreate
  (long port,
   long numBuffers)
{
  UdpObjectPtr xx;

  EnterCallback();
  LOG_ENTER()
  xx = static_cast<UdpObjectPtr>(newobject(gClass));
  if (xx)
  {
    bool                okSoFar = true;
 #if OPEN_TRANSPORT_SUPPORTED
    OSStatus            result;
    OTConfigurationRef  this_config;
#endif /* OPEN_TRANSPORT_SUPPORTED */

#if defined(BE_VERBOSE)
    xx->fVerbose = false;
#endif /* BE_VERBOSE */
    presetObjectPointers(xx);
    if ((port < 0) || (port > MAX_PORT) || (numBuffers < 0))
    {
      LOG_ERROR_1(OUTPUT_PREFIX "invalid parameters for device")
      okSoFar = false;
    }
    if (numBuffers == 0)
      numBuffers = NUM_RX_BUFFERS;
    if (okSoFar)
      okSoFar = initObject(xx, port, numBuffers);
#if OPEN_TRANSPORT_SUPPORTED
 #if defined(COMPILE_FOR_OSX_4)
    xx->fDataNotifier = NewOTNotifyUPP(udpPortNotifier);
 #endif /* COMPILE_FOR_OSX_4 */
    if (okSoFar)
    {
      xx->fAccessControl = acquireOpenTransport(OUR_NAME, static_cast<ushort>(port),
                                                true); /* act as a server - we own this port! */
      if (! xx->fAccessControl)
        okSoFar = false;
    }
    /* Get the desired configuration */
    if (okSoFar)
    {
      this_config = OTCreateConfiguration(kUDPName);
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

 #if defined(COMPILE_FOR_OSX_4)
      xx->fEndpoint = OTOpenEndpointInContext(this_config, 0, &info, &result,
                                              xx->fAccessControl->fContext);
 #endif /* COMPILE_FOR_OSX_4 */
 #if defined(COMPILE_FOR_OS9_4)
      xx->fEndpoint = OTOpenEndpoint(this_config, 0, &info, &result);
 #endif /* COMPILE_FOR_OS9_4 */
      if (result == kOTNoError)
      {
        xx->fServiceType = info.servtype;
        xx->fAddressSize = short(info.addr);
        if ((info.tsdu > 0) && (info.tsdu < MAX_BUFFER_TO_SEND))
        	xx->fTransferSize = short(info.tsdu);
        else
        	xx->fTransferSize = MAX_BUFFER_TO_SEND;
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
        reportEndpointState(xx);
        okSoFar = false;
      }
    }
    if (okSoFar)
    {
      WRAP_OT_CALL(xx, result, "OTSetBlocking", OTSetBlocking(xx->fEndpoint))
      if (result != kOTNoError)
      {
        REPORT_ERROR(OUTPUT_PREFIX "OTSetBlocking failed (%ld = %s)", result)
        reportEndpointState(xx);
        okSoFar = false;
      }
    }
    if (okSoFar)
    {
 #if defined(COMPILE_FOR_OSX_4)
      WRAP_OT_CALL(xx, result, "OTInstallNotifier", OTInstallNotifier(xx->fEndpoint,
                                                                      xx->fDataNotifier,
                                                                      xx))
 #endif /* COMPILE_FOR_OSX_4 */
 #if defined(COMPILE_FOR_OS9_4)
      WRAP_OT_CALL(xx, result, "OTInstallNotifier", OTInstallNotifier(xx->fEndpoint,
                                                                      udpPortNotifier,
                                                                      xx))
 #endif /* COMPILE_FOR_OS9_4 */
      if (result != kOTNoError)
      {
        REPORT_ERROR(OUTPUT_PREFIX "OTInstallNotifier failed (%ld = %s)", result)
        reportEndpointState(xx);
        okSoFar = false;
      }
    }
    if (okSoFar)
    	udpPortSetPort(xx, false);
#endif /* OPEN_TRANSPORT_SUPPORTED */
    if (! okSoFar)
    {
      freeobject(reinterpret_cast<PObject>(xx));
      xx = NULL_PTR;
    }
  }
  ExitCodeResource();
  return xx;
} /* udpPortCreate */

/*------------------------------------ udpPortFree ---*/
Pvoid udpPortFree
  (UdpObjectPtr xx)
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
        case TCP_OBJECT_BOUND:
          WRAP_OT_CALL(xx, result, "OTUnbind", OTUnbind(xx->fEndpoint))
          if (result != kOTNoError)
          {
            REPORT_ERROR(OUTPUT_PREFIX "OTUnbind failed (%ld = %s)", result)
		        reportEndpointState(xx);
          }
          /* Fall through */

        case TCP_OBJECT_UNBOUND:
          WRAP_OT_CALL(xx, result, "OTCloseProvider", OTCloseProvider(xx->fEndpoint))
          if (result != kOTNoError)
          {
            REPORT_ERROR(OUTPUT_PREFIX "OTCloseProvider failed (%ld = %s)", result)
		        reportEndpointState(xx);
          }
          break;

      }
    }
 #if defined(COMPILE_FOR_OSX_4)
    DisposeOTNotifyUPP(xx->fDataNotifier);
 #endif /* COMPILE_FOR_OSX_4 */
#endif /* OPEN_TRANSPORT_SUPPORTED */
    releaseObjectMemory(xx);
#if OPEN_TRANSPORT_SUPPORTED
    relinquishOpenTransport(OUR_NAME, &xx->fAccessControl);
#endif /* OPEN_TRANSPORT_SUPPORTED */
  }
  LOG_EXIT()
  ExitMaxMessageHandler()
} /* udpPortFree */

/*------------------------------------ udpPortSetPort ---*/
bool udpPortSetPort
  (UdpObjectPtr xx,
   const bool		bangOnError)
{
  bool okSoFar = true;

  if (xx)
  {
#if OPEN_TRANSPORT_SUPPORTED
    OSStatus result;

 #if SYSLOG_AVAILABLE
    Syslog(SYSLOG_LEVEL, OUTPUT_PREFIX "entering udpPortSetPort");
 #endif /* SYSLOG_AVAILABLE */
    switch (xx->fState)
    {
      case TCP_OBJECT_BOUND:
        WRAP_OT_CALL(xx, result, "OTUnbind", OTUnbind(xx->fEndpoint))
        if (result != kOTNoError)
        {
          REPORT_ERROR(OUTPUT_PREFIX "OTUnbind failed (%ld = %s)", result)
		      reportEndpointState(xx);
          okSoFar = false;
        }
        setObjectState(xx, TCP_OBJECT_UNBOUND);
        /* Fall through */

      case TCP_OBJECT_UNBOUND:
        break;

    }
    if (okSoFar)
    {
      TBind       bind_request;
      InetAddress in_address;

      OTInitInetAddress(&in_address, xx->fSelfPort, xx->fSelfAddress);
      bind_request.addr.len = sizeof(in_address);
      bind_request.addr.buf = reinterpret_cast<Puchar>(&in_address);
      bind_request.qlen = 1;
      WRAP_OT_CALL(xx, result, "OTBind", OTBind(xx->fEndpoint, &bind_request, NULL_PTR))
      if (result != kOTNoError)
      {
        REPORT_ERROR(OUTPUT_PREFIX "OTBind failed (%ld = %s)", result)
        reportEndpointState(xx);
        okSoFar = false;
      }
    }
#endif /* OPEN_TRANSPORT_SUPPORTED */
    if ((! okSoFar) && bangOnError)
      signalError(xx);
#if (OPEN_TRANSPORT_SUPPORTED && SYSLOG_AVAILABLE)
    Syslog(SYSLOG_LEVEL, OUTPUT_PREFIX "exiting udpPortSetPort");
#endif /* OPEN_TRANSPORT_SUPPORTED and SYSLOG_AVAILABLE */
  }
  return true;
} /* udpPortSetPort */

StandardInfoRoutine(UdpObjectPtr)