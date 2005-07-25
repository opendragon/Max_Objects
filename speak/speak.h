/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       speak.h                                                                 */
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
/*  Created:    2001/04/03                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(SPEAK_H_))
 #define SPEAK_H_ /* */
 
 /*#define USE_SYSLOG /* */

 #include "MissingAndExtra.h"
 #include <Gestalt.h>
 #include <SpeechSynthesis.h>

 #include "speakBuffers.h"
 
 #define OUR_NAME      "speak"
 #define OUR_RES_NUMB  17157
 #define OUTPUT_PREFIX "speak: "
 
 #define SPEAK_SAMPLE_RATE 100 /* sampling rate to determine if busy! */
 #define INITIAL_VOICE 1
 
struct SpeakData
{
  Object        fObject;
  SpeechChannel fSpeaker;
  DataBufferHdl fBuffer;
  VoiceSpec     fVoice;
  POutlet       fDoneOut;
  POutlet       fPausedOut;
  POutlet       fResultOut;
  POutlet       fErrorBangOut;
  PQelem        fDoneQueue;
  SpeechDoneUPP fCompletionRoutine;
  short         fVoiceNumber;
  bool          fActive;
  bool          fPaused;
  bool          fSpelling;
}; /* SpeakData */

typedef SpeakData * SpeakPtr;

Pvoid cmd_Continue
  (SpeakPtr xx);

Pvoid cmd_Pause
  (SpeakPtr xx);

Pvoid cmd_Pitch
  (SpeakPtr xx,
   double   newPitch);

Pvoid cmd_Pitchq
  (SpeakPtr xx);

Pvoid cmd_Rate
  (SpeakPtr xx,
   double   newRate);

Pvoid cmd_Rateq
  (SpeakPtr xx);

Pvoid cmd_Say
  (SpeakPtr xx,
   PSymbol  message,
   short    argc,
   PAtom    argv);

Pvoid cmd_Spell
  (SpeakPtr xx,
   PSymbol  onOff);

Pvoid cmd_Stop
  (SpeakPtr xx);

Pvoid cmd_Voice
  (SpeakPtr xx,
   long     newVoice);

Pvoid cmd_VoiceMax
  (SpeakPtr xx);

Pvoid cmd_Voiceq
  (SpeakPtr xx);

Pvoid cmd_Volume
  (SpeakPtr xx,
   double   newVolume);

Pvoid cmd_Volumeq
  (SpeakPtr xx);

pascal void speechComplete
  (SpeechChannel	chan,
   long						refCon);

void transmitBuffer
  (SpeakPtr xx);

StandardRoutineDeclarations(SpeakPtr)

mextern(PSymbol) gEmptySymbol;    /* Pointer to unique Symbol for '' */
mextern(PSymbol) gOffSymbol;      /* Pointer to unique Symbol for 'off' */
mextern(PSymbol) gOnSymbol;       /* Pointer to unique Symbol for 'on' */
mextern(PSymbol) gPitchSymbol;    /* Pointer to unique Symbol for 'pitch' */
mextern(PSymbol) gRateSymbol;     /* Pointer to unique Symbol for 'rate' */
mextern(PSymbol) gVoiceSymbol;    /* Pointer to unique Symbol for 'voice' */
mextern(PSymbol) gVoiceMaxSymbol; /* Pointer to unique Symbol for 'voicemax' */
mextern(PSymbol) gVolumeSymbol;   /* Pointer to unique Symbol for 'volume' */

#endif /* not SPEAK_H_ */
