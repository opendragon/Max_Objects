/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       gvp100_In1.cpp                                                          */
/*                                                                                      */
/*  Contains:   The routine cmd_In1().                                                  */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2013 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2013/10/14                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "gvp100.h"

/*------------------------------------ cmd_In1 ---*/
void cmd_In1(GvpData * xx,
             long      rr)
{
    /* We've received a byte. Check if it matches what we are expecting. */
    if (xx && (! xx->fStopping))
    {
        short       prevLock = lockout_set(1);
        GvpPacket * aPacket;
        long        incoming = (rr & 0x00ff);

#if defined(BE_VERBOSE)
        if (xx->fVerbose)
        {
            LOG_POST_2(xx, OUTPUT_PREFIX "saw byte: 0x%lx", incoming)
        }
#endif /* BE_VERBOSE */
        switch (xx->fState)
        {
            case kStateIdle:
                LOG_ERROR_2(xx, OUTPUT_PREFIX "unexpected character 0x%lx seen.", incoming)
                outlet_bang(xx->fErrorBangOut);
                break;

            case kStateAwaitingAck:
                if (GVP_ACK_CHAR == incoming)
                {
                    xx->fState = kStateSelected;
                    gvpReportStateChange(xx);
                }
                break;

            case kStateSelected:
                if (GVP_NACK_CHAR == incoming)
                {
                    LOG_ERROR_1(xx, OUTPUT_PREFIX "NACK seen.")
                    outlet_bang(xx->fErrorBangOut);
                    xx->fState = kStateIdle;
                    gvpReportStateChange(xx);
                }
                break;

            case kStateAwaitingByteCount1:
                if (1 == incoming)
                {
#if defined(BE_VERBOSE)
                    if (xx->fVerbose)
                    {
                        LOG_POST_1(xx, OUTPUT_PREFIX "saw byte count of 1.")
                    }
#endif /* BE_VERBOSE */
                    xx->fState = kStateAwaitingStatusByte;
                    gvpReportStateChange(xx);
                }
                else
                {
                    LOG_ERROR_1(xx, OUTPUT_PREFIX "bad byte count seen.")
                    outlet_bang(xx->fErrorBangOut);
                }
                break;

            case kStateAwaitingByteCountN:
#if defined(BE_VERBOSE)
                if (xx->fVerbose)
                {
                    LOG_POST_2(xx, OUTPUT_PREFIX "saw byte count of %ld.", incoming)
                }
#endif /* BE_VERBOSE */
                break;

            case kStateAwaitingStatusByte:
#if defined(BE_VERBOSE)
                if (xx->fVerbose)
                {
                    LOG_POST_2(xx, OUTPUT_PREFIX "status 0x%lx", incoming)
                }
#endif /* BE_VERBOSE */
                if (incoming & GVP_STATUS_BUFFER_WARNING)
                {
                    LOG_POST_1(xx, OUTPUT_PREFIX "buffer warning issued.")
                }
                if (incoming & GVP_STATUS_PROTOCOL_ERROR)
                {
                    LOG_ERROR_1(xx, OUTPUT_PREFIX "protocol error issued.")
                    outlet_bang(xx->fErrorBangOut);
                }
                else if (! (incoming & GVP_STATUS_COMMAND_ACCEPTED))
                {
                    LOG_POST_1(xx, OUTPUT_PREFIX "invalid status detected.")
                }
                if (xx->fSendCompletion)
                {
                    outlet_bang(xx->fCommandComplete);
                    xx->fSendCompletion = false;
#if defined(BE_VERBOSE)
                    if (xx->fVerbose)
                    {
                        LOG_POST_1(xx, OUTPUT_PREFIX "completion signalled")
                    }
#endif /* BE_VERBOSE */
                }
                /* Are there any messages waiting to go? */
                aPacket = gvpGetFirstPacket(xx);
                if (aPacket)
                {
                    if (aPacket->fSize >= 0)
                    {
                        unsigned char dataValue;
                        static t_atom dataList[128];

                        for (short ii = 0; ii < aPacket->fSize; ++ii)
                        {
                            dataValue = aPacket->fBuffer[ii];
                            SETLONG(dataList + ii, dataValue);
                        }
                        outlet_list(xx->fDataSendOut, 0L, aPacket->fSize, dataList);
                        xx->fSendCompletion = aPacket->fSendCompletion;
                    }
                    else
                    {
                        /* EndSequence to be processed. */
                        outlet_bang(xx->fSequenceComplete);
                    }
                    xx->fState = aPacket->fState;
                    gvpReportStateChange(xx);
                    gvpReleasePacket(xx, aPacket);
                }
                else
                {
                    xx->fState = kStateSelected;
                    gvpReportStateChange(xx);
                }
                break;

            default:
                break;
        }
        lockout_set(prevLock);
    }
} // cmd_In1
