/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       pfsmProcessing.c                                                        */
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
/*  Created:    2000/06/04                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "pfsm.h"
#include <math.h>

/*------------------------------------ pfsmPerformTransition ---*/
static void pfsmPerformTransition
  (PfsmPtr				xx,
   TransitionPtr	trans,
   PAtom					input,
   const short		argc,
   PAtom					argv)
{
  if (trans->fDollarCount || trans->fDoubleDollarCount || trans->fDollarStarCount)
  {
    short newSize;
    PAtom element = trans->fOutput, pile = NULL_PTR, target = NULL_PTR;

    /* We have a string of Atoms with one or more extras. */
    /* For each '$$', we add the number of additional elements, but remove one */
    /* Atom for the '$$' itself. '$' entries don't affect the count. */
    newSize = short(trans->fOutputCount + (trans->fDoubleDollarCount * (argc - 1)));
    if (newSize)
      target = pile = GETBYTES(newSize, Atom);
    for (short ii = 0; ii < trans->fOutputCount; ii++, element++)
    {
      if ((element->a_type == A_LONG) || (element->a_type == A_FLOAT))
        *target++ = *element;
      else if (element->a_type == A_SYM)
      {
        if (element->a_w.w_sym == gDollarSymbol)
          *target++ = *input;
        else if (element->a_w.w_sym == gDollarStarSymbol)
        {
          SETSYM(target, trans->fNextState->fSymbol);
          target++;
        }
        else if (element->a_w.w_sym == gDoubleDollarSymbol)
        {
          PAtom walker = argv;

          for (short jj = 0; jj < argc; jj++)
            *target++ = *walker++;
        }
        else
          *target++ = *element;
      }
      else if (element->a_type == A_DOLLAR)
        *target++ = *input;
      else
        *target++ = *element;
    }
    if (newSize && pile)
    {
      genericListOutput(xx->fResultOut, newSize, pile);
      FREEBYTES(pile, newSize)
    }
  }
  else if (trans->fOutputCount && trans->fOutput)
    /* We have a simple string of Atoms, so just blast them out. */
    genericListOutput(xx->fResultOut, trans->fOutputCount, trans->fOutput);
  xx->fCurrentState = trans->fNextState;
  if (xx->fCurrentState->fIsError)
    outlet_bang(xx->fStopBangOut);
  if (xx->fCurrentState->fIsStop)
  {
    outlet_bang(xx->fStopBangOut);
    if (xx->fAutoRestart)
    {
      if (xx->fVerbose)
        LOG_POST_1(OUTPUT_PREFIX "autorestarting")
      xx->fCurrentState = xx->fStartState;
    }
  }
  if (xx->fVerbose)
    LOG_POST_2(OUTPUT_PREFIX "transition to state '%s'",
                xx->fCurrentState->fSymbol->s_name)
} /* pfsmPerformTransition */

/*------------------------------------ pfsmCollectATransition ---*/
static bool pfsmCheckProbability
  (const bool		isRandom,
   const float	probability)
{
  double test;

  if (! isRandom)
    return true;

  test = fabs(Random() / 32767.0);
  return (test < probability);
} /* pfsmCheckProbability

/*------------------------------------ pfsmProcessData ---*/
void pfsmProcessData
  (PfsmPtr 			xx,
   PAtom   			input,
   const short	argc,
   PAtom   			argv)
{
  bool matched = false;

  if (xx->fRunning && xx->fCurrentState)
  {
    TransitionPtr trans = xx->fCurrentState->fTransitions;
    char          specialChar;

    if (trans)
    {
      short aType = input->a_type;

      if ((aType == A_SYM) && (input->a_w.w_sym == gDollarSymbol))
        aType = A_DOLLAR;
      switch (aType)
      {
        case A_FLOAT:
          LOG_ERROR_2(OUTPUT_PREFIX "invalid data of type 'float' seen (%g)",
                      double(input->a_w.w_float))
          break;

        case A_LONG:
          if (xx->fVerbose)
            LOG_POST_2(OUTPUT_PREFIX "long data=%ld", input->a_w.w_long)
          for ( ; trans && (! matched); trans = trans->fNext)
          {
            switch (trans->fKind)
            {
              case MatchAnyNumber:
                /* Wildcard match found. */
                matched = true;
                break;

              case MatchNumber:
                /* Direct match found. */
                matched = (trans->fMatch.fAsNumber == input->a_w.w_long);
                break;

              case MatchNumberRange:
                /* Range match found. */
                matched = ((trans->fMatch.fAsRange.fLowerIsOpen ||
                            (trans->fMatch.fAsRange.fLower <= input->a_w.w_long)) &&
                            (trans->fMatch.fAsRange.fUpperIsOpen ||
                            (trans->fMatch.fAsRange.fUpper >= input->a_w.w_long)));
                break;

            }
            if (matched)
            {
              if (pfsmCheckProbability(trans->fIsRandom, trans->fProbability))
              {
                pfsmPerformTransition(xx, trans, input, argc, argv);
                break;

              }
              matched = false;
            }
          }
          if (! trans)
            LOG_POST_1(OUTPUT_PREFIX "input seen with no matching transition")
          break;

        case A_SYM:
          if (xx->fVerbose)
            LOG_POST_2(OUTPUT_PREFIX "string data=%s", input->a_w.w_sym->s_name)
          for ( ; trans && (! matched); trans = trans->fNext)
          {
            switch (trans->fKind)
            {
              case MatchAnySymbol:
                /* Wildcard match found. */
                matched = true;
                break;

              case MatchSymbol:
                /* Direct match found. */
                matched = (trans->fMatch.fAsSymbol == input->a_w.w_sym);
                break;

            }
            if (matched)
            {
              if (pfsmCheckProbability(trans->fIsRandom, trans->fProbability))
              {
                pfsmPerformTransition(xx, trans, input, argc, argv);
                break;

              }
              matched = false;
            }
          }
          if (! trans)
            LOG_POST_1(OUTPUT_PREFIX "input seen with no matching transition")
          break;

        case A_SEMI:
        case A_COMMA:
        case A_DOLLAR:
          switch (aType)
          {
            case A_SEMI:
              specialChar = ';';
              break;

            case A_COMMA:
              specialChar = ',';
              break;

            case A_DOLLAR:
              specialChar = '$';
              break;

          }
          if (xx->fVerbose)
            LOG_POST_2(OUTPUT_PREFIX "character data=%c", specialChar)
          for ( ; trans && (! matched); trans = trans->fNext)
          {
            if (trans->fKind == MatchSpecial)
              matched = (trans->fMatch.fAsSpecial == specialChar);
            if (matched)
            {
              if (pfsmCheckProbability(trans->fIsRandom, trans->fProbability))
              {
                pfsmPerformTransition(xx, trans, input, argc, argv);
                break;

              }
              matched = false;
            }
          }
          if (! trans)
            LOG_POST_1(OUTPUT_PREFIX "input seen with no matching transition")
          break;

        default:
          LOG_ERROR_2(OUTPUT_PREFIX "input of an unknown type (%hd) seen", aType)
          break;

      }
    }
    else
      LOG_POST_2(OUTPUT_PREFIX "input seen for state '%s' with no output transitions",
                  xx->fCurrentState->fSymbol->s_name)
  }
  else
    LOG_ERROR_1(OUTPUT_PREFIX "data seen when not running")
  if (! matched)
  {
    outlet_bang(xx->fErrorBangOut);
    xx->fCurrentState = NULL_PTR;
  }
} /* pfsmProcessData */
