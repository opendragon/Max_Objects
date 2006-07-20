/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       Vdecode.c                                                               */
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
#include "displayAboutBox.h"
#include "loadAboutBoxData.h"
#include "loadOtherSegments.h"
#include "reportAnything.h"
#include "reportVersion.h"

/* Forward references: */
Pvoid VdecodeCreate
  (long whichBase1,
   long whichBase2,
   long whichBase3,
   long whichBase4,
   long whichBase5);

Pvoid VdecodeFree
  (VdecodePtr xx);

/*------------------------------------ main ---*/
void main
  (Pfptr ff)
{
  EnterCodeResource();
  PrepareCallback();
  FNS = ff;   /* Connect up the function macros. */
  /* Allocate class memory and set up class. */
  setup(reinterpret_cast<t_messlist**>(&gClass), reinterpret_cast<method>(VdecodeCreate),
        reinterpret_cast<method>(VdecodeFree), short(sizeof(VdecodeData)),
        reinterpret_cast<method>(0L), A_LONG, A_DEFLONG, A_DEFLONG, A_DEFLONG, A_DEFLONG,
        A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Anything), "anything", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Assist), "assist", A_CANT, A_NOTHING);
  addbang(reinterpret_cast<method>(cmd_Bang));
  addfloat(reinterpret_cast<method>(cmd_Float));
  addmess(reinterpret_cast<method>(cmd_Info), "info", A_CANT, A_NOTHING);
  addint(reinterpret_cast<method>(cmd_Int));
  addmess(reinterpret_cast<method>(cmd_List), "list", A_GIMME, A_NOTHING);
  /* Add object class to New Object List */
  finder_addclass("Arith/Logic/Bitwise", OUR_NAME);
  finder_addclass("Lists", OUR_NAME);
  /* Make sure that our string list resource is present */
  rescopy('STR#', OUR_RES_NUMB);
  loadAboutBoxData();
  gVersionNumber = reportVersion(OUR_NAME);
  loadOtherSegments();
  ExitCodeResource();
} /* main */

/*------------------------------------ VdecodeCreate ---*/
Pvoid VdecodeCreate
  (long whichBase1,
   long whichBase2,
   long whichBase3,
   long whichBase4,
   long whichBase5)
{
  VdecodePtr xx;

  EnterCallback();
  LOG_ENTER()
  xx = static_cast<VdecodePtr>(newobject(gClass));
  if (xx)
  {
    if (CheckBases(&xx->fInfo, whichBase1, whichBase2, whichBase3, whichBase4,
                  whichBase5))
    {
      xx->fPreviousLong = 0;
      xx->fResultOut = static_cast<POutlet>(outlet_new(xx, NULL_PTR));
      if (! xx->fResultOut)
      {
        LOG_ERROR_1(OUTPUT_PREFIX "unable to create port for object")
        freeobject(reinterpret_cast<PObject>(xx));
        xx = NULL_PTR;
      }
    }
    else
    {
      LOG_ERROR_1(OUTPUT_PREFIX "bad base list")
      freeobject(reinterpret_cast<PObject>(xx));
      xx = NULL_PTR;
    }
  }
  ExitCodeResource();
  return xx;
} /* VdecodeCreate */

/*------------------------------------ VdecodeFree ---*/
Pvoid VdecodeFree
  (VdecodePtr xx)
{
#pragma unused(xx)
  EnterCallback();
  LOG_EXIT()
  ExitMaxMessageHandler()
} /* VdecodeFree */

StandardAnythingRoutine(VdecodePtr)
StandardInfoRoutine(VdecodePtr)