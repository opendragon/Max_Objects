/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       tcpServer.cpp                                                           */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the tcpServer module.     */
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
#include "tcpServer.h"
#include "reportVersion.h"

/* Forward references: */
void * tcpServerCreate(long port,
                       long numbBuffers);

void tcpServerFree(TcpObjectData * xx);

/*------------------------------------ main ---*/
int main(void)
{
    /* Allocate class memory and set up class. */
    t_class * temp = class_new(OUR_NAME, reinterpret_cast<method>(tcpServerCreate),
                               reinterpret_cast<method>(tcpServerFree), sizeof(TcpObjectData),
                               reinterpret_cast<method>(0L), A_DEFLONG, A_DEFLONG, 0);

    if (temp)
    {
        class_addmethod(temp, reinterpret_cast<method>(cmd_Anything), MESSAGE_ANYTHING, A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Assist), MESSAGE_ASSIST, A_CANT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Disconnect), "disconnect", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Float), MESSAGE_FLOAT, A_FLOAT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Int), MESSAGE_INT, A_LONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_List), MESSAGE_LIST, A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Listen), "listen", A_SYM, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Mode), "mode", A_SYM, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Port), "port", A_LONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Self), "self", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Send), "send", A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Status), MESSAGE_BANG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Status), "status", 0);
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
/*------------------------------------ tcpServerCreate ---*/
void * tcpServerCreate(long port,
                       long numBuffers)
{
    TcpObjectData * xx = static_cast<TcpObjectData *>(object_alloc(gClass));

    if (xx)
    {
        bool               okSoFar = true;
        OSStatus           result;
        OTConfigurationRef this_config;

#if defined(BE_VERBOSE)
        xx->fVerbose = false;
#endif /* BE_VERBOSE */
        presetObjectPointers(xx);
        if ((port < 0) || (port > MAX_PORT) || (numBuffers < 0))
        {
            LOG_ERROR_1(xx, OUTPUT_PREFIX "invalid parameters for device")
            okSoFar = false;
        }
        if (0 == numBuffers)
        {
            numBuffers = NUM_RX_BUFFERS;
        }
        if (okSoFar)
        {
            xx->fServerAddress = 0L;
            okSoFar = initObject(OUR_NAME, xx, port, numBuffers);
        }
        xx->fDataNotifier = NewOTNotifyUPP(tcpServerNotifier);
        if (okSoFar)
        {
            xx->fAccessControl = acquireOpenTransport(OUR_NAME, static_cast<unsigned short>(port), true);
            if (! xx->fAccessControl)
            {
                okSoFar = false;
            }
        }
        /* Get the desired configuration */
        if (okSoFar)
        {
            this_config = OTCreateConfiguration(kTCPName);
            if ((kOTInvalidConfigurationPtr == this_config) || (kOTNoMemoryConfigurationPtr == this_config))
            {
                LOG_ERROR_1(xx, OUTPUT_PREFIX "unable to obtain an OT configuration")
                okSoFar = false;
            }
        }
        /* Set up the endpoint */
        if (okSoFar)
        {
            TEndpointInfo info;

            xx->fSocket = OTOpenEndpointInContext(this_config, 0, &info, &result,
                                                    xx->fAccessControl->fContext);
            if (kOTNoError == result)
            {
                xx->fServiceType = info.servtype;
                xx->fAddressSize = short(info.addr);
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
            WRAP_OT_CALL(xx, result, "OTSetAsynchronous", OTSetAsynchronous(xx->fSocket))
            if (result != kOTNoError)
            {
                REPORT_ERROR(xx, OUTPUT_PREFIX "OTSetAsynchronous failed (%ld = %s)", result)
                reportEndpointState(OUR_NAME, xx);
                okSoFar = false;
            }
        }
        if (okSoFar)
        {
            WRAP_OT_CALL(xx, result, "OTSetBlocking", OTSetBlocking(xx->fSocket))
            if (result != kOTNoError)
            {
                REPORT_ERROR(xx, OUTPUT_PREFIX "OTSetBlocking failed (%ld = %s)", result)
                reportEndpointState(OUR_NAME, xx);
                okSoFar = false;
            }
        }
        if (okSoFar)
        {
            WRAP_OT_CALL(xx, result, "OTInstallNotifier", OTInstallNotifier(xx->fSocket,
                                                                            xx->fDataNotifier,
                                                                            xx))
            if (result != kOTNoError)
            {
                REPORT_ERROR(xx, OUTPUT_PREFIX "OTInstallNotifier failed (%ld = %s)", result)
                reportEndpointState(OUR_NAME, xx);
                okSoFar = false;
            }
        }
        if (okSoFar)
        {
            okSoFar = tcpServerSetPort(xx, false);
        }
        if (! okSoFar)
        {
            freeobject(reinterpret_cast<t_object *>(xx));
            xx = NULL_PTR;
        }
    }
    return xx;
} // tcpServerCreate
/*------------------------------------ tcpServerFree ---*/
void tcpServerFree(TcpObjectData * xx)
{
    if (xx)
    {
        OSStatus result;

        xx->fClosing = true;
        if (xx->fSocket != kOTInvalidEndpointRef)
        {
            switch (xx->fState)
            {
                case kTcpStateConnected:
                    tcpServerDisconnect(xx, true);
                /* Fall through */

                case kTcpStateListening:
                /* Fall through */

                case kTcpStateBound:
                    WRAP_OT_CALL(xx, result, "OTUnbind", OTUnbind(xx->fSocket))
                    if (result != kOTNoError)
                    {
                        REPORT_ERROR(xx, OUTPUT_PREFIX "OTUnbind failed (%ld = %s)", result)
                        reportEndpointState(OUR_NAME, xx);
                    }
                /* Fall through */

                case kTcpStateUnbound:
                    WRAP_OT_CALL(xx, result, "OTCloseProvider", OTCloseProvider(xx->fSocket))
                    if (result != kOTNoError)
                    {
                        REPORT_ERROR(xx, OUTPUT_PREFIX "OTCloseProvider failed (%ld = %s)", result)
                        reportEndpointState(OUR_NAME, xx);
                    }
                    break;
            }
        }
        DisposeOTNotifyUPP(xx->fDataNotifier);
        releaseObjectMemory(OUR_NAME, xx);
    }
} // tcpServerFree
/*------------------------------------ tcpServerSetPort ---*/
bool tcpServerSetPort(TcpObjectData * xx,
                      const bool      bangOnError)
{
    bool okSoFar = true;

    if (xx)
    {
        OSStatus result;

        switch (xx->fState)
        {
            case kTcpStateConnected:
                LOG_ERROR_1(xx, OUTPUT_PREFIX "connection is active")
                okSoFar = false;
                break;
            /* Fall through */

            case kTcpStateListening:
                LOG_ERROR_1(xx, OUTPUT_PREFIX "actively listening")
                okSoFar = false;
                break;
            /* Fall through */

            case kTcpStateBound:
                WRAP_OT_CALL(xx, result, "OTUnbind", OTUnbind(xx->fSocket))
                if (result != kOTNoError)
                {
                    REPORT_ERROR(xx, OUTPUT_PREFIX "OTUnbind failed (%ld = %s)", result)
                    reportEndpointState(OUR_NAME, xx);
                    okSoFar = false;
                }
                setObjectState(xx, kTcpStateUnbound);
            /* Fall through */

            case kTcpStateUnbound:
                break;

            default:
                LOG_ERROR_3(xx, OUTPUT_PREFIX "unexpected state (%ld = %s)", static_cast<long>(xx->fState),
                            mapStateToSymbol(xx->fState)->s_name)
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
            WRAP_OT_CALL(xx, result, "OTBind", OTBind(xx->fSocket, &bind_request, NULL_PTR))
            if (result != kOTNoError)
            {
                REPORT_ERROR(xx, OUTPUT_PREFIX "OTBind failed (%ld = %s)", result)
                reportEndpointState(OUR_NAME, xx);
                okSoFar = false;
            }
        }
        if ((! okSoFar) && bangOnError)
        {
            signalError(xx);
        }
    }
    return okSoFar;
} // tcpServerSetPort
/*------------------------------------ tcpServerDisconnect ---*/
bool tcpServerDisconnect(TcpObjectData * xx,
                         const bool      forced)
{
    bool okSoFar = true;

    if (xx)
    {
        switch (xx->fState)
        {
            case kTcpStateUnbound:
                LOG_ERROR_1(xx, OUTPUT_PREFIX "not bound")
                okSoFar = false;
                break;

            case kTcpStateBound:
                LOG_ERROR_1(xx, OUTPUT_PREFIX "not connected")
                okSoFar = false;
                break;

            case kTcpStateListening:
                LOG_ERROR_1(xx, OUTPUT_PREFIX "listening")
                okSoFar = false;
                break;

            case kTcpStateConnected:
                break;
        }
        if (okSoFar || forced)
        {
            OSStatus result;

            if ((! forced) && ((T_COTS_ORD == xx->fServiceType) || (T_TRANS_ORD == xx->fServiceType)))
            {
                WRAP_OT_CALL(xx, result, "OTSndOrderlyDisconnect",
                             OTSndOrderlyDisconnect(xx->fSocket))
                if (kOTNoDataErr == result)
                {
                    setObjectState(xx, kTcpStateDisconnecting);
                }
                else
                {
                    REPORT_ERROR(xx, OUTPUT_PREFIX "OTSndOrderlyDisconnect failed (%ld = %s)", result)
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
                snd_call.addr.buf = reinterpret_cast<unsigned char *>(&in_addr);
                WRAP_OT_CALL(xx, result, "OTSndDisconnect",
                             OTSndDisconnect(xx->fSocket, &snd_call))
                if (kOTNoError == result)
                {
                    setObjectState(xx, kTcpStateBound);
                }
                else
                {
                    REPORT_ERROR(xx, OUTPUT_PREFIX "OTSndDisconnect failed (%ld = %s)", result)
                    reportEndpointState(OUR_NAME, xx);
                    okSoFar = false;
                }
            }
        }
        if (! okSoFar)
        {
            signalError(xx);
        }
    }
    return okSoFar;
} // tcpServerDisconnect
