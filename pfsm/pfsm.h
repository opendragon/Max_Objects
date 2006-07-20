/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       pfsm.h                                                                  */
/*                                                                                      */
/*  Contains:   The data type declarations and forward references to routines.          */
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
/*  Created:    2000/05/30                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(PFSM_H_))
 #define PFSM_H_ /* */
 
 /*#define USE_SYSLOG /* */

 #include "MissingAndExtra.h"
 #include "genericListOutput.h"

 #define OUR_NAME      "pfsm"
 #define OUR_RES_NUMB  17136
 #define OUTPUT_PREFIX "pfsm: "
 
 #define kCommentCharacter '#'
 #define kSugarCharacter   '\''

 #define HASH_TABLE_SIZE 53 /* a small prime that's not too small */

enum Criteria
{
  MatchAnyNumber,
  MatchAnySymbol,
  MatchNumber,
  MatchNumberRange,
  MatchSymbol,
  MatchSpecial,
  MatchUnknown
}; /* Criteria */

typedef struct TransitionData * TransitionPtr;

struct SymbolLink
{
  SymbolLink *  fNext;
  PSymbol       fSymbol;
  TransitionPtr fTransitions;
  bool          fIsError;
  bool          fIsStop;
}; /* SymbolLink */

typedef SymbolLink * SymbolLinkPtr;

typedef SymbolLinkPtr * SymbolLinkHdl;

struct RangeData
{
  long fLower;
  long fUpper;
  bool fLowerIsOpen;
  bool fUpperIsOpen;
}; /* RangeData */

union MatchData
{
  long      fAsNumber;
  PSymbol   fAsSymbol;
  char      fAsSpecial;
  RangeData fAsRange;
}; /* MatchData */

struct TransitionData
{
  TransitionData *  fNext;
  Criteria          fKind;
  SymbolLinkPtr     fNextState;
  PAtom             fOutput;
  short             fOutputCount;
  short             fDollarCount;
  short             fDoubleDollarCount;
  short             fDollarStarCount;
  float             fProbability;
  MatchData         fMatch;
  bool              fIsRandom;
}; /* TransitionData */

struct PfsmData
{
  Object        fObject;
  POutlet       fResultOut;
  POutlet       fStopBangOut;
  POutlet       fErrorBangOut;
  SymbolLinkPtr fCurrentState;
  SymbolLinkPtr fStartState;
  PBinbuf       fBuffer;
  SymbolLinkHdl fStateSymbols;
  long          fBufferTypeOffset;
  long          fBufferStuffOffset;
  bool          fAutoRestart;
  bool          fRunning;
  bool          fVerbose;
}; /* PfsmData */

typedef PfsmData * PfsmPtr;

Pvoid cmd_Autorestart
  (PfsmPtr xx,
   PSymbol onOff);

Pvoid cmd_Clear
  (PfsmPtr xx);

Pvoid cmd_Describe
  (PfsmPtr xx);

Pvoid cmd_Do
  (PfsmPtr xx,
   PSymbol message,
   short   argc,
   PAtom   argv);

Pvoid cmd_Goto
  (PfsmPtr xx,
   PSymbol stateName);

Pvoid cmd_Load
  (PfsmPtr xx,
   PSymbol fileName);

Pvoid cmd_Start
  (PfsmPtr xx);

Pvoid cmd_Status
  (PfsmPtr xx);

Pvoid cmd_Stop
  (PfsmPtr xx);

Pvoid cmd_Trace
  (PfsmPtr xx,
   PSymbol onOff);

void pfsmClearHashTable
  (PfsmPtr xx);

bool pfsmLoadTables
  (PfsmPtr xx,
   PSymbol filename);

SymbolLinkPtr pfsmLookupStateSymbol
  (PfsmPtr xx,
   PSymbol name);

void pfsmProcessCommand
  (PfsmPtr			xx,
   PAtom   			command,
   const short	argc,
   PAtom   			argv);

void pfsmProcessData
  (PfsmPtr 			xx,
   PAtom   			input,
   const short	argc,
   PAtom   			argv);

void pfsmReportHashTable
  (PfsmPtr xx);

StandardRoutineDeclarations(PfsmPtr)

mextern(PSymbol) gAnyNumberSymbol;    /* Pointer to unique Symbol for '@n' */
mextern(PSymbol) gAnySymbolSymbol;    /* Pointer to unique Symbol for '@s' */
mextern(PSymbol) gCloseSquareSymbol;  /* Pointer to unique Symbol for ']' */
mextern(PSymbol) gColonSymbol;        /* Pointer to unique Symbol for ':' */
mextern(PSymbol) gDollarStarSymbol;   /* Pointer to unique Symbol for '$*' */
mextern(PSymbol) gDollarSymbol;       /* Pointer to unique Symbol for '$' */
mextern(PSymbol) gDoubleDollarSymbol; /* Pointer to unique Symbol for '$$' */
mextern(PSymbol) gEmptySymbol;        /* Pointer to unique Symbol for '' */
mextern(PSymbol) gEqualSymbol;        /* Pointer to unique Symbol for '=' */
mextern(PSymbol) gErrorSymbol;        /* Pointer to unique Symbol for 'error' */
mextern(PSymbol) gOffSymbol;          /* Pointer to unique Symbol for 'off' */
mextern(PSymbol) gOnSymbol;           /* Pointer to unique Symbol for 'on' */
mextern(PSymbol) gOpenSquareSymbol;   /* Pointer to unique Symbol for '[' */
mextern(PSymbol) gRandomMoveSymbol;   /* Pointer to unique Symbol for '-?' */
mextern(PSymbol) gRangeSymbol;        /* Pointer to unique Symbol for '@r' */
mextern(PSymbol) gTransitionSymbol;   /* Pointer to unique Symbol for '->' */

#endif /* not PFSM_H_ */
