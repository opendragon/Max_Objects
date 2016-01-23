/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       udpPortLowLevel.cpp                                                     */
/*                                                                                      */
/*  Contains:   The common utility routines.                                            */
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

#include "udpPort.h"

/*------------------------------------ makeReceiveBufferAvailable ---*/
bool
makeReceiveBufferAvailable(UdpObjectData * xx)
{
    bool okSoFar = true;

    if (xx)
    {
        int       sock = CFSocketGetNative(xx->fSocket);
        socklen_t addrLen = sizeof(xx->fReceiveBuffer->fSender);
        ssize_t   bytesRead = recvfrom(sock,
                                       reinterpret_cast<UInt8 *>(&xx->fReceiveBuffer->fNumElements),
                                       MAX_BUFFER_TO_RECEIVE, 0,
                                       reinterpret_cast<sockaddr *>(&xx->fReceiveBuffer->fSender),
                                       &addrLen);

        if (0 > bytesRead)
        {
            LOG_ERROR_2(xx, OUTPUT_PREFIX "recvfrom failed (%ld)", static_cast<long>(errno))
            signalError(xx);
            okSoFar = false;
        }
        else if (0 == bytesRead)
        {
            LOG_ERROR_2(xx, OUTPUT_PREFIX "recvfrom failed (%ld)", static_cast<long>(EPIPE))
            signalError(xx);
            okSoFar = false;
        }
        else
        {
            /* Get the next available buffer from the buffer pool */
            UdpBufferLink * temp = xx->fPoolHead;

            xx->fReceiveBuffer->fNumBytesInUse = bytesRead;
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
    }
    return okSoFar;
} // makeReceiveBufferAvailable

/*------------------------------------ processReceiveQueue ---*/
void
processReceiveQueue(UdpObjectData * xx)
{
    if (xx)
    {
        short           prev_lock = lockout_set(1);
        UdpBufferLink * temp;
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
            numMessages = validateBuffer(xx, temp->fData, xx->fRawMode);
            for (long ii = 0; ii < numMessages; ++ii)
            {
                /* Allow interrupts while we process the buffer */
                lockout_set(prev_lock);
                numAtoms = 0;
                gotStuff = convertBufferToAtoms(xx, &walker, temp->fData->fSender, numAtoms,
                                                temp->fData->fNumBytesInUse, xx->fRawMode);
                prev_lock = lockout_set(1);
                if (0 < numAtoms)
                {
                    outlet_anything(xx->fResultOut, gFromSymbol, numAtoms, gotStuff);
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

/*------------------------------------ setObjectState ---*/
void
setObjectState(UdpObjectData * xx,
               const UdpState  newState)
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
void
transmitBuffer(UdpObjectData * xx,
               DataBuffer *    aBuffer)
{
    if (xx && aBuffer && xx->fSocket)
    {
        int sock = CFSocketGetNative(xx->fSocket);

        if (0 <= sock)
        {
            sockaddr_in outAddress;
            void *      outBuffer;
            size_t      outLength;

            memset(&outAddress, 0, sizeof(outAddress));
            outAddress.sin_family = AF_INET;
            outAddress.sin_port = htons(xx->fPartnerPort);
            memcpy(&outAddress.sin_addr, &xx->fPartnerAddress, sizeof(outAddress.sin_addr));
            if (xx->fRawMode)
            {
                outLength = aBuffer->fNumBytesInUse;
                outBuffer = &aBuffer->fElements;
            }
            else
            {
                long numBytes = aBuffer->fNumBytesInUse + SIZEOF_DATABUFFER_HDR;

                aBuffer->fSanityCheck = htons(static_cast<short>(-(numBytes +
                                                                   aBuffer->fNumElements)));
                aBuffer->fNumElements = htons(aBuffer->fNumElements);
                outLength = numBytes;
                outBuffer = &aBuffer->fNumElements;
            }
            ssize_t bytesWritten = sendto(sock, outBuffer, outLength, 0,
                                          reinterpret_cast<sockaddr *>(&outAddress),
                                          static_cast<socklen_t>(sizeof(outAddress)));

            if (0 > bytesWritten)
            {
                LOG_ERROR_2(xx, OUTPUT_PREFIX "sendto failed (%ld)", static_cast<long>(errno))
                signalError(xx);
            }
            else if (0 == bytesWritten)
            {
                LOG_ERROR_2(xx, OUTPUT_PREFIX "sendto failed (%ld)", static_cast<long>(EPIPE))
                signalError(xx);
            }
        }
        else
        {
            LOG_ERROR_2(xx, OUTPUT_PREFIX "Could not get native socket (%ld)",
                        static_cast<long>(errno))
            signalError(xx);
        }
    }
} // transmitBuffer
