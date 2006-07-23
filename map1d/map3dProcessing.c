/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       map3dProcessing.c                                                       */
/*                                                                                      */
/*  Contains:   The low-level support routines for the map3d module.                    */
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
/*  Created:    2001/04/07                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "map3d.h"
#include <math.h>

/*------------------------------------ map3dOutputResult ---*/
void map3dOutputResult
  (Map3dPtr     xx,
   RangeDataPtr result)
{
  if (result->fDollarsPresent || result->fDoubleDollarsPresent ||
      result->fSingleDollarsPresent)
  {
    short outputCount = short(result->fOutputCount + (2 * result->fDollarsPresent) +
                              (2 * result->fDoubleDollarsPresent));
    PAtom inWalker = result->fOutput, tempList = GETBYTES(outputCount, Atom);

    if (tempList)
    {
      PAtom outWalker = tempList;

      for (short ii = 0; ii < result->fOutputCount; ++ii)
      {
        if (inWalker->a_type == A_SYM)
        {
          if (inWalker->a_w.w_sym == gDollarSymbol)
          {
            /* Output previous input */
            if (xx->fPreviousInput[0].fIsFloat)
              SETFLOAT(outWalker, getFOIFloat(xx->fPreviousInput[0]));
            else
              SETLONG(outWalker, getFOILong(xx->fPreviousInput[0]));
            if (xx->fPreviousInput[1].fIsFloat)
              SETFLOAT(outWalker + 1, getFOIFloat(xx->fPreviousInput[1]));
            else
              SETLONG(outWalker + 1, getFOILong(xx->fPreviousInput[1]));
            if (xx->fPreviousInput[2].fIsFloat)
              SETFLOAT(outWalker + 2, getFOIFloat(xx->fPreviousInput[2]));
            else
              SETLONG(outWalker + 2, getFOILong(xx->fPreviousInput[2]));
            outWalker += 3;
          }
          else if (inWalker->a_w.w_sym == gDoubleDollarSymbol)
          {
            /* Output offset of previous input */
            if (result->fLeftRightDontCare)
            {
	            if (xx->fPreviousInput[0].fIsFloat)
	              SETFLOAT(outWalker, getFOIFloat(xx->fPreviousInput[0]));
	            else
	              SETLONG(outWalker, getFOILong(xx->fPreviousInput[0]));
            }
            else
            {
	            if (result->fLeft.fKind == MatchFloat)
	              SETFLOAT(outWalker, float(getFOIValue(xx->fPreviousInput[0]) -
	                                        getFOIValue(result->fLeft.fValue)));
	            else if (result->fLeft.fKind == MatchInteger)
	            {
	              if (xx->fPreviousInput[0].fIsFloat)
	                SETFLOAT(outWalker, float(getFOIValue(xx->fPreviousInput[0]) -
	                                          getFOIValue(result->fLeft.fValue)));
	              else
	                SETLONG(outWalker, getFOILong(xx->fPreviousInput[0]) -
	                                    getFOILong(result->fLeft.fValue));
	            }
	            else if (xx->fPreviousInput[0].fIsFloat)
	              SETFLOAT(outWalker, getFOIFloat(xx->fPreviousInput[0]));
	            else
	              SETLONG(outWalker, getFOILong(xx->fPreviousInput[0]));
            }
            if (result->fBottomTopDontCare)
            {
	            if (xx->fPreviousInput[1].fIsFloat)
	              SETFLOAT(outWalker + 1, getFOIFloat(xx->fPreviousInput[2]));
	            else
	              SETLONG(outWalker + 1, getFOILong(xx->fPreviousInput[2]));
            }
            else
            {
	            if (result->fBottom.fKind == MatchFloat)
	              SETFLOAT(outWalker + 1, float(getFOIValue(xx->fPreviousInput[1]) -
	                                            getFOIValue(result->fBottom.fValue)));
	            else if (result->fBottom.fKind == MatchInteger)
	            {
	              if (xx->fPreviousInput[1].fIsFloat)
	                SETFLOAT(outWalker + 1, float(getFOIValue(xx->fPreviousInput[1]) -
	                                              getFOIValue(result->fBottom.fValue)));
	              else
	                SETLONG(outWalker + 1, getFOILong(xx->fPreviousInput[1]) -
	                                        getFOILong(result->fBottom.fValue));
	            }
	            else if (xx->fPreviousInput[1].fIsFloat)
	              SETFLOAT(outWalker + 1, getFOIFloat(xx->fPreviousInput[2]));
	            else
	              SETLONG(outWalker + 1, getFOILong(xx->fPreviousInput[2]));
            }
            if (result->fForwardBackDontCare)
            {
	            if (xx->fPreviousInput[2].fIsFloat)
	              SETFLOAT(outWalker + 2, getFOIFloat(xx->fPreviousInput[2]));
	            else
	              SETLONG(outWalker + 2, getFOILong(xx->fPreviousInput[2]));
            }
            else
            {
	            if (result->fForward.fKind == MatchFloat)
	              SETFLOAT(outWalker + 2, float(getFOIValue(xx->fPreviousInput[2]) -
	                                            getFOIValue(result->fForward.fValue)));
	            else if (result->fForward.fKind == MatchInteger)
	            {
	              if (xx->fPreviousInput[2].fIsFloat)
	                SETFLOAT(outWalker + 2, float(getFOIValue(xx->fPreviousInput[2]) -
	                                              getFOIValue(result->fForward.fValue)));
	              else
	                SETLONG(outWalker + 2, getFOILong(xx->fPreviousInput[2]) -
	                                        getFOILong(result->fBottom.fValue));
	            }
	            else if (xx->fPreviousInput[2].fIsFloat)
	              SETFLOAT(outWalker + 2, getFOIFloat(xx->fPreviousInput[2]));
	            else
	              SETLONG(outWalker + 2, getFOILong(xx->fPreviousInput[2]));
            }
            outWalker += 3;
          }
          else if (inWalker->a_w.w_sym == gDollarXSymbol)
          {
            /* Output previous input */
            if (xx->fPreviousInput[0].fIsFloat)
              SETFLOAT(outWalker, getFOIFloat(xx->fPreviousInput[0]));
            else
              SETLONG(outWalker, getFOILong(xx->fPreviousInput[0]));
            ++outWalker;
          }
          else if (inWalker->a_w.w_sym == gDollarYSymbol)
          {
            /* Output previous input */
            if (xx->fPreviousInput[1].fIsFloat)
              SETFLOAT(outWalker, getFOIFloat(xx->fPreviousInput[1]));
            else
              SETLONG(outWalker, getFOILong(xx->fPreviousInput[1]));
            ++outWalker;
          }
          else if (inWalker->a_w.w_sym == gDollarZSymbol)
          {
            /* Output previous input */
            if (xx->fPreviousInput[2].fIsFloat)
              SETFLOAT(outWalker, getFOIFloat(xx->fPreviousInput[2]));
            else
              SETLONG(outWalker, getFOILong(xx->fPreviousInput[2]));
            ++outWalker;
          }
          else if (inWalker->a_w.w_sym == gDoubleDollarXSymbol)
          {
            /* Output offset of previous input */
            if (result->fLeftRightDontCare)
            {
	            if (xx->fPreviousInput[0].fIsFloat)
	              SETFLOAT(outWalker, getFOIFloat(xx->fPreviousInput[0]));
	            else
	              SETLONG(outWalker, getFOILong(xx->fPreviousInput[0]));
            }
            else
            {
	            if (result->fLeft.fKind == MatchFloat)
	              SETFLOAT(outWalker, float(getFOIValue(xx->fPreviousInput[0]) -
	                                        getFOIValue(result->fLeft.fValue)));
	            else if (result->fLeft.fKind == MatchInteger)
	            {
	              if (xx->fPreviousInput[0].fIsFloat)
	                SETFLOAT(outWalker, float(getFOIValue(xx->fPreviousInput[0]) -
	                                          getFOIValue(result->fLeft.fValue)));
	              else
	                SETLONG(outWalker, getFOILong(xx->fPreviousInput[0]) -
	                                    getFOILong(result->fLeft.fValue));
	            }
	            else if (xx->fPreviousInput[0].fIsFloat)
	              SETFLOAT(outWalker, getFOIFloat(xx->fPreviousInput[0]));
	            else
	              SETLONG(outWalker, getFOILong(xx->fPreviousInput[0]));
            }
            ++outWalker;
          }
          else if (inWalker->a_w.w_sym == gDoubleDollarYSymbol)
          {
            /* Output offset of previous input */
            if (result->fBottomTopDontCare)
            {
	            if (xx->fPreviousInput[1].fIsFloat)
	              SETFLOAT(outWalker, getFOIFloat(xx->fPreviousInput[2]));
	            else
	              SETLONG(outWalker, getFOILong(xx->fPreviousInput[2]));
            }
            else
            {
	            if (result->fBottom.fKind == MatchFloat)
	              SETFLOAT(outWalker, float(getFOIValue(xx->fPreviousInput[1]) -
	                                        getFOIValue(result->fBottom.fValue)));
	            else if (result->fBottom.fKind == MatchInteger)
	            {
	              if (xx->fPreviousInput[1].fIsFloat)
	                SETFLOAT(outWalker, float(getFOIValue(xx->fPreviousInput[1]) -
	                                          getFOIValue(result->fBottom.fValue)));
	              else
	                SETLONG(outWalker, getFOILong(xx->fPreviousInput[1]) -
	              getFOILong(result->fBottom.fValue));
	            }
	            else if (xx->fPreviousInput[1].fIsFloat)
	              SETFLOAT(outWalker, getFOIFloat(xx->fPreviousInput[2]));
	            else
	              SETLONG(outWalker, getFOILong(xx->fPreviousInput[2]));
            }
            ++outWalker;
          }
          else if (inWalker->a_w.w_sym == gDoubleDollarZSymbol)
          {
            /* Output offset of previous input */
            if (result->fForwardBackDontCare)
            {
	            if (xx->fPreviousInput[2].fIsFloat)
	              SETFLOAT(outWalker, getFOIFloat(xx->fPreviousInput[2]));
	            else
	              SETLONG(outWalker, getFOILong(xx->fPreviousInput[2]));
            }
            else
            {
	            if (result->fForward.fKind == MatchFloat)
	              SETFLOAT(outWalker, float(getFOIValue(xx->fPreviousInput[2]) -
	                                        getFOIValue(result->fForward.fValue)));
	            else if (result->fForward.fKind == MatchInteger)
	            {
	              if (xx->fPreviousInput[2].fIsFloat)
	                SETFLOAT(outWalker, float(getFOIValue(xx->fPreviousInput[2]) -
	                                          getFOIValue(result->fForward.fValue)));
	              else
	                SETLONG(outWalker, getFOILong(xx->fPreviousInput[2]) -
	                                    getFOILong(result->fBottom.fValue));
	            }
	            else if (xx->fPreviousInput[2].fIsFloat)
	              SETFLOAT(outWalker, getFOIFloat(xx->fPreviousInput[2]));
	            else
	              SETLONG(outWalker, getFOILong(xx->fPreviousInput[2]));
            }
            ++outWalker;
          }
          else
          {
            *outWalker = *inWalker;
          	++outWalker;
          }
        }
        else if (inWalker->a_type == A_DOLLAR)
        {
          /* Output previous input */
          if (xx->fPreviousInput[0].fIsFloat)
            SETFLOAT(outWalker, getFOIFloat(xx->fPreviousInput[0]));
          else
            SETLONG(outWalker, getFOILong(xx->fPreviousInput[0]));
          if (xx->fPreviousInput[1].fIsFloat)
            SETFLOAT(outWalker + 1, getFOIFloat(xx->fPreviousInput[1]));
          else
            SETLONG(outWalker + 1, getFOILong(xx->fPreviousInput[1]));
          if (xx->fPreviousInput[2].fIsFloat)
            SETFLOAT(outWalker + 2, getFOIFloat(xx->fPreviousInput[2]));
          else
            SETLONG(outWalker + 2, getFOILong(xx->fPreviousInput[2]));
          outWalker += 3;
        }
        else
        {
          *outWalker = *inWalker;
          ++outWalker;
        }
        ++inWalker;
      }
      outlet_anything(xx->fResultOut, gResultSymbol, outputCount, tempList);
      FREEBYTES(tempList, outputCount)
    }
  }
  else
    outlet_anything(xx->fResultOut, gResultSymbol, result->fOutputCount, result->fOutput);
} /* map3dOutputResult */

/*------------------------------------ map3dProcessData ---*/
void map3dProcessData
  (Map3dPtr       				xx,
   const FloatOrInteger &	xCoord,
   const FloatOrInteger &	yCoord,
   const FloatOrInteger &	zCoord)
{
  RangeDataPtr walker = xx->fFirstRange;

  for ( ; walker; walker = walker->fNext)
  {
    RangeElement toTest = walker->fLeft;
    bool         okSoFar;

    if (walker->fLeftRightDontCare)
    	okSoFar = true;
    else if (xCoord.fIsFloat)
    {
      float inValue = getFOIFloat(xCoord);

      switch (toTest.fKind)
      {
        case MatchInfinity:
          okSoFar = true;
          break;

        case MatchFloat:
          if (toTest.fIsClosed)
            okSoFar = (inValue >= getFOIFloat(toTest.fValue));
          else
            okSoFar = (inValue > getFOIFloat(toTest.fValue));
          break;

        case MatchInteger:
          if (toTest.fIsClosed)
            okSoFar = (inValue >= getFOILong(toTest.fValue));
          else
            okSoFar = (inValue > getFOILong(toTest.fValue));
          break;

        default:
          okSoFar = false;
          break;

      }
      if (okSoFar)
      {
        toTest = walker->fRight;

        switch (toTest.fKind)
        {
          case MatchInfinity:
            okSoFar = true;
            break;

          case MatchFloat:
            if (toTest.fIsClosed)
              okSoFar = (inValue <= getFOIFloat(toTest.fValue));
            else
              okSoFar = (inValue < getFOIFloat(toTest.fValue));
            break;

          case MatchInteger:
            if (toTest.fIsClosed)
              okSoFar = (inValue <= getFOILong(toTest.fValue));
            else
              okSoFar = (inValue < getFOILong(toTest.fValue));
            break;

          default:
            okSoFar = false;
            break;

        }	            
      }
    }
    else
    {
      long inValue = getFOILong(xCoord);

      switch (toTest.fKind)
      {
        case MatchInfinity:
          okSoFar = true;
          break;

        case MatchFloat:
          if (toTest.fIsClosed)
            okSoFar = (inValue >= getFOIFloat(toTest.fValue));
          else
            okSoFar = (inValue > getFOIFloat(toTest.fValue));
          break;

        case MatchInteger:
          if (toTest.fIsClosed)
            okSoFar = (inValue >= getFOILong(toTest.fValue));
          else
            okSoFar = (inValue > getFOILong(toTest.fValue));
          break;

        default:
          okSoFar = false;
          break;

      }
      if (okSoFar)
      {
        toTest = walker->fRight;
        switch (toTest.fKind)
        {
          case MatchInfinity:
            okSoFar = true;
            break;

          case MatchFloat:
            if (toTest.fIsClosed)
              okSoFar = (inValue <= getFOIFloat(toTest.fValue));
            else
              okSoFar = (inValue < getFOIFloat(toTest.fValue));
            break;

          case MatchInteger:
            if (toTest.fIsClosed)
              okSoFar = (inValue <= getFOILong(toTest.fValue));
            else
              okSoFar = (inValue < getFOILong(toTest.fValue));
            break;

          default:
            okSoFar = false;
            break;

        }	            
      }
    }
    if (! walker->fBottomTopDontCare)
    {
	    if (yCoord.fIsFloat)
	    {
	      float inValue = getFOIFloat(yCoord);

	      if (okSoFar)
	      {
	        toTest = walker->fBottom;
	        switch (toTest.fKind)
	        {
	          case MatchInfinity:
	            okSoFar = true;
	            break;

	          case MatchFloat:
	            if (toTest.fIsClosed)
	              okSoFar = (inValue >= getFOIFloat(toTest.fValue));
	            else
	              okSoFar = (inValue > getFOIFloat(toTest.fValue));
	            break;

	          case MatchInteger:
	            if (toTest.fIsClosed)
	              okSoFar = (inValue >= getFOILong(toTest.fValue));
	            else
	              okSoFar = (inValue > getFOILong(toTest.fValue));
	            break;

	          default:
	            okSoFar = false;
	            break;

	        }
	      }
	      if (okSoFar)
	      {
	        toTest = walker->fTop;
	        switch (toTest.fKind)
	        {
	          case MatchInfinity:
	            okSoFar = true;
	            break;

	          case MatchFloat:
	            if (toTest.fIsClosed)
	              okSoFar = (inValue <= getFOIFloat(toTest.fValue));
	            else
	              okSoFar = (inValue < getFOIFloat(toTest.fValue));
	            break;

	          case MatchInteger:
	            if (toTest.fIsClosed)
	              okSoFar = (inValue <= getFOILong(toTest.fValue));
	            else
	              okSoFar = (inValue < getFOILong(toTest.fValue));
	            break;

	          default:
	            okSoFar = false;
	            break;

	        }	            
	      }
	    }
	    else
	    {
	      long inValue = getFOILong(yCoord);

	      if (okSoFar)
	      {
	        toTest = walker->fBottom;
	        switch (toTest.fKind)
	        {
	          case MatchInfinity:
	            okSoFar = true;
	            break;

	          case MatchFloat:
	            if (toTest.fIsClosed)
	              okSoFar = (inValue >= getFOIFloat(toTest.fValue));
	            else
	              okSoFar = (inValue > getFOIFloat(toTest.fValue));
	            break;

	          case MatchInteger:
	            if (toTest.fIsClosed)
	              okSoFar = (inValue >= getFOILong(toTest.fValue));
	            else
	              okSoFar = (inValue > getFOILong(toTest.fValue));
	            break;

	          default:
	            okSoFar = false;
	            break;

	        }
	      }
	      if (okSoFar)
	      {
	        toTest = walker->fTop;
	        switch (toTest.fKind)
	        {
	          case MatchInfinity:
	            okSoFar = true;
	            break;

	          case MatchFloat:
	            if (toTest.fIsClosed)
	              okSoFar = (inValue <= getFOIFloat(toTest.fValue));
	            else
	              okSoFar = (inValue < getFOIFloat(toTest.fValue));
	            break;

	          case MatchInteger:
	            if (toTest.fIsClosed)
	              okSoFar = (inValue <= getFOILong(toTest.fValue));
	            else
	              okSoFar = (inValue < getFOILong(toTest.fValue));
	            break;

	          default:
	            okSoFar = false;
	            break;

	        }	            
	      }
	    }
    }
    if (! walker->fForwardBackDontCare)
    {
	    if (zCoord.fIsFloat)
	    {
	      float inValue = getFOIFloat(zCoord);

	      if (okSoFar)
	      {
	        toTest = walker->fForward;
	        switch (toTest.fKind)
	        {
	          case MatchInfinity:
	            okSoFar = true;
	            break;

	          case MatchFloat:
	            if (toTest.fIsClosed)
	              okSoFar = (inValue >= getFOIFloat(toTest.fValue));
	            else
	              okSoFar = (inValue > getFOIFloat(toTest.fValue));
	            break;

	          case MatchInteger:
	            if (toTest.fIsClosed)
	              okSoFar = (inValue >= getFOILong(toTest.fValue));
	            else
	              okSoFar = (inValue > getFOILong(toTest.fValue));
	            break;

	          default:
	            okSoFar = false;
	            break;

	        }
	      }
	      if (okSoFar)
	      {
	        toTest = walker->fBack;
	        switch (toTest.fKind)
	        {
	          case MatchInfinity:
	            okSoFar = true;
	            break;

	          case MatchFloat:
	            if (toTest.fIsClosed)
	              okSoFar = (inValue <= getFOIFloat(toTest.fValue));
	            else
	              okSoFar = (inValue < getFOIFloat(toTest.fValue));
	            break;

	          case MatchInteger:
	            if (toTest.fIsClosed)
	              okSoFar = (inValue <= getFOILong(toTest.fValue));
	            else
	              okSoFar = (inValue < getFOILong(toTest.fValue));
	            break;

	          default:
	            okSoFar = false;
	            break;

	        }	            
	      }
	    }
	    else
	    {
	      long inValue = getFOILong(zCoord);

	      if (okSoFar)
	      {
	        toTest = walker->fForward;
	        switch (toTest.fKind)
	        {
	          case MatchInfinity:
	            okSoFar = true;
	            break;

	          case MatchFloat:
	            if (toTest.fIsClosed)
	              okSoFar = (inValue >= getFOIFloat(toTest.fValue));
	            else
	              okSoFar = (inValue > getFOIFloat(toTest.fValue));
	            break;

	          case MatchInteger:
	            if (toTest.fIsClosed)
	              okSoFar = (inValue >= getFOILong(toTest.fValue));
	            else
	              okSoFar = (inValue > getFOILong(toTest.fValue));
	            break;

	          default:
	            okSoFar = false;
	            break;

	        }
	      }
	      if (okSoFar)
	      {
	        toTest = walker->fBack;
	        switch (toTest.fKind)
	        {
	          case MatchInfinity:
	            okSoFar = true;
	            break;

	          case MatchFloat:
	            if (toTest.fIsClosed)
	              okSoFar = (inValue <= getFOIFloat(toTest.fValue));
	            else
	              okSoFar = (inValue < getFOIFloat(toTest.fValue));
	            break;

	          case MatchInteger:
	            if (toTest.fIsClosed)
	              okSoFar = (inValue <= getFOILong(toTest.fValue));
	            else
	              okSoFar = (inValue < getFOILong(toTest.fValue));
	            break;

	          default:
	            okSoFar = false;
	            break;

	        }	            
	      }
	    }
    }
    if (okSoFar)
      break;

  }
  xx->fPreviousResult = walker;
  if (walker)
  {
    // We found a match!
    xx->fPreviousInput[0] = xCoord;
    xx->fPreviousInput[1] = yCoord;
    xx->fPreviousInput[2] = zCoord;
    map3dOutputResult(xx, walker);
  }
} /* map3dProcessData */
