/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       wqt.c                                                                   */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the wqt module.           */
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
/*  Created:    1998/10/24                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#define IS_MAIN /* */
#include "wqt.h"
#include "loadOtherSegments.h"
#include "reportAnything.h"
#include "reportVersion.h"

/* Forward references: */
Pvoid
wqtCreate(PSymbol initial);

Pvoid
wqtFree(QtPtr xx);

/*------------------------------------ main ---*/
void
main(Pfptr ff)
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
    setup(reinterpret_cast<t_messlist**>(&gClass), reinterpret_cast<method>(wqtCreate),
          reinterpret_cast<method>(wqtFree), short(sizeof(QtData)),
          reinterpret_cast<method>(0L), A_DEFSYM, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Active), "active", A_DEFLONG, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Anything), "anything", A_GIMME, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Assist), "assist", A_CANT, A_NOTHING);
    addbang(reinterpret_cast<method>(cmd_Bang));
    addmess(reinterpret_cast<method>(cmd_Begin), "begin", A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Close), "close", A_CANT, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Controller), "controller", A_SYM, A_SYM, A_SYM, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Count), "count", A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_DblClick), "dblclick", A_CANT, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Duration), "duration", A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_End), "end", A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Film), "film", A_CANT, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_GetRate), "getrate", A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_GetVolume), "getvolume", A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Idle), "idle", A_CANT, A_NOTHING);
    addint(reinterpret_cast<method>(cmd_Int));
    addmess(reinterpret_cast<method>(cmd_Load), "load", A_DEFSYM, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Mute), "mute", A_DEFLONG, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Open), "open", A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Pause), "pause", A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Rate), "rate", A_LONG, A_DEFLONG, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Resume), "resume", A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Save), "save", A_CANT, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Segment), "segment", A_LONG, A_DEFLONG, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Start), "start", A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Pause), "stop", A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Time), "time", A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Unload), "unload", A_DEFSYM, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Update), "update", A_CANT, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Volume), "volume", A_DEFLONG, A_NOTHING);
    /* Add object class to New Object List */
    finder_addclass("Graphics", OUR_NAME);
    /* Make sure that our string list resource is present */
    rescopy('STR#', OUR_RES_NUMB);
  }
  else
    error(OUTPUT_PREFIX "QuickTime required");
  qtSetUpGlobals(OUR_NAME, OUR_RES_NUMB);
  gVersionNumber = reportVersion(OUR_NAME);
  loadOtherSegments();
  ExitCodeResource();
} /* main */

/*------------------------------------ wqtCreate ---*/
Pvoid
wqtCreate(PSymbol initial)
{
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
      qtSetUpObject(xx, reinterpret_cast<PPatcher>(gensym("#P")->s_thing));    /* Max stores our parent here */
      xx->fWindow = wind_new(xx,      /* Owning object */
      WIND_INITIAL_LEFT, WIND_INITIAL_TOP,
      WIND_INITIAL_RIGHT, WIND_INITIAL_BOTTOM,
      WSGROW | WCOLOR);
      if (! (xx->fResultOut && xx->fDurationOut && xx->fErrorBangOut &&
            xx->fStoppedBangOut && xx->fErrorQueue && xx->fStoppedQueue && xx->fWindow))
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
        gSaveSymbol->s_thing = reinterpret_cast<PObject>(xx);    /* prepare for binbuf operation */
      }
      else
      {
        freeobject(reinterpret_cast<PObject>(xx));
        xx = NULL_PTR;
      }
    }
  }
  ExitCodeResource();
  return xx;
} /* wqtCreate */

/*------------------------------------ wqtFree ---*/
Pvoid
wqtFree(QtPtr xx)
{
  EnterCallback();
  if (xx)
  {
    deferred_Unload(xx, gEmptySymbol);
    if (xx->fWindow)
      wind_invis(xx->fWindow);
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
    //TBD
  }
  LOG_EXIT()
  ExitMaxMessageHandler()
} /* wqtFree */

StandardAnythingRoutine(QtPtr)
