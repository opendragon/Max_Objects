/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       tcpMultiServer_Status.cpp                                               */
/*                                                                                      */
/*  Contains:   The routine cmd_Status().                                               */
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
/*  Created:    2000/10/15                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "tcpMultiServer.h"

/*------------------------------------ cmd_Status ---*/
STATUS_HEADER(TcpMultiServerData)
{
    if (xx)
    {
        t_atom response[5];

        if (client)
        {
            short               resp_length = 2;
            TcpConnectionData * connection = tcpMultiServerValidateClient(xx, client);

            if (connection)
            {
#if 0
                atom_setlong(response, client);
                atom_setsym(response + 1, mapStateToSymbol(connection->fState));
                if (kTcpStateConnected == connection->fState)
                {
                    if (! connection->fPartnerName)
                    {
                        OSStatus result;
                        TBind    other_address;
                        char *   other_data = GET_BYTES(xx->fAddressSize, char);

                        if (other_data)
                        {
                            other_address.addr.buf = reinterpret_cast<unsigned char *>(other_data);
                            other_address.addr.maxlen = xx->fAddressSize;
                            WRAP_OT_CALL(xx, result, "OTGetProtAddress",
                                         OTGetProtAddress(connection->fDataEndpoint, NULL,
                                                          &other_address))
                            if (kOTNoError == result)
                            {
                                InetAddress * other_inet_ptr =
                                                        reinterpret_cast<InetAddress *>(other_data);
                                char          temp_name[32];

                                OTInetHostToString(other_inet_ptr->fHost, temp_name);
                                connection->fPartnerName = gensym(temp_name);
                                connection->fClientAddress = other_inet_ptr->fHost;
                                connection->fClientPort = other_inet_ptr->fPort;
                            }
                            else
                            {
                                REPORT_ERROR(xx, OUTPUT_PREFIX "OTGetProtAddress failed (%ld = %s)",
                                             result)
                                reportEndpointState(xx, connection->fDataEndpoint);
                            }
                        }
                        FREE_BYTES(other_data);
                    }
                    if (connection->fPartnerName)
                    {
                        atom_setsym(response + 2, connection->fPartnerName);
                        atom_setlong(response + 3, TO_INT(connection->fClientPort));
                        atom_setsym(response + 4, connection->fRawMode ? gRawSymbol : gMaxSymbol);
                        resp_length += 3;
                    }
                }
                outlet_anything(xx->fResultOut, gStatusSymbol, resp_length, response);
#endif//0
            }
            else
            {
                LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid client (%ld)", client)
                signalError(xx);
            }
        }
        else
        {
            atom_setlong(response, 0L);
            atom_setsym(response + 1, mapStateToSymbol(xx->fState));
            atom_setlong(response + 2, xx->fActiveConnections);
            atom_setlong(response + 3, xx->fServerPort);
            outlet_anything(xx->fResultOut, gStatusSymbol, 4, response);
        }
    }
} // cmd_Status
