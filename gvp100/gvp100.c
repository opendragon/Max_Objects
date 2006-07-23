/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       gvp100.c                                                                */
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
#include "displayAboutBox.h"
#include "loadAboutBoxData.h"
#include "loadOtherSegments.h"
#include "reportAnything.h"
#include "reportVersion.h"

/* Forward references: */
Pvoid gvpCreate
  (long selectAddress,
   long pollRate,
   long poolSize);

Pvoid gvpFree
  (GvpControlPtr xx);

Pvoid gvpProcessClock
  (GvpControlPtr xx);

Pvoid gvpProcessQueue
  (GvpControlPtr xx);

Pvoid gvpProcessResponse
  (GvpControlPtr xx,
   long          rr);

/*------------------------------------ main ---*/
void main
  (Pfptr ff)
{
  EnterCodeResource();
  PrepareCallback();
  FNS = ff;   /* Connect up the function macros. */
  /* Allocate class memory and set up class. */
  setup(reinterpret_cast<t_messlist**>(&gClass), reinterpret_cast<method>(gvpCreate), 
        reinterpret_cast<method>(gvpFree), short(sizeof(GvpControl)), 
        reinterpret_cast<method>(0L), A_LONG, A_DEFLONG, A_DEFLONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_AllStop), "allstop", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Anything), "anything", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Assist), "assist", A_CANT, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_BreakDone), "breakdone", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Info), "info", A_CANT, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetCrosspoint), "c", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetCrosspoint), "crosspoint", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetDskClipLevel), "!c", A_FLOAT, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetDskClipLevel), "!clip", A_FLOAT, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetDskAnalogControl), "d", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetDskAnalogControl), "dskanalogcontrol", A_GIMME,
          A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetEffectsAnalogControl), "e", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetEffectsAnalogControl), "effectsanalogcontrol",
          A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetEffectsPosition), "!e", A_FLOAT, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetEffectsPosition), "!effects", A_FLOAT,
          A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_EndSequence), "endsequence", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetJoystick), "!j", A_FLOAT, A_FLOAT, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetJoystick), "!joystick", A_FLOAT, A_FLOAT,
          A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_LearnEmem), "learn-emem", A_DEFLONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetTransitionMode), "m", A_SYM, A_DEFSYM,
          A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetPushbutton), "off", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetPushbutton), "on", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetPushbutton), "p", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetPushbutton), "push", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetTransitionRate), "r", A_SYM, A_DEFLONG,
          A_DEFSYM, A_DEFSYM, A_DEFSYM, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_RecallEmem), "recall-emem", A_DEFLONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetTakePosition), "!t", A_FLOAT, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetTakePosition), "!take", A_FLOAT, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetTransitionMode), "transitionmode", A_SYM,
          A_DEFSYM, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetTransitionRate), "transitionrate", A_SYM,
          A_DEFLONG, A_DEFSYM, A_DEFSYM, A_DEFSYM, A_NOTHING);
#if defined(BE_VERBOSE)
  addmess(reinterpret_cast<method>(cmd_Verbose), "v", A_DEFSYM, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Verbose), "verbose", A_DEFSYM, A_NOTHING);
#endif /* BE_VERBOSE */
  addmess(reinterpret_cast<method>(cmd_SetWipePattern), "w", A_DEFLONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetWipePattern), "wipepattern", A_DEFLONG,
          A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_XReset), "x", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_XReset), "xreset", A_NOTHING);
  addinx(reinterpret_cast<method>(gvpProcessResponse), 1);
  /* Add object class to New Object List */
  finder_addclass("Devices", OUR_NAME);
  /* Make sure that our string list resource is present */
  rescopy('STR#', OUR_RES_NUMB);
  loadAboutBoxData();
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
  gVersionNumber = reportVersion(OUR_NAME);
  loadOtherSegments();
  ExitCodeResource();
} /* main */

/*------------------------------------ gvpReportStateChange ---*/
void gvpReportStateChange
  (GvpControlPtr xx)
{
#if defined(REPORT_STATE_CHANGES)
  if (xx && xx->fVerbose)
  {
    switch (xx->fState)
    {
      case GVP_IDLE:
        LOG_POST_1(OUTPUT_PREFIX "-> GVP_IDLE")
        break;

      case GVP_AWAITING_BREAK_DONE:
        LOG_POST_1(OUTPUT_PREFIX "-> GVP_AWAITING_BREAK_DONE")
        break;

      case GVP_AWAITING_ACK:
        LOG_POST_1(OUTPUT_PREFIX "-> GVP_AWAITING_ACK")
        break;

      case GVP_SELECTED:
        LOG_POST_1(OUTPUT_PREFIX "-> GVP_SELECTED")
        break;

      case GVP_AWAITING_BYTE_COUNT_1:
        LOG_POST_1(OUTPUT_PREFIX "-> GVP_AWAITING_BYTE_COUNT_1")
        break;

      case GVP_AWAITING_BYTE_COUNT_n:
        LOG_POST_1(OUTPUT_PREFIX "-> GVP_AWAITING_BYTE_COUNT_n")
        break;

      case GVP_AWAITING_STATUS_BYTE:
        LOG_POST_1(OUTPUT_PREFIX "-> GVP_AWAITING_STATUS_BYTE")
        break;

      default:
        LOG_POST_2(OUTPUT_PREFIX "-> unrecognized state (%ld)", xx->fState)
        break;

    }
  }
#else /* not REPORT_STATE_CHANGES */
 #pragma unused(xx)
#endif /* not REPORT_STATE_CHANGES */
} /* gvpReportStateChange */

/*------------------------------------ gvpCreate ---*/
Pvoid gvpCreate
  (long selectAddress,
   long pollRate,
   long poolSize)
{
  GvpControlPtr xx;

  EnterCallback();
  LOG_ENTER()
  xx = static_cast<GvpControlPtr>(newobject(gClass));
  if (xx)
  {
    xx->fSendCompletion = xx->fStopping = false;
    xx->fPollClock = NULL_PTR;
    xx->fPollQueue = NULL_PTR;
    xx->fPool = NULL_PTR;
    if ((selectAddress < 0) || (selectAddress > 254) || ((selectAddress & 1) != 0))
    {
      LOG_ERROR_2(OUTPUT_PREFIX "invalid select address (%ld) for device", selectAddress)
      xx->fSelectAddress = 0;
    }
    else
      xx->fSelectAddress = short(selectAddress);
    if ((pollRate < 0) || (pollRate > MAX_POLL_RATE))
    {
      LOG_ERROR_2(OUTPUT_PREFIX "invalid polling rate (%ld) for device", pollRate)
      xx->fPollRate = SER_SAMPLE_RATE;
    }
    else
      xx->fPollRate = short(pollRate ? pollRate : SER_SAMPLE_RATE);
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
    xx->fBreakSendOut = static_cast<POutlet>(bangout(xx));
    xx->fDataSendOut = static_cast<POutlet>(outlet_new(xx, 0L));  /* list, int */
    xx->fSampleBangOut = static_cast<POutlet>(bangout(xx));
    xx->fCommandComplete = static_cast<POutlet>(bangout(xx));
    xx->fSequenceComplete = static_cast<POutlet>(bangout(xx));
    xx->fPollClock = static_cast<PClock>(clock_new(xx,
                                            reinterpret_cast<method>(gvpProcessClock)));
    xx->fPollQueue = static_cast<PQelem>(qelem_new(xx,
                                            reinterpret_cast<method>(gvpProcessQueue)));
    xx->fFirst = xx->fLast = NULL_PTR;
    xx->fPool = GETBYTES(xx->fPoolSize, GvpPacket);
    if (xx->fErrorBangOut && xx->fDataSendOut && xx->fBreakSendOut &&
        xx->fSampleBangOut && xx->fSequenceComplete && xx->fPollClock && xx->fPollQueue &&
        xx->fPool)
    {
      GvpPacketPtr prev = NULL_PTR;
      GvpPacketPtr curr = xx->fPool;

      for (short index = 0; index < xx->fPoolSize; ++index)
      {
        curr->fPrev = prev;
        if (prev)
          prev->fNext = curr;
        prev = curr++;
      }
      clock_delay(xx->fPollClock, xx->fPollRate);
#if defined(BE_VERBOSE)
      xx->fVerbose = false;
#endif /* BE_VERBOSE */
      xx->fState = GVP_IDLE;
      gvpReportStateChange(xx);
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
} /* gvpCreate */

/*------------------------------------ gvpFree ---*/
Pvoid gvpFree
  (GvpControlPtr xx)
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
} /* gvpFree */

/*------------------------------------ gvpPerformWriteCommand ---*/
void gvpPerformWriteCommand
  (GvpControlPtr  xx,
   const long           effectsAddress,
   const GvpCommandCode commandCode,
   const uchar          numBytesToFollow,
   Puchar         			bytesToFollow,
   const GvpState       newState,
   const bool           lastCommand)
{
  uchar 			dataValue;
  static Atom	dataList[MAX_MESSAGE_BYTES + 3];

  if (xx)
  {
    short prevLock = lockout_set(1);

    if (xx->fState == GVP_SELECTED)
    {
      /* Send data, set expected state. */
      if (commandCode == GVP_end_sequence)
        outlet_bang(xx->fSequenceComplete);
      else
      {
        SETLONG(dataList, numBytesToFollow + 2);
        SETLONG(dataList + 1, long(effectsAddress & 0x00FF));
        SETLONG(dataList + 2, long(commandCode));
        for (short ii = 0; ii < numBytesToFollow; ++ii)
        {
          dataValue = *bytesToFollow++;
          SETLONG(dataList + ii + 3, dataValue);
        }
        outlet_list(xx->fDataSendOut, 0L, short(numBytesToFollow + 3), dataList);
      }
      xx->fSendCompletion = lastCommand;
      xx->fState = newState;
      gvpReportStateChange(xx);       
    }
    else if (gvpCheckPoolSpace(xx, 1))
    {
      GvpPacketPtr newPacket = gvpNewPacket(xx);

      newPacket->fPrev = xx->fLast;
      newPacket->fState = newState;
      if (xx->fLast)
        xx->fLast->fNext = newPacket;
      else
        xx->fFirst = newPacket;
      xx->fLast = newPacket;
      if (commandCode == GVP_end_sequence)
        /* Record the EndSequence request. */
        newPacket->fSize = -1;
      else
      {
        newPacket->fSendCompletion = lastCommand;
        newPacket->fSize = short(numBytesToFollow + 3);
        newPacket->fBuffer[0] = static_cast<uchar>(numBytesToFollow + 2);
        newPacket->fBuffer[1] = static_cast<uchar>(effectsAddress & 0x00FF);
        newPacket->fBuffer[2] = static_cast<uchar>(commandCode);
        if (numBytesToFollow > 0)
          memcpy(newPacket->fBuffer + 3, bytesToFollow, numBytesToFollow);
      }
    }
    else
      LOG_ERROR_1(OUTPUT_PREFIX "insufficient pool space to hold commands")
    lockout_set(prevLock);
  }
} /* gvpPerformWriteCommand */

/*------------------------------------ gvpProcessClock ---*/
Pvoid gvpProcessClock
  (GvpControlPtr xx)
{
  EnterCallback();
  if (xx)
    qelem_set(xx->fPollQueue);
  ExitMaxMessageHandler()
} /* gvpProcessClock */

/*------------------------------------ gvpProcessQueue ---*/
Pvoid gvpProcessQueue
  (GvpControlPtr xx)
{
  EnterCallback();
  if (xx && (! xx->fStopping))
  {
    short prevLock = lockout_set(1);

    outlet_bang(xx->fSampleBangOut);
    clock_delay(xx->fPollClock, xx->fPollRate);
    lockout_set(prevLock);
    evnum_incr();
  }
  ExitMaxMessageHandler()
} /* gvpProcessQueue */

/*------------------------------------ gvpProcessResponse ---*/
Pvoid gvpProcessResponse
  (GvpControlPtr xx,
   long          rr)
{
  bool canAdvance = false, inError = false;

  EnterCallback();
  /* We've received a byte. Check if it matches what we are expecting. */
  if (xx && (! xx->fStopping))
  {
    short        prevLock = lockout_set(1);
    GvpPacketPtr aPacket;
    long         incoming = (rr & 0x00ff);

#if defined(BE_VERBOSE)
    if (xx->fVerbose)
      LOG_POST_2(OUTPUT_PREFIX "saw byte: 0x%lx", incoming)
#endif /* BE_VERBOSE */
    switch (xx->fState)
    {
      case GVP_IDLE:
        LOG_ERROR_2(OUTPUT_PREFIX "unexpected character 0x%lx seen.", incoming)
        outlet_bang(xx->fErrorBangOut);
        break;

      case GVP_AWAITING_ACK:
        if (incoming == GVP_ACK_CHAR)
        {
          xx->fState = GVP_SELECTED;
          gvpReportStateChange(xx);
        }
        break;

      case GVP_SELECTED:
        if (incoming == GVP_NACK_CHAR)
        {
          LOG_ERROR_1(OUTPUT_PREFIX "NACK seen.")
          outlet_bang(xx->fErrorBangOut);
          xx->fState = GVP_IDLE;
          gvpReportStateChange(xx);
        }
        break;

      case GVP_AWAITING_BYTE_COUNT_1:
        if (incoming == 1)
        {
#if defined(BE_VERBOSE)
          if (xx->fVerbose)
            LOG_POST_1(OUTPUT_PREFIX "saw byte count of 1.")
#endif /* BE_VERBOSE */
          xx->fState = GVP_AWAITING_STATUS_BYTE;
          gvpReportStateChange(xx);
        }
        else
        {
          LOG_ERROR_1(OUTPUT_PREFIX "bad byte count seen.")
          outlet_bang(xx->fErrorBangOut);
        }
        break;

      case GVP_AWAITING_BYTE_COUNT_n:
#if defined(BE_VERBOSE)
        if (xx->fVerbose)
          LOG_POST_2(OUTPUT_PREFIX "saw byte count of %ld.", incoming)
#endif /* BE_VERBOSE */
        break;

      case GVP_AWAITING_STATUS_BYTE:
#if defined(BE_VERBOSE)
        if (xx->fVerbose)
          LOG_POST_2(OUTPUT_PREFIX "status 0x%lx", incoming)
#endif /* BE_VERBOSE */
        if (incoming & GVP_STATUS_BUFFER_WARNING)
          LOG_POST_1(OUTPUT_PREFIX "buffer warning issued.")
        if (incoming & GVP_STATUS_PROTOCOL_ERROR)
        {
          LOG_ERROR_1(OUTPUT_PREFIX "protocol error issued.")
          outlet_bang(xx->fErrorBangOut);
        }
        else if (! (incoming & GVP_STATUS_COMMAND_ACCEPTED))
          LOG_POST_1(OUTPUT_PREFIX "invalid status detected.")
        if (xx->fSendCompletion)
        {
          outlet_bang(xx->fCommandComplete);
          xx->fSendCompletion = false;
#if defined(BE_VERBOSE)
          if (xx->fVerbose)
            LOG_POST_1(OUTPUT_PREFIX "completion signalled")
#endif /* BE_VERBOSE */
        }
        /* Are there any messages waiting to go? */
        aPacket = gvpGetFirstPacket(xx);
        if (aPacket)
        {
          if (aPacket->fSize >= 0)
          {
            uchar				dataValue;
            static Atom	dataList[128];

            for (short ii = 0; ii < aPacket->fSize; ++ii)
            {
              dataValue = aPacket->fBuffer[ii];
              SETLONG(dataList + ii, dataValue);
            }
            outlet_list(xx->fDataSendOut, 0L, aPacket->fSize, dataList);
            xx->fSendCompletion = aPacket->fSendCompletion;
          }
          else
            /* EndSequence to be processed. */
            outlet_bang(xx->fSequenceComplete);
          xx->fState = aPacket->fState;
          gvpReportStateChange(xx);
          gvpReleasePacket(xx, aPacket);
        }
        else
        {
          xx->fState = GVP_SELECTED;
          gvpReportStateChange(xx);
        }
        break;

    }
    lockout_set(prevLock);      
  }
  ExitMaxMessageHandler()
} /* gvpProcessResponse */

StandardAnythingRoutine(GvpControlPtr)
StandardInfoRoutine(GvpControlPtr)