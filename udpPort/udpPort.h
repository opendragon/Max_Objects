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
# define UDPPORT_H_ /* */

# include "missingAndExtra.h"
# include "udpBuffers.h"

# define NUM_RX_BUFFERS 25

# define DEFAULT_PORT 0x00FFFFL
# define MAX_PORT     0x00FFFFL

# define BE_VERBOSE /* */

// # define TRACE_OT_CALLS /* */
// # define TRACE_MAX_MESSAGES /* */

# define ADD_TO_ADDRESS(yy, amount) (reinterpret_cast<char *>(yy) + amount)

# define signalError(xx)   qelem_set(xx->fErrorQueue)
# define signalReceive(xx) qelem_set(xx->fReceiveQueue)

# define OUR_NAME      "udpPort"
// # define OUR_RES_NUMB  17181
# define OUTPUT_PREFIX "udpPort: "

enum UdpState
{
    kUdpStateUnbound,
    kUdpStateBound
}; // UdpState

struct UdpBufferLink
{
    UdpBufferLink * fPrevious;
    UdpBufferLink * fNext;
    DataBuffer *    fData;
}; // UdpBufferLink

struct UdpObjectData
{
    t_object           fObject;
    t_outlet *         fErrorBangOut;
    t_outlet *         fResultOut;
    t_qelem *          fErrorQueue;
    t_qelem *          fReceiveQueue;
    UdpBufferLink **   fLinkBase;
    UdpBufferLink *    fReceiveHead;
    UdpBufferLink *    fReceiveTail;
    UdpBufferLink *    fPoolHead;
    UdpBufferLink *    fPoolTail;
    DataBuffer **      fBufferBase;
    DataBuffer *       fReceiveBuffer;
    DataBuffer *       fSendBuffer;
    CFSocketRef        fSocket;
    unsigned long      fSelfAddress;
    t_symbol *         fSelfName;
    unsigned short     fSelfPort;
    UInt32             fServiceType;
    in_addr            fPartnerAddress;
    unsigned short     fPartnerPort;
    short              fAddressSize;
    UdpState           fState;
    bool               fClosing;
    bool               fPartnerKnown;
    bool               fRawMode;
    bool               fVerbose;
}; // UdpObjectData

void cmd_Mode(UdpObjectData * xx,
              t_symbol *      rawOrCooked);

void cmd_Port(UdpObjectData * xx,
              long            number);

void cmd_Self(UdpObjectData * xx);

void cmd_Send(UdpObjectData * xx,
              t_symbol *      message,
              short           argc,
              t_atom *        argv);

void cmd_SendTo(UdpObjectData * xx,
                t_symbol *      ipAddress,
                long            portNumber);

void cmd_Status(UdpObjectData * xx);

void cmd_Verbose(UdpObjectData * xx,
                 t_symbol *      onOff);

bool initObject(UdpObjectData * xx,
                const long      port,
                const long      numBuffers);

bool makeReceiveBufferAvailable(UdpObjectData * xx);

t_symbol * mapStateToSymbol(const UdpState aState);

void presetObjectPointers(UdpObjectData * xx);

void processReceiveQueue(UdpObjectData * xx);

void releaseObjectMemory(UdpObjectData * xx);

void setObjectState(UdpObjectData * xx,
                    const UdpState  newState);

void setUpStateSymbols(void);

void transmitBuffer(UdpObjectData * xx,
                    DataBuffer *    aBuffer);

bool udpPortSetPort(UdpObjectData * xx,
                    const bool      bangOnError);

StandardRoutineDeclarations(UdpObjectData *);

# if (defined(TRACE_OT_CALLS) && defined(BE_VERBOSE))
#  define WRAP_OT_CALL(xx, res, name, aCall) \
    { \
        res = aCall; \
        if (xx->fVerbose)\
        { \
            post(xx, OUTPUT_PREFIX name " --> (%ld = %s)", static_cast<long>(res), mapErrorCodeToString(res));\
        } \
    }
# else /* not TRACE_OT_CALLS or not BE_VERBOSE */
#  define WRAP_OT_CALL(xx, res, name, aCall) \
    res = aCall;
# endif /* not TRACE_OT_CALLS or not BE_VERBOSE */

# define REPORT_ERROR(xx, msg, value) \
    LOG_ERROR_3(xx, msg, static_cast<long>(value), mapErrorCodeToString(value))

mextern(t_symbol *) gBoundSymbol;   /* Pointer to unique symbol for 'bound' */
mextern(t_symbol *) gEmptySymbol;   /* Pointer to unique symbol for '' */
mextern(t_symbol *) gFromSymbol;    /* Pointer to unique symbol for 'from' */
mextern(t_symbol *) gMaxSymbol;     /* Pointer to unique symbol for 'max' */
mextern(t_symbol *) gOffSymbol;     /* Pointer to unique symbol for 'off' */
mextern(t_symbol *) gOnSymbol;      /* Pointer to unique symbol for 'on' */
mextern(t_symbol *) gRawSymbol;     /* Pointer to unique symbol for 'raw' */
mextern(t_symbol *) gSelfSymbol;    /* Pointer to unique symbol for 'self' */
mextern(t_symbol *) gStatusSymbol;  /* Pointer to unique symbol for 'status' */
mextern(t_symbol *) gUnboundSymbol; /* Pointer to unique symbol for 'unbound' */
mextern(t_symbol *) gUnknownSymbol; /* Pointer to unique symbol for 'unknown' */

#endif /* not UDPPORT_H_ */
