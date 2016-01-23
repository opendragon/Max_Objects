/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       qtCommon.h                                                              */
/*                                                                                      */
/*  Contains:   The data type declarations and forward references to routines.          */
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

#if (! defined(QTCOMMON_H_))
 #define QTCOMMON_H_ /* */ 
 
 #include "MissingAndExtra.h"
 #include "ext_wind.h"
 #include <movies.h>
 #include <FixMath.h>

 #define MAX_FILE_NAME     100 /* far more than should be needed */
 #define QT_CONTROL_SYMBOL "##qtcontrol##"
 #define OUTPUT_PREFIX     "qt: "
 
 #define MAX_MOVIE_VOLUME 255
 #define MIN_MOVIE_VOLUME (-255)

 #define SignalError(xx)   qelem_set(xx->fErrorQueue)
 #define SignalStopped(xx) qelem_set(xx->fStoppedQueue)
 
/* Values to be based to the controller via the 'ctrl' message: */
 #define CTRL_MOVIE_IDLE  1L
 #define CTRL_MOVIE_FREED 2L
 #define CTRL_MOVIE_MOVED 3L
 #define CTRL_MOVIE_NEW   4L
 
 /*#define  TRACE_MESSAGES   /* */
 /*#define  TRACE_CONTROLLER /* */

struct QtMovie
{
  Movie       fMovie;
  QtMovie * * fPreviousMovie;
  QtMovie * * fNextMovie;
  FSSpec      fFileSpec;
  TimeValue   fDuration;
  PSymbol     fName;
  short       fResRef;
  bool        fMovieFileValid;
}; /* QtMovie */

typedef QtMovie *    QtMoviePtr;
typedef QtMoviePtr * QtMovieHdl;

typedef void (*QtMethod) (Pvoid, long, long);

struct QtData
{
  union
  {
    Box    fBox;    /* BQT_H_ */
    Object fObject; /* WQT_H_ */
  } fVariant;
  POutlet     fDurationOut;
  POutlet     fResultOut;
  POutlet     fStoppedBangOut;
  POutlet     fErrorBangOut;
  PQelem      fErrorQueue;
  PQelem      fStoppedQueue;
  QtData *    fPreviousBlock;
  QtData *    fNextBlock;
  QtMovieHdl  fFirstMovie;
  QtMovieHdl  fLastMovie;
  PPatcher    fPatcher;
  Pvoid       fController;
  QtMethod    fControllerMethods;
  Movie       fCurrentMovie;
  Movie       fPendingMovie;
  PSymbol     fCurrentName;
  Fixed       fCurrentRate;
  TimeValue   fCurrentDuration;
  TimeValue   fBeginSegment;
  TimeValue   fSegmentDuration;
  int         fMovieCount;
  Rect        fOldBox;      /* BQT_H_ only */
  bool        fBorderState; /* BQT_H_ only */
  Wind *      fWindow;      /* WQT_H_ only */
  bool        fStopSignalled;
}; /* QtData */

typedef QtData * QtPtr;

static Pvoid
ProcessErrorQueue(QtPtr xx);

static Pvoid
ProcessMovieQueue(QtPtr xx);

static Pvoid
ProcessStoppedQueue(QtPtr xx);

QtMovieHdl
qtAddMovie(QtPtr      xx,
           QtMovieHdl aMovie);

bool
qtAdjustController(QtPtr xx);

OSErr
qtDisposeOfMovie(QtMoviePtr thisMovie);

static bool
qtFreeController(QtPtr xx);

bool
qtInit(void);

QtMovieHdl
qtLocateMovie(QtPtr   xx,
              PSymbol aName);

static void
qtMapSymbolToLowerCase(Ptr outName,
                       Ptr inName);

static bool
qtMoveController(QtPtr xx);

void
qtRemoveMovie(QtPtr      xx,
              QtMovieHdl aMovie);

void
qtSaveMovies(QtPtr xx,
             Pvoid ww);

QtMovieHdl
qtSearchKnownMovies(QtPtr   xx,
                    PSymbol aName);

static bool
qtSetController(QtPtr xx);

void qtSetCurrentMovie
  (QtPtr      xx,
   QtMovieHdl aMovie);

void qtSetUpGlobals(Pchar       ourName,
                    const short resourceBase);

void
qtSetUpObject(QtPtr    xx,
              PPatcher pp);

Pvoid
deferred_Load(QtPtr   xx,
              PSymbol name);

Pvoid
deferred_Unload(QtPtr   xx,
                PSymbol name);

Pvoid
cmd_Active(QtPtr xx,
           long  num);

Pvoid
cmd_Begin(QtPtr xx);

Pvoid
cmd_Controller(QtPtr   xx,
               PSymbol in,
               PSymbol out1,
               PSymbol out2);

Pvoid
cmd_Count(QtPtr xx);

Pvoid
cmd_Duration(QtPtr xx);

Pvoid
cmd_End(QtPtr xx);

Pvoid
cmd_Film(QtPtr   xx,
         PSymbol which);

Pvoid
cmd_GetRate(QtPtr xx);

Pvoid
cmd_GetVolume(QtPtr xx);

Pvoid
cmd_Load(QtPtr   xx,
         PSymbol name);

Pvoid
cmd_Mute(QtPtr xx,
         long  muting);

Pvoid
cmd_Pause(QtPtr xx);

Pvoid
cmd_Rate(QtPtr xx,
         long  numerator,
         long  denominator);

Pvoid
cmd_Resume(QtPtr xx);

Pvoid
cmd_Segment(QtPtr xx,
            long  begin,
            long  end);

Pvoid
cmd_Start(QtPtr xx);

Pvoid
cmd_Time(QtPtr xx);

Pvoid
cmd_Unload(QtPtr   xx,
           PSymbol name);

Pvoid
cmd_Update(QtPtr xx);

Pvoid
cmd_Volume(QtPtr xx,
           long  num);

StandardRoutineDeclarations(QtPtr)

mextern(bool)    gInited;       /* EnterMovie() has been called */
mextern(Fixed)   gDefaultRate;  /* Movie play rate */
mextern(Fixed)   gStoppedRate;  /* Movie play rate when stopped */
mextern(PQelem)  gMovieTask;    /* Queue for executing MovieTask() */
mextern(QtPtr)   gAnchor;       /* Linked list of qt objects */
mextern(PSymbol) gCtrlSymbol;   /* Pointer to unique Symbol for 'ctrl' */
mextern(PSymbol) gEmptySymbol;  /* Pointer to unique Symbol for '' */
mextern(PSymbol) gOurSymbol;    /* Pointer to unique Symbol matching our name */
mextern(short)   gResourceBase; /* Base value for resources */
mextern(PSymbol) gSaveSymbol;   /* Pointer to unique Symbol for '#M' */

#endif /* not QTCOMMON_H_ */
