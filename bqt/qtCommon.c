/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       qtCommon.c                                                              */
/*                                                                                      */
/*  Contains:   The common utility routines.                                            */
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
/*  Created:    1998/10/30                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "qtCommon.h"

/*------------------------------------ ProcessErrorQueue ---*/
static Pvoid ProcessErrorQueue
  (QtPtr xx)
{
  EnterCallback();
  if (xx)
  {
    short prevLock = lockout_set(1);

    outlet_bang(xx->fErrorBangOut);
    lockout_set(prevLock);
    evnum_incr();
  }
  ExitMaxMessageHandler()
} /* ProcessErrorQueue */

/*------------------------------------ ProcessMovieQueue ---*/
static Pvoid ProcessMovieQueue
  (QtPtr xx)
{
#if defined(BQT_H_)
  GrafPtr oldPort;
#endif /* BQT_H_ */

  EnterCallback();
  for (xx = xx->fNextBlock; xx; xx = xx->fNextBlock)
  {
    if (xx->fCurrentMovie)
    {
#if defined(WQT_H_)
      if (xx->fWindow && xx->fWindow->w_vis)
      {
        if (IsMovieDone(xx->fCurrentMovie))
        {
          if (! xx->fStopSignalled)
          {
            SignalStopped(xx);
            xx->fStopSignalled = true;
            xx->fCurrentRate = gStoppedRate;
            SetMovieRate(xx->fCurrentMovie, gStoppedRate);
          }
        }
        else if (! qtAdjustController(xx))
          MoviesTask(xx->fCurrentMovie, 0);
      }
#endif /* WQT_H_ */
#if defined(BQT_H_)
      if (IsMovieDone(xx->fCurrentMovie))
      {
        if (! xx->fStopSignalled)
        {
          SignalStopped(xx);
          xx->fStopSignalled = true;
          xx->fCurrentRate = gStoppedRate;
          SetMovieRate(xx->fCurrentMovie, gStoppedRate);
        }
      }
      else
      {
        oldPort = patcher_setport(xx->fBox.b_patcher);
        if (oldPort)
        {
          if ((! box_nodraw(static_cast<PBox>(xx))) &&
              EqualRect(&xx->fBox.b_rect, &xx->fOldBox))
          {
            if (! qtAdjustController(xx))
              MoviesTask(xx->fCurrentMovie, 0);
          }
          SetPort(oldPort);
        }   
      }
#endif /* BQT_H */
    }
  }
  /* Retrigger the queue */
  qelem_set(gMovieTask);
  ExitMaxMessageHandler()
} /* ProcessMovieQueue */

/*------------------------------------ ProcessStoppedQueue ---*/
static Pvoid ProcessStoppedQueue
  (QtPtr xx)
{
  EnterCallback();
  if (xx)
  {
    short prevLock = lockout_set(1);

    outlet_bang(xx->fStoppedBangOut);
    lockout_set(prevLock);
    evnum_incr();
  }
  ExitMaxMessageHandler()
} /* ProcessStoppedQueue */

/*------------------------------------ qtAddMovie ---*/
QtMovieHdl qtAddMovie
  (QtPtr      xx,
   QtMovieHdl aMovie)
{
#pragma unused(xx)
  bool       okSoFar = true;
  OSErr      err = noErr;
  QtMoviePtr thisMovie;
  RgnHandle  oldClip;
  GrafPtr    grafPort;
  Rect       movieBounds;

  HLock(reinterpret_cast<Handle>(aMovie));
  thisMovie = *aMovie;
  oldClip = NewRgn();
  GetClip(oldClip);
  GetPort(&grafPort);
#if defined(COMPILE_FOR_OSX_4)
  GetPortBounds(grafPort, &movieBounds);
  ClipRect(&movieBounds);
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
  ClipRect(&grafPort->portRect);
#endif /* COMPILE_FOR_OS9_4 */
  err = NewMovieFromFile(&thisMovie->fMovie, thisMovie->fResRef, NULL_PTR, NULL_PTR,
                          newMovieActive, NULL_PTR);
  if (err != noErr)
  {
    SetClip(oldClip);
    DisposeRgn(oldClip);
    LOG_ERROR_3(OUTPUT_PREFIX "problem loading movie from file '%s' -> error (%ld)",
                thisMovie->fName->s_name, long(err))
    okSoFar = false;
  }
  else
  {
    thisMovie->fDuration = GetMovieDuration(thisMovie->fMovie);
    GetMovieBox(thisMovie->fMovie, &movieBounds);
    /* Adjust the movie bounding box. */
#if defined(WQT_H_)
    OffsetRect(&movieBounds, short(- movieBounds.left), short(- movieBounds.top));
    SetMovieBox(thisMovie->fMovie, &movieBounds);
    if (! EmptyRect(&movieBounds))
    {
      /* We have something to show. */
      wind_vis(xx->fWindow);
      wind_setport(xx->fWindow);
      SetMovieGWorld(thisMovie->fMovie, NULL_PTR, NULL_HDL);
    }
#endif /* WQT_H_ */
#if defined(BQT_H_)
    OffsetRect(&movieBounds, short(xx->fBox.b_rect.left + 1 - movieBounds.left),
                short(xx->fBox.b_rect.top + 1 - movieBounds.top));
    SetMovieBox(thisMovie->fMovie, &movieBounds);
    if (patcher_setport(xx->fBox.b_patcher))
    {
      SetMovieGWorld(thisMovie->fMovie, NULL_PTR, NULL_HDL);
      bqtClipToBox(xx, thisMovie->fMovie);
    }
    else
      /* We have to defer setting up the movie until update time. */
      xx->fPendingMovie = thisMovie->fMovie;
#endif /* BQT_H_ */
    SetPort(grafPort);
    SetClip(oldClip);
    DisposeRgn(oldClip);
  }
  HUnlock(reinterpret_cast<Handle>(aMovie));
  if (! okSoFar)
  {
    disposhandle(reinterpret_cast<Handle>(aMovie));
    aMovie = NULL_HDL;
  }
  return aMovie;
} /* qtAddMovie */

/*------------------------------------ qtAdjustController ---*/
bool qtAdjustController
  (QtPtr xx)
{
  if (! xx->fController)
    return false;

  if (NOGOOD(xx->fController))
  {
    LOG_ERROR_1(OUTPUT_PREFIX "adjust/controller corrupt")
    xx->fController = NULL_PTR;
    return false;

  }
  if (! xx->fControllerMethods)
    xx->fControllerMethods =
            reinterpret_cast<QtMethod>(egetfn(reinterpret_cast<PObject>(xx->fController),
                                                gCtrlSymbol));
  (*xx->fControllerMethods) (xx->fController, CTRL_MOVIE_IDLE, long(xx->fCurrentMovie));
  return true;
} /* qtAdjustController */

/*------------------------------------ qtDisposeOfMovie ---*/
OSErr qtDisposeOfMovie
  (QtMoviePtr thisMovie)
{
  OSErr err = noErr;

  //TBD -- any other cleanup required for a movie element.
  if (thisMovie->fMovie)
  {
    DisposeMovie(thisMovie->fMovie);
    thisMovie->fMovie = NULL_PTR;
  }
  if (thisMovie->fMovieFileValid)
  {
    err = CloseMovieFile(thisMovie->fResRef);
    if (err != noErr)
      LOG_ERROR_3(OUTPUT_PREFIX "problem closing file '%s' -> error (%ld)",
                  thisMovie->fName->s_name, long(err))
  }
  return err;
} /* qtDisposeOfMovie */

/*------------------------------------ qtFreeController ---*/
static bool qtFreeController
  (QtPtr xx)
{
#if defined(TRACE_CONTROLLER)
  LOG_POST_1("free controller")
#endif /* TRACE_CONTROLLER */
  if (! xx->fController)
    return false;

  if (NOGOOD(xx->fController))
  {
    LOG_ERROR_1(OUTPUT_PREFIX "free/controller corrupt")
    xx->fController = NULL_PTR;
    return false;

  }
  if (! xx->fControllerMethods)
    xx->fControllerMethods =
              reinterpret_cast<QtMethod>(egetfn(reinterpret_cast<PObject>(xx->fController),
                                                gCtrlSymbol));
  (*xx->fControllerMethods) (xx->fController, CTRL_MOVIE_FREED, NULL_PTR);
  return true;
} /* qtFreeController */

/*------------------------------------ qtInit ---*/
bool qtInit
  (void)
{
  bool    okSoFar = true;
  PSymbol checkGlobal = gensym(QT_CONTROL_SYMBOL);

  /* Determine if EnterMovies() has been called successfully in some other */
  /* object related to this one. */
  if (checkGlobal->s_thing != reinterpret_cast<PObject>(checkGlobal))
  {
    OSErr err = EnterMovies();

    if (err == noErr)
    {
      gInited = true;
      checkGlobal->s_thing = reinterpret_cast<PObject>(checkGlobal);
      gAnchor = static_cast<QtPtr>(newobject(gClass));
      gAnchor->fNextBlock = gAnchor->fPreviousBlock = NULL_PTR;
      gMovieTask = static_cast<PQelem>(qelem_new(gAnchor,
                                            reinterpret_cast<method>(ProcessMovieQueue)));
      qelem_set(gMovieTask);
    }
    else
    {
      LOG_ERROR_2(OUTPUT_PREFIX "unable to initialize movie toolbox -> error (%ld)",
                  long(err))
      okSoFar = false;
    }
  }
  return okSoFar;
} /* qtInit */

/*------------------------------------ qtLocateMovie ---*/
QtMovieHdl qtLocateMovie
  (QtPtr   xx,
   PSymbol aName)
{
  QtMovieHdl aMovie = reinterpret_cast<QtMovieHdl>(newhandle(sizeof(QtMovie)));
  QtMoviePtr thisMovie;
  char       tempName[MAX_FILE_NAME + 2];    /* allow for length or null terminator */
  short      volRefNum;
  bool       okSoFar = true;
  OSErr      err = noErr;

  HLock(reinterpret_cast<Handle>(aMovie));
  thisMovie = *aMovie;
  thisMovie->fMovieFileValid = false;
  if (aName == gEmptySymbol)
  {
#if defined(COMPILE_FOR_OSX_4)
    long  aType[1];
    long  dummy;

    *aType = long('MooV');
    if (! open_dialog(tempName, &volRefNum, &dummy, aType, 1))
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
    SFTypeList  aType;
    OSType      dummy;

    *aType = long('MooV');
    if (! open_dialog(tempName, &volRefNum, &dummy, aType, 1))
#endif /* COMPILE_FOR_OS9_4 */
      qtMapSymbolToLowerCase(tempName, tempName);
    else
      okSoFar = false;
  }
  else
  {
    qtMapSymbolToLowerCase(tempName, aName->s_name);
    /* Attempt to find the movie file. */
    if (locatefiletype(tempName, &volRefNum, long('MooV'), 0L))
    {
      LOG_ERROR_2(OUTPUT_PREFIX "unable to find file '%s'", tempName)
      okSoFar = false;
    }
  }
  if (okSoFar)
  {
    /* Convert the file name into a Pascal string. */
#if defined(COMPILE_FOR_OSX_4)
    Str255  tempString;
    
    thisMovie->fName = gensym(tempName);
    CopyCStringToPascal(tempName, tempString);
    err = FSMakeFSSpec(volRefNum, 0L, tempString, &thisMovie->fFileSpec);
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
    thisMovie->fName = gensym(tempName);
    C2PStr(static_cast<Ptr>(tempName));
    err = FSMakeFSSpec(volRefNum, 0L,
                    reinterpret_cast<ConstStr255Param>(const_cast<Qchar>(tempName)),
                      &thisMovie->fFileSpec);
    P2CStr(reinterpret_cast<Puchar>(tempName));
#endif /* COMPILE_FOR_OS9_4 */
    if (err != noErr)
    {
      LOG_ERROR_3(OUTPUT_PREFIX "problem building file spec for file '%s' -> error (%ld)", tempName,
                  long(err))
      okSoFar = false;
    }
  }
  if (okSoFar)
  {
    /* Attempt to open the movie file. */
    err = OpenMovieFile(&thisMovie->fFileSpec, &thisMovie->fResRef, fsRdPerm);
    if (err != noErr)
    {
      LOG_ERROR_3(OUTPUT_PREFIX "problem opening file '%s' -> error (%ld)",
                  thisMovie->fName->s_name, long(err))
      okSoFar = false;
    }
  }
  if (okSoFar)
  {
    thisMovie->fMovieFileValid = true;
    ++xx->fMovieCount;
    thisMovie->fNextMovie = NULL_HDL;
    thisMovie->fPreviousMovie = xx->fLastMovie;
    (*xx->fLastMovie)->fNextMovie = aMovie;
    xx->fLastMovie = aMovie;
    if (! xx->fFirstMovie)
      xx->fFirstMovie = aMovie;
  }
  HUnlock(reinterpret_cast<Handle>(aMovie));
  if (! okSoFar)
  {
    disposhandle(reinterpret_cast<Handle>(aMovie));
    aMovie = NULL_HDL;
  }
  return aMovie;
} /* qtLocateMovie */

/*------------------------------------ qtMapSymbolToLowerCase ---*/
static void qtMapSymbolToLowerCase
  (Ptr outName,
   Ptr inName)
{
  Ptr nextChar = inName;

  /* Get the lower-case version of the filename. */
  strncpy(outName, inName, MAX_FILE_NAME);
  /* Note that using tolower() requires the inclusion of <ctype.h>, which */
  /* brings in <stdio.h> (!?!?!), which has conflicts with the Max file */
  /* routines. */
  for ( ; *nextChar; ++nextChar)
  {
    if ((*nextChar >= 'A') && (*nextChar <= 'Z'))
      *nextChar = char(*nextChar + ('a' - 'A'));
  }
} /* qtMapSymbolToLowerCase */

#if defined(BQT_H_)
/*------------------------------------ qtMoveController ---*/
static bool qtMoveController
  (QtPtr xx)
{
 #if defined(TRACE_CONTROLLER)
  LOG_POST_1("move controller")
 #endif /* TRACE_CONTROLLER */
  if (! xx->fController)
    return false;

  if (NOGOOD(xx->fController))
  {
    LOG_ERROR_1(OUTPUT_PREFIX "move/controller corrupt")
    xx->fController = NULL_PTR;
    return false;

  }  
  if (! xx->fControllerMethods)
    xx->fControllerMethods = reinterpret_cast<QtMethod>(egetfn(xx->fController,
                                                                gCtrlSymbol));
  (*xx->fControllerMethods) (xx->fController, CTRL_MOVIE_MOVED, &xx->fBox.b_rect);
  return true;
} /* qtMoveController */
#endif /* BQT_H_ */

/*------------------------------------ qtRemoveMovie ---*/
void qtRemoveMovie
  (QtPtr      xx,
   QtMovieHdl aMovie)
{
  QtMoviePtr thisMovie;

  HLock(reinterpret_cast<Handle>(aMovie));
  thisMovie = *aMovie;
  if (xx->fFirstMovie == aMovie)
    xx->fFirstMovie = thisMovie->fNextMovie;
  if (xx->fLastMovie == aMovie)
    xx->fLastMovie = thisMovie->fPreviousMovie;
  if (thisMovie->fNextMovie)
    (*(thisMovie->fNextMovie))->fPreviousMovie = thisMovie->fPreviousMovie;
  if (thisMovie->fPreviousMovie)
    (*(thisMovie->fPreviousMovie))->fNextMovie = thisMovie->fNextMovie;
  if (xx->fCurrentMovie == thisMovie->fMovie)
  {
    if (thisMovie->fNextMovie)
      qtSetCurrentMovie(xx, thisMovie->fNextMovie);
    else if (thisMovie->fPreviousMovie)
      qtSetCurrentMovie(xx, thisMovie->fPreviousMovie);
    else
    {
      qtSetCurrentMovie(xx, NULL_HDL);
#if defined(WQT_H_)
      wind_invis(static_cast<Pvoid>(xx->fWindow));
#endif /* WQT_H_ */
    }
  }
  --xx->fMovieCount;
  if (qtDisposeOfMovie(thisMovie) != noErr)
    SignalError(xx);
  HUnlock(reinterpret_cast<Handle>(aMovie));
  disposhandle(reinterpret_cast<Handle>(aMovie));
} /* qtRemoveMovie */

/*------------------------------------ qtSaveMovies ---*/
void qtSaveMovies
  (QtPtr xx,
   Pvoid ww)
{
  for (QtMovieHdl aMovie = xx->fFirstMovie, aNext; aMovie; aMovie = aNext)
  {
    aNext = (*aMovie)->fNextMovie;
    if (xx->fCurrentMovie != (*aMovie)->fMovie)
      binbuf_vinsert(ww, "sss", gSaveSymbol, gensym("load"), (*aMovie)->fName);
  }
} /* qtSaveMovies */

/*------------------------------------ qtSearchKnownMovies ---*/
QtMovieHdl qtSearchKnownMovies
  (QtPtr   xx,
   PSymbol aName)
{
  char       tempName[MAX_FILE_NAME + 2];
  PSymbol    aSymbol;
  QtMovieHdl aMovie = xx->fFirstMovie;

  if (aName == gEmptySymbol)
    return NULL_HDL;

  qtMapSymbolToLowerCase(tempName, aName->s_name);
  aSymbol = gensym(tempName);
  for ( ; aMovie; )
  {
    if ((*aMovie)->fName == aSymbol)
      break;

    aMovie = (*aMovie)->fNextMovie;
  }
  return aMovie;
} /* qtSearchKnownMovies */

/*------------------------------------ qtSetController ---*/
static bool qtSetController
  (QtPtr xx)
{
#if defined(TRACE_CONTROLLER)
  LOG_POST_1("set controller")
#endif /* TRACE_CONTROLLER */
  if (! xx->fController)
    return false;

  if (NOGOOD(xx->fController))
  {
    LOG_ERROR_1(OUTPUT_PREFIX "set/controller corrupt")
    xx->fController = NULL_PTR;
    return false;

  }    
  if (! xx->fControllerMethods)
    xx->fControllerMethods =
              reinterpret_cast<QtMethod>(egetfn(reinterpret_cast<PObject>(xx->fController),
                                                gCtrlSymbol));
  (*xx->fControllerMethods) (xx->fController, CTRL_MOVIE_NEW, long(xx->fCurrentMovie));
  return true;
} /* qtSetController */

/*------------------------------------ qtSetCurrentMovie ---*/
void qtSetCurrentMovie
  (QtPtr      xx,
   QtMovieHdl aMovie)
{
  if (aMovie)
  {
    QtMoviePtr thisMovie;

    HLock(reinterpret_cast<Handle>(aMovie));
    thisMovie = *aMovie;
    if (thisMovie->fName == gEmptySymbol)
    {
      if (xx->fCurrentMovie)
        qtFreeController(xx);
      xx->fCurrentMovie = NULL_PTR;
      xx->fCurrentName = NULL_PTR;
    }
    else if (xx->fCurrentMovie == thisMovie->fMovie)
    {
      xx->fStopSignalled = true;
      xx->fCurrentRate = gDefaultRate;
      xx->fBeginSegment = -1;
      xx->fSegmentDuration = 0;
      SetMovieActiveSegment(xx->fCurrentMovie, xx->fBeginSegment, xx->fSegmentDuration);
      qtAdjustController(xx);
    }
    else
    {
#if defined(WQT_H_)
      Rect    windowPosition;
      GrafPtr oldPort;
#endif /* WQT_H_ */

      xx->fStopSignalled = true;
      xx->fCurrentMovie = thisMovie->fMovie;
      xx->fCurrentName = thisMovie->fName;
      xx->fCurrentRate = gDefaultRate;
      xx->fCurrentDuration = thisMovie->fDuration;
      xx->fBeginSegment = -1;
      xx->fSegmentDuration = 0;
      SetMovieActiveSegment(xx->fCurrentMovie, xx->fBeginSegment, xx->fSegmentDuration);
      qtSetController(xx);
      outlet_int(xx->fDurationOut, long(xx->fCurrentDuration));
#if defined(BQT_H_)
      //TBD
#endif /* BQT_H_ */
#if defined(WQT_H_)
      GetMovieBox(thisMovie->fMovie, &windowPosition);
      OffsetRect(&windowPosition, WIND_INITIAL_LEFT, WIND_INITIAL_TOP);
      wind_invis(xx->fWindow);
      xx->fWindow->w_x1 = windowPosition.left;
      xx->fWindow->w_x2 = windowPosition.right;
      xx->fWindow->w_y1 = windowPosition.top;
      xx->fWindow->w_y2 = windowPosition.bottom;
      OffsetRect(&windowPosition, short(- windowPosition.left), short(- windowPosition.top));
      wind_settitle(xx->fWindow, thisMovie->fName->s_name, FALSE);
      wind_vis(xx->fWindow);
      oldPort = wind_setport(xx->fWindow);
      InvalRect(&windowPosition);
      SetPort(oldPort);
      SetMovieActive(thisMovie->fMovie, TRUE);
      UpdateMovie(thisMovie->fMovie);
      MoviesTask(thisMovie->fMovie, 0);
#endif /* WQT_H_ */
    }
    HUnlock(reinterpret_cast<Handle>(aMovie));
  }
  else
  {
    if (xx->fCurrentMovie)
      qtFreeController(xx);
    xx->fCurrentMovie = NULL_PTR;
    xx->fCurrentName = NULL_PTR;
  }
} /* qtSetCurrentMovie */

/*------------------------------------ qtSetUpGlobals ---*/
void qtSetUpGlobals
  (Pchar				ourName,
   const short	resourceBase)
{
  gEmptySymbol = gensym("");
  gCtrlSymbol = gensym("ctrl");
  gOurSymbol = gensym(ourName);
  gSaveSymbol = gensym("#M"); /* used with binbuf */
  gInited = false;
  gDefaultRate = FixRatio(1, 1);
  gStoppedRate = FixRatio(0, 1);
  gResourceBase = resourceBase;
} /* qtSetUpGlobals */

/*------------------------------------ qtSetUpObject ---*/
void qtSetUpObject
  (QtPtr    xx,
   PPatcher pp)
{
  xx->fControllerMethods = NULL_PTR;
  xx->fController = NULL_PTR;
  xx->fPendingMovie = NULL_PTR;
  xx->fErrorBangOut = static_cast<POutlet>(bangout(xx));
  xx->fStoppedBangOut = static_cast<POutlet>(bangout(xx));
  xx->fDurationOut = static_cast<POutlet>(intout(xx));
  xx->fResultOut = static_cast<POutlet>(intout(xx));
  xx->fErrorQueue = static_cast<PQelem>(qelem_new(xx, reinterpret_cast<method>(ProcessErrorQueue)));
  xx->fStoppedQueue = static_cast<PQelem>(qelem_new(xx, reinterpret_cast<method>(ProcessStoppedQueue)));
  xx->fFirstMovie = xx->fLastMovie = NULL_HDL;
  xx->fPreviousBlock = xx->fNextBlock = NULL_PTR;
  xx->fWindow = NULL_PTR;
  xx->fCurrentMovie = NULL_PTR;
  xx->fCurrentName = NULL_PTR;
  xx->fPatcher = pp;
  xx->fMovieCount = 0;
} /* qtSetUpObject */
