/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       spaceball.c                                                             */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the spaceball module.     */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2001 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2001/07/13                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#define IS_MAIN /* */
#include "spaceball.h"
#include "displayAboutBox.h"
#include "loadAboutBoxData.h"
#include "loadOtherSegments.h"
#include "reportVersion.h"

/* Forward references: */
Pvoid spaceballCreate
  (PSymbol addOrDelta,
   long    pollRate);

Pvoid spaceballFree
  (SpaceballControlPtr xx);

Pvoid spaceballProcessClock
  (SpaceballControlPtr xx);

Pvoid spaceballProcessQueue
  (SpaceballControlPtr xx);

/*------------------------------------ main ---*/
void main
  (Pfptr ff)
{
  EnterCodeResource();
  PrepareCallback();
  FNS = ff;   /* Connect up the function macros. */
  /* Allocate class memory and set up class. */
  setup(reinterpret_cast<t_messlist**>(&gClass),
        reinterpret_cast<method>(spaceballCreate),
        reinterpret_cast<method>(spaceballFree), short(sizeof(SpaceballControl)),
        reinterpret_cast<method>(0L), A_DEFSYM, A_DEFLONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Anything), "anything", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Assist), "assist", A_CANT, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Info), "info", A_CANT, A_NOTHING);
  addint(reinterpret_cast<method>(cmd_Int));
  addmess(reinterpret_cast<method>(cmd_List), "list", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Mode), "mode", A_SYM, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Reset), "reset", A_NOTHING);
#if defined(BE_VERBOSE)
  addmess(reinterpret_cast<method>(cmd_Verbose), "verbose", A_DEFSYM, A_NOTHING);
#endif /* BE_VERBOSE */
  addmess(reinterpret_cast<method>(cmd_Zero), "zero", A_NOTHING);
  /* Add object class to New Object List */
  finder_addclass("Devices", OUR_NAME);
  /* Make sure that our string list resource is present */
  rescopy('STR#', OUR_RES_NUMB);
  loadAboutBoxData();
  gAddSymbol = gensym("add");
  gButtonSymbol = gensym("button");
  gDeltaSymbol = gensym("delta");
  gEmptySymbol = gensym("");
  gOffSymbol = gensym("off");
  gOnSymbol = gensym("on");
  gRotateSymbol = gensym("rotate");
  gTranslateSymbol = gensym("translate");
  gVersionNumber = reportVersion(OUR_NAME);
  loadOtherSegments();
  ExitCodeResource();
} /* main */

/*------------------------------------ spaceballCreate ---*/
Pvoid spaceballCreate
  (PSymbol addOrDelta,
   long    pollRate)
{
  SpaceballControlPtr xx;

  EnterCallback();
  LOG_ENTER()
  xx = static_cast<SpaceballControlPtr>(newobject(gClass));
  if (xx)
  {
    xx->fPollClock = NULL_PTR;
    xx->fPollQueue = NULL_PTR;
    xx->fBufferPos = xx->fDelayCounter = 0;
    xx->fButtons = 0;
    xx->fChunkPulseSent = xx->fInited = xx->fNextCharEscaped = xx->fOutputBlocked = 
        xx->fReset = xx->fSkipping = xx->fModeDelta = xx->fStopping = false;
    if ((pollRate < 0) || (pollRate > MAX_POLL_RATE))
    {
      LOG_ERROR_2(OUTPUT_PREFIX "invalid polling rate (%ld) for device", pollRate)
      xx->fPollRate = SER_SAMPLE_RATE;
    }
    else
      xx->fPollRate = short(pollRate ? pollRate : SER_SAMPLE_RATE);
    /* Set up our connections and private data */
    xx->fProxy = proxy_new(xx, 1L, &xx->fInletNumber);
    xx->fErrorBangOut = static_cast<POutlet>(bangout(xx));
    xx->fChunkSendOut = static_cast<POutlet>(bangout(xx));
    xx->fDataSendOut = static_cast<POutlet>(outlet_new(xx, 0L));  /* list, int */
    xx->fSampleBangOut = static_cast<POutlet>(bangout(xx));
    xx->fDataOut = static_cast<POutlet>(outlet_new(xx, 0L));   /* normally just a list */
    xx->fPollClock = static_cast<PClock>(clock_new(xx, reinterpret_cast<method>(spaceballProcessClock)));
    xx->fPollQueue = static_cast<PQelem>(qelem_new(xx, reinterpret_cast<method>(spaceballProcessQueue)));
    if (! (xx->fProxy && xx->fErrorBangOut && xx->fDataSendOut && xx->fChunkSendOut &&
            xx->fSampleBangOut && xx->fDataOut && xx->fPollClock && xx->fPollQueue))
    {
      LOG_ERROR_1(OUTPUT_PREFIX "unable to create port or clock for device")
      freeobject(reinterpret_cast<PObject>(xx));
      xx = NULL_PTR;
    }
    else
    {
      xx->fResetDuration = short((2000 + xx->fPollRate - 1) / xx->fPollRate);
      xx->fInitDuration = short((1000 + xx->fPollRate - 1) / xx->fPollRate);
      if (addOrDelta != gEmptySymbol)
        spaceballSetMode(xx, addOrDelta);
      clock_delay(xx->fPollClock, xx->fPollRate);
    }
  }
  ExitCodeResource();
  return xx;
} /* spaceballCreate */

/*------------------------------------ spaceballFree ---*/
Pvoid spaceballFree
  (SpaceballControlPtr xx)
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
    if (xx->fProxy)
    {
      freeobject(reinterpret_cast<PObject>(xx->fProxy));
      xx->fProxy = NULL_PTR;
    }
  }
  LOG_EXIT()
  ExitMaxMessageHandler()
} /* spaceballFree */

/*------------------------------------ spaceballPerformWriteCommand ---*/
void spaceballPerformWriteCommand
  (SpaceballControlPtr xx,
   const short         numBytesToSend,
   Puchar              bytesToFollow)
{
  uchar				dataValue;
  static Atom	dataList[OUT_BUFFER_SIZE];

  if (xx)
  {
    short prevLock = lockout_set(1);

    /* Make sure serialX is in 'chunk' mode. */
    if (! xx->fChunkPulseSent)
    {
      outlet_bang(xx->fChunkSendOut);
      xx->fChunkPulseSent = true;
    }
    /* Send data. */
    for (short ii = 0; ii < numBytesToSend; ii++)
    {
      dataValue = *bytesToFollow++;
      SETLONG(dataList + ii, dataValue);
    }
    outlet_list(xx->fDataSendOut, 0L, numBytesToSend, dataList);
    lockout_set(prevLock);
  }
} /* spaceballPerformWriteCommand */

/*------------------------------------ spaceballProcessClock ---*/
Pvoid spaceballProcessClock
  (SpaceballControlPtr xx)
{
  EnterCallback();
  if (xx && (! xx->fStopping))
    qelem_set(xx->fPollQueue);
  ExitMaxMessageHandler()
} /* spaceballProcessClock */

/*------------------------------------ spaceballProcessQueue ---*/
Pvoid spaceballProcessQueue
  (SpaceballControlPtr xx)
{
  EnterCallback();
  if (xx && (! xx->fStopping))
  {
    short prevLock = lockout_set(1);

    if (! xx->fReset)
    {
      static uchar resetString[] = "@RESET\015\015";

      if (! xx->fDelayCounter)
      {
        spaceballPerformWriteCommand(xx, sizeof(resetString) - 1, resetString);
        xx->fDelayCounter++;
      }
      else if (xx->fDelayCounter++ >= xx->fResetDuration)
      {
        xx->fReset = true;
        xx->fDelayCounter = 0;
      }
    }
    else if (! xx->fInited)
    {
      static uchar initString[] = "CB\015NT\015FR?\015P@r@r\015MSSV\015Z\015BcCcCc\015";

      if (! xx->fDelayCounter)
      {
        spaceballPerformWriteCommand(xx, sizeof(initString) - 1, initString);
        spaceballZeroValues(xx);
        xx->fDelayCounter++;
      }
      else if (xx->fDelayCounter++ >= xx->fInitDuration)
      {
        xx->fInited = true;
        xx->fDelayCounter = 0;
      }
    }
    outlet_bang(xx->fSampleBangOut);
    clock_delay(xx->fPollClock, xx->fPollRate);
    lockout_set(prevLock);
    evnum_incr();
  }
  ExitMaxMessageHandler()
} /* spaceballProcessQueue */

/*------------------------------------ spaceballProcessResponse ---*/
Pvoid spaceballProcessResponse
  (SpaceballControlPtr xx,
   long                rr)
{
  EnterCallback();
  if (xx)
  {
    short prevLock = lockout_set(1);
    uchar incoming = static_cast<uchar>(rr & 0x00ff);
    short slot = xx->fBufferPos;

    if (xx->fNextCharEscaped)
    {
      xx->fNextCharEscaped = false;
      if (incoming != SPACEBALL_RESPONSE_ESCAPE)
        incoming &= 0x001f;
      if (slot < IN_BUFFER_SIZE)
      {
        xx->fBuffer[slot] = incoming;
        xx->fBufferPos++;
      }
      else
      {
        xx->fBufferPos = 0;
        xx->fSkipping = true;
      }
    }
    else if (incoming == SPACEBALL_RESPONSE_END_CMD)
    {
      if (xx->fSkipping)
        xx->fSkipping = false;
      else if (xx->fBufferPos > 0)
        spaceballProcessPacket(xx);
      xx->fBufferPos = 0;
    }
    else if (incoming == SPACEBALL_RESPONSE_NEW_LINE)
    {
      if (! xx->fSkipping)
      {
        if (slot)
        {
          if (slot < IN_BUFFER_SIZE)
          {
            xx->fBuffer[slot] = incoming;
            xx->fBufferPos++;
          }
          else
          {
            xx->fBufferPos = 0;
            xx->fSkipping = true;
          }
        }
      }
    }
    else if (incoming == SPACEBALL_RESPONSE_XOFF)
      xx->fOutputBlocked = true;
    else if (incoming == SPACEBALL_RESPONSE_XON)
      xx->fOutputBlocked = false;
    else if (incoming == SPACEBALL_RESPONSE_ESCAPE)
    {
      if (! xx->fSkipping)
        xx->fNextCharEscaped = true;
    }
    else if (! xx->fSkipping)
    {
      if (slot < IN_BUFFER_SIZE)
      {
        xx->fBuffer[slot] = incoming;
        xx->fBufferPos++;
      }
      else
      {
        xx->fBufferPos = 0;
        xx->fSkipping = true;
      }
    }        
    lockout_set(prevLock);
  }
  ExitMaxMessageHandler()
} /* spaceballProcessResponse */

/*------------------------------------ spaceballSetMode ---*/
bool spaceballSetMode
  (SpaceballControlPtr xx,
   PSymbol             addOrDelta)
{
  bool okSoFar = true;

  if (addOrDelta == gAddSymbol)
    xx->fModeDelta = false;
  else if (addOrDelta == gDeltaSymbol)
    xx->fModeDelta = true;
  else
  {
    LOG_ERROR_2(OUTPUT_PREFIX "unrecognized processing mode (%s)", addOrDelta->s_name)
    okSoFar = false;
  }
  return okSoFar;
} /* spaceballSetMode */

StandardInfoRoutine(SpaceballControlPtr)