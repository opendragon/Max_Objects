/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       tcpCommon.h                                                             */
/*                                                                                      */
/*  Contains:   The data type declarations and forward references to routines.          */
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

#if (! defined(TCPCOMMON_H_))
 #define TCPCOMMON_H_ /* */
 
 #include "tcpMapping.h"
 
struct TcpObjectData
{
  Object             fObject;
  POutlet            fErrorBangOut;
  POutlet            fResultOut;
  PQelem             fErrorQueue;
  PQelem             fRebindQueue;
  PQelem             fReceiveQueue;
  AccessOTControlPtr fAccessControl;
  TcpBufferLinkHdl   fLinkBase;
  TcpBufferLinkPtr   fReceiveHead;
  TcpBufferLinkPtr   fReceiveTail;
  TcpBufferLinkPtr   fPoolHead;
  TcpBufferLinkPtr   fPoolTail;
  DataBufferHdl      fBufferBase;
  DataBufferPtr      fReceiveBuffer;
  DataBufferPtr      fSendBuffer;
  EndpointRef        fEndpoint;
  PSymbol            fPartnerName;
  PSymbol            fSelfName;
  ulong              fClientAddress;
  ushort             fClientPort;
  ulong              fServerAddress;
  ushort             fServerPort;
  UInt32             fServiceType;
  short              fAddressSize;
  TcpObjectState     fState;
  bool               fClosing;
  bool               fRawMode;
  bool               fVerbose;
 #if defined(COMPILE_FOR_CATS)
  OTNotifyUPP        fDataNotifier;
 #endif /* COMPILE_FOR_CATS */
}; /* TcpObjectData */

typedef TcpObjectData * TcpObjectPtr;

Pvoid cmd_Disconnect
  (TcpObjectPtr xx);

Pvoid cmd_Mode
  (TcpObjectPtr xx,
   PSymbol      rawOrCooked);

Pvoid cmd_Port
  (TcpObjectPtr xx,
   long         number);

Pvoid cmd_Self
  (TcpObjectPtr xx);

Pvoid cmd_Send
  (TcpObjectPtr xx,
   PSymbol      message,
   short        argc,
   PAtom        argv);

Pvoid cmd_Status
  (TcpObjectPtr xx);

Pvoid cmd_Verbose
  (TcpObjectPtr xx,
   PSymbol      onOff);

bool initObject
  (Qchar				name,
   TcpObjectPtr xx,
   const long   port,
   const long   numBuffers);

bool makeReceiveBufferAvailable
  (TcpObjectPtr xx);

void presetObjectPointers
  (TcpObjectPtr xx);

Pvoid processRebindQueue
  (TcpObjectPtr xx);

Pvoid processReceiveQueue
  (TcpObjectPtr xx);

void releaseObjectMemory
  (Qchar				name,
   TcpObjectPtr xx);

void reportEndpointState
	(Qchar				name,
	 TcpObjectPtr	xx);

void setObjectState
  (TcpObjectPtr   			xx,
   const TcpObjectState	newState);

void transmitBuffer
  (TcpObjectPtr  xx,
   DataBufferPtr aBuffer);

StandardRoutineDeclarations(TcpObjectPtr)

#endif /* not TCPCOMMON_H_ */
