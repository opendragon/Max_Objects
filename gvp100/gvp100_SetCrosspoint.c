/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       gvp100_SetCrosspoint.c                                                  */
/*                                                                                      */
/*  Contains:   The routine cmd_SetCrosspoint().                                        */
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
/*  Created:    1998/07/07                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "gvp100.h"

/*------------------------------------ cmd_SetCrosspoint ---*/
Pvoid cmd_SetCrosspoint
  (GvpControlPtr xx,
   PSymbol       ss,
   short         argc,
   PAtom         argv)
{
#pragma unused(ss)
  EnterCallback();
  if (xx)
  {
    GvpCommandCode aCommand = GVP_no_command;
    bool           okSoFar = true;
    PSymbol        aName;
    long           number;
    uchar          dummy;

    for (short jj = 0; okSoFar && (jj < argc); jj++)
    {
      switch (argv[jj].a_type)
      {
        case A_FLOAT:
          number = long(argv[jj].a_w.w_float);
          if (jj & 1)
          {
            /* odd index - validate and do it! */
            if ((number < 0) || (number > 9))
            {
              LOG_ERROR_2(OUTPUT_PREFIX "invalid crosspoint number (%g)",
                          double(argv[jj].a_w.w_float))
              okSoFar = false;
            }
            else
            {
              dummy = static_cast<uchar>(number);
              gvpPerformWriteCommand(xx, 0, aCommand, 1, &dummy,
                                      GVP_AWAITING_BYTE_COUNT_1, jj == (argc - 1));
            }
          }
          else
          {
            /* even index - invalid! */
            LOG_ERROR_2(OUTPUT_PREFIX "invalid bus name (%g)",
                        double(argv[jj].a_w.w_float))
            okSoFar = false;
          }
          break;

        case A_LONG:
          number = argv[jj].a_w.w_long;
          if (jj & 1)
          {
            /* odd index - validate and do it! */
            if ((number < 0) || (number > 9))
            {
              LOG_ERROR_2(OUTPUT_PREFIX "invalid crosspoint number (%ld)", number)
              okSoFar = false;
            }
            else
            {
              dummy = static_cast<uchar>(number);
              gvpPerformWriteCommand(xx, 0, aCommand, 1, &dummy,
                                      GVP_AWAITING_BYTE_COUNT_1, jj == (argc - 1));
            }
          }
          else
          {
            /* even index - invalid! */
            LOG_ERROR_2(OUTPUT_PREFIX "invalid bus name (%ld)", number)
            okSoFar = false;
          }
          break;

        case A_SYM:
          aName = argv[jj].a_w.w_sym;
          if (jj & 1)
          {
            /* odd index - validate and do it! */
            if (aName == gBlackSymbol)
              number = 0;
            else if ((aName == gBackgroundSymbol) || (aName == gBSymbol))
              number = 9;
            else
            {
              LOG_ERROR_2(OUTPUT_PREFIX "invalid crosspoint name '%s'", aName->s_name)
              okSoFar = false;
            }
            if (okSoFar)
            {
              dummy = static_cast<uchar>(number);
              gvpPerformWriteCommand(xx, 0, aCommand, 1, &dummy,
                                      GVP_AWAITING_BYTE_COUNT_1, jj == (argc - 1));
            }
          }
          else
          {
            /* even index - validate and remember it! */
            number = 0;
            if ((aName == gProgramSymbol) || (aName == gPgmSymbol))
              aCommand = GVP_CMD_WRITE_PROGRAM_BACKGROUND_BUS;
            else if ((aName == gPresetSymbol) || (aName == gPreviewSymbol))
              aCommand = GVP_CMD_WRITE_PRESET_BACKGROUND_BUS;
            else if ((aName == gKeySymbol) || (aName == gInsertSymbol) ||
                    (aName == gKSymbol) || (aName == gISymbol))
              aCommand = GVP_CMD_WRITE_KEY_BUS;
            else
            {
              LOG_ERROR_2(OUTPUT_PREFIX "invalid bus '%s'", aName->s_name)
              okSoFar = false;
            }
          }
          break;

        default:
          okSoFar = false;
          LOG_ERROR_2(OUTPUT_PREFIX "input of an unknown type (%hd) seen",
                      argv[jj].a_type)
          break;

      }
    }
    if (okSoFar)
    {
      if (argc & 1)
      {
        /* the last element was a solitaire - do it! */
        dummy = static_cast<uchar>(number);
        gvpPerformWriteCommand(xx, 0, aCommand, 1, &dummy, GVP_AWAITING_BYTE_COUNT_1,
                                true);
      }
    }
    else
      outlet_bang(xx->fErrorBangOut);
  }
  ExitMaxMessageHandler()
} /* cmd_SetCrosspoint */
