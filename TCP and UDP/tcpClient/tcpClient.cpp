/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       tcpClient.cpp                                                           */
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
#include "reportVersion.h"

/*------------------------------------ tcpClientCreate ---*/
static void * tcpClientCreate(t_symbol * ipAddress,
                              const long port,
                              const long numBuffers)
{
    TcpObjectData * xx = static_cast<TcpObjectData *>(object_alloc(gClass));
    
    if (xx)
    {
        bool               okSoFar = true;
        long               actBuffCount;
        short              byte_0 = DEFAULT_IP_ADDR_0;
        short              byte_1 = DEFAULT_IP_ADDR_1;
        short              byte_2 = DEFAULT_IP_ADDR_2;
        short              byte_3 = DEFAULT_IP_ADDR_3;
#if 0
        OSStatus           result;
        OTConfigurationRef this_config;
#endif//0
        
#if defined(BE_VERBOSE)
        xx->fVerbose = false;
#endif /* BE_VERBOSE */
        presetObjectPointers(xx);
        if ((0 > port) || (MAX_PORT < port) || (0 > numBuffers) ||
            (! checkIpString(ipAddress, byte_0, byte_1, byte_2, byte_3)))
        {
            LOG_ERROR_1(xx, OUTPUT_PREFIX "invalid parameters for device")
            okSoFar = false;
        }
        if (0 == numBuffers)
        {
            actBuffCount = NUM_RX_BUFFERS;
        }
        else
        {
            actBuffCount = numBuffers;
        }
        if (okSoFar)
        {
            okSoFar = initObject(OUR_NAME, xx, port, actBuffCount);
        }
        /* Get the desired configuration */
        if (okSoFar)
        {
#if 0
            this_config = OTCreateConfiguration(kTCPName);
            if ((kOTInvalidConfigurationPtr == this_config) ||
                (kOTNoMemoryConfigurationPtr == this_config))
            {
                LOG_ERROR_1(xx, OUTPUT_PREFIX "unable to obtain an OT configuration")
                okSoFar = false;
            }
#endif//0
        }
        /* Set up the endpoint */
        if (okSoFar)
        {
#if 0
            TEndpointInfo info;
            
            xx->fSocket = OTOpenEndpointInContext(this_config, 0, &info, &result,
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
#endif//0
        }
        if (okSoFar)
        {
#if 0
            WRAP_OT_CALL(xx, result, "OTSetAsynchronous", OTSetAsynchronous(xx->fSocket))
            if (kOTNoError != result)
            {
                REPORT_ERROR(xx, OUTPUT_PREFIX "OTSetAsynchronous failed (%ld = %s)", result)
                reportEndpointState(OUR_NAME, xx);
                okSoFar = false;
            }
#endif//0
        }
        if (okSoFar)
        {
#if 0
            WRAP_OT_CALL(xx, result, "OTSetBlocking", OTSetBlocking(xx->fSocket))
            if (kOTNoError != result)
            {
                REPORT_ERROR(xx, OUTPUT_PREFIX "OTSetBlocking failed (%ld = %s)", result)
                reportEndpointState(OUR_NAME, xx);
                okSoFar = false;
            }
#endif//0
        }
        if (okSoFar)
        {
#if 0
            WRAP_OT_CALL(xx, result, "OTInstallNotifier", OTInstallNotifier(xx->fSocket,
                                                                            xx->fDataNotifier,
                                                                            xx))
            if (kOTNoError != result)
            {
                REPORT_ERROR(xx, OUTPUT_PREFIX "OTInstallNotifier failed (%ld = %s)", result)
                reportEndpointState(OUR_NAME, xx);
                okSoFar = false;
            }
#endif//0
        }
        if (okSoFar)
        {
            okSoFar = tcpClientSetServer(xx, byte_0, byte_1, byte_2, byte_3, false);
        }
        if (okSoFar)
        {
#if 0
            WRAP_OT_CALL(xx, result, "OTBind", OTBind(xx->fSocket, NULL, NULL))
            if (kOTNoError != result)
            {
                REPORT_ERROR(xx, OUTPUT_PREFIX "OTBind failed (%ld = %s)", result)
                reportEndpointState(OUR_NAME, xx);
                okSoFar = false;
            }
#endif//0
        }
        if (! okSoFar)
        {
            freeobject(reinterpret_cast<t_object *>(xx));
            xx = NULL;
        }
    }
    return xx;
} // tcpClientCreate

/*------------------------------------ tcpClientFree ---*/
static void tcpClientFree(TcpObjectData * xx)
{
    if (xx)
    {
#if 0
        OSStatus result;
#endif//0
        
        xx->fClosing = true;
        if (xx->fSocket)
        {
            switch (xx->fState)
            {
                case kTcpStateConnected:
                    tcpClientDisconnect(xx, true);
                    /* Fall through */
                    
                case kTcpStateConnecting:
                    /* Fall through */
                    
                case kTcpStateBound:
#if 0
                    WRAP_OT_CALL(xx, result, "OTUnbind", OTUnbind(xx->fSocket))
                    if (kOTNoError != result)
                    {
                        REPORT_ERROR(xx, OUTPUT_PREFIX "OTUnbind failed (%ld = %s)", result)
                        reportEndpointState(OUR_NAME, xx);
                    }
#endif//0
                    /* Fall through */
                    
                case kTcpStateUnbound:
#if 0
                    WRAP_OT_CALL(xx, result, "OTCloseProvider", OTCloseProvider(xx->fSocket))
                    if (kOTNoError != result)
                    {
                        REPORT_ERROR(xx, OUTPUT_PREFIX "OTCloseProvider failed (%ld = %s)", result)
                        reportEndpointState(OUR_NAME, xx);
                    }
#endif//0
                    break;
                    
                default:
                    break;
                    
            }
        }
        releaseObjectMemory(OUR_NAME, xx);
    }
} // tcpClientFree

/*------------------------------------ main ---*/
int C74_EXPORT main(void)
{
    /* Allocate class memory and set up class. */
    t_class * temp = class_new(OUR_NAME, reinterpret_cast<method>(tcpClientCreate),
                               reinterpret_cast<method>(tcpClientFree),
                               static_cast<long>(sizeof(TcpObjectData)),
                               reinterpret_cast<method>(NULL), A_DEFSYM, A_DEFLONG, A_DEFLONG, 0);

    if (temp)
    {
        class_addmethod(temp, reinterpret_cast<method>(cmd_Anything), MESSAGE_ANYTHING, A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Assist), MESSAGE_ASSIST, A_CANT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Float), MESSAGE_FLOAT, A_FLOAT, 0);
        class_addmethod(temp, reinterpret_cast<method>(stdinletinfo), MESSAGE_INLETINFO, A_CANT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Int), MESSAGE_INT, A_LONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_List), MESSAGE_LIST, A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Connect), "connect", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Disconnect), "disconnect", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Mode), "mode", A_SYM, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Port), "port", A_LONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Self), "self", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Send), "send", A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Server), "server", A_SYM, 0);
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

#define CHECK_A_BYTE(endChar, sumPtr)    \
    { \
        for (accum = -1; okSoFar; ) \
        { \
            a_char = *nextChar++; \
            if (a_char == endChar) \
            { \
                if (0 > accum)\
                { \
                    okSoFar = false;\
                } \
                break; \
      \
            } \
            if (('0' <= a_char) && ('9' >= a_char)) \
            { \
                if (0 > accum)\
                { \
                    accum = (a_char - '0');\
                } \
                else \
                { \
                    accum *= 10; \
                    accum += (a_char - '0'); \
                    if (255 > accum)\
                    { \
                        okSoFar = false;\
                    } \
                } \
            } \
            else\
            { \
                okSoFar = false;\
            } \
        } \
        if (okSoFar)\
        { \
            sumPtr = accum;\
        } \
    }

/*------------------------------------ checkIpString ---*/
bool checkIpString(t_symbol * ipAddress,
                   short &    byte_0,
                   short &    byte_1,
                   short &    byte_2,
                   short &    byte_3)
{
    bool okSoFar = true;

    if (ipAddress != gEmptySymbol)
    {
        char * nextChar = ipAddress->s_name;
        short  accum;
        char   a_char;

        /* Get bytes. */
        CHECK_A_BYTE('.', byte_0)
        CHECK_A_BYTE('.', byte_1)
        CHECK_A_BYTE('.', byte_2)
        CHECK_A_BYTE(0, byte_3)
    }
    return okSoFar;
} // checkIpString

/*------------------------------------ tcpClientSetPort ---*/
bool tcpClientSetPort(TcpObjectData * xx,
                      const bool      bangOnError)
{
    bool okSoFar = true;

    if (xx)
    {
        switch (xx->fState)
        {
            case kTcpStateUnbound:
                break;

            case kTcpStateBound:
                break;

            case kTcpStateConnecting:
                LOG_ERROR_1(xx, OUTPUT_PREFIX "connecting")
                okSoFar = false;
                break;

            case kTcpStateConnected:
                LOG_ERROR_1(xx, OUTPUT_PREFIX "connection is active")
                okSoFar = false;
                break;

            default:
                break;
                
        }
        if ((! okSoFar) && bangOnError)
        {
            signalError(xx);
        }
    }
    return true;
} // tcpClientSetPort

/*------------------------------------ tcpClientSetServer ---*/
bool tcpClientSetServer(TcpObjectData * xx,
                        const short     byte_0,
                        const short     byte_1,
                        const short     byte_2,
                        const short     byte_3,
                        const bool      bangOnError)
{
    bool okSoFar = true;

    if (xx)
    {
        switch (xx->fState)
        {
            case kTcpStateUnbound:
                break;

            case kTcpStateBound:
                break;

            case kTcpStateConnecting:
                LOG_ERROR_1(xx, OUTPUT_PREFIX "connecting")
                okSoFar = false;
                break;

            case kTcpStateConnected:
                LOG_ERROR_1(xx, OUTPUT_PREFIX "connection is active")
                okSoFar = false;
                break;

            default:
                break;
                
        }
        if (okSoFar)
        {
            xx->fServerAddress = static_cast<unsigned long>(((byte_0 & 0x0FF) << 24) |
                                                            ((byte_1 & 0x0FF) << 16) |
                                                            ((byte_2 & 0x0FF) << 8) |
                                                            (byte_3 & 0x0FF));
        }
        if ((! okSoFar) && bangOnError)
        {
            signalError(xx);
        }
    }
    return okSoFar;
} // tcpClientSetServer

/*------------------------------------ tcpClientConnect ---*/
bool tcpClientConnect(TcpObjectData * xx)
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
                break;

            case kTcpStateConnecting:
                LOG_ERROR_1(xx, OUTPUT_PREFIX "already connecting")
                okSoFar = false;
                break;

            case kTcpStateConnected:
                LOG_ERROR_1(xx, OUTPUT_PREFIX "already connected")
                okSoFar = false;
                break;

            default:
                break;
                
        }
        if (okSoFar)
        {
#if 0
            OSStatus    result;
            TCall       snd_call;
            InetAddress in_addr;

            setObjectState(xx, kTcpStateConnecting);
            OTInitInetAddress(&in_addr, xx->fServerPort, xx->fServerAddress);
            OTMemzero(&snd_call, sizeof(snd_call));
            snd_call.addr.len = sizeof(in_addr);
            snd_call.addr.buf = reinterpret_cast<unsigned char *>(&in_addr);
            WRAP_OT_CALL(xx, result, "OTConnect", OTConnect(xx->fSocket, &snd_call, NULL))
            if (kOTNoDataError != result)
            {
                REPORT_ERROR(xx, OUTPUT_PREFIX "OTConnect failed (%ld = %s)", result)
                reportEndpointState(OUR_NAME, xx);
                okSoFar = false;
            }
#endif//0
        }
        if (! okSoFar)
        {
            signalError(xx);
        }
    }
    return okSoFar;
} // tcpClientConnect

/*------------------------------------ tcpClientDisconnect ---*/
bool tcpClientDisconnect(TcpObjectData * xx,
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

            case kTcpStateConnecting:
                LOG_ERROR_1(xx, OUTPUT_PREFIX "connecting")
                okSoFar = false;
                break;

            case kTcpStateConnected:
                break;

            default:
                break;
                
        }
        if (okSoFar || forced)
        {
#if 0
            OSStatus result;

            if ((! forced) && ((T_COTS_ORD == xx->fServiceType) ||
                               (T_TRANS_ORD == xx->fServiceType)))
            {
                WRAP_OT_CALL(xx, result, "OTSndOrderlyDisconnect",
                             OTSndOrderlyDisconnect(xx->fSocket))
                if (kOTNoDataErr == result)
                {
                    setObjectState(xx, kTcpStateDisconnecting);
                }
                else
                {
                    REPORT_ERROR(xx, OUTPUT_PREFIX "OTSndOrderlyDisconnect failed (%ld = %s)",
                                 result)
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
                WRAP_OT_CALL(xx, result, "OTSndDisconnect", OTSndDisconnect(xx->fSocket, &snd_call))
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
#endif//0
        }
        if (! okSoFar)
        {
            signalError(xx);
        }
    }
    return okSoFar;
} // tcpClientDisconnect
