/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       udpPort.h                                                               */
/*                                                                                      */
/*  Contains:   The data type declarations and forward references to routines.          */
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

#if (! defined(UDPPORT_H_))
 #define UDPPORT_H_ /* */

// #define USE_SYSLOG  /* must be set in order to pull in the definitions that we need! */
 #define SYSLOG_LEVEL	LOG_INFO
 
 #include "MissingAndExtra.h"
 #include "acquireOpenTransport.h"
 #include "udpBuffers.h"
 #if (OPEN_TRANSPORT_SUPPORTED && defined(USE_SYSLOG))
  #include "syslog.h"
 #endif /* OPEN_TRANSPORT_SUPPORTED and USE_SYSLOG */

 #define NUM_RX_BUFFERS 25

 #define DEFAULT_PORT 0x00FFFFL
 #define MAX_PORT     0x00FFFFL
 
 #define BE_VERBOSE /* */
 
// #define TRACE_OT_CALLS /* */
// #define TRACE_MAX_MESSAGES /* */
 
 #define ADD_TO_ADDRESS(yy, amount) (reinterpret_cast<Ptr>(yy) + amount) 

 #define signalError(xx)   qelem_set(xx->fErrorQueue)
 #define signalRebind(xx)  qelem_set(xx->fRebindQueue)
 #define signalReceive(xx) qelem_set(xx->fReceiveQueue)

 #define OUR_NAME      "udpPort"
 #define OUR_RES_NUMB  17181
 #define OUTPUT_PREFIX "udpPort: "

enum UdpObjectState
{
  TCP_OBJECT_UNBOUND,
  TCP_OBJECT_BOUND
}; /* UdpObjectState */

struct UdpBufferLinkData
{
  UdpBufferLinkData * fPrevious;
  UdpBufferLinkData * fNext;
  DataBufferPtr       fData;
}; /* UdpBufferLinkData */

typedef UdpBufferLinkData * UdpBufferLinkPtr;
typedef UdpBufferLinkPtr *  UdpBufferLinkHdl;
 
struct UdpObjectData
{
  Object							fObject;
  POutlet							fErrorBangOut;
  POutlet							fResultOut;
  PQelem							fErrorQueue;
  PQelem							fRebindQueue;
  PQelem 							fReceiveQueue;
  AccessOTControlPtr	fAccessControl;
  UdpBufferLinkHdl		fLinkBase;
  UdpBufferLinkPtr		fReceiveHead;
  UdpBufferLinkPtr		fReceiveTail;
  UdpBufferLinkPtr		fPoolHead;
  UdpBufferLinkPtr		fPoolTail;
  DataBufferHdl				fBufferBase;
  DataBufferPtr				fReceiveBuffer;
  DataBufferPtr				fSendBuffer;
  EndpointRef					fEndpoint;
  ulong								fSelfAddress;
  PSymbol							fSelfName;
  ushort							fSelfPort;
  UInt32							fServiceType;
  ulong								fPartnerAddress;
  ushort							fPartnerPort;
  short								fAddressSize;
  short								fTransferSize;
  UdpObjectState			fState;
  bool								fClosing;
  bool								fPartnerKnown;								
  bool								fRawMode;
  bool								fVerbose;
 #if defined(COMPILE_FOR_CATS)
  OTNotifyUPP					fDataNotifier;
 #endif /* COMPILE_FOR_CATS */
}; /* UdpObjectData */

typedef UdpObjectData * UdpObjectPtr;

Pvoid cmd_Mode
  (UdpObjectPtr xx,
   PSymbol      rawOrCooked);

Pvoid cmd_Port
  (UdpObjectPtr xx,
   long         number);

Pvoid cmd_Self
  (UdpObjectPtr xx);

Pvoid cmd_Send
  (UdpObjectPtr xx,
   PSymbol      message,
   short        argc,
   PAtom        argv);

Pvoid cmd_SendTo
  (UdpObjectPtr xx,
   PSymbol      ipAddress,
   long         portNumber);

Pvoid cmd_Status
  (UdpObjectPtr xx);

Pvoid cmd_Verbose
  (UdpObjectPtr xx,
   PSymbol      onOff);
 
bool checkIpString
  (PSymbol	ipAddress,
   short &	byte_0,
   short &	byte_1,
   short &	byte_2,
   short &  byte_3);

Ptr describeEndpointState
  (EndpointRef anEndpoint);

bool initObject
  (UdpObjectPtr xx,
   const long   port,
   const long   numBuffers);

bool makeReceiveBufferAvailable
  (UdpObjectPtr xx);

Ptr mapEventToString
  (const OTEventCode	code);

PSymbol mapStateToSymbol
  (const UdpObjectState	aState);

void presetObjectPointers
  (UdpObjectPtr xx);

Pvoid processRebindQueue
  (UdpObjectPtr xx);

Pvoid processReceiveQueue
  (UdpObjectPtr xx);

void releaseObjectMemory
  (UdpObjectPtr xx);

void reportEndpointState
	(UdpObjectPtr	xx);

void setObjectState
  (UdpObjectPtr   			xx,
   const UdpObjectState	newState);

void setUpStateSymbols
  (void);
    
void transmitBuffer
  (UdpObjectPtr  xx,
   DataBufferPtr aBuffer);

pascal void udpPortNotifier
  (Pvoid       context,
   OTEventCode code,
   OTResult    result,
   Pvoid       cookie);

bool udpPortSetPort
  (UdpObjectPtr xx,
   const bool		bangOnError);

StandardRoutineDeclarations(UdpObjectPtr)

 #if (defined(TRACE_OT_CALLS) && defined(BE_VERBOSE))
  #if defined(USE_SYSLOG)
   #define WRAP_OT_CALL(xx, res, name, aCall) \
{\
  res = aCall;\
  Syslog(SYSLOG_LEVEL, OUTPUT_PREFIX name " --> (%ld = %s)", long(res),\
          mapErrorCodeToString(res));\
  if (xx->fVerbose)\
    post(OUTPUT_PREFIX name " --> (%ld = %s)", long(res), mapErrorCodeToString(res));\
}
  #else /* not USE_SYSLOG */
   #define WRAP_OT_CALL(xx, res, name, aCall) \
{\
  res = aCall;\
  if (xx->fVerbose)\
    post(OUTPUT_PREFIX name " --> (%ld = %s)", long(res), mapErrorCodeToString(res));\
}
  #endif /* not USE_SYSLOG */
 #else /* not TRACE_OT_CALLS or not BE_VERBOSE */
  #define WRAP_OT_CALL(xx, res, name, aCall) \
res = aCall;
 #endif /* not TRACE_OT_CALLS or not BE_VERBOSE */
  
 #define REPORT_ERROR(msg, value) \
LOG_ERROR_3(msg, long(value), mapErrorCodeToString(value))

 #if defined(macintosh)
 // Big-endian, so network byte order is our order
  #define htonl(xx) (xx)
  #define htons(xx) (xx)
  #define ntohl(xx) (xx)
  #define ntohs(xx) (xx)
 #else /* not macintosh */
  #define htonl(xx) (((xx & 0x0FF) << 24) | (((xx >> 8) & 0x0FF) << 16) | (((xx >> 16) & 0x0FF) << 8) | ((xx >> 24) & 0x0FF))
  #define htons(xx) (((xx & 0x0FF) << 8) | ((xx >> 8) & 0x0FF))
  #define ntohl(xx) (((xx & 0x0FF) << 24) | (((xx >> 8) & 0x0FF) << 16) | (((xx >> 16) & 0x0FF) << 8) | ((xx >> 24) & 0x0FF))
  #define ntohs(xx) (((xx & 0x0FF) << 8) | ((xx >> 8) & 0x0FF))
 #endif /* not macintosh */

mextern(PSymbol) gBoundSymbol;   /* Pointer to unique Symbol for 'bound' */
mextern(PSymbol) gEmptySymbol;   /* Pointer to unique Symbol for '' */
mextern(PSymbol) gFromSymbol;    /* Pointer to unique Symbol for 'from' */
mextern(PSymbol) gMaxSymbol;     /* Pointer to unique Symbol for 'max' */
mextern(PSymbol) gOffSymbol;     /* Pointer to unique Symbol for 'off' */
mextern(PSymbol) gOnSymbol;      /* Pointer to unique Symbol for 'on' */
mextern(PSymbol) gRawSymbol;     /* Pointer to unique Symbol for 'raw' */
mextern(PSymbol) gSelfSymbol;    /* Pointer to unique Symbol for 'self' */
mextern(PSymbol) gStatusSymbol;  /* Pointer to unique Symbol for 'status' */
mextern(PSymbol) gUnboundSymbol; /* Pointer to unique Symbol for 'unbound' */
mextern(PSymbol) gUnknownSymbol; /* Pointer to unique Symbol for 'unknown' */
mextern(short)   gResourceBase;  /* Base value for resources */

#endif /* not UDPPORT_H_ */
