/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       shotgun.cpp                                                             */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the Vmean module.         */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2005 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2005/07/24                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#define IS_MAIN /* */
#include "shotgun.h"
#include "reportAnything.h"
#include "reportVersion.h"

/*------------------------------------ ShotgunCreate ---*/
static void * ShotgunCreate(const long numOutlets)
{
    ShotgunData * xx = static_cast<ShotgunData *>(object_alloc(gClass));
    
    if (xx)
    {
        xx->fNumOutlets = 0;
        xx->fResultsOut = NULL;
        if (0 > numOutlets)
        {
            LOG_ERROR_1(xx, OUTPUT_PREFIX "invalid parameter for object")
            freeobject(reinterpret_cast<t_object *>(xx));
            xx = NULL;
        }
        else
        {
            xx->fNumOutlets = (numOutlets ? numOutlets : 2);
            t_outlet ** outletWalker = GET_BYTES(xx->fNumOutlets, t_outlet *);
            
            if (outletWalker)
            {
                xx->fResultsOut = outletWalker;
                for (short ii = 0; ii < xx->fNumOutlets; ++ii, ++outletWalker)
                {
                    *outletWalker = static_cast<t_outlet *>(bangout(xx));
                    if (! *outletWalker)
                    {
                        LOG_ERROR_1(xx, OUTPUT_PREFIX "unable to allocate port for object")
                        freeobject(reinterpret_cast<t_object *>(xx));
                        xx = NULL;
                        break;
                    }
                    
                }
            }
            else
            {
                LOG_ERROR_1(xx, OUTPUT_PREFIX "unable to allocate memory for object")
                freeobject(reinterpret_cast<t_object *>(xx));
                xx = NULL;
            }
        }
    }
    return xx;
} // ShotgunCreate

/*------------------------------------ ShotgunFree ---*/
static void ShotgunFree(ShotgunData * xx)
{
    if (xx)
    {
        if (xx->fNumOutlets && xx->fResultsOut)
        {
            FREE_BYTES(xx->fResultsOut);
            xx->fResultsOut = NULL;
        }
    }
} // ShotgunFree

/*------------------------------------ main ---*/
int main(void)
{
    /* Allocate class memory and set up class. */
    t_class * temp = class_new(OUR_NAME, reinterpret_cast<method>(ShotgunCreate),
                               reinterpret_cast<method>(ShotgunFree), sizeof(ShotgunData),
                               reinterpret_cast<method>(0L), A_DEFLONG, 0);

    if (temp)
    {
        class_addmethod(temp, reinterpret_cast<method>(cmd_Anything), MESSAGE_ANYTHING, A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Assist), MESSAGE_ASSIST, A_CANT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Bang), MESSAGE_BANG, 0);
        class_register(CLASS_BOX, temp);
    }
    gClass = temp;
    reportVersion(OUR_NAME);
    return 0;
} // main

StandardAnythingRoutine(ShotgunData)
