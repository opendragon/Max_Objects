/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       memory.cpp                                                              */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the memory module.        */
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
/*  Created:    2000/06/21                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#define IS_MAIN /* */
#include "memory.h"
#include "reportAnything.h"
#include "reportVersion.h"

/* Forward references: */
void * memoryCreate(t_symbol * commonName);

void memoryFree(MemoryData * xx);

static void memoryResolveCommonName(MemoryData * xx,
                                    t_symbol *   name);

/*------------------------------------ main ---*/
int main(void)
{
    /* Allocate class memory and set up class. */
    t_class * temp = class_new(OUR_NAME, reinterpret_cast<method>(memoryCreate), reinterpret_cast<method>(memoryFree),
                               sizeof(MemoryData), reinterpret_cast<method>(0L), A_DEFSYM, 0);

    if (temp)
    {
        class_addmethod(temp, reinterpret_cast<method>(cmd_Anything), MESSAGE_ANYTHING, A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Assist), MESSAGE_ASSIST, A_CANT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Clear), "clear", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Forget), "forget", A_SYM, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Load), "load", A_SYM, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Recall), "recall", A_SYM, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Remember), "remember", A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Store), "store", A_SYM, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Trace), "trace", A_DEFSYM, 0);
        class_register(CLASS_BOX, temp);
    }
    gClass = temp;
    gEmptySymbol = gensym("");
    gOffSymbol = gensym("off");
    gOnSymbol = gensym("on");
    gMemoryAnchor = NULL_PTR;
    reportVersion(OUR_NAME);
    return 0;
} // main
/*------------------------------------ memoryResolveCommonName ---*/
static void memoryResolveCommonName(MemoryData * xx,
                                    t_symbol *   name)
{
    MemoryDescriptor * descriptor = NULL_PTR;

    if (name != gEmptySymbol)
    {
        for (descriptor = gMemoryAnchor; descriptor; descriptor = descriptor->fNext)
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
        xx->fSymbols = descriptor;
    }
    else
    {
        descriptor = GETBYTES(1, MemoryDescriptor);
        if (descriptor)
        {
            descriptor->fSymbolTable = NULL_HDL;
            descriptor->fReferenceCount = 1;
            xx->fSymbols = descriptor;
            descriptor->fPrevious = NULL_PTR;
            if (name == gEmptySymbol)
            {
                descriptor->fTag = NULL_PTR;
                descriptor->fNext = NULL_PTR;
            }
            else
            {
                descriptor->fTag = name;
                descriptor->fNext = gMemoryAnchor;
                if (gMemoryAnchor)
                {
                    gMemoryAnchor->fPrevious = descriptor;
                }
                gMemoryAnchor = descriptor;
            }
            memoryInitializeHashTable(xx);
        }
    }
} // memoryResolveCommonName
/*------------------------------------ memoryCreate ---*/
void * memoryCreate(t_symbol * commonName)
{
    MemoryData * xx = static_cast<MemoryData *>(object_alloc(gClass));

    if (xx)
    {
        xx->fVerbose = false;
        xx->fErrorBangOut = static_cast<t_outlet *>(bangout(xx));
        xx->fResultOut = static_cast<t_outlet *>(outlet_new(xx, NULL_PTR));
        xx->fSymbols = NULL_PTR;
        xx->fSymbolCount = 0;
        memoryResolveCommonName(xx, commonName);
        if (! (xx->fResultOut || xx->fErrorBangOut || xx->fSymbols))
        {
            LOG_ERROR_1(xx, OUTPUT_PREFIX "unable to create port for object")
            freeobject(reinterpret_cast<t_object *>(xx));
            xx = NULL_PTR;
        }
    }
    return xx;
} // memoryCreate
/*------------------------------------ memoryFree ---*/
void memoryFree(MemoryData * xx)
{
    if (xx)
    {
        MemoryDescriptor * descriptor = xx->fSymbols;

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
                if (gMemoryAnchor == descriptor)
                {
                    gMemoryAnchor = descriptor->fNext;
                }
                memoryClearHashTable(xx);
                FREEBYTES(descriptor, 1);
                xx->fSymbols = NULL_PTR;
            }
        }
    }
} // memoryFree

StandardAnythingRoutine(MemoryData *)
