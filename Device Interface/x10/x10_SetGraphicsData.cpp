/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       x10_SetGraphicsData.cpp                                                 */
/*                                                                                      */
/*  Contains:   The routine cmd_SetGraphicsData().                                      */
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

/*------------------------------------ cmd_SetGraphicsData ---*/
void cmd_SetGraphicsData(X10ControlData * xx,
                         long             objectNumber,
                         long             objectData)
{
    bool okSoFar = true;

    if (xx->fMinorState != x10CP290MinorIdle)
    {
        LOG_ERROR_1(xx, OUTPUT_PREFIX "attempt to send command while awaiting response.")
        okSoFar = false;
    }
    if ((objectNumber < 0) || (objectNumber > 255))
    {
        LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid object number (%ld)", objectNumber)
        okSoFar = false;
    }
    if ((objectData < 0) || (objectData > 0x0FFFF))
    {
        LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid object data (%ld)", objectData)
        okSoFar = false;
    }
    if (okSoFar)
    {
        switch (xx->fKind)
        {
            case X10KindCM11:
                LOG_ERROR_1(xx, OUTPUT_PREFIX "function not yet implemented.")
                // TBD
                break;

            case X10KindCP290:
                *xx->fOutCmd = x10CP290DownloadGraphicsData;
                *xx->fOutArea = static_cast<unsigned char>((objectNumber << 1) & 0xFF);
                *(xx->fOutArea + 1) = static_cast<unsigned char>((objectNumber >> 7) | 0x4);
                *(xx->fOutArea + 2) = static_cast<unsigned char>((objectData >> 8) & 0xFF);
                *(xx->fOutArea + 3) = static_cast<unsigned char>(objectData & 0xFF);
                x10CP290CalcSum(xx, 0, 4);
                x10CP290SendBuffer(xx, 5);
                xx->fMajorState = x10CP290MajorIdle;
                outlet_bang(xx->fCommandComplete);
                break;

            default:
                break;
                
        }
    }
    else
    {
        outlet_bang(xx->fErrorBangOut);
    }
} // cmd_SetGraphicsData
