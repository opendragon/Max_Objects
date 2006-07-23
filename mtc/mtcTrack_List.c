/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       mtcTrack_List.c                                                         */
/*                                                                                      */
/*  Contains:   The routine cmd_List().                                                 */
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

#include "mtcTrack.h"
#include <math.h>
#include <float.h>

/*------------------------------------ collectSamples ---*/
static bool collectSamples
  (MtcTrackPtr	xx,
   const short	argc,
   PAtom				argv)
{
  bool okSoFar = true;

  if ((argc % 3) || (argc > ((((MAX_POINTS * 3) * 3) + 1) / 2)))
  {
    LOG_ERROR_2(OUTPUT_PREFIX "bad vector size (%hd)", argc)
    okSoFar = false;
  }
  if (okSoFar)
  {
    short        count = short(argc / 3);
    MtcSamplePtr sWalk = xx->fSamples;
    PAtom        aWalk = argv;

    for (short ii = 0; ii < xx->fMaxSamples; ++ii)
      xx->fSamples[ii].fAvailable = false;
    if (count > xx->fMaxSamples)
      count = xx->fMaxSamples;
    for (short ii = 0; okSoFar && (ii < count); ++ii)
    {
      switch (aWalk->a_type)
      {
        case A_FLOAT:
          sWalk->fThisX = aWalk->a_w.w_float;
          break;

        case A_LONG:
          sWalk->fThisX = aWalk->a_w.w_long;
          break;

        case A_SYM:
          LOG_ERROR_2(OUTPUT_PREFIX "non-numeric in list (%s)", aWalk->a_w.w_sym->s_name)
          okSoFar = false;
          break;

        default:
          LOG_ERROR_1(OUTPUT_PREFIX "unknown value in list")
          okSoFar = false;
          break;

      }
      if (okSoFar)
      {
        ++aWalk;
        switch (aWalk->a_type)
        {
          case A_FLOAT:
            sWalk->fThisY = aWalk->a_w.w_float;
            break;

          case A_LONG:
            sWalk->fThisY = aWalk->a_w.w_long;
            break;

          case A_SYM:
            LOG_ERROR_2(OUTPUT_PREFIX "non-numeric in list (%s)",
                        aWalk->a_w.w_sym->s_name)
            okSoFar = false;
            break;

          default:
            LOG_ERROR_1(OUTPUT_PREFIX "unknown value in list")
            okSoFar = false;
            break;

        }
      }
      if (okSoFar)
      {
        ++aWalk;
        switch (aWalk->a_type)
        {
          case A_FLOAT:
            sWalk->fThisP = aWalk->a_w.w_float;
            break;

          case A_LONG:
            sWalk->fThisP = aWalk->a_w.w_long;
            break;

          case A_SYM:
            LOG_ERROR_2(OUTPUT_PREFIX "non-numeric in list (%s)",
                        aWalk->a_w.w_sym->s_name)
            okSoFar = false;
            break;

          default:
            LOG_ERROR_1(OUTPUT_PREFIX "unknown value in list")
            okSoFar = false;
            break;

        }
      }
      if (okSoFar)
      {
        ++aWalk;
        sWalk->fAvailable = true;
        ++sWalk;
      }
    }
    if (okSoFar)
      xx->fSampleCount = count;
  }
  if (! okSoFar)
    outlet_bang(xx->fErrorBangOut);
  return okSoFar;
} /* collectSamples */

/*------------------------------------ calculateDistances ---*/
static void calculateDistances
  (MtcTrackPtr xx)
{
  MtcSamplePtr sWalk = xx->fSamples;

  for (short ii = 0; ii < xx->fSampleCount; ++ii, ++sWalk)
  {
    Pdouble        actPtr = sWalk->fActDistance;
    double         thisX = sWalk->fThisX, thisY = sWalk->fThisY;
    MtcRetainedPtr rWalk = xx->fRetainedData;

    for (short jj = 0; jj < xx->fRetainedCount; ++jj, ++rWalk, ++actPtr)
    {
      if (rWalk->fValid)
        *actPtr = ((rWalk->fLastX - thisX) * (rWalk->fLastX - thisX)) +
                    ((rWalk->fLastY - thisY) * (rWalk->fLastY - thisY));
      else
        *actPtr = FLT_MAX;
    }
  }
} /* calculateDistances */

/*------------------------------------ assignSamples ---*/
static void assignSamples
  (MtcTrackPtr xx)
{
  MtcRetainedPtr rWalk = xx->fRetainedData;
  MtcSamplePtr   sWalk;
  short          rest = xx->fSampleCount;
  short          oldCount = xx->fRetainedCount, newCount = 0;

  for (short ii = 0; ii < oldCount; ++ii, ++rWalk)
  {
    /* Match the current set of retained points */
    double currBest = -1;
    short  currMatch = -1;

    sWalk = xx->fSamples;
    for (short jj = 0; jj < xx->fSampleCount; ++jj, ++sWalk)
    {
      if (sWalk->fAvailable)
      {
        /* Note that the distances are always positive */
        double thisDistance = *(sWalk->fActDistance + ii);

        if (currBest >= 0)
        {
          if (thisDistance < currBest)
          {
            currBest = thisDistance;
            currMatch = jj;
          }
        }
        else if ((xx->fThreshold <= 0) || (thisDistance <= xx->fThreshold))
        {
          /* Only select this candidate if it's within the threshold, or */
          /* we're not using a threshold. */
          currBest = thisDistance;
          currMatch = jj;
        }
      }      
    }
    if (currBest >= 0)
    {
      sWalk = xx->fSamples + currMatch;
      sWalk->fAvailable = false;
      rWalk->fNewP = sWalk->fThisP;
      rWalk->fNewX = sWalk->fThisX;
      rWalk->fNewY = sWalk->fThisY;
      rWalk->fDeltaX = rWalk->fNewX - rWalk->fLastX;
      rWalk->fDeltaY = rWalk->fNewY - rWalk->fLastY;
      rWalk->fForce = (rWalk->fNewP + rWalk->fLastP) / 2;
      rWalk->fVelocity = sqrt((rWalk->fDeltaX * rWalk->fDeltaX) +
                              (rWalk->fDeltaY * rWalk->fDeltaY));
      rWalk->fValid = true;
      --rest;
      ++newCount;
    }
    else
      rWalk->fValid = false;
  }
  /* Remove any unmatched retained points, by moving later points earlier */
  if (newCount < oldCount)
  {
    MtcRetainedPtr outRetPtr = xx->fRetainedData;

    rWalk = xx->fRetainedData;
    for (short ii = 0; ii < newCount; ++ii, ++rWalk)
    {
      if (rWalk->fValid)
      {
        if (outRetPtr != rWalk)
          *outRetPtr = *rWalk;
        ++outRetPtr;
      }
    }
  }
  /* Check if we have samples left over: */
  if (rest > 0)
  {
    rWalk = xx->fRetainedData + newCount;
    sWalk = xx->fSamples;
    for (short jj = 0; rest && (jj < xx->fSampleCount); ++jj, ++sWalk)
    {
      if (sWalk->fAvailable)
      {
        rWalk->fNewP = rWalk->fLastP = sWalk->fThisP;
        rWalk->fNewX = rWalk->fLastX = sWalk->fThisX;
        rWalk->fNewY = rWalk->fLastY = sWalk->fThisY;
        rWalk->fDeltaX = rWalk->fDeltaY = 0;
        rWalk->fVelocity = 0;
        rWalk->fForce = sWalk->fThisP;
        rWalk->fValid = true;
        ++rWalk;
        --rest;
        ++newCount;
      }
    }
  }
  xx->fRetainedCount = newCount;
} /* assignSamples */

/*------------------------------------ generateOutput ---*/
static void generateOutput
  (MtcTrackPtr xx)
{
  Atom           resultVector[8];
  short          outCount = xx->fRetainedCount, offset = 0;
  short          resultSize = (sizeof(resultVector) / sizeof(*resultVector));
  MtcRetainedPtr rWalk = xx->fRetainedData;

  if (xx->fAddBatchNumber)
    offset = 1;
  else
    --resultSize;
  if (xx->fAddIndex)
    ++offset;
  else
    --resultSize;
  outlet_int(xx->fPointCountOut, long(outCount));
  for (short ii = 0; ii < outCount; ++ii, ++rWalk)
  {
    if (xx->fAddBatchNumber)
    {
      SETLONG(resultVector, xx->fBatchNumber);
      if (xx->fAddIndex)
        SETLONG(resultVector + 1, long(ii));
    }
    else if (xx->fAddIndex)
      SETLONG(resultVector, long(ii));
    SETFLOAT(resultVector + offset, rWalk->fLastX);
    SETFLOAT(resultVector + offset + 1, rWalk->fLastY);
    SETFLOAT(resultVector + offset + 2, rWalk->fNewX);
    SETFLOAT(resultVector + offset + 3, rWalk->fNewY);
    SETFLOAT(resultVector + offset + 4, float(rWalk->fVelocity));
    SETFLOAT(resultVector + offset + 5, float(rWalk->fForce));
    outlet_list(xx->fResultOut, NULL_PTR, resultSize, resultVector);
  }
  ++xx->fBatchNumber;
  outlet_bang(xx->fLinesCompleteOut);
} /* generateOutput */

/*------------------------------------ cmd_List ---*/
Pvoid cmd_List
  (MtcTrackPtr xx,
   PSymbol     message,
   short       argc,
   PAtom       argv)
{
#pragma unused(message)
  EnterCallback();
  if (xx)
  {
    if (collectSamples(xx, argc, argv))
    {
      MtcRetainedPtr rWalk = xx->fRetainedData;

      /* Previous new values should become the old values: */
      for (short ii = 0; ii < xx->fRetainedCount; ++ii, ++rWalk)
      {
        rWalk->fLastX = rWalk->fNewX;
        rWalk->fLastY = rWalk->fNewY;
        rWalk->fLastP = rWalk->fNewP;
      }
      calculateDistances(xx);
      assignSamples(xx);
      generateOutput(xx);
    }
  }
  ExitMaxMessageHandler()
} /* cmd_List */
