/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       bqt.c                                                                   */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the bqt module.           */
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
/*  Created:    1998/11/01                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#define IS_MAIN /* */
#include "bqt.h"
#include "loadOtherSegments.h"
#include "reportAnything.h"
#include "reportVersion.h"

/* Forward references: */
Pvoid bqtCreate
  (PSymbol ss,
   short   argc,
   PAtom   argv);

Pvoid bqtFree
  (QtPtr xx);

Pvoid bqtMenu
  (PPatcher pp,
   long     left,
   long     top,
   long     font);

/*------------------------------------ main ---*/
void main
  (Pfptr ff)
{
  OSStatus err;
  long     result;

  EnterCodeResource();
  PrepareCallback();
  FNS = ff;   /* Connect up the function macros. */
  err = Gestalt(gestaltQuickTime, &result);
  if (err == noErr)
  {
    /* Allocate class memory and set up class. */
    setup(reinterpret_cast<t_messlist**>(&gClass), reinterpret_cast<method>(bqtCreate),
          reinterpret_cast<method>(bqtFree), short(sizeof(QtData)), reinterpret_cast<method>(bqtMenu), A_GIMME, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Active), "active", A_DEFLONG, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Anything), "anything", A_GIMME, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Assist), "assist", A_CANT, A_NOTHING);
    addbang(reinterpret_cast<method>(cmd_Bang));
    addmess(reinterpret_cast<method>(cmd_Begin), "begin", A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Border), "border", A_LONG, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Controller), "controller", A_SYM, A_SYM, A_SYM, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Count), "count", A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Duration), "duration", A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_End), "end", A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Film), "film", A_CANT, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_GetRate), "getrate", A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_GetVolume), "getvolume", A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Info), "info", A_CANT, A_NOTHING);
    addint(reinterpret_cast<method>(cmd_Int));
    addmess(reinterpret_cast<method>(cmd_Load), "load", A_DEFSYM, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Mute), "mute", A_DEFLONG, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Pause), "pause", A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Psave), "psave", A_CANT, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Rate), "rate", A_LONG, A_DEFLONG, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Resume), "resume", A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Segment), "segment", A_LONG, A_DEFLONG, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Start), "start", A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Pause), "stop", A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Time), "time", A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Unload), "unload", A_DEFSYM, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Update), "update", A_CANT, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Volume), "volume", A_DEFLONG, A_NOTHING);
    /* Make sure that our string list resource is present */
    rescopy('STR#', OUR_RES_NUMB);
  }
  else
    LOG_ERROR_1(OUTPUT_PREFIX "QuickTime required")
  qtSetUpGlobals(OUR_NAME, OUR_RES_NUMB);
  GetIndPattern(&gRectPat, sysPatListID, 4);
  gVersionNumber = reportVersion(OUR_NAME);
  loadOtherSegments();
  ExitCodeResource();
} /* main */

/*------------------------------------ bqtCheckForMove ---*/
static void bqtCheckForMove
  (QtPtr xx)
{
  short deltaX, deltaY;
  Rect  currentBox;

  if (! EqualRect(&xx->fOldBox, &xx->fVariant.fBox.b_rect))
  {
    if (xx->fCurrentMovie)
    {
      deltaX = short(xx->fVariant.fBox.b_rect.left - xx->fOldBox.left);
      deltaY = short(xx->fVariant.fBox.b_rect.top - xx->fOldBox.top);
      bqtClipToBox(xx, xx->fCurrentMovie);
      if (deltaX || deltaY)
      {
        GetMovieBox(xx->fCurrentMovie, &currentBox);
        OffsetRect(&currentBox, deltaX, deltaY);
        SetMovieBox(xx->fCurrentMovie, &currentBox);
      }
      qtMoveController(xx);
    }
    xx->fOldBox = xx->fVariant.fBox.b_rect;
  }
} /* bqtCheckForMove */

/*------------------------------------ bqtClipToBox ---*/
static void bqtClipToBox
  (QtPtr xx,
   Movie aMovie)
{
  RgnHandle currentRegion, oldRegion;
  Rect      aRect;

  currentRegion = NewRgn();
  oldRegion = NewRgn();
  GetClip(oldRegion);
  aRect = xx->fVariant.fBox.b_rect;
  InsetRect(&aRect, 1, 1);
  RectRgn(currentRegion, &aRect);
  SectRgn(currentRegion, oldRegion, currentRegion);
  SetMovieDisplayClipRgn(aMovie, currentRegion);
  DisposeRgn(currentRegion);
  DisposeRgn(oldRegion);
} /*  bqtClipToBox */

/*------------------------------------ bqtCreate ---*/
Pvoid bqtCreate
  (PSymbol ss,
   short   argc,
   PAtom   argv)
{
#pragma unused(ss)
  QtPtr xx = NULL_PTR;
  bool  okSoFar = true;

  EnterCallback();
  LOG_ENTER()
  if (! gInited)
    okSoFar = qtInit();
  if (okSoFar)
  {
    xx = static_cast<QtPtr>(newobject(gClass));
    if (xx)
    {
      Rect    aRect;
      long    flags;
      PSymbol initial;

      qtSetUpObject(xx, reinterpret_cast<PPatcher>(argv->a_w.w_obj));
      aRect.left = short((argv + 1)->a_w.w_long);
      aRect.top = short((argv + 2)->a_w.w_long);
      aRect.right = short((argv + 3)->a_w.w_long + aRect.left);
      aRect.bottom = short((argv + 4)->a_w.w_long + aRect.top);
      flags = (argv + 5)->a_w.w_long;
      if (argc > 6)
        initial = (argv + 6)->a_w.w_sym;
      else
        initial = gEmptySymbol;
      box_new(reinterpret_cast<PBox>(xx), xx->fPatcher, F_DRAWFIRSTIN | F_GROWBOTH,
              aRect.left, aRect.top, aRect.right, aRect.bottom);
      xx->fBorderState = (flags != 0);
      xx->fVariant.fBox.b_outline = xx->fBorderState;
      xx->fVariant.fBox.b_firstin = xx;
      if (! (xx->fResultOut && xx->fDurationOut && xx->fErrorBangOut &&
            xx->fStoppedBangOut && xx->fErrorQueue && xx->fStoppedQueue))
      {
        LOG_ERROR_1(OUTPUT_PREFIX "unable to create port or queue for object")
        okSoFar = false;
      }
      if (okSoFar)
      {
        if (initial == gEmptySymbol)
        {
          /* Fool the movie controller, if there is one! */
          QtMovieHdl aMovie = reinterpret_cast<QtMovieHdl>(newhandle(sizeof(QtMovie)));
          QtMoviePtr thisMovie;

          HLock(reinterpret_cast<Handle>(aMovie));
          thisMovie = *aMovie;
          thisMovie->fMovieFileValid = false;
          thisMovie->fName = gEmptySymbol;
          thisMovie->fMovie = NewMovie(newMovieActive);
          thisMovie->fNextMovie = NULL_HDL;
          thisMovie->fPreviousMovie = xx->fLastMovie;
          (*xx->fLastMovie)->fNextMovie = aMovie;
          xx->fLastMovie = aMovie;
          if (! xx->fFirstMovie)
            xx->fFirstMovie = aMovie;
          xx->fCurrentMovie = thisMovie->fMovie;
          xx->fCurrentName = gEmptySymbol;
          HUnlock(reinterpret_cast<Handle>(aMovie));
        }
        else
          deferred_Load(xx, initial);
      }
      if (okSoFar)
      {
        /* Connect this block into the block list. */
        xx->fNextBlock = gAnchor->fNextBlock;
        if (gAnchor->fNextBlock)
          gAnchor->fNextBlock->fPreviousBlock = xx;
        gAnchor->fNextBlock = xx;
        xx->fPreviousBlock = gAnchor;
        box_ready(reinterpret_cast<PBox>(xx));
        xx->fOldBox = xx->fVariant.fBox.b_rect;
        gSaveSymbol->s_thing = reinterpret_cast<PObject>(xx);    /* prepare for binbuf operation */
      }
      else
      {
        box_free(reinterpret_cast<PBox>(xx));
        xx = NULL_PTR;
      }
    }
  }
  ExitCodeResource();
  return xx;
} /* bqtCreate */

/*------------------------------------ bqtFree ---*/
Pvoid bqtFree
  (QtPtr xx)
{
  EnterCallback();
  if (xx)
  {
    deferred_Unload(xx, gEmptySymbol);
    if (xx->fErrorQueue)
    {
      qelem_unset(xx->fErrorQueue);
      qelem_free(xx->fErrorQueue);
      xx->fErrorQueue = NULL_PTR;
    }
    if (xx->fStoppedQueue)
    {
      qelem_unset(xx->fStoppedQueue);
      qelem_free(xx->fStoppedQueue);
      xx->fStoppedQueue = NULL_PTR;
    }
    /* Detach this block from the block list. */
    if (xx->fNextBlock)
      xx->fNextBlock->fPreviousBlock = xx->fPreviousBlock;
    if (xx->fPreviousBlock)
      xx->fPreviousBlock->fNextBlock = xx->fNextBlock;
    box_free(reinterpret_cast<PBox>(xx));
  }
  LOG_EXIT()
  ExitMaxMessageHandler()
} /* bqtFree */

/*------------------------------------ bqtMenu ---*/
Pvoid bqtMenu
  (PPatcher pp,
   long     left,
   long     top,
   long     font)
{
#pragma unused(font)
  Atom  args[8];
  Pvoid result;

  EnterCallback();
  SETOBJ(args, reinterpret_cast<PObject>(pp));
  SETLONG(args + 1, left);
  SETLONG(args + 2, top);
  SETLONG(args + 3, DEFAULT_BOX_WIDTH);
  SETLONG(args + 4, DEFAULT_BOX_HEIGHT);
  SETLONG(args + 5, DEFAULT_BOX_FLAGS);
  SETSYM(args + 6, gEmptySymbol);
  result = bqtCreate(gOurSymbol, 7, args);
  ExitCodeResource();
  return result;
} /* bqtMenu */

StandardAnythingRoutine(QtPtr)