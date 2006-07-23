/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       listen.h                                                                */
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
/*  Created:    2001/04/08                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(LISTEN_H_))
 #define LISTEN_H_ /* */
 
 /*#define USE_SYSLOG /* */

 #include "MissingAndExtra.h"
 #include <SpeechRecognition.h>
 
 #if FOR_MAC_PPC
  #define SPEECH_RECOGNITION_SUPPORTED 1
 #else /* not FOR_MAC_PPC */
  #define SPEECH_RECOGNITION_SUPPORTED 0
 #endif /* not FOR_MAC_PPC */

 #define OUR_NAME      "listen"
 #define OUR_RES_NUMB  17148
 #define OUTPUT_PREFIX "listen: "

 #define HASH_TABLE_SIZE 53 /* a small prime that's not too small */

 #define kCommentCharacter '#'
 #define kSugarCharacter   '\''
 #define kOpenModelChar    '<'
 #define kCloseModelChar   '>'
 #define SR_CONTROL_SYMBOL "##srcontrol##" 

enum
{
  kSRVersionUsed = 0x0150
};

struct MatchData
{
  PAtom fData;
  short fCount;
}; /* MatchData */

typedef MatchData * MatchDataPtr;

typedef struct PathListData * PathListPtr;

struct ModelData
{
  ModelData *     fNext;
  PSymbol         fSymbol;
  bool            fDefined;
  bool            fReached;
 #if SPEECH_RECOGNITION_SUPPORTED
  SRLanguageModel fModel;
  MatchDataPtr    fMatch;
  PathListPtr     fPaths;
 #endif /* SPEECH_RECOGNITION_SUPPORTED */
}; /* ModelData */

typedef ModelData * ModelDataPtr;

typedef ModelDataPtr * ModelDataHdl;

struct ElementData
{
  ElementData * fNext;
  union
  {
    struct
    {
      ModelDataPtr   fDescription;
 #if SPEECH_RECOGNITION_SUPPORTED
      SRSpeechObject fModelRef;
 #endif /* SPEECH_RECOGNITION_SUPPORTED */
    } fModelElement;
    struct
    {
      PSymbol        fSymbol;
      MatchDataPtr   fMatch;
    } fWordElement;
  } fVar;
  bool          fIsModel;
  bool          fIsReference;
  bool          fOptional;
  bool          fRepeatable;
}; /* ElementData */

typedef ElementData * ElementPtr;

struct PathListData
{
  PathListPtr fNext;
  ElementPtr  fFirst;
  ElementPtr  fLast;
}; /* PathListData */

struct ListenData
{
  Object              fObject;
  POutlet             fErrorBangOut;
  POutlet             fResultOut;
  PQelem              fResultQueue;
  ModelDataHdl        fModels;
  PAtom               fPreviousResult;
  short               fPreviousCount;
  PBinbuf             fBuffer;
  long                fBufferTypeOffset;
  long                fBufferStuffOffset;
  Atom                fBufferPrevious;
  bool                fBufferPreviousDefined;
 #if SPEECH_RECOGNITION_SUPPORTED
  SRRecognizer        fRecognizer;
  SRRecognitionSystem fRecognitionSystem;
  SRRecognitionResult fRecognized;
  bool                fProvideFeedback;
  bool                fModelLoaded;
  bool                fStarted;
 #endif /* SPEECH_RECOGNITION_SUPPORTED */
}; /* ListenData */

typedef ListenData * ListenPtr;

void clearHashTable
  (ListenPtr xx);

void clearModelData
  (ListenPtr xx);

Pvoid cmd_Load
  (ListenPtr xx,
   PSymbol   fileName);

Pvoid cmd_Recognize
  (ListenPtr xx,
   PSymbol   message,
   short     argc,
   PAtom     argv);

Pvoid cmd_Start
  (ListenPtr xx);

Pvoid cmd_Status
  (ListenPtr xx);

Pvoid cmd_Stop
  (ListenPtr xx);

 #if SPEECH_RECOGNITION_SUPPORTED
void createLanguageModel
  (ListenPtr    xx,
   ModelDataPtr aModel);
 #endif /* SPEECH_RECOGNITION_SUPPORTED */

 #if SPEECH_RECOGNITION_SUPPORTED
SRPath createPath
  (ListenPtr   xx,
   PathListPtr aPath);
 #endif /* SPEECH_RECOGNITION_SUPPORTED */

void doLoadLanguageFile
  (ListenPtr xx,
   Pchar     fileName);

void initializeHashTable
  (ListenPtr xx);

ModelDataPtr lookupModel
  (ListenPtr xx,
   PSymbol   name);

void reportUnexpected
  (PAtom what);

bool symbolIsAModel
  (PSymbol aSymbol);

StandardRoutineDeclarations(ListenPtr)

mextern(PSymbol) gCloseCurlySymbol;  /* Pointer to unique Symbol for '}' */
mextern(PSymbol) gCloseSquareSymbol; /* Pointer to unique Symbol for ']' */
mextern(PSymbol) gEmptySymbol;       /* Pointer to unique Symbol for '' */
mextern(PSymbol) gEqualsSymbol;      /* Pointer to unique Symbol for '=' */
mextern(PSymbol) gIdleSymbol;        /* Pointer to unique Symbol for 'idle' */
mextern(PSymbol) gLoadedSymbol;      /* Pointer to unique Symbol for 'loaded' */
mextern(PSymbol) gLowerOSymbol;      /* Pointer to unique Symbol for 'o' */
mextern(PSymbol) gLowerRSymbol;      /* Pointer to unique Symbol for 'r' */
mextern(PSymbol) gNoSymbol;          /* Pointer to unique Symbol for 'no' */
mextern(PSymbol) gOpenCurlySymbol;   /* Pointer to unique Symbol for '{' */
mextern(PSymbol) gOpenSquareSymbol;  /* Pointer to unique Symbol for '[' */
mextern(PSymbol) gRejectedWordToken; /* Pointer to unique Symbol for '!?*$' */
mextern(PSymbol) gResultSymbol;      /* Pointer to unique Symbol for 'result' */
mextern(PSymbol) gStartedSymbol;     /* Pointer to unique Symbol for 'started' */
mextern(PSymbol) gStatusSymbol;      /* Pointer to unique Symbol for 'status' */
mextern(PSymbol) gTopLevelSymbol;    /* Pointer to unique Symbol for '<TopLevel>' */
mextern(PSymbol) gUpperOSymbol;      /* Pointer to unique Symbol for 'O' */
mextern(PSymbol) gUpperRSymbol;      /* Pointer to unique Symbol for 'R' */
mextern(PSymbol) gVertBarSymbol;     /* Pointer to unique Symbol for '|' */
mextern(PSymbol) gYesSymbol;         /* Pointer to unique Symbol for 'yes' */
mextern(OSType)  gSRVersionSelector minit('otvr');
mextern(UInt32)  gSRVersion;

#endif /* not LISTEN_H_ */
