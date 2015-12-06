/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       mtcTrack.cpp                                                            */
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
#include "reportAnything.h"
#include "reportVersion.h"

/*------------------------------------ setUpMemory ---*/
static bool setUpMemory(MtcTrackData * xx)
{
    long howMany2 = (xx->fHowMany * xx->fMaxSamples);
    bool okSoFar = false;
    
    xx->fActDistances = GET_BYTES(howMany2, double);
    xx->fRetainedData = GET_BYTES(xx->fHowMany, MtcRetainedData);
    xx->fSamples = GET_BYTES(xx->fMaxSamples, MtcSampleData);
    if (xx->fActDistances && xx->fRetainedData && xx->fSamples)
    {
        MtcRetainedData * retainedWalk = xx->fRetainedData;
        MtcSampleData *   sampleWalk = xx->fSamples;
        double *          actWalk = xx->fActDistances;
        
        for (long ii = 0; ii < xx->fHowMany; ++ii, ++retainedWalk)
        {
            retainedWalk->fLastP = retainedWalk->fLastX = retainedWalk->fLastY = 0;
            retainedWalk->fDeltaX = retainedWalk->fDeltaY = retainedWalk->fNewP = 0;
            retainedWalk->fNewX = retainedWalk->fNewY = 0;
            retainedWalk->fVelocity = retainedWalk->fForce = 0;
            retainedWalk->fValid = false;
        }
        for (long ii = 0; ii < xx->fMaxSamples; ++ii, ++sampleWalk)
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
} // setUpMemory

/*------------------------------------ MtcTrackCreate ---*/
static void * MtcTrackCreate(const long howMany,
                             t_symbol * optionOne,
                             t_symbol * optionTwo)
{
    MtcTrackData * xx = static_cast<MtcTrackData *>(object_alloc(gClass));
    
    if (xx)
    {
        xx->fHowMany = howMany;
        xx->fMaxSamples = (((howMany * 3) + 1) / 2);
        xx->fErrorBangOut = NULL;
        xx->fResultOut = NULL;
        xx->fPointCountOut = NULL;
        xx->fLinesCompleteOut = NULL;
        xx->fRetainedCount = xx->fSampleCount = 0;
        xx->fBatchNumber = 0;
        xx->fAddBatchNumber = xx->fAddIndex = false;
        xx->fThreshold = -1; /* So we don't initially use a threshold */
        xx->fActDistances = NULL;
        xx->fRetainedData = NULL;
        xx->fSamples = NULL;
        if ((1 > howMany) || (MAX_POINTS < howMany))
        {
            LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid number of points (%ld}", howMany)
            freeobject(reinterpret_cast<t_object *>(xx));
            xx = NULL;
        }
        else
        {
            bool okSoFar = true;
            
            if (optionOne == gBatchSymbol)
            {
                xx->fAddBatchNumber = true;
            }
            else if (optionOne == gIndexSymbol)
            {
                xx->fAddIndex = true;
            }
            else if (optionOne != gEmptySymbol)
            {
                LOG_ERROR_2(xx, OUTPUT_PREFIX "unrecognized option (%s)", optionOne->s_name)
                okSoFar = false;
            }
            if (okSoFar)
            {
                if (optionTwo == gBatchSymbol)
                {
                    xx->fAddBatchNumber = true;
                }
                else if (optionTwo == gIndexSymbol)
                {
                    xx->fAddIndex = true;
                }
                else if (optionTwo != gEmptySymbol)
                {
                    LOG_ERROR_2(xx, OUTPUT_PREFIX "unrecognized option (%s)", optionTwo->s_name)
                    okSoFar = false;
                }
            }
            if (okSoFar)
            {
                xx->fErrorBangOut = static_cast<t_outlet *>(bangout(xx));
                xx->fPointCountOut = static_cast<t_outlet *>(intout(xx));
                xx->fLinesCompleteOut = static_cast<t_outlet *>(bangout(xx));
                xx->fResultOut = static_cast<t_outlet *>(outlet_new(xx, NULL));
                if (xx->fResultOut && xx->fPointCountOut && xx->fLinesCompleteOut &&
                    xx->fErrorBangOut)
                {
                    if (! setUpMemory(xx))
                    {
                        freeobject(reinterpret_cast<t_object *>(xx));
                        xx = NULL;
                    }
                }
                else
                {
                    LOG_ERROR_1(xx, OUTPUT_PREFIX "unable to create port for object")
                    freeobject(reinterpret_cast<t_object *>(xx));
                    xx = NULL;
                }
            }
            else
            {
                freeobject(reinterpret_cast<t_object *>(xx));
                xx = NULL;
            }
        }
    }
    return xx;
} // MtcTrackCreate

/*------------------------------------ MtcTrackFree ---*/
static void MtcTrackFree(MtcTrackData * xx)
{
    if (xx)
    {
        FREE_BYTES(xx->fActDistances);
        FREE_BYTES(xx->fSamples);
        FREE_BYTES(xx->fRetainedData);
    }
} // MtcTrackFree

/*------------------------------------ main ---*/
int C74_EXPORT main(void)
{
    /* Allocate class memory and set up class. */
    t_class * temp = class_new(OUR_NAME, reinterpret_cast<method>(MtcTrackCreate),
                               reinterpret_cast<method>(MtcTrackFree),
                               static_cast<long>(sizeof(MtcTrackData)),
                               reinterpret_cast<method>(NULL), A_LONG, A_DEFSYM, A_DEFSYM, 0);

    if (temp)
    {
        class_addmethod(temp, reinterpret_cast<method>(cmd_Anything), MESSAGE_ANYTHING, A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Assist), MESSAGE_ASSIST, A_CANT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Batch), "batch", A_CANT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Clear), "clear", A_CANT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Index), "index", A_CANT, 0);
        class_addmethod(temp, reinterpret_cast<method>(stdinletinfo), MESSAGE_INLETINFO, A_CANT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_List), MESSAGE_LIST, A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Threshold), "threshold", A_FLOAT, 0);
        class_register(CLASS_BOX, temp);
    }
    gClass = temp;
    gBatchSymbol = gensym("batch");
    gEmptySymbol = gensym("");
    gIndexSymbol = gensym("index");
    gOffSymbol = gensym("off");
    gOnSymbol = gensym("on");
    reportVersion();
    return 0;
} // main

StandardAnythingRoutine(MtcTrackData)
