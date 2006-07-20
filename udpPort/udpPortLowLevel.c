/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       udpPortLowLevel.c                                                       */
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
bool makeReceiveBufferAvailable
  (UdpObjectPtr xx)
{
  bool okSoFar = true;

  if (xx)
  {
#if OPEN_TRANSPORT_SUPPORTED
    OSStatus    err;
    OTFlags     flags;
    InetAddress sender;
    TUnitData	  inData;

 #if SYSLOG_AVAILABLE
    Syslog(SYSLOG_LEVEL, OUTPUT_PREFIX "entering makeReceiveBufferAvailable");
 #endif /* SYSLOG_AVAILABLE */
    inData.addr.maxlen = inData.addr.len = sizeof(sender);
    inData.addr.buf = reinterpret_cast<UInt8*>(&sender);
    inData.opt.maxlen = inData.opt.len = 0;
    inData.opt.buf = NULL_PTR;
		inData.udata.maxlen = MAX_BUFFER_TO_RECEIVE;
		inData.udata.len = 0;
		inData.udata.buf = reinterpret_cast<UInt8*>(&xx->fReceiveBuffer->fNumElements);
    WRAP_OT_CALL(xx, err, "OTRcvUData", OTRcvUData(xx->fEndpoint, &inData, &flags))
    if (err < 0)
    {
      REPORT_ERROR(OUTPUT_PREFIX "OTRcvUData failed (%ld = %s)", err)
      reportEndpointState(xx);
      okSoFar = false;
    }
    else if ((flags == T_MORE) || (inData.addr.len == 0))
    {
    	LOG_POST_1(OUTPUT_PREFIX "OTRcvUData oversize packet received and ignored.");
    	okSoFar = false;
    }
    else
    {
      /* Get the next available buffer from the buffer pool */
      UdpBufferLinkPtr temp = xx->fPoolHead;

      xx->fReceiveBuffer->fNumBytesInUse = short(inData.udata.len);
      xx->fReceiveBuffer->fSender = sender;
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
    Syslog(SYSLOG_LEVEL, OUTPUT_PREFIX "exiting makeReceiveBufferAvailable");
 #endif /* SYSLOG_AVAILABLE */
#endif /* OPEN_TRANSPORT_SUPPORTED */
  }
  return okSoFar;
} /* makeReceiveBufferAvailable */

/*------------------------------------ processRebindQueue ---*/
Pvoid processRebindQueue
  (UdpObjectPtr xx)
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
        reportEndpointState(xx);
        signalError(xx);            
      }
    }
    else
    {
      WRAP_OT_CALL(xx, result, "OTBind", OTBind(xx->fEndpoint, NULL_PTR, NULL_PTR))
      if (result != kOTNoError)
      {
        REPORT_ERROR(OUTPUT_PREFIX "OTBind failed (%ld = %s)", result)
        reportEndpointState(xx);
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
  (UdpObjectPtr xx)
{
  EnterCallback();
  if (xx)
  {
#if OPEN_TRANSPORT_SUPPORTED
    short            prev_lock = lockout_set(1);
    UdpBufferLinkPtr temp;
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
      numMessages = validateBuffer(temp->fData, xx->fRawMode);
      for (short ii = 0; ii < numMessages; ++ii)
      {
        /* Allow interrupts while we process the buffer */
        lockout_set(prev_lock);
        numAtoms = 0;
        gotStuff = convertBufferToAtoms(&walker, temp->fData->fSender, numAtoms,
																				temp->fData->fNumBytesInUse, xx->fRawMode);
        prev_lock = lockout_set(1);
        if (numAtoms > 0)
        {
          outlet_anything(xx->fResultOut, gFromSymbol, numAtoms, gotStuff);
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

/*------------------------------------ setObjectState ---*/
void setObjectState
  (UdpObjectPtr					xx,
   const UdpObjectState	newState)
{
#if (! defined(BE_VERBOSE))
 #pragma unused(name)
#endif /* not BE_VERBOSE */
  if (xx)
  {
    xx->fState = newState;
#if (OPEN_TRANSPORT_SUPPORTED && SYSLOG_AVAILABLE)
    Syslog(SYSLOG_LEVEL, OUTPUT_PREFIX "set object state to %s",
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
  (UdpObjectPtr  xx,
   DataBufferPtr aBuffer)
{
  if (xx && aBuffer)
  {
#if OPEN_TRANSPORT_SUPPORTED
    OSStatus		result;
    TUnitData		outData;
    InetAddress	outAddress;

 #if SYSLOG_AVAILABLE
    Syslog(SYSLOG_LEVEL, OUTPUT_PREFIX "entering transmitBuffer");
 #endif /* SYSLOG_AVAILABLE */
    OTInitInetAddress(&outAddress, xx->fPartnerPort, xx->fPartnerAddress);       
   	outData.addr.len = outData.addr.maxlen = sizeof(outAddress);
  	outData.addr.buf = reinterpret_cast<UInt8*>(&outAddress);
  	outData.opt.len = outData.opt.maxlen = 0;
  	outData.opt.buf = NULL_PTR;
  	outData.udata.maxlen = xx->fTransferSize;
    if (xx->fRawMode)
    {
    	outData.udata.len = aBuffer->fNumBytesInUse;
  		outData.udata.buf = reinterpret_cast<UInt8*>(&aBuffer->fElements);
		}
		else
		{
      short num_bytes = short(aBuffer->fNumBytesInUse + SIZEOF_DATABUFFER_HDR);
      
	    aBuffer->fSanityCheck = htons(short(- (num_bytes + aBuffer->fNumElements)));
	    aBuffer->fNumElements = htons(aBuffer->fNumElements);
    	outData.udata.len = num_bytes;
    	outData.udata.buf = reinterpret_cast<UInt8*>(&aBuffer->fNumElements);
		}
		WRAP_OT_CALL(xx, result, "OTSndUData", OTSndUData(xx->fEndpoint, &outData))
    if (result < 0)
    {
      REPORT_ERROR(OUTPUT_PREFIX "OTSndUData failed (%ld = %s)", result)
      reportEndpointState(xx);
    }
 #if SYSLOG_AVAILABLE
    Syslog(SYSLOG_LEVEL, OUTPUT_PREFIX "exiting transmitBuffer");
 #endif /* SYSLOG_AVAILABLE */
#endif /* OPEN_TRANSPORT_SUPPORTED */
  }
} /* transmitBuffer */
