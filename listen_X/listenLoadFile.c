/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       listenLoadFile.c                                                        */
/*                                                                                      */
/*  Contains:   The low-level support routines for the listen module.                   */
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
/*  Created:    2001/04/10                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "listen.h"

#define TRACE_ATOMS 0

/*============================ Local routines ==================*/

/*------------------------------------ traceAtom ---*/
static void traceAtom
  (Qchar	tag,
   PAtom	value)
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

    case A_SEMI:
      LOG_POST_2("%s(semicolon)", tag)
      break;

    case A_COMMA:
      LOG_POST_2("%s(comma)", tag)
      break;

    case A_DOLLAR:
      LOG_POST_2("%s(dollar)", tag)
      break;

    default:
      LOG_POST_3("%s(unexpected atom, type=%hd)", tag, value->a_type)
      break;

  }
} /* traceAtom */

/*------------------------------------ pushBackAtom ---*/
static bool pushBackAtom
  (ListenPtr xx,
   PAtom     anAtom)
{
#if SPEECH_RECOGNITION_SUPPORTED
  if (xx->fBufferPreviousDefined)
  {
    LOG_ERROR_1(OUTPUT_PREFIX "cannot push back atom")
    return false;

  }
 #if TRACE_ATOMS
  traceAtom("pushBackAtom", anAtom);
 #endif /* TRACE_ATOMS */
  xx->fBufferPrevious = *anAtom;
  xx->fBufferPreviousDefined = true;
  return true;
#else /* not SPEECH_RECOGNITION_SUPPORTED */
 #pragma unused(xx, anAtom)
  return false;
#endif /* not SPEECH_RECOGNITION_SUPPORTED */
} /* pushBackPAtom/

/*------------------------------------ getNextAtom ---*/
static bool getNextAtom
  (ListenPtr xx,
   PAtom     value)
{
#if SPEECH_RECOGNITION_SUPPORTED
  Atom skipper;
  bool result = true;

  for ( ; result; )
  {
    if (xx->fBufferPreviousDefined)
    {
      *value = xx->fBufferPrevious;
      xx->fBufferPreviousDefined = false;
    }
    else
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
#else /* not SPEECH_RECOGNITION_SUPPORTED */
 #pragma unused(xx, value)
  return false;
#endif /* not SPEECH_RECOGNITION_SUPPORTED */
} /* getNextPAtom/

/*------------------------------------ skipToEol ---*/
static void skipToEol
  (ListenPtr xx)
{
#if SPEECH_RECOGNITION_SUPPORTED
  for ( ; ; )
  {
    Atom holder;
    bool result = getNextAtom(xx, &holder);

    if (! result)
      break;

 #if TRACE_ATOMS
    traceAtom("skipToEol", &holder);
 #endif /* TRACE_ATOMS */
    if (holder.a_type == A_SEMI)
      break;

  }
#else /* not SPEECH_RECOGNITION_SUPPORTED */
 #pragma unused(xx)
#endif /* not SPEECH_RECOGNITION_SUPPORTED */
} /* skipToEol */

/*------------------------------------ getASymbol ---*/
static ModelDataPtr getASymbol
  (ListenPtr xx)
{
#if SPEECH_RECOGNITION_SUPPORTED
  Atom         holder;
  bool         result = getNextAtom(xx, &holder);
  ModelDataPtr model = NULL_PTR;

  if (result)
  {
 #if TRACE_ATOMS
    traceAtom("getASymbol", &holder);
 #endif /* TRACE_ATOMS */
    if (holder.a_type == A_SYM)
    {
      if (symbolIsAModel(holder.a_w.w_sym))
      {
        model = lookupModel(xx, holder.a_w.w_sym);
        if (model)
        {
          if (model->fDefined)
          {
            LOG_ERROR_2(OUTPUT_PREFIX "duplicate model definition: '%s'",
                        holder.a_w.w_sym->s_name)
            model = NULL_PTR;
          }
        }
      }
      else
        LOG_ERROR_2(OUTPUT_PREFIX "not a valid model name: '%s'",
                    holder.a_w.w_sym->s_name)
    }
    else
      reportUnexpected(&holder);
  }
  return model;
#else /* not SPEECH_RECOGNITION_SUPPORTED */
 #pragma unused(xx)
  return NULL_PTR;
#endif /* not SPEECH_RECOGNITION_SUPPORTED */
} /* getASymbol */

/*------------------------------------ getMatchData ---*/
static MatchDataPtr getMatchData
  (ListenPtr xx)
{
#if SPEECH_RECOGNITION_SUPPORTED
  PBinbuf      tempBuffer = static_cast<PBinbuf>(binbuf_new());
  short        count = 0;
  MatchDataPtr matchData = NULL_PTR;
  bool         sawClose = false;

  do
  {
    Atom holder;
    bool result = getNextAtom(xx, &holder);

    if (result)
    {
 #if TRACE_ATOMS
      traceAtom("getMatchData1", &holder);
 #endif /* TRACE_ATOMS */
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
  /* At this point, we have all the Atoms between '{' and '}' collected into the Binbuf. */
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
 #if TRACE_ATOMS
            traceAtom("getMatchData2", &tempAtom);
 #endif /* TRACE_ATOMS */
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
 #pragma unused(xx)
  return NULL_PTR;
#endif /* not SPEECH_RECOGNITION_SUPPORTED */
} /* getMatchData */

/*------------------------------------ getModifierList ---*/
static bool getModifierList
  (ListenPtr xx,
   bool &    optional,
   bool &    repeatable)
{
#if SPEECH_RECOGNITION_SUPPORTED
  bool okSoFar = true, sawClose = false;

  if (xx)
  {
    optional = false;
    repeatable = false;
    do
    {
      Atom holder;
      bool result = getNextAtom(xx, &holder);

      if (result)
      {
 #if TRACE_ATOMS
        traceAtom("getModifierList", &holder);
 #endif /* TRACE_ATOMS */
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
  }
  return okSoFar;
#else /* not SPEECH_RECOGNITION_SUPPORTED */
 #pragma unused(xx, optional, repeatable)
  return false;
#endif /* not SPEECH_RECOGNITION_SUPPORTED */
} /* getModifierList */

/*------------------------------------ getAPath ---*/
static PathListPtr getAPath
  (ListenPtr xx,
   bool &    lastPath)
{
#if SPEECH_RECOGNITION_SUPPORTED
  bool moreStuff = true, okSoFar = true, pushBack = false;

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
        bool result = getNextAtom(xx, &holder);

        if (result)
        {
 #if TRACE_ATOMS
          traceAtom("getAPath1", &holder);
 #endif /* TRACE_ATOMS */
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
                  element->fIsModel = true;
                  element->fVar.fModelElement.fModelRef = NULL_PTR;
                  element->fVar.fModelElement.fDescription = lookupModel(xx,
                                                                        holder.a_w.w_sym);
                }
                else
                {
                  Atom holder2;

                  element->fIsModel = false;
                  element->fVar.fWordElement.fSymbol = holder.a_w.w_sym;
                  element->fVar.fWordElement.fMatch = NULL_PTR;
                  /* Check for a match list for this word. */
                  result = getNextAtom(xx, &holder2);
                  if (result)
                  {
 #if TRACE_ATOMS
                    traceAtom("getAPath2", &holder2);
 #endif /* TRACE_ATOMS */
                    if (holder2.a_type == A_SYM)
                    {
                      if (holder2.a_w.w_sym == gOpenCurlySymbol)
                        element->fVar.fWordElement.fMatch = getMatchData(xx);
                      else
                        pushBack = true;
                    }
                    else
                      pushBack = true;
                    if (pushBack)
                    {
                      pushBack = false;
                      if (! pushBackAtom(xx, &holder2))
                      {
                        LOG_ERROR_1(OUTPUT_PREFIX "could not push back input atom")
                        okSoFar = false;
                      }
                    }
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
                result = getNextAtom(xx, &holder3);
                if (result)
                {
 #if TRACE_ATOMS
                  traceAtom("getAPath3", &holder3);
 #endif /* TRACE_ATOMS */
                  if (holder3.a_type == A_SYM)
                  {
                    if (holder3.a_w.w_sym == gOpenSquareSymbol)
                      okSoFar = getModifierList(xx, element->fOptional,
                                                element->fRepeatable);
                    else
                      pushBack = true;
                  }
                  else
                    pushBack = true;
                }
                if (pushBack)
                {
                  pushBack = false;
                  if (! pushBackAtom(xx, &holder3))
                  {
                    LOG_ERROR_1(OUTPUT_PREFIX "could not push back input atom")
                    okSoFar = false;
                  }
                }
              }
            }
          }
          else if (holder.a_type == A_SEMI)
          {
            /* We've got the end of this path and of all the paths for this model. */
            lastPath = true;
            moreStuff = false;
          }
          else
          {
            reportUnexpected(&holder);
            okSoFar = false;
          }
        }
        else
        {
          LOG_ERROR_1(OUTPUT_PREFIX "premature end of input")
          okSoFar = false;
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
 #pragma unused(xx, lastPath)
  return NULL_PTR;
#endif /* not SPEECH_RECOGNITION_SUPPORTED */
} /* getAPath */

/*------------------------------------ getAModel ---*/
static ModelDataPtr getAModel
  (ListenPtr xx)
{
#if SPEECH_RECOGNITION_SUPPORTED

  if (xx)
  {
    bool         okSoFar = true;
    ModelDataPtr aModel = getASymbol(xx);

    if (aModel)
    {
      Atom holder;
      bool result = getNextAtom(xx, &holder);

      if (result)
      {
 #if TRACE_ATOMS
        traceAtom("getAModel1", &holder);
 #endif /* TRACE_ATOMS */
        if (holder.a_type == A_SYM)
        {
          /* Check if we have a match list for this model. */
          if (holder.a_w.w_sym == gOpenCurlySymbol)
          {
            aModel->fMatch = getMatchData(xx);
            result = getNextAtom(xx, &holder);
 #if TRACE_ATOMS
            traceAtom("getAModel2", &holder);
 #endif /* TRACE_ATOMS */
            okSoFar = result;
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
        LOG_ERROR_1(OUTPUT_PREFIX "missing '='")
        okSoFar = false;
      }
      if (okSoFar)
      {
        if (holder.a_type == A_SYM)
        {
          if (holder.a_w.w_sym == gEqualsSymbol)
          {
            bool lastPath = false;

            /* At this point, we've seen the model name, an optional match list, and */
            /* an equals sign. */
            do
            {
              PathListPtr aPath = getAPath(xx, lastPath);

              if (aPath)
              {
                aPath->fNext = aModel->fPaths;
                aModel->fPaths = aPath;
              }
              else
                break;

            }
            while (! lastPath);
          }
          else
          {
            /* Missing equals sign. */
            reportUnexpected(&holder);
            okSoFar = false;
          }
        }
        else
        {
          reportUnexpected(&holder);
          okSoFar = false;
        }
      }
      if (okSoFar)
        aModel->fDefined = true;
      else
      {
        MatchDataPtr match;

        for (PathListPtr pWalker = aModel->fPaths, path; pWalker; pWalker = path)
        {
          /* Clean out aPath */
          path = pWalker->fNext;
          for (ElementPtr eWalker = pWalker->fFirst, element; eWalker; eWalker = element)
          {
            element = eWalker->fNext;
            if (! eWalker->fIsModel)
            {
              match = eWalker->fVar.fWordElement.fMatch;
              if (match)
              {
                FREEBYTES(match->fData, match->fCount)
                FREEBYTES(match, 1)
              }
            }
            FREEBYTES(eWalker, 1)
          }
          FREEBYTES(pWalker, 1)
        }
        aModel->fPaths = NULL_PTR;
        match = aModel->fMatch;
        if (match)
        {
          FREEBYTES(match->fData, match->fCount)
          FREEBYTES(match, 1)
          aModel->fMatch = NULL_PTR;
        }
      }
    }
    return (okSoFar ? aModel : NULL_PTR);

  }
  else
    return NULL_PTR;

#else /* not SPEECH_RECOGNITION_SUPPORTED */
 #pragma unused(xx)
  return NULL_PTR;
#endif /* not SPEECH_RECOGNITION_SUPPORTED */
} /* getAModel */

/*------------------------------------ reachModel ---*/
static void reachModel
  (ModelDataPtr aModel)
{
#if SPEECH_RECOGNITION_SUPPORTED
  if (aModel && (! aModel->fReached))
  {
    aModel->fReached = true;
    for (PathListPtr aPath = aModel->fPaths; aPath; aPath = aPath->fNext)
    {
      for (ElementPtr element = aPath->fFirst; element; element = element->fNext)
      {
        if (element->fIsModel)
          reachModel(element->fVar.fModelElement.fDescription);
      }
    }
  }
#else /* not SPEECH_RECOGNITION_SUPPORTED */
 #pragma unused(aModel)
#endif /* not SPEECH_RECOGNITION_SUPPORTED */
} /* reachModel */

/*------------------------------------ circularModel ---*/
static bool circularModel
  (ModelDataPtr aModel,
   ModelDataPtr testModel)
{
#if SPEECH_RECOGNITION_SUPPORTED
  bool failed = false;

  if (aModel && (! aModel->fReached))
  {
    aModel->fReached = true;
    for (PathListPtr aPath = aModel->fPaths; aPath && (! failed); aPath = aPath->fNext)
    {
      for (ElementPtr element = aPath->fFirst; element && (! failed); element = element->fNext)
      {
        if (element->fIsModel)
        {
          ModelDataPtr bModel = element->fVar.fModelElement.fDescription;

          if (bModel == testModel)
          {
            LOG_ERROR_1(OUTPUT_PREFIX "circular reference in language system")
            failed = true;
          }
          else
            failed = circularModel(bModel, testModel);
        }
      }
    }
  }
  return failed;
#else /* not SPEECH_RECOGNITION_SUPPORTED */
 #pragma unused(aModel, testModel)
  return false;
#endif /* not SPEECH_RECOGNITION_SUPPORTED */
} /* circularModel */

/*------------------------------------ clearReachedFlag ---*/
static void clearReachedFlag
  (ListenPtr xx)
{
#if SPEECH_RECOGNITION_SUPPORTED
  if (xx && xx->fModels)
  {
    for (short ii = 0; ii < HASH_TABLE_SIZE; ++ii)
    {
      for (ModelDataPtr slot = *(xx->fModels + ii); slot; slot = slot->fNext)
        slot->fReached = false;
    }
  }
#else /* not SPEECH_RECOGNITION_SUPPORTED */
 #pragma unused(xx)
#endif /* not SPEECH_RECOGNITION_SUPPORTED */
} /* clearReachedFlag */

/*------------------------------------ checkDefined ---*/
static bool checkDefined
  (ListenPtr xx)
{
#if SPEECH_RECOGNITION_SUPPORTED
  bool okSoFar = true;

  if (xx && xx->fModels)
  {
    for (short ii = 0; (ii < HASH_TABLE_SIZE) && okSoFar; ++ii)
    {
      for (ModelDataPtr slot = *(xx->fModels + ii); slot && okSoFar; slot = slot->fNext)
      {
        if (! slot->fDefined)
        {
          LOG_ERROR_2(OUTPUT_PREFIX "language model '%s' not defined",
                      slot->fSymbol->s_name)
          okSoFar = false;
        }
      }
    }
  }
  return okSoFar;
#else /* not SPEECH_RECOGNITION_SUPPORTED */
 #pragma unused(xx)
  return false;
#endif /* not SPEECH_RECOGNITION_SUPPORTED */
} /* checkDefined */

/*------------------------------------ checkReached ---*/
static bool checkReached
  (ListenPtr xx)
{
#if SPEECH_RECOGNITION_SUPPORTED
  bool okSoFar = true;

  if (xx && xx->fModels)
  {
    for (short ii = 0; (ii < HASH_TABLE_SIZE) && okSoFar; ++ii)
    {
      for (ModelDataPtr slot = *(xx->fModels + ii); slot && okSoFar; slot = slot->fNext)
      {
        if (! slot->fReached)
        {
          LOG_ERROR_2(OUTPUT_PREFIX "language model '%s' not referenced",
                      slot->fSymbol->s_name)
          okSoFar = false;
        }
      }
    }
  }
  return okSoFar;
#else /* not SPEECH_RECOGNITION_SUPPORTED */
 #pragma unused(xx)
  return false;
#endif /* not SPEECH_RECOGNITION_SUPPORTED */
} /* checkReached */

/*------------------------------------ checkForCircularity ---*/
static bool checkForCircularity
  (ListenPtr xx)
{
#if SPEECH_RECOGNITION_SUPPORTED
  bool okSoFar = true;

  if (xx && xx->fModels)
  {
    for (short ii = 0; (ii < HASH_TABLE_SIZE) && okSoFar; ++ii)
    {
      for (ModelDataPtr slot = *(xx->fModels + ii); slot && okSoFar; slot = slot->fNext)
      {
        clearReachedFlag(xx);
        okSoFar = (! circularModel(slot, slot));
      }
    }
  }
  return okSoFar;
#else /* not SPEECH_RECOGNITION_SUPPORTED */
 #pragma unused(xx)
  return false;
#endif /* not SPEECH_RECOGNITION_SUPPORTED */
} /* checkForCircularity */

/*============================ Global routines =================*/

/*------------------------------------ clearHashTable ---*/
void clearHashTable
  (ListenPtr xx)
{
#if SPEECH_RECOGNITION_SUPPORTED
  if (xx && xx->fModels)
  {
    for (short ii = 0; ii < HASH_TABLE_SIZE; ++ii)
    {
      for (ModelDataPtr slot = *(xx->fModels + ii), next; slot; slot = next)
      {
        MatchDataPtr match = slot->fMatch;

        next = slot->fNext;
        if (match)
        {
          FREEBYTES(match->fData, match->fCount)
          FREEBYTES(match, 1)
        }
        for (PathListPtr pWalker = slot->fPaths, path; pWalker; pWalker = path)
        {
          path = pWalker->fNext;
          for (ElementPtr eWalker = pWalker->fFirst, element; eWalker; eWalker = element)
          {
            element = eWalker->fNext;
            if (eWalker->fIsModel)
            {
              if (eWalker->fIsReference && eWalker->fVar.fModelElement.fModelRef)
                SRReleaseObject(eWalker->fVar.fModelElement.fModelRef);
            }
            else
            {
              match = eWalker->fVar.fWordElement.fMatch;
              if (match)
              {
                FREEBYTES(match->fData, match->fCount)
                FREEBYTES(match, 1)
              }
            }
            FREEBYTES(eWalker, 1)
          }
          FREEBYTES(pWalker, 1)
        }
        FREEBYTES(slot, 1)
      }
    }
    FREEBYTES(xx->fModels, HASH_TABLE_SIZE)
  }
#else /* not SPEECH_RECOGNITION_SUPPORTED */
 #pragma unused(xx)
#endif /* not SPEECH_RECOGNITION_SUPPORTED */
} /* clearHashTable */

/*------------------------------------ clearModelData ---*/
void clearModelData
  (ListenPtr xx)
{
#if SPEECH_RECOGNITION_SUPPORTED
  if (xx)
  {
    FREEBYTES(xx->fPreviousResult, xx->fPreviousCount)
    xx->fPreviousCount = 0;
    if (xx->fBuffer)
    {
      freeobject(reinterpret_cast<PObject>(xx->fBuffer));
      xx->fBuffer = NULL_PTR;
    }
    if (xx->fRecognizer)
      SRSetLanguageModel(xx->fRecognizer, NULL_PTR);
    xx->fModelLoaded = false;
    clearHashTable(xx);
  }
#else /* not SPEECH_RECOGNITION_SUPPORTED */
 #pragma unused(xx)
#endif /* not SPEECH_RECOGNITION_SUPPORTED */
} /* clearModelData */

#if SPEECH_RECOGNITION_SUPPORTED
/*------------------------------------ createLanguageModel ---*/
void createLanguageModel
  (ListenPtr    xx,
   ModelDataPtr aModel)
{
  if (xx && aModel)
  {
    SRLanguageModel anSRModel = NULL_PTR;
    bool            okSoFar = true;
    Pchar           name = aModel->fSymbol->s_name;
    OSErr           result = SRNewLanguageModel(xx->fRecognitionSystem, &anSRModel, name,
                                                long(strlen(name)));

    if (result == noErr)
    {
      MatchDataPtr match = aModel->fMatch;

      if (match)
      {
        long refCon = long(match);

        result = SRSetProperty(anSRModel, kSRRefCon, &refCon, sizeof(refCon));
        if (result != noErr)
          LOG_ERROR_2(OUTPUT_PREFIX "problem setting reference constant (%ld)", result)
      }
    }
    else
      LOG_ERROR_2(OUTPUT_PREFIX "problem creating language model (%ld)", result)
    if (result == noErr)
    {
      for (PathListPtr path = aModel->fPaths; path && okSoFar && (result == noErr);
      			path = path->fNext)
      {
        SRPath anSRPath = createPath(xx, path);

        if (anSRPath)
        {
          result = SRAddLanguageObject(anSRModel, anSRPath);
          if (result != noErr)
            LOG_ERROR_2(OUTPUT_PREFIX "problem adding path to language model (%ld)",
                        result)
        }
        else
          okSoFar = false;
      }
    }
    if (okSoFar && (result == noErr))
      aModel->fModel = anSRModel;
    else
      SRReleaseObject(anSRModel);
  }
} /* createLanguageModel */
#endif /* SPEECH_RECOGNITION_SUPPORTED */

#if SPEECH_RECOGNITION_SUPPORTED
/*------------------------------------ createPath ---*/
SRPath createPath
  (ListenPtr   xx,
   PathListPtr aPath)
{
  SRPath anSRPath = NULL_PTR;
  OSErr  result = noErr;

  if (xx && aPath)
  {        
    result = SRNewPath(xx->fRecognitionSystem, &anSRPath);
    if (result == noErr)
    {
      for (ElementPtr element = aPath->fFirst; element && (result == noErr);
      			element = element->fNext)
      {
        SRSpeechObject thisElement = NULL_PTR;

        if (element->fIsModel)
        {
          ModelDataPtr aModel = element->fVar.fModelElement.fDescription;

          if (aModel && aModel->fModel)
          {
            SRSpeechObject newRef = NULL_PTR;                        

            result = SRGetReference(aModel->fModel, &newRef);
            if (result == noErr)
            {
              element->fIsReference = true;
              element->fVar.fModelElement.fModelRef = newRef;
              thisElement = newRef;
              result = SRAddLanguageObject(anSRPath, newRef);
              if (result != noErr)
                LOG_ERROR_2(OUTPUT_PREFIX "problem adding reference to path (%ld)",
                            result)
            }
            else
              LOG_ERROR_2(OUTPUT_PREFIX "problem creating reference (%ld)", result)
          }
          else
          {
            createLanguageModel(xx, aModel);
            thisElement = aModel->fModel;
            result = SRAddLanguageObject(anSRPath, aModel->fModel);
            if (result != noErr)
              LOG_ERROR_2(OUTPUT_PREFIX "problem adding model to path (%ld)", result)
          }
        }
        else
        {
          Pchar  wordChars = element->fVar.fWordElement.fSymbol->s_name;
          SRWord anSRWord = NULL_PTR;

          result = SRNewWord(xx->fRecognitionSystem, &anSRWord, wordChars,
                            long(strlen(wordChars)));
          if (result == noErr)
          {
            MatchDataPtr match = element->fVar.fWordElement.fMatch;

            thisElement = anSRWord;
            if (match)
            {
              long refCon = long(match);

              result = SRSetProperty(anSRWord, kSRRefCon, &refCon, sizeof(refCon));
              if (result != noErr)
                LOG_ERROR_2(OUTPUT_PREFIX "problem setting reference constant (%ld)",
                            result)
            }
          }
          else
            LOG_ERROR_3(OUTPUT_PREFIX "problem creating word '%s' (%ld)", wordChars,
                        result)
          if (result == noErr)
          {
            result = SRAddLanguageObject(anSRPath, anSRWord);
            if (result != noErr)
              LOG_ERROR_2(OUTPUT_PREFIX "problem adding word to path (%ld)", result)
          }
        }
        if (thisElement && (result == noErr))
        {
          if (element->fOptional)
          {
            bool isOptional = true;

            result = SRSetProperty(thisElement, kSROptional, &isOptional,
                                    sizeof(isOptional));
            if (result != noErr)
              LOG_ERROR_2(OUTPUT_PREFIX "problem making path element optional (%ld)",
                          result)
          }
          if ((result == noErr) && (element->fRepeatable))
          {
            bool isRepeatable = true;

            result = SRSetProperty(thisElement, kSRRepeatable, &isRepeatable,
                                    sizeof(isRepeatable));
            if (result != noErr)
              LOG_ERROR_2(OUTPUT_PREFIX "problem making path element repeatable (%ld)",
                          result)
          }
        }
      }
    }
    else
      LOG_ERROR_2(OUTPUT_PREFIX "problem creating path (%ld)", result)
  }
  if (result != noErr)
  {
    SRReleaseObject(anSRPath);
    anSRPath = NULL_PTR;
  }
  return anSRPath;
} /* createPath */
#endif /* SPEECH_RECOGNITION_SUPPORTED */

/*------------------------------------ doLoadLanguageFile ---*/
void doLoadLanguageFile
  (ListenPtr xx,
   Pchar     fileName)
{
#if SPEECH_RECOGNITION_SUPPORTED
  bool         okSoFar = true;
  PBinbuf      inBuffer = static_cast<PBinbuf>(binbuf_new());
  short        vol = 0;
  ModelDataPtr topModel = NULL_PTR, aModel = NULL_PTR;

  clearModelData(xx);
  if (! inBuffer)
    LOG_ERROR_1(OUTPUT_PREFIX "problem allocating binbuf")
  else if (locatefiletype(fileName, &vol, long('TEXT'), 0L))
    LOG_ERROR_2(OUTPUT_PREFIX "problem locating file '%s'", fileName)
  else if (binbuf_read(inBuffer, fileName, vol, 0))
    LOG_ERROR_2(OUTPUT_PREFIX "problem opening and reading '%s'", fileName)
  else
  {
    // Process the input file:
    xx->fBufferPreviousDefined = false;
    xx->fBuffer = inBuffer;
    xx->fBufferTypeOffset = xx->fBufferStuffOffset = 0;
    initializeHashTable(xx);
    topModel = getASymbol(xx);
    do
    {
      aModel = getAModel(xx);
    }
    while (aModel);
    clearReachedFlag(xx);
    okSoFar = checkDefined(xx);
    if (okSoFar)
      reachModel(topModel);
    if (okSoFar && checkReached(xx) && checkForCircularity(xx))
    {
      OSErr result;

      clearReachedFlag(xx);
      createLanguageModel(xx, topModel);
      result = SRSetLanguageModel(xx->fRecognizer, topModel->fModel);
      if (result != noErr)
      {
        LOG_ERROR_2(OUTPUT_PREFIX "problem setting language model (%ld)", result)
        okSoFar = false;
      }
    }
    else
    {
      clearModelData(xx);
      okSoFar = false;
    }
    xx->fBuffer = NULL_PTR;
    freeobject(reinterpret_cast<PObject>(inBuffer));
  }
  if (okSoFar)
    xx->fModelLoaded = true;
  else
    outlet_bang(xx->fErrorBangOut);
#else /* not SPEECH_RECOGNITION_SUPPORTED */
 #pragma unused(xx, fileName)
#endif /* SPEECH_RECOGNITION_SUPPORTED */
} /* doLoadLanguageFile */

/*------------------------------------ initializeHashTable ---*/
void initializeHashTable
  (ListenPtr xx)
{
#if SPEECH_RECOGNITION_SUPPORTED
  clearHashTable(xx);
  xx->fModels = GETBYTES(HASH_TABLE_SIZE, ModelDataPtr);
  if (xx->fModels)
  {
    for (short ii = 0; ii < HASH_TABLE_SIZE; ++ii)
      *(xx->fModels + ii) = NULL_PTR;
  }
#else /* not SPEECH_RECOGNITION_SUPPORTED */
 #pragma unused(xx)
#endif /* not SPEECH_RECOGNITION_SUPPORTED */
} /* initializeHashTable */

/*------------------------------------ lookupModel ---*/
ModelDataPtr lookupModel
  (ListenPtr xx,
   PSymbol   name)
{
#if SPEECH_RECOGNITION_SUPPORTED
  ModelDataPtr slot = NULL_PTR;

  if (xx && xx->fModels)
  {
    ModelDataPtr prev = NULL_PTR;
    short        ii = short(long(name) % HASH_TABLE_SIZE);

    for (slot = *(xx->fModels + ii); slot; slot = slot->fNext)
    {
      if (slot->fSymbol == name)
        break;

      prev = slot;
    }
    if (! slot)
    {
      slot = GETBYTES(1, ModelData);
      if (slot)
      {
        slot->fNext = NULL_PTR;
        if (prev)
          prev->fNext = slot;
        else
          *(xx->fModels + ii) = slot;
        slot->fSymbol = name;
        slot->fDefined = slot->fReached = false;
        slot->fModel = NULL_PTR;
        slot->fMatch = NULL_PTR;
        slot->fPaths = NULL_PTR;
      }
    }
  }
  return slot;
#else /* not SPEECH_RECOGNITION_SUPPORTED */
 #pragma unused(xx, name)
  return NULL_PTR;
#endif /* not SPEECH_RECOGNITION_SUPPORTED */
} /* lookupModel */

/*------------------------------------ reportUnexpected ---*/
void reportUnexpected
  (PAtom what)
{
#if SPEECH_RECOGNITION_SUPPORTED
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
#else /* not SPEECH_RECOGNITION_SUPPORTED */
 #pragma unused(what)
#endif /* not SPEECH_RECOGNITION_SUPPORTED */
} /* reportUnexpected */

/*------------------------------------ symbolIsAModel ---*/
bool symbolIsAModel
  (PSymbol aSymbol)
{
#if SPEECH_RECOGNITION_SUPPORTED
  Qchar		walker = aSymbol->s_name;
  size_t	len = strlen(walker);

  if ((*walker == kOpenModelChar) && (len > 2) &&
      (*(walker + len - 1) == kCloseModelChar))
    return true;

  return false;
#else /* not SPEECH_RECOGNITION_SUPPORTED */
 #pragma unused(aSymbol)
  return false;
#endif /* not SPEECH_RECOGNITION_SUPPORTED */
} /* symbolIsAModel */