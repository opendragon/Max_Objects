/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       udpPort_Port.cpp                                                        */
/*                                                                                      */
/*  Contains:   The routine cmd_Port().                                                 */
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

/*------------------------------------ cmd_Port ---*/
PORT_HEADER(UdpObjectData)
{
    REPORT_MAX_MESSAGE("port")
    if (xx)
    {
        if ((0 > number) || (MAX_PORT < number))
        {
            LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid port (%ld)", number)
            signalError(xx);
        }
        else if (xx->fSelfPort != number)
        {
            xx->fSelfPort = static_cast<unsigned short>(number);
            if (udpPortSetPort(xx, true))
            {
#if defined(BE_VERBOSE)
                if (xx->fVerbose)
                {
                    LOG_POST_2(xx, OUTPUT_PREFIX "port changed to %ld", number)
                }
#endif /* BE_VERBOSE */
            }
        }
    }
} // cmd_Port
