/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       listen_Recognize.c                                                      */
/*                                                                                      */
/*  Contains:   The routine cmd_Recgonize().                                            */
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
/*  Created:    2001/04/14                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "listen.h"

#define TRACE_ATOMS_SIMPLE 0

/*------------------------------------ traceAtomSimple ---*/
static void
traceAtomSimple(Qchar tag,
                PAtom value)
{
  switch (value->a_type)
  {
    case A_FLOAT:
      LOG_POST_3("%s(floating point: %g)", tag, double(value->a_w.w_float))
      break;

    case A_LONG:
      LOG_POST_3("%s(long: %ld)", tag, value->a_w.w_long)
      break;

    case A_SYM:
      LOG_POST_3("%s(symbol: '%s')", tag, value->a_w.w_sym->s_name)
      break;

    default:
      LOG_POST_3("%s(unexpected atom, type=%hd)", tag, value->a_type)
      break;

  }
} /* traceAtomSimple */

/*------------------------------------ getNextAtomSimple ---*/
static bool
getNextAtomSimple(const short argc,
                  PAtom       argv,
                  short &     nextArg,
                  PAtom       value)
{
#if SPEECH_RECOGNITION_SUPPORTED
  short ii = nextArg++;

  if (ii < argc)
  {
    *value = *(argv + ii);
    return true;

  }
  return false;
#else /* not SPEECH_RECOGNITION_SUPPORTED */
 #pragma unused(argc, argv, nextArg, value)
  return false;
#endif /* not SPEECH_RECOGNITION_SUPPORTED */
} /* getNextAtomSimple */

/*------------------------------------ pushBackAtomSimple ---*/
static bool
pushBackAtomSimple(short & nextArg)
{
#if SPEECH_RECOGNITION_SUPPORTED
  short ii = nextArg--;

  if (ii <= 0)
  {
    LOG_ERROR_1(OUTPUT_PREFIX "cannot push back atom")
    return false;

  }    
  return true;
#else /* not SPEECH_RECOGNITION_SUPPORTED */
 #pragma unused(nextArg)
  return false;
#endif /* not SPEECH_RECOGNITION_SUPPORTED */
} /* pushBackAtomSimple */

/*------------------------------------ getMatchDataSimple ---*/
static MatchDataPtr
getMatchDataSimple(const short argc,
                   PAtom       argv,
                   short &     nextArg)
{
#if SPEECH_RECOGNITION_SUPPORTED
  PBinbuf      tempBuffer = static_cast<PBinbuf>(binbuf_new());
  short        count = 0;
  MatchDataPtr matchData = NULL_PTR;
  bool         sawClose = false;

  do
  {
    Atom holder;
    bool result = getNextAtomSimple(argc, argv, nextArg, &holder);

    if (result)
    {
 #if TRACE_ATOMS_SIMPLE
      traceAtomSimple("getMatchDataSimple1", &holder);
 #endif /* TRACE_ATOMS_SIMPLE */
      if (holder.a_type == A_SYM)
      {
        if (holder.a_w.w_sym == gCloseCurlySymbol)
          sawClose = true;
      }
      if (! sawClose)
      {
        binbuf_append(tempBuffer, NULL_PTR, 1, &holder);
        ++count;
      }
    }
    else
    {
      LOG_ERROR_1(OUTPUT_PREFIX "missing '}'")
      count = 0;
      sawClose = true;
    }
  }
  while (! sawClose);
  // At this point, we have all the Atoms between '{' and '}' collected into the Binbuf.
  if (count)
  {
    matchData = GETBYTES(1, MatchData);
    if (matchData)
    {
      PAtom tempList = GETBYTES(count, Atom);

      if (tempList)
      {
        bool  okSoFar = true;
        long  stuffOffset = 0, typeOffset = 0;
        Atom  tempAtom;

        /* Prefill the atom vector, in case of early termination. */
        for (short ii = 0; ii < count; ++ii)
          SETLONG(tempList + ii, 0);
        for (short ii = 0; okSoFar && (ii < count); ++ii)
        {       
          okSoFar = (! binbuf_getatom(tempBuffer, &typeOffset, &stuffOffset, &tempAtom));
          if (okSoFar)
          {
 #if TRACE_ATOMS_SIMPLE
            traceAtomSimple("getMatchDataSimple2", &holder);
 #endif /* TRACE_ATOMS_SIMPLE */
            tempList[ii] = tempAtom;
          }
        }
        matchData->fCount = count;
        matchData->fData = tempList;
      }
      else
      {
        FREEBYTES(tempList, count)
        LOG_ERROR_1(OUTPUT_PREFIX "problem allocating space for list")
        FREEBYTES(matchData, 1)
      }
    }
    else
      LOG_ERROR_1(OUTPUT_PREFIX "problem allocating space for match data")
  }
  freeobject(reinterpret_cast<PObject>(tempBuffer));
  return matchData;
#else /* not SPEECH_RECOGNITION_SUPPORTED */
 #pragma unused(argc, argv, nextArg)
  return NULL_PTR;
#endif /* not SPEECH_RECOGNITION_SUPPORTED */
} /* getMatchDataSimple */

/*------------------------------------ getModifierListSimple ---*/
static bool
getModifierListSimple(const short argc,
                      PAtom       argv,
                      short &     nextArg,
                      bool &      optional,
                      bool &      repeatable)
{
#if SPEECH_RECOGNITION_SUPPORTED
  bool okSoFar = true, sawClose = false;

  optional = repeatable = false;
  do
  {
    Atom holder;
    bool result = getNextAtomSimple(argc, argv, nextArg, &holder);

    if (result)
    {
#if TRACE_ATOMS_SIMPLE
      traceAtomSimple("getModifierListSimple", &holder);
#endif /* TRACE_ATOMS_SIMPLE */
      if (holder.a_type == A_SYM)
      {
        if (holder.a_w.w_sym == gCloseSquareSymbol)
          sawClose = true;
        else if ((holder.a_w.w_sym == gLowerOSymbol) ||
                (holder.a_w.w_sym == gUpperOSymbol))
          optional = true;
        else if ((holder.a_w.w_sym == gLowerRSymbol) ||
                (holder.a_w.w_sym == gUpperRSymbol))
          repeatable = true;
        else
        {
          LOG_ERROR_2(OUTPUT_PREFIX "unrecognized option '%s'",
                      holder.a_w.w_sym->s_name)
          okSoFar = false;
        }
      }
      else
      {
        reportUnexpected(&holder);
        okSoFar = false;
      }
    }
    else
    {
      LOG_ERROR_1(OUTPUT_PREFIX "missing ']'")
      sawClose = true;
      okSoFar = false;
    }
  }
  while (! sawClose);
  return okSoFar;
#else /* not SPEECH_RECOGNITION_SUPPORTED */
 #pragma unused(argc, argv, nextArg, optional, repeatable)
  return false;
#endif /* not SPEECH_RECOGNITION_SUPPORTED */
} /* getModifierListSimple */

/*------------------------------------ getASimplePath ---*/
static PathListPtr
getASimplePath(ListenPtr   xx,
               bool &      lastPath,
               const short argc,
               PAtom       argv,
               short &     nextArg)
{
#if SPEECH_RECOGNITION_SUPPORTED
  bool moreStuff = true, okSoFar = true;

  if (xx)
  {
    PathListPtr aPath = GETBYTES(1, PathListData);

    lastPath = false;
    if (aPath)
    {
      aPath->fFirst = aPath->fLast = NULL_PTR;
      aPath->fNext = NULL_PTR;
      do
      {
        Atom holder;
        bool result = getNextAtomSimple(argc, argv, nextArg, &holder);

        if (result)
        {
 #if TRACE_ATOMS_SIMPLE
          traceAtomSimple("getASimplePath1", &holder);
 #endif /* TRACE_ATOMS_SIMPLE */
          if (holder.a_type == A_SYM)
          {
            if (holder.a_w.w_sym == gVertBarSymbol)
              /* Stop scanning - we've got the end of this path. */
              moreStuff = false;
            else
            {
              ElementPtr element = GETBYTES(1, ElementData);

              if (element)
              {
                element->fNext = NULL_PTR;
                element->fIsReference = element->fOptional = element->fRepeatable = false;
                if (symbolIsAModel(holder.a_w.w_sym))
                {
                  LOG_ERROR_1(OUTPUT_PREFIX "model names can only be used in loaded "
                              "files")
                  okSoFar = false;
                }
                else
                {
                  Atom holder2;

                  element->fIsModel = false;
                  element->fVar.fWordElement.fSymbol = holder.a_w.w_sym;
                  element->fVar.fWordElement.fMatch = NULL_PTR;
                  /* Check for a match list for this word. */
                  result = getNextAtomSimple(argc, argv, nextArg, &holder2);
                  if (result)
                  {
 #if TRACE_ATOMS_SIMPLE
                    traceAtomSimple("getASimplePath2", &holder2);
 #endif /* TRACE_ATOMS_SIMPLE */
                    if (holder2.a_type == A_SYM)
                    {
                      if (holder2.a_w.w_sym == gOpenCurlySymbol)
                        element->fVar.fWordElement.fMatch = getMatchDataSimple(argc, argv,
                                                                                nextArg);
                      else
                      {
                        if (! pushBackAtomSimple(nextArg))
                        {
                          LOG_ERROR_1(OUTPUT_PREFIX "could not push back input atom")
                          okSoFar = false;
                        }
                      }
                    }
                    else
                    {
                      reportUnexpected(&holder2);
                      okSoFar = false;
                    }
                  }
                  else
                  {
                    // We've got the end of this path and of all the paths for this model.
                    lastPath = true;
                    moreStuff = false;
                  }
                }
              }
              if (okSoFar)
              {
                Atom holder3;

                if (aPath->fLast)
                {
                  aPath->fLast->fNext = element;
                  aPath->fLast = element;
                }
                else
                  aPath->fFirst = aPath->fLast = element;
                /* Check for options for this element. */
                result = getNextAtomSimple(argc, argv, nextArg, &holder3);
                if (result)
                {
 #if TRACE_ATOMS_SIMPLE
                  traceAtomSimple("getASimplePath3", &holder3);
 #endif /* TRACE_ATOMS_SIMPLE */
                  if (holder3.a_type == A_SYM)
                  {
                    if (holder3.a_w.w_sym == gOpenSquareSymbol)
                      okSoFar = getModifierListSimple(argc, argv, nextArg,
                                                        element->fOptional,
                                                        element->fRepeatable);
                    else
                    {
                      if (! pushBackAtomSimple(nextArg))
                      {
                        LOG_ERROR_1(OUTPUT_PREFIX "could not push back input atom")
                        okSoFar = false;
                      }
                    }
                  }
                  else
                  {
                    reportUnexpected(&holder3);
                    okSoFar = false;
                  }
                }
                else
                {
                  // We've got the end of this path and of all the paths for this model.
                  lastPath = true;
                  moreStuff = false;
                }
              }
            }
          }
          else
          {
            reportUnexpected(&holder);
            okSoFar = false;
          }
        }
        else
        {
          /* We've got the end of this path and of all the paths for this model. */
          lastPath = true;
          moreStuff = false;
        }
      }
      while (moreStuff && okSoFar);
      if (! okSoFar)
      {
        /* Clean out aPath */
        for (ElementPtr eWalker = aPath->fFirst, element; eWalker; eWalker = element)
        {
          element = eWalker->fNext;
          if (! eWalker->fIsModel)
          {
            MatchDataPtr match = eWalker->fVar.fWordElement.fMatch;

            if (match)
            {
              FREEBYTES(match->fData, match->fCount)
              FREEBYTES(match, 1)
            }
          }
          FREEBYTES(eWalker, 1)
        }
        FREEBYTES(aPath, 1)
      }
    }
    else
      LOG_ERROR_1(OUTPUT_PREFIX "problem allocating space for path")
    return aPath;

  }
  return NULL_PTR;
#else /* not SPEECH_RECOGNITION_SUPPORTED */
 #pragma unused(xx, lastPath, argc, argv, nextArg)
  return NULL_PTR;
#endif /* not SPEECH_RECOGNITION_SUPPORTED */
} /* getASimplePath */

/*------------------------------------ getASimpleModel ---*/
static ModelDataPtr
getASimpleModel(ListenPtr   xx,
                const short argc,
                PAtom       argv,
                short &     nextArg)
{
#if SPEECH_RECOGNITION_SUPPORTED    
  if (xx)
  {
    ModelDataPtr aModel = lookupModel(xx, gTopLevelSymbol);
    bool         lastPath = false;

    if (aModel)
    {
      do
      {
        PathListPtr aPath = getASimplePath(xx, lastPath, argc, argv, nextArg);

        if (! aPath)
            break;
            
        aPath->fNext = aModel->fPaths;
        aModel->fPaths = aPath;
      }
      while (! lastPath);
    }
    return aModel;

  }
  return NULL_PTR;
#else /* not SPEECH_RECOGNITION_SUPPORTED */
 #pragma unused(xx, argc, argv, nextArg)
  return NULL_PTR;
#endif /* not SPEECH_RECOGNITION_SUPPORTED */
} /* getASimpleModel */

/*------------------------------------ cmd_Recognize ---*/
Pvoid
cmd_Recognize(ListenPtr xx,
              PSymbol   message,
              short     argc,
              PAtom     argv)
{
#if SPEECH_RECOGNITION_SUPPORTED
 #pragma unused(message)
#else /* not SPEECH_RECOGNITION_SUPPORTED */
 #pragma unused(message, argc, argv)
#endif /* not SPEECH_RECOGNITION_SUPPORTED */
  bool  okSoFar = true;

  EnterCallback();
  if (xx)
  {
#if SPEECH_RECOGNITION_SUPPORTED
    short        argWalk = 0;
    ModelDataPtr aModel = NULL_PTR;
    OSErr        result;

    clearModelData(xx);
    initializeHashTable(xx);
    aModel = getASimpleModel(xx, argc, argv, argWalk);
    if (aModel)
    {
      createLanguageModel(xx, aModel);
      result = SRSetLanguageModel(xx->fRecognizer, aModel->fModel);
      if (result == noErr)
        xx->fModelLoaded = true;
      else
        LOG_ERROR_2(OUTPUT_PREFIX "problem setting language model (%ld)", result)
    }
#endif /* SPEECH_RECOGNITION_SUPPORTED */
  }
  ExitMaxMessageHandler()
} /* cmd_Recognize */
