/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       map3d_Set.c                                                             */
/*                                                                                      */
/*  Contains:   The routine cmd_Set().                                                  */
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
/*  Created:    2001/05/08                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "map3d.h"

/*------------------------------------ cmd_Set ---*/
Pvoid cmd_Set
  (Map3dPtr xx,
   PSymbol  message,
   short    argc,
   PAtom    argv)
{
#pragma unused(message)
  EnterCallback();
  if (xx)
  {
    if (argc == 3)
    {
      PAtom           next = argv;
      long            slot;
      bool            okSoFar = false, infPositive = true;
      PSymbol         which = NULL_PTR;
      RangeElementPtr data = NULL_PTR;

      switch (next->a_type)
      {
        case A_LONG:
          slot = next->a_w.w_long;
          okSoFar = true;
          break;

        case A_FLOAT:
          LOG_ERROR_2(OUTPUT_PREFIX "unexpected float (%g)", double(next->a_w.w_float))
          break;

        case A_SYM:
          LOG_ERROR_2(OUTPUT_PREFIX "unexpected symbol (%s)", next->a_w.w_sym->s_name)
          break;

        default:
          LOG_ERROR_2(OUTPUT_PREFIX "input of an unknown type (%hd) seen", next->a_type)
          break;

      }
      if (okSoFar)
      {
        ++next;
        switch (next->a_type)
        {
          case A_LONG:
            LOG_ERROR_2(OUTPUT_PREFIX "unexpected long (%ld)", next->a_w.w_long)
            okSoFar = false;
            break;

          case A_FLOAT:
            LOG_ERROR_2(OUTPUT_PREFIX "unexpected float (%g)", double(next->a_w.w_float))
            okSoFar = false;
            break;

          case A_SYM:
            which = next->a_w.w_sym;
            break;

          default:
            LOG_ERROR_2(OUTPUT_PREFIX "input of an unknown type (%hd) seen", next->a_type)
            okSoFar = false;
            break;

        }
        ++next;
        if (which && (slot > 0) && (slot <= xx->fRangeCount))
        {
          RangeDataPtr walker = xx->fFirstRange;

          for (long ii = 1; walker && (ii < slot); ++ii)
            walker = walker->fNext;
          if (walker)
          {
          	bool	leftRight = false, bottomTop = false, forwardBack = false;
          	
            if (which == gLeftSymbol)
            {
              infPositive = false;
              data = &walker->fLeft;
              leftRight = true;
            }
            else if (which == gRightSymbol)
            {
              data = &walker->fRight;
              leftRight = true;
            }
            else if (which == gBottomSymbol)
            {
              infPositive = false;
              data = &walker->fBottom;
              bottomTop = true;
            }
            else if (which == gTopSymbol)
            {
              data = &walker->fTop;
              bottomTop = true;
            }
            else if (which == gBackSymbol)
            {
              data = &walker->fBack;
              forwardBack = true;
            }
            else if (which == gForwardSymbol)
            {
              infPositive = false;
              data = &walker->fForward;
              forwardBack = true;
            }
            else
              LOG_ERROR_2(OUTPUT_PREFIX "unknown point (%s)", which->s_name)
            if ((leftRight && walker->fLeftRightDontCare) ||
            		(bottomTop && walker->fBottomTopDontCare) ||
            		(forwardBack && walker->fForwardBackDontCare))
            {
            	LOG_ERROR_1(OUTPUT_PREFIX "can't change don't care entry")
            	data = NULL_PTR;
            }
          }
        }
        if (data)
        {
          switch (next->a_type)
          {
            case A_LONG:
              data->fKind = MatchInteger;
              setFOI2Integer(data->fValue, next->a_w.w_long);
              break;

            case A_FLOAT:
              data->fKind = MatchFloat;
              setFOI2Float(data->fValue, next->a_w.w_float);
              break;

            case A_SYM:
              which = next->a_w.w_sym;
              if (infPositive)
              {
                if ((which == gPosInfSymbol1) || (which == gPosInfSymbol2) ||
                    (which == gPosInfSymbol3) || (which == gPosInfSymbol4))
                  data->fKind = MatchInfinity;
                else
                  LOG_ERROR_2(OUTPUT_PREFIX "unexpected symbol (%s)", which->s_name)
              }
              else
              {
                if ((which == gNegInfSymbol1) || (which == gNegInfSymbol2))
                  data->fKind = MatchInfinity;
                else
                  LOG_ERROR_2(OUTPUT_PREFIX "unexpected symbol (%s)", which->s_name)
              }
              break;

            default:
              LOG_ERROR_2(OUTPUT_PREFIX "input of an unknown type (%hd) seen", next->a_type)
              break;

          }
        }
      }
    }
    else
      LOG_ERROR_1(OUTPUT_PREFIX "incorrectly formatted request")
  }
  ExitMaxMessageHandler()
} /* cmd_Set */
