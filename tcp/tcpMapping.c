/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       tcpMapping.c                                                            */
/*                                                                                      */
/*  Contains:   The common utility routines.                                            */
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
/*  Created:    2000/10/18                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "tcpMapping.h"

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
  (const TcpObjectState	aState)
{
  switch (aState)
  {
    case TCP_OBJECT_UNBOUND:
      return gUnboundSymbol;

    case TCP_OBJECT_BOUND:
      return gBoundSymbol;

    case TCP_OBJECT_LISTENING:
      return gListeningSymbol;

    case TCP_OBJECT_CONNECTING:
      return gConnectingSymbol;

    case TCP_OBJECT_CONNECTED:
      return gConnectedSymbol;

    case TCP_OBJECT_DISCONNECTING:
      return gDisconnectingSymbol;

    default:
      return gUnknownSymbol;

  }
} /* mapStateToSymbol */

/*------------------------------------ setUpStateSymbols ---*/
void setUpStateSymbols
  (void)
{
  gBoundSymbol = gensym("bound");
  gConnectedSymbol = gensym("connected");
  gConnectingSymbol = gensym("connecting");
  gDisconnectingSymbol = gensym("disconnecting");
  gListeningSymbol = gensym("listening");
  gUnboundSymbol = gensym("unbound");
  gUnknownSymbol = gensym("unknown");
} /* setUpStateSymbols */