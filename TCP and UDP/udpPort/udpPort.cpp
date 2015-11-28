/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       udpPort.cpp                                                             */
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
#include "reportVersion.h"
#include <fcntl.h>

/*------------------------------------ udpPortCreate ---*/
static void * udpPortCreate(const long port,
                            const long numBuffers)
{
    UdpObjectData * xx = static_cast<UdpObjectData *>(object_alloc(gClass));
    
    if (xx)
    {
        bool okSoFar = true;
        long actBuffCount;
        
#if defined(BE_VERBOSE)
        xx->fVerbose = false;
#endif /* BE_VERBOSE */
        presetObjectPointers(xx);
        if ((0 > port) || (MAX_PORT < port) || (0 > numBuffers))
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
            okSoFar = initObject(xx, port, actBuffCount);
        }
        if (okSoFar)
        {
            udpPortSetPort(xx, false);
        }
        else
        {
            freeobject(reinterpret_cast<t_object *>(xx));
            xx = NULL;
        }
    }
    return xx;
} // udpPortCreate

/*------------------------------------ udpPortFree ---*/
static void udpPortFree(UdpObjectData * xx)
{
    if (xx)
    {
        xx->fClosing = true;
        if (xx->fSocket)
        {
            CFSocketInvalidate(xx->fSocket);
            CFRelease(xx->fSocket);
            xx->fSocket = NULL;
        }
        releaseObjectMemory(xx);
    }
} // udpPortFree

/*------------------------------------ main ---*/
int main(void)
{
    /* Allocate class memory and set up class. */
    t_class * temp = class_new(OUR_NAME, reinterpret_cast<method>(udpPortCreate),
                               reinterpret_cast<method>(udpPortFree), sizeof(UdpObjectData),
                               reinterpret_cast<method>(0L), A_DEFLONG, A_DEFLONG, 0);

    if (temp)
    {
        class_addmethod(temp, reinterpret_cast<method>(cmd_Anything), MESSAGE_ANYTHING, A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Assist), MESSAGE_ASSIST, A_CANT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Float), MESSAGE_FLOAT, A_FLOAT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Int), MESSAGE_INT, A_LONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_List), MESSAGE_LIST, A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Mode), "mode", A_SYM, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Port), "port", A_LONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Self), "self", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Send), "send", A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_SendTo), "sendTo", A_SYM, A_LONG, 0);
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
    gFromSymbol = gensym("from");
    gMaxSymbol = gensym("max");
    gOffSymbol = gensym("off");
    gOnSymbol = gensym("on");
    gRawSymbol = gensym("raw");
    gSelfSymbol = gensym("self");
    gStatusSymbol = gensym("status");
    setUpStateSymbols();
    reportVersion(OUR_NAME);
    return 0;
} // main

/*------------------------------------ processErrorQueue ---*/
static void processErrorQueue(UdpObjectData * xx)
{
    if (xx)
    {
        short prev_lock = lockout_set(1);

        outlet_bang(xx->fErrorBangOut);
        lockout_set(prev_lock);
#if USE_EVNUM
        evnum_incr();
#endif /* USE_EVNUM */
    }
} // processErrorQueue

/*------------------------------------ initObject ---*/
bool initObject(UdpObjectData * xx,
                const long      port,
                const long      numBuffers)
{
    bool okSoFar = true;

    if (xx)
    {
        long buffSize = static_cast<long>(BUFF_MEMORY_TO_ALLOC * (numBuffers + 2));

        xx->fSelfPort = static_cast<unsigned short>(port ? port : DEFAULT_PORT);
        memset(&xx->fPartnerAddress, 0, sizeof(xx->fPartnerAddress));
        xx->fPartnerPort = 0;
        xx->fPartnerKnown = false;
        xx->fErrorBangOut = static_cast<t_outlet *>(bangout(xx));
        xx->fResultOut = static_cast<t_outlet *>(outlet_new(xx, NULL));
        setObjectState(xx, kUdpStateUnbound);
        xx->fSocket = NULL;
        xx->fErrorQueue = MAKE_QELEM(xx, processErrorQueue);
        xx->fReceiveQueue = MAKE_QELEM(xx, processReceiveQueue);
        xx->fBufferBase = reinterpret_cast<DataBuffer **>(sysmem_newhandle(buffSize));
        if (xx->fBufferBase)
        {
            sysmem_lockhandle(reinterpret_cast<t_handle>(xx->fBufferBase), 1);
            xx->fSendBuffer = *xx->fBufferBase;
            xx->fReceiveBuffer = reinterpret_cast<DataBuffer *>(ADD_TO_ADDRESS(xx->fSendBuffer,
                                                                           BUFF_MEMORY_TO_ALLOC));
        }
        xx->fLinkBase = MAKE_TYPED_HANDLE(UdpBufferLink, numBuffers);
        if (xx->fLinkBase)
        {
            DataBuffer *    this_buffer =
                                reinterpret_cast<DataBuffer *>(ADD_TO_ADDRESS(xx->fReceiveBuffer,
                                                               BUFF_MEMORY_TO_ALLOC));
            UdpBufferLink * prev_link = NULL;
            UdpBufferLink * this_link = NULL;

            sysmem_lockhandle(reinterpret_cast<t_handle>(xx->fLinkBase), 1);
            xx->fPoolHead = *xx->fLinkBase;
            this_link = xx->fPoolHead;
            for (long link_count = 0; link_count < numBuffers; ++link_count)
            {
                this_link->fPrevious = prev_link;
                this_link->fData = this_buffer;
                this_buffer = reinterpret_cast<DataBuffer *>(ADD_TO_ADDRESS(this_buffer,
                                                                            BUFF_MEMORY_TO_ALLOC));
                this_link->fNext = NULL;
                if (prev_link)
                {
                    prev_link->fNext = this_link;
                }
                prev_link = this_link;
                this_link = reinterpret_cast<UdpBufferLink *>(ADD_TO_ADDRESS(this_link,
                                                                             sizeof(UdpBufferLink)));
            }
            xx->fPoolTail = prev_link;
        }
        xx->fClosing = xx->fRawMode = false;
        if (! (xx->fResultOut && xx->fErrorBangOut && xx->fErrorQueue && xx->fBufferBase &&
               xx->fReceiveQueue && xx->fLinkBase))
        {
            LOG_ERROR_1(xx, OUTPUT_PREFIX "unable to create port or buffer for object")
            okSoFar = false;
        }
    }
    return okSoFar;
} // initObject

/*------------------------------------ mapStateToSymbol ---*/
t_symbol * mapStateToSymbol(const UdpState aState)
{
    t_symbol * result;

    switch (aState)
    {
        case kUdpStateUnbound:
            result = gUnboundSymbol;
            break;

        case kUdpStateBound:
            result = gBoundSymbol;
            break;

        default:
            result = gUnknownSymbol;
            break;
            
    }
    return result;
} // mapStateToSymbol

/*------------------------------------ presetObjectPointers ---*/
void presetObjectPointers(UdpObjectData * xx)
{
    if (xx)
    {
        xx->fErrorBangOut = xx->fResultOut = NULL;
        xx->fErrorQueue = xx->fReceiveQueue = NULL;
        xx->fSendBuffer = xx->fReceiveBuffer = NULL;
        xx->fBufferBase = NULL;
        xx->fLinkBase = NULL;
        xx->fSocket = NULL;
        xx->fSelfName = NULL;
        xx->fReceiveHead = xx->fReceiveTail = xx->fPoolHead = xx->fPoolTail = NULL;
    }
} // presetObjectPointers

/*------------------------------------ releaseObjectMemory ---*/
void releaseObjectMemory(UdpObjectData * xx)
{
    if (xx)
    {
        if (xx->fErrorQueue)
        {
            qelem_unset(xx->fErrorQueue);
            qelem_free(xx->fErrorQueue);
            xx->fErrorQueue = NULL;
        }
        if (xx->fReceiveQueue)
        {
            qelem_unset(xx->fReceiveQueue);
            qelem_free(xx->fReceiveQueue);
            xx->fReceiveQueue = NULL;
        }
        xx->fReceiveBuffer = xx->fSendBuffer = NULL;
        if (xx->fBufferBase)
        {
            sysmem_lockhandle(reinterpret_cast<t_handle>(xx->fBufferBase), 0);
            sysmem_freehandle(reinterpret_cast<t_handle>(xx->fBufferBase));
            xx->fBufferBase = NULL;
        }
        xx->fReceiveHead = xx->fReceiveTail = xx->fPoolHead = xx->fPoolTail = NULL;
        if (xx->fLinkBase)
        {
            sysmem_lockhandle(reinterpret_cast<t_handle>(xx->fLinkBase), 0);
            sysmem_freehandle(reinterpret_cast<t_handle>(xx->fLinkBase));
            xx->fLinkBase = NULL;
        }
    }
} // releaseObjectMemory

/*------------------------------------ setUpStateSymbols ---*/
void setUpStateSymbols(void)
{
    gBoundSymbol = gensym("bound");
    gUnboundSymbol = gensym("unbound");
    gUnknownSymbol = gensym("unknown");
} // setUpStateSymbols

/*------------------------------------ socketReadCallback ---*/
static void socketReadCallback(CFSocketRef          sr,
                               CFSocketCallBackType type,
                               CFDataRef            address,
                               const void *         data,
                               void *               info)
{
    UdpObjectData * xx = reinterpret_cast<UdpObjectData *>(info);

    if (! makeReceiveBufferAvailable(xx))
    {
        signalError(xx);
    }
} // socketReadCallback

/*------------------------------------ udpPortSetPort ---*/
bool udpPortSetPort(UdpObjectData * xx,
                    const bool      bangOnError)
{
    bool okSoFar = true;

    if (xx)
    {
        if (xx->fSocket)
        {
            CFSocketInvalidate(xx->fSocket);
            CFRelease(xx->fSocket);
            xx->fSocket = NULL;
        }
        int err;
        int sock = socket(AF_INET, SOCK_DGRAM, 0);

        if (0 > sock)
        {
            err = errno;
            okSoFar = false;
        }
        else
        {
            err = 0;
        }
        if (okSoFar)
        {
            sockaddr_in addr;

            memset(&addr, 0, sizeof(addr));
            addr.sin_len = sizeof(addr);
            addr.sin_family = AF_INET;
            addr.sin_port = htons(xx->fSelfPort);
            addr.sin_addr.s_addr = INADDR_ANY;
            err = bind(sock, reinterpret_cast<sockaddr *>(&addr), sizeof(addr));
            if (0 > err)
            {
                err = errno;
                okSoFar = false;
            }
        }
        if (okSoFar)
        {
            int flags = fcntl(sock, F_GETFL);

            err = fcntl(sock, F_SETFL, flags | O_NONBLOCK);
            if (0 > err)
            {
                err = errno;
                okSoFar = false;
            }
        }
        if (okSoFar)
        {
            const CFSocketContext context = { 0, xx, NULL, NULL, NULL };

            xx->fSocket = CFSocketCreateWithNative(kCFAllocatorDefault, sock, kCFSocketReadCallBack,
                                                   socketReadCallback, &context);
            sock = -1;
            CFRunLoopSourceRef rls = CFSocketCreateRunLoopSource(kCFAllocatorDefault, xx->fSocket,
                                                                 0);

            CFRunLoopAddSource(CFRunLoopGetCurrent(), rls, kCFRunLoopDefaultMode);
            CFRelease(rls);
            setObjectState(xx, kUdpStateBound);
        }
        if (! okSoFar)
        {
            close(sock);
            LOG_ERROR_2(xx, OUTPUT_PREFIX "set port failed (%ld)", static_cast<long>(err))
            if (bangOnError)
            {
                signalError(xx);
            }
        }
    }
    return okSoFar;
} // udpPortSetPort
