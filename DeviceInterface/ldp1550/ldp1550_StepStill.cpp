/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       ldp1550_StepStill.cpp                                                   */
/*                                                                                      */
/*  Contains:   The routine cmd_StepStill().                                            */
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

/*------------------------------------ cmd_StepStill ---*/
STEPSTILL_HEADER(LdpData)
{
    if (xx)
    {
        LdpCommandCode aCommand = kLdpNoCommand;

        if (xx->fFirst)
        {
            LOG_ERROR_1(xx, OUTPUT_PREFIX "attempt to send 'step&still' command while busy")
            outlet_bang(xx->fErrorBangOut);
        }
        else if ((mode == gEmptySymbol) || (mode == gFwdSymbol))
        {
            aCommand = kLdpCommandStepStillForward;
        }
        else if (mode == gRevSymbol)
        {
            aCommand = kLdpCommandStepStillReverse;
        }
        else
        {
            LOG_ERROR_1(xx, OUTPUT_PREFIX "bad argument to command 'step&still'")
            outlet_bang(xx->fErrorBangOut);
        }
        if (kLdpNoCommand != aCommand)
        {
            short prevLock = lockout_set(1);

            if (ldpCheckPoolSpace(xx, 3))
            {
                ldpInitCommands(xx);
                ldpAddCommand(xx, aCommand, kLdpStateAwaitingAck);
                ldpAddCommand(xx, kLdpCommandSignalAccepted, kLdpStateNotWaiting);
                ldpAddCommand(xx, kLdpCommandSignalDone, kLdpStateNotWaiting);
                ldpSendCommand(xx);
            }
            else
            {
                LOG_ERROR_1(xx, OUTPUT_PREFIX "no room for 'step&still' command")
                outlet_bang(xx->fErrorBangOut);
            }
            lockout_set(prevLock);
        }
    }
} // cmd_StepStill
