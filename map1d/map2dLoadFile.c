/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       map2dLoadFile.c                                                         */
/*                                                                                      */
/*  Contains:   The low-level support routines for the map2d module.                    */
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
/*  Created:    2000/11/15                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "map2d.h"

/*------------------------------------ map2dClearRangeList ---*/
void map2dClearRangeList
  (Map2dPtr xx)
{
  if (xx)
  {
    while (xx->fFirstRange)
    {
      RangeDataPtr walker = xx->fFirstRange;

      xx->fFirstRange = walker->fNext;
      FREEBYTES(walker->fOutput, walker->fOutputCount)
      FREEBYTES(walker, 1)
    }
    xx->fPreviousResult = xx->fLastRange = NULL_PTR;
    xx->fRangeCount = 0;
  }
} /* map2dClearRangeList */

/*------------------------------------ map2dGetNextAtomInBuffer ---*/
static bool map2dGetNextAtomInBuffer
  (Map2dPtr xx,
   PAtom    value)
{
  Atom skipper;
  bool result = true;

  while (result)
  {
    result = (! binbuf_getatom(xx->fBuffer, &xx->fBufferTypeOffset,
                                &xx->fBufferStuffOffset, value));
    if (result && (value->a_type == A_SYM))
    {
      if (value->a_w.w_sym->s_name[0] == kCommentCharacter)
      {
        /* skip a comment */
        while (result)
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
} /* map2dGetNextPAtom/

/*------------------------------------ map2dReportUnexpected ---*/
static void map2dReportUnexpected
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
} /* map2dReportUnexpected */

/*------------------------------------ map2dCollectARange ---*/
static bool map2dCollectARange
  (Map2dPtr xx)
{
  Atom     holder;
  bool     result = map2dGetNextAtomInBuffer(xx, &holder);
  bool     bottomClosed = false, leftClosed = false;
  bool     rightClosed = false, topClosed = false;
  bool     bottomTopDontCare = false, leftRightDontCare = false;
  float    bottomFloatValue, leftFloatValue;
  float    rightFloatValue, topFloatValue;
  long     bottomIntValue, leftIntValue;
  long     rightIntValue, topIntValue;
  Category bottomMatch = MatchUnknown, leftMatch = MatchUnknown;
  Category rightMatch = MatchUnknown, topMatch = MatchUnknown;
  short    outputCount = 0;
  PBinbuf  collector = NULL_PTR;
  short    dollarsPresent = 0, doubleDollarsPresent = 0;
  short    singleDollarsPresent = 0;

  if (result)
  {
    if (holder.a_type == A_SYM)
    {
      if (holder.a_w.w_sym == gAsteriskSymbol)
        leftRightDontCare = true;
      else if (holder.a_w.w_sym == gOpenSquareSymbol)
        leftClosed = true;
      else if (holder.a_w.w_sym != gOpenRoundSymbol)
      {
        map2dReportUnexpected(&holder);
        result = false;
      }
    }
    else
    {
      map2dReportUnexpected(&holder);
      result = false;
    }
  }
  // Get left value
  if (result && (! leftRightDontCare))
  {
    result = map2dGetNextAtomInBuffer(xx, &holder);
    if (result)
    {
      switch (holder.a_type)
      {
        case A_SYM:
          if ((holder.a_w.w_sym == gNegInfSymbol1) ||
              (holder.a_w.w_sym == gNegInfSymbol2))
            leftMatch = MatchInfinity;
          else
          {
            map2dReportUnexpected(&holder);
            result = false;
          }
          break;

        case A_FLOAT:
          leftMatch = MatchFloat;
          leftFloatValue = holder.a_w.w_float;
          break;

        case A_LONG:
          leftMatch = MatchInteger;
          leftIntValue = holder.a_w.w_long;
          break;

        default:
          map2dReportUnexpected(&holder);
          result = false;
          break;

      }
    }
  }
  // Get right value
  if (result && (! leftRightDontCare))
  {
    result = map2dGetNextAtomInBuffer(xx, &holder);
    if (result)
    {
      switch (holder.a_type)
      {
        case A_SYM:
          if ((holder.a_w.w_sym == gPosInfSymbol1) ||
              (holder.a_w.w_sym == gPosInfSymbol2) ||
              (holder.a_w.w_sym == gPosInfSymbol3) ||
              (holder.a_w.w_sym == gPosInfSymbol4))
            rightMatch = MatchInfinity;
          else
          {
            map2dReportUnexpected(&holder);
            result = false;
          }
          break;

        case A_FLOAT:
          rightMatch = MatchFloat;
          rightFloatValue = holder.a_w.w_float;
          break;

        case A_LONG:
          rightMatch = MatchInteger;
          rightIntValue = holder.a_w.w_long;
          break;

        default:
          map2dReportUnexpected(&holder);
          result = false;
          break;

      }
    }
  }
  // Get right bracket
  if (result && (! leftRightDontCare))
  {
    result = map2dGetNextAtomInBuffer(xx, &holder);
    if (holder.a_type == A_SYM)
    {
      if (holder.a_w.w_sym == gCloseSquareSymbol)
        rightClosed = true;
      else if (holder.a_w.w_sym != gCloseRoundSymbol)
      {
        map2dReportUnexpected(&holder);
        result = false;
      }
    }
    else
    {
      map2dReportUnexpected(&holder);
      result = false;
    }
  }
  if (result)
  {
    result = map2dGetNextAtomInBuffer(xx, &holder);
    if (holder.a_type == A_SYM)
    {
      if (holder.a_w.w_sym == gAsteriskSymbol)
        bottomTopDontCare = true;
      else if (holder.a_w.w_sym == gOpenSquareSymbol)
        bottomClosed = true;
      else if (holder.a_w.w_sym != gOpenRoundSymbol)
      {
        map2dReportUnexpected(&holder);
        result = false;
      }
    }
    else
    {
      map2dReportUnexpected(&holder);
      result = false;
    }
  }
  // Get bottom value
  if (result && (! bottomTopDontCare))
  {
    result = map2dGetNextAtomInBuffer(xx, &holder);
    if (result)
    {
      switch (holder.a_type)
      {
        case A_SYM:
          if ((holder.a_w.w_sym == gNegInfSymbol1) ||
              (holder.a_w.w_sym == gNegInfSymbol2))
            bottomMatch = MatchInfinity;
          else
          {
            map2dReportUnexpected(&holder);
            result = false;
          }
          break;

        case A_FLOAT:
          bottomMatch = MatchFloat;
          bottomFloatValue = holder.a_w.w_float;
          break;

        case A_LONG:
          bottomMatch = MatchInteger;
          bottomIntValue = holder.a_w.w_long;
          break;

        default:
          map2dReportUnexpected(&holder);
          result = false;
          break;

      }
    }
  }
  // Get top value
  if (result && (! bottomTopDontCare))
  {
    result = map2dGetNextAtomInBuffer(xx, &holder);
    if (result)
    {
      switch (holder.a_type)
      {
        case A_SYM:
          if ((holder.a_w.w_sym == gPosInfSymbol1) ||
              (holder.a_w.w_sym == gPosInfSymbol2) ||
              (holder.a_w.w_sym == gPosInfSymbol3) ||
              (holder.a_w.w_sym == gPosInfSymbol4))
            topMatch = MatchInfinity;
          else
          {
            map2dReportUnexpected(&holder);
            result = false;
          }
          break;

        case A_FLOAT:
          topMatch = MatchFloat;
          topFloatValue = holder.a_w.w_float;
          break;

        case A_LONG:
          topMatch = MatchInteger;
          topIntValue = holder.a_w.w_long;
          break;

        default:
          map2dReportUnexpected(&holder);
          result = false;
          break;

      }
    }
  }
  // Get top bracket
  if (result && (! bottomTopDontCare))
  {
    result = map2dGetNextAtomInBuffer(xx, &holder);
    if (holder.a_type == A_SYM)
    {
      if (holder.a_w.w_sym == gCloseSquareSymbol)
        topClosed = true;
      else if (holder.a_w.w_sym != gCloseRoundSymbol)
      {
        map2dReportUnexpected(&holder);
        result = false;
      }
    }
    else
    {
      map2dReportUnexpected(&holder);
      result = false;
    }
  }
  // Collect the return values
  if (result)
  {
    result = map2dGetNextAtomInBuffer(xx, &holder);
    collector = static_cast<PBinbuf>(binbuf_new());

    /* Collect the output values, terminated by a semicolon: */
    if (result && collector)
    {
      while (holder.a_type != A_SEMI)
      {
        /* We have a value; attach it. */
        binbuf_append(collector, NULL_PTR, 1, &holder);
        outputCount++;
        if (holder.a_type == A_SYM)
        {
          if (holder.a_w.w_sym == gDollarSymbol)
            dollarsPresent++;
          else if (holder.a_w.w_sym == gDoubleDollarSymbol)
            doubleDollarsPresent++;
          else if ((holder.a_w.w_sym == gDollarXSymbol) ||
                  (holder.a_w.w_sym == gDollarYSymbol) ||
                  (holder.a_w.w_sym == gDoubleDollarXSymbol) ||
                  (holder.a_w.w_sym == gDoubleDollarYSymbol))
            singleDollarsPresent++;
        }
        else if (holder.a_type == A_DOLLAR)
          dollarsPresent++;
        result = map2dGetNextAtomInBuffer(xx, &holder);
        if (! result)
          break;

      }
    }
  }
  // Assemble the information and attach to the list
  if (result)
  {
    RangeDataPtr newData = GETBYTES(1, RangeData);

    if (newData)
    {
      newData->fNext = NULL_PTR;
      newData->fOutputCount = outputCount;
      newData->fDollarsPresent = dollarsPresent;
      newData->fDoubleDollarsPresent = doubleDollarsPresent;
      newData->fSingleDollarsPresent = singleDollarsPresent;
      newData->fBottomTopDontCare = bottomTopDontCare;
      newData->fLeftRightDontCare = leftRightDontCare;     
      if (! bottomTopDontCare)
      {
	      if (bottomMatch == MatchFloat)
	        setFOI2Float(newData->fBottom.fValue, bottomFloatValue);
	      else if (bottomMatch == MatchInteger)
	        setFOI2Integer(newData->fBottom.fValue, bottomIntValue);
	      if (topMatch == MatchFloat)
	        setFOI2Float(newData->fTop.fValue, topFloatValue);
	      else if (topMatch == MatchInteger)
	        setFOI2Integer(newData->fTop.fValue, topIntValue);
	      newData->fBottom.fKind = bottomMatch;
	      newData->fTop.fKind = topMatch;
	      newData->fBottom.fIsClosed = bottomClosed;
	      newData->fTop.fIsClosed = topClosed;
      }
      if (! leftRightDontCare)
      {
	      if (leftMatch == MatchFloat)
	        setFOI2Float(newData->fLeft.fValue, leftFloatValue);
	      else if (leftMatch == MatchInteger)
	        setFOI2Integer(newData->fLeft.fValue, leftIntValue);
	      if (rightMatch == MatchFloat)
	        setFOI2Float(newData->fRight.fValue, rightFloatValue);
	      else if (rightMatch == MatchInteger)
	        setFOI2Integer(newData->fRight.fValue, rightIntValue);
	      newData->fLeft.fKind = leftMatch;
	      newData->fRight.fKind = rightMatch;
	      newData->fLeft.fIsClosed = leftClosed;
	      newData->fRight.fIsClosed = rightClosed;
      }
      if (outputCount)
      {
        long  tyOffset = 0, stOffset = 0;
        PAtom vector = GETBYTES(newData->fOutputCount, Atom);

        newData->fOutput = vector;
        for (short ii = 0; ii < newData->fOutputCount; ii++, vector++)
        {
          if (binbuf_getatom(collector, &tyOffset, &stOffset, vector))
            break;

        }
      }
      else
        newData->fOutput = NULL_PTR;
      if (xx->fLastRange)
        xx->fLastRange->fNext = newData;
      xx->fLastRange = newData;
      if (! xx->fFirstRange)
        xx->fFirstRange = newData;
    }
  }
  if (collector)
    freeobject(reinterpret_cast<PObject>(collector));
  return result;
} /* map2dCollectARange */

/*------------------------------------ map2dLoadRangeList ---*/
bool map2dLoadRangeList
  (Map2dPtr xx,
   PSymbol  filename)
{
  bool  result = false;
  short vol = 0;

  if (locatefiletype(filename->s_name, &vol, long('TEXT'), 0L))
    LOG_ERROR_2(OUTPUT_PREFIX "problem locating file '%s'", filename->s_name)
  else if (binbuf_read(xx->fBuffer, filename->s_name, vol, 0))
    LOG_ERROR_2(OUTPUT_PREFIX "problem opening and reading '%s'", filename->s_name)
  else
  {
    map2dClearRangeList(xx);
    xx->fBufferTypeOffset = 0;
    xx->fBufferStuffOffset = 0;
    while (map2dCollectARange(xx))
    {
      xx->fRangeCount++;
      if (xx->fVerbose)
        LOG_POST_1(OUTPUT_PREFIX "read range specification")
    }
    result = true;
  }
  return result;
} /* map2dLoadRangeList */

/*------------------------------------ map2dGetNextAtomInList ---*/
static bool map2dGetNextAtomInList
  (short &			offset,
   const short	numAtoms,
   PAtom  			inList,
   PAtom  			result)
{
  bool  okSoFar = false;

  if (offset < numAtoms)
  {
    okSoFar = true;
    *result = *(inList + offset);
  }
  else if (offset == numAtoms)
  {
    okSoFar = true;
    result->a_type = A_SEMI;
  }
  offset++;
  return okSoFar;
} /* map2dGetNextAtomInList */

/*------------------------------------ map2dConvertListToRange ---*/
RangeDataPtr map2dConvertListToRange
  (const short	offset,
   const short	numAtoms,
   PAtom				inList)
{
  short        nextAtom = offset;
  Atom         holder;
  bool         result = map2dGetNextAtomInList(nextAtom, numAtoms, inList, &holder);
  bool         bottomClosed = false, leftClosed = false;
  bool         rightClosed = false, topClosed = false;
  bool         bottomTopDontCare = false, leftRightDontCare = false;
  float        bottomFloatValue, leftFloatValue;
  float        rightFloatValue, topFloatValue;
  long         bottomIntValue, leftIntValue;
  long         rightIntValue, topIntValue;
  Category     bottomMatch = MatchUnknown, leftMatch = MatchUnknown;
  Category     rightMatch = MatchUnknown, topMatch = MatchUnknown;
  short        outputCount = 0;
  PBinbuf      collector = NULL_PTR;
  short        dollarsPresent = 0, doubleDollarsPresent = 0;
  short        singleDollarsPresent = 0;
  RangeDataPtr newData = NULL_PTR;

  if (result)
  {
    if (holder.a_type == A_SYM)
    {
      if (holder.a_w.w_sym == gAsteriskSymbol)
      	leftRightDontCare = true;
      else if (holder.a_w.w_sym == gOpenSquareSymbol)
        leftClosed = true;
      else if (holder.a_w.w_sym != gOpenRoundSymbol)
      {
        map2dReportUnexpected(&holder);
        result = false;
      }
    }
    else
    {
      map2dReportUnexpected(&holder);
      result = false;
    }
  }
  // Get left value
  if (result && (! leftRightDontCare))
  {
    result = map2dGetNextAtomInList(nextAtom, numAtoms, inList, &holder);
    if (result)
    {
      switch (holder.a_type)
      {
        case A_SYM:
          if ((holder.a_w.w_sym == gNegInfSymbol1) ||
              (holder.a_w.w_sym == gNegInfSymbol2))
            leftMatch = MatchInfinity;
          else
          {
            map2dReportUnexpected(&holder);
            result = false;
          }
          break;

        case A_FLOAT:
          leftMatch = MatchFloat;
          leftFloatValue = holder.a_w.w_float;
          break;

        case A_LONG:
          leftMatch = MatchInteger;
          leftIntValue = holder.a_w.w_long;
          break;

        default:
          map2dReportUnexpected(&holder);
          result = false;
          break;

      }
    }
  }
  // Get right value
  if (result && (! leftRightDontCare))
  {
    result = map2dGetNextAtomInList(nextAtom, numAtoms, inList, &holder);
    if (result)
    {
      switch (holder.a_type)
      {
        case A_SYM:
          if ((holder.a_w.w_sym == gPosInfSymbol1) ||
              (holder.a_w.w_sym == gPosInfSymbol2) ||
              (holder.a_w.w_sym == gPosInfSymbol3) ||
              (holder.a_w.w_sym == gPosInfSymbol4))
            rightMatch = MatchInfinity;
          else
          {
            map2dReportUnexpected(&holder);
            result = false;
          }
          break;

        case A_FLOAT:
          rightMatch = MatchFloat;
          rightFloatValue = holder.a_w.w_float;
          break;

        case A_LONG:
          rightMatch = MatchInteger;
          rightIntValue = holder.a_w.w_long;
          break;

        default:
          map2dReportUnexpected(&holder);
          result = false;
          break;

      }
    }
  }
  // Get right bracket
  if (result && (! leftRightDontCare))
  {
    result = map2dGetNextAtomInList(nextAtom, numAtoms, inList, &holder);
    if (holder.a_type == A_SYM)
    {
      if (holder.a_w.w_sym == gCloseSquareSymbol)
        rightClosed = true;
      else if (holder.a_w.w_sym != gCloseRoundSymbol)
      {
        map2dReportUnexpected(&holder);
        result = false;
      }
    }
    else
    {
      map2dReportUnexpected(&holder);
      result = false;
    }
  }
  if (result)
  {
    result = map2dGetNextAtomInList(nextAtom, numAtoms, inList, &holder);
    if (holder.a_type == A_SYM)
    {
      if (holder.a_w.w_sym == gAsteriskSymbol)
      	bottomTopDontCare = true;
      else if (holder.a_w.w_sym == gOpenSquareSymbol)
        bottomClosed = true;
      else if (holder.a_w.w_sym != gOpenRoundSymbol)
      {
        map2dReportUnexpected(&holder);
        result = false;
      }
    }
    else
    {
      map2dReportUnexpected(&holder);
      result = false;
    }
  }
  // Get bottom value
  if (result && (! bottomTopDontCare))
  {
    result = map2dGetNextAtomInList(nextAtom, numAtoms, inList, &holder);
    if (result)
    {
      switch (holder.a_type)
      {
        case A_SYM:
          if ((holder.a_w.w_sym == gNegInfSymbol1) ||
              (holder.a_w.w_sym == gNegInfSymbol2))
            bottomMatch = MatchInfinity;
          else
          {
            map2dReportUnexpected(&holder);
            result = false;
          }
          break;

        case A_FLOAT:
          bottomMatch = MatchFloat;
          bottomFloatValue = holder.a_w.w_float;
          break;

        case A_LONG:
          bottomMatch = MatchInteger;
          bottomIntValue = holder.a_w.w_long;
          break;

        default:
          map2dReportUnexpected(&holder);
          result = false;
          break;

      }
    }
  }
  // Get top value
  if (result && (! bottomTopDontCare))
  {
    result = map2dGetNextAtomInList(nextAtom, numAtoms, inList, &holder);
    if (result)
    {
      switch (holder.a_type)
      {
        case A_SYM:
          if ((holder.a_w.w_sym == gPosInfSymbol1) ||
              (holder.a_w.w_sym == gPosInfSymbol2) ||
              (holder.a_w.w_sym == gPosInfSymbol3) ||
              (holder.a_w.w_sym == gPosInfSymbol4))
            topMatch = MatchInfinity;
          else
          {
            map2dReportUnexpected(&holder);
            result = false;
          }
          break;

        case A_FLOAT:
          topMatch = MatchFloat;
          topFloatValue = holder.a_w.w_float;
          break;

        case A_LONG:
          topMatch = MatchInteger;
          topIntValue = holder.a_w.w_long;
          break;

        default:
          map2dReportUnexpected(&holder);
          result = false;
          break;

      }
    }
  }
  // Get top bracket
  if (result && (! bottomTopDontCare))
  {
    result = map2dGetNextAtomInList(nextAtom, numAtoms, inList, &holder);
    if (holder.a_type == A_SYM)
    {
      if (holder.a_w.w_sym == gCloseSquareSymbol)
        topClosed = true;
      else if (holder.a_w.w_sym != gCloseRoundSymbol)
      {
        map2dReportUnexpected(&holder);
        result = false;
      }
    }
    else
    {
      map2dReportUnexpected(&holder);
      result = false;
    }
  }
  // Collect the return values
  if (result)
  {
    result = map2dGetNextAtomInList(nextAtom, numAtoms, inList, &holder);
    collector = static_cast<PBinbuf>(binbuf_new());

    /* Collect the output values, terminated by a semicolon: */
    if (result && collector)
    {
      while (holder.a_type != A_SEMI)
      {
        /* We have a value; attach it. */
        binbuf_append(collector, NULL_PTR, 1, &holder);
        outputCount++;
        if (holder.a_type == A_SYM)
        {
          if (holder.a_w.w_sym == gDollarSymbol)
            dollarsPresent++;
          else if (holder.a_w.w_sym == gDoubleDollarSymbol)
            doubleDollarsPresent++;
          else if ((holder.a_w.w_sym == gDollarXSymbol) ||
                  (holder.a_w.w_sym == gDollarYSymbol) ||
                  (holder.a_w.w_sym == gDoubleDollarXSymbol) ||
                  (holder.a_w.w_sym == gDoubleDollarYSymbol))
            singleDollarsPresent++;
        }
        else if (holder.a_type == A_DOLLAR)
          dollarsPresent++;
        result = map2dGetNextAtomInList(nextAtom, numAtoms, inList, &holder);
        if (! result)
          break;

      }
    }
  }
  // Assemble the information and attach to the list
  if (result)
  {
    newData = GETBYTES(1, RangeData);
    if (newData)
    {
      newData->fNext = NULL_PTR;
      newData->fOutputCount = outputCount;
      newData->fDollarsPresent = dollarsPresent;
      newData->fDoubleDollarsPresent = doubleDollarsPresent;
      newData->fSingleDollarsPresent = singleDollarsPresent;
      newData->fBottomTopDontCare = bottomTopDontCare;
      newData->fLeftRightDontCare = leftRightDontCare;     
      if (! bottomTopDontCare)
      {
	      if (bottomMatch == MatchFloat)
	        setFOI2Float(newData->fBottom.fValue, bottomFloatValue);
	      else if (bottomMatch == MatchInteger)
	        setFOI2Integer(newData->fBottom.fValue, bottomIntValue);
	      if (topMatch == MatchFloat)
	        setFOI2Float(newData->fTop.fValue, topFloatValue);
	      else if (topMatch == MatchInteger)
	        setFOI2Integer(newData->fTop.fValue, topIntValue);
	      newData->fBottom.fKind = bottomMatch;
	      newData->fTop.fKind = topMatch;
	      newData->fBottom.fIsClosed = bottomClosed;
	      newData->fTop.fIsClosed = topClosed;
      }
      if (! leftRightDontCare)
      {
	      if (leftMatch == MatchFloat)
	        setFOI2Float(newData->fLeft.fValue, leftFloatValue);
	      else if (leftMatch == MatchInteger)
	        setFOI2Integer(newData->fLeft.fValue, leftIntValue);
	      if (rightMatch == MatchFloat)
	        setFOI2Float(newData->fRight.fValue, rightFloatValue);
	      else if (rightMatch == MatchInteger)
	        setFOI2Integer(newData->fRight.fValue, rightIntValue);
	      newData->fLeft.fKind = leftMatch;
	      newData->fRight.fKind = rightMatch;
	      newData->fLeft.fIsClosed = leftClosed;
	      newData->fRight.fIsClosed = rightClosed;
      }
      if (outputCount)
      {
        long  tyOffset = 0, stOffset = 0;
        PAtom vector = GETBYTES(newData->fOutputCount, Atom);

        newData->fOutput = vector;
        for (short ii = 0; ii < newData->fOutputCount; ii++, vector++)
        {
          if (binbuf_getatom(collector, &tyOffset, &stOffset, vector))
            break;

        }
      }
      else
        newData->fOutput = NULL_PTR;
    }
  }
  if (collector)
    freeobject(reinterpret_cast<PObject>(collector));
  return newData;
} /* map2dConvertListToRange */
