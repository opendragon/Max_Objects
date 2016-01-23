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
# define TCPCOMMON_H_ /* */

# include "tcpMapping.h"

struct TcpObjectData
{
    t_object           fObject;
    t_outlet *         fErrorBangOut;
    t_outlet *         fResultOut;
    t_qelem *          fErrorQueue;
    t_qelem *          fRebindQueue;
    t_qelem *          fReceiveQueue;
    TcpBufferLink **   fLinkBase;
    TcpBufferLink *    fReceiveHead;
    TcpBufferLink *    fReceiveTail;
    TcpBufferLink *    fPoolHead;
    TcpBufferLink *    fPoolTail;
    DataBuffer **      fBufferBase;
    DataBuffer *       fReceiveBuffer;
    DataBuffer *       fSendBuffer;
    CFSocketRef        fSocket;
    t_symbol *         fPartnerName;
    t_symbol *         fSelfName;
    unsigned long      fClientAddress;
    unsigned short     fClientPort;
    unsigned long      fServerAddress;
    unsigned short     fServerPort;
    UInt32             fServiceType;
    short              fAddressSize;
    TcpState           fState;
    bool               fClosing;
    bool               fRawMode;
    bool               fVerbose;
}; // TcpObjectData

# define DISCONNECT_HEADER(type) \
    void\
    cmd_Disconnect(type * xx)

# define MODE_HEADER(type) \
    void\
    cmd_Mode(type *     xx,\
             t_symbol * rawOrCooked)

# define SEND_HEADER(type) \
    void\
    cmd_Send(type *     xx,\
             t_symbol * message,\
             const long argc,\
             t_atom *   argv)

# define STATUS_HEADER(type) \
    void\
    cmd_Status(type * xx)

DISCONNECT_HEADER(TcpObjectData);
MODE_HEADER(TcpObjectData);
PORT_HEADER(TcpObjectData);
SELF_HEADER(TcpObjectData);
SEND_HEADER(TcpObjectData);
STATUS_HEADER(TcpObjectData);
VERBOSE_HEADER(TcpObjectData);

bool
initObject(const char *    name,
           TcpObjectData * xx,
           const long      port,
           const long      numBuffers);

bool
makeReceiveBufferAvailable(TcpObjectData * xx);

void
presetObjectPointers(TcpObjectData * xx);

void
processRebindQueue(TcpObjectData * xx);

void
processReceiveQueue(TcpObjectData * xx);

void
releaseObjectMemory(const char *    name,
                    TcpObjectData * xx);

void
reportEndpointState(const char *    name,
                    TcpObjectData * xx);

void
setObjectState(TcpObjectData *      xx,
               const TcpState newState);

void
transmitBuffer(TcpObjectData *  xx,
               DataBuffer *     aBuffer);

StandardRoutineDeclarations(TcpObjectData);

#endif /* not TCPCOMMON_H_ */
