/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       ldp1550_In1.cpp                                                         */
/*                                                                                      */
/*  Contains:   The routine cmd_In1().                                                  */
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

/*------------------------------------ checkResponse ---*/
static void checkResponse(LdpData *  xx,
                          const long rr)
{
    switch (rr)
    {
        case kLdpReturnError:
            LOG_ERROR_1(xx, OUTPUT_PREFIX "device response = ERROR")
            break;

        case kLdpReturnLidOpen:
            LOG_ERROR_1(xx, OUTPUT_PREFIX "device response = LID OPEN")
            break;

        case kLdpReturnNotTarget:
            LOG_ERROR_1(xx, OUTPUT_PREFIX "device response = NOT TARGET")
            break;

        case kLdpReturnNoFrame:
            LOG_ERROR_1(xx, OUTPUT_PREFIX "device response = NO FRAME")
            break;

        case kLdpReturnNack:
            LOG_ERROR_1(xx, OUTPUT_PREFIX "device response = NACK")
            break;

        default:
            LOG_ERROR_2(xx, OUTPUT_PREFIX "unexpected input %02.2lx", rr)
            break;
    }
} /* checkResponse */

/*------------------------------------ cmd_In1 ---*/
void cmd_In1(LdpData * xx,
             long      rr)
{
    /* We've received a byte. Check if it matches what we are expecting. */
    if (xx)
    {
        /* Peek at the first packet: Don't grab it until we're happy! */
        bool        canAdvance = false;
        bool        inError = false;
        short       prevLock = lockout_set(1);
        LdpPacket * aPacket = xx->fFirst;

        if (aPacket)
        {
            switch (aPacket->fState)
            {
                case kLdpStateAwaitingAck:
                    if (kLdpReturnAck == rr)
                    {
                        canAdvance = true;
                    }
                    else
                    {
                        checkResponse(xx, rr);
                        inError = true;
                    }
                    break;

                case kLdpStateAwaitingComplete:
                    if (kLdpReturnComplete == rr)
                    {
                        canAdvance = true;
                        if (xx->fInterruptPoint == aPacket)
                        {
                            xx->fInterruptPoint = NULL_PTR;
                        }
                    }
                    else
                    {
                        checkResponse(xx, rr);
                        inError = true;
                    }
                    break;

                case kLdpStateAwaitingChapterByte1:
                    if ((rr >= '0') && (rr <= '9'))
                    {
                        aPacket->fState = kLdpStateAwaitingChapterByte2;
                        xx->fChapterNumber = static_cast<short>(rr - '0');
                    }
                    else
                    {
                        checkResponse(xx, rr);
                        xx->fChapterNumber = 0;
                        inError = true;
                    }
                    break;

                case kLdpStateAwaitingChapterByte2:
                    if ((rr >= '0') && (rr <= '9'))
                    {
                        xx->fChapterNumber = static_cast<short>(((xx->fChapterNumber) * 10) + rr - '0');
                        outlet_int(xx->fChapterNumberOut, static_cast<long>(xx->fChapterNumber));
                        canAdvance = true;
                    }
                    else
                    {
                        checkResponse(xx, rr);
                        inError = true;
                    }
                    break;

                case kLdpStateAwaitingFrameByte1:
                    if ((rr >= '0') && (rr <= '9'))
                    {
                        aPacket->fState = kLdpStateAwaitingFrameByte2;
                        xx->fFrameNumber = rr - '0';
                    }
                    else
                    {
                        checkResponse(xx, rr);
                        xx->fFrameNumber = 0;
                        inError = true;
                    }
                    break;

                case kLdpStateAwaitingFrameByte2:
                    if ((rr >= '0') && (rr <= '9'))
                    {
                        aPacket->fState = kLdpStateAwaitingFrameByte3;
                        xx->fFrameNumber = ((xx->fFrameNumber) * 10) + rr - '0';
                    }
                    else
                    {
                        checkResponse(xx, rr);
                        xx->fFrameNumber = 0;
                        inError = true;
                    }
                    break;

                case kLdpStateAwaitingFrameByte3:
                    if ((rr >= '0') && (rr <= '9'))
                    {
                        aPacket->fState = kLdpStateAwaitingFrameByte4;
                        xx->fFrameNumber = ((xx->fFrameNumber) * 10) + rr - '0';
                    }
                    else
                    {
                        checkResponse(xx, rr);
                        xx->fFrameNumber = 0;
                        inError = true;
                    }
                    break;

                case kLdpStateAwaitingFrameByte4:
                    if ((rr >= '0') && (rr <= '9'))
                    {
                        aPacket->fState = kLdpStateAwaitingFrameByte5;
                        xx->fFrameNumber = ((xx->fFrameNumber) * 10) + rr - '0';
                    }
                    else
                    {
                        checkResponse(xx, rr);
                        xx->fFrameNumber = 0;
                        inError = true;
                    }
                    break;

                case kLdpStateAwaitingFrameByte5:
                    if ((rr >= '0') && (rr <= '9'))
                    {
                        xx->fFrameNumber = ((xx->fFrameNumber) * 10) + rr - '0';
                        outlet_int(xx->fFrameNumberOut, xx->fFrameNumber);
                        canAdvance = true;
                    }
                    else
                    {
                        checkResponse(xx, rr);
                        inError = true;
                    }
                    break;

                case kLdpStateAwaitingStatusByte1:
                    aPacket->fState = kLdpStateAwaitingStatusByte2;
                    break;

                case kLdpStateAwaitingStatusByte2:
                    aPacket->fState = kLdpStateAwaitingStatusByte3;
                    break;

                case kLdpStateAwaitingStatusByte3:
                    aPacket->fState = kLdpStateAwaitingStatusByte4;
                    break;

                case kLdpStateAwaitingStatusByte4:
                    aPacket->fState = kLdpStateAwaitingStatusByte5;
                    outlet_int(xx->fKeyModeStatus, rr);
                    if (rr & 0x08)
                    {
                        outlet_bang(xx->fProgramStopCodeOut);
                    }
                    break;

                case kLdpStateAwaitingStatusByte5:
                    outlet_int(xx->fCommandStatus, rr);
                    canAdvance = true;
                    break;

                default:
                    break;
            }
        }
        else
        {
            checkResponse(xx, rr);
            inError = true;
        }
        if (canAdvance)
        {
            for ( ; ; )
            {
                /* Consume the first packet */
                aPacket = ldpGetFirstPacket(xx);
                ldpReleasePacket(xx, aPacket);
                aPacket = xx->fFirst;
                if ((! aPacket) || ((aPacket->fState != kLdpStateNotWaiting) &&
                                    (aPacket->fCommand != kLdpCommandSignalAccepted) &&
                                    (aPacket->fCommand != kLdpCommandSignalDone)))
                {
                    break;
                }

                if (kLdpCommandSignalDone == aPacket->fCommand)
                {
                    outlet_bang(xx->fCommandComplete);
                }
                else if (kLdpCommandSignalAccepted == aPacket->fCommand)
                {
                    outlet_bang(xx->fCommandAccepted);
                }
            }
        }
        if (inError)
        {
            outlet_bang(xx->fErrorBangOut);
            ldpClearPackets(xx);
        }
        else if (canAdvance && aPacket)
        {
            LdpCommandCode aCommand = aPacket->fCommand;

            if (kLdpCommandCanInterrupt == aCommand)
            {
                xx->fInterruptPoint = aPacket;
            }
            else if ((aCommand != kLdpNoCommand) && (aCommand != kLdpCommandSignalDone))
            {
                outlet_int(xx->fCommandsOut, static_cast<long>(aCommand));
            }
        }
        lockout_set(prevLock);
    }
} // cmd_In1
