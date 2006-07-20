/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       changes.c                                                               */
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
#include "displayAboutBox.h"
#include "loadAboutBoxData.h"
#include "loadOtherSegments.h"
#include "reportVersion.h"

/* Forward references: */
Pvoid changesCreate
  (long whichIndex1,
   long whichIndex2,
   long whichIndex3,
   long whichIndex4,
   long whichIndex5);

Pvoid changesFree
  (ChangesPtr xx);

/*------------------------------------ main ---*/
void main
  (Pfptr ff)
{
  EnterCodeResource();
  PrepareCallback();
  FNS = ff;   /* Connect up the function macros. */
  /* Allocate class memory and set up class. */
  setup(reinterpret_cast<t_messlist**>(&gClass), reinterpret_cast<method>(changesCreate),
  reinterpret_cast<method>(changesFree), short(sizeof(ChangesData)),
  reinterpret_cast<method>(0L), A_LONG, A_DEFLONG, A_DEFLONG,
  A_DEFLONG, A_DEFLONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Anything), "anything", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Assist), "assist", A_CANT, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Info), "info", A_CANT, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_List), "list", A_GIMME, A_NOTHING);
  /* Add object class to New Object List */
  finder_addclass("Lists", OUR_NAME);
  /* Make sure that our string list resource is present */
  rescopy('STR#', OUR_RES_NUMB);
  loadAboutBoxData();
  gVersionNumber = reportVersion(OUR_NAME);
  loadOtherSegments();
  ExitCodeResource();
} /* main */

/*------------------------------------ changesCreate ---*/
Pvoid changesCreate
  (long whichIndex1,
   long whichIndex2,
   long whichIndex3,
   long whichIndex4,
   long whichIndex5)
{
  ChangesPtr xx;

  EnterCallback();
  LOG_ENTER()
  xx = static_cast<ChangesPtr>(newobject(gClass));
  if (xx)
  {
    xx->fLastIndex = -1;
    xx->fWhichIndex[0] = short(whichIndex1);
    xx->fPreviousValue[0].a_type = A_NOTHING;
    if (whichIndex1)
      xx->fLastIndex = 0;
    xx->fWhichIndex[1] = short(whichIndex2);
    xx->fPreviousValue[1].a_type = A_NOTHING;
    if (whichIndex2)
      xx->fLastIndex = 1;
    xx->fWhichIndex[2] = short(whichIndex3);
    xx->fPreviousValue[2].a_type = A_NOTHING;
    if (whichIndex3)
      xx->fLastIndex = 2;
    xx->fWhichIndex[3] = short(whichIndex4);
    xx->fPreviousValue[3].a_type = A_NOTHING;
    if (whichIndex4)
      xx->fLastIndex = 3;
    xx->fWhichIndex[4] = short(whichIndex5);
    xx->fPreviousValue[4].a_type = A_NOTHING;
    if (whichIndex5)
      xx->fLastIndex = 4;
    xx->fResultOut = static_cast<POutlet>(outlet_new(xx, NULL_PTR));
    if (! xx->fResultOut)
    {
      LOG_ERROR_1(OUTPUT_PREFIX "unable to create port for object")
      freeobject(reinterpret_cast<PObject>(xx));
      xx = NULL_PTR;
    }
  }
  ExitCodeResource();
  return xx;
} /* changesCreate */

/*------------------------------------ changesFree ---*/
Pvoid changesFree
  (ChangesPtr xx)
{
#pragma unused(xx)
  EnterCallback();
  LOG_EXIT()
  ExitMaxMessageHandler()
} /* changesFree */

StandardInfoRoutine(ChangesPtr)