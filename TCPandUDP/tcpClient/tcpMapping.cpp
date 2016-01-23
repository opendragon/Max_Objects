/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       tcpMapping.cpp                                                          */
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

#if 0
/*------------------------------------ describeEndpointState ---*/
const char *
describeEndpointState(EndpointRef anEndpoint)
{
    const char * result = "Invalid";

    if (kOTInvalidEndpointRef != anEndpoint)
    {
        switch (OTGetEndpointState(anEndpoint))
        {
            case T_UNINIT:
                result = "Uninitialized";
                break;

            case T_UNBND:
                result = "Unbound";
                break;

            case T_IDLE:
                result = "Idle";
                break;

            case T_OUTCON:
                result = "Outgoing connection request";
                break;

            case T_INCON:
                result = "Incoming connection request";
                break;

            case T_DATAXFER:
                result = "Data transfer";
                break;

            case T_OUTREL:
                result = "Outgoing release request";
                break;

            case T_INREL:
                result = "Incoming release request";
                break;

            default:
                result = "Unknown";
                break;
        }
    }
    return result;
} // describeEndpointState
#endif//0

#if 0
/*------------------------------------ mapEventToString ---*/
const char *
mapEventToString(const OTEventCode code)
{
    const char * result;

    if (IsOTPrivateEvent(code))
    {
        result = "Unknown private event";
    }
    else if (IsOTCompleteEvent(code))
    {
        switch (code)
        {
            case T_BINDCOMPLETE:
                result = "T_BINDCOMPLETE";
                break;

            case T_UNBINDCOMPLETE:
                result = "T_UNBINDCOMPLETE";
                break;

            case T_ACCEPTCOMPLETE:
                result = "T_ACCEPTCOMPLETE";
                break;

            case T_REPLYCOMPLETE:
                result = "T_REPLYCOMPLETE";
                break;

            case T_DISCONNECTCOMPLETE:
                result = "T_DISCONNECTCOMPLETE";
                break;

            case T_OPTMGMTCOMPLETE:
                result = "T_OPTMGMTCOMPLETE";
                break;

            case T_OPENCOMPLETE:
                result = "T_OPENCOMPLETE";
                break;

            case T_GETPROTADDRCOMPLETE:
                result = "T_GETPROTADDRCOMPLETE";
                break;

            case T_RESOLVEADDRCOMPLETE:
                result = "T_RESOLVEADDRCOMPLETE";
                break;

            case T_GETINFOCOMPLETE:
                result = "T_GETINFOCOMPLETE";
                break;

            case T_SYNCCOMPLETE:
                result = "T_SYNCCOMPLETE";
                break;

            case T_MEMORYRELEASED:
                result = "T_MEMORYRELEASED";
                break;

            case T_REGNAMECOMPLETE:
                result = "T_REGNAMECOMPLETE";
                break;

            case T_DELNAMECOMPLETE:
                result = "T_DELNAMECOMPLETE";
                break;

            case T_LKUPNAMECOMPLETE:
                result = "T_LKUPNAMECOMPLETE";
                break;

            case T_LKUPNAMERESULT:
                result = "T_LKUPNAMERESULT";
                break;

            case kOTSyncIdleEvent:
                result = "kOTSyncIdleEvent";
                break;

            default:
                result = "Unknown completion event";
                break;
        }
    }
    else if (IsOTProtocolEvent(code))
    {
        switch (code)
        {
            case kOTProviderIsDisconnected:
                result = "kOTProviderIsDisconnected";
                break;

            case kOTProviderIsReconnected:
                result = "kOTProviderIsReconnected";
                break;

            default:
                result = "Unknown protocol event";
                break;
        }
    }
    else if (IsOTSignalEvent(code))
    {
        result = "Unknown signal event";
    }
    else if (IsOTStreamEvent(code))
    {
        switch (code)
        {
            case kGetmsgEvent:
                result = "kGetmsgEvent";
                break;

            case kStreamReadEvent:
                result = "kStreamReadEvent";
                break;

            case kStreamWriteEvent:
                result = "kStreamWriteEvent";
                break;

            case kStreamIoctlEvent:
                result = "kStreamIoctlEvent";
                break;

            case kStreamOpenEvent:
                result = "kStreamOpenEvent";
                break;

            case kPollEvent:
                result = "kPollEvent";
                break;

            default:
                result = "Unknown stream event";
                break;
        }
    }
    else
    {
        switch (code)
        {
            case T_LISTEN:
                result = "T_LISTEN";
                break;

            case T_CONNECT:
                result = "T_CONNECT";
                break;

            case T_DATA:
                result = "T_DATA";
                break;

            case T_EXDATA:
                result = "T_EXDATA";
                break;

            case T_DISCONNECT:
                result = "T_DISCONNECT";
                break;

            case T_ERROR:
                result = "T_ERROR";
                break;

            case T_UDERR:
                result = "T_UDERR";
                break;

            case T_ORDREL:
                result = "T_ORDREL";
                break;

            case T_GODATA:
                result = "T_GODATA";
                break;

            case T_GOEXDATA:
                result = "T_GOEXDATA";
                break;

            case T_REQUEST:
                result = "T_REQUEST";
                break;
                
            case T_REPLY:
                result = "T_REPLY";
                break;
                
            case T_PASSCON:
                result = "T_PASSCON";
                break;
                
            case T_RESET:
                result = "T_RESET";
                break;
                
            default:
                result = "Unknown or miscellaneous event";
                break;
        }
    }
    return result;
} // mapEventToString
#endif//0

/*------------------------------------ mapStateToSymbol ---*/
t_symbol *
mapStateToSymbol(const TcpState aState)
{
    t_symbol * result;

    switch (aState)
    {
        case kTcpStateUnbound:
            result = gUnboundSymbol;
            break;

        case kTcpStateBound:
            result = gBoundSymbol;
            break;

        case kTcpStateListening:
            result = gListeningSymbol;
            break;

        case kTcpStateConnecting:
            result = gConnectingSymbol;
            break;

        case kTcpStateConnected:
            result = gConnectedSymbol;
            break;

        case kTcpStateDisconnecting:
            result = gDisconnectingSymbol;
            break;

        default:
            result = gUnknownSymbol;
            break;
            
    }
    return result;
} // mapStateToSymbol

/*------------------------------------ setUpStateSymbols ---*/
void
setUpStateSymbols(void)
{
    gBoundSymbol = gensym("bound");
    gConnectedSymbol = gensym("connected");
    gConnectingSymbol = gensym("connecting");
    gDisconnectingSymbol = gensym("disconnecting");
    gListeningSymbol = gensym("listening");
    gUnboundSymbol = gensym("unbound");
    gUnknownSymbol = gensym("unknown");
} // setUpStateSymbols
