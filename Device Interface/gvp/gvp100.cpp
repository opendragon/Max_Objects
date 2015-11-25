/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       gvp100.cpp                                                              */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the gvp100 module.        */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 1998 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    1998/06/14                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#define IS_MAIN /* */
#include "gvp100.h"
#include "reportAnything.h"
#include "reportVersion.h"

/*------------------------------------ gvpProcessClock ---*/
static void gvpProcessClock(GvpData * xx)
{
    if (xx)
    {
        qelem_set(xx->fPollQueue);
    }
} // gvpProcessClock

/*------------------------------------ gvpProcessQueue ---*/
static void gvpProcessQueue(GvpData * xx)
{
    if (xx && (! xx->fStopping))
    {
        short prevLock = lockout_set(1);
        
        outlet_bang(xx->fSampleBangOut);
        clock_delay(xx->fPollClock, xx->fPollRate);
        lockout_set(prevLock);
#if USE_EVNUM
        evnum_incr();
#endif /* USE_EVNUM */
    }
} // gvpProcessQueue

/*------------------------------------ gvpCreate ---*/
static void * gvpCreate(const long selectAddress,
                        const long pollRate,
                        const long poolSize)
{
    GvpData * xx = static_cast<GvpData *>(object_alloc(gClass));
    
    if (xx)
    {
        xx->fSendCompletion = xx->fStopping = false;
        xx->fPollClock = NULL;
        xx->fPollQueue = NULL;
        xx->fPool = NULL;
        if ((0 > selectAddress) || (254 < selectAddress) || (0 != (selectAddress & 1)))
        {
            LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid select address (%ld) for device", selectAddress)
            xx->fSelectAddress = 0;
        }
        else
        {
            xx->fSelectAddress = selectAddress;
        }
        if ((pollRate < 0) || (pollRate > MAX_POLL_RATE))
        {
            LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid polling rate (%ld) for device", pollRate)
            xx->fPollRate = SER_SAMPLE_RATE;
        }
        else
        {
            xx->fPollRate = static_cast<short>(pollRate ? pollRate : SER_SAMPLE_RATE);
        }
        if ((poolSize < 0) || (poolSize > MAX_POOL_SIZE))
        {
            LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid pool size (%ld) for device", poolSize)
            xx->fPoolSize = POOL_SIZE;
        }
        else
        {
            xx->fPoolSize = static_cast<short>(poolSize ? poolSize : POOL_SIZE);
        }
        /* Set up our connections and private data */
        intin(xx, 1);
        xx->fErrorBangOut = static_cast<t_outlet *>(bangout(xx));
        xx->fBreakSendOut = static_cast<t_outlet *>(bangout(xx));
        xx->fDataSendOut = static_cast<t_outlet *>(outlet_new(xx, 0L));  /* list, int */
        xx->fSampleBangOut = static_cast<t_outlet *>(bangout(xx));
        xx->fCommandComplete = static_cast<t_outlet *>(bangout(xx));
        xx->fSequenceComplete = static_cast<t_outlet *>(bangout(xx));
        xx->fPollClock = MAKE_CLOCK(xx, gvpProcessClock);
        xx->fPollQueue = MAKE_QELEM(xx, gvpProcessQueue);
        xx->fFirst = xx->fLast = NULL;
        xx->fPool = GET_BYTES(xx->fPoolSize, GvpPacket);
        if (xx->fErrorBangOut && xx->fDataSendOut && xx->fBreakSendOut && xx->fSampleBangOut &&
            xx->fSequenceComplete && xx->fPollClock && xx->fPollQueue && xx->fPool)
        {
            GvpPacket * prev = NULL;
            GvpPacket * curr = xx->fPool;
            
            for (short index = 0; index < xx->fPoolSize; ++index)
            {
                curr->fPrev = prev;
                if (prev)
                {
                    prev->fNext = curr;
                }
                prev = curr++;
            }
            clock_delay(xx->fPollClock, xx->fPollRate);
#if defined(BE_VERBOSE)
            xx->fVerbose = false;
#endif /* BE_VERBOSE */
            xx->fState = kStateIdle;
            gvpReportStateChange(xx);
        }
        else
        {
            LOG_ERROR_1(xx, OUTPUT_PREFIX "unable to create port or clock for device")
            freeobject(reinterpret_cast<t_object *>(xx));
            xx = NULL;
        }
    }
    return xx;
} // gvpCreate

/*------------------------------------ gvpFree ---*/
static void gvpFree(GvpData * xx)
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
        FREE_BYTES(xx->fPool);
    }
} // gvpFree

/*------------------------------------ main ---*/
int main(void)
{
    /* Allocate class memory and set up class. */
    t_class * temp = class_new(OUR_NAME, reinterpret_cast<method>(gvpCreate),
                               reinterpret_cast<method>(gvpFree), sizeof(GvpData),
                               reinterpret_cast<method>(0L), A_LONG, A_DEFLONG, A_DEFLONG, 0);

    if (temp)
    {
        class_addmethod(temp, reinterpret_cast<method>(cmd_AllStop), "allstop", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Anything), MESSAGE_ANYTHING, A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Assist), MESSAGE_ASSIST, A_CANT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_BreakDone), "breakdone", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_EndSequence), "endsequence", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_In1), MESSAGE_IN1, A_LONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_LearnEmem), "learn-emem", A_DEFLONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_RecallEmem), "recall-emem", A_DEFLONG,
                        0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_SetCrosspoint), "c", A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_SetCrosspoint), "crosspoint", A_GIMME,
                        0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_SetDskAnalogControl), "d", A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_SetDskAnalogControl), "dskanalogcontrol",
                        A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_SetDskClipLevel), "!c", A_FLOAT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_SetDskClipLevel), "!clip", A_FLOAT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_SetEffectsAnalogControl), "e", A_GIMME,
                        0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_SetEffectsAnalogControl),
                        "effectsanalogcontrol", A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_SetEffectsPosition), "!e", A_FLOAT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_SetEffectsPosition), "!effects", A_FLOAT,
                        0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_SetJoystick), "!j", A_FLOAT, A_FLOAT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_SetJoystick), "!joystick", A_FLOAT,
                        A_FLOAT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_SetPushbutton), "off", A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_SetPushbutton), "on", A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_SetPushbutton), "p", A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_SetPushbutton), "push", A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_SetTakePosition), "!t", A_FLOAT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_SetTakePosition), "!take", A_FLOAT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_SetTransitionMode), "m", A_SYM, A_DEFSYM,
                        0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_SetTransitionMode), "transitionmode",
                        A_SYM, A_DEFSYM, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_SetTransitionRate), "r", A_SYM,
                        A_DEFLONG, A_DEFSYM, A_DEFSYM, A_DEFSYM, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_SetTransitionRate), "transitionrate",
                        A_SYM, A_DEFLONG, A_DEFSYM, A_DEFSYM, A_DEFSYM, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_SetWipePattern), "w", A_DEFLONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_SetWipePattern), "wipepattern",
                        A_DEFLONG, 0);
#if defined(BE_VERBOSE)
        class_addmethod(temp, reinterpret_cast<method>(cmd_Verbose), "v", A_DEFSYM, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Verbose), "verbose", A_DEFSYM, 0);
#endif /* BE_VERBOSE */
        class_addmethod(temp, reinterpret_cast<method>(cmd_XReset), "x", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_XReset), "xreset", 0);
        class_register(CLASS_BOX, temp);
    }
    gClass = temp;
    gASymbol = gensym("a");
    gAutoSymbol = gensym("auto");
    gBSymbol = gensym("b");
    gBackgroundSymbol = gensym("background");
    gBlackSymbol = gensym("black");
    gDSymbol = gensym("d");
    gDoItSymbol = gensym("doit");
    gDskSymbol = gensym("dsk");
    gFSymbol = gensym("f");
    gF2bSymbol = gensym("f2b");
    gEmptySymbol = gensym("");
    gISymbol = gensym("i");
    gInsertSymbol = gensym("insert");
    gKSymbol = gensym("k");
    gKeySymbol = gensym("key");
    gOffSymbol = gensym("off");
    gOnSymbol = gensym("on");
    gPSymbol = gensym("p");
    gPgmSymbol = gensym("pgm");
    gPresetSymbol = gensym("preset");
    gPreviewSymbol = gensym("preview");
    gProgramSymbol = gensym("program");
    gPushSymbol = gensym("push");
    reportVersion(OUR_NAME);
    return 0;
} // main

/*------------------------------------ gvpReportStateChange ---*/
void gvpReportStateChange(GvpData * xx)
{
#if defined(REPORT_STATE_CHANGES)
    if (xx && xx->fVerbose)
    {
        switch (xx->fState)
        {
            case kStateIdle:
                LOG_POST_1(xx, OUTPUT_PREFIX "-> kStateIdle")
                break;

            case kStateAwaitingBreakDone:
                LOG_POST_1(xx, OUTPUT_PREFIX "-> kStateAwaitingBreakDone")
                break;

            case kStateAwaitingAck:
                LOG_POST_1(xx, OUTPUT_PREFIX "-> kStateAwaitingAck")
                break;

            case kStateSelected:
                LOG_POST_1(xx, OUTPUT_PREFIX "-> kStateSelected")
                break;

            case kStateAwaitingByteCount1:
                LOG_POST_1(xx, OUTPUT_PREFIX "-> kStateAwaitingByteCount1")
                break;

            case kStateAwaitingByteCountN:
                LOG_POST_1(xx, OUTPUT_PREFIX "-> kStateAwaitingByteCountN")
                break;

            case kStateAwaitingStatusByte:
                LOG_POST_1(xx, OUTPUT_PREFIX "-> kStateAwaitingStatusByte")
                break;

            default:
                LOG_POST_2(xx, OUTPUT_PREFIX "-> unrecognized state (%ld)", xx->fState)
                break;
                
        }
    }
#else /* not REPORT_STATE_CHANGES */
# pragma unused(xx)
#endif /* not REPORT_STATE_CHANGES */
} // gvpReportStateChange

/*------------------------------------ gvpPerformWriteCommand ---*/
void gvpPerformWriteCommand(GvpData *            xx,
                            const long           effectsAddress,
                            const GvpCommandCode commandCode,
                            const unsigned char  numBytesToFollow,
                            unsigned char *      bytesToFollow,
                            const GvpState       newState,
                            const bool           lastCommand)
{
    unsigned char dataValue;
    static t_atom dataList[MAX_MESSAGE_BYTES + 3];

    if (xx)
    {
        short prevLock = lockout_set(1);

        if (kStateSelected == xx->fState)
        {
            /* Send data, set expected state. */
            if (kCommandEndSequence == commandCode)
            {
                outlet_bang(xx->fSequenceComplete);
            }
            else
            {
                A_SETLONG(dataList, numBytesToFollow + 2);
                A_SETLONG(dataList + 1, TO_INT(effectsAddress & 0x00FF));
                A_SETLONG(dataList + 2, TO_INT(commandCode));
                for (short ii = 0; ii < numBytesToFollow; ++ii)
                {
                    dataValue = *bytesToFollow++;
                    A_SETLONG(dataList + ii + 3, dataValue);
                }
                outlet_list(xx->fDataSendOut, 0L, static_cast<short>(numBytesToFollow + 3),
                            dataList);
            }
            xx->fSendCompletion = lastCommand;
            xx->fState = newState;
            gvpReportStateChange(xx);
        }
        else if (gvpCheckPoolSpace(xx, 1))
        {
            GvpPacket * newPacket = gvpNewPacket(xx);

            newPacket->fPrev = xx->fLast;
            newPacket->fState = newState;
            if (xx->fLast)
            {
                xx->fLast->fNext = newPacket;
            }
            else
            {
                xx->fFirst = newPacket;
            }
            xx->fLast = newPacket;
            if (kCommandEndSequence == commandCode)
            {
                /* Record the EndSequence request. */
                newPacket->fSize = -1;
            }
            else
            {
                newPacket->fSendCompletion = lastCommand;
                newPacket->fSize = static_cast<short>(numBytesToFollow + 3);
                newPacket->fBuffer[0] = static_cast<unsigned char>(numBytesToFollow + 2);
                newPacket->fBuffer[1] = static_cast<unsigned char>(effectsAddress & 0x00FF);
                newPacket->fBuffer[2] = static_cast<unsigned char>(commandCode);
                if (numBytesToFollow > 0)
                {
                    memcpy(newPacket->fBuffer + 3, bytesToFollow, numBytesToFollow);
                }
            }
        }
        else
        {
            LOG_ERROR_1(xx, OUTPUT_PREFIX "insufficient pool space to hold commands")
        }
        lockout_set(prevLock);
    }
} // gvpPerformWriteCommand

StandardAnythingRoutine(GvpData)
