/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       x10_SetHouseCode.cpp                                                    */
/*                                                                                      */
/*  Contains:   The routine cmd_SetHouseCode().                                         */
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

#include "x10.h"

/*------------------------------------ cmd_SetHouseCode ---*/
SETHOUSECODE_HEADER(X10ControlData)
{
    bool           okSoFar = true;
    unsigned short aCode = x10IdentifyHouseCode(code);

    switch (xx->fKind)
    {
        case X10KindCM11:
            if (x10CM11MinorIdle != xx->fMinorState)
            {
                LOG_ERROR_1(xx, OUTPUT_PREFIX "attempt to send command while awaiting response.")
                okSoFar = false;
            }
            break;

        case X10KindCP290:
            if (x10CP290MinorIdle != xx->fMinorState)
            {
                LOG_ERROR_1(xx, OUTPUT_PREFIX "attempt to send command while awaiting response.")
                okSoFar = false;
            }
            break;

        default:
            break;
            
    }
    if (! aCode)
    {
        LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid housecode (%s)", code->s_name)
        okSoFar = false;
    }
    if (okSoFar)
    {
        switch (xx->fKind)
        {
            case X10KindCM11:
                xx->fHouseCodeChar = (aCode - 1);
                x10CM11SendClockInfo(xx, false, true);
                break;

            case X10KindCP290:
                xx->fCompletedWhenStatus = true;
                *xx->fOutCmd = x10CP290DownloadBaseHouseCode;
                *xx->fOutArea = static_cast<unsigned char>(kHouseCodeToBits[aCode - 1] << 4);
                x10CP290SendBuffer(xx, 1);
                xx->fMajorState = x10CP290MajorAwaitingACK;
                break;

            default:
                break;
        }
    }
    else
    {
        outlet_bang(xx->fErrorBangOut);
    }
} // cmd_SetHouseCode
