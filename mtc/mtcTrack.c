/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       mtcTrack.c                                                              */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the mtcTrack module.      */
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
/*  Created:    2001/04/18                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#define IS_MAIN /* */
#include "mtcTrack.h"
#include "displayAboutBox.h"
#include "loadAboutBoxData.h"
#include "loadOtherSegments.h"
#include "reportAnything.h"
#include "reportVersion.h"

/* Forward references: */
Pvoid MtcTrackCreate
  (long    howMany,
   PSymbol optionOne,
   PSymbol optionTwo);

Pvoid MtcTrackFree
  (MtcTrackPtr xx);

static bool setUpMemory
  (MtcTrackPtr xx);

/*------------------------------------ main ---*/
void main
  (Pfptr ff)
{
  EnterCodeResource();
  PrepareCallback();
  FNS = ff;   /* Connect up the function macros. */
  /* Allocate class memory and set up class. */
  setup(reinterpret_cast<t_messlist**>(&gClass), reinterpret_cast<method>(MtcTrackCreate),
        reinterpret_cast<method>(MtcTrackFree), short(sizeof(MtcTrackData)),
        reinterpret_cast<method>(0L), A_LONG, A_DEFSYM, A_DEFSYM, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Anything), "anything", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Assist), "assist", A_CANT, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Batch), "batch", A_DEFSYM, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Clear), "clear", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Index), "index", A_DEFSYM, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Info), "info", A_CANT, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_List), "list", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Threshold), "threshold", A_FLOAT, A_NOTHING);
  /* Add object class to New Object List */
  finder_addclass("Lists", OUR_NAME);
  /* Make sure that our string list resource is present */
  rescopy('STR#', OUR_RES_NUMB);
  loadAboutBoxData();
  gBatchSymbol = gensym("batch");
  gEmptySymbol = gensym("");
  gIndexSymbol = gensym("index");
  gOffSymbol = gensym("off");
  gOnSymbol = gensym("on");
  gVersionNumber = reportVersion(OUR_NAME);
  loadOtherSegments();
  ExitCodeResource();
} /* main */

/*------------------------------------ MtcTrackCreate ---*/
Pvoid MtcTrackCreate
  (long    howMany,
   PSymbol optionOne,
   PSymbol optionTwo)
{
  MtcTrackPtr xx;

  EnterCallback();
  LOG_ENTER()
  xx = static_cast<MtcTrackPtr>(newobject(gClass));
  if (xx)
  {
    xx->fHowMany = short(howMany);
    xx->fMaxSamples = short(((howMany * 3) + 1) / 2);
    xx->fErrorBangOut = NULL_PTR;
    xx->fResultOut = NULL_PTR;
    xx->fPointCountOut = NULL_PTR;
    xx->fLinesCompleteOut = NULL_PTR;
    xx->fRetainedCount = xx->fSampleCount = 0;
    xx->fBatchNumber = 0;
    xx->fAddBatchNumber = xx->fAddIndex = false;
    xx->fThreshold = -1; /* So we don't initially use a threshold */
    xx->fActDistances = NULL_PTR;
    xx->fRetainedData = NULL_PTR;
    xx->fSamples = NULL_PTR;
    if ((howMany < 1) || (howMany > MAX_POINTS))
    {
      LOG_ERROR_2(OUTPUT_PREFIX "invalid number of points (%ld}", howMany)
      freeobject(reinterpret_cast<PObject>(xx));
      xx = NULL_PTR;
    }
    else
    {
      bool okSoFar = true;

      if (optionOne == gBatchSymbol)
        xx->fAddBatchNumber = true;
      else if (optionOne == gIndexSymbol)
        xx->fAddIndex = true;
      else if (optionOne != gEmptySymbol)
      {
        LOG_ERROR_2(OUTPUT_PREFIX "unrecognized option (%s)", optionOne->s_name)
        okSoFar = false;
      }
      if (okSoFar)
      {
        if (optionTwo == gBatchSymbol)
          xx->fAddBatchNumber = true;
        else if (optionTwo == gIndexSymbol)
          xx->fAddIndex = true;
        else if (optionTwo != gEmptySymbol)
        {
          LOG_ERROR_2(OUTPUT_PREFIX "unrecognized option (%s)", optionTwo->s_name)
          okSoFar = false;
        }
      }
      if (okSoFar)
      {            
        xx->fErrorBangOut = static_cast<POutlet>(bangout(xx));
        xx->fPointCountOut = static_cast<POutlet>(intout(xx));
        xx->fLinesCompleteOut = static_cast<POutlet>(bangout(xx));
        xx->fResultOut = static_cast<POutlet>(outlet_new(xx, NULL_PTR));
        if (xx->fResultOut && xx->fPointCountOut && xx->fLinesCompleteOut &&
            xx->fErrorBangOut)
        {
          if (! setUpMemory(xx))
          {
            freeobject(reinterpret_cast<PObject>(xx));
            xx = NULL_PTR;
          }
        }
        else
        {
          LOG_ERROR_1(OUTPUT_PREFIX "unable to create port for object")
          freeobject(reinterpret_cast<PObject>(xx));
          xx = NULL_PTR;
        }
      }
      else
      {
        freeobject(reinterpret_cast<PObject>(xx));
        xx = NULL_PTR;
      }
    }
  }
  ExitCodeResource();
  return xx;
} /* MtcTrackCreate */

/*------------------------------------ MtcTrackFree ---*/
Pvoid MtcTrackFree
  (MtcTrackPtr xx)
{
  EnterCallback();
  if (xx)
  {
    long howMany2 = xx->fHowMany * xx->fMaxSamples;

    FREEBYTES(xx->fActDistances, howMany2)
    FREEBYTES(xx->fSamples, xx->fMaxSamples)
    FREEBYTES(xx->fRetainedData, xx->fHowMany)
  }
  LOG_EXIT()
  ExitMaxMessageHandler()
} /* MtcTrackFree */

/*------------------------------------ setUpMemory ---*/
static bool setUpMemory
  (MtcTrackPtr xx)
{
  long howMany2 = xx->fHowMany * xx->fMaxSamples;
  bool okSoFar = false;

  xx->fActDistances = GETBYTES(howMany2, double);
  xx->fRetainedData = GETBYTES(xx->fHowMany, MtcRetainedData);
  xx->fSamples = GETBYTES(xx->fMaxSamples, MtcSampleData);
  if (xx->fActDistances && xx->fRetainedData && xx->fSamples)
  {
    MtcRetainedPtr retainedWalk = xx->fRetainedData;
    MtcSamplePtr   sampleWalk = xx->fSamples;
    Pdouble        actWalk = xx->fActDistances;

    for (short ii = 0; ii < xx->fHowMany; ++ii, ++retainedWalk)
    {
      retainedWalk->fLastP = retainedWalk->fLastX = retainedWalk->fLastY =
          retainedWalk->fDeltaX = retainedWalk->fDeltaY = retainedWalk->fNewP =
          retainedWalk->fNewX = retainedWalk->fNewY = 0;
      retainedWalk->fVelocity = retainedWalk->fForce = 0;
      retainedWalk->fValid = false;
    }
    for (short ii = 0; ii < xx->fMaxSamples; ++ii, ++sampleWalk)
    {
      sampleWalk->fThisP = sampleWalk->fThisX = sampleWalk->fThisY = 0;
      sampleWalk->fActDistance = actWalk;
      sampleWalk->fAvailable = false;
      actWalk += xx->fHowMany;
    }
    memset(xx->fActDistances, 0, howMany2 * sizeof(double));
    okSoFar = true;
  }
  return okSoFar;
} /* setUpMemory */

StandardAnythingRoutine(MtcTrackPtr)
StandardInfoRoutine(MtcTrackPtr)