/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       Vjet.c                                                                  */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the Vjet module.          */
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
/*  Created:    2000/07/14                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#define IS_MAIN /* */
#include "Vjet.h"
#include "displayAboutBox.h"
#include "loadAboutBoxData.h"
#include "loadOtherSegments.h"
#include "reportVersion.h"

/* Forward references: */
Pvoid VjetCreate
  (long howMany);

Pvoid VjetFree
  (VObjectPtr xx);

Pvoid VjetProcessInput
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
  setup(reinterpret_cast<t_messlist**>(&gClass), reinterpret_cast<method>(VjetCreate),
        reinterpret_cast<method>(VjetFree), short(sizeof(VObjectData)),
        reinterpret_cast<method>(0L), A_LONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Anything), "anything", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Assist), "assist", A_CANT, A_NOTHING);
  addbang(reinterpret_cast<method>(cmd_Bang));
  addmess(reinterpret_cast<method>(cmd_Info), "info", A_CANT, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_List), "list", A_GIMME, A_NOTHING);
  addinx(reinterpret_cast<method>(VjetProcessInput), 1);
  /* Add object class to New Object List */
  finder_addclass("Lists", OUR_NAME);
  /* Make sure that our string list resource is present */
  rescopy('STR#', OUR_RES_NUMB);
  loadAboutBoxData();
  gVersionNumber = reportVersion(OUR_NAME);
  loadOtherSegments();
  ExitCodeResource();
} /* main */

/*------------------------------------ VjetCreate ---*/
Pvoid VjetCreate
  (long howMany)
{
  VObjectPtr xx;

  EnterCallback();
  LOG_ENTER()
  xx = static_cast<VObjectPtr>(newobject(gClass));
  if (xx)
  {
    xx->fPreviousList = NULL_PTR;
    xx->fPreviousLength = 0;
    if (howMany > 0)
    {
      xx->fHowMany = short(howMany);
      intin(xx, 1);
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
      LOG_ERROR_2(OUTPUT_PREFIX "invalid segment size (%ld)", howMany)
      freeobject(reinterpret_cast<PObject>(xx));
      xx = NULL_PTR;
    }
  }
  ExitCodeResource();
  return xx;
} /* VjetCreate */

/*------------------------------------ VjetFree ---*/
Pvoid VjetFree
  (VObjectPtr xx)
{
  EnterCallback();
  if (xx)
    clearPrevious(xx);
  LOG_EXIT()
  ExitMaxMessageHandler()
} /* VjetFree */

/*------------------------------------ VjetProcessInput ---*/
Pvoid VjetProcessInput
  (VObjectPtr xx,
   long       number)
{
  EnterCallback();
  /* We've received a number. */
  if (xx)
  {
    if (number > 0)
      xx->fHowMany = short(number);
    else
      LOG_ERROR_2(OUTPUT_PREFIX "invalid segment size (%ld)", number)
  }
  ExitMaxMessageHandler()
} /* VjetProcessInput */

/*------------------------------------ clearPrevious ---*/
void clearPrevious
  (VObjectPtr xx)
{
  FREEBYTES(xx->fPreviousList, xx->fPreviousLength)
} /* clearPrevious */

StandardInfoRoutine(VObjectPtr)