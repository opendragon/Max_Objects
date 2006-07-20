/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       Vsplit.c                                                                */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the Vdrop module.         */
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
/*  Created:    2005/07/23                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#define IS_MAIN /* */
#include "Vsplit.h"
#include "displayAboutBox.h"
#include "loadAboutBoxData.h"
#include "loadOtherSegments.h"
#include "reportVersion.h"

/* Forward references: */
Pvoid VsplitCreate
  (long howMany);

Pvoid VsplitFree
  (VObjectPtr xx);

Pvoid VsplitProcessInput
  (VObjectPtr xx,
   long       number);

/*------------------------------------ main ---*/
void main
  (Pfptr ff)
{
  EnterCodeResource();
  PrepareCallback();
  FNS = ff;   /* Connect up the function macros. */
  /* Allocate class memory and set up class. */
  setup(reinterpret_cast<t_messlist**>(&gClass), reinterpret_cast<method>(VsplitCreate),
        reinterpret_cast<method>(VsplitFree), short(sizeof(VObjectData)),
        reinterpret_cast<method>(0L), A_LONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Anything), "anything", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Assist), "assist", A_CANT, A_NOTHING);
  addbang(reinterpret_cast<method>(cmd_Bang));
  addmess(reinterpret_cast<method>(cmd_Info), "info", A_CANT, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_List), "list", A_GIMME, A_NOTHING);
  addinx(reinterpret_cast<method>(VsplitProcessInput), 1);
  /* Add object class to New Object List */
  finder_addclass("Lists", OUR_NAME);
  /* Make sure that our string list resource is present */
  rescopy('STR#', OUR_RES_NUMB);
  loadAboutBoxData();
  gVersionNumber = reportVersion(OUR_NAME);
  loadOtherSegments();
  ExitCodeResource();
} /* main */

/*------------------------------------ VsplitCreate ---*/
Pvoid VsplitCreate
  (long howMany)
{
  VObjectPtr xx;

  EnterCallback();
  LOG_ENTER()
  xx = static_cast<VObjectPtr>(newobject(gClass));
  if (xx)
  {
    xx->fHowMany = short(howMany);
    xx->fPreviousList = xx->fPreviousRightList = NULL_PTR;
    xx->fPreviousLength = xx->fPreviousRightLength = 0;
    intin(xx, 1);
    xx->fRightResultOut = static_cast<POutlet>(outlet_new(xx, NULL_PTR));
    xx->fResultOut = static_cast<POutlet>(outlet_new(xx, NULL_PTR));
    if ((! xx->fResultOut) || (! xx->fRightResultOut))
    {
      LOG_ERROR_1(OUTPUT_PREFIX "unable to create port for object")
      freeobject(reinterpret_cast<PObject>(xx));
      xx = NULL_PTR;
    }
  }
  ExitCodeResource();
  return xx;
} /* VsplitCreate */

/*------------------------------------ VsplitFree ---*/
Pvoid VsplitFree
  (VObjectPtr xx)
{
  EnterCallback();
  if (xx)
    clearPrevious(xx);
  LOG_EXIT()
  ExitMaxMessageHandler()
} /* VsplitFree */

/*------------------------------------ VsplitProcessInput ---*/
Pvoid VsplitProcessInput
  (VObjectPtr xx,
   long       number)
{
  EnterCallback();
  /* We've received a number. */
  if (xx)
    xx->fHowMany = short(number);
  ExitMaxMessageHandler()
} /* VsplitProcessInput */

/*------------------------------------ clearPrevious ---*/
void clearPrevious
  (VObjectPtr xx)
{
  FREEBYTES(xx->fPreviousList, xx->fPreviousLength)
  FREEBYTES(xx->fPreviousRightList, xx->fPreviousRightLength)
} /* clearPrevious */

StandardInfoRoutine(VObjectPtr)