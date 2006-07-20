/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       VreduceScanCommon.c                                                     */
/*                                                                                      */
/*  Contains:   Shared routines.                                                        */
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
/*  Created:    2000/11/29                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "VreduceScanCommon.h"

/*------------------------------------ CheckInput ---*/
bool CheckInput
  (Qchar            			name,
   const short            argc,
   PAtom									argv,
   const InputRestriction check)
{
  bool  okSoFar = true;

  for (short ii = 0; okSoFar && (ii < argc); ii++)
  {
    switch (argv[ii].a_type)
    {
      case A_LONG:
        if ((check & IR_NONZERO) == IR_NONZERO)
        {
          if (! argv[ii].a_w.w_long)
          {
            LOG_ERROR_2("%szero in input list", name)
            okSoFar = false;
          }
        }
        break;

      case A_SYM:
        LOG_ERROR_3("%ssymbol (%s) in input list", name, argv[ii].a_w.w_sym->s_name)
        okSoFar = false;
        break;

      case A_FLOAT:
        if ((check & IR_INTEGER) == IR_INTEGER)
        {
          LOG_ERROR_3("%sfloating point value (%g) in input list", name,
                      double(argv[ii].a_w.w_float))
          okSoFar = false;
        }
        else if ((check & IR_NONZERO) == IR_NONZERO)
        {
          if (! argv[ii].a_w.w_float)
          {
            LOG_ERROR_2("%szero in input list", name)
            okSoFar = false;
          }
        }
        break;

      default:
        LOG_ERROR_3("%sinput of an unknown type (%hd) seen", name, argv[ii].a_type)
        okSoFar = false;
        break;

    }
  }
  return okSoFar;
} /* CheckInput */

/*------------------------------------ IdentifySymbol ---*/
OpCode IdentifySymbol
  (PSymbol          name,
   InputRestrictPtr check)
{
  InputRestriction category = IR_NONE;
  OpCode           result = OP_unknown;

  if (name == gAddSymbol)
    result = OP_ADD;
  else if (name == gAndSymbol)
    result = OP_AND;
  else if (name == gBitAndSymbol)
  {
    category = IR_INTEGER;
    result = OP_BITAND;
  }
  else if (name == gBitOrSymbol)
  {
    category = IR_INTEGER;
    result = OP_BITOR;
  }
  else if (name == gDivideSymbol)
  {
    category = IR_NONZERO;
    result = OP_DIVIDE;
  }
  else if (name == gExorSymbol)
  {
    category = IR_INTEGER;
    result = OP_EXOR;
  }
  else if (name == gMaxSymbol)
    result = OP_MAX;
  else if (name == gMinSymbol)
    result = OP_MIN;
  else if (name == gModulusSymbol)
  {
    category = IR_INTEGER_NONZERO;
    result = OP_MODULUS;
  }
  else if (name == gMultiplySymbol)
    result = OP_MULTIPLY;
  else if (name == gOrSymbol)
    result = OP_OR;
  else if (name == gSubtractSymbol)
    result = OP_SUBTRACT;
  if (check)
    *check = category;
  return result;
} /* IdentifySymbol */

/*------------------------------------ SetUpCommonSymbols ---*/
void SetUpCommonSymbols
  (void)
{
  gAddSymbol = gensym("+");
  gAndSymbol = gensym("&&");
  gBitAndSymbol = gensym("&");
  gBitOrSymbol = gensym("|");
  gDivideSymbol = gensym("/");
  gExorSymbol = gensym("^");
  gMaxSymbol = gensym("max");
  gMinSymbol = gensym("min");
  gModulusSymbol = gensym("%");
  gMultiplySymbol = gensym("*");
  gOrSymbol = gensym("||");
  gSubtractSymbol = gensym("-");    
} /* SetUpCommonSymbols */
