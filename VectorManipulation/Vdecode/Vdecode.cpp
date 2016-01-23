/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       Vdecode.cpp                                                             */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the Vdecode module.       */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2003 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2003/06/15                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#define IS_MAIN /* */
#include "Vdecode.h"
#include "reportAnything.h"
#include "reportVersion.h"

/*------------------------------------ VdecodeCreate ---*/
static void *
VdecodeCreate(const long whichBase1,
              const long whichBase2,
              const long whichBase3,
              const long whichBase4,
              const long whichBase5)
{
    VdecodeData * xx = static_cast<VdecodeData *>(object_alloc(gClass));
    
    if (xx)
    {
        if (checkBases(&xx->fInfo, whichBase1, whichBase2, whichBase3, whichBase4, whichBase5))
        {
            xx->fPreviousLong = 0;
            xx->fResultOut = static_cast<t_outlet *>(outlet_new(xx, NULL));
            if (! xx->fResultOut)
            {
                LOG_ERROR_1(xx, OUTPUT_PREFIX "unable to create port for object")
                freeobject(reinterpret_cast<t_object *>(xx));
                xx = NULL;
            }
        }
        else
        {
            LOG_ERROR_1(xx, OUTPUT_PREFIX "bad base list")
            freeobject(reinterpret_cast<t_object *>(xx));
            xx = NULL;
        }
    }
    return xx;
} // VdecodeCreate

/*------------------------------------ VdecodeFree ---*/
static void
VdecodeFree(VdecodeData * xx)
{
#pragma unused(xx)
} // VdecodeFree

/*------------------------------------ main ---*/
int C74_EXPORT
main(void)
{
    /* Allocate class memory and set up class. */
    t_class * temp = class_new(OUR_NAME, reinterpret_cast<method>(VdecodeCreate),
                               reinterpret_cast<method>(VdecodeFree),
                               static_cast<long>(sizeof(VdecodeData)),
                               reinterpret_cast<method>(NULL), A_LONG, A_DEFLONG, A_DEFLONG,
                               A_DEFLONG, A_DEFLONG, 0);

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
    reportVersion();
    return 0;
} /* main */

StandardAnythingRoutine(VdecodeData)
