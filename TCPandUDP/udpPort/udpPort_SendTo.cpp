/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       udpPort_SendTo.cpp                                                      */
/*                                                                                      */
/*  Contains:   The routine cmd_SendTo().                                               */
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

/*------------------------------------ cmd_SendTo ---*/
SENDTO_HEADER(UdpObjectData)
{
    REPORT_MAX_MESSAGE("server")
    if (xx)
    {
        if ((0 > portNumber) || (MAX_PORT < portNumber))
        {
            LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid port (%ld)", portNumber)
            signalError(xx);
        }
        else if ((ipAddress == gEmptySymbol) || (1 == inet_pton(AF_INET, ipAddress->s_name,
                                                                &xx->fPartnerAddress)))
        {
            xx->fPartnerPort = static_cast<unsigned short>(portNumber);
            xx->fPartnerKnown = true;
        }
        else
        {
            LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid host address '%s'", ipAddress->s_name)
            signalError(xx);
        }
    }
} // cmd_SendTo
