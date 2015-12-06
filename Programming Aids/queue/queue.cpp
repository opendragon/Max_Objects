/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       queue.cpp                                                               */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the queue module.         */
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
/*  Created:    2001/04/24                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#define IS_MAIN /* */
#include "queue.h"
#include "reportAnything.h"
#include "reportVersion.h"

/*------------------------------------ queueCreate ---*/
static void * queueCreate(const long depth)
{
    QueueData * xx = static_cast<QueueData *>(object_alloc(gClass));
    
    if (xx)
    {
        if (0 <= depth)
        {
            xx->fVerbose = false;
            xx->fMaxDepth = depth;
            xx->fErrorBangOut = static_cast<t_outlet *>(bangout(xx));
            xx->fDepth = 0;
            xx->fDepthOut = static_cast<t_outlet *>(intout(xx));
            xx->fResultOut = static_cast<t_outlet *>(outlet_new(xx, NULL));
            xx->fFirstInQueue = xx->fLastInQueue = NULL;
            if (! (xx->fResultOut && xx->fDepthOut && xx->fErrorBangOut))
            {
                LOG_ERROR_1(xx, OUTPUT_PREFIX "unable to create port for object")
                freeobject(reinterpret_cast<t_object *>(xx));
                xx = NULL;
            }
        }
        else
        {
            LOG_ERROR_2(xx, OUTPUT_PREFIX "bad queue depth (%ld)", depth)
            freeobject(reinterpret_cast<t_object *>(xx));
            xx = NULL;
        }
    }
    return xx;
} // queueCreate

/*------------------------------------ queueFree ---*/
static void queueFree(QueueData * xx)
{
    if (xx)
    {
        queueClear(xx);
    }
} // queueFree

/*------------------------------------ main ---*/
int C74_EXPORT main(void)
{
    /* Allocate class memory and set up class. */
    t_class * temp = class_new(OUR_NAME, reinterpret_cast<method>(queueCreate),
                               reinterpret_cast<method>(queueFree),
                               static_cast<long>(sizeof(QueueData)), reinterpret_cast<method>(NULL),
                               A_LONG, 0);

    if (temp)
    {
        class_addmethod(temp, reinterpret_cast<method>(cmd_Add), "add", A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Anything), MESSAGE_ANYTHING, A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Assist), MESSAGE_ASSIST, A_CANT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Bang), MESSAGE_BANG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Clear), "clear", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Depth), "depth", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Fetch), "fetch", 0);
        class_addmethod(temp, reinterpret_cast<method>(stdinletinfo), MESSAGE_INLETINFO, A_CANT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Pull), "pull", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_SetDepth), "setdepth", A_LONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Trace), "trace", A_DEFSYM, 0);
        class_register(CLASS_BOX, temp);
    }
    gClass = temp;
    gEmptySymbol = gensym("");
    gOffSymbol = gensym("off");
    gOnSymbol = gensym("on");
    reportVersion();
    return 0;
} // main

StandardAnythingRoutine(QueueData)
