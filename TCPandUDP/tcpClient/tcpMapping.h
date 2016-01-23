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
# define TCPMAPPING_H_ /* */

# include "missingAndExtra.h"
# include "tcpBuffers.h"

# define NUM_RX_BUFFERS 25

# define DEFAULT_PORT 0x00FFFFL
# define MAX_PORT     0x00FFFFL

# define BE_VERBOSE /* */

# define USE_FORCED_DISCONNECTS true /* false for orderly disconnects */
// # define TRACE_OT_CALLS /* */
// # define TRACE_MAX_MESSAGES /* */

# define ADD_TO_ADDRESS(yy, amount) (reinterpret_cast<char *>(yy) + amount)

# define signalError(xx)   qelem_set(xx->fErrorQueue)
# define signalRebind(xx)  qelem_set(xx->fRebindQueue)
# define signalReceive(xx) qelem_set(xx->fReceiveQueue)

enum TcpState
{
    kTcpStateUnbound,
    kTcpStateBound,
    kTcpStateListening,
    kTcpStateConnecting,
    kTcpStateConnected,
    kTcpStateDisconnecting
}; // TcpState

struct TcpBufferLink
{
    TcpBufferLink * fPrevious;
    TcpBufferLink * fNext;
    DataBuffer *    fData;
# if defined(TCPMULTISERVER_H_)
    unsigned short  fIdentifier;
    bool            fRawMode;
# endif /* TCPMULTISERVER_H_ */
}; /* TcpBufferLink */

# if (defined(TRACE_OT_CALLS) && defined(BE_VERBOSE))
#  define WRAP_OT_CALL(xx, res, name, aCall) \
    { \
        res = aCall; \
        if (xx->fVerbose)\
        { \
            post(xx, OUTPUT_PREFIX name " --> (%ld = %s)", static_cast<long>(res), \
                    mapErrorCodeToString(res));\
        } \
    }
# else /* not TRACE_OT_CALLS or not BE_VERBOSE */
#  define WRAP_OT_CALL(xx, res, name, aCall) \
    res = aCall;
# endif /* not TRACE_OT_CALLS or not BE_VERBOSE */

# define REPORT_ERROR(xx, msg, value) \
    LOG_ERROR_3(xx, msg, static_cast<long>(value), mapErrorCodeToString(value))

#if 0
const char *
describeEndpointState(EndpointRef anEndpoint);
#endif//0

#if 0
const char *
mapEventToString(const OTEventCode code);
#endif//0

t_symbol *
mapStateToSymbol(const TcpState aState);

void
setUpStateSymbols(void);

mextern(t_symbol *) gBoundSymbol;         /* Pointer to unique symbol for 'bound' */
mextern(t_symbol *) gConnectedSymbol;     /* Pointer to unique symbol for 'connected' */
mextern(t_symbol *) gConnectingSymbol;    /* Pointer to unique symbol for 'connecting' */
mextern(t_symbol *) gDisconnectingSymbol; /* Pointer to unique symbol for 'disconnecting' */
mextern(t_symbol *) gEmptySymbol;         /* Pointer to unique symbol for '' */
mextern(t_symbol *) gListeningSymbol;     /* Pointer to unique symbol for 'listening' */
mextern(t_symbol *) gMaxSymbol;           /* Pointer to unique symbol for 'max' */
mextern(t_symbol *) gOffSymbol;           /* Pointer to unique symbol for 'off' */
mextern(t_symbol *) gOnSymbol;            /* Pointer to unique symbol for 'on' */
mextern(t_symbol *) gRawSymbol;           /* Pointer to unique symbol for 'raw' */
mextern(t_symbol *) gReplySymbol;         /* Pointer to unique symbol for 'reply' */
mextern(t_symbol *) gSelfSymbol;          /* Pointer to unique symbol for 'self' */
mextern(t_symbol *) gStatusSymbol;        /* Pointer to unique symbol for 'status' */
mextern(t_symbol *) gUnboundSymbol;       /* Pointer to unique symbol for 'unbound' */
mextern(t_symbol *) gUnknownSymbol;       /* Pointer to unique symbol for 'unknown' */

#endif /* not TCPMAPPING_H_ */
