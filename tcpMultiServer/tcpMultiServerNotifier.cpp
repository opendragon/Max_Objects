/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       tcpMultiServerNotifier.cpp                                              */
/*                                                                                      */
/*  Contains:   The routine tcpServerNotifier().                                        */
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

/*------------------------------------ processListenEvent ---*/
static void processListenEvent(TcpMultiServerData * xx)
{
    if (xx)
    {
        TCall               call;
        InetAddress         caddr;
        OTResult            look_result;
        // OTLink *         acceptor_link;
        // EPInfo *         acceptor;
        OSStatus            result;
        TcpConnectionData * candidate = NULL_PTR;
        bool                okSoFar = false;

        call.addr.maxlen = sizeof(caddr);
        call.addr.buf = reinterpret_cast<unsigned char *>(&caddr);
        call.opt.maxlen = 0;
        call.opt.buf = NULL_PTR;
        call.udata.maxlen = 0;
        call.udata.buf = NULL_PTR;
        WRAP_OT_CALL(xx, result, "OTListen", OTListen(xx->fListenEndpoint, &call))
        if (result != kOTNoDataErr)
        {
            if (kOTNoError == result)
            {
                // Find an available connection:
                if (xx->fActiveConnections < xx->fMaximumConnections)
                {
                    for (unsigned short ii = 0; ii < xx->fMaximumConnections; ++ii)
                    {
                        candidate = *(xx->fConnections + ii);
                        if (candidate && (candidate->fDataEndpoint != kOTInvalidEndpointRef) && (! candidate->fActive))
                        {
                            okSoFar = true;
                            break;
                        }
                    }
                }
                if (okSoFar)
                {
                    WRAP_OT_CALL(xx, result, "OTAccept",
                                 OTAccept(xx->fListenEndpoint, candidate->fDataEndpoint, &call))
                    if (result != kOTNoError)
                    {
                        look_result = OTLook(xx->fListenEndpoint);
                        if ((kOTLookErr == result) && (T_DISCONNECT == look_result))
                        {
                            //          DoRcvDisconnect(xx);
                        }
                        else
                        {
                            //          DBAlert2("Notifier: T_LISTEN - OTAccept error %d look_result %x",
                            //                      result, look_result);
                        }
                    }
                }
                if (okSoFar)
                {
                    //
                }
                else
                {
                    // We can't handle this connection!
                    TCall       snd_call;
                    InetAddress in_addr;

                    OTInitInetAddress(&in_addr, xx->fServerPort, xx->fServerAddress);
                    OTMemzero(&snd_call, sizeof(snd_call));
                    snd_call.addr.len = sizeof(in_addr);
                    snd_call.addr.buf = reinterpret_cast<unsigned char *>(&in_addr);
                    WRAP_OT_CALL(xx, result, "OTSndDisconnect", OTSndDisconnect(xx->fListenEndpoint,
                                                                                &snd_call))
                    if (kOTNoError == result)
                    {
                        setObjectState(xx, kTcpStateBound);
                    }
                    else
                    {
                        REPORT_ERROR(xx, OUTPUT_PREFIX "OTSndDisconnect failed (%ld = %s)", result)
                        reportEndpointState(xx, xx->fListenEndpoint);
                    }
                }
            }
            else
            {
                look_result = OTLook(xx->fListenEndpoint);
                if ((kOTLookErr == result) && (T_DISCONNECT == look_result))
                {
                    //          DoRcvDisconnect(xx);
                }
                else
                {
                    //          DBAlert2("Notifier: T_LISTEN - OTListen error %d look_result %x",
                    //                  result, look_result);
                }
            }
        }
    }
} // processListenEvent
/*------------------------------------ tcpMultiServerDataNotifier ---*/
pascal void tcpMultiServerDataNotifier(void *      context,
                                       OTEventCode code,
                                       OTResult    result,
                                       void *      cookie)
{
#pragma unused(result, cookie)
    TcpConnectionData * connection = static_cast<TcpConnectionData *>(context);

    if (connection)
    {
        TcpMultiServerData * xx = connection->fOwner;
        OSStatus err;
        // OTResult          endpoint_state;
        TDiscon              discon;
        bool                 do_error_bang = false;

#if defined(BE_VERBOSE)
        if (xx->fVerbose)
        {
            LOG_POST_3(xx, OUTPUT_PREFIX "notifier code: 0x%lx = %s", static_cast<long>(code), mapEventToString(code))
        }
#endif /* BE_VERBOSE */
        switch (code)
        {
            case T_PASSCON:
                if (! connection->fActive)
                {
                    connection->fActive = true;
                    ++xx->fActiveConnections;
                }
                setConnectionState(connection, kTcpStateConnected);
                break;

            case T_CONNECT:
                break;

            case T_DATA:
                do_error_bang = (! makeReceiveBufferAvailable(connection));
                break;

            case T_DISCONNECT:
                WRAP_OT_CALL(xx, err, "OTRcvDisconnect",
                             OTRcvDisconnect(connection->fDataEndpoint, &discon))
                if (kOTNoError == err)
                {
#if defined(BE_VERBOSE)
                    if (xx->fVerbose)
                    {
                        LOG_POST_2(xx, OUTPUT_PREFIX "disconnect reason (%ld)",
                                   static_cast<long>(E2OSStatus(discon.reason)))
                    }
#endif /* BE_VERBOSE */
                }
                else
                {
                    if (kOTNoDisconnectErr == err)
                    {
                        break;
                    }

                    REPORT_ERROR(xx, OUTPUT_PREFIX "OTRcvDisconnect failed (%ld = %s)", err)
                    reportEndpointState(xx, connection->fDataEndpoint);
                    do_error_bang = true;
                }
                setConnectionState(connection, kTcpStateBound);
                if (connection->fActive)
                {
                    connection->fActive = false;
                    --xx->fActiveConnections;
                }
                break;

            case T_DISCONNECTCOMPLETE:
                setConnectionState(connection, kTcpStateBound);
                if (connection->fActive)
                {
                    connection->fActive = false;
                    --xx->fActiveConnections;
                }
                break;

            case T_EXDATA:
                break;

            case T_GODATA:
                break;

            case T_GOEXDATA:
                break;

            case T_MEMORYRELEASED:
                break;

            case T_ORDREL:
                WRAP_OT_CALL(xx, err, "OTRcvOrderlyDisconnect",
                             OTRcvOrderlyDisconnect(connection->fDataEndpoint))
                if (kOTNoError == err)
                {
                    //
                }
                else
                {
                    if (kOTNoDisconnectErr == err)
                    {
                        break;
                    }

                    REPORT_ERROR(xx, OUTPUT_PREFIX "OTRcvOrderlyDisconnect failed (%ld = %s)", err)
                    reportEndpointState(xx, connection->fDataEndpoint);
                    do_error_bang = true;
                }
                if (connection->fState != kTcpStateDisconnecting)
                {
                    WRAP_OT_CALL(xx, err, "OTSndOrderlyDisconnect",
                                 OTSndOrderlyDisconnect(connection->fDataEndpoint))
                    if (err != kOTNoError)
                    {
                        REPORT_ERROR(xx, OUTPUT_PREFIX "OTSndOrderlyDisconnect failed (%ld = %s)", err)
                        reportEndpointState(xx, connection->fDataEndpoint);
                        do_error_bang = true;
                    }
                }
                setConnectionState(connection, kTcpStateBound);
                if (connection->fActive)
                {
                    connection->fActive = false;
                    --xx->fActiveConnections;
                }
                break;

            case T_UNBINDCOMPLETE:
                setConnectionState(connection, kTcpStateUnbound);
                break;

            default:
                break;
        }
        if (do_error_bang)
        {
            signalError(xx);
        }
    }
} // tcpMultiServerDataNotifier
/*------------------------------------ tcpMultiServerListenNotifier ---*/
pascal void tcpMultiServerListenNotifier(void *      context,
                                         OTEventCode code,
                                         OTResult    result,
                                         void *      cookie)
{
#pragma unused(result, cookie)
    TcpMultiServerData * xx = static_cast<TcpMultiServerData *>(context);

    if (xx)
    {
        OSStatus err;
        // OTResult endpointState;
        TDiscon  discon;
        bool     do_error_bang = false;

#if defined(BE_VERBOSE)
        if (xx->fVerbose)
        {
            LOG_POST_3(xx, OUTPUT_PREFIX "notifier code: 0x%lx = %s", static_cast<long>(code), mapEventToString(code))
        }
#endif /* BE_VERBOSE */
        switch (code)
        {
            case T_BINDCOMPLETE:
                setObjectState(xx, kTcpStateBound);
                break;

            case T_CONNECT:
                break;

            case T_DISCONNECT:
                WRAP_OT_CALL(xx, err, "OTRcvDisconnect", OTRcvDisconnect(xx->fListenEndpoint, &discon))
                if (kOTNoError == err)
                {
#if defined(BE_VERBOSE)
                    if (xx->fVerbose)
                    {
                        LOG_POST_2(xx, OUTPUT_PREFIX "disconnect reason (%ld)",
                                   static_cast<long>(E2OSStatus(discon.reason)))
                    }
#endif /* BE_VERBOSE */
                }
                else
                {
                    if (kOTNoDisconnectErr == err)
                    {
                        break;
                    }

                    REPORT_ERROR(xx, OUTPUT_PREFIX "OTRcvDisconnect failed (%ld = %s)", err)
                    reportEndpointState(xx, xx->fListenEndpoint);
                    do_error_bang = true;
                }
                setObjectState(xx, kTcpStateListening);
                break;

            case T_DISCONNECTCOMPLETE:
                setObjectState(xx, kTcpStateListening);
                break;

            case T_EXDATA:
                break;

            case T_GODATA:
                break;

            case T_GOEXDATA:
                break;

            case T_LISTEN:
                if (kTcpStateListening == xx->fState)
                {
                    processListenEvent(xx);
                }
                else
                {
                    LOG_ERROR_1(xx, OUTPUT_PREFIX "Listen request received while not listening")
                    do_error_bang = true;
                }
                break;

            case T_MEMORYRELEASED:
                break;

            case T_ORDREL:
                WRAP_OT_CALL(xx, err, "OTRcvOrderlyDisconnect",
                             OTRcvOrderlyDisconnect(xx->fListenEndpoint))
                if (kOTNoError == err)
                {
                    //
                }
                else
                {
                    if (kOTNoDisconnectErr == err)
                    {
                        break;
                    }

                    REPORT_ERROR(xx, OUTPUT_PREFIX "OTRcvOrderlyDisconnect failed (%ld = %s)", err)
                    reportEndpointState(xx, xx->fListenEndpoint);
                    do_error_bang = true;
                }
                if (xx->fState != kTcpStateDisconnecting)
                {
                    WRAP_OT_CALL(xx, err, "OTSndOrderlyDisconnect",
                                 OTSndOrderlyDisconnect(xx->fListenEndpoint))
                    if (err != kOTNoError)
                    {
                        REPORT_ERROR(xx, OUTPUT_PREFIX "OTSndOrderlyDisconnect failed (%ld = %s)", err)
                        reportEndpointState(xx, xx->fListenEndpoint);
                        do_error_bang = true;
                    }
                }
                setObjectState(xx, kTcpStateBound);
                break;

            case T_PASSCON:
                break;

            case T_UNBINDCOMPLETE:
                setObjectState(xx, kTcpStateUnbound);
                break;

            default:
                break;
        }
        if (do_error_bang)
        {
            signalError(xx);
        }
    }
} // tcpMultiServerListenNotifier
