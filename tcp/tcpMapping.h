/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       tcpMapping.h                                                            */
/*                                                                                      */
/*  Contains:   The data type declarations and forward references to routines.          */
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
/*  Created:    2000/10/14                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(TCPMAPPING_H_))
 #define TCPMAPPING_H_ /* */
 
 /* #define USE_SYSLOG /* */
 
 #include "MissingAndExtra.h"
 #include "acquireOpenTransport.h"
 #include "tcpBuffers.h"
 #if (OPEN_TRANSPORT_SUPPORTED && defined(USE_SYSLOG))
  #include "syslog.h"
 #endif /* OPEN_TRANSPORT_SUPPORTED and USE_SYSLOG */

 #define NUM_RX_BUFFERS 25

 #define DEFAULT_PORT 0x00FFFFL
 #define MAX_PORT     0x00FFFFL
 
 #define BE_VERBOSE /* */
 
 #define USE_FORCED_DISCONNECTS true /* false for orderly disconnects */
 #define TRACE_OT_CALLS /* */
 #define TRACE_MAX_MESSAGES /* */
 
 #define ADD_TO_ADDRESS(yy, amount) (reinterpret_cast<Ptr>(yy) + amount) 

 #define signalError(xx)   qelem_set(xx->fErrorQueue)
 #define signalRebind(xx)  qelem_set(xx->fRebindQueue)
 #define signalReceive(xx) qelem_set(xx->fReceiveQueue)

enum TcpObjectState
{
  TCP_OBJECT_UNBOUND,
  TCP_OBJECT_BOUND,
  TCP_OBJECT_LISTENING,
  TCP_OBJECT_CONNECTING,
  TCP_OBJECT_CONNECTED,
  TCP_OBJECT_DISCONNECTING
}; /* TcpObjectState */

struct TcpBufferLinkData
{
  TcpBufferLinkData * fPrevious;
  TcpBufferLinkData * fNext;
  DataBufferPtr       fData;
 #if defined(TCPMULTISERVER_H_)
  ushort              fIdentifier;
  bool                fRawMode;
 #endif /* TCPMULTISERVER_H_ */
}; /* TcpBufferLinkData */

typedef TcpBufferLinkData * TcpBufferLinkPtr;
typedef TcpBufferLinkPtr *  TcpBufferLinkHdl;
 
 #if (defined(TRACE_OT_CALLS) && defined(BE_VERBOSE))
  #if defined(USE_SYSLOG)
   #define WRAP_OT_CALL(xx, res, name, aCall) \
{\
  res = aCall;\
  Syslog(LOG_DEBUG, OUTPUT_PREFIX name " --> (%ld = %s)", long(res),\
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
 
Ptr describeEndpointState
  (EndpointRef anEndpoint);

Ptr mapEventToString
  (const OTEventCode	code);

PSymbol mapStateToSymbol
  (const TcpObjectState	aState);

void setUpStateSymbols
  (void);

mextern(PSymbol) gBoundSymbol;         /* Pointer to unique Symbol for 'bound' */
mextern(PSymbol) gConnectedSymbol;     /* Pointer to unique Symbol for 'connected' */
mextern(PSymbol) gConnectingSymbol;    /* Pointer to unique Symbol for 'connecting' */
mextern(PSymbol) gDisconnectingSymbol; /* Pointer to unique Symbol for 'disconnecting' */
mextern(PSymbol) gEmptySymbol;         /* Pointer to unique Symbol for '' */
mextern(PSymbol) gListeningSymbol;     /* Pointer to unique Symbol for 'listening' */
mextern(PSymbol) gMaxSymbol;           /* Pointer to unique Symbol for 'max' */
mextern(PSymbol) gOffSymbol;           /* Pointer to unique Symbol for 'off' */
mextern(PSymbol) gOnSymbol;            /* Pointer to unique Symbol for 'on' */
mextern(PSymbol) gRawSymbol;           /* Pointer to unique Symbol for 'raw' */
mextern(PSymbol) gReplySymbol;         /* Pointer to unique Symbol for 'reply' */
mextern(PSymbol) gSelfSymbol;          /* Pointer to unique Symbol for 'self' */
mextern(PSymbol) gStatusSymbol;        /* Pointer to unique Symbol for 'status' */
mextern(PSymbol) gUnboundSymbol;       /* Pointer to unique Symbol for 'unbound' */
mextern(PSymbol) gUnknownSymbol;       /* Pointer to unique Symbol for 'unknown' */
mextern(short)   gResourceBase;        /* Base value for resources */
    
#endif /* not TCPMAPPING_H_ */
