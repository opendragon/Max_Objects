/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       stack.c                                                                 */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the stack module.         */
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
/*  Created:    2000/06/22                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#define IS_MAIN /* */
#include "stack.h"
#include "displayAboutBox.h"
#include "loadAboutBoxData.h"
#include "loadOtherSegments.h"
#include "reportAnything.h"
#include "reportVersion.h"

/* Forward references: */
Pvoid stackCreate
  (PSymbol commonName);

Pvoid stackFree
  (StackPtr xx);

static void stackResolveCommonName
  (StackPtr xx,
   PSymbol  name);

/*------------------------------------ main ---*/
void main
  (Pfptr ff)
{
  EnterCodeResource();
  PrepareCallback();
  FNS = ff;   /* Connect up the function macros. */
  /* Allocate class memory and set up class. */
  setup(reinterpret_cast<t_messlist**>(&gClass), reinterpret_cast<method>(stackCreate),
        reinterpret_cast<method>(stackFree), short(sizeof(StackData)),
        reinterpret_cast<method>(0L), A_DEFSYM, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Anything), "anything", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Assist), "assist", A_CANT, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Clear), "clear", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Depth), "depth", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Dup), "dup", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Info), "info", A_CANT, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Pop), "pop", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Push), "push", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Swap), "swap", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Top), "top", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_TopPop), "top+pop", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Trace), "trace", A_DEFSYM, A_NOTHING);
  /* Add object class to New Object List */
  finder_addclass("Data", OUR_NAME);
  /* Make sure that our string list resource is present */
  rescopy('STR#', OUR_RES_NUMB);
  loadAboutBoxData();
  gEmptySymbol = gensym("");
  gOffSymbol = gensym("off");
  gOnSymbol = gensym("on");
  gStackAnchor = NULL_PTR;
  gVersionNumber = reportVersion(OUR_NAME);
  loadOtherSegments();
  ExitCodeResource();
} /* main */

/*------------------------------------ stackResolveCommonName ---*/
static void stackResolveCommonName
  (StackPtr xx,
   PSymbol  name)
{
  StackDescriptorPtr descriptor = NULL_PTR;

  if (name != gEmptySymbol)
  {
    for (descriptor = gStackAnchor; descriptor; descriptor = descriptor->fNext)
    {
      if (descriptor->fTag == name)
        break;

    }
  }
  if (descriptor)
  {
    ++descriptor->fReferenceCount;
    xx->fStack = descriptor;
  }
  else
  {
    descriptor = GETBYTES(1, StackDescriptor);
    if (descriptor)
    {
      descriptor->fTopOfStack = NULL_PTR;
      descriptor->fDepth = 0;
      descriptor->fReferenceCount = 1;
      xx->fStack = descriptor;
      descriptor->fPrevious = NULL_PTR;
      if (name == gEmptySymbol)
      {
        descriptor->fTag = NULL_PTR;
        descriptor->fNext = NULL_PTR;
      }
      else
      {
        descriptor->fTag = name;
        descriptor->fNext = gStackAnchor;
        if (gStackAnchor)
          gStackAnchor->fPrevious = descriptor;
        gStackAnchor = descriptor;
      }       
    }
  }
} /* stackResolveCommonName */

/*------------------------------------ stackCreate ---*/
Pvoid stackCreate
  (PSymbol commonName)
{
  StackPtr xx;

  EnterCallback();
  LOG_ENTER()
  xx = static_cast<StackPtr>(newobject(gClass));
  if (xx)
  {
    xx->fVerbose = false;
    xx->fErrorBangOut = static_cast<POutlet>(bangout(xx));
    xx->fDepthOut = static_cast<POutlet>(intout(xx));
    xx->fResultOut = static_cast<POutlet>(outlet_new(xx, NULL_PTR));
    xx->fStack = NULL_PTR;
    stackResolveCommonName(xx, commonName);
    if (! (xx->fResultOut || xx->fErrorBangOut || xx->fDepthOut ||
            xx->fStack))
    {
      LOG_ERROR_1(OUTPUT_PREFIX "unable to create port for object")
      freeobject(reinterpret_cast<PObject>(xx));
      xx = NULL_PTR;
    }
  }
  ExitCodeResource();
  return xx;
} /* stackCreate */

/*------------------------------------ stackFree ---*/
Pvoid stackFree
  (StackPtr xx)
{
  EnterCallback();
  if (xx)
  {
    StackDescriptorPtr descriptor = xx->fStack;

    if (descriptor)
    {
      if (! --descriptor->fReferenceCount)
      {
        if (descriptor->fNext)
          descriptor->fNext->fPrevious = descriptor->fPrevious;
        if (descriptor->fPrevious)
          descriptor->fPrevious->fNext = descriptor->fNext;
        if (gStackAnchor == descriptor)
          gStackAnchor = descriptor->fNext;
        stackClear(xx);
        FREEBYTES(xx->fStack, 1)
      }
    }
  }
  LOG_EXIT()
  ExitMaxMessageHandler()
} /* stackFree */

StandardAnythingRoutine(StackPtr)
StandardInfoRoutine(StackPtr)