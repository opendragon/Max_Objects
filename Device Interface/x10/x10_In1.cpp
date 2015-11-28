/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       x10_In1.cpp                                                             */
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
/*  Created:    2013/10/12                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "x10.h"

/*------------------------------------ x10DoCM11 ---*/
static void x10DoCM11(X10ControlData * xx,
                      const long       rr)
{
    long workingCmd = (rr & 0x0ff);

    // post("    saw %lx state: %d[%d]", rr, xx->fMajorState, xx->fMinorState);//!!
    switch (xx->fMinorState)
    {
        case x10CM11MinorIdle:
            if (CM11_POLL_CHAR == workingCmd)
            {
                // Prepare to receive a status poll
                outlet_int(xx->fCommandsOut, CM11_POLL_ACK);
                xx->fMinorState = x10CM11MinorAwaitingByteCount;
            }
            else if (CM11_CLOCK_CHAR == workingCmd)
            {
                // Prepare to receive a clock request
                x10CM11SendClockInfo(xx, false, false);
            }
            break;

        case x10CM11MinorAwaitingChecksum:
            if (workingCmd == xx->fChecksum)
            {
                xx->fMinorState = x10CM11MinorAwaitingReady;
                outlet_int(xx->fCommandsOut, CM11_CHECKSUM_ACK);
            }
            else if ((xx->fIndex < xx->fOutCount) && (workingCmd == *(xx->fOutBuffer + xx->fIndex)))
            {
                // We're getting an echo!
                ++xx->fIndex;
            }
            else if (xx->fIgnoreChecksum)
            {
                xx->fMinorState = x10CM11MinorIdle;
                xx->fIgnoreChecksum = false;
                if (xx->fCompletedWhenStatus)
                {
                    outlet_bang(xx->fCommandComplete);
                }
            }
            else
            {
                x10CM11ResendBuffer(xx);
            }
            break;

        case x10CM11MinorAwaitingReady:
            if (CM11_READY_CHAR == workingCmd)
            {
                xx->fMinorState = x10CM11MinorIdle;
                // Determine if there are any more addresses
                if (x10CM11MajorSendingSingleRequest == xx->fMajorState)
                {
                    outlet_bang(xx->fCommandComplete);
                    xx->fMajorState = x10CM11MajorIdle;
                }
                else if (xx->fDeviceMap)
                {
                    x10CM11SendDeviceAddress(xx);
                }
                else if (x10CM11MajorSendingMultiRequest == xx->fMajorState)
                {
                    x10CM11SendFunctionCode(xx);
                    xx->fMajorState = x10CM11MajorIdle;
                }
                else if (xx->fCompletedWhenStatus)
                {
                    outlet_bang(xx->fCommandComplete);
                }
            }
            else if (CM11_CHECKSUM_ACK == workingCmd)
            {
                // We're getting an echo!
            }
            else
            {
                outlet_bang(xx->fErrorBangOut);
            }
            break;

        case x10CM11MinorAwaitingByteCount:
            if (workingCmd != CM11_POLL_CHAR)
            {
                xx->fIndex = static_cast<unsigned short>(workingCmd);
                xx->fMinorState = x10CM11MinorGettingPollChars;
            }
            break;

        case x10CM11MinorGettingPollChars:
            if (! --xx->fIndex)
            {
                xx->fMinorState = x10CM11MinorIdle;
            }
            break;

        default:
            break;

    }
} // x10DoCM11

/*------------------------------------ x10DoCP290 ---*/
static void x10DoCP290(X10ControlData * xx,
                       const long       rr)
{
    switch (xx->fMinorState)
    {
        case x10CP290MinorIdle:
            if (ALL_ON == rr)
            {
                xx->fMinorState = x10CP290MinorSawFF;
                xx->fAllOnCount = 1;
            }
            break;

        case x10CP290MinorSawFF:
            if (ALL_ON == rr)
            {
                ++xx->fAllOnCount;
            }
            else if (6 <= xx->fAllOnCount)
            {
                xx->fAllOnCount = 0;
                outlet_int(xx->fDeviceStatus, rr);
                switch (xx->fMajorState)
                {
                    case x10CP290MajorIdle:
                        /* Unexpected stuff? */
                        xx->fMinorState = x10CP290MinorIdle;
                        break;

                    case x10CP290MajorAwaitingACK:
                        outlet_bang(xx->fCommandComplete);
                        xx->fMajorState = x10CP290MajorIdle;
                        xx->fMinorState = x10CP290MinorIdle;
                        break;

                    case x10CP290MajorAwaitingACKAndUpload:
                        xx->fMinorState = x10CP290MinorIdle;
                        xx->fMajorState = x10CP290MajorAwaitingUpload;
                        break;

                    case x10CP290MajorAwaitingClockAndHouseCode:
                        xx->fMinorState = x10CP290MinorSawStatus;
                        break;

                    case x10CP290MajorAwaitingUpload:
                        xx->fMinorState = x10CP290MinorSawStatus;
                        break;

                    case x10CP290MajorAwaitingGraphicsData:
                        xx->fMinorState = x10CP290MinorSawStatus;
                        xx->fEntryCount = 0;
                        break;

                    case x10CP290MajorAwaitingTimerEvents:
                        xx->fMinorState = x10CP290MinorSawStatus;
                        xx->fEntryCount = 0;
                        break;

                    default:
                        xx->fMajorState = x10CP290MajorIdle;
                        xx->fMinorState = x10CP290MinorIdle;
                        break;
                        
                }
            }
            else
            {
                xx->fAllOnCount = 0;
                xx->fMinorState = x10CP290MinorIdle;
            }
            break;

            /* other cases might change minor state to x10CP290MinorSawStatus */
        case x10CP290MinorSawStatus:
            switch (xx->fMajorState)
            {
                case x10CP290MajorAwaitingUpload:
                    xx->fMinorState = x10CP290MinorSawHouseCode;
                    break;

                case x10CP290MajorAwaitingClockAndHouseCode:
                    outlet_int(xx->fMinuteOut, rr);
                    xx->fMinorState = x10CP290MinorSawMinutes;
                    break;

                case x10CP290MajorAwaitingGraphicsData:
                    if (ALL_ON == rr)
                    {
                        ++xx->fEntryCount;
                        xx->fMinorState = x10CP290MinorSawIcon2;
                    }
                    else
                    {
                        xx->fMinorState = x10CP290MinorSawIcon1;
                    }
                    break;

                case x10CP290MajorAwaitingTimerEvents:
                    if (ALL_ON == rr)
                    {
                        ++xx->fEntryCount;
                        xx->fMinorState = x10CP290MinorSawEvent8;
                    }
                    else
                    {
                        xx->fEventByteCount = 1;
                        xx->fMinorState = x10CP290MinorSawEvent1;
                    }
                    break;

                default:
                    xx->fMajorState = x10CP290MajorIdle;
                    xx->fMinorState = x10CP290MinorIdle;
                    break;
                    
            }
            break;

        case x10CP290MinorSawHouseCode:
            switch (xx->fMajorState)
            {
                case x10CP290MajorAwaitingUpload:
                    xx->fMinorState = x10CP290MinorSawUnits1;
                    break;

                default:
                    xx->fMajorState = x10CP290MajorIdle;
                    xx->fMinorState = x10CP290MinorIdle;
                    break;
                    
            }
            break;

        case x10CP290MinorSawUnits1:
            switch (xx->fMajorState)
            {
                case x10CP290MajorAwaitingUpload:
                    xx->fMinorState = x10CP290MinorSawUnits2;
                    break;

                default:
                    xx->fMajorState = x10CP290MajorIdle;
                    xx->fMinorState = x10CP290MinorIdle;
                    break;
                    
            }
            break;

        case x10CP290MinorSawUnits2:
            switch (xx->fMajorState)
            {
                case x10CP290MajorAwaitingUpload:
                    outlet_int(xx->fHouseCode, TO_INT(rr >> 4));
                    xx->fMinorState = x10CP290MinorSawBaseHouseCode;
                    break;

                default:
                    xx->fMajorState = x10CP290MajorIdle;
                    xx->fMinorState = x10CP290MinorIdle;
                    break;
                    
            }
            break;

        case x10CP290MinorSawBaseHouseCode:
            switch (xx->fMajorState)
            {
                case x10CP290MajorAwaitingUpload:
                    outlet_bang(xx->fCommandComplete);
                    xx->fMajorState = x10CP290MajorIdle;
                    xx->fMinorState = x10CP290MinorIdle;
                    break;

                case x10CP290MajorAwaitingClockAndHouseCode:
                    outlet_bang(xx->fCommandComplete);
                    xx->fMajorState = x10CP290MajorIdle;
                    xx->fMinorState = x10CP290MinorIdle;
                    break;

                default:
                    xx->fMajorState = x10CP290MajorIdle;
                    xx->fMinorState = x10CP290MinorIdle;
                    break;
                    
            }
            break;

        case x10CP290MinorSawMinutes:
            switch (xx->fMajorState)
            {
                case x10CP290MajorAwaitingClockAndHouseCode:
                    outlet_int(xx->fHourOut, rr);
                    xx->fMinorState = x10CP290MinorSawHours;
                    break;

                default:
                    xx->fMajorState = x10CP290MajorIdle;
                    xx->fMinorState = x10CP290MinorIdle;
                    break;
                    
            }
            break;

        case x10CP290MinorSawHours:
            switch (xx->fMajorState)
            {
                case x10CP290MajorAwaitingClockAndHouseCode:
                    outlet_int(xx->fDayOut, rr);
                    xx->fMinorState = x10CP290MinorSawDays;
                    break;

                default:
                    xx->fMajorState = x10CP290MajorIdle;
                    xx->fMinorState = x10CP290MinorIdle;
                    break;
                    
            }
            break;

        case x10CP290MinorSawDays:
            switch (xx->fMajorState)
            {
                case x10CP290MajorAwaitingClockAndHouseCode:
                    outlet_int(xx->fHouseCode, TO_INT(rr >> 4));
                    xx->fMinorState = x10CP290MinorSawBaseHouseCode;
                    break;

                default:
                    xx->fMajorState = x10CP290MajorIdle;
                    xx->fMinorState = x10CP290MinorIdle;
                    break;
                    
            }
            break;

        case x10CP290MinorSawIcon1:
            switch (xx->fMajorState)
            {
                case x10CP290MajorAwaitingGraphicsData:
                    xx->fMinorState = x10CP290MinorSawIcon2;
                    break;

                default:
                    xx->fMajorState = x10CP290MajorIdle;
                    xx->fMinorState = x10CP290MinorIdle;
                    break;
                    
            }
            break;

        case x10CP290MinorSawIcon2:
            switch (xx->fMajorState)
            {
                case x10CP290MajorAwaitingGraphicsData:
                    if (256 <= xx->fEntryCount)
                    {
                        outlet_bang(xx->fCommandComplete);
                        xx->fEntryCount = 0;
                        xx->fMajorState = x10CP290MajorIdle;
                        xx->fMinorState = x10CP290MinorIdle;
                    }
                    else if (ALL_ON == rr)
                    {
                        ++xx->fEntryCount;
                    }
                    else
                    {
                        xx->fMinorState = x10CP290MinorSawIcon1;
                    }
                    break;

                default:
                    xx->fMajorState = x10CP290MajorIdle;
                    xx->fMinorState = x10CP290MinorIdle;
                    break;
                    
            }
            break;

        case x10CP290MinorSawEvent1:
            switch (xx->fMajorState)
            {
                case x10CP290MajorAwaitingTimerEvents:
                    if (8 == xx->fEventByteCount)
                    {
                        xx->fMinorState = x10CP290MinorSawEvent8;
                    }
                    else
                    {
                        ++xx->fEventByteCount;
                    }
                    break;

                default:
                    xx->fMajorState = x10CP290MajorIdle;
                    xx->fMinorState = x10CP290MinorIdle;
                    break;
                    
            }
            break;

        case x10CP290MinorSawEvent8:
            switch (xx->fMajorState)
            {
                case x10CP290MajorAwaitingTimerEvents:
                    if (128 <= xx->fEntryCount)
                    {
                        outlet_bang(xx->fCommandComplete);
                        xx->fEntryCount = 0;
                        xx->fMajorState = x10CP290MajorIdle;
                        xx->fMinorState = x10CP290MinorIdle;
                    }
                    else if (ALL_ON == rr)
                    {
                        ++xx->fEntryCount;
                    }
                    else
                    {
                        xx->fEventByteCount = 1;
                        xx->fMinorState = x10CP290MinorSawEvent1;
                    }
                    break;

                default:
                    xx->fMajorState = x10CP290MajorIdle;
                    xx->fMinorState = x10CP290MinorIdle;
                    break;
                    
            }
            break;

        default:
            break;
            
    }
} // x10DoCP290

/*------------------------------------ cmd_In1 ---*/
IN1_HEADER(X10ControlData)
{
    /* We've received a byte. Check if it matches what we are expecting. */
    if (xx)
    {
        switch (xx->fKind)
        {
            case X10KindCM11:
                x10DoCM11(xx, msg);
                break;


            case X10KindCP290:
                x10DoCP290(xx, msg);
                break;

            default:
                break;
                
        }
    }
} // cmd_In1
