/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       changes.cpp                                                             */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the changes module.       */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 1998 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    1998/10/01                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#define IS_MAIN /* */
#include "changes.h"
#include "reportVersion.h"

/*------------------------------------ changesCreate ---*/
static void * changesCreate(const long whichIndex1,
                            const long whichIndex2,
                            const long whichIndex3,
                            const long whichIndex4,
                            const long whichIndex5)
{
    ChangesData * xx = static_cast<ChangesData *>(object_alloc(gClass));
    
    if (xx)
    {
        xx->fLastIndex = -1;
        xx->fWhichIndex[0] = whichIndex1;
        xx->fPreviousValue[0].a_type = A_NOTHING;
        if (whichIndex1)
        {
            xx->fLastIndex = 0;
        }
        xx->fWhichIndex[1] = whichIndex2;
        xx->fPreviousValue[1].a_type = A_NOTHING;
        if (whichIndex2)
        {
            xx->fLastIndex = 1;
        }
        xx->fWhichIndex[2] = whichIndex3;
        xx->fPreviousValue[2].a_type = A_NOTHING;
        if (whichIndex3)
        {
            xx->fLastIndex = 2;
        }
        xx->fWhichIndex[3] = whichIndex4;
        xx->fPreviousValue[3].a_type = A_NOTHING;
        if (whichIndex4)
        {
            xx->fLastIndex = 3;
        }
        xx->fWhichIndex[4] = whichIndex5;
        xx->fPreviousValue[4].a_type = A_NOTHING;
        if (whichIndex5)
        {
            xx->fLastIndex = 4;
        }
        xx->fResultOut = static_cast<t_outlet *>(outlet_new(xx, NULL));
        if (! xx->fResultOut)
        {
            LOG_ERROR_1(xx, OUTPUT_PREFIX "unable to create port for object")
            freeobject(reinterpret_cast<t_object *>(xx));
            xx = NULL;
        }
    }
    return xx;
} // changesCreate

/*------------------------------------ changesFree ---*/
static void changesFree(ChangesData * xx)
{
} // changesFree

/*------------------------------------ main ---*/
int main(void)
{
    /* Allocate class memory and set up class. */
    t_class * temp = class_new(OUR_NAME, reinterpret_cast<method>(changesCreate),
                               reinterpret_cast<method>(changesFree), sizeof(ChangesData),
                               reinterpret_cast<method>(0L), A_LONG, A_DEFLONG, A_DEFLONG,
                               A_DEFLONG, A_DEFLONG, 0);

    if (temp)
    {
        class_addmethod(temp, reinterpret_cast<method>(cmd_Anything), MESSAGE_ANYTHING, A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Assist), MESSAGE_ASSIST, A_CANT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_List), MESSAGE_LIST, A_GIMME, 0);
        class_register(CLASS_BOX, temp);
    }
    gClass = temp;
    reportVersion(OUR_NAME);
    return 0;
} // main
