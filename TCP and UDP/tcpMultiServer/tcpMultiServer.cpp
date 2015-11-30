/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       tcpMultiServer.cpp                                                      */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the tcpMultiServer        */
/*              module.                                                                 */
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

#define IS_MAIN /* */
#include "tcpMultiServer.h"
#include "reportVersion.h"

/*------------------------------------ tcpMultiServerCreate ---*/
static void * tcpMultiServerCreate(const long port,
                                   const long clients,
                                   const long numBuffers)
{
    TcpMultiServerData * xx = static_cast<TcpMultiServerData *>(object_alloc(gClass));
    
    if (xx)
    {
#if 0
        bool               okSoFar = true;
        OSStatus           result;
        OTConfigurationRef this_config;
        
#if defined(BE_VERBOSE)
        xx->fVerbose = false;
#endif /* BE_VERBOSE */
        presetObjectPointers(xx);
        if ((0 > port) || (MAX_PORT < port) || (0 > clients) || (MAX_CLIENTS < clients) ||
            (0 > numBuffers))
        {
            LOG_ERROR_1(xx, OUTPUT_PREFIX "invalid parameters for device")
            okSoFar = false;
        }
        if (okSoFar)
        {
            long actBuffCount;

            xx->fServerAddress = 0L;
            xx->fMaximumConnections = static_cast<unsigned short>(clients ? clients : 5);
            if (0 == numBuffers)
            {
                actBuffCount = (NUM_RX_BUFFERS * clients);
            }
            else
            {
                actBuffCount = numBuffers;
            }
            xx->fActiveConnections = 0;
            okSoFar = initObject(xx, port, actBuffCount);
            if (okSoFar)
            {
                xx->fConnections = GET_BYTES(xx->fMaximumConnections, TcpConnectionData *);
                xx->fConnectionBase = MAKE_TYPED_HANDLE(TcpConnectionData, xx->fMaximumConnections);
                if (! (xx->fConnections && xx->fConnectionBase))
                {
                    okSoFar = false;
                }
            }
        }
        xx->fDataNotifier = NewOTNotifyUPP(tcpMultiServerDataNotifier);
        xx->fListenNotifier = NewOTNotifyUPP(tcpMultiServerListenNotifier);
        if (okSoFar)
        {
            xx->fAccessControl = acquireOpenTransport(OUR_NAME, static_cast<unsigned short>(port),
                                                      true);
            if (! xx->fAccessControl)
            {
                okSoFar = false;
            }
        }
        /* Get the desired configurations */
        if (okSoFar)
        {
            this_config = OTCreateConfiguration("tilisten,tcp");
            if ((kOTInvalidConfigurationPtr == this_config) ||
                (kOTNoMemoryConfigurationPtr == this_config))
            {
                LOG_ERROR_1(xx, OUTPUT_PREFIX "unable to obtain an OT listener configuration")
                okSoFar = false;
            }
        }
        /* Set up the endpoint */
        if (okSoFar)
        {
            TEndpointInfo info;
            
            xx->fListenEndpoint = OTOpenEndpointInContext(this_config, 0, &info, &result,
                                                          xx->fAccessControl->fContext);
            if (kOTNoError == result)
            {
                xx->fServiceType = info.servtype;
                xx->fAddressSize = info.addr;
            }
            else
            {
                REPORT_ERROR(xx, OUTPUT_PREFIX "unable to obtain an endpoint (%ld = %s)", result)
                okSoFar = false;
            }
        }
        if (okSoFar)
        {
            InetInterfaceInfo interfaceInfo;
            
            WRAP_OT_CALL(xx, result, "OTInetGetInterfaceInfo",
                         OTInetGetInterfaceInfo(&interfaceInfo, kDefaultInetInterface));
            if (kOTNoError == result)
            {
                xx->fServerAddress = interfaceInfo.fAddress;
            }
            else
            {
                REPORT_ERROR(xx, OUTPUT_PREFIX "OTInetGetInterfaceInfo failed (%ld = %s)", result)
                okSoFar = false;
            }
        }
        if (okSoFar)
        {
            WRAP_OT_CALL(xx, result, "OTSetAsynchronous", OTSetAsynchronous(xx->fListenEndpoint))
            if (kOTNoError != result)
            {
                REPORT_ERROR(xx, OUTPUT_PREFIX "OTSetAsynchronous failed (%ld = %s)", result)
                reportEndpointState(xx, xx->fListenEndpoint);
                okSoFar = false;
            }
        }
        if (okSoFar)
        {
            WRAP_OT_CALL(xx, result, "OTSetBlocking", OTSetBlocking(xx->fListenEndpoint))
            if (kOTNoError != result)
            {
                REPORT_ERROR(xx, OUTPUT_PREFIX "OTSetBlocking failed (%ld = %s)", result)
                reportEndpointState(xx, xx->fListenEndpoint);
                okSoFar = false;
            }
        }
        if (okSoFar)
        {
            WRAP_OT_CALL(xx, result, "OTInstallNotifier",
                         OTInstallNotifier(xx->fListenEndpoint, xx->fListenNotifier,
                                           xx))
            if (kOTNoError != result)
            {
                REPORT_ERROR(xx, OUTPUT_PREFIX "OTInstallNotifier failed (%ld = %s)", result)
                reportEndpointState(xx, xx->fListenEndpoint);
                okSoFar = false;
            }
        }
        if (okSoFar)
        {
            okSoFar = tcpMultiServerSetPort(xx, false);
        }
        if (okSoFar)
        {
            okSoFar = tcpMultiServerConstructConnections(xx);
        }
        if (! okSoFar)
        {
            freeobject(reinterpret_cast<t_object *>(xx));
            xx = NULL;
        }
#endif//0
    }
    return xx;
} // tcpMultiServerCreate

/*------------------------------------ tcpMultiServerFree ---*/
static void tcpMultiServerFree(TcpMultiServerData * xx)
{
    if (xx)
    {
#if 0
        OSStatus            result;
        TcpConnectionData * connection;
        
        xx->fClosing = true;
        // Close off all other connections:
        for (unsigned short ii = 0; ii < xx->fMaximumConnections; ++ii)
        {
            connection = *(xx->fConnections + ii);
            if (connection && (kOTInvalidEndpointRef != connection->fDataEndpoint) &&
                connection->fActive)
            {
                tcpMultiServerDisconnect(xx, connection, true);
            }
        }
        if (kOTInvalidEndpointRef != xx->fListenEndpoint)
        {
            switch (xx->fState)
            {
                case kTcpStateConnected:
                    tcpMultiServerDisconnect(xx, NULL, true); /*!!*/
                    /* Fall through */
                    
                case kTcpStateListening:
                    /* Fall through */
                    
                case kTcpStateBound:
                    WRAP_OT_CALL(xx, result, "OTUnbind", OTUnbind(xx->fListenEndpoint))
                    if (kOTNoError != result)
                    {
                        REPORT_ERROR(xx, OUTPUT_PREFIX "OTUnbind failed (%ld = %s)", result)
                        reportEndpointState(xx, xx->fListenEndpoint);
                    }
                    /* Fall through */
                    
                case kTcpStateUnbound:
                    WRAP_OT_CALL(xx, result, "OTCloseProvider",
                                 OTCloseProvider(xx->fListenEndpoint))
                    if (kOTNoError == result)
                    {
                        xx->fListenEndpoint = kOTInvalidEndpointRef;
                    }
                    else
                    {
                        REPORT_ERROR(xx, OUTPUT_PREFIX "OTCloseProvider failed (%ld = %s)", result)
                        reportEndpointState(xx, xx->fListenEndpoint);
                    }
                    break;
                    
                default:
                    break;
                    
            }
        }
        DisposeOTNotifyUPP(xx->fDataNotifier);
        DisposeOTNotifyUPP(xx->fListenNotifier);
        releaseObjectMemory(xx);
#endif//0
    }
} // tcpMultiServerFree

/*------------------------------------ main ---*/
int C74_EXPORT main(void)
{
    /* Allocate class memory and set up class. */
    t_class * temp = class_new(OUR_NAME, reinterpret_cast<method>(tcpMultiServerCreate),
                               reinterpret_cast<method>(tcpMultiServerFree),
                               static_cast<long>(sizeof(TcpMultiServerData)),
                               reinterpret_cast<method>(NULL), A_DEFLONG, A_DEFLONG, A_DEFLONG, 0);

    if (temp)
    {
        class_addmethod(temp, reinterpret_cast<method>(cmd_Anything), MESSAGE_ANYTHING, A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Assist), MESSAGE_ASSIST, A_CANT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Bang), MESSAGE_BANG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Disconnect), "disconnect", A_DEFLONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(stdinletinfo), MESSAGE_INLETINFO, A_CANT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Listen), "listen", A_SYM, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Mode), "mode", A_LONG, A_SYM, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Port), "port", A_LONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Self), "self", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Send), "send", A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Status), "status", A_DEFLONG, 0);
#if defined(BE_VERBOSE)
        class_addmethod(temp, reinterpret_cast<method>(cmd_Verbose), "verbose", A_DEFSYM, 0);
#endif /* BE_VERBOSE */
        class_register(CLASS_BOX, temp);
    }
    gClass = temp;
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
    reportVersion(OUR_NAME);
    return 0;
} // main

/*------------------------------------ tcpMultiServerConstructConnections ---*/
static bool tcpMultiServerConstructConnections(TcpMultiServerData * xx)
{
    TcpConnectionData ** ptr_walker = xx->fConnections;
    TcpConnectionData *  a_connection;
    DataBuffer *         next_buffer = *xx->fBufferBase;
    bool                 okSoFar = true;
#if 0
    TEndpointInfo        info;
    OSStatus             result;
    OTConfigurationRef   data_config = NULL;
    OTConfigurationRef   a_config;

    data_config = OTCreateConfiguration(kTCPName);
    if ((kOTInvalidConfigurationPtr == data_config) || (kOTNoMemoryConfigurationPtr == data_config))
    {
        LOG_ERROR_1(xx, OUTPUT_PREFIX "unable to obtain an OT data configuration")
        okSoFar = false;
    }
    sysmem_lockhandle(reinterpret_cast<t_handle>(xx->fConnectionBase), 1);
    a_connection = *xx->fConnectionBase;
    memset(xx->fConnections, 0, xx->fMaximumConnections * sizeof(TcpConnectionData *));
    memset(*xx->fConnectionBase, 0, sizeof(TcpConnectionData) * xx->fMaximumConnections);
    for (unsigned short ii = 0; okSoFar && (ii < xx->fMaximumConnections); ++ii)
    {
        *ptr_walker++ = a_connection;
        a_connection->fDataEndpoint = kOTInvalidEndpointRef;
        a_connection->fOwner = xx;
        a_connection->fIdentifier = static_cast<unsigned short>(ii + 1);
        a_connection->fActive = false;
        a_connection->fSendBuffer = next_buffer;
        a_connection->fClientAddress = 0L;
        a_connection->fClientPort = 0;
        a_connection->fPartnerName = NULL;
        setConnectionState(a_connection, kTcpStateUnbound);
        a_config = OTCloneConfiguration(data_config);
        if (! a_config)
        {
            okSoFar = false;
        }
        if (okSoFar)
        {
            a_connection->fDataEndpoint = OTOpenEndpointInContext(a_config, 0, &info, &result,
                                                                  xx->fAccessControl->fContext);
            if (kOTNoError == result)
            {
                a_connection->fServiceType = info.servtype;
            }
            else
            {
                REPORT_ERROR(xx, OUTPUT_PREFIX "unable to obtain an endpoint (%ld = %s)", result)
                okSoFar = false;
            }
        }
        if (okSoFar)
        {
            WRAP_OT_CALL(xx, result, "OTSetAsynchronous",
                         OTSetAsynchronous(a_connection->fDataEndpoint))
            if (kOTNoError != result)
            {
                REPORT_ERROR(xx, OUTPUT_PREFIX "OTSetAsynchronous failed (%ld = %s)", result)
                reportEndpointState(xx, a_connection->fDataEndpoint);
                okSoFar = false;
            }
        }
        if (okSoFar)
        {
            WRAP_OT_CALL(xx, result, "OTSetBlocking", OTSetBlocking(a_connection->fDataEndpoint))
            if (kOTNoError != result)
            {
                REPORT_ERROR(xx, OUTPUT_PREFIX "OTSetBlocking failed (%ld = %s)", result)
                reportEndpointState(xx, a_connection->fDataEndpoint);
                okSoFar = false;
            }
        }
        if (okSoFar)
        {
            WRAP_OT_CALL(xx, result, "OTInstallNotifier",
                         OTInstallNotifier(a_connection->fDataEndpoint,
                                           xx->fDataNotifier, a_connection))
            if (kOTNoError != result)
            {
                REPORT_ERROR(xx, OUTPUT_PREFIX "OTInstallNotifier failed (%ld = %s)", result)
                reportEndpointState(xx, a_connection->fDataEndpoint);
                okSoFar = false;
            }
        }
        a_connection->fReceiveBuffer =
                            reinterpret_cast<DataBuffer *>(ADD_TO_ADDRESS(a_connection->fSendBuffer,
                                                                          BUFF_MEMORY_TO_ALLOC));
        next_buffer = reinterpret_cast<DataBuffer *>(ADD_TO_ADDRESS(a_connection->fReceiveBuffer,
                                                                    BUFF_MEMORY_TO_ALLOC));
        a_connection = reinterpret_cast<TcpConnectionData *>(ADD_TO_ADDRESS(a_connection,
                                                                        sizeof(TcpConnectionData)));
    }
    if ((kOTInvalidConfigurationPtr != data_config) && (kOTNoMemoryConfigurationPtr != data_config))
    {
        OTDestroyConfiguration(data_config);
    }
#endif//0
    return okSoFar;
} // tcpMultiServerConstructConnections

/*------------------------------------ tcpMultiServerSetPort ---*/
bool tcpMultiServerSetPort(TcpMultiServerData * xx,
                           const bool           bangOnError)
{
    bool okSoFar = true;

    if (xx)
    {
#if 0
        OSStatus result;

        switch (xx->fState)
        {
            case kTcpStateListening:
                LOG_ERROR_1(xx, OUTPUT_PREFIX "actively listening")
                okSoFar = false;
                break;
            /* Fall through */

            case kTcpStateBound:
                WRAP_OT_CALL(xx, result, "OTUnbind", OTUnbind(xx->fListenEndpoint))
                if (kOTNoError != result)
                {
                    REPORT_ERROR(xx, OUTPUT_PREFIX "OTUnbind failed (%ld = %s)", result)
                    reportEndpointState(xx, xx->fListenEndpoint);
                    okSoFar = false;
                }
                setObjectState(xx, kTcpStateUnbound);
            /* Fall through */

            case kTcpStateUnbound:
                break;

            default:
                LOG_ERROR_3(xx, OUTPUT_PREFIX "unexpected state (%ld = %s)",
                            static_cast<long>(xx->fState), mapStateToSymbol(xx->fState)->s_name)
                okSoFar = false;
                break;
                
        }
        if (okSoFar)
        {
            TBind       bind_request;
            InetAddress in_address;

            OTInitInetAddress(&in_address, xx->fServerPort, xx->fServerAddress);
            bind_request.addr.len = sizeof(in_address);
            bind_request.addr.buf = reinterpret_cast<unsigned char *>(&in_address);
            bind_request.qlen = 1;
            WRAP_OT_CALL(xx, result, "OTBind", OTBind(xx->fListenEndpoint, &bind_request, NULL))
            if (kOTNoError != result)
            {
                REPORT_ERROR(xx, OUTPUT_PREFIX "OTBind failed (%ld = %s)", result)
                reportEndpointState(xx, xx->fListenEndpoint);
                okSoFar = false;
            }
        }
        if ((! okSoFar) && bangOnError)
        {
            signalError(xx);
        }
#endif//0
    }
    return okSoFar;
} // tcpMultiServerSetPort

/*------------------------------------ tcpMultiServerDisconnect ---*/
bool tcpMultiServerDisconnect(TcpMultiServerData * xx,
                              TcpConnectionData *  connection,
                              const bool           forced)
{
    bool okSoFar = true;

    if (xx && connection)
    {
#if 0
        switch (connection->fState)
        {
            case kTcpStateUnbound:
                if (! xx->fClosing)
                {
                    LOG_ERROR_1(xx, OUTPUT_PREFIX "not bound")
                    okSoFar = false;
                }
                break;

            case kTcpStateBound:
                if (! xx->fClosing)
                {
                    LOG_ERROR_1(xx, OUTPUT_PREFIX "not connected")
                    okSoFar = false;
                }
                break;

            case kTcpStateConnected:
                break;
                
            default:
                break;
                
        }
        if (okSoFar || forced)
        {
            OSStatus result;

            if ((! forced) && ((T_COTS_ORD == connection->fServiceType) ||
                               (T_TRANS_ORD == connection->fServiceType)))
            {
                WRAP_OT_CALL(xx, result, "OTSndOrderlyDisconnect",
                             OTSndOrderlyDisconnect(connection->fDataEndpoint))
                if (kOTNoDataErr == result)
                {
                    setConnectionState(connection, kTcpStateDisconnecting);
                }
                else
                {
                    REPORT_ERROR(xx, OUTPUT_PREFIX "OTSndOrderlyDisconnect failed (%ld = %s)",
                                 result)
                    reportEndpointState(xx, connection->fDataEndpoint);
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
                snd_call.addr.buf = reinterpret_cast<unsigned char *>(&in_addr);
                WRAP_OT_CALL(xx, result, "OTSndDisconnect",
                             OTSndDisconnect(connection->fDataEndpoint, &snd_call))
                if (kOTNoError == result)
                {
                    setConnectionState(connection, kTcpStateBound);
                }
                else
                {
                    REPORT_ERROR(xx, OUTPUT_PREFIX "OTSndDisconnect failed (%ld = %s)", result)
                    reportEndpointState(xx, connection->fDataEndpoint);
                    okSoFar = false;
                }
            }
        }
        if (! okSoFar)
        {
            signalError(xx);
        }
#endif//0
    }
    return okSoFar;
} // tcpMultiServerDisconnect

/*------------------------------------ tcpMultiServerValidateClient ---*/
TcpConnectionData * tcpMultiServerValidateClient(TcpMultiServerData * xx,
                                                 const long           client)
{
    TcpConnectionData * candidate = NULL;

    if ((0 < client) && (client <= xx->fMaximumConnections))
    {
        candidate = *(xx->fConnections + client - 1);
    }
    return candidate;
} // tcpMultiServerValidateClient
