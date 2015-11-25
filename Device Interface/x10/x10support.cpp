/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       x10support.cpp                                                          */
/*                                                                                      */
/*  Contains:   The support routines for the x10 module.                                */
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

/*------------------------------------ x10CM11DoDirect ---*/
void x10CM11DoDirect(X10ControlData *      xx,
                     const X10FunctionCode funcCode,
                     t_symbol *            code,
                     const long            map,
                     const long            level)
{
    bool            okSoFar = true;
    bool            doSingle = false;
    bool            isDimOrBright = false;
    unsigned short  aCode = x10IdentifyHouseCode(code);
    X10FunctionCode workingCode = funcCode;
    long            workingLevel = level;

    if ((xx->fMinorState != x10CM11MinorIdle) || (xx->fMajorState != x10CM11MajorIdle))
    {
        LOG_ERROR_1(xx, OUTPUT_PREFIX "attempt to send command while awaiting response.")
        okSoFar = false;
    }
    if (! aCode)
    {
        LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid housecode (%s)", code->s_name)
        okSoFar = false;
    }
    if ((map <= 0) || (map > 0x0FFFF))
    {
        LOG_ERROR_3(xx, OUTPUT_PREFIX "invalid map (0x%lx = %ld)", map, map)
        okSoFar = false;
    }
    if (x10CM11FunctionDim == workingCode)
    {
        if (workingLevel < 0)
        {
            workingCode = x10CM11FunctionBright;
            workingLevel = -workingLevel;
            if (workingLevel > 22)
            {
                LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid dim level (%ld)", level)
                okSoFar = false;
            }
        }
        else if ((! workingLevel) || (workingLevel > 22))
        {
            LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid dim level (%ld)", level)
            okSoFar = false;
        }
    }
    else if (workingLevel)
    {
        LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid dim level (%ld)", level)
        okSoFar = false;
    }
    if (okSoFar)
    {
        if (0x0FFFF == map)
        {
            if (x10CM11FunctionOn == workingCode)
            {
                workingCode = x10CM11FunctionAllOn;
                doSingle = true;
            }
            else if (x10CM11FunctionOff == workingCode)
            {
                workingCode = x10CM11FunctionUnitsOff;
                doSingle = true;
            }
        }
        if ((x10CM11FunctionBright == workingCode) || (x10CM11FunctionDim == workingCode))
        {
            isDimOrBright = true;
        }
        xx->fCompletedWhenStatus = true;
        xx->fDimLevel = static_cast<short>(isDimOrBright ? workingLevel : 0);
        xx->fDeviceMap = static_cast<unsigned short>(doSingle ? 0 : map);
        xx->fHouseCodeChar = static_cast<short>(aCode - 1);
        xx->fFunctionChar = workingCode;
        if (doSingle)
        {
            xx->fMajorState = x10CM11MajorSendingSingleRequest;
            x10CM11SendFunctionCode(xx);
        }
        else
        {
            xx->fMajorState = x10CM11MajorSendingMultiRequest;
            x10CM11SendDeviceAddress(xx);
        }
    }
    else
    {
        outlet_bang(xx->fErrorBangOut);
    }
} // x10CM11DoDirect

/*------------------------------------ x10CM11ResendBuffer ---*/
void x10CM11ResendBuffer(X10ControlData * xx)
{
    if (--xx->fRetries)
    {
        for (unsigned short ii = 0; ii < xx->fOutCount; ++ii)
        {
            outlet_int(xx->fCommandsOut, TO_INT(*(xx->fOutBuffer + ii)));
        }
        xx->fIndex = 0;
        xx->fMinorState = x10CM11MinorAwaitingChecksum;
        // post("resend");//!!
    }
    else
    {
        xx->fMinorState = x10CM11MinorIdle;
        xx->fMajorState = x10CM11MajorFailure;
        outlet_bang(xx->fErrorBangOut);
    }
} // x10CM11ResendBuffer

/*------------------------------------ x10CM11SendBuffer ---*/
void x10CM11SendBuffer(X10ControlData *     xx,
                       const unsigned short howMany)
{
    unsigned short checker = 0;

    xx->fRetries = MAX_RETRIES;
    for (unsigned short ii = 0; ii < howMany; ++ii)
    {
        checker += *(xx->fOutBuffer + ii);
        outlet_int(xx->fCommandsOut, TO_INT(*(xx->fOutBuffer + ii)));
    }
    xx->fIndex = 0;
    xx->fOutCount = howMany;
    xx->fChecksum = static_cast<unsigned short>(checker & 0xff);
    xx->fMinorState = x10CM11MinorAwaitingChecksum;
    // post("count %d checksum %x", xx->fOutCount, xx->fChecksum);//!!
} // x10CM11SendBuffer

/*------------------------------------ x10CM11SendClockInfo ---*/
void x10CM11SendClockInfo(X10ControlData * xx,
                          const bool       doReset,
                          const bool       reportCompletion)
{
    static int      monthOffset[] = {
        0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365
    };
    CFAbsoluteTime  rightNow = CFAbsoluteTimeGetCurrent();
    CFTimeZoneRef   timeZone = CFTimeZoneCopyDefault();
    CFGregorianDate now = CFAbsoluteTimeGetGregorianDate(rightNow, timeZone);
    SInt32          weekDay = CFAbsoluteTimeGetDayOfWeek(rightNow, timeZone);
    int             hoursAndMinutes = (now.hour * 60) + now.minute;
    int             yearDay = monthOffset[now.month - 1] + now.day;
    CFRelease(timeZone);
    *xx->fOutBuffer = CM11_CLOCK_ACK;
    *(xx->fOutBuffer + 1) = static_cast<unsigned char>(now.second);
    *(xx->fOutBuffer + 2) = static_cast<unsigned char>(hoursAndMinutes % 120);
    *(xx->fOutBuffer + 3) = static_cast<unsigned char>(now.hour >> 1);
    *(xx->fOutBuffer + 4) = static_cast<unsigned char>(yearDay);
    *(xx->fOutBuffer + 5) = static_cast<unsigned char>((0x0080 >> weekDay) |
                                                       ((yearDay & 0x0100) >> 1));
    *(xx->fOutBuffer + 6) = static_cast<unsigned char>((xx->fHouseCodeChar << 4) |
                                                       (doReset ? 1 : 0));
    xx->fIgnoreChecksum = true;
    xx->fCompletedWhenStatus = reportCompletion;
    x10CM11SendBuffer(xx, 7);
} // x10CM11SendClockInfo

/*------------------------------------ x10CM11SendDeviceAddress ---*/
void x10CM11SendDeviceAddress(X10ControlData * xx)
{
    unsigned short deviceCode = 0;

    *xx->fOutBuffer = 0x4;
    for (unsigned short ii = 0; ii < NUM_HOUSECODES; ++ii)
    {
        if (xx->fDeviceMap & kUnitCodeToBits[ii])
        {
            deviceCode = ii;
            xx->fDeviceMap &= ~ kUnitCodeToBits[ii];
            break;
        }
    }
    *(xx->fOutBuffer + 1) = static_cast<unsigned char>((kHouseCodeToBits[xx->fHouseCodeChar] << 4) |
                                                       kHouseCodeToBits[deviceCode]);
    x10CM11SendBuffer(xx, 2);
} // x10CM11SendDeviceAddress

/*------------------------------------ x10CM11SendFunctionCode ---*/
void x10CM11SendFunctionCode(X10ControlData * xx)
{
    *xx->fOutBuffer = static_cast<unsigned char>(((xx->fDimLevel & 0x01f) << 3) | 0x6);
    *(xx->fOutBuffer + 1) = static_cast<unsigned char>((kHouseCodeToBits[xx->fHouseCodeChar] << 4) |
                                                       (xx->fFunctionChar & 0x0f));
    x10CM11SendBuffer(xx, 2);
} // x10SendFunctionCode

/*------------------------------------ x10CP290CalcSum ---*/
void x10CP290CalcSum(X10ControlData *     xx,
                     const unsigned short offset,
                     const unsigned short howMany)
{
    unsigned long sum = 0;

    for (unsigned short ii = 0; ii < howMany; ++ii)
    {
        sum += *(xx->fOutArea + offset + ii);
    }
    *(xx->fOutArea + offset + howMany) = static_cast<unsigned char>(sum & 0xFF);
} // x10CP290CalcSum

/*------------------------------------ x10CP290DoDirect ---*/
void x10CP290DoDirect(X10ControlData *      xx,
                      const X10FunctionCode funcCode,
                      t_symbol *            code,
                      const long            map,
                      const long            level)
{
    bool           okSoFar = true;
    unsigned short aCode = x10IdentifyHouseCode(code);

    if (xx->fMinorState != x10CP290MinorIdle)
    {
        LOG_ERROR_1(xx, OUTPUT_PREFIX "attempt to send command while awaiting response.")
        okSoFar = false;
    }
    if (! aCode)
    {
        LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid housecode (%s)", code->s_name)
        okSoFar = false;
    }
    if ((map <= 0) || (map > 0x0FFFF))
    {
        LOG_ERROR_3(xx, OUTPUT_PREFIX "invalid map (0x%lx = %ld)", map, map)
        okSoFar = false;
    }
    if ((level < 0) || (level > 15))
    {
        LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid dim level (%ld)", level)
        okSoFar = false;
    }
    if (okSoFar)
    {
        *xx->fOutCmd = x10CP290Direct;
        *xx->fOutArea = static_cast<unsigned char>((((15 - level) & 0x0F) << 4) | funcCode);
        *(xx->fOutArea + 1) = static_cast<unsigned char>(kHouseCodeToBits[aCode - 1] << 4);
        *(xx->fOutArea + 2) = static_cast<unsigned char>(map >> 8);
        *(xx->fOutArea + 3) = static_cast<unsigned char>(map & 0xFF);
        x10CP290CalcSum(xx, 0, 4);
        x10CP290SendBuffer(xx, 5);
        xx->fMajorState = x10CP290MajorAwaitingACKAndUpload;
    }
    else
    {
        outlet_bang(xx->fErrorBangOut);
    }
} // x10CP290DoDirect

/*------------------------------------ x10CP290DoSingleton ---*/
void x10CP290DoSingleton(X10ControlData *     xx,
                         const X10CommandCode cmd,
                         const X10MajorState  newState)
{
    bool okSoFar = true;

    if (xx->fMinorState != x10CP290MinorIdle)
    {
        LOG_ERROR_1(xx, OUTPUT_PREFIX "attempt to send command while awaiting response.")
        okSoFar = false;
    }
    if (okSoFar)
    {
        *xx->fOutCmd = cmd;
        x10CP290SendBuffer(xx, 0);
        xx->fMajorState = newState;
    }
    else
    {
        outlet_bang(xx->fErrorBangOut);
    }
} // x10CP290DoSingleton

/*------------------------------------ x10CP290DoTimerEvent ---*/
void x10CP290DoTimerEvent(X10ControlData *  xx,
                          const X10ModeCode mode,
                          t_symbol *        code,
                          const long        map,
                          const long        eventNumber,
                          const long        dayMap,
                          const long        hourMinute,
                          t_symbol *        function,
                          const long        level)
{
    bool            okSoFar = true;
    X10FunctionCode funcCode = x10FunctionUnknown;
    unsigned short  aCode = x10IdentifyHouseCode(code);

    if (xx->fMinorState != x10CP290MinorIdle)
    {
        LOG_ERROR_1(xx, OUTPUT_PREFIX "attempt to send command while awaiting response.")
        okSoFar = false;
    }
    if (! aCode)
    {
        LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid housecode (%s)", code->s_name)
        okSoFar = false;
    }
    if ((map < 0) || (map > 0x0FFFF))
    {
        LOG_ERROR_3(xx, OUTPUT_PREFIX "invalid map (0x%lx = %ld)", map, map)
        okSoFar = false;
    }
    if ((eventNumber < 0) || (eventNumber > 127))
    {
        LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid event number (%ld)", eventNumber)
        okSoFar = false;
    }
    if ((dayMap < 0) || (dayMap > 0x7F))
    {
        LOG_ERROR_3(xx, OUTPUT_PREFIX "invalid day map (0x%lx = %ld)", dayMap, dayMap)
        okSoFar = false;
    }
    if ((hourMinute < 0) || (hourMinute > (60 * 24)))
    {
        LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid hours and minutes (%ld)", hourMinute)
        okSoFar = false;
    }
    if (function == gDimSymbol)
    {
        funcCode = x10CP290FunctionDimOut;
    }
    else if (function == gOffSymbol)
    {
        funcCode = x10CP290FunctionOff;
    }
    else if (function == gOnSymbol)
    {
        funcCode = x10CP290FunctionOn;
    }
    else
    {
        LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid function (%s)", function->s_name)
        okSoFar = false;
    }
    if ((level < 0) || (level > 15))
    {
        LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid dim level (%ld)", level)
        okSoFar = false;
    }
    if (okSoFar)
    {
        switch (xx->fKind)
        {
            case X10KindCM11:
                // TBD
                break;

            case X10KindCP290:
                *xx->fOutCmd = x10CP290DownloadTimerEvent;
                *xx->fOutArea = static_cast<unsigned char>((eventNumber << 3) & 0x0FF);
                *(xx->fOutArea + 1) = static_cast<unsigned char>((eventNumber >> 5) & 0x03);
                *(xx->fOutArea + 2) = mode;
                *(xx->fOutArea + 3) = static_cast<unsigned char>(dayMap);
                *(xx->fOutArea + 4) = static_cast<unsigned char>(hourMinute / 60);
                *(xx->fOutArea + 5) = static_cast<unsigned char>(hourMinute % 60);
                *(xx->fOutArea + 6) = static_cast<unsigned char>(map & 0xFF);
                *(xx->fOutArea + 7) = static_cast<unsigned char>(map >> 8);
                *(xx->fOutArea + 8) = static_cast<unsigned char>(kHouseCodeToBits[aCode - 1] << 4);
                *(xx->fOutArea + 9) = static_cast<unsigned char>((((15 - level) & 0x0F) << 4) |
                                                                 funcCode);
                x10CP290CalcSum(xx, 2, 8);
                x10CP290SendBuffer(xx, 11);
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
} // x10CP290DoTimerEvent

/*------------------------------------ x10CP290SendBuffer ---*/
void x10CP290SendBuffer(X10ControlData *     xx,
                        const unsigned short howMany)
{
    for (unsigned short ii = 0; ii < (howMany + COMMAND_PREFIX_SIZE + 1); ++ii)
    {
        outlet_int(xx->fCommandsOut, TO_INT(*(xx->fOutBuffer + ii)));
    }
} // x10CP290SendBuffer

/*------------------------------------ x10IdentifyHouseCode ---*/
unsigned short x10IdentifyHouseCode(t_symbol * aSym)
{
    for (unsigned short ii = 0; ii < NUM_HOUSECODES; ++ii)
    {
        if (aSym == gHouseCodes[ii])
        {
            return static_cast<unsigned short>(ii + 1);
        }
    }
    return 0;
} // x10IdentifyHouseCode
