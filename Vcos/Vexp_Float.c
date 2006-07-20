/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       Vexp_Float.c                                                            */
/*                                                                                      */
/*  Contains:   The routine cmd_Float().                                                */
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
/*  Created:    2001/05/21                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "Vexp.h"
#include <cfloat>

/*------------------------------------ cmd_Float ---*/
Pvoid cmd_Float
  (VObjectPtr xx,
   double     msg)
{
  EnterCallback();
  if (xx)
  {
    double newValue;

    clearPrevious(xx);
    if (msg < gMinInput)
    {
      LOG_ERROR_2(OUTPUT_PREFIX "Underflow due to small value (%g) in input", msg)
      newValue = 0;    
    }
    else if (msg > gMaxInput)
    {
      LOG_ERROR_2(OUTPUT_PREFIX "Overflow due to large value (%g) in input", msg)
      newValue = INFINITY;
    }
    else
      newValue = exp(msg);
    xx->fPreviousFloat = newValue;
    xx->fPreviousKind = A_FLOAT;
    outlet_float(xx->fResultOut, xx->fPreviousFloat);
  }
  ExitMaxMessageHandler()
} /* cmd_Float */
