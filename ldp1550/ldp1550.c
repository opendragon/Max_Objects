/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       ldp1550.c                                                               */
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
#include "displayAboutBox.h"
#include "loadAboutBoxData.h"
#include "loadOtherSegments.h"
#include "reportAnything.h"
#include "reportVersion.h"

/* Forward references: */
Pvoid ldpCreate
  (long pollRate,
   long infoRate,
   long poolSize);

Pvoid ldpFree
  (LdpControlPtr xx);

Pvoid ldpProcessClock
  (LdpControlPtr xx);

Pvoid ldpProcessQueue
  (LdpControlPtr xx);

Pvoid ldpProcessResponse
  (LdpControlPtr xx,
   long          rr);

/*------------------------------------ CheckResponse ---*/
static void CheckResponse
  (const long rr)
{
  switch (rr)
  {
    case LDP_ERROR:
      LOG_ERROR_1(OUTPUT_PREFIX "device response = ERROR")
      break;

    case LDP_LID_OPEN:
      LOG_ERROR_1(OUTPUT_PREFIX "device response = LID OPEN")
      break;

    case LDP_NOT_TARGET:
      LOG_ERROR_1(OUTPUT_PREFIX "device response = NOT TARGET")
      break;

    case LDP_NO_FRAME:
      LOG_ERROR_1(OUTPUT_PREFIX "device response = NO FRAME")
      break;

    case LDP_NACK:
      LOG_ERROR_1(OUTPUT_PREFIX "device response = NACK")
      break;

    default:
      LOG_ERROR_2(OUTPUT_PREFIX "unexpected input %02.2lx", rr)
      break;

  }
} /* CheckResponse */

/*------------------------------------ main ---*/
void main
  (Pfptr ff)
{
  EnterCodeResource();
  PrepareCallback();
  FNS = ff;   /* Connect up the function macros. */
  /* Allocate class memory and set up class. */
  setup(reinterpret_cast<t_messlist**>(&gClass), reinterpret_cast<method>(ldpCreate),
        reinterpret_cast<method>(ldpFree), short(sizeof(LdpControl)),
        reinterpret_cast<method>(0L), A_DEFLONG, A_DEFLONG, A_DEFLONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_AllInfo), "allinfo", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Anything), "anything", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Assist), "assist", A_CANT, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Chapter), "chapter", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Continue), "continue", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Frame), "frame", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Info), "info", A_CANT, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Memory), "memory", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Mode), "mode", A_SYM, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_MSearch), "msearch", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Picture), "picture", A_SYM, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Play), "play", A_DEFSYM, A_DEFLONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_PlayTill), "playtill", A_LONG, A_DEFSYM, A_DEFLONG,
          A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_PSCEnable), "pscenable", A_SYM, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Repeat), "repeat", A_LONG, A_DEFSYM, A_DEFLONG,
          A_DEFLONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Reset), "reset", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Search), "search", A_LONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Show), "show", A_SYM, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Sound), "sound", A_LONG, A_SYM, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Status), "status", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_StepStill), "step&still", A_DEFSYM, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Still), "still", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Stop), "stop", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_XReset), "xreset", A_NOTHING);
  addinx(reinterpret_cast<method>(ldpProcessResponse), 1);
  /* Add object class to New Object List */
  finder_addclass("Devices", OUR_NAME);
  /* Make sure that our string list resource is present */
  rescopy('STR#', OUR_RES_NUMB);
  loadAboutBoxData();
  gChapterSymbol = gensym("chapter");
  gFwdSymbol = gensym("fwd");
  gEmptySymbol = gensym("");
  gFastSymbol = gensym("fast");
  gFrameSymbol = gensym("frame");
  gOffSymbol = gensym("off");
  gOnSymbol = gensym("on");
  gRevSymbol = gensym("rev");
  gRevFastSymbol = gensym("rev-fast");
  gRevScanSymbol = gensym("rev-scan");
  gRevSlowSymbol = gensym("rev-slow");
  gRevStepSymbol = gensym("rev-step");
  gScanSymbol = gensym("scan");
  gSlowSymbol = gensym("slow");
  gStepSymbol = gensym("step");
  gVersionNumber = reportVersion(OUR_NAME);
  loadOtherSegments();
  ExitCodeResource();
} /* main */

/*------------------------------------ ldpCreate ---*/
Pvoid ldpCreate
  (long pollRate,
   long infoRate,
   long poolSize)
{
  LdpControlPtr xx;

  EnterCallback();
  LOG_ENTER()
  xx = static_cast<LdpControlPtr>(newobject(gClass));
  if (xx)
  {
    xx->fPollClock = NULL_PTR;
    xx->fPollQueue = NULL_PTR;
    xx->fPool = NULL_PTR;
    if ((pollRate < 0) || (pollRate > MAX_POLL_RATE))
    {
      LOG_ERROR_2(OUTPUT_PREFIX "invalid polling rate (%ld) for device", pollRate)
      xx->fPollRate = SER_SAMPLE_RATE;
    }
    else
      xx->fPollRate = short(pollRate ? pollRate : SER_SAMPLE_RATE);
    if (infoRate < 0)
    {
      LOG_ERROR_2(OUTPUT_PREFIX "invalid info rate (%ld) for device", infoRate)
      xx->fInfoRate = INFO_SAMPLE_RATE;
    }
    else
      xx->fInfoRate = short(infoRate ? infoRate : INFO_SAMPLE_RATE);
    if ((poolSize < 0) || (poolSize > MAX_POOL_SIZE))
    {
      LOG_ERROR_2(OUTPUT_PREFIX "invalid pool size (%ld) for device", poolSize)
      xx->fPoolSize = POOL_SIZE;
    }
    else
      xx->fPoolSize = short(poolSize ? poolSize : POOL_SIZE);
    /* Set up our connections and private data */
    intin(xx, 1);
    xx->fErrorBangOut = static_cast<POutlet>(bangout(xx));
    xx->fFrameNumberOut = static_cast<POutlet>(intout(xx));
    xx->fChapterNumberOut = static_cast<POutlet>(intout(xx));
    xx->fCommandAccepted = static_cast<POutlet>(bangout(xx));
    xx->fCommandComplete = static_cast<POutlet>(bangout(xx));
    xx->fProgramStopCodeOut = static_cast<POutlet>(bangout(xx));
    xx->fCommandStatus = static_cast<POutlet>(intout(xx));
    xx->fKeyModeStatus = static_cast<POutlet>(intout(xx));
    xx->fPollerOut = static_cast<POutlet>(bangout(xx));
    xx->fCommandsOut = static_cast<POutlet>(intout(xx));
    xx->fPollClock = static_cast<PClock>(clock_new(xx,
                                              reinterpret_cast<method>(ldpProcessClock)));
    xx->fPollQueue = static_cast<PQelem>(qelem_new(xx,
                                              reinterpret_cast<method>(ldpProcessQueue)));
    xx->fFrameNumber = xx->fInfoCount = 0;
    xx->fInfoSamplingEnabled = (infoRate > 0);
    xx->fStopping = false;
    xx->fMode = LDP_FRAME_MODE;
    xx->fFirst = xx->fLast = xx->fInterruptPoint = NULL_PTR;
    xx->fPool = GETBYTES(xx->fPoolSize, LdpPacket);
    if (xx->fFrameNumberOut && xx->fChapterNumberOut && xx->fCommandComplete &&
        xx->fCommandStatus && xx->fKeyModeStatus && xx->fPollerOut &&
        xx->fCommandsOut && xx->fPollClock && xx->fPollQueue && xx->fPool)
    {
      LdpPacketPtr prev = NULL_PTR, curr = xx->fPool;

      for (short index = 0; index < xx->fPoolSize; index++)
      {
        curr->fPrev = prev;
        if (prev)
          prev->fNext = curr;
        prev = curr++;
      }
      prev->fNext = NULL_PTR;
      xx->fPoolAvailable = xx->fPoolSize;
      clock_delay(xx->fPollClock, xx->fPollRate);
    }
    else
    {
      LOG_ERROR_1(OUTPUT_PREFIX "unable to create port or clock for device")
      freeobject(reinterpret_cast<PObject>(xx));
      xx = NULL_PTR;
    }
  }
  ExitCodeResource();
  return xx;
} /* ldpCreate */

/*------------------------------------ ldpFree ---*/
Pvoid ldpFree
  (LdpControlPtr xx)
{
  EnterCallback();
  if (xx)
  {
  	xx->fStopping = true;
    if (xx->fPollClock)
    {
      clock_unset(xx->fPollClock);
      clock_free(reinterpret_cast<PObject>(xx->fPollClock));
      xx->fPollClock = NULL_PTR;
    }
    if (xx->fPollQueue)
    {
      qelem_unset(xx->fPollQueue);
      qelem_free(xx->fPollQueue);
      xx->fPollQueue = NULL_PTR;
    }
    FREEBYTES(xx->fPool, xx->fPoolSize)
  }
  LOG_EXIT()
  ExitMaxMessageHandler()
} /* ldpFree */

/*------------------------------------ ldpProcessClock ---*/
Pvoid ldpProcessClock
  (LdpControlPtr xx)
{
  EnterCallback();
  if (xx && (! xx->fStopping))
    qelem_set(xx->fPollQueue);
  ExitMaxMessageHandler()
} /* ldpProcessClock */

/*------------------------------------ ldpProcessQueue ---*/
Pvoid ldpProcessQueue
  (LdpControlPtr xx)
{
  EnterCallback();
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
          ldpInsertCommand(xx, xx->fInterruptPoint, LDP_CHAPTER_INQ,
                            LDP_AWAITING_CHAPTER_BYTE_1);
          ldpInsertCommand(xx, xx->fInterruptPoint, LDP_ADDR_INQ,
                            LDP_AWAITING_FRAME_BYTE_1);
          ldpInsertCommand(xx, xx->fInterruptPoint, LDP_STATUS_INQ,
                            LDP_AWAITING_STATUS_BYTE_1);
          ldpSendCommand(xx);
        }
        else if (! xx->fFirst)
        {
          /*ldpInitCommands(xx);*/
          ldpAddCommand(xx, LDP_CHAPTER_INQ, LDP_AWAITING_CHAPTER_BYTE_1);
          ldpAddCommand(xx, LDP_ADDR_INQ, LDP_AWAITING_FRAME_BYTE_1);
          ldpAddCommand(xx, LDP_STATUS_INQ, LDP_AWAITING_STATUS_BYTE_1);
          ldpSendCommand(xx);
        }
      }
      xx->fInfoCount = xx->fInfoRate;
    }
    lockout_set(prevLock);
    evnum_incr();
  }
  ExitMaxMessageHandler()
} /* ldpProcessQueue */

/*------------------------------------ ldpProcessResponse ---*/
Pvoid ldpProcessResponse
  (LdpControlPtr xx,
   long          rr)
{
  bool canAdvance = false, inError = false;

  EnterCallback();
  /* We've received a byte. Check if it matches what we are expecting. */
  if (xx)
  {
    /* Peek at the first packet: Don't grab it until we're happy! */
    short        prevLock = lockout_set(1);
    LdpPacketPtr aPacket = xx->fFirst;

    if (aPacket)
    {
      switch (aPacket->fState)
      {
        case LDP_AWAITING_ACK:
          if (rr == LDP_ACK)
            canAdvance = true;
          else
          {
            CheckResponse(rr);
            inError = true;
          }
          break;

        case LDP_AWAITING_COMPLETE:
          if (rr == LDP_COMPLETE)
          {
            canAdvance = true;
            if (xx->fInterruptPoint == aPacket)
              xx->fInterruptPoint = NULL_PTR;
          }
          else
          {
            CheckResponse(rr);
            inError = true;
          }
          break;

        case LDP_AWAITING_CHAPTER_BYTE_1:
          if ((rr >= '0') && (rr <= '9'))
          {
            aPacket->fState = LDP_AWAITING_CHAPTER_BYTE_2;
            xx->fChapterNumber = short(rr - '0');
          }
          else
          {
            CheckResponse(rr);
            xx->fChapterNumber = 0;
            inError = true;
          }
          break;

        case LDP_AWAITING_CHAPTER_BYTE_2:
          if ((rr >= '0') && (rr <= '9'))
          {
            xx->fChapterNumber = short(((xx->fChapterNumber) * 10) + rr - '0');
            outlet_int(xx->fChapterNumberOut, long(xx->fChapterNumber));
            canAdvance = true;
          }
          else
          {
            CheckResponse(rr);
            inError = true;
          }
          break;

        case LDP_AWAITING_FRAME_BYTE_1:
          if ((rr >= '0') && (rr <= '9'))
          {
            aPacket->fState = LDP_AWAITING_FRAME_BYTE_2;
            xx->fFrameNumber = rr - '0';
          }
          else
          {
            CheckResponse(rr);
            xx->fFrameNumber = 0;
            inError = true;
          }
          break;

        case LDP_AWAITING_FRAME_BYTE_2:
          if ((rr >= '0') && (rr <= '9'))
          {
            aPacket->fState = LDP_AWAITING_FRAME_BYTE_3;
            xx->fFrameNumber = ((xx->fFrameNumber) * 10) + rr - '0';
          }
          else
          {
            CheckResponse(rr);
            xx->fFrameNumber = 0;
            inError = true;
          }
          break;

        case LDP_AWAITING_FRAME_BYTE_3:
          if ((rr >= '0') && (rr <= '9'))
          {
            aPacket->fState = LDP_AWAITING_FRAME_BYTE_4;
            xx->fFrameNumber = ((xx->fFrameNumber) * 10) + rr - '0';
          }
          else
          {
            CheckResponse(rr);
            xx->fFrameNumber = 0;
            inError = true;
          }
          break;

        case LDP_AWAITING_FRAME_BYTE_4:
          if ((rr >= '0') && (rr <= '9'))
          {
            aPacket->fState = LDP_AWAITING_FRAME_BYTE_5;
            xx->fFrameNumber = ((xx->fFrameNumber) * 10) + rr - '0';
          }
          else
          {
            CheckResponse(rr);
            xx->fFrameNumber = 0;
            inError = true;
          }
          break;

        case LDP_AWAITING_FRAME_BYTE_5:
          if ((rr >= '0') && (rr <= '9'))
          {
            xx->fFrameNumber = ((xx->fFrameNumber) * 10) + rr - '0';
            outlet_int(xx->fFrameNumberOut, xx->fFrameNumber);
            canAdvance = true;
          }
          else
          {
            CheckResponse(rr);
            inError = true;
          }
          break;

        case LDP_AWAITING_STATUS_BYTE_1:
          aPacket->fState = LDP_AWAITING_STATUS_BYTE_2;
          break;

        case LDP_AWAITING_STATUS_BYTE_2:
          aPacket->fState = LDP_AWAITING_STATUS_BYTE_3;
          break;

        case LDP_AWAITING_STATUS_BYTE_3:
          aPacket->fState = LDP_AWAITING_STATUS_BYTE_4;
          break;

        case LDP_AWAITING_STATUS_BYTE_4:
          aPacket->fState = LDP_AWAITING_STATUS_BYTE_5;
          outlet_int(xx->fKeyModeStatus, rr);
          if (rr & 0x08)
            outlet_bang(xx->fProgramStopCodeOut);
          break;

        case LDP_AWAITING_STATUS_BYTE_5:
          outlet_int(xx->fCommandStatus, rr);
          canAdvance = true;
          break;

      }
    }
    else
    {
      CheckResponse(rr);
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
        if ((! aPacket) || ((aPacket->fState != LDP_not_waiting) &&
            (aPacket->fCommand != LDP_signal_accepted) &&
            (aPacket->fCommand != LDP_signal_done)))
          break;

        if (aPacket->fCommand == LDP_signal_done)
          outlet_bang(xx->fCommandComplete);
        else if (aPacket->fCommand == LDP_signal_accepted)
          outlet_bang(xx->fCommandAccepted);
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

      if (aCommand == LDP_can_interrupt)
        xx->fInterruptPoint = aPacket;
      else if ((aCommand != LDP_no_command) && (aCommand != LDP_signal_done))
        outlet_int(xx->fCommandsOut, long(aCommand));     
    }
    lockout_set(prevLock);
  }
  ExitMaxMessageHandler()
} /* ldpProcessResponse */

StandardAnythingRoutine(LdpControlPtr)
StandardInfoRoutine(LdpControlPtr)