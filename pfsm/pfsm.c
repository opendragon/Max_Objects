/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       pfsm.c                                                                  */
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
#include "displayAboutBox.h"
#include "loadAboutBoxData.h"
#include "loadOtherSegments.h"
#include "reportAnything.h"
#include "reportVersion.h"

/* Forward references: */
Pvoid pfsmCreate
  (PSymbol initialFile);

Pvoid pfsmFree
  (PfsmPtr xx);

/*------------------------------------ main ---*/
void main
  (Pfptr ff)
{
  EnterCodeResource();
  PrepareCallback();
  FNS = ff;   /* Connect up the function macros. */
  /* Allocate class memory and set up class. */
  setup(reinterpret_cast<t_messlist**>(&gClass), reinterpret_cast<method>(pfsmCreate),
        reinterpret_cast<method>(pfsmFree), short(sizeof(PfsmData)),
        reinterpret_cast<method>(0L), A_DEFSYM, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Anything), "anything", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Assist), "assist", A_CANT, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Autorestart), "autorestart", A_DEFSYM, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Clear), "clear", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Describe), "describe", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Do), "do", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Goto), "goto", A_SYM, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Info), "info", A_CANT, A_NOTHING);
  addint(reinterpret_cast<method>(cmd_Int));
  addmess(reinterpret_cast<method>(cmd_List), "list", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Load), "load", A_SYM, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Start), "start", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Status), "status", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Stop), "stop", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Trace), "trace", A_DEFSYM, A_NOTHING);
  /* Add object class to New Object List */
  finder_addclass("Arith/Logic/Bitwise", OUR_NAME);
  finder_addclass("Lists", OUR_NAME);
  /* Make sure that our string list resource is present */
  rescopy('STR#', OUR_RES_NUMB);
  loadAboutBoxData();
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
  gVersionNumber = reportVersion(OUR_NAME);
  loadOtherSegments();
  ExitCodeResource();
} /* main */

/*------------------------------------ pfsmCreate ---*/
Pvoid pfsmCreate
  (PSymbol initialFile)
{
  PfsmPtr xx;

  EnterCallback();
  LOG_ENTER()
  xx = static_cast<PfsmPtr>(newobject(gClass));
  if (xx)
  {
    xx->fAutoRestart = xx->fVerbose = xx->fRunning = false;
    xx->fCurrentState = xx->fStartState = NULL_PTR;
    xx->fStateSymbols = NULL_HDL;
    xx->fErrorBangOut = static_cast<POutlet>(bangout(xx));
    xx->fStopBangOut = static_cast<POutlet>(bangout(xx));
    xx->fResultOut = static_cast<POutlet>(outlet_new(xx, NULL_PTR));
    xx->fBuffer = static_cast<PBinbuf>(binbuf_new());
    if (! (xx->fResultOut || xx->fErrorBangOut || xx->fStopBangOut || xx->fBuffer))
    {
      LOG_ERROR_1(OUTPUT_PREFIX "unable to create port for object")
      freeobject(reinterpret_cast<PObject>(xx));
      xx = NULL_PTR;
    }
    else
    {
      if (initialFile == gEmptySymbol)
        LOG_POST_1(OUTPUT_PREFIX "no initial state file")
      else
      {
        LOG_POST_2(OUTPUT_PREFIX "initial file: %s", initialFile->s_name)
        pfsmLoadTables(xx, initialFile);
      }
    }
  }
  ExitCodeResource();
  return xx;
} /* pfsmCreate */

/*------------------------------------ pfsmFree ---*/
Pvoid pfsmFree
  (PfsmPtr xx)
{
  EnterCallback();
  if (xx)
  {
    if (xx->fBuffer)
    {
      freeobject(reinterpret_cast<PObject>(xx->fBuffer));
      xx->fBuffer = NULL_PTR;
    }
    pfsmClearHashTable(xx);
  }
  LOG_EXIT()
  ExitMaxMessageHandler()
} /* pfsmFree */

StandardAnythingRoutine(PfsmPtr)
StandardInfoRoutine(PfsmPtr)