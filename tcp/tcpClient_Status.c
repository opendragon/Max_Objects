/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       tcpClient_Status.c                                                      */
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
Pvoid cmd_Status
  (TcpObjectPtr xx)
{
  EnterCallback();
  REPORT_MAX_MESSAGE("status")
  if (xx)
  {
#if OPEN_TRANSPORT_SUPPORTED
    PSymbol sym_state = mapStateToSymbol(xx->fState);
    Atom    response[5];
    short   resp_length = 1;

    SETSYM(response, sym_state);
    if (xx->fState == TCP_OBJECT_CONNECTED)
    {
      if (! xx->fPartnerName)
      {
        OSStatus result;
        TBind    self_address, other_address;
        Ptr      self_data = GETBYTES(xx->fAddressSize, char);
        Ptr      other_data = GETBYTES(xx->fAddressSize, char);

        if (self_data && other_data)
        {
          self_address.addr.buf = reinterpret_cast<Puchar>(self_data);
          self_address.addr.maxlen = xx->fAddressSize;
          other_address.addr.buf = reinterpret_cast<Puchar>(other_data);
          other_address.addr.maxlen = xx->fAddressSize;
          WRAP_OT_CALL(xx, result, "OTGetProtAddress", OTGetProtAddress(xx->fEndpoint,
                                                                        &self_address,
                                                                        &other_address))
          if (result == kOTNoError)
          {
            InetAddress * other_inet_ptr = reinterpret_cast<InetAddress*>(other_data);
            InetAddress * self_inet_ptr = reinterpret_cast<InetAddress*>(self_data);
            char          tempName[32];

            OTInetHostToString(other_inet_ptr->fHost, tempName);
            xx->fPartnerName = gensym(tempName);
            xx->fClientPort = self_inet_ptr->fPort;
          }
          else
          {
            REPORT_ERROR(OUTPUT_PREFIX "OTGetProtAddress failed (%ld = %s)", result)
		        reportEndpointState(OUR_NAME, xx);
          }
        }
        FREEBYTES(other_data, xx->fAddressSize)
        FREEBYTES(self_data, xx->fAddressSize)
      }
      if (xx->fPartnerName)
      {
        SETSYM(response + 1, xx->fPartnerName);
        SETLONG(response + 2, long(xx->fServerPort));
        SETSYM(response + 3, xx->fRawMode ? gRawSymbol : gMaxSymbol);
        resp_length += 3;
      }
    }
    outlet_anything(xx->fResultOut, gStatusSymbol, resp_length, response);
#endif /* OPEN_TRANSPORT_SUPPORTED */
  }
  ExitMaxMessageHandler()
} /* cmd_Status */
