/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       Vmean.cpp                                                               */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the Vmean module.         */
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
/*  Created:    2001/04/06                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#define IS_MAIN /* */
#include "Vmean.h"
#include "reportAnything.h"
#include "reportVersion.h"

/*------------------------------------ identifySymbol ---*/
static OpCode identifySymbol(t_symbol * name)
{
    OpCode result = OP_unknown;
    
    if ((name == gASymbol) || (name == gArithSymbol))
    {
        result = OP_Arithmetic;
    }
    else if ((name == gGSymbol) || (name == gGeomSymbol))
    {
        result = OP_Geometric;
    }
    else if ((name == gHSymbol) || (name == gHarmSymbol))
    {
        result = OP_Harmonic;
    }
    return result;
} // identifySymbol

/*------------------------------------ VmeanCreate ---*/
static void * VmeanCreate(t_symbol * operation)
{
    VmeanData * xx = static_cast<VmeanData *>(object_alloc(gClass));
    
    if (xx)
    {
        xx->fOperation = identifySymbol(operation);
        if (OP_unknown == xx->fOperation)
        {
            LOG_ERROR_1(xx, OUTPUT_PREFIX "unknown operation")
            freeobject(reinterpret_cast<t_object *>(xx));
            xx = NULL;
        }
        else
        {
            xx->fPreviousFloat = 0;
            xx->fResultOut = static_cast<t_outlet *>(floatout(xx));
            if (! xx->fResultOut)
            {
                LOG_ERROR_1(xx, OUTPUT_PREFIX "unable to create port for object")
                freeobject(reinterpret_cast<t_object *>(xx));
                xx = NULL;
            }
        }
    }
    return xx;
} // VmeanCreate

/*------------------------------------ VmeanFree ---*/
static void VmeanFree(VmeanData * xx)
{
#pragma unused(xx)
} // VmeanFree

/*------------------------------------ main ---*/
int C74_EXPORT main(void)
{
    /* Allocate class memory and set up class. */
    t_class * temp = class_new(OUR_NAME, reinterpret_cast<method>(VmeanCreate),
                               reinterpret_cast<method>(VmeanFree),
                               static_cast<long>(sizeof(VmeanData)), reinterpret_cast<method>(NULL),
                               A_SYM, 0);

    if (temp)
    {
        class_addmethod(temp, reinterpret_cast<method>(cmd_Anything), MESSAGE_ANYTHING, A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Assist), MESSAGE_ASSIST, A_CANT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Bang), MESSAGE_BANG, 0);
        class_addmethod(temp, reinterpret_cast<method>(stdinletinfo), MESSAGE_INLETINFO, A_CANT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_List), MESSAGE_LIST, A_GIMME, 0);
        class_register(CLASS_BOX, temp);
    }
    gClass = temp;
    gASymbol = gensym("a");
    gArithSymbol = gensym("arith");
    gGSymbol = gensym("g");
    gGeomSymbol = gensym("geom");
    gHSymbol = gensym("h");
    gHarmSymbol = gensym("harm");
    reportVersion();
    return 0;
} /* main */

StandardAnythingRoutine(VmeanData)
