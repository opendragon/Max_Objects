/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       tcpMultiServerLowLevel.cpp                                              */
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

#include "tcpMultiServer.h"

/*------------------------------------ processErrorQueue ---*/
static void processErrorQueue(TcpMultiServerData * xx)
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

/*------------------------------------ processRebindQueue ---*/
static void processRebindQueue(TcpMultiServerData * xx)
{
    if (xx)
    {
#if 0
        short    prev_lock = lockout_set(1);
        OSStatus result = kOTNoError;

        if (kTcpStateBound == xx->fState)
        {
            WRAP_OT_CALL(xx, result, "OTUnbind", OTUnbind(xx->fListenEndpoint))
            if (result != kOTNoError)
            {
                REPORT_ERROR(xx, OUTPUT_PREFIX "OTUnbind failed (%ld = %s)", result)
                reportEndpointState(xx, xx->fListenEndpoint);
                signalError(xx);
            }
        }
        else
        {
            TBind       bind_request;
            InetAddress in_address;

            OTInitInetAddress(&in_address, xx->fServerPort, xx->fServerAddress);
            bind_request.addr.len = sizeof(in_address);
            bind_request.addr.buf = reinterpret_cast<unsigned char *>(&in_address);
            bind_request.qlen = 1;
            WRAP_OT_CALL(xx, result, "OTBind", OTBind(xx->fListenEndpoint, &bind_request,
                                                      NULL))
            if (result != kOTNoError)
            {
                REPORT_ERROR(xx, OUTPUT_PREFIX "OTBind failed (%ld = %s)", result)
                reportEndpointState(xx, xx->fListenEndpoint);
                signalError(xx);
            }
        }
        lockout_set(prev_lock);
#if USE_EVNUM
        evnum_incr();
#endif /* USE_EVNUM */
#endif//0
    }
} // processRebindQueue

/*------------------------------------ processReceiveQueue ---*/
static void processReceiveQueue(TcpMultiServerData * xx)
{
    if (xx)
    {
        short           prev_lock = lockout_set(1);
        TcpBufferLink * temp;
        t_atom *        got_stuff;
        short           numAtoms;
        short           numMessages;

        for ( ; ; )
        {
            char * walker;

            /* Is the received list empty? */
            temp = xx->fReceiveHead;
            if (! temp)
            {
                break;
            }

            /* Grab the head of the received list */
            if (temp->fNext)
            {
                temp->fNext->fPrevious = NULL;
            }
            xx->fReceiveHead = temp->fNext;
            if (! xx->fReceiveHead)
            {
                xx->fReceiveTail = NULL;
            }
            temp->fNext = NULL;
            walker = reinterpret_cast<char *>(&temp->fData->fNumElements);
            numMessages = validateBuffer(xx, OUR_NAME, temp->fData, temp->fRawMode);
            for (short ii = 0; ii < numMessages; ++ii)
            {
                /* Allow interrupts while we process the buffer */
                lockout_set(prev_lock);
                numAtoms = 0;
                got_stuff = convertBufferToAtoms(xx, OUR_NAME, &walker, numAtoms, temp->fIdentifier,
                                                 temp->fData->fNumBytesInUse, temp->fRawMode);
                prev_lock = lockout_set(1);
                if (numAtoms > 0)
                {
                    outlet_anything(xx->fResultOut, gReplySymbol, numAtoms, got_stuff);
                    FREE_BYTES(got_stuff);
                }
            }
            /* Add the temp link to the buffer pool */
            temp->fPrevious = xx->fPoolTail;
            if (temp->fPrevious)
            {
                temp->fPrevious->fNext = temp;
            }
            else
            {
                xx->fPoolHead = temp;
            }
            xx->fPoolTail = temp;
        }
        lockout_set(prev_lock);
#if USE_EVNUM
        evnum_incr();
#endif /* USE_EVNUM */
    }
} // processReceiveQueue

/*------------------------------------ initObject ---*/
bool initObject(TcpMultiServerData * xx,
                const long           port,
                const long           numBuffers)
{
    bool okSoFar = true;

    if (xx)
    {
#if 0
        long buffSize = static_cast<long>((numBuffers + (xx->fMaximumConnections * 2)) *
                                          BUFF_MEMORY_TO_ALLOC);
        
        xx->fServerPort = static_cast<unsigned short>(port ? port : DEFAULT_PORT);
        xx->fErrorBangOut = static_cast<t_outlet *>(bangout(xx));
        xx->fResultOut = static_cast<t_outlet *>(outlet_new(xx, NULL));
        setObjectState(xx, kTcpStateUnbound);
        xx->fListenEndpoint = kOTInvalidEndpointRef;
        xx->fErrorQueue = MAKE_QELEM(xx, processErrorQueue);
        xx->fRebindQueue = MAKE_QELEM(xx, processRebindQueue);
        xx->fReceiveQueue = MAKE_QELEM(xx, processReceiveQueue);
        xx->fBufferBase = reinterpret_cast<DataBuffer **>(sysmem_newhandle(buffSize));
        if (xx->fBufferBase)
        {
            sysmem_lockhandle(reinterpret_cast<t_handle>(xx->fBufferBase), 1);
        }
        xx->fLinkBase = MAKE_TYPED_HANDLE(TcpBufferLink, numBuffers);
        if (xx->fLinkBase)
        {
            // Offset by the number of buffers needed for the receive/send buffers dedicated to each connection:
            DataBuffer *    this_buffer =
                                    reinterpret_cast<DataBuffer *>(ADD_TO_ADDRESS(*xx->fBufferBase,
                                                                    xx->fMaximumConnections * 2 *
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
        xx->fClosing = false;
#if defined(BE_VERBOSE)
        xx->fVerbose = false;
#endif /* BE_VERBOSE */
        if (! (xx->fResultOut &&  xx->fErrorBangOut && xx->fErrorQueue && xx->fRebindQueue &&
               xx->fReceiveQueue && xx->fBufferBase && xx->fLinkBase))
        {
            LOG_ERROR_1(xx, OUTPUT_PREFIX "unable to create port or buffer for object")
            okSoFar = false;
        }
#endif//0
    }
    return okSoFar;
} // initObject

/*------------------------------------ makeReceiveBufferAvailable ---*/
bool makeReceiveBufferAvailable(TcpConnectionData * connection)
{
    bool okSoFar = true;

    if (connection)
    {
#if 0
        TcpMultiServerData * xx = connection->fOwner;
        OSStatus             err;
        OTFlags              flags;

        WRAP_OT_CALL(connection->fOwner, err, "OTRcv", OTRcv(connection->fDataEndpoint,
                                                         &connection->fReceiveBuffer->fNumElements,
                                                             MAX_BUFFER_TO_RECEIVE, &flags))
        if (0 > err)
        {
            REPORT_ERROR(xx, OUTPUT_PREFIX "OTRcv failed (%ld = %s)", err)
            reportEndpointState(xx, connection->fDataEndpoint);
            okSoFar = false;
        }
        else
        {
            /* Get the next available buffer from the buffer pool */
            TcpBufferLink * temp = xx->fPoolHead;

            connection->fReceiveBuffer->fNumBytesInUse = static_cast<short>(err);
            if (temp)
            {
                DataBuffer * swapper = temp->fData;

                if (temp->fNext)
                {
                    temp->fNext->fPrevious = NULL;
                }
                xx->fPoolHead = temp->fNext;
                if (! xx->fPoolHead)
                {
                    xx->fPoolTail = NULL;
                }
                temp->fNext = NULL;
                /* Exchange the receive buffer and the pool buffer */
                temp->fData = connection->fReceiveBuffer;
                connection->fReceiveBuffer = swapper;
                /* Add the pool buffer to the receive list */
                temp->fPrevious = xx->fReceiveTail;
                if (temp->fPrevious)
                {
                    temp->fPrevious->fNext = temp;
                }
                else
                {
                    xx->fReceiveHead = temp;
                }
                xx->fReceiveTail = temp;
                temp->fIdentifier = connection->fIdentifier;
                temp->fRawMode = connection->fRawMode;
            }
            else
            {
                LOG_ERROR_1(xx, OUTPUT_PREFIX "receive buffer queue exhausted")
                okSoFar = false;
            }
            signalReceive(xx);
        }
#endif//0
    }
    return okSoFar;
} // makeReceiveBufferAvailable

/*------------------------------------ presetObjectPointers ---*/
void presetObjectPointers(TcpMultiServerData * xx)
{
    if (xx)
    {
#if 0
        xx->fErrorBangOut = xx->fResultOut = NULL;
        xx->fErrorQueue = xx->fRebindQueue = xx->fReceiveQueue = NULL;
        xx->fListenEndpoint = kOTInvalidEndpointRef;
        xx->fAccessControl = NULL;
        xx->fBufferBase = NULL;
        xx->fLinkBase = NULL;
        xx->fSelfName = NULL;
        xx->fReceiveHead = xx->fReceiveTail = xx->fPoolHead = xx->fPoolTail = NULL;
        xx->fConnections = NULL;
        xx->fConnectionBase = NULL;
#endif//0
    }
} // presetObjectPointers

/*------------------------------------ releaseObjectMemory ---*/
void releaseObjectMemory(TcpMultiServerData * xx)
{
    if (xx)
    {
#if 0
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
        if (xx->fConnectionBase)
        {
            OSStatus            result;
            TcpConnectionData * conn_walker = *xx->fConnectionBase;

            for (unsigned short ii = 0; ii < xx->fMaximumConnections; ++ii, ++conn_walker)
            {
                // Do any special clean-up here. TBD
                if (conn_walker->fDataEndpoint != kOTInvalidEndpointRef)
                {
                    WRAP_OT_CALL(xx, result, "OTCloseProvider",
                                 OTCloseProvider(conn_walker->fDataEndpoint))
                    if (kOTNoError == result)
                    {
                        conn_walker->fDataEndpoint = kOTInvalidEndpointRef;
                    }
                    else
                    {
                        REPORT_ERROR(xx, OUTPUT_PREFIX "OTCloseProvider failed (%ld = %s)", result)
                        reportEndpointState(xx, conn_walker->fDataEndpoint);
                    }
                }
            }
            sysmem_lockhandle(reinterpret_cast<t_handle>(xx->fConnectionBase), 0);
            sysmem_freehandle(reinterpret_cast<t_handle>(xx->fConnectionBase));
            xx->fConnectionBase = NULL;
        }
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
        FREE_BYTES(xx->fConnections);
#endif//0
    }
} // releaseObjectMemory

#if 0
/*------------------------------------ reportEndpointState ---*/
void reportEndpointState(TcpMultiServerData * xx,
                         EndpointRef          endpoint)
{
    if (xx)
    {
        LOG_ERROR_2(xx, OUTPUT_PREFIX "Endpoint state: %s", describeEndpointState(endpoint));
    }
} // reportEndpointState
#endif//0

/*------------------------------------ setConnectionState ---*/
void setConnectionState(TcpConnectionData * connection,
                        const TcpState      newState)
{
    if (connection)
    {
#if defined(BE_VERBOSE)
        TcpMultiServerData * xx = connection->fOwner;
#endif /* BE_VERBOSE */

        connection->fState = newState;
#if defined(BE_VERBOSE)
        if (xx && xx->fVerbose)
        {
            LOG_POST_3(xx, OUTPUT_PREFIX "--> %s/%d", mapStateToSymbol(newState)->s_name,
                       connection->fIdentifier)
        }
#endif /* BE_VERBOSE */
    }
} // setConnectionState

/*------------------------------------ setObjectState ---*/
void setObjectState(TcpMultiServerData * xx,
                    const TcpState       newState)
{
    if (xx)
    {
        xx->fState = newState;
#if defined(BE_VERBOSE)
        if (xx->fVerbose)
        {
            LOG_POST_2(xx, OUTPUT_PREFIX "--> %s/0", mapStateToSymbol(newState)->s_name)
        }
#endif /* BE_VERBOSE */
    }
} // setObjectState

#if 0
/*------------------------------------ transmitBuffer ---*/
void transmitBuffer(TcpMultiServerData * xx,
                    EndpointRef          out,
                    DataBuffer *         aBuffer,
                    const bool           rawMode)
{
    if (xx && aBuffer)
    {
        OSStatus result;

        if (rawMode)
        {
            WRAP_OT_CALL(xx, result, "OTSnd", OTSnd(out, &aBuffer->fElements,
                                                    aBuffer->fNumBytesInUse, 0L))
        }
        else
        {
            short num_bytes = static_cast<short>(aBuffer->fNumBytesInUse + SIZEOF_DATABUFFER_HDR);

            aBuffer->fSanityCheck = htons(static_cast<short>(-(num_bytes + aBuffer->fNumElements)));
            aBuffer->fNumElements = htons(aBuffer->fNumElements);
            WRAP_OT_CALL(xx, result, "OTSnd", OTSnd(out, &aBuffer->fNumElements, num_bytes, 0L))
        }
        if (0 > result)
        {
            REPORT_ERROR(xx, OUTPUT_PREFIX "OTSnd failed (%ld = %s)", result)
            reportEndpointState(xx, out);
        }
    }
} // transmitBuffer
#endif//0
