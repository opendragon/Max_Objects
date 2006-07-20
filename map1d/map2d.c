/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       map2d.c                                                                 */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the Two-Dimensional       */
/*              mapping module.                                                         */
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
/*  Created:    2000/11/15                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#define IS_MAIN /* */
#include "map2d.h"
#include "displayAboutBox.h"
#include "loadAboutBoxData.h"
#include "loadOtherSegments.h"
#include "reportAnything.h"
#include "reportVersion.h"

/* Forward references: */
Pvoid map2dCreate
  (PSymbol initialFile);

Pvoid map2dFree
  (Map2dPtr xx);

/*------------------------------------ main ---*/
void main
  (Pfptr ff)
{
  EnterCodeResource();
  PrepareCallback();
  FNS = ff;   /* Connect up the function macros. */
  /* Allocate class memory and set up class. */
  setup(reinterpret_cast<t_messlist**>(&gClass), reinterpret_cast<method>(map2dCreate),
        reinterpret_cast<method>(map2dFree), short(sizeof(Map2dData)),
        reinterpret_cast<method>(0L), A_DEFSYM, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Add), "add", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_After), "after", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Anything), "anything", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Assist), "assist", A_CANT, A_NOTHING);
  addbang(reinterpret_cast<method>(cmd_Bang));
  addmess(reinterpret_cast<method>(cmd_Before), "before", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Clear), "clear", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Count), "count", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Delete), "delete", A_LONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Dump), "dump", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Get), "get", A_LONG, A_SYM, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Info), "info", A_CANT, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_List), "list", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Load), "load", A_SYM, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Replace), "replace", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Set), "set", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Show), "show", A_LONG, A_NOTHING);
#if defined(BE_VERBOSE)
  addmess(reinterpret_cast<method>(cmd_Verbose), "verbose", A_DEFSYM, A_NOTHING);
#endif /* BE_VERBOSE */
  /* Add object class to New Object List */
  finder_addclass("Arith/Logic/Bitwise", OUR_NAME);
  finder_addclass("Lists", OUR_NAME);
  /* Make sure that our string list resource is present */
  rescopy('STR#', OUR_RES_NUMB);
  loadAboutBoxData();
  SetUpSymbols();
  gBottomSymbol = gensym("bottom");
  gLeftSymbol = gensym("left");
  gRightSymbol = gensym("right");
  gTopSymbol = gensym("top");
  gVersionNumber = reportVersion(OUR_NAME);
  loadOtherSegments();
  ExitCodeResource();
} /* main */

/*------------------------------------ map2dCreate ---*/
Pvoid map2dCreate
  (PSymbol initialFile)
{
  Map2dPtr xx;

  EnterCallback();
  LOG_ENTER()
  xx = static_cast<Map2dPtr>(newobject(gClass));
  if (xx)
  {
    xx->fVerbose = false;
    xx->fResultOut = static_cast<POutlet>(outlet_new(xx, NULL_PTR));
    xx->fFirstRange = xx->fLastRange = xx->fPreviousResult = NULL_PTR;
    setFOI2Integer(xx->fPreviousInput[0], 0);
    setFOI2Integer(xx->fPreviousInput[1], 0);
    xx->fRangeCount = 0;
    xx->fBuffer = static_cast<PBinbuf>(binbuf_new());
    if (! (xx->fResultOut || xx->fBuffer))
    {
      LOG_ERROR_1(OUTPUT_PREFIX "unable to create port for object")
      freeobject(reinterpret_cast<PObject>(xx));
      xx = NULL_PTR;
    }
    else
    {
      if (initialFile == gEmptySymbol)
        LOG_POST_1(OUTPUT_PREFIX "no initial map file")
      else
      {
        LOG_POST_2(OUTPUT_PREFIX "initial file: %s", initialFile->s_name)
        map2dLoadRangeList(xx, initialFile);
      }
    }
  }
  ExitCodeResource();
  return xx;
} /* map2dCreate */

/*------------------------------------ map2dFree ---*/
Pvoid map2dFree
  (Map2dPtr xx)
{
  EnterCallback();
  if (xx)
  {
    if (xx->fBuffer)
    {
      freeobject(reinterpret_cast<PObject>(xx->fBuffer));
      xx->fBuffer = NULL_PTR;
    }
    map2dClearRangeList(xx);
  }
  LOG_EXIT()
  ExitMaxMessageHandler()
} /* map2dFree */

StandardAnythingRoutine(Map2dPtr)
StandardInfoRoutine(Map2dPtr)