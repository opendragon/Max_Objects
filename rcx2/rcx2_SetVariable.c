/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       rcx2_SetVariable.c                                                      */
/*                                                                                      */
/*  Contains:   The routine cmd_SetVariable().                                          */
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

/*------------------------------------ cmd_SetVariable ---*/
Pvoid cmd_SetVariable
  (Rcx2ControlPtr xx,
   long          slot,
   long          newValue)
{
  EnterCallback();
  if (xx)
  {
    if (rcx2Synchronize(xx))
    {
      if ((slot > 0) && (slot <= RCX_NUM_VARIABLES))
      {
        uchar setVariableCommand[] = { RCX_SET_VARIABLE_CMD, 0, RCX_VARIABLE_TYPE, 0, 0 };

        setVariableCommand[1] = static_cast<uchar>(slot - 1);
        setVariableCommand[3] = static_cast<uchar>(newValue & 0x0FF);
        setVariableCommand[4] = static_cast<uchar>((newValue >> 8) & 0x0FF);
        if (rcx2SendCommand(xx, setVariableCommand, sizeof(setVariableCommand),
                            RCX_SET_VARIABLE_REPLY, true))
          outlet_bang(xx->fCommandComplete);
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
} /* cmd_SetVariable */