/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       mtcTrack.h                                                              */
/*                                                                                      */
/*  Contains:   The data type declarations and forward references to routines.          */
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
/*  Created:    2001/04/18                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(MTCTRACK_H_))
 #define MTCTRACK_H_  /* */
 
 /*#define USE_SYSLOG /* */

 #include "MissingAndExtra.h"
 #include "genericListOutput.h"

 #define OUR_NAME      "mtcTrack"
 #define OUR_RES_NUMB  17167
 #define OUTPUT_PREFIX "mtcTrack: "
 
 #define MAX_POINTS 10
 
struct MtcSampleData
{
  float   fThisP;
  float   fThisX;
  float   fThisY;
  Pdouble fActDistance;
  bool    fAvailable;
}; /* MtcSampleData */

typedef MtcSampleData * MtcSamplePtr;

struct MtcRetainedData
{
  float  fLastP;
  float  fLastX;
  float  fLastY;
  float  fDeltaX;
  float  fDeltaY;
  float  fNewP;
  float  fNewX;
  float  fNewY;
  double fVelocity;
  double fForce;
  bool   fFresh;
  bool   fValid;
}; /* MtcRetainedData */

typedef MtcRetainedData * MtcRetainedPtr;

struct MtcTrackData
{
  Object         fObject;
  POutlet        fResultOut;
  POutlet        fPointCountOut;
  POutlet        fErrorBangOut;
  POutlet        fLinesCompleteOut;
  MtcRetainedPtr fRetainedData;
  MtcSamplePtr   fSamples;
  Pdouble        fActDistances;
  double         fThreshold;
  long           fBatchNumber;
  short          fHowMany;
  short          fMaxSamples;
  short          fRetainedCount;
  short          fSampleCount;
  bool           fAddBatchNumber;
  bool           fAddIndex;
}; /* MtcTrackData */

typedef MtcTrackData * MtcTrackPtr;

Pvoid cmd_Batch
  (MtcTrackPtr xx,
   PSymbol     onOff);

Pvoid cmd_Clear
  (MtcTrackPtr xx);

Pvoid cmd_Index
  (MtcTrackPtr xx,
   PSymbol     onOff);

Pvoid cmd_Threshold
  (MtcTrackPtr xx,
   double      num);

StandardRoutineDeclarations(MtcTrackPtr)

mextern(PSymbol) gBatchSymbol; /* Pointer to unique symbol for 'batch' */
mextern(PSymbol) gEmptySymbol; /* Pointer to unique symbol for '' */
mextern(PSymbol) gIndexSymbol; /* Pointer to unique symbol for 'index' */
mextern(PSymbol) gOffSymbol;   /* Pointer to unique symbol for 'off' */
mextern(PSymbol) gOnSymbol;    /* Pointer to unique symbol for 'on' */

#endif /* not MTCTRACK_H_ */
