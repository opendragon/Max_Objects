/*--------------------------------------------------------------------------------------*//*                                                                                      *//*  File:       Vabs.c                                                                  *//*                                                                                      *//*  Contains:   The main entry point and support routines for the Vabs module.          *//*                                                                                      *//*  Written by: Norman Jaffe                                                            *//*                                                                                      *//*  Copyright:  (c) 2001 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   *//*                                                                                      *//*              All rights reserved. Redistribution and use in source and binary forms, *//*              with or without modification, are permitted provided that the following *//*              conditions are met:                                                     *//*                * Redistributions of source code must retain the above copyright      *//*                  notice, this list of conditions and the following disclaimer.       *//*                * Redistributions in binary form must reproduce the above copyright   *//*                  notice, this list of conditions and the following disclaimer in the *//*                  documentation and/or other materials provided with the              *//*                  distribution.                                                       *//*                * Neither the name of the copyright holders nor the names of its      *//*                  contributors may be used to endorse or promote products derived     *//*                  from this software without specific prior written permission.       *//*                                                                                      *//*              THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS     *//*              "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT       *//*              LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A *//*              PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT      *//*              OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,   *//*              SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT        *//*              LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,   *//*              DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY   *//*              THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT     *//*              (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE   *//*              OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.    *//*                                                                                      *//*  Created:    2001/04/06                                                              *//*                                                                                      *//*--------------------------------------------------------------------------------------*/#define IS_MAIN /* */#include "Vabs.h"#include "displayAboutBox.h"#include "loadAboutBoxData.h"#include "loadOtherSegments.h"#include "reportAnything.h"#include "reportVersion.h"/* Forward references: */Pvoid VabsCreate  (void);Pvoid VabsFree  (VObjectPtr xx);/*------------------------------------ main ---*/void main  (Pfptr ff){  EnterCodeResource();  PrepareCallback();  FNS = ff;   /* Connect up the function macros. */  /* Allocate class memory and set up class. */  setup(reinterpret_cast<t_messlist**>(&gClass), reinterpret_cast<method>(VabsCreate),        reinterpret_cast<method>(VabsFree), short(sizeof(VObjectData)),        reinterpret_cast<method>(0L), A_NOTHING);  addmess(reinterpret_cast<method>(cmd_Anything), "anything", A_GIMME, A_NOTHING);  addmess(reinterpret_cast<method>(cmd_Assist), "assist", A_CANT, A_NOTHING);  addbang(reinterpret_cast<method>(cmd_Bang));  addfloat(reinterpret_cast<method>(cmd_Float));  addmess(reinterpret_cast<method>(cmd_Info), "info", A_CANT, A_NOTHING);  addint(reinterpret_cast<method>(cmd_Int));  addmess(reinterpret_cast<method>(cmd_List), "list", A_GIMME, A_NOTHING);  /* Add object class to New Object List */  finder_addclass("Arith/Logic/Bitwise", OUR_NAME);  finder_addclass("Lists", OUR_NAME);  /* Make sure that our string list resource is present */  rescopy('STR#', OUR_RES_NUMB);  loadAboutBoxData();  gVersionNumber = reportVersion(OUR_NAME);  loadOtherSegments();  ExitCodeResource();} /* main *//*------------------------------------ VabsCreate ---*/Pvoid VabsCreate  (void){  VObjectPtr xx;  EnterCallback();  LOG_ENTER()  xx = static_cast<VObjectPtr>(newobject(gClass));  if (xx)  {    xx->fResultOut = static_cast<POutlet>(outlet_new(xx, NULL_PTR));    xx->fPreviousKind = A_NOTHING;    xx->fPreviousList = NULL_PTR;    xx->fPreviousLength = 0;    if (! xx->fResultOut)    {      LOG_ERROR_1(OUTPUT_PREFIX "unable to create port for object")      freeobject(reinterpret_cast<PObject>(xx));      xx = NULL_PTR;    }  }  ExitCodeResource();  return xx;} /* VabsCreate *//*------------------------------------ VabsFree ---*/Pvoid VabsFree  (VObjectPtr xx){  EnterCallback();  if (xx)    clearPrevious(xx);  LOG_EXIT()  ExitMaxMessageHandler()} /* VabsFree *//*------------------------------------ clearPrevious ---*/void clearPrevious  (VObjectPtr xx){  FREEBYTES(xx->fPreviousList, xx->fPreviousLength)} /* clearPrevious */StandardAnythingRoutine(VObjectPtr)StandardInfoRoutine(VObjectPtr)