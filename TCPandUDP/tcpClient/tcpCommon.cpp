/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       tcpCommon.cpp                                                           */
/*                                                                                      */
/*  Contains:   The common utility routines.                                            */
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
/*  Created:    1998/09/28                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "tcpCommon.h"

/*------------------------------------ processErrorQueue ---*/
static void
processErrorQueue(TcpObjectData * xx)
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
bool
initObject(const char *    name,
           TcpObjectData * xx,
           const long      port,
           const long      numBuffers)
{
    bool okSoFar = true;

    if (xx)
    {
        long buffSize = static_cast<long>(BUFF_MEMORY_TO_ALLOC * (numBuffers + 2));

        xx->fServerPort = static_cast<unsigned short>(port ? port : DEFAULT_PORT);
        xx->fClientAddress = 0L;
        xx->fClientPort = 0;
        xx->fErrorBangOut = static_cast<t_outlet *>(bangout(xx));
        xx->fResultOut = static_cast<t_outlet *>(outlet_new(xx, NULL));
        setObjectState(xx, kTcpStateUnbound);
        xx->fSocket = NULL;
        xx->fErrorQueue = MAKE_QELEM(xx, processErrorQueue);
        xx->fRebindQueue = MAKE_QELEM(xx, processRebindQueue);
        xx->fReceiveQueue = MAKE_QELEM(xx, processReceiveQueue);
        xx->fBufferBase = reinterpret_cast<DataBuffer **>(sysmem_newhandle(buffSize));
        if (xx->fBufferBase)
        {
            sysmem_lockhandle(reinterpret_cast<t_handle>(xx->fBufferBase), 1);
            xx->fSendBuffer = *xx->fBufferBase;
            xx->fReceiveBuffer = reinterpret_cast<DataBuffer *>(ADD_TO_ADDRESS(xx->fSendBuffer,
                                                                           BUFF_MEMORY_TO_ALLOC));
        }
        xx->fLinkBase = MAKE_TYPED_HANDLE(TcpBufferLink, numBuffers);
        if (xx->fLinkBase)
        {
            DataBuffer *    this_buffer =
                            reinterpret_cast<DataBuffer *>(ADD_TO_ADDRESS(xx->fReceiveBuffer,
                                                                          BUFF_MEMORY_TO_ALLOC));
            TcpBufferLink * prev_link = NULL;
            TcpBufferLink * this_link = NULL;

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
                this_link = reinterpret_cast<TcpBufferLink *>(ADD_TO_ADDRESS(this_link,
                                                                             sizeof(TcpBufferLink)));
            }
            xx->fPoolTail = prev_link;
        }
        xx->fClosing = xx->fRawMode = false;
        if (! (xx->fResultOut && xx->fErrorBangOut && xx->fErrorQueue && xx->fBufferBase &&
               xx->fRebindQueue && xx->fReceiveQueue && xx->fLinkBase))
        {
            LOG_ERROR_2(xx, "%s: unable to create port or buffer for object", name)
            okSoFar = false;
        }
    }
    return okSoFar;
} // initObject

/*------------------------------------ presetObjectPointers ---*/
void
presetObjectPointers(TcpObjectData * xx)
{
    if (xx)
    {
        xx->fErrorBangOut = xx->fResultOut = NULL;
        xx->fErrorQueue = xx->fRebindQueue = xx->fReceiveQueue = NULL;
        xx->fSendBuffer = xx->fReceiveBuffer = NULL;
        xx->fBufferBase = NULL;
        xx->fLinkBase = NULL;
        xx->fSocket = NULL;
        xx->fPartnerName = xx->fSelfName = NULL;
        xx->fReceiveHead = xx->fReceiveTail = xx->fPoolHead = xx->fPoolTail = NULL;
    }
} // presetObjectPointers

/*------------------------------------ releaseObjectMemory ---*/
void
releaseObjectMemory(const char *    name,
                    TcpObjectData * xx)
{
#pragma unused(name)
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
        if (xx->fRebindQueue)
        {
            qelem_unset(xx->fRebindQueue);
            qelem_free(xx->fRebindQueue);
            xx->fRebindQueue = NULL;
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

/*------------------------------------ reportEndpointState ---*/
void
reportEndpointState(const char *    name,
                    TcpObjectData * xx)
{
    if (xx)
    {
#if 0
        LOG_ERROR_3(xx, "%s: Endpoint state: %s", name, describeEndpointState(xx->fSocket));
#endif//0
    }
} // reportEndpointState
