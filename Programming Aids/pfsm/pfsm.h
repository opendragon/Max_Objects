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
# define PFSM_H_ /* */

# include "missingAndExtra.h"
# include "genericListOutput.h"

# define OUR_NAME      "pfsm"
// # define OUR_RES_NUMB  17136
# define OUTPUT_PREFIX "pfsm: "

# define kCommentCharacter '#'
# define kSugarCharacter   '\''

# define HASH_TABLE_SIZE 53 /* a small prime that's not too small */

enum Criteria
{
    MatchAnyNumber,
    MatchAnySymbol,
    MatchNumber,
    MatchNumberRange,
    MatchSymbol,
    MatchSpecial,
    MatchUnknown
}; // Criteria

struct TransitionData;

struct SymbolLink
{
    SymbolLink *     fNext;
    t_symbol *       fSymbol;
    TransitionData * fTransitions;
    bool             fIsError;
    bool             fIsStop;
}; // SymbolLink

struct RangeData
{
    long fLower;
    long fUpper;
    bool fLowerIsOpen;
    bool fUpperIsOpen;
}; // RangeData

union MatchData
{
    long       fAsNumber;
    t_symbol * fAsSymbol;
    char       fAsSpecial;
    RangeData  fAsRange;
}; // MatchData

struct TransitionData
{
    TransitionData * fNext;
    Criteria         fKind;
    SymbolLink *     fNextState;
    t_atom *            fOutput;
    short            fOutputCount;
    short            fDollarCount;
    short            fDoubleDollarCount;
    short            fDollarStarCount;
    float            fProbability;
    MatchData        fMatch;
    bool             fIsRandom;
}; // TransitionData

struct PfsmData
{
    t_object      fObject;
    t_outlet *    fResultOut;
    t_outlet *    fStopBangOut;
    t_outlet *    fErrorBangOut;
    SymbolLink *  fCurrentState;
    SymbolLink *  fStartState;
    t_binbuf *    fBuffer;
    SymbolLink ** fStateSymbols;
    long          fBufferTypeOffset;
    long          fBufferStuffOffset;
    bool          fAutoRestart;
    bool          fRunning;
    bool          fVerbose;
}; // PfsmData

void cmd_Autorestart(PfsmData * xx,
                     t_symbol * onOff);

CLEAR_HEADER(PfsmData);

void cmd_Describe(PfsmData * xx);

void cmd_Do(PfsmData * xx,
            t_symbol * message,
            short      argc,
            t_atom *   argv);

void cmd_Goto(PfsmData * xx,
              t_symbol * stateName);

LOAD_HEADER(PfsmData);

void cmd_Start(PfsmData * xx);

void cmd_Status(PfsmData * xx);

void cmd_Stop(PfsmData * xx);

void cmd_Trace(PfsmData * xx,
               t_symbol * onOff);

void pfsmClearHashTable(PfsmData * xx);

bool pfsmLoadTables(PfsmData * xx,
                    t_symbol * fileName);

SymbolLink * pfsmLookupStateSymbol(PfsmData * xx,
                                   t_symbol * name);

void pfsmProcessCommand(PfsmData *  xx,
                        t_atom *    command,
                        const short argc,
                        t_atom *    argv);

void pfsmProcessData(PfsmData *  xx,
                     t_atom *    input,
                     const short argc,
                     t_atom *    argv);

void pfsmReportHashTable(PfsmData * xx);

StandardRoutineDeclarations(PfsmData);

mextern(t_symbol *) gAnyNumberSymbol;    /* Pointer to unique symbol for '@n' */
mextern(t_symbol *) gAnySymbolSymbol;    /* Pointer to unique symbol for '@s' */
mextern(t_symbol *) gCloseSquareSymbol;  /* Pointer to unique symbol for ']' */
mextern(t_symbol *) gColonSymbol;        /* Pointer to unique symbol for ':' */
mextern(t_symbol *) gDollarStarSymbol;   /* Pointer to unique symbol for '$*' */
mextern(t_symbol *) gDollarSymbol;       /* Pointer to unique symbol for '$' */
mextern(t_symbol *) gDoubleDollarSymbol; /* Pointer to unique symbol for '$$' */
mextern(t_symbol *) gEmptySymbol;        /* Pointer to unique symbol for '' */
mextern(t_symbol *) gEqualSymbol;        /* Pointer to unique symbol for '=' */
mextern(t_symbol *) gErrorSymbol;        /* Pointer to unique symbol for 'error' */
mextern(t_symbol *) gOffSymbol;          /* Pointer to unique symbol for 'off' */
mextern(t_symbol *) gOnSymbol;           /* Pointer to unique symbol for 'on' */
mextern(t_symbol *) gOpenSquareSymbol;   /* Pointer to unique symbol for '[' */
mextern(t_symbol *) gRandomMoveSymbol;   /* Pointer to unique symbol for '-?' */
mextern(t_symbol *) gRangeSymbol;        /* Pointer to unique symbol for '@r' */
mextern(t_symbol *) gTransitionSymbol;   /* Pointer to unique symbol for '->' */

#endif /* not PFSM_H_ */
