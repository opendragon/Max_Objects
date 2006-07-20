/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       tcpMultiServerLowLevel.c                                                */
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
static Pvoid processErrorQueue
  (TcpMultiServerPtr xx)
{
  EnterCallback();
  if (xx)
  {
    short prev_lock = lockout_set(1);

    outlet_bang(xx->fErrorBangOut);
    lockout_set(prev_lock);
    evnum_incr();
  }
  ExitMaxMessageHandler()
} /* processErrorQueue */

/*------------------------------------ processRebindQueue ---*/
static Pvoid processRebindQueue
	(TcpMultiServerPtr xx)
{
  EnterCallback();
  if (xx)
  {
#if OPEN_TRANSPORT_SUPPORTED
    short    prev_lock = lockout_set(1);
    OSStatus result = kOTNoError;

    if (xx->fState == TCP_OBJECT_BOUND)
    {
      WRAP_OT_CALL(xx, result, "OTUnbind", OTUnbind(xx->fListenEndpoint))
      if (result != kOTNoError)
      {
        REPORT_ERROR(OUTPUT_PREFIX "OTUnbind failed (%ld = %s)", result)
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
      bind_request.addr.buf = reinterpret_cast<Puchar>(&in_address);
      bind_request.qlen = 1;
      WRAP_OT_CALL(xx, result, "OTBind", OTBind(xx->fListenEndpoint, &bind_request,
                                                NULL_PTR))
      if (result != kOTNoError)
      {
        REPORT_ERROR(OUTPUT_PREFIX "OTBind failed (%ld = %s)", result)
        reportEndpointState(xx, xx->fListenEndpoint);
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
static Pvoid processReceiveQueue
  (TcpMultiServerPtr xx)
{
  EnterCallback();
  if (xx)
  {
#if OPEN_TRANSPORT_SUPPORTED
    short            prev_lock = lockout_set(1);
    TcpBufferLinkPtr temp;
    PAtom            got_stuff;
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
      numMessages = validateBuffer(OUR_NAME, temp->fData, temp->fRawMode);
      for (short ii = 0; ii < numMessages; ++ii)
      {
        /* Allow interrupts while we process the buffer */
        lockout_set(prev_lock);
        numAtoms = 0;
        got_stuff = convertBufferToAtoms(OUR_NAME, &walker, numAtoms, temp->fIdentifier,
                                         temp->fData->fNumBytesInUse, temp->fRawMode);
        prev_lock = lockout_set(1);
        if (numAtoms > 0)
        {
          outlet_anything(xx->fResultOut, gReplySymbol, numAtoms, got_stuff);
          FREEBYTES(got_stuff, numAtoms)
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

/*------------------------------------ initObject ---*/
bool initObject
  (TcpMultiServerPtr	xx,
   const long					port,
   const long					numBuffers)
{
  bool okSoFar = true;

  if (xx)
  {
#if (OPEN_TRANSPORT_SUPPORTED && SYSLOG_AVAILABLE)
    Syslog(SYSLOG_LEVEL, OUTPUT_PREFIX "entering initObject");
#endif /* OPEN_TRANSPORT_SUPPORTED and SYSLOG_AVAILABLE */
    xx->fServerPort = static_cast<ushort>(port ? port : DEFAULT_PORT);
    xx->fErrorBangOut = static_cast<POutlet>(bangout(xx));
    xx->fResultOut = static_cast<POutlet>(outlet_new(xx, NULL_PTR));
    setObjectState(xx, TCP_OBJECT_UNBOUND);
    xx->fListenEndpoint = kOTInvalidEndpointRef;
    xx->fErrorQueue = static_cast<PQelem>(qelem_new(xx,
                                            reinterpret_cast<method>(processErrorQueue)));
    xx->fRebindQueue = static_cast<PQelem>(qelem_new(xx,
                                            reinterpret_cast<method>(processRebindQueue)));
    xx->fReceiveQueue = static_cast<PQelem>(qelem_new(xx,
                                          reinterpret_cast<method>(processReceiveQueue)));
    xx->fBufferBase = reinterpret_cast<DataBufferHdl>(newhandle(long((numBuffers +
                                (xx->fMaximumConnections * 2)) * BUFF_MEMORY_TO_ALLOC)));
    if (xx->fBufferBase)
      HLock(reinterpret_cast<Handle>(xx->fBufferBase));
    xx->fLinkBase = reinterpret_cast<TcpBufferLinkHdl>(newhandle(long(numBuffers *
                                                            sizeof(TcpBufferLinkData))));
    if (xx->fLinkBase)
    {
      // Offset by the number of buffers needed for the receive/send buffers dedicated to each connection:
      DataBufferPtr    this_buffer =
                                reinterpret_cast<DataBufferPtr>(ADD_TO_ADDRESS(*xx->fBufferBase,
                                    xx->fMaximumConnections * 2 * BUFF_MEMORY_TO_ALLOC));
      TcpBufferLinkPtr prev_link = NULL_PTR;
      TcpBufferLinkPtr this_link = NULL_PTR;

      HLock(reinterpret_cast<Handle>(xx->fLinkBase));
      xx->fPoolHead = *xx->fLinkBase;
      this_link = xx->fPoolHead;
      for (long link_count = 0; link_count < numBuffers; ++link_count)
      {
        this_link->fPrevious = prev_link;
        this_link->fData = this_buffer;
        this_buffer = reinterpret_cast<DataBufferPtr>(ADD_TO_ADDRESS(this_buffer,
                                                                BUFF_MEMORY_TO_ALLOC));
        this_link->fNext = NULL_PTR;
        if (prev_link)
          prev_link->fNext = this_link;
        prev_link = this_link;
        this_link = reinterpret_cast<TcpBufferLinkPtr>(ADD_TO_ADDRESS(this_link,
                                                              sizeof(TcpBufferLinkData)));
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
      LOG_ERROR_1(OUTPUT_PREFIX "unable to create port or buffer for object")
      okSoFar = false;
    }
  }
#if (OPEN_TRANSPORT_SUPPORTED && SYSLOG_AVAILABLE)
  Syslog(SYSLOG_LEVEL, OUTPUT_PREFIX "exiting initObject");
#endif /* OPEN_TRANSPORT_SUPPORTED and SYSLOG_AVAILABLE */
  return okSoFar;
} /* initObject */

/*------------------------------------ makeReceiveBufferAvailable ---*/
bool makeReceiveBufferAvailable
	(TcpConnectionPtr connection)
{
  bool okSoFar = true;

  if (connection)
  {
#if OPEN_TRANSPORT_SUPPORTED
    TcpMultiServerPtr xx = connection->fOwner;
    OSStatus          err;
    OTFlags           flags;

 #if SYSLOG_AVAILABLE
    Syslog(SYSLOG_LEVEL, OUTPUT_PREFIX "entering makeReceiveBufferAvailable (%d)",
            connection->fIdentifier);
 #endif /* SYSLOG_AVAILABLE */
    WRAP_OT_CALL(connection->fOwner, err, "OTRcv", OTRcv(connection->fDataEndpoint,
                                                &connection->fReceiveBuffer->fNumElements,
                                                          MAX_BUFFER_TO_RECEIVE, &flags))
    if (err < 0)
    {
      REPORT_ERROR(OUTPUT_PREFIX "OTRcv failed (%ld = %s)", err)
      reportEndpointState(xx, connection->fDataEndpoint);
      okSoFar = false;
    }
    else
    {
      /* Get the next available buffer from the buffer pool */
      TcpBufferLinkPtr temp = xx->fPoolHead;

      connection->fReceiveBuffer->fNumBytesInUse = short(err);
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
        temp->fData = connection->fReceiveBuffer;
        connection->fReceiveBuffer = swapper;
        /* Add the pool buffer to the receive list */
        temp->fPrevious = xx->fReceiveTail;
        if (temp->fPrevious)
          temp->fPrevious->fNext = temp;
        else
          xx->fReceiveHead = temp;
        xx->fReceiveTail = temp;
        temp->fIdentifier = connection->fIdentifier;
        temp->fRawMode = connection->fRawMode;
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

/*------------------------------------ presetObjectPointers ---*/
void presetObjectPointers
  (TcpMultiServerPtr xx)
{
	if (xx)
	{
	  xx->fErrorBangOut = xx->fResultOut = NULL_PTR;
	  xx->fErrorQueue = xx->fRebindQueue = xx->fReceiveQueue = NULL_PTR;
	  xx->fListenEndpoint = kOTInvalidEndpointRef;
	  xx->fAccessControl = NULL_PTR;
	  xx->fBufferBase = NULL_HDL;
	  xx->fLinkBase = NULL_HDL;
	  xx->fSelfName = NULL_PTR;
	  xx->fReceiveHead = xx->fReceiveTail = xx->fPoolHead = xx->fPoolTail = NULL_PTR;
	  xx->fConnections = NULL_HDL;
	  xx->fConnectionBase = NULL_HDL;
	}
} /* presetObjectPointers */

/*------------------------------------ releaseObjectMemory ---*/
void releaseObjectMemory
  (TcpMultiServerPtr xx)
{
  if (xx)
  {
#if (OPEN_TRANSPORT_SUPPORTED && SYSLOG_AVAILABLE)
    Syslog(SYSLOG_LEVEL, OUTPUT_PREFIX "entering releaseObjectMemory");
#endif /* OPEN_TRANSPORT_SUPPORTED and SYSLOG_AVAILABLE */
    if (xx->fErrorQueue)
    {
      qelem_unset(xx->fErrorQueue);
      qelem_free(xx->fErrorQueue);
      xx->fErrorQueue = NULL_PTR;
    }
    if (xx->fReceiveQueue)
    {
      qelem_unset(xx->fReceiveQueue);
      qelem_free(xx->fReceiveQueue);
      xx->fReceiveQueue = NULL_PTR;
    }
    if (xx->fRebindQueue)
    {
      qelem_unset(xx->fRebindQueue);
      qelem_free(xx->fRebindQueue);
      xx->fRebindQueue = NULL_PTR;
    }
    if (xx->fConnectionBase)
    {
#if OPEN_TRANSPORT_SUPPORTED
      OSStatus         result;
      TcpConnectionPtr conn_walker = *xx->fConnectionBase;

      for (ushort ii = 0; ii < xx->fMaximumConnections; ++ii)
      {
        // Do any special clean-up here. TBD
        if (conn_walker->fDataEndpoint != kOTInvalidEndpointRef)
        {
          WRAP_OT_CALL(xx, result, "OTCloseProvider",
                        OTCloseProvider(conn_walker->fDataEndpoint))
          if (result == kOTNoError)
            conn_walker->fDataEndpoint = kOTInvalidEndpointRef;
          else
          {
            REPORT_ERROR(OUTPUT_PREFIX "OTCloseProvider failed (%ld = %s)", result)
		        reportEndpointState(xx, conn_walker->fDataEndpoint);
          }
        }
        conn_walker++;
      }
  #endif /* OPEN_TRANSPORT_SUPPORTED */
      HUnlock(reinterpret_cast<Handle>(xx->fConnectionBase));
      disposhandle(reinterpret_cast<Handle>(xx->fConnectionBase));
      xx->fConnectionBase = NULL_HDL;
    }
    if (xx->fBufferBase)
    {
      HUnlock(reinterpret_cast<Handle>(xx->fBufferBase));
      disposhandle(reinterpret_cast<Handle>(xx->fBufferBase));
      xx->fBufferBase = NULL_HDL;
    }
    xx->fReceiveHead = xx->fReceiveTail = xx->fPoolHead = xx->fPoolTail = NULL_PTR;
    if (xx->fLinkBase)
    {
      HUnlock(reinterpret_cast<Handle>(xx->fLinkBase));
      disposhandle(reinterpret_cast<Handle>(xx->fLinkBase));
      xx->fLinkBase = NULL_HDL;
    }
    FREEBYTES(xx->fConnections, xx->fMaximumConnections)
#if (OPEN_TRANSPORT_SUPPORTED && SYSLOG_AVAILABLE)
    Syslog(SYSLOG_LEVEL, OUTPUT_PREFIX "exiting releaseObjectMemory");
#endif /* OPEN_TRANSPORT_SUPPORTED and SYSLOG_AVAILABLE */
  }
} /* releaseObjectMemory */

/*------------------------------------ reportEndpointState ---*/
void reportEndpointState
  (TcpMultiServerPtr	xx,
   EndpointRef				endpoint)
{
  if (xx)
  	LOG_ERROR_2(OUTPUT_PREFIX "Endpoint state: %s", describeEndpointState(endpoint));
} /* reportEndpointState */

/*------------------------------------ setConnectionState ---*/
void setConnectionState
  (TcpConnectionPtr			connection,
   const TcpObjectState	newState)
{
  if (connection)
  {
#if defined(BE_VERBOSE)
    TcpMultiServerPtr xx = connection->fOwner;
#endif /* BE_VERBOSE */

    connection->fState = newState;
#if (OPEN_TRANSPORT_SUPPORTED && SYSLOG_AVAILABLE)
    Syslog(SYSLOG_LEVEL, OUTPUT_PREFIX "set connection state %d to %s",
            connection->fIdentifier, mapStateToSymbol(newState)->s_name);
#endif /* OPEN_TRANSPORT_SUPPORTED and SYSLOG_AVAILABLE */
#if defined(BE_VERBOSE)
    if (xx &&  xx->fVerbose)
      LOG_POST_3(OUTPUT_PREFIX "--> %s/%d", mapStateToSymbol(newState)->s_name,
                  connection->fIdentifier)
#endif /* BE_VERBOSE */
  }
} /* setConnectionState */

/*------------------------------------ setObjectState ---*/
void setObjectState
  (TcpMultiServerPtr		xx,
   const TcpObjectState	newState)
{
  if (xx)
  {
    xx->fState = newState;
#if (OPEN_TRANSPORT_SUPPORTED && SYSLOG_AVAILABLE)
    Syslog(SYSLOG_LEVEL, OUTPUT_PREFIX "set object state to %s",
            mapStateToSymbol(newState)->s_name);
#endif /* OPEN_TRANSPORT_SUPPORTED and SYSLOG_AVAILABLE */
#if defined(BE_VERBOSE)
    if (xx->fVerbose)
      LOG_POST_2(OUTPUT_PREFIX "--> %s/0", mapStateToSymbol(newState)->s_name)
#endif /* BE_VERBOSE */
  }
} /* setObjectState */

/*------------------------------------ transmitBuffer ---*/
void transmitBuffer
  (TcpMultiServerPtr xx,
   EndpointRef       out,
   DataBufferPtr     aBuffer,
   const bool        rawMode)
{
#if (! OPEN_TRANSPORT_SUPPORTED)
 #pragma unused(out)
#endif /* not OPEN_TRANSPORT_SUPPORTED */
  if (xx && aBuffer)
  {
#if OPEN_TRANSPORT_SUPPORTED
    OSStatus result;

 #if SYSLOG_AVAILABLE
    Syslog(SYSLOG_LEVEL, OUTPUT_PREFIX "entering transmitBuffer");
 #endif /* SYSLOG_AVAILABLE */
    if (rawMode)
	    WRAP_OT_CALL(xx, result, "OTSnd", OTSnd(out, &aBuffer->fElements, aBuffer->fNumBytesInUse, 0L))
    else
    {
      short    num_bytes = short(aBuffer->fNumBytesInUse + SIZEOF_DATABUFFER_HDR);
      
	    aBuffer->fSanityCheck = htons(short(- (num_bytes + aBuffer->fNumElements)));
	    aBuffer->fNumElements = htons(aBuffer->fNumElements);
	    WRAP_OT_CALL(xx, result, "OTSnd", OTSnd(out, &aBuffer->fNumElements, num_bytes, 0L))
    }
    if (result < 0)
    {
      REPORT_ERROR(OUTPUT_PREFIX "OTSnd failed (%ld = %s)", result)
      reportEndpointState(xx, out);
    }
 #if SYSLOG_AVAILABLE
    Syslog(SYSLOG_LEVEL, OUTPUT_PREFIX "exiting transmitBuffer");
 #endif /* SYSLOG_AVAILABLE */
#endif /* OPEN_TRANSPORT_SUPPORTED */
  }
} /* transmitBuffer */