/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       tcpServerLowLevel.c                                                     */
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
Pvoid processRebindQueue
  (TcpObjectPtr xx)
{
  EnterCallback();
  if (xx)
  {
#if OPEN_TRANSPORT_SUPPORTED
    short    prev_lock = lockout_set(1);
    OSStatus result = kOTNoError;

    if (xx->fState == TCP_OBJECT_BOUND)
    {
      WRAP_OT_CALL(xx, result, "OTUnbind", OTUnbind(xx->fEndpoint))
      if (result != kOTNoError)
      {
        REPORT_ERROR(OUTPUT_PREFIX "OTUnbind failed (%ld = %s)", result)
        signalError(xx);            
      }
    }
    else
    {
      TBind       bind_request;
      InetAddress in_address;

      OTInitInetAddress(&in_address, xx->fServerPort, xx->fServerAddress);       
      bind_request.addr.len = sizeof(in_address);
      bind_request.addr.buf = reinterpret_cast<Puchar>(&in_address);
      bind_request.qlen = 1;
      WRAP_OT_CALL(xx, result, "OTBind", OTBind(xx->fEndpoint, &bind_request, NULL_PTR))
      if (result != kOTNoError)
      {
        REPORT_ERROR(OUTPUT_PREFIX "OTBind failed (%ld = %s)", result)
        signalError(xx);            
      }
    }
    lockout_set(prev_lock);
    evnum_incr();
#endif /* OPEN_TRANSPORT_SUPPORTED */
  }
  ExitMaxMessageHandler()
} /* processRebindQueue */

/*------------------------------------ processReceiveQueue ---*/
Pvoid processReceiveQueue
  (TcpObjectPtr xx)
{
  EnterCallback();
  if (xx)
  {
#if OPEN_TRANSPORT_SUPPORTED
    short            prev_lock = lockout_set(1);
    TcpBufferLinkPtr temp;
    PAtom            gotStuff;
    short            numAtoms, numMessages;

    for ( ; ; )
    {
      Pchar walker;

      /* Is the received list empty? */
      temp = xx->fReceiveHead;
      if (! temp)
        break;

      /* Grab the head of the received list */
      if (temp->fNext)
        temp->fNext->fPrevious = NULL_PTR;
      xx->fReceiveHead = temp->fNext;
      if (! xx->fReceiveHead)
        xx->fReceiveTail = NULL_PTR;
      temp->fNext = NULL_PTR;
      walker = reinterpret_cast<Pchar>(&temp->fData->fNumElements);
      numMessages = validateBuffer(OUR_NAME, temp->fData, xx->fRawMode);
      for (short ii = 0; ii < numMessages; ++ii)
      {
        /* Allow interrupts while we process the buffer */
        lockout_set(prev_lock);
        numAtoms = 0;
        gotStuff = convertBufferToAtoms(OUR_NAME, &walker, numAtoms, 0,
                                        temp->fData->fNumBytesInUse, xx->fRawMode);
        prev_lock = lockout_set(1);
        if (numAtoms > 0)
        {
          outlet_anything(xx->fResultOut, gReplySymbol, numAtoms, gotStuff);
          FREEBYTES(gotStuff, numAtoms);
        }
      }
      /* Add the temp link to the buffer pool */
      temp->fPrevious = xx->fPoolTail;
      if (temp->fPrevious)
        temp->fPrevious->fNext = temp;
      else
        xx->fPoolHead = temp;
      xx->fPoolTail = temp;
    }
    lockout_set(prev_lock);
    evnum_incr();
#endif /* OPEN_TRANSPORT_SUPPORTED */
  }
  ExitMaxMessageHandler()
} /* processReceiveQueue */

/*------------------------------------ makeReceiveBufferAvailable ---*/
bool makeReceiveBufferAvailable
  (TcpObjectPtr xx)
{
  bool okSoFar = true;

  if (xx)
  {
#if OPEN_TRANSPORT_SUPPORTED
    OSStatus err;
    OTFlags  flags;

 #if SYSLOG_AVAILABLE
    Syslog(LOG_DEBUG, OUTPUT_PREFIX "entering makeReceiveBufferAvailable");
 #endif /* SYSLOG_AVAILABLE */
    WRAP_OT_CALL(xx, err, "OTRcv", OTRcv(xx->fEndpoint,
                                        &xx->fReceiveBuffer->fNumElements,
                                        MAX_BUFFER_TO_RECEIVE, &flags))
    if (err < 0)
    {
      REPORT_ERROR(OUTPUT_PREFIX "OTRcv failed (%ld = %s)", err)
      okSoFar = false;
    }
    else
    {
      /* Get the next available buffer from the buffer pool */
      TcpBufferLinkPtr temp = xx->fPoolHead;

      xx->fReceiveBuffer->fNumBytesInUse = short(err);
      if (temp)
      {
        DataBufferPtr swapper = temp->fData;

        if (temp->fNext)
          temp->fNext->fPrevious = NULL_PTR;
        xx->fPoolHead = temp->fNext;
        if (! xx->fPoolHead)
          xx->fPoolTail = NULL_PTR;
        temp->fNext = NULL_PTR;
        /* Exchange the receive buffer and the pool buffer */
        temp->fData = xx->fReceiveBuffer;
        xx->fReceiveBuffer = swapper;
        /* Add the pool buffer to the receive list */
        temp->fPrevious = xx->fReceiveTail;
        if (temp->fPrevious)
          temp->fPrevious->fNext = temp;
        else
          xx->fReceiveHead = temp;
        xx->fReceiveTail = temp;
      }
      else
      {
        LOG_ERROR_1(OUTPUT_PREFIX "receive buffer queue exhausted")
        okSoFar = false;
      }
      signalReceive(xx);      
    }
 #if SYSLOG_AVAILABLE
    Syslog(LOG_DEBUG, OUTPUT_PREFIX "exiting makeReceiveBufferAvailable");
 #endif /* SYSLOG_AVAILABLE */
#endif /* OPEN_TRANSPORT_SUPPORTED */
  }
  return okSoFar;
} /* makeReceiveBufferAvailable */

/*------------------------------------ setObjectState ---*/
void setObjectState
  (TcpObjectPtr					xx,
   const TcpObjectState	newState)
{
#if (! defined(BE_VERBOSE))
 #pragma unused(name)
#endif /* not BE_VERBOSE */
  if (xx)
  {
    xx->fState = newState;
#if (OPEN_TRANSPORT_SUPPORTED && SYSLOG_AVAILABLE)
    Syslog(LOG_DEBUG, OUTPUT_PREFIX "set object state to %s",
            mapStateToSymbol(newState)->s_name);
#endif /* OPEN_TRANSPORT_SUPPORTED and SYSLOG_AVAILABLE */
#if defined(BE_VERBOSE)
    if (xx->fVerbose)
      LOG_POST_2(OUTPUT_PREFIX "--> %s", mapStateToSymbol(newState)->s_name)
#endif /* BE_VERBOSE */
  }
} /* setObjectState */

/*------------------------------------ transmitBuffer ---*/
void transmitBuffer
  (TcpObjectPtr  xx,
   DataBufferPtr aBuffer)
{
  if (xx && aBuffer)
  {
#if OPEN_TRANSPORT_SUPPORTED
    OSStatus result;

 #if SYSLOG_AVAILABLE
    Syslog(LOG_DEBUG, OUTPUT_PREFIX "entering transmitBuffer");
 #endif /* SYSLOG_AVAILABLE */
    if (xx->fRawMode)
			WRAP_OT_CALL(xx, result, "OTSnd", OTSnd(xx->fEndpoint, &aBuffer->fElements,
                                            aBuffer->fNumBytesInUse, 0L))
    else
    {
      short    num_bytes = short(aBuffer->fNumBytesInUse + SIZEOF_DATABUFFER_HDR);

	    aBuffer->fSanityCheck = htons(short(- (num_bytes + aBuffer->fNumElements)));
	    aBuffer->fNumElements = htons(aBuffer->fNumElements);
	    WRAP_OT_CALL(xx, result, "OTSnd", OTSnd(xx->fEndpoint, &aBuffer->fNumElements,
	                                            num_bytes, 0L))
    }
    if (result < 0)
      REPORT_ERROR(OUTPUT_PREFIX "OTSnd failed (%ld = %s)", result)
 #if SYSLOG_AVAILABLE
    Syslog(LOG_DEBUG, OUTPUT_PREFIX "exiting transmitBuffer");
 #endif /* SYSLOG_AVAILABLE */
#endif /* OPEN_TRANSPORT_SUPPORTED */
  }
} /* transmitBuffer */
