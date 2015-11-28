/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       tcpServerLowLevel.cpp                                                   */
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
/*  Created:    2000/11/04                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "tcpServer.h"

/*------------------------------------ processRebindQueue ---*/
void processRebindQueue(TcpObjectData * xx)
{
    if (xx)
    {
#if 0
        short    prev_lock = lockout_set(1);
        OSStatus result = kOTNoError;

        if (kTcpStateBound == xx->fState)
        {
            WRAP_OT_CALL(xx, result, "OTUnbind", OTUnbind(xx->fSocket))
            if (kOTNoError != result)
            {
                REPORT_ERROR(xx, OUTPUT_PREFIX "OTUnbind failed (%ld = %s)", result)
                reportEndpointState(OUR_NAME, xx);
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
            WRAP_OT_CALL(xx, result, "OTBind", OTBind(xx->fSocket, &bind_request, NULL))
            if (kOTNoError != result)
            {
                REPORT_ERROR(xx, OUTPUT_PREFIX "OTBind failed (%ld = %s)", result)
                reportEndpointState(OUR_NAME, xx);
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
void processReceiveQueue(TcpObjectData * xx)
{
    if (xx)
    {
        short           prev_lock = lockout_set(1);
        TcpBufferLink * temp;
        t_atom *        gotStuff;
        long            numAtoms;
        long            numMessages;

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
            numMessages = validateBuffer(xx, OUR_NAME, temp->fData, xx->fRawMode);
            for (long ii = 0; ii < numMessages; ++ii)
            {
                /* Allow interrupts while we process the buffer */
                lockout_set(prev_lock);
                numAtoms = 0;
                gotStuff = convertBufferToAtoms(xx, OUR_NAME, &walker, numAtoms, 0,
                                                temp->fData->fNumBytesInUse, xx->fRawMode);
                prev_lock = lockout_set(1);
                if (0 < numAtoms)
                {
                    outlet_anything(xx->fResultOut, gReplySymbol, numAtoms, gotStuff);
                    FREE_BYTES(gotStuff);
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

/*------------------------------------ makeReceiveBufferAvailable ---*/
bool makeReceiveBufferAvailable(TcpObjectData * xx)
{
    bool okSoFar = true;

    if (xx)
    {
#if 0
        OSStatus err;
        OTFlags  flags;

        WRAP_OT_CALL(xx, err, "OTRcv", OTRcv(xx->fSocket,
                                             &xx->fReceiveBuffer->fNumElements,
                                             MAX_BUFFER_TO_RECEIVE, &flags))
        if (0 > err)
        {
            REPORT_ERROR(xx, OUTPUT_PREFIX "OTRcv failed (%ld = %s)", err)
            reportEndpointState(OUR_NAME, xx);
            okSoFar = false;
        }
        else
        {
            /* Get the next available buffer from the buffer pool */
            TcpBufferLink * temp = xx->fPoolHead;

            xx->fReceiveBuffer->fNumBytesInUse = err;
            if (temp)
            {
                DataBufferPtr swapper = temp->fData;

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
                temp->fData = xx->fReceiveBuffer;
                xx->fReceiveBuffer = swapper;
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

/*------------------------------------ setObjectState ---*/
void setObjectState(TcpObjectData * xx,
                    const TcpState  newState)
{
    if (xx)
    {
        xx->fState = newState;
#if defined(BE_VERBOSE)
        if (xx->fVerbose)
        {
            LOG_POST_2(xx, OUTPUT_PREFIX "--> %s", mapStateToSymbol(newState)->s_name)
        }
#endif /* BE_VERBOSE */
    }
} // setObjectState

/*------------------------------------ transmitBuffer ---*/
void transmitBuffer(TcpObjectData * xx,
                    DataBuffer *    aBuffer)
{
    if (xx && aBuffer)
    {
#if 0
        OSStatus result;

        if (xx->fRawMode)
        {
            WRAP_OT_CALL(xx, result, "OTSnd", OTSnd(xx->fSocket, &aBuffer->fElements,
                                                    aBuffer->fNumBytesInUse, 0L))
        }
        else
        {
            short numBytes = aBuffer->fNumBytesInUse + SIZEOF_DATABUFFER_HDR;

            aBuffer->fSanityCheck = htons(static_cast<short>(-(numBytes + aBuffer->fNumElements)));
            aBuffer->fNumElements = htons(aBuffer->fNumElements);
            WRAP_OT_CALL(xx, result, "OTSnd", OTSnd(xx->fSocket, &aBuffer->fNumElements, numBytes,
                                                    0L))
        }
        if (0 > result)
        {
            REPORT_ERROR(xx, OUTPUT_PREFIX "OTSnd failed (%ld = %s)", result)
            reportEndpointState(OUR_NAME, xx);
        }
#endif//0
    }
} // transmitBuffer
