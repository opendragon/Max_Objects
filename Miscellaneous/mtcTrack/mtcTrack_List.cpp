/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       mtcTrack_List.cpp                                                       */
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
static bool collectSamples(MtcTrackData * xx,
                           const short    argc,
                           t_atom *       argv)
{
    bool okSoFar = true;

    if ((argc % 3) || (argc > ((((MAX_POINTS * 3) * 3) + 1) / 2)))
    {
        LOG_ERROR_2(xx, OUTPUT_PREFIX "bad vector size (%d)", static_cast<int>(argc))
        okSoFar = false;
    }
    if (okSoFar)
    {
        short           count = static_cast<short>(argc / 3);
        MtcSampleData * sWalk = xx->fSamples;
        t_atom *        aWalk = argv;

        for (short ii = 0; ii < xx->fMaxSamples; ++ii)
        {
            xx->fSamples[ii].fAvailable = false;
        }
        if (count > xx->fMaxSamples)
        {
            count = xx->fMaxSamples;
        }
        for (short ii = 0; okSoFar && (ii < count); ++ii)
        {
            switch (aWalk->a_type)
            {
                case A_FLOAT:
                    sWalk->fThisX = aWalk->a_w.w_float;
                    break;

                case A_LONG:
                    sWalk->fThisX = TO_DBL(aWalk->a_w.w_long);
                    break;

                case A_SYM:
                    LOG_ERROR_2(xx, OUTPUT_PREFIX "non-numeric in list (%s)",
                                aWalk->a_w.w_sym->s_name)
                    okSoFar = false;
                    break;

                default:
                    LOG_ERROR_1(xx, OUTPUT_PREFIX "unknown value in list")
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
                        sWalk->fThisY = TO_DBL(aWalk->a_w.w_long);
                        break;

                    case A_SYM:
                        LOG_ERROR_2(xx, OUTPUT_PREFIX "non-numeric in list (%s)",
                                    aWalk->a_w.w_sym->s_name)
                        okSoFar = false;
                        break;

                    default:
                        LOG_ERROR_1(xx, OUTPUT_PREFIX "unknown value in list")
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
                        sWalk->fThisP = TO_DBL(aWalk->a_w.w_long);
                        break;

                    case A_SYM:
                        LOG_ERROR_2(xx, OUTPUT_PREFIX "non-numeric in list (%s)",
                                    aWalk->a_w.w_sym->s_name)
                        okSoFar = false;
                        break;

                    default:
                        LOG_ERROR_1(xx, OUTPUT_PREFIX "unknown value in list")
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
        {
            xx->fSampleCount = count;
        }
    }
    if (! okSoFar)
    {
        outlet_bang(xx->fErrorBangOut);
    }
    return okSoFar;
} // collectSamples

/*------------------------------------ calculateDistances ---*/
static void calculateDistances(MtcTrackData * xx)
{
    MtcSampleData * sWalk = xx->fSamples;

    for (short ii = 0; ii < xx->fSampleCount; ++ii, ++sWalk)
    {
        double *          actPtr = sWalk->fActDistance;
        double            thisX = sWalk->fThisX;
        double            thisY = sWalk->fThisY;
        MtcRetainedData * rWalk = xx->fRetainedData;

        for (short jj = 0; jj < xx->fRetainedCount; ++jj, ++rWalk, ++actPtr)
        {
            if (rWalk->fValid)
            {
                *actPtr = ((rWalk->fLastX - thisX) * (rWalk->fLastX - thisX)) +
                          ((rWalk->fLastY - thisY) * (rWalk->fLastY - thisY));
            }
            else
            {
                *actPtr = FLT_MAX;
            }
        }
    }
} // calculateDistances

/*------------------------------------ assignSamples ---*/
static void assignSamples(MtcTrackData * xx)
{
    MtcRetainedData * rWalk = xx->fRetainedData;
    MtcSampleData *   sWalk;
    short             rest = xx->fSampleCount;
    short             oldCount = xx->fRetainedCount;
    short             newCount = 0;

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
        {
            rWalk->fValid = false;
        }
    }
    /* Remove any unmatched retained points, by moving later points earlier */
    if (newCount < oldCount)
    {
        MtcRetainedData * outRetPtr = xx->fRetainedData;

        rWalk = xx->fRetainedData;
        for (short ii = 0; ii < newCount; ++ii, ++rWalk)
        {
            if (rWalk->fValid)
            {
                if (outRetPtr != rWalk)
                {
                    *outRetPtr = *rWalk;
                }
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
} // assignSamples

/*------------------------------------ generateOutput ---*/
static void generateOutput(MtcTrackData * xx)
{
    t_atom            resultVector[8];
    short             outCount = xx->fRetainedCount;
    short             offset = 0;
    short             resultSize = (sizeof(resultVector) / sizeof(*resultVector));
    MtcRetainedData * rWalk = xx->fRetainedData;

    if (xx->fAddBatchNumber)
    {
        offset = 1;
    }
    else
    {
        --resultSize;
    }
    if (xx->fAddIndex)
    {
        ++offset;
    }
    else
    {
        --resultSize;
    }
    outlet_int(xx->fPointCountOut, TO_INT(outCount));
    for (short ii = 0; ii < outCount; ++ii, ++rWalk)
    {
        if (xx->fAddBatchNumber)
        {
            A_SETLONG(resultVector, xx->fBatchNumber);
            if (xx->fAddIndex)
            {
                A_SETLONG(resultVector + 1, TO_INT(ii));
            }
        }
        else if (xx->fAddIndex)
        {
            A_SETLONG(resultVector, TO_INT(ii));
        }
        A_SETFLOAT(resultVector + offset, TO_DBL(rWalk->fLastX));
        A_SETFLOAT(resultVector + offset + 1, TO_DBL(rWalk->fLastY));
        A_SETFLOAT(resultVector + offset + 2, TO_DBL(rWalk->fNewX));
        A_SETFLOAT(resultVector + offset + 3, TO_DBL(rWalk->fNewY));
        A_SETFLOAT(resultVector + offset + 4, TO_DBL(rWalk->fVelocity));
        A_SETFLOAT(resultVector + offset + 5, TO_DBL(rWalk->fForce));
        outlet_list(xx->fResultOut, NULL, resultSize, resultVector);
    }
    ++xx->fBatchNumber;
    outlet_bang(xx->fLinesCompleteOut);
} // generateOutput

/*------------------------------------ cmd_List ---*/
LIST_HEADER(MtcTrackData)
{
#pragma unused(message)
    if (xx)
    {
        if (collectSamples(xx, argc, argv))
        {
            MtcRetainedData * rWalk = xx->fRetainedData;

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
} // cmd_List
