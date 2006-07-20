/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       x10.c                                                                   */
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
#include "displayAboutBox.h"
#include "loadAboutBoxData.h"
#include "loadOtherSegments.h"
#include "reportAnything.h"
#include "reportVersion.h"

/* Forward references: */
Pvoid x10Create
  (PSymbol kind,
   long    pollRate);

Pvoid x10Free
  (X10ControlPtr xx);

Pvoid x10ProcessClock
  (X10ControlPtr xx);

Pvoid x10ProcessQueue
  (X10ControlPtr xx);

Pvoid x10ProcessResponse
  (X10ControlPtr xx,
   long          rr);

static char kHouseCodeNames[NUM_HOUSECODES][2] =
{
  "A", "B", "C", "D", "E", "F", "G", "H",
  "I", "J", "K", "L", "M", "N", "O", "P"
};

static char kBitsToHouseCode[NUM_HOUSECODES] =
{
  'M', /* 0x0 */
  'E', /* 0x1 */
  'C', /* 0x2 */
  'K', /* 0x3 */
  'O', /* 0x4 */
  'G', /* 0x5 */
  'A', /* 0x6 */
  'I', /* 0x7 */
  'N', /* 0x8 */
  'F', /* 0x9 */
  'D', /* 0xA */
  'L', /* 0xB */
  'P', /* 0xC */
  'H', /* 0xD */
  'B', /* 0xE */
  'J'  /* 0xF */
};

/*------------------------------------ main ---*/
void main
  (Pfptr ff)
{
  EnterCodeResource();
  PrepareCallback();
  FNS = ff;   /* Connect up the function macros. */
  /* Allocate class memory and set up class. */
  setup(reinterpret_cast<t_messlist**>(&gClass), reinterpret_cast<method>(x10Create),
        reinterpret_cast<method>(x10Free), short(sizeof(X10Control)),
        reinterpret_cast<method>(0L),A_DEFSYM, A_DEFLONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Anything), "anything", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Assist), "assist", A_CANT, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_ClearTimerEvent), "clear!", A_LONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetClock), "clock!", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Dim), "dim", A_SYM, A_LONG, A_DEFLONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_GetTimerEvents), "events?", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetEveryDayEvent), "everyday!", A_SYM, A_LONG,
          A_LONG, A_LONG, A_SYM, A_DEFLONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetGraphicsData), "graphics!", A_LONG, A_DEFLONG,
          A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_GetGraphicsData), "graphics?", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetHouseCode), "housecode!", A_SYM, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_GetHouseCode), "housecode?", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Info), "info", A_CANT, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Kind), "kind", A_SYM, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetNormalEvent), "normal!", A_SYM, A_LONG, A_LONG,
          A_LONG, A_LONG, A_SYM, A_DEFLONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Off), "off", A_SYM, A_LONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_On), "on", A_SYM, A_LONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Reset), "reset", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetSecurityEvent), "security!", A_SYM, A_LONG,
          A_LONG, A_LONG, A_LONG, A_SYM, A_DEFLONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetTodayEvent), "today!", A_SYM, A_LONG, A_LONG,
          A_LONG, A_SYM, A_DEFLONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetTomorrowEvent), "tomorrow!", A_SYM, A_LONG,
          A_LONG, A_LONG, A_SYM,  A_DEFLONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_XReset), "xreset", A_NOTHING);
  addinx(reinterpret_cast<method>(x10ProcessResponse), 1);
  /* Add object class to New Object List */
  finder_addclass("Devices", OUR_NAME);
  /* Make sure that our string list resource is present */
  rescopy('STR#', OUR_RES_NUMB);
  loadAboutBoxData();
  gCM11Symbol = gensym("cm11");
  gCP290Symbol = gensym("cp290");
  gDimSymbol = gensym("dim");
  gEmptySymbol = gensym("");
  gOffSymbol = gensym("off");
  gOnSymbol = gensym("on");
  for (ushort ii = 0; ii < NUM_HOUSECODES; ii++)
    gHouseCodes[ii] = gensym(kHouseCodeNames[ii]);
  gVersionNumber = reportVersion(OUR_NAME);
  loadOtherSegments();
  ExitCodeResource();
} /* main */

/*------------------------------------ x10Create ---*/
Pvoid x10Create
  (PSymbol kind,
   long    pollRate)
{
  X10ControlPtr xx;

  EnterCallback();
  LOG_ENTER()
  xx = static_cast<X10ControlPtr>(newobject(gClass));
  if (xx)
  {
    xx->fHouseCodeChar = 0;
    xx->fFunctionChar = x10CM11FunctionUnitsOff;
    xx->fDimLevel = 0;
    xx->fDeviceMap = 0;
    xx->fCompletedWhenStatus = xx->fIgnoreChecksum = xx->fStopping = false;
    xx->fOutCmd = xx->fOutArea = NULL_PTR;
    if ((kind == gCM11Symbol) || (kind == gEmptySymbol))
      xx->fKind = X10KindCM11;
    else if (kind == gCP290Symbol)
      xx->fKind = X10KindCP290;
    else
    {
      LOG_ERROR_2(OUTPUT_PREFIX "invalid kind '%s' for device", kind->s_name)
      xx->fKind = X10KindCM11;
    }
    if ((pollRate < 0) || (pollRate > MAX_POLL_RATE))
    {
      LOG_ERROR_2(OUTPUT_PREFIX "invalid polling rate (%ld) for device", pollRate)
      xx->fPollRate = SER_SAMPLE_RATE;
    }
    else
      xx->fPollRate = short(pollRate ? pollRate : SER_SAMPLE_RATE);
    /* Set up our connections and private data */
    intin(xx, 1);
    xx->fErrorBangOut = static_cast<POutlet>(bangout(xx));
    xx->fMinuteOut = static_cast<POutlet>(intout(xx));
    xx->fHourOut = static_cast<POutlet>(intout(xx));
    xx->fDayOut = static_cast<POutlet>(intout(xx));
    xx->fDeviceStatus = static_cast<POutlet>(intout(xx));
    xx->fCommandComplete = static_cast<POutlet>(bangout(xx));
    xx->fHouseCode = static_cast<POutlet>(intout(xx));
    xx->fPollerOut = static_cast<POutlet>(bangout(xx));
    xx->fCommandsOut = static_cast<POutlet>(intout(xx));
    xx->fPollClock = static_cast<PClock>(clock_new(xx,
                                            reinterpret_cast<method>(x10ProcessClock)));
    xx->fPollQueue = static_cast<PQelem>(qelem_new(xx,
                                            reinterpret_cast<method>(x10ProcessQueue)));
    xx->fOutBuffer = GETBYTES(OUTBUFF_SIZE, uchar);
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

    }
    xx->fAllOnCount = xx->fEntryCount = xx->fEventByteCount = 0;
    if (xx->fErrorBangOut && xx->fMinuteOut && xx->fHourOut && xx->fDayOut &&
        xx->fDeviceStatus && xx->fCommandComplete && xx->fHouseCode &&
        xx->fPollerOut && xx->fCommandsOut && xx->fPollClock && xx->fPollQueue &&
        xx->fOutBuffer)
    {
      for (short ii = 0; ii < COMMAND_PREFIX_SIZE; ii++)
        *(xx->fOutBuffer + ii) = ALL_ON;
      xx->fOutCmd = xx->fOutBuffer + COMMAND_PREFIX_SIZE;
      xx->fOutArea = xx->fOutBuffer + COMMAND_PREFIX_SIZE + 1;
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
} /* x10Create */

/*------------------------------------ x10Free ---*/
Pvoid x10Free
  (X10ControlPtr xx)
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
    FREEBYTES(xx->fOutBuffer, OUTBUFF_SIZE)
  }
  LOG_EXIT()
  ExitMaxMessageHandler()
} /* x10Free */

/*------------------------------------ x10ProcessClock ---*/
Pvoid x10ProcessClock
  (X10ControlPtr xx)
{
  EnterCallback();
  if (xx && (! xx->fStopping))
    qelem_set(xx->fPollQueue);
  ExitMaxMessageHandler()
} /* x10ProcessClock */

/*------------------------------------ x10ProcessQueue ---*/
Pvoid x10ProcessQueue
  (X10ControlPtr xx)
{
  EnterCallback();
  if (xx && (! xx->fStopping))
  {
    short prevLock = lockout_set(1);

    outlet_bang(xx->fPollerOut);
    clock_delay(xx->fPollClock, xx->fPollRate);
    lockout_set(prevLock);
    evnum_incr();
  }
  ExitMaxMessageHandler()
} /* x10ProcessQueue */

/*------------------------------------ x10DoCM11 ---*/
static void x10DoCM11
  (X10ControlPtr	xx,
   const long			rr)
{
	long	workingCmd = (rr & 0x0ff);
	
  //post("    saw %lx state: %d[%d]", rr, xx->fMajorState, xx->fMinorState);//!!
  switch (xx->fMinorState)
  {
    case x10CM11MinorIdle:
      if (workingCmd == CM11_POLL_CHAR)
      {
        // Prepare to receive a status poll
        outlet_int(xx->fCommandsOut, CM11_POLL_ACK);
        xx->fMinorState = x10CM11MinorAwaitingByteCount;
      }
      else if (workingCmd == CM11_CLOCK_CHAR)
        // Prepare to receive a clock request
        x10CM11SendClockInfo(xx, false, false);
      break;

    case x10CM11MinorAwaitingChecksum:
      if (workingCmd == xx->fChecksum)
      {
        xx->fMinorState = x10CM11MinorAwaitingReady;
        outlet_int(xx->fCommandsOut, CM11_CHECKSUM_ACK);
      }
      else if ((xx->fIndex < xx->fOutCount) && (workingCmd == *(xx->fOutBuffer + xx->fIndex)))
        // We're getting an echo!
        xx->fIndex++;
      else if (xx->fIgnoreChecksum)
      {
        xx->fMinorState = x10CM11MinorIdle;
        xx->fIgnoreChecksum = false;
        if (xx->fCompletedWhenStatus)
          outlet_bang(xx->fCommandComplete);
      }
      else
        x10CM11ResendBuffer(xx);
      break;

    case x10CM11MinorAwaitingReady:
      if (workingCmd == CM11_READY_CHAR)
      {
        xx->fMinorState = x10CM11MinorIdle;
        // Determine if there are any more addresses
        if (xx->fMajorState == x10CM11MajorSendingSingleRequest)
        {
          outlet_bang(xx->fCommandComplete);
          xx->fMajorState = x10CM11MajorIdle;
        }
        else if (xx->fDeviceMap)
          x10CM11SendDeviceAddress(xx);
        else if (xx->fMajorState == x10CM11MajorSendingMultiRequest)
        {
          x10CM11SendFunctionCode(xx);
          xx->fMajorState = x10CM11MajorIdle;
        }
        else if (xx->fCompletedWhenStatus)
          outlet_bang(xx->fCommandComplete);
      }
      else if (workingCmd == CM11_CHECKSUM_ACK)
      {
        // We're getting an echo!
      }
      else
        outlet_bang(xx->fErrorBangOut);
      break;

    case x10CM11MinorAwaitingByteCount:
      if (workingCmd != CM11_POLL_CHAR)
      {
        xx->fIndex = static_cast<ushort>(workingCmd);
        xx->fMinorState = x10CM11MinorGettingPollChars;
      }
      break;

    case x10CM11MinorGettingPollChars:
      if (! --xx->fIndex)
        xx->fMinorState = x10CM11MinorIdle;
      break;

  }
} /* x10DoCM11 */

/*------------------------------------ x10DoCP290 ---*/
static void x10DoCP290
  (X10ControlPtr	xx,
   const long			rr)
{
  switch (xx->fMinorState)
  {
    case x10CP290MinorIdle:
      if (rr == ALL_ON)
      {
        xx->fMinorState = x10CP290MinorSawFF;
        xx->fAllOnCount = 1;
      }
      break;

    case x10CP290MinorSawFF:
      if (rr == ALL_ON)
        ++xx->fAllOnCount;
      else if (xx->fAllOnCount >= 6)
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
          if (rr == ALL_ON)
          {
            xx->fEntryCount++;
            xx->fMinorState = x10CP290MinorSawIcon2;
          }
          else
            xx->fMinorState = x10CP290MinorSawIcon1;
          break;

        case x10CP290MajorAwaitingTimerEvents:
          if (rr == ALL_ON)
          {
            xx->fEntryCount++;
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
          outlet_int(xx->fHouseCode, long(rr >> 4));
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
          outlet_int(xx->fHouseCode, long(rr >> 4));
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
          if (xx->fEntryCount >= 256)
          {
            outlet_bang(xx->fCommandComplete);
            xx->fEntryCount = 0;
            xx->fMajorState = x10CP290MajorIdle;
            xx->fMinorState = x10CP290MinorIdle;
          }
          else if (rr == ALL_ON)
            xx->fEntryCount++;
          else
            xx->fMinorState = x10CP290MinorSawIcon1;
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
          if (xx->fEventByteCount == 8)
            xx->fMinorState = x10CP290MinorSawEvent8;
          else
            xx->fEventByteCount++;
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
          if (xx->fEntryCount >= 128)
          {
            outlet_bang(xx->fCommandComplete);
            xx->fEntryCount = 0;
            xx->fMajorState = x10CP290MajorIdle;
            xx->fMinorState = x10CP290MinorIdle;
          }
          else if (rr == ALL_ON)
            xx->fEntryCount++;
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

  }
} /* x10DoCP290 */

/*------------------------------------ x10ProcessResponse ---*/
Pvoid x10ProcessResponse
  (X10ControlPtr	xx,
   const long			rr)
{
  EnterCallback();
  /* We've received a byte. Check if it matches what we are expecting. */
  if (xx)
  {
    switch (xx->fKind)
    {
      case X10KindCM11:
        x10DoCM11(xx, rr);
        break;


      case X10KindCP290:
        x10DoCP290(xx, rr);
        break;

    }
  }
  ExitMaxMessageHandler()
} /* x10ProcessResponse */

StandardAnythingRoutine(X10ControlPtr)
StandardInfoRoutine(X10ControlPtr)