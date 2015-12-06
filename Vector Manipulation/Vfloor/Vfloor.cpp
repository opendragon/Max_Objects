/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       Vfloor.cpp                                                              */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the Vfloor module.        */
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
/*  Created:    2000/11/13                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#define IS_MAIN /* */
#include "Vfloor.h"
#include "reportAnything.h"
#include "reportVersion.h"

/*------------------------------------ VfloorCreate ---*/
static void * VfloorCreate(t_symbol * mode)
{
    VObjectData * xx = static_cast<VObjectData *>(object_alloc(gClass));
    
    if (xx)
    {
        xx->fResultOut = static_cast<t_outlet *>(outlet_new(xx, NULL));
        xx->fPreviousKind = A_NOTHING;
        xx->fPreviousList = NULL;
        xx->fPreviousLength = 0;
        if (xx->fResultOut)
        {
            if (mode == gEmptySymbol)
            {
                xx->fOutputMode = A_GIMME;
            }
            else if (mode == gFSymbol)
            {
                xx->fOutputMode = A_FLOAT;
            }
            else if (mode == gISymbol)
            {
                xx->fOutputMode = A_LONG;
            }
            else if (mode == gMSymbol)
            {
                xx->fOutputMode = A_GIMME;
            }
            else
            {
                LOG_ERROR_2(xx, OUTPUT_PREFIX "unknown mode '%s'", mode->s_name)
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
    return xx;
} // VfloorCreate

/*------------------------------------ VfloorFree ---*/
static void VfloorFree(VObjectData * xx)
{
    if (xx)
    {
        clearPrevious(xx);
    }
} // VfloorFree

/*------------------------------------ main ---*/
int C74_EXPORT main(void)
{
    /* Allocate class memory and set up class. */
    t_class * temp = class_new(OUR_NAME, reinterpret_cast<method>(VfloorCreate),
                               reinterpret_cast<method>(VfloorFree),
                               static_cast<long>(sizeof(VObjectData)),
                               reinterpret_cast<method>(NULL), A_DEFSYM, 0);

    if (temp)
    {
        class_addmethod(temp, reinterpret_cast<method>(cmd_Anything), MESSAGE_ANYTHING, A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Assist), MESSAGE_ASSIST, A_CANT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Bang), MESSAGE_BANG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Float), MESSAGE_FLOAT, A_FLOAT, 0);
        class_addmethod(temp, reinterpret_cast<method>(stdinletinfo), MESSAGE_INLETINFO, A_CANT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Int), MESSAGE_INT, A_LONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_List), MESSAGE_LIST, A_GIMME, 0);
        class_register(CLASS_BOX, temp);
    }
    gClass = temp;
    gEmptySymbol = gensym("");
    gFSymbol = gensym("f");
    gISymbol = gensym("i");
    gMSymbol = gensym("m");
    reportVersion();
    return 0;
} // main

/*------------------------------------ clearPrevious ---*/
void clearPrevious(VObjectData * xx)
{
    FREE_BYTES(xx->fPreviousList);
} // clearPrevious

StandardAnythingRoutine(VObjectData)
