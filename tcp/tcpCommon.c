/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       tcpCommon.c                                                             */
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
static Pvoid processErrorQueue
  (TcpObjectPtr xx)
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

/*------------------------------------ initObject ---*/
bool initObject
  (Qchar				name,
   TcpObjectPtr xx,
   const long   port,
   const long   numBuffers)
{
  bool okSoFar = true;

  if (xx)
  {
#if (OPEN_TRANSPORT_SUPPORTED && SYSLOG_AVAILABLE)
    Syslog(SYSLOG_LEVEL, "%s: entering initObject", name);
#endif /* OPEN_TRANSPORT_SUPPORTED and SYSLOG_AVAILABLE */
    xx->fServerPort = static_cast<ushort>(port ? port : DEFAULT_PORT);
    xx->fClientAddress = 0L;
    xx->fClientPort = 0;
    xx->fErrorBangOut = static_cast<POutlet>(bangout(xx));
    xx->fResultOut = static_cast<POutlet>(outlet_new(xx, NULL_PTR));
    setObjectState(xx, TCP_OBJECT_UNBOUND);
    xx->fEndpoint = kOTInvalidEndpointRef;
    xx->fErrorQueue = static_cast<PQelem>(qelem_new(xx,
                                            reinterpret_cast<method>(processErrorQueue)));
    xx->fRebindQueue = static_cast<PQelem>(qelem_new(xx,
                                            reinterpret_cast<method>(processRebindQueue)));
    xx->fReceiveQueue = static_cast<PQelem>(qelem_new(xx,
                                          reinterpret_cast<method>(processReceiveQueue)));
    xx->fBufferBase = reinterpret_cast<DataBufferHdl>(newhandle(long(BUFF_MEMORY_TO_ALLOC *
                                                                (numBuffers + 2))));
    if (xx->fBufferBase)
    {
      HLock(reinterpret_cast<Handle>(xx->fBufferBase));
      xx->fSendBuffer = *xx->fBufferBase;
      xx->fReceiveBuffer = reinterpret_cast<DataBufferPtr>(ADD_TO_ADDRESS(xx->fSendBuffer,
                                                                    BUFF_MEMORY_TO_ALLOC));
    }
    xx->fLinkBase = reinterpret_cast<TcpBufferLinkHdl>(newhandle(long(sizeof(TcpBufferLinkData) *
                                                                      numBuffers)));
    if (xx->fLinkBase)
    {
      DataBufferPtr    this_buffer =
                              reinterpret_cast<DataBufferPtr>(ADD_TO_ADDRESS(xx->fReceiveBuffer,
                                                                    BUFF_MEMORY_TO_ALLOC));
      TcpBufferLinkPtr prev_link = NULL_PTR, this_link = NULL_PTR;

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
    xx->fClosing = xx->fRawMode = false;
    if (! (xx->fResultOut && xx->fErrorBangOut && xx->fErrorQueue && xx->fBufferBase &&
            xx->fRebindQueue && xx->fReceiveQueue && xx->fLinkBase))
    {
      LOG_ERROR_2("%s: unable to create port or buffer for object", name)
      okSoFar = false;
    }
  }
#if (OPEN_TRANSPORT_SUPPORTED && SYSLOG_AVAILABLE)
  Syslog(SYSLOG_LEVEL, "%s: exiting initObject", name);
#endif /* OPEN_TRANSPORT_SUPPORTED and SYSLOG_AVAILABLE */
  return okSoFar;
} /* initObject */

/*------------------------------------ presetObjectPointers ---*/
void presetObjectPointers
  (TcpObjectPtr xx)
{
  if (xx)
  {
    xx->fErrorBangOut = xx->fResultOut = NULL_PTR;
    xx->fErrorQueue = xx->fRebindQueue = xx->fReceiveQueue = NULL_PTR;
    xx->fSendBuffer = xx->fReceiveBuffer = NULL_PTR;
    xx->fAccessControl = NULL_PTR;
    xx->fBufferBase = NULL_HDL;
    xx->fLinkBase = NULL_HDL;
    xx->fEndpoint = kOTInvalidEndpointRef;
    xx->fPartnerName = xx->fSelfName = NULL_PTR;
    xx->fReceiveHead = xx->fReceiveTail = xx->fPoolHead = xx->fPoolTail = NULL_PTR;
  }
} /* presetObjectPointers */

/*------------------------------------ releaseObjectMemory ---*/
void releaseObjectMemory
  (Qchar				name,
   TcpObjectPtr xx)
{
#if ((! OPEN_TRANSPORT_SUPPORTED) || (! SYSLOG_AVAILABLE))
 #pragma unused(name)
#endif /* not OPEN_TRANSPORT_SUPPORTED */
  if (xx)
  {
#if (OPEN_TRANSPORT_SUPPORTED && SYSLOG_AVAILABLE)
    Syslog(SYSLOG_LEVEL, "%s: entering releaseObjectMemory", name);
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
    xx->fReceiveBuffer = xx->fSendBuffer = NULL_PTR;
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
#if (OPEN_TRANSPORT_SUPPORTED && SYSLOG_AVAILABLE)
    Syslog(SYSLOG_LEVEL, "%s: exiting releaseObjectMemory", name);
#endif /* OPEN_TRANSPORT_SUPPORTED and SYSLOG_AVAILABLE */
  }
} /* releaseObjectMemory */

/*------------------------------------ reportEndpointState ---*/
void reportEndpointState
  (Qchar				name,
   TcpObjectPtr xx)
{
  if (xx)
  	LOG_ERROR_3("%s: Endpoint state: %s", name, describeEndpointState(xx->fEndpoint));
} /* reportEndpointState */
