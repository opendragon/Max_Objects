/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       Vcollect.cpp                                                            */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the Vcollect module.      */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2002 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2002/08/27                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#define IS_MAIN /* */
#include "Vcollect.h"
#include "reportVersion.h"

/* Forward references: */
void * VcollectCreate(void);

void VcollectFree(VcollectData * xx);

/*------------------------------------ main ---*/
int main(void)
{
    /* Allocate class memory and set up class. */
    t_class * temp = class_new(OUR_NAME, reinterpret_cast<method>(VcollectCreate),
                               reinterpret_cast<method>(VcollectFree), sizeof(VcollectData),
                               reinterpret_cast<method>(0L), 0);

    if (temp)
    {
        class_addmethod(temp, reinterpret_cast<method>(cmd_Anything), MESSAGE_ANYTHING, A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Assist), MESSAGE_ASSIST, A_CANT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Bang), MESSAGE_BANG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Clear), "clear", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Count), "count", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Float), MESSAGE_FLOAT, A_FLOAT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Int), MESSAGE_INT, A_LONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_List), MESSAGE_LIST, A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Start), "start", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Stop), "stop", 0);
        class_register(CLASS_BOX, temp);
    }
    gClass = temp;
    gClearSymbol = gensym("clear");
    gCountSymbol = gensym("count");
    gStartSymbol = gensym("start");
    gStopSymbol = gensym("stop");
    reportVersion(OUR_NAME);
    return 0;
} // main
/*------------------------------------ VcollectCreate ---*/
void * VcollectCreate(void)
{
    VcollectData * xx = static_cast<VcollectData *>(object_alloc(gClass));

    if (xx)
    {
        xx->fLengthOut = static_cast<t_outlet *>(intout(xx));
        xx->fResultOut = static_cast<t_outlet *>(outlet_new(xx, NULL_PTR));
        xx->fProxy = proxy_new(xx, 1L, &xx->fInletNumber);
        xx->fFirstChunk = xx->fFreeChunks = xx->fLastChunk = NULL_PTR;
        xx->fAtomsInLastChunk = xx->fTotalAtoms = 0;
        xx->fCollecting = true;
        if (! (xx->fLengthOut && xx->fResultOut && xx->fProxy))
        {
            LOG_ERROR_1(xx, OUTPUT_PREFIX "unable to create port for object")
            freeobject(reinterpret_cast<t_object *>(xx));
            xx = NULL_PTR;
        }
    }
    return xx;
} // VcollectCreate
/*------------------------------------ VcollectFree ---*/
void VcollectFree(VcollectData * xx)
{
    if (xx)
    {
        if (xx->fProxy)
        {
            freeobject(reinterpret_cast<t_object *>(xx->fProxy));
            xx->fProxy = NULL_PTR;
        }
        /* Now return any chunks to the free list: */
        if (xx->fTotalAtoms > 0)
        {
            xx->fLastChunk->fNext = xx->fFreeChunks;
            xx->fFreeChunks = xx->fFirstChunk;
            xx->fFirstChunk = xx->fLastChunk = NULL_PTR;
            xx->fTotalAtoms = xx->fAtomsInLastChunk = 0;
        }
        /* And, finally, return the free list to the system: */
        for ( ; xx->fFreeChunks; )
        {
            CollectChunk * next = xx->fFreeChunks->fNext;

            FREEBYTES(xx->fFreeChunks, sizeof(CollectChunk));
            xx->fFreeChunks = next;
        }
    }
} // VcollectFree
/*------------------------------------ collectAddAtoms ---*/
void collectAddAtoms(VcollectData * xx,
                     t_atom *       atoms,
                     const short    count)
{
    if (xx->fCollecting && (count > 0))
    {
        short lastCount = xx->fAtomsInLastChunk;

        if (! xx->fLastChunk)
        {
            if (xx->fFreeChunks)
            {
                xx->fFirstChunk = xx->fFreeChunks;
                xx->fFreeChunks = xx->fFreeChunks->fNext;
            }
            else
            {
                xx->fFirstChunk = GETBYTES(1, CollectChunk);
            }
            xx->fLastChunk = xx->fFirstChunk;
            xx->fLastChunk->fNext = NULL_PTR;
        }
        if (xx->fLastChunk)
        {
            short          offset = 0;
            short          thisMove;
            short          workingCount = count;
            CollectChunk * outWalk = xx->fLastChunk;

            for ( ; workingCount > 0; )
            {
                if ((lastCount + workingCount) <= CHUNK_SIZE)
                {
                    /* They all fit in the current chunk */
                    memcpy(&outWalk->fAtoms[lastCount], atoms + offset, workingCount * sizeof(t_atom));
                    xx->fAtomsInLastChunk += workingCount;
                    xx->fTotalAtoms += workingCount;
                    break;
                }

                /* There will be an overflow */
                thisMove = static_cast<short>(CHUNK_SIZE - lastCount);
                memcpy(&outWalk->fAtoms[lastCount], atoms + offset, thisMove * sizeof(t_atom));
                workingCount -= thisMove;
                offset += thisMove;
                xx->fTotalAtoms += thisMove;
                /* Get another chunk */
                if (xx->fFreeChunks)
                {
                    outWalk->fNext = xx->fFreeChunks;
                    xx->fFreeChunks = xx->fFreeChunks->fNext;
                }
                else
                {
                    outWalk->fNext = GETBYTES(1, CollectChunk);
                }
                outWalk = outWalk->fNext;
                if (! outWalk)
                {
                    break;
                }

                xx->fLastChunk = outWalk;
                outWalk->fNext = NULL_PTR;
                lastCount = xx->fAtomsInLastChunk = 0;
            }
        }
    }
} // collectAddAtoms
