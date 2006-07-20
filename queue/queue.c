/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       queue.c                                                                 */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the queue module.         */
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
/*  Created:    2001/04/24                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#define IS_MAIN /* */
#include "queue.h"
#include "displayAboutBox.h"
#include "loadAboutBoxData.h"
#include "loadOtherSegments.h"
#include "reportAnything.h"
#include "reportVersion.h"

/* Forward references: */
Pvoid queueCreate
  (long depth);

Pvoid queueFree
  (QueuePtr xx);

static void queueResolveCommonName
  (QueuePtr xx,
   PSymbol  name);

/*------------------------------------ main ---*/
void main
  (Pfptr ff)
{
  EnterCodeResource();
  PrepareCallback();
  FNS = ff;   /* Connect up the function macros. */
  /* Allocate class memory and set up class. */
  setup(reinterpret_cast<t_messlist**>(&gClass), reinterpret_cast<method>(queueCreate),
        reinterpret_cast<method>(queueFree), short(sizeof(QueueData)),
        reinterpret_cast<method>(0L), A_LONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Add), "add", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Anything), "anything", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Assist), "assist", A_CANT, A_NOTHING);
  addbang(reinterpret_cast<method>(cmd_Bang));
  addmess(reinterpret_cast<method>(cmd_Clear), "clear", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Depth), "depth", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Fetch), "fetch", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Info), "info", A_CANT, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Pull), "pull", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetDepth), "setdepth", A_LONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Trace), "trace", A_DEFSYM, A_NOTHING);
  /* Add object class to New Object List */
  finder_addclass("Data", OUR_NAME);
  /* Make sure that our string list resource is present */
  rescopy('STR#', OUR_RES_NUMB);
  loadAboutBoxData();
  gEmptySymbol = gensym("");
  gOffSymbol = gensym("off");
  gOnSymbol = gensym("on");
  gVersionNumber = reportVersion(OUR_NAME);
  loadOtherSegments();
  ExitCodeResource();
} /* main */

/*------------------------------------ queueCreate ---*/
Pvoid queueCreate
  (long depth)
{
  QueuePtr xx;

  EnterCallback();
  LOG_ENTER()
  xx = static_cast<QueuePtr>(newobject(gClass));
  if (xx)
  {
    if (depth >= 0)
    {
      xx->fVerbose = false;
      xx->fMaxDepth = depth;
      xx->fErrorBangOut = static_cast<POutlet>(bangout(xx));
      xx->fDepth = 0;
      xx->fDepthOut = static_cast<POutlet>(intout(xx));
      xx->fResultOut = static_cast<POutlet>(outlet_new(xx, NULL_PTR));
      xx->fFirstInQueue = xx->fLastInQueue = NULL_PTR;
      if (! (xx->fResultOut || xx->fDepthOut || xx->fErrorBangOut))
      {
        LOG_ERROR_1(OUTPUT_PREFIX "unable to create port for object")
        freeobject(reinterpret_cast<PObject>(xx));
        xx = NULL_PTR;
      }
    }
    else
    {
      LOG_ERROR_2(OUTPUT_PREFIX "bad queue depth (%ld)", depth)
      freeobject(reinterpret_cast<PObject>(xx));
      xx = NULL_PTR;
    }
  }
  ExitCodeResource();
  return xx;
} /* queueCreate */

/*------------------------------------ queueFree ---*/
Pvoid queueFree
  (QueuePtr xx)
{
  EnterCallback();
  if (xx)
    queueClear(xx);
  LOG_EXIT()
  ExitMaxMessageHandler()
} /* queueFree */

StandardAnythingRoutine(QueuePtr)
StandardInfoRoutine(QueuePtr)