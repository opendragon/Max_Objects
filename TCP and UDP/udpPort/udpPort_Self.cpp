/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       udpPort_Self.cpp                                                        */
/*                                                                                      */
/*  Contains:   The routine cmd_Self().                                                 */
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

#include "udpPort.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <net/if.h>

/*------------------------------------ cmd_Self ---*/
void cmd_Self(UdpObjectData * xx)
{
    REPORT_MAX_MESSAGE("self");
    if (xx && xx->fSocket)
    {
        t_atom response[1];

        if (! xx->fSelfName)
        {
            ifaddrs * myaddrs;

            if (! getifaddrs(&myaddrs))
            {
                char hostStr[NI_MAXHOST + 1];
                int  err;

                for (ifaddrs * ifa = myaddrs; ifa; ifa = ifa->ifa_next)
                {
                    if (ifa->ifa_addr && (ifa->ifa_flags & IFF_UP) &&
                        (! (ifa->ifa_flags & IFF_LOOPBACK)) &&
                        (AF_INET == ifa->ifa_addr->sa_family))
                    {
                        err = getnameinfo(reinterpret_cast<const sockaddr *>(ifa->ifa_addr),
                                          static_cast<socklen_t>(sizeof(sockaddr_in)), hostStr,
                                          sizeof(hostStr), NULL, 0, NI_NUMERICHOST);
                        if (0 == err)
                        {
                            xx->fSelfName = gensym(hostStr);
                            break;
                        }

                        LOG_ERROR_2(xx, OUTPUT_PREFIX "getnameinfo failed (%ld)",
                                    static_cast<long>(errno))
                    }
                }
                freeifaddrs(myaddrs);
            }
        }
        if (xx->fSelfName)
        {
            A_SETSYM(response, xx->fSelfName);
            outlet_anything(xx->fResultOut, gSelfSymbol, 1, response);
        }
    }
} // cmd_Self
