/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       gvp100_SetTakePosition.c                                                */
/*                                                                                      */
/*  Contains:   The routine cmd_SetTakePosition().                                      */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 1998 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    1998/09/08                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "gvp100.h"

/*------------------------------------ cmd_SetTakePosition ---*/
Pvoid cmd_SetTakePosition
  (GvpControlPtr xx,
   double        pos)
{
  EnterCallback();
  if (xx)
  {
    if ((pos < 0) || (pos > 100.0))
    {
      LOG_ERROR_2(OUTPUT_PREFIX "invalid take position (%g)", pos)
      outlet_bang(xx->fErrorBangOut);
    }
    else
    {
      long  temp = long(long(pos * 4095 / 100.0) << 4);
      uchar dummy[3];

      dummy[0] = 11;
      dummy[1] = static_cast<uchar>(temp & 0x00ff);
      dummy[2] = static_cast<uchar>((temp >> 8) & 0x00ff);
      gvpPerformWriteCommand(xx, 1, GVP_CMD_WRITE_ANALOG_CONTROL, 3, dummy,
                              GVP_AWAITING_BYTE_COUNT_1, true);
    }
  }
  ExitMaxMessageHandler()
} /* cmd_SetTakePosition */
