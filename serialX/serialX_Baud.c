/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       serialX_Baud.c                                                          */
/*                                                                                      */
/*  Contains:   The routine cmd_Baud().                                                 */
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
/*  Created:    1998/06/21                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "serialX.h"

/*------------------------------------ cmd_Baud ---*/
Pvoid cmd_Baud
  (SerialXControlPtr xx,
   PSymbol           message,
   short             argc,
   PAtom             argv)
{
#pragma unused(message)
  EnterCallback();
  if (argc != 1)
    LOG_ERROR_1(OUTPUT_PREFIX "bad number of arguments")
  else
  {
    long temp = xx->fBaudRate, result;
    bool skipCheck = false, okSoFar = true;

    switch (argv->a_type)
    {
      case A_LONG:
        temp = argv->a_w.w_long;
        break;

      case A_SYM:
        result = serialXCheckSymbolBaudRate(argv->a_w.w_sym, temp);
        skipCheck = true;
        break;

      case A_FLOAT:
        temp = long(argv->a_w.w_float);
        break;

      default:
        LOG_ERROR_2(OUTPUT_PREFIX "input of an unknown type (%hd) seen", argv->a_type)
        okSoFar = false;
        break;

    }
    if (okSoFar && (temp != xx->fBaudRate))
    {
      if (! skipCheck)
        result = serialXCheckLongBaudRate(temp);
      if (result == -1)
        LOG_ERROR_1(OUTPUT_PREFIX "bad baud rate specified")
      else
      {
        xx->fBaudRate = temp;
        if ((result == baud115K) || (result == baud230K))
          xx->fBaudRateBits = baud57600;
        else
          xx->fBaudRateBits = result;
        serialXSetPortCharacteristics(xx);
      }
    }
  }
  ExitMaxMessageHandler()
} /* cmd_Baud */
