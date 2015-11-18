/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       ldp1550.cpp                                                             */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the ldp1550 module.       */
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
#include "ldp1550.h"
#include "reportAnything.h"
#include "reportVersion.h"

/* Forward references: */
void * ldpCreate(long pollRate,
                 long infoRate,
                 long poolSize);

void ldpFree(LdpData * xx);

void ldpProcessClock(LdpData * xx);

void ldpProcessQueue(LdpData * xx);

/*------------------------------------ main ---*/
int main(void)
{
    /* Allocate class memory and set up class. */
    t_class * temp = class_new(OUR_NAME, reinterpret_cast<method>(ldpCreate), reinterpret_cast<method>(ldpFree),
                               sizeof(LdpData), reinterpret_cast<method>(0L), A_DEFLONG, A_DEFLONG, A_DEFLONG, 0);

    if (temp)
    {
        class_addmethod(temp, reinterpret_cast<method>(cmd_AllInfo), "allinfo", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Anything), MESSAGE_ANYTHING, A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Assist), MESSAGE_ASSIST, A_CANT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Chapter), "chapter", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Continue), "continue", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Frame), "frame", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_In1), MESSAGE_IN1, A_LONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Memory), "memory", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Mode), "mode", A_SYM, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_MSearch), "msearch", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Picture), "picture", A_SYM, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Play), "play", A_DEFSYM, A_DEFLONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_PlayTill), "playtill", A_LONG, A_DEFSYM, A_DEFLONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_PSCEnable), "pscenable", A_SYM, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Repeat), "repeat", A_LONG, A_DEFSYM, A_DEFLONG, A_DEFLONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Reset), "reset", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Search), "search", A_LONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Show), "show", A_SYM, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Sound), "sound", A_LONG, A_SYM, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Status), "status", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_StepStill), "step&still", A_DEFSYM, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Still), "still", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Stop), "stop", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_XReset), "xreset", 0);
        class_register(CLASS_BOX, temp);
    }
    gClass = temp;
    gChapterSymbol = gensym("chapter");
    gEmptySymbol = gensym("");
    gFastSymbol = gensym("fast");
    gFrameSymbol = gensym("frame");
    gFwdSymbol = gensym("fwd");
    gOffSymbol = gensym("off");
    gOnSymbol = gensym("on");
    gRevFastSymbol = gensym("rev-fast");
    gRevScanSymbol = gensym("rev-scan");
    gRevSlowSymbol = gensym("rev-slow");
    gRevStepSymbol = gensym("rev-step");
    gRevSymbol = gensym("rev");
    gScanSymbol = gensym("scan");
    gSlowSymbol = gensym("slow");
    gStepSymbol = gensym("step");
    reportVersion(OUR_NAME);
    return 0;
} // main
/*------------------------------------ ldpCreate ---*/
void * ldpCreate(long pollRate,
                 long infoRate,
                 long poolSize)
{
    LdpData * xx = static_cast<LdpData *>(object_alloc(gClass));

    if (xx)
    {
        xx->fPollClock = NULL_PTR;
        xx->fPollQueue = NULL_PTR;
        xx->fPool = NULL_PTR;
        if ((pollRate < 0) || (pollRate > MAX_POLL_RATE))
        {
            LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid polling rate (%ld) for device", pollRate)
            xx->fPollRate = SER_SAMPLE_RATE;
        }
        else
        {
            xx->fPollRate = static_cast<short>(pollRate ? pollRate : SER_SAMPLE_RATE);
        }
        if (infoRate < 0)
        {
            LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid info rate (%ld) for device", infoRate)
            xx->fInfoRate = INFO_SAMPLE_RATE;
        }
        else
        {
            xx->fInfoRate = static_cast<short>(infoRate ? infoRate : INFO_SAMPLE_RATE);
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
        xx->fFrameNumberOut = static_cast<t_outlet *>(intout(xx));
        xx->fChapterNumberOut = static_cast<t_outlet *>(intout(xx));
        xx->fCommandAccepted = static_cast<t_outlet *>(bangout(xx));
        xx->fCommandComplete = static_cast<t_outlet *>(bangout(xx));
        xx->fProgramStopCodeOut = static_cast<t_outlet *>(bangout(xx));
        xx->fCommandStatus = static_cast<t_outlet *>(intout(xx));
        xx->fKeyModeStatus = static_cast<t_outlet *>(intout(xx));
        xx->fPollerOut = static_cast<t_outlet *>(bangout(xx));
        xx->fCommandsOut = static_cast<t_outlet *>(intout(xx));
        xx->fPollClock = static_cast<t_clock *>(clock_new(xx, reinterpret_cast<method>(ldpProcessClock)));
        xx->fPollQueue = static_cast<t_qelem *>(qelem_new(xx, reinterpret_cast<method>(ldpProcessQueue)));
        xx->fFrameNumber = xx->fInfoCount = 0;
        xx->fInfoSamplingEnabled = (infoRate > 0);
        xx->fStopping = false;
        xx->fMode = kLdpModeFrame;
        xx->fFirst = xx->fLast = xx->fInterruptPoint = NULL_PTR;
        xx->fPool = GETBYTES(xx->fPoolSize, LdpPacket);
        if (xx->fFrameNumberOut && xx->fChapterNumberOut && xx->fCommandComplete && xx->fCommandStatus &&
            xx->fKeyModeStatus && xx->fPollerOut && xx->fCommandsOut && xx->fPollClock && xx->fPollQueue && xx->fPool)
        {
            LdpPacket * prev = NULL_PTR;
            LdpPacket * curr = xx->fPool;

            for (short index = 0; index < xx->fPoolSize; ++index)
            {
                curr->fPrev = prev;
                if (prev)
                {
                    prev->fNext = curr;
                }
                prev = curr++;
            }
            prev->fNext = NULL_PTR;
            xx->fPoolAvailable = xx->fPoolSize;
            clock_delay(xx->fPollClock, xx->fPollRate);
        }
        else
        {
            LOG_ERROR_1(xx, OUTPUT_PREFIX "unable to create port or clock for device")
            freeobject(reinterpret_cast<t_object *>(xx));
            xx = NULL_PTR;
        }
    }
    return xx;
} // ldpCreate
/*------------------------------------ ldpFree ---*/
void ldpFree(LdpData * xx)
{
    if (xx)
    {
        xx->fStopping = true;
        if (xx->fPollClock)
        {
            clock_unset(xx->fPollClock);
            clock_free(reinterpret_cast<t_object *>(xx->fPollClock));
            xx->fPollClock = NULL_PTR;
        }
        if (xx->fPollQueue)
        {
            qelem_unset(xx->fPollQueue);
            qelem_free(xx->fPollQueue);
            xx->fPollQueue = NULL_PTR;
        }
        FREEBYTES(xx->fPool, xx->fPoolSize);
    }
} // ldpFree
/*------------------------------------ ldpProcessClock ---*/
void ldpProcessClock(LdpData * xx)
{
    if (xx && (! xx->fStopping))
    {
        qelem_set(xx->fPollQueue);
    }
} // ldpProcessClock
/*------------------------------------ ldpProcessQueue ---*/
void ldpProcessQueue(LdpData * xx)
{
    if (xx && (! xx->fStopping))
    {
        short prevLock = lockout_set(1);

        outlet_bang(xx->fPollerOut);
        clock_delay(xx->fPollClock, xx->fPollRate);
        if (xx->fInfoSamplingEnabled && (xx->fInfoCount-- <= 0))
        {
            /* Process information request */
            if (ldpCheckPoolSpace(xx, 3))
            {
                if (xx->fInterruptPoint)
                {
                    ldpInsertCommand(xx, xx->fInterruptPoint, kLdpCommandChapterInquiry, kLdpStateAwaitingChapterByte1);
                    ldpInsertCommand(xx, xx->fInterruptPoint, kLdpCommandAddressInquiry, kLdpStateAwaitingFrameByte1);
                    ldpInsertCommand(xx, xx->fInterruptPoint, kLdpCommandStatusInquiry, kLdpStateAwaitingStatusByte1);
                    ldpSendCommand(xx);
                }
                else if (! xx->fFirst)
                {
                    /*ldpInitCommands(xx);*/
                    ldpAddCommand(xx, kLdpCommandChapterInquiry, kLdpStateAwaitingChapterByte1);
                    ldpAddCommand(xx, kLdpCommandAddressInquiry, kLdpStateAwaitingFrameByte1);
                    ldpAddCommand(xx, kLdpCommandStatusInquiry, kLdpStateAwaitingStatusByte1);
                    ldpSendCommand(xx);
                }
            }
            xx->fInfoCount = xx->fInfoRate;
        }
        lockout_set(prevLock);
        evnum_incr();
    }
} // ldpProcessQueue

StandardAnythingRoutine(LdpData *)
