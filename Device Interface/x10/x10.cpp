/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       x10.cpp                                                                 */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the x10 module.           */
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

#define IS_MAIN /* */
#include "x10.h"
#include "reportAnything.h"
#include "reportVersion.h"

static char kHouseCodeNames[NUM_HOUSECODES][2] =
{
    "A", "B", "C", "D", "E", "F", "G", "H",
    "I", "J", "K", "L", "M", "N", "O", "P"
};

/*------------------------------------ x10ProcessClock ---*/
static void x10ProcessClock(X10ControlData * xx)
{
    if (xx && (! xx->fStopping))
    {
        qelem_set(xx->fPollQueue);
    }
} // x10ProcessClock

/*------------------------------------ x10ProcessQueue ---*/
static void x10ProcessQueue(X10ControlData * xx)
{
    if (xx && (! xx->fStopping))
    {
        short prevLock = lockout_set(1);
        
        outlet_bang(xx->fPollerOut);
        clock_delay(xx->fPollClock, xx->fPollRate);
        lockout_set(prevLock);
#if USE_EVNUM
        evnum_incr();
#endif /* USE_EVNUM */
    }
} // x10ProcessQueue

/*------------------------------------ x10Create ---*/
static void * x10Create(t_symbol * kind,
                        const long pollRate)
{
    X10ControlData * xx = static_cast<X10ControlData *>(object_alloc(gClass));
    
    if (xx)
    {
        xx->fHouseCodeChar = 0;
        xx->fFunctionChar = x10CM11FunctionUnitsOff;
        xx->fDimLevel = 0;
        xx->fDeviceMap = 0;
        xx->fCompletedWhenStatus = xx->fIgnoreChecksum = xx->fStopping = false;
        xx->fOutCmd = xx->fOutArea = NULL;
        if ((kind == gCM11Symbol) || (kind == gEmptySymbol))
        {
            xx->fKind = X10KindCM11;
        }
        else if (kind == gCP290Symbol)
        {
            xx->fKind = X10KindCP290;
        }
        else
        {
            LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid kind '%s' for device", kind->s_name)
            xx->fKind = X10KindCM11;
        }
        if ((0 > pollRate) || (MAX_POLL_RATE < pollRate))
        {
            LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid polling rate (%ld) for device", pollRate)
            xx->fPollRate = SER_SAMPLE_RATE;
        }
        else
        {
            xx->fPollRate = (pollRate ? pollRate : SER_SAMPLE_RATE);
        }
        /* Set up our connections and private data */
        intin(xx, 1);
        xx->fErrorBangOut = static_cast<t_outlet *>(bangout(xx));
        xx->fMinuteOut = static_cast<t_outlet *>(intout(xx));
        xx->fHourOut = static_cast<t_outlet *>(intout(xx));
        xx->fDayOut = static_cast<t_outlet *>(intout(xx));
        xx->fDeviceStatus = static_cast<t_outlet *>(intout(xx));
        xx->fCommandComplete = static_cast<t_outlet *>(bangout(xx));
        xx->fHouseCode = static_cast<t_outlet *>(intout(xx));
        xx->fPollerOut = static_cast<t_outlet *>(bangout(xx));
        xx->fCommandsOut = static_cast<t_outlet *>(intout(xx));
        xx->fPollClock = MAKE_CLOCK(xx, x10ProcessClock);
        xx->fPollQueue = MAKE_QELEM(xx, x10ProcessQueue);
        xx->fOutBuffer = GET_BYTES(OUTBUFF_SIZE, unsigned char);
        switch (xx->fKind)
        {
            case X10KindCM11:
                xx->fMinorState = x10CM11MinorIdle;
                xx->fMajorState = x10CM11MajorIdle;
                break;
                
            case X10KindCP290:
                xx->fMinorState = x10CP290MinorIdle;
                xx->fMajorState = x10CP290MajorIdle;
                break;
                
            default:
                break;
                
        }
        xx->fAllOnCount = xx->fEntryCount = xx->fEventByteCount = 0;
        if (xx->fErrorBangOut && xx->fMinuteOut && xx->fHourOut && xx->fDayOut &&
            xx->fDeviceStatus && xx->fCommandComplete && xx->fHouseCode &&
            xx->fPollerOut && xx->fCommandsOut && xx->fPollClock && xx->fPollQueue &&
            xx->fOutBuffer)
        {
            for (short ii = 0; ii < COMMAND_PREFIX_SIZE; ++ii)
            {
                *(xx->fOutBuffer + ii) = ALL_ON;
            }
            xx->fOutCmd = xx->fOutBuffer + COMMAND_PREFIX_SIZE;
            xx->fOutArea = xx->fOutBuffer + COMMAND_PREFIX_SIZE + 1;
            clock_delay(xx->fPollClock, xx->fPollRate);
        }
        else
        {
            LOG_ERROR_1(xx, OUTPUT_PREFIX "unable to create port or clock for device")
            freeobject(reinterpret_cast<t_object *>(xx));
            xx = NULL;
        }
    }
    return xx;
} // x10Create

/*------------------------------------ x10Free ---*/
static void x10Free(X10ControlData * xx)
{
    if (xx)
    {
        xx->fStopping = true;
        if (xx->fPollClock)
        {
            clock_unset(xx->fPollClock);
            clock_free(reinterpret_cast<t_object *>(xx->fPollClock));
            xx->fPollClock = NULL;
        }
        if (xx->fPollQueue)
        {
            qelem_unset(xx->fPollQueue);
            qelem_free(xx->fPollQueue);
            xx->fPollQueue = NULL;
        }
        FREE_BYTES(xx->fOutBuffer);
    }
} // x10Free

/*------------------------------------ main ---*/
int main(void)
{
    /* Allocate class memory and set up class. */
    t_class * temp = class_new(OUR_NAME, reinterpret_cast<method>(x10Create),
                               reinterpret_cast<method>(x10Free), sizeof(X10ControlData),
                               reinterpret_cast<method>(0L), A_DEFSYM, A_DEFLONG, 0);
    
    if (temp)
    {
        class_addmethod(temp, reinterpret_cast<method>(cmd_Anything), MESSAGE_ANYTHING, A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Assist), MESSAGE_ASSIST, A_CANT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_ClearTimerEvent), "clear!", A_LONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Dim), "dim", A_SYM, A_LONG, A_DEFLONG,
                        0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_GetGraphicsData), "graphics?", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_GetHouseCode), "housecode?", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_GetTimerEvents), "events?", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_In1), MESSAGE_IN1, A_LONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Kind), "kind", A_SYM, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Off), "off", A_SYM, A_LONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_On), "on", A_SYM, A_LONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Reset), "reset", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_SetClock), "clock!", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_SetEveryDayEvent), "everyday!", A_SYM,
                        A_LONG, A_LONG, A_LONG, A_SYM, A_DEFLONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_SetGraphicsData), "graphics!", A_LONG,
                        A_DEFLONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_SetHouseCode), "housecode!", A_SYM, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_SetNormalEvent), "normal!", A_SYM,
                        A_LONG, A_LONG, A_LONG, A_LONG, A_SYM, A_DEFLONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_SetSecurityEvent), "security!", A_SYM,
                        A_LONG, A_LONG, A_LONG, A_LONG, A_SYM, A_DEFLONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_SetTodayEvent), "today!", A_SYM, A_LONG,
                        A_LONG, A_LONG, A_SYM, A_DEFLONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_SetTomorrowEvent), "tomorrow!", A_SYM,
                        A_LONG, A_LONG, A_LONG, A_SYM,  A_DEFLONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_XReset), "xreset", 0);
        class_register(CLASS_BOX, temp);
    }
    gClass = temp;
    gCM11Symbol = gensym("cm11");
    gCP290Symbol = gensym("cp290");
    gDimSymbol = gensym("dim");
    gEmptySymbol = gensym("");
    gOffSymbol = gensym("off");
    gOnSymbol = gensym("on");
    for (unsigned short ii = 0; ii < NUM_HOUSECODES; ++ii)
    {
        gHouseCodes[ii] = gensym(kHouseCodeNames[ii]);
    }
    reportVersion(OUR_NAME);
    return 0;
} // main

StandardAnythingRoutine(X10ControlData)
