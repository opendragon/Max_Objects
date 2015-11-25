/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       stack.cpp                                                               */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the stack module.         */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2000 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2000/06/22                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#define IS_MAIN /* */
#include "stack.h"
#include "reportAnything.h"
#include "reportVersion.h"

/*------------------------------------ stackResolveCommonName ---*/
static void stackResolveCommonName(StackData * xx,
                                   t_symbol *  name)
{
    StackDescriptor * descriptor = NULL;
    
    if (name != gEmptySymbol)
    {
        for (descriptor = gStackAnchor; descriptor; descriptor = descriptor->fNext)
        {
            if (descriptor->fTag == name)
            {
                break;
            }
            
        }
    }
    if (descriptor)
    {
        ++descriptor->fReferenceCount;
        xx->fStack = descriptor;
    }
    else
    {
        descriptor = GET_BYTES(1, StackDescriptor);
        if (descriptor)
        {
            descriptor->fTopOfStack = NULL;
            descriptor->fDepth = 0;
            descriptor->fReferenceCount = 1;
            xx->fStack = descriptor;
            descriptor->fPrevious = NULL;
            if (name == gEmptySymbol)
            {
                descriptor->fTag = NULL;
                descriptor->fNext = NULL;
            }
            else
            {
                descriptor->fTag = name;
                descriptor->fNext = gStackAnchor;
                if (gStackAnchor)
                {
                    gStackAnchor->fPrevious = descriptor;
                }
                gStackAnchor = descriptor;
            }
        }
    }
} // stackResolveCommonName

/*------------------------------------ stackCreate ---*/
static void * stackCreate(t_symbol * commonName)
{
    StackData * xx = static_cast<StackData *>(object_alloc(gClass));
    
    if (xx)
    {
        xx->fVerbose = false;
        xx->fErrorBangOut = static_cast<t_outlet *>(bangout(xx));
        xx->fDepthOut = static_cast<t_outlet *>(intout(xx));
        xx->fResultOut = static_cast<t_outlet *>(outlet_new(xx, NULL));
        xx->fStack = NULL;
        stackResolveCommonName(xx, commonName);
        if (! (xx->fResultOut && xx->fErrorBangOut && xx->fDepthOut && xx->fStack))
        {
            LOG_ERROR_1(xx, OUTPUT_PREFIX "unable to create port for object")
            freeobject(reinterpret_cast<t_object *>(xx));
            xx = NULL;
        }
    }
    return xx;
} // stackCreate

/*------------------------------------ stackFree ---*/
static void stackFree(StackData * xx)
{
    if (xx)
    {
        StackDescriptor * descriptor = xx->fStack;
        
        if (descriptor)
        {
            if (! --descriptor->fReferenceCount)
            {
                if (descriptor->fNext)
                {
                    descriptor->fNext->fPrevious = descriptor->fPrevious;
                }
                if (descriptor->fPrevious)
                {
                    descriptor->fPrevious->fNext = descriptor->fNext;
                }
                if (gStackAnchor == descriptor)
                {
                    gStackAnchor = descriptor->fNext;
                }
                stackClear(xx);
                FREE_BYTES(xx->fStack);
            }
        }
    }
} // stackFree

/*------------------------------------ main ---*/
int main(void)
{
    /* Allocate class memory and set up class. */
    t_class * temp = class_new(OUR_NAME, reinterpret_cast<method>(stackCreate),
                               reinterpret_cast<method>(stackFree), sizeof(StackData),
                               reinterpret_cast<method>(0L), A_DEFSYM, 0);

    if (temp)
    {
        class_addmethod(temp, reinterpret_cast<method>(cmd_Anything), MESSAGE_ANYTHING, A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Assist), MESSAGE_ASSIST, A_CANT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Clear), "clear", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Depth), "depth", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Dup), "dup", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Pop), "pop", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Push), "push", A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Swap), "swap", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Top), "top", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_TopPop), "top+pop", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Trace), "trace", A_DEFSYM, 0);
        class_register(CLASS_BOX, temp);
    }
    gClass = temp;
    gEmptySymbol = gensym("");
    gOffSymbol = gensym("off");
    gOnSymbol = gensym("on");
    gStackAnchor = NULL;
    reportVersion(OUR_NAME);
    return 0;
} // main

StandardAnythingRoutine(StackData)
