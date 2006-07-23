/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       pfsmLoadFile.c                                                          */
/*                                                                                      */
/*  Contains:   The low-level support routines for the pfsm module.                     */
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
/*  Created:    2000/06/05                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "pfsm.h"

/*------------------------------------ pfsmClearHashTable ---*/
void pfsmClearHashTable
  (PfsmPtr xx)
{
  if (xx && xx->fStateSymbols)
  {
    for (short ii = 0; ii < HASH_TABLE_SIZE; ++ii)
    {
      for (SymbolLinkPtr slot = *(xx->fStateSymbols + ii), next; slot; slot = next)
      {
        next = slot->fNext;
        for (TransitionPtr walker = slot->fTransitions, trans; walker; walker = trans)
        {
          trans = walker->fNext;
          FREEBYTES(walker->fOutput, walker->fOutputCount)
          FREEBYTES(walker, 1)
        }
        FREEBYTES(slot, 1)
      }
    }
    FREEBYTES(xx->fStateSymbols, HASH_TABLE_SIZE)
  }
} /* pfsmClearHashTable */

/*------------------------------------ pfsmReportHashTable ---*/
void pfsmReportHashTable
  (PfsmPtr xx)
{
  if (xx && xx->fStateSymbols)
  {
    Pchar	stateDescriptor;
    Qchar	outputTag;

    for (short ii = 0; ii < HASH_TABLE_SIZE; ++ii)
    {
      for (SymbolLinkPtr slot = *(xx->fStateSymbols + ii); slot; slot = slot->fNext)
      {
        if (slot->fIsError)
          stateDescriptor = OUTPUT_PREFIX " error state %s ";
        else if (slot->fIsStop)
          stateDescriptor = OUTPUT_PREFIX " stop state %s ";
        else if (slot == xx->fStartState)
          stateDescriptor = OUTPUT_PREFIX " start state %s ";
        else
          stateDescriptor = OUTPUT_PREFIX " state %s ";
        LOG_POST_2(stateDescriptor, slot->fSymbol->s_name)
        for (TransitionPtr trans = slot->fTransitions; trans; trans = trans->fNext)
        {
          Pchar nextName = trans->fNextState->fSymbol->s_name;

          outputTag = (trans->fOutputCount ? "with output:" : "");
          switch (trans->fKind)
          {
            case MatchAnyNumber:
              LOG_POST_3(OUTPUT_PREFIX "  any_number -> %s %s", nextName, outputTag)
              break;

            case MatchAnySymbol:
              LOG_POST_3(OUTPUT_PREFIX "  any_string -> %s %s", nextName, outputTag)
              break;

            case MatchNumber:
              LOG_POST_4(OUTPUT_PREFIX "  %ld -> %s %s", trans->fMatch.fAsNumber,
                        nextName, outputTag)
              break;

            case MatchNumberRange:
              if (trans->fMatch.fAsRange.fLowerIsOpen)
              {
                if (trans->fMatch.fAsRange.fUpperIsOpen)
                  LOG_POST_3(OUTPUT_PREFIX "  range any_number ... any_number -> %s %s",
                            nextName, outputTag)
                else
                  LOG_POST_4(OUTPUT_PREFIX "  range any_number ... %ld -> %s %s",
                            trans->fMatch.fAsRange.fUpper, nextName, outputTag)
              }
              else if (trans->fMatch.fAsRange.fUpperIsOpen)
                LOG_POST_4(OUTPUT_PREFIX "  range %ld ... any_number -> %s %s",
                          trans->fMatch.fAsRange.fLower, nextName, outputTag)
              else
                LOG_POST_5(OUTPUT_PREFIX "  range %ld ... %ld -> %s %s",
                          trans->fMatch.fAsRange.fLower, trans->fMatch.fAsRange.fUpper,
                          nextName, outputTag)
              break;

            case MatchSymbol:
              LOG_POST_4(OUTPUT_PREFIX "  %s -> %s %s", trans->fMatch.fAsSymbol->s_name,
                        nextName, outputTag)
              break;

            case MatchSpecial:
              LOG_POST_4(OUTPUT_PREFIX "  %c -> %s %s", trans->fMatch.fAsSpecial,
                        nextName, outputTag)
              break;

          }
          if (trans->fOutputCount)
          {
            for (short jj = 0; jj < trans->fOutputCount; ++jj)
              postatom(trans->fOutput + jj);
          }
          if (trans->fIsRandom)
            LOG_POST_2(OUTPUT_PREFIX "    with probability %g", trans->fProbability)
        }
      }
    }
  }
} /* pfsmReportHashTable */

/*------------------------------------ pfsmInitializeHashTable ---*/
static void pfsmInitializeHashTable
  (PfsmPtr xx)
{
  pfsmClearHashTable(xx);
  xx->fStateSymbols = GETBYTES(HASH_TABLE_SIZE, SymbolLinkPtr);
  if (xx->fStateSymbols)
  {
    for (short ii = 0; ii < HASH_TABLE_SIZE; ++ii)
      *(xx->fStateSymbols + ii) = NULL_PTR;
  }
} /* pfsmInitializeHashTable */

/*------------------------------------ pfsmAddStateSymbol ---*/
static SymbolLinkPtr pfsmAddStateSymbol
  (PfsmPtr xx,
   PSymbol name)
{
  short         ii = short(long(name) % HASH_TABLE_SIZE);
  SymbolLinkPtr prev = NULL_PTR, slot = *(xx->fStateSymbols + ii);

  for ( ; slot; slot = slot->fNext)
  {
    if (slot->fSymbol == name)
      break;

    prev = slot;
  }
  /* Note that we should never have duplicates, so we just use this as */
  /* a safety valve */
  if (! slot)
  {
    slot = GETBYTES(1, SymbolLink);
    slot->fNext = NULL_PTR;
    slot->fSymbol = name;
    slot->fTransitions = NULL_PTR;
    slot->fIsError = false;
    slot->fIsStop = false;
    if (prev)
      prev->fNext = slot;
    else
      *(xx->fStateSymbols + ii) = slot;
  }
  return slot;
} /* pfsmAddStatePSymbol/

/*------------------------------------ pfsmLookupStateSymbol ---*/
SymbolLinkPtr pfsmLookupStateSymbol
  (PfsmPtr xx,
   PSymbol name)
{
  short         ii = short(long(name) % HASH_TABLE_SIZE);
  SymbolLinkPtr slot = *(xx->fStateSymbols + ii);

  for ( ; slot; slot = slot->fNext)
  {
    if (slot->fSymbol == name)
      break;

  }
  return slot;
} /* pfsmLookupStatePSymbol/

/*------------------------------------ pfsmGetNextAtom ---*/
static bool pfsmGetNextAtom
  (PfsmPtr xx,
   PAtom   value)
{
  Atom skipper;
  bool result = true;

  for ( ; result; )
  {
    result = (! binbuf_getatom(xx->fBuffer, &xx->fBufferTypeOffset,
                                &xx->fBufferStuffOffset, value));
    if (result && (value->a_type == A_SYM))
    {
      if (value->a_w.w_sym->s_name[0] == kCommentCharacter)
      {
        /* skip a comment */
        for ( ; result; )
        {
          result = (! binbuf_getatom(xx->fBuffer, &xx->fBufferTypeOffset,
                                    &xx->fBufferStuffOffset, &skipper));
          if (skipper.a_type == A_SEMI)
            break;

        }
        /* At this point, we've read the trailing semicolon, so we need to get the next */
        /* atom. */
      }
      else
        /* We've seen a symbol other than '#' */
        break;

    }
    else
      /* We've seen something other than a symbol, so we can leave... */
      break;

  }
  return result;
} /* pfsmGetNextPAtom/

/*------------------------------------ pfsmReportUnexpected ---*/
static void pfsmReportUnexpected
  (PAtom what)
{
  switch (what->a_type)
  {
    case A_FLOAT:
      LOG_ERROR_2(OUTPUT_PREFIX "unexpected floating point (%g)",
                  double(what->a_w.w_float))
      break;

    case A_LONG:
      LOG_ERROR_2(OUTPUT_PREFIX "unexpected long (%ld)", what->a_w.w_long)
      break;

    case A_SYM:
      LOG_ERROR_2(OUTPUT_PREFIX "unexpected symbol (%s)", what->a_w.w_sym->s_name)
      break;

    case A_SEMI:
      LOG_ERROR_1(OUTPUT_PREFIX "unexpected semicolon")
      break;

    case A_COMMA:
      LOG_ERROR_1(OUTPUT_PREFIX "unexpected comma")
      break;

    case A_DOLLAR:
      LOG_ERROR_1(OUTPUT_PREFIX "unexpected dollar")
      break;

    default:
      LOG_ERROR_2(OUTPUT_PREFIX "unexpected atom, type=%hd", what->a_type)
      break;

  }
} /* pfsmReportUnexpected */

/*------------------------------------ pfsmGetStateSymbols ---*/
static bool pfsmGetStateSymbols
  (PfsmPtr xx)
{
  Atom holder;
  bool result = pfsmGetNextAtom(xx, &holder);

  if (result)
  {
    if ((holder.a_type == A_SYM) && (holder.a_w.w_sym == gOpenSquareSymbol))
      result = pfsmGetNextAtom(xx, &holder);
    else
    {
      pfsmReportUnexpected(&holder);
      result = false;
    }
  }
  for ( ; result; )
  {
    if (holder.a_type == A_SYM)
    {
      if (holder.a_w.w_sym == gCloseSquareSymbol)
      {
        /* last symbol seen, complete processing. */
        if (xx->fVerbose)
          LOG_POST_1(OUTPUT_PREFIX "read state symbols")
        break;

      }
      /* Add this symbol to the set of known symbols. */
      if (pfsmLookupStateSymbol(xx, holder.a_w.w_sym))
      {
        LOG_ERROR_2(OUTPUT_PREFIX "state symbol '%s' already present",
                    holder.a_w.w_sym->s_name)
        result = false;
      }
      else
      {
        pfsmAddStateSymbol(xx, holder.a_w.w_sym);
        result = pfsmGetNextAtom(xx, &holder);
      }
    }
    else
    {
      pfsmReportUnexpected(&holder);
      result = false;
    }
  }
  return result;
} /* pfsmGetStateSymbols */

/*------------------------------------ pfsmGetErrorSymbols ---*/
static bool pfsmGetErrorSymbols
  (PfsmPtr xx)
{
  Atom          holder;
  bool          result = pfsmGetNextAtom(xx, &holder);
  SymbolLinkPtr symbol;

  if (result)
  {
    if ((holder.a_type == A_SYM) && (holder.a_w.w_sym == gOpenSquareSymbol))
      result = pfsmGetNextAtom(xx, &holder);
    else
    {
      pfsmReportUnexpected(&holder);
      result = false;
    }
  }
  for ( ; result; )
  {
    if (holder.a_type == A_SYM)
    {
      if (holder.a_w.w_sym == gCloseSquareSymbol)
      {
        /* last symbol seen, complete processing. */
        if (xx->fVerbose)
          LOG_POST_1(OUTPUT_PREFIX "read error states")
        break;

      }
      /* Locate the symbol and update it's information. */
      symbol = pfsmLookupStateSymbol(xx, holder.a_w.w_sym);
      if (symbol)
      {
        symbol->fIsError = true;
        result = pfsmGetNextAtom(xx, &holder);
      }
      else
      {
        LOG_ERROR_2(OUTPUT_PREFIX "unknown error state symbol '%s'",
                    holder.a_w.w_sym->s_name)
        result = false;
      }
    }
    else
    {
      pfsmReportUnexpected(&holder);
      result = false;
    }
  }
  return result;
} /* pfsmGetErrorSymbols */

/*------------------------------------ pfsmGetStartSymbol ---*/
static bool pfsmGetStartSymbol
  (PfsmPtr xx)
{
  Atom          holder;
  bool          result = pfsmGetNextAtom(xx, &holder);
  SymbolLinkPtr symbol;

  if (result)
  {
    if (holder.a_type == A_SYM)
    {
      symbol = pfsmLookupStateSymbol(xx, holder.a_w.w_sym);
      if (symbol)
      {
        xx->fStartState = symbol;
        if (xx->fVerbose)
          LOG_POST_1(OUTPUT_PREFIX "read start state")
      }
      else
      {
        LOG_ERROR_2(OUTPUT_PREFIX "start state symbol '%s' not known",
                    holder.a_w.w_sym->s_name)
        result = false;
      }
    }
    else
    {
      pfsmReportUnexpected(&holder);
      result = false;
    }
  }
  return result;
} /* pfsmGetStartPSymbol/

/*------------------------------------ pfsmGetStopSymbols ---*/
static bool pfsmGetStopSymbols
  (PfsmPtr xx)
{
  Atom          holder;
  bool          result = pfsmGetNextAtom(xx, &holder);
  SymbolLinkPtr symbol;

  if (result)
  {
    if ((holder.a_type == A_SYM) && (holder.a_w.w_sym == gOpenSquareSymbol))
      result = pfsmGetNextAtom(xx, &holder);
    else
    {
      pfsmReportUnexpected(&holder);
      result = false;
    }
  }
  for ( ; result; )
  {
    if (holder.a_type == A_SYM)
    {
      if (holder.a_w.w_sym == gCloseSquareSymbol)
      {
        /* last symbol seen, complete processing. */
        if (xx->fVerbose)
          LOG_POST_1(OUTPUT_PREFIX "read stop states")
        break;

      }
      /* Locate the symbol and update it's information. */
      symbol = pfsmLookupStateSymbol(xx, holder.a_w.w_sym);
      if (symbol)
      {
        symbol->fIsStop = true;
        result = pfsmGetNextAtom(xx, &holder);
      }
      else
      {
        LOG_ERROR_2(OUTPUT_PREFIX "unknown stop state symbol '%s'",
                    holder.a_w.w_sym->s_name)
        result = false;
      }
    }
    else
    {
      pfsmReportUnexpected(&holder);
      result = false;
    }
  }
  return result;
} /* pfsmGetStopSymbols */

/*------------------------------------ pfsmGetNextNumber ---*/
static bool pfsmGetNextNumber
  (PfsmPtr	xx,
   long &		value,
   bool &		isOpenEnded)
{
  Atom holder;
  bool result = pfsmGetNextAtom(xx, &holder);

  if (result)
  {
    if (holder.a_type == A_LONG)
    {
      value = holder.a_w.w_long;
      isOpenEnded = false;
    }
    else if (holder.a_type == A_SYM)
    {
      if (holder.a_w.w_sym == gAnyNumberSymbol)
      {
        value = 0;
        isOpenEnded = true;
      }
      else
      {
        /* Check for syntactic sugar: 'x is the same as the ASCII for the given */
        /* character. */
        Qchar	matcher = holder.a_w.w_sym->s_name;

        if (*matcher == kSugarCharacter)
        {
          if (matcher[1] && (! matcher[2]))
          {
            value = matcher[1];
            isOpenEnded = false;
          }
          else
            result = false;
        }
        else
          result = false;
      }
    }
    else
      result = false;
    if (! result)
      pfsmReportUnexpected(&holder);
  }
  return result;
} /* pfsmGetNextNumber */

/*------------------------------------ pfsmCollectATransition ---*/
static bool pfsmCollectATransition
  (PfsmPtr xx)
{
  Atom          holder;
  SymbolLinkPtr inputSymbol = NULL_PTR, outputSymbol = NULL_PTR;
  float         probability;
  bool          isRandom = false, openLow, openHigh;
  Criteria      whatKind = MatchUnknown;
  PSymbol       whatSymbol = NULL_PTR;
  char          whatChar = '\0';
  long          whatValue = 0, lowValue, highValue;
  bool          result = pfsmGetNextAtom(xx, &holder);

  if (result)
  {
    if (holder.a_type == A_SYM)
    {
      inputSymbol = pfsmLookupStateSymbol(xx, holder.a_w.w_sym);
      if (inputSymbol)
        result = pfsmGetNextAtom(xx, &holder);
      else
      {
        LOG_ERROR_2(OUTPUT_PREFIX "input state symbol '%s' not known",
                    holder.a_w.w_sym->s_name)
        result = false;
      }
    }
    else
    {
      pfsmReportUnexpected(&holder);
      result = false;
    }
  }
  if (result)
  {
    /* Remember input data: */
    switch (holder.a_type)
    {
      case A_SYM:
        if (! strcmp(holder.a_w.w_sym->s_name, "$"))
        {
          whatKind = MatchSpecial;
          whatChar = '$';
        }
        else if (holder.a_w.w_sym == gAnyNumberSymbol)
          whatKind = MatchAnyNumber;
        else if (holder.a_w.w_sym == gAnySymbolSymbol)
          whatKind = MatchAnySymbol;
        else if (holder.a_w.w_sym == gRangeSymbol)
        {
          if (pfsmGetNextNumber(xx, lowValue, openLow) &&
              pfsmGetNextNumber(xx, highValue, openHigh))
            whatKind = MatchNumberRange;
          else
          {
            LOG_ERROR_1(OUTPUT_PREFIX "invalid range specification seen")
            result = false;
          }
        }
        else
        {
          /* Check for syntactic sugar: 'x is the same as the ASCII for the given */
          /* character. */
          Qchar	matcher = holder.a_w.w_sym->s_name;

          whatKind = MatchSymbol;
          whatSymbol = holder.a_w.w_sym;
          if (*matcher == kSugarCharacter)
          {
            if (matcher[1] && (! matcher[2]))
            {
              whatKind = MatchNumber;
              whatValue = matcher[1];
            }
          }
        }
        break;

      case A_LONG:
        whatKind = MatchNumber;
        whatValue = holder.a_w.w_long;
        break;

      case A_SEMI:
        whatKind = MatchSpecial;
        whatChar = ';';
        break;

      case A_COMMA:
        whatKind = MatchSpecial;
        whatChar = ',';
        break;

      case A_DOLLAR:
        whatKind = MatchSpecial;
        whatChar = '$';
        break;

      default:
        pfsmReportUnexpected(&holder);
        result = false;
        break;

    }
    if (result)
      result = pfsmGetNextAtom(xx, &holder);
  }
  if (result)
  {
    if (holder.a_type == A_SYM)
    {
      if ((holder.a_w.w_sym != gTransitionSymbol) &&
          (holder.a_w.w_sym != gRandomMoveSymbol))
      {
        pfsmReportUnexpected(&holder);
        result = false;
      }
      if (result && (holder.a_w.w_sym == gRandomMoveSymbol))
      {
        isRandom = true;
        result = pfsmGetNextAtom(xx, &holder);
        if (holder.a_type == A_FLOAT)
        {
          probability = holder.a_w.w_float;
          if ((probability <= 0) || (probability >= 1))
          {
            pfsmReportUnexpected(&holder);
            result = false;
          }
        }
        else
        {
          pfsmReportUnexpected(&holder);
          result = false;                             
        }
      }
      if (result)
        result = pfsmGetNextAtom(xx, &holder);
    }
    else
    {
      pfsmReportUnexpected(&holder);
      result = false;
    }
  }
  if (result)
  {
    if (holder.a_type == A_SYM)
    {
      outputSymbol = pfsmLookupStateSymbol(xx, holder.a_w.w_sym);
      if (outputSymbol)
        result = pfsmGetNextAtom(xx, &holder);
      else
      {
        LOG_ERROR_2(OUTPUT_PREFIX "output state symbol '%s' not known",
                    holder.a_w.w_sym->s_name)
        result = false;
      }
    }
    else
    {
      pfsmReportUnexpected(&holder);
      result = false;
    }
  }
  if (result)
  {
    TransitionPtr prevTrans = inputSymbol->fTransitions;
    TransitionPtr newTrans = prevTrans;

    for ( ; newTrans; newTrans = newTrans->fNext)
    {
      if (newTrans)
        prevTrans = newTrans;
    }
    newTrans = GETBYTES(1, TransitionData);
    if (newTrans)
    {
      PBinbuf collector = static_cast<PBinbuf>(binbuf_new());

      newTrans->fNext = NULL_PTR;
      newTrans->fNextState = outputSymbol;
      newTrans->fIsRandom = isRandom;
      newTrans->fKind = whatKind;
      switch (whatKind)
      {
        case MatchNumber:
          newTrans->fMatch.fAsNumber = whatValue;
          break;

        case MatchNumberRange:
          newTrans->fMatch.fAsRange.fLower = lowValue;
          newTrans->fMatch.fAsRange.fUpper = highValue;
          newTrans->fMatch.fAsRange.fLowerIsOpen = openLow;
          newTrans->fMatch.fAsRange.fUpperIsOpen = openHigh;
          break;

        case MatchSymbol:
          newTrans->fMatch.fAsSymbol = whatSymbol;
          break;

        case MatchSpecial:
          newTrans->fMatch.fAsSpecial = whatChar;
          break;

      }
      newTrans->fProbability = probability;
      newTrans->fOutputCount = newTrans->fDollarCount = newTrans->fDoubleDollarCount = 
            newTrans->fDollarStarCount = 0;
      /* Collect the output values, terminated by a semicolon: */
      for ( ; holder.a_type != A_SEMI; )
      {
        /* We have a value; attach it. */
        binbuf_append(collector, NULL_PTR, 1, &holder);
        ++newTrans->fOutputCount;
        /* Note that the '$' symbols are handled in the transition engine. */
        result = pfsmGetNextAtom(xx, &holder);
        if (! result)
          break;

      }
      /* Assemble the pieces into a rule to be added to the transition tables. */
      if (newTrans->fOutputCount)
      {
        long  tyOffset = 0, stOffset = 0;
        PAtom vector = GETBYTES(newTrans->fOutputCount, Atom);

        newTrans->fOutput = vector;
        for (short ii = 0; ii < newTrans->fOutputCount; ++ii, ++vector)
        {
          if (binbuf_getatom(collector, &tyOffset, &stOffset, vector))
            break;

          if (vector->a_type == A_SYM)
          {
            if (vector->a_w.w_sym == gDollarSymbol)
              ++newTrans->fDollarCount;
            else if (vector->a_w.w_sym == gDoubleDollarSymbol)
              ++newTrans->fDoubleDollarCount;
            else if (vector->a_w.w_sym == gDollarStarSymbol)
              ++newTrans->fDollarStarCount;
          }
          else if (vector->a_type == A_DOLLAR)
            ++newTrans->fDollarCount;
        }
      }
      else
        newTrans->fOutput = NULL_PTR;
      freeobject(reinterpret_cast<PObject>(collector));
      /* Add the new transition to the input symbol. */
      if (prevTrans)
        prevTrans->fNext = newTrans;
      else
        inputSymbol->fTransitions = newTrans;
    }
  }
  return result;
} /* pfsmCollectATransition */

/*------------------------------------ pfsmLoadTables ---*/
bool pfsmLoadTables
  (PfsmPtr xx,
   PSymbol filename)
{
  bool  result = false;
  short vol = 0;

  if (locatefiletype(filename->s_name, &vol, long('TEXT'), 0L))
    LOG_ERROR_2(OUTPUT_PREFIX "problem locating file '%s'", filename->s_name)
  else if (binbuf_read(xx->fBuffer, filename->s_name, vol, 0))
    LOG_ERROR_2(OUTPUT_PREFIX "problem opening and reading '%s'", filename->s_name)
  else
  {
    pfsmInitializeHashTable(xx);
    xx->fBufferTypeOffset = 0;
    xx->fBufferStuffOffset = 0;
    if (pfsmGetStateSymbols(xx) && pfsmGetStartSymbol(xx) && pfsmGetStopSymbols(xx) &&
        pfsmGetErrorSymbols(xx))
    {
      for ( ; pfsmCollectATransition(xx); )
      {
        if (xx->fVerbose)
          LOG_POST_1(OUTPUT_PREFIX "read state transition")
      }
      result = true;
    }
  }
  return result;
} /* pfsmLoadTables */
