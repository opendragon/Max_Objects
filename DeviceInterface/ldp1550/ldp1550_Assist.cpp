/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       lpd1550_Assist.cpp                                                      */
/*                                                                                      */
/*  Contains:   The routine cmd_Assist().                                               */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 1996 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    1996/09/21                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "ldp1550.h"

/*------------------------------------ cmd_Assist ---*/
ASSIST_HEADER(LdpData)
{
#pragma unused(xx, bb)
    if (ASSIST_INLET == msg)
    {
        switch (arg)
        {
            case 0:
                strncpy_zero(dstString, "Command channel", MAX_ASSISTANCE);
                break;

            case 1:
                strncpy_zero(dstString, "Serial device feedback", MAX_ASSISTANCE);
                break;
                
            default:
                break;
                
        }
    }
    else if (ASSIST_OUTLET == msg)
    {
        switch (arg)
        {
            case 0:
                strncpy_zero(dstString, "Commands to serial device", MAX_ASSISTANCE);
                break;

            case 1:
                strncpy_zero(dstString, "Poller output to serial device", MAX_ASSISTANCE);
                break;

            case 2:
                strncpy_zero(dstString, "Key mode status output", MAX_ASSISTANCE);
                break;

            case 3:
                strncpy_zero(dstString, "Command status output", MAX_ASSISTANCE);
                break;

            case 4:
                strncpy_zero(dstString, "Program stop code detected", MAX_ASSISTANCE);
                break;

            case 5:
                strncpy_zero(dstString, "Command complete output", MAX_ASSISTANCE);
                break;

            case 6:
                strncpy_zero(dstString, "Command accepted output", MAX_ASSISTANCE);
                break;

            case 7:
                strncpy_zero(dstString, "Chapter number output", MAX_ASSISTANCE);
                break;

            case 8:
                strncpy_zero(dstString, "Frame number output", MAX_ASSISTANCE);
                break;

            case 9:
                strncpy_zero(dstString, "Error detected output", MAX_ASSISTANCE);
                break;
                
            default:
                break;
                
        }
    }
} // cmd_Assist
