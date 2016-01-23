/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       tcpMultiServer_Disconnect.cpp                                           */
/*                                                                                      */
/*  Contains:   The routine cmd_Disconnect().                                           */
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

#include "tcpMultiServer.h"

/*------------------------------------ cmd_Disconnect ---*/
DISCONNECT_HEADER(TcpMultiServerData)
{
    REPORT_MAX_MESSAGE("disconnect")
    if (xx)
    {
        TcpConnectionData * connection;

        if (client)
        {
            connection = tcpMultiServerValidateClient(xx, client);
            if (connection)
            {
                if (tcpMultiServerDisconnect(xx, connection, USE_FORCED_DISCONNECTS))
                {
#if defined(BE_VERBOSE)
                    if (xx->fVerbose)
                    {
                        LOG_POST_1(xx, OUTPUT_PREFIX "disconnecting from client")
                    }
#endif /* BE_VERBOSE */
                }
            }
            else
            {
                LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid client (%ld)", client)
                signalError(xx);
            }
        }
        else
        {
            for (short ii = 0; ii < xx->fMaximumConnections; ++ii)
            {
                connection = *(xx->fConnections + ii);
#if 0
                if (connection && (kOTInvalidEndpointRef != connection->fDataEndpoint) &&
                    connection->fActive)
                {
                    tcpMultiServerDisconnect(xx, connection, true);
                }
#endif//0
            }
        }
    }
 } // cmd_Disconnect
