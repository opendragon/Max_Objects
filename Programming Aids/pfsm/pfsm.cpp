/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       pfsm.cpp                                                                */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the Probabilistic Finite  */
/*              State Machine module.                                                   */
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
/*  Created:    2000/05/30                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#define IS_MAIN /* */
#include "pfsm.h"
#include "reportAnything.h"
#include "reportVersion.h"

/*------------------------------------ pfsmCreate ---*/
static void * pfsmCreate(t_symbol * initialFile)
{
    PfsmData * xx = static_cast<PfsmData *>(object_alloc(gClass));
    
    if (xx)
    {
        xx->fAutoRestart = xx->fVerbose = xx->fRunning = false;
        xx->fCurrentState = xx->fStartState = NULL;
        xx->fStateSymbols = NULL;
        xx->fErrorBangOut = static_cast<t_outlet *>(bangout(xx));
        xx->fStopBangOut = static_cast<t_outlet *>(bangout(xx));
        xx->fResultOut = static_cast<t_outlet *>(outlet_new(xx, NULL));
        xx->fBuffer = static_cast<t_binbuf *>(binbuf_new());
        if (! (xx->fResultOut && xx->fErrorBangOut && xx->fStopBangOut && xx->fBuffer))
        {
            LOG_ERROR_1(xx, OUTPUT_PREFIX "unable to create port for object")
            freeobject(reinterpret_cast<t_object *>(xx));
            xx = NULL;
        }
        else if (initialFile == gEmptySymbol)
        {
            LOG_POST_1(xx, OUTPUT_PREFIX "no initial state file")
        }
        else
        {
            LOG_POST_2(xx, OUTPUT_PREFIX "initial file: %s", initialFile->s_name)
            pfsmLoadTables(xx, initialFile);
        }
    }
    return xx;
} // pfsmCreate

/*------------------------------------ pfsmFree ---*/
static void pfsmFree(PfsmData * xx)
{
    if (xx)
    {
        if (xx->fBuffer)
        {
            freeobject(reinterpret_cast<t_object *>(xx->fBuffer));
            xx->fBuffer = NULL;
        }
        pfsmClearHashTable(xx);
    }
} // pfsmFree

/*------------------------------------ main ---*/
int C74_EXPORT main(void)
{
    /* Allocate class memory and set up class. */
    t_class * temp = class_new(OUR_NAME, reinterpret_cast<method>(pfsmCreate),
                               reinterpret_cast<method>(pfsmFree),
                               static_cast<long>(sizeof(PfsmData)), reinterpret_cast<method>(NULL),
                               A_DEFSYM, 0);

    if (temp)
    {
        class_addmethod(temp, reinterpret_cast<method>(cmd_Anything), MESSAGE_ANYTHING, A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Assist), MESSAGE_ASSIST, A_CANT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Autorestart), "autorestart", A_DEFSYM,
                        0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Clear), "clear", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Describe), "describe", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Do), "do", A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Goto), "goto", A_SYM, 0);
        class_addmethod(temp, reinterpret_cast<method>(stdinletinfo), MESSAGE_INLETINFO, A_CANT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Int), MESSAGE_INT, A_LONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_List), MESSAGE_LIST, A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Load), "load", A_SYM, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Start), "start", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Status), "status", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Stop), "stop", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Trace), "trace", A_DEFSYM, 0);
        class_register(CLASS_BOX, temp);
    }
    gClass = temp;
    gAnyNumberSymbol = gensym("@n");
    gAnySymbolSymbol = gensym("@s");
    gCloseSquareSymbol = gensym("]");
    gColonSymbol = gensym(":");
    gDollarStarSymbol = gensym("$*");
    gDollarSymbol = gensym("$");
    gDoubleDollarSymbol = gensym("$$");
    gEmptySymbol = gensym("");
    gEqualSymbol = gensym("=");
    gErrorSymbol = gensym("error");
    gOffSymbol = gensym("off");
    gOnSymbol = gensym("on");
    gOpenSquareSymbol = gensym("[");
    gRandomMoveSymbol = gensym("-?");
    gRangeSymbol = gensym("@r");
    gTransitionSymbol = gensym("->");
    reportVersion();
    return 0;
} // main

StandardAnythingRoutine(PfsmData)
