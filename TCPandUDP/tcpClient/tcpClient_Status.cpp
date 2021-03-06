/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       tcpClient_Status.cpp                                                    */
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

#include "tcpClient.h"

/*------------------------------------ cmd_Status ---*/
STATUS_HEADER(TcpObjectData)
{
    REPORT_MAX_MESSAGE("status")
    if (xx)
    {
        t_symbol * sym_state = mapStateToSymbol(xx->fState);
        t_atom     response[5];
        short      resp_length = 1;

        atom_setsym(response, sym_state);
        if (kTcpStateConnected == xx->fState)
        {
            if (! xx->fPartnerName)
            {
#if 0
                OSStatus result;
                TBind    self_address;
                TBind    other_address;
                char *   self_data = GET_BYTES(xx->fAddressSize, char);
                char *   other_data = GET_BYTES(xx->fAddressSize, char);

                if (self_data && other_data)
                {
                    self_address.addr.buf = reinterpret_cast<unsigned char *>(self_data);
                    self_address.addr.maxlen = xx->fAddressSize;
                    other_address.addr.buf = reinterpret_cast<unsigned char *>(other_data);
                    other_address.addr.maxlen = xx->fAddressSize;
                    WRAP_OT_CALL(xx, result, "OTGetProtAddress", OTGetProtAddress(xx->fSocket,
                                                                                  &self_address,
                                                                                  &other_address))
                    if (kOTNoError == result)
                    {
                        InetAddress * other_inet_ptr = reinterpret_cast<InetAddress *>(other_data);
                        InetAddress * self_inet_ptr = reinterpret_cast<InetAddress *>(self_data);
                        char          tempName[32];

                        OTInetHostToString(other_inet_ptr->fHost, tempName);
                        xx->fPartnerName = gensym(tempName);
                        xx->fClientPort = self_inet_ptr->fPort;
                    }
                    else
                    {
                        REPORT_ERROR(xx, OUTPUT_PREFIX "OTGetProtAddress failed (%ld = %s)", result)
                        reportEndpointState(OUR_NAME, xx);
                    }
                }
                FREE_BYTES(other_data);
                FREE_BYTES(self_data);
#endif//0
            }
            if (xx->fPartnerName)
            {
                atom_setsym(response + 1, xx->fPartnerName);
                atom_setlong(response + 2, static_cast<long>(xx->fServerPort));
                atom_setsym(response + 3, xx->fRawMode ? gRawSymbol : gMaxSymbol);
                resp_length += 3;
            }
        }
        outlet_anything(xx->fResultOut, gStatusSymbol, resp_length, response);
    }
} // cmd_Status
