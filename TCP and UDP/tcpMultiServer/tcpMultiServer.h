/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       tcpMultiServer.h                                                        */
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

#if (! defined(TCPMULTISERVER_H_))
# define TCPMULTISERVER_H_ /* */

# include "missingAndExtra.h"
# include "tcpMapping.h"

# define OUR_NAME      "tcpMultiServer"
// # define OUR_RES_NUMB  17144
# define OUTPUT_PREFIX "tcpMultiServer: "

# define MAX_CLIENTS 100

struct TcpMultiServerData;

struct TcpConnectionData
{
    TcpMultiServerData * fOwner;
    DataBuffer *         fReceiveBuffer;
    DataBuffer *         fSendBuffer;
#if 0
    EndpointRef          fDataEndpoint;
#endif//0
    t_symbol *           fPartnerName;
    unsigned long        fClientAddress;
    unsigned long        fClientPort;
    UInt32               fServiceType;
    TcpState             fState;
    unsigned short       fIdentifier;
    bool                 fActive;
    bool                 fRawMode;
}; // TcpConnectionData

struct TcpMultiServerData
{
    t_object             fObject;
    t_outlet *           fErrorBangOut;
    t_outlet *           fResultOut;
    t_qelem *            fErrorQueue;
    t_qelem *            fRebindQueue;
    t_qelem *            fReceiveQueue;
#if 0
    AccessOTControlPtr   fAccessControl;
#endif//0
    TcpBufferLink **     fLinkBase;
    TcpBufferLink *      fPoolHead;
    TcpBufferLink *      fPoolTail;
    DataBuffer **        fBufferBase;
    TcpBufferLink *      fReceiveHead;
    TcpBufferLink *      fReceiveTail;
#if 0
    EndpointRef          fListenEndpoint;
#endif//0
    t_symbol *           fSelfName;
    unsigned long        fServerAddress;
    unsigned short       fServerPort;
    UInt32               fServiceType;
    short                fAddressSize;
    TcpState             fState;
    bool                 fClosing;
    bool                 fVerbose;
    unsigned short       fActiveConnections;
    unsigned short       fMaximumConnections;
    TcpConnectionData ** fConnectionBase;
    TcpConnectionData ** fConnections;
#if 0
    OTNotifyUPP          fDataNotifier;
    OTNotifyUPP          fListenNotifier;
#endif//0
}; // TcpMultiServerData

void cmd_Disconnect(TcpMultiServerData * xx,
                    long                 client);

void cmd_Listen(TcpMultiServerData * xx,
                t_symbol *           onOff);

void cmd_Mode(TcpMultiServerData * xx,
              long                 client,
              t_symbol *           rawOrCooked);

void cmd_Port(TcpMultiServerData * xx,
              long                 number);

void cmd_Self(TcpMultiServerData * xx);

void cmd_Send(TcpMultiServerData * xx,
              t_symbol *           message,
              short                argc,
              t_atom *             argv);

void cmd_Status(TcpMultiServerData * xx,
                long                 client);

VERBOSE_HEADER(TcpMultiServerData);

bool initObject(TcpMultiServerData * xx,
                long                 port,
                long                 numBuffers);

bool makeReceiveBufferAvailable(TcpConnectionData * connection);

void presetObjectPointers(TcpMultiServerData * xx);

void releaseObjectMemory(TcpMultiServerData * xx);

#if 0
void reportEndpointState(TcpMultiServerData * xx,
                         EndpointRef          endpoint);
#endif//0

void setConnectionState(TcpConnectionData * connection,
                        const TcpState      newState);

void setObjectState(TcpMultiServerData * xx,
                    const TcpState       newState);

bool tcpMultiServerDisconnect(TcpMultiServerData * xx,
                              TcpConnectionData *  connection,
                              const bool           forced);

#if 0
pascal void tcpMultiServerDataNotifier(void *      context,
                                       OTEventCode code,
                                       OTResult    result,
                                       void *      cookie);
#endif//0

#if 0
pascal void tcpMultiServerListenNotifier(void *      context,
                                         OTEventCode code,
                                         OTResult    result,
                                         void *      cookie);
#endif//0

bool tcpMultiServerSetPort(TcpMultiServerData * xx,
                           const bool           bangOnError);

TcpConnectionData * tcpMultiServerValidateClient(TcpMultiServerData * xx,
                                                 const long           client);

#if 0
void transmitBuffer(TcpMultiServerData * xx,
                    EndpointRef          out,
                    DataBuffer *         aBuffer,
                    const bool           rawMode);
#endif//0

StandardRoutineDeclarations(TcpMultiServerData);

#endif /* not TCPMULTISERVER_H_ */
