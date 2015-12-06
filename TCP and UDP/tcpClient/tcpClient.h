/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       tcpClient.h                                                             */
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

#if (! defined(TCPCLIENT_H_))
# define TCPCLIENT_H_ /* */

# include "tcpCommon.h"

# define OUR_DESC    "TCP/IP client"
# define OUR_NAME    "tcpClient"
# define OUR_VERSION "1.1.7"
# define OUR_YEAR    "1998"

# define DEFAULT_IP_ADDR_0 10
# define DEFAULT_IP_ADDR_1 11
# define DEFAULT_IP_ADDR_2 12
# define DEFAULT_IP_ADDR_3 13

bool checkIpString(t_symbol * ipAddress,
                   short &    byte_0,
                   short &    byte_1,
                   short &    byte_2,
                   short &    byte_3);

# define CONNECT_HEADER(type) \
    void cmd_Connect(type * xx)

# define SERVER_HEADER(type) \
    void cmd_Server(type *     xx,\
                    t_symbol * ipAddress)

CONNECT_HEADER(TcpObjectData);
SERVER_HEADER(TcpObjectData);

#if 0
pascal void tcpClientNotifier(void *      context,
                              OTEventCode code,
                              OTResult    result,
                              void *      cookie);
#endif//0

bool tcpClientConnect(TcpObjectData * xx);

bool tcpClientDisconnect(TcpObjectData * xx,
                         const bool      forced);

bool tcpClientSetPort(TcpObjectData * xx,
                      const bool      bangOnError);

bool tcpClientSetServer(TcpObjectData * xx,
                        const short     byte_0,
                        const short     byte_1,
                        const short     byte_2,
                        const short     byte_3,
                        const bool      bangOnError);

#endif /* not TCPCLIENT_H_ */
