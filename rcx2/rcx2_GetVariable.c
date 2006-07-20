/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       rcx2_GetVariable.c                                                      */
/*                                                                                      */
/*  Contains:   The routine cmd_GetVariable().                                          */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2005 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2005/11/19                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "rcx2.h"

/*------------------------------------ cmd_GetVariable ---*/
Pvoid cmd_GetVariable
  (Rcx2ControlPtr xx,
   long          slot)
{
  EnterCallback();
  if (xx)
  {
    if (rcx2Synchronize(xx))
    {
      if ((slot > 0) && (slot <= RCX_NUM_VARIABLES))
      {
        long value;

        if (rcx2GetValue(xx, MAKE_RCX_VALUE(RCX_VARIABLE_TYPE, slot - 1), value))
        {
          Atom response[1];

          outlet_bang(xx->fCommandComplete);
          SETLONG(response, value);
          outlet_anything(xx->fDataOut, gVariableSymbol, 1, response);
        }
        else
          outlet_bang(xx->fErrorBangOut);
      }
      else
      {
        LOG_ERROR_2(OUTPUT_PREFIX "bad variable index (%ld)", slot)
        outlet_bang(xx->fErrorBangOut);
      }
    }
    else
      outlet_bang(xx->fErrorBangOut);
  }
  ExitMaxMessageHandler()
} /* cmd_GetVariable */
