/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       tcpClient_Server.cpp                                                    */
/*                                                                                      */
/*  Contains:   The routine cmd_Server().                                               */
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
/*  Created:    1998/09/24                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "tcpClient.h"

/*------------------------------------ cmd_Server ---*/
void cmd_Server(TcpObjectData * xx,
                t_symbol *      ipAddress)
{
    REPORT_MAX_MESSAGE("server")
    if (xx)
    {
        short byte_0 = DEFAULT_IP_ADDR_0;
        short byte_1 = DEFAULT_IP_ADDR_1;
        short byte_2 = DEFAULT_IP_ADDR_2;
        short byte_3 = DEFAULT_IP_ADDR_3;

        if (checkIpString(ipAddress, byte_0, byte_1, byte_2, byte_3))
        {
            unsigned long tempAddress = static_cast<unsigned long>(((byte_0 & 0x0FF) << 24) | ((byte_1 & 0x0FF) << 16) |
                                                                   ((byte_2 & 0x0FF) << 8) | (byte_3 & 0x0FF));

            if (tempAddress != xx->fServerAddress)
            {
                if (tcpClientSetServer(xx, byte_0, byte_1, byte_2, byte_3, true))
                {
#if defined(BE_VERBOSE)
                    if (xx->fVerbose)
                    {
                        LOG_POST_5(xx, OUTPUT_PREFIX "server changed to %d.%d.%d.%d", static_cast<int>(byte_0),
                                   static_cast<int>(byte_1), static_cast<int>(byte_2), static_cast<int>(byte_3))
                    }
#endif /* BE_VERBOSE */
                }
            }
        }
        else
        {
            LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid host address '%s'", ipAddress->s_name)
            signalError(xx);
        }
    }
} // cmd_Server
