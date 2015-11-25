/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       gvp100_SetCrosspoint.cpp                                                */
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
void cmd_SetCrosspoint(GvpData *  xx,
                       t_symbol * ss,
                       short      argc,
                       t_atom *   argv)
{
#pragma unused(ss)
    if (xx)
    {
        GvpCommandCode aCommand = kNoCommand;
        bool           okSoFar = true;
        t_symbol *     aName;
        t_atom_long    number;
        unsigned char  dummy;

        for (short jj = 0; okSoFar && (jj < argc); ++jj)
        {
            switch (argv[jj].a_type)
            {
                case A_FLOAT:
                    number = TO_INT(argv[jj].a_w.w_float);
                    if (jj & 1)
                    {
                        /* odd index - validate and do it! */
                        if ((0 > number) || (9 < number))
                        {
                            LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid crosspoint number (%g)",
                                        TO_DBL(argv[jj].a_w.w_float))
                            okSoFar = false;
                        }
                        else
                        {
                            dummy = static_cast<unsigned char>(number);
                            gvpPerformWriteCommand(xx, 0, aCommand, 1, &dummy,
                                                   kStateAwaitingByteCount1, jj == (argc - 1));
                        }
                    }
                    else
                    {
                        /* even index - invalid! */
                        LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid bus name (%g)",
                                    TO_DBL(argv[jj].a_w.w_float))
                        okSoFar = false;
                    }
                    break;

                case A_LONG:
                    number = argv[jj].a_w.w_long;
                    if (jj & 1)
                    {
                        /* odd index - validate and do it! */
                        if ((0 > number) || (9 < number))
                        {
                            LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid crosspoint number (" LONG_FORMAT
                                        ")", number)
                            okSoFar = false;
                        }
                        else
                        {
                            dummy = static_cast<unsigned char>(number);
                            gvpPerformWriteCommand(xx, 0, aCommand, 1, &dummy,
                                                   kStateAwaitingByteCount1, jj == (argc - 1));
                        }
                    }
                    else
                    {
                        /* even index - invalid! */
                        LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid bus name (%ld)", number)
                        okSoFar = false;
                    }
                    break;

                case A_SYM:
                    aName = argv[jj].a_w.w_sym;
                    if (jj & 1)
                    {
                        /* odd index - validate and do it! */
                        if (aName == gBlackSymbol)
                        {
                            number = 0;
                        }
                        else if ((aName == gBackgroundSymbol) || (aName == gBSymbol))
                        {
                            number = 9;
                        }
                        else
                        {
                            LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid crosspoint name '%s'",
                                        aName->s_name)
                            okSoFar = false;
                        }
                        if (okSoFar)
                        {
                            dummy = static_cast<unsigned char>(number);
                            gvpPerformWriteCommand(xx, 0, aCommand, 1, &dummy,
                                                   kStateAwaitingByteCount1, jj == (argc - 1));
                        }
                    }
                    else
                    {
                        /* even index - validate and remember it! */
                        number = 0;
                        if ((aName == gProgramSymbol) || (aName == gPgmSymbol))
                        {
                            aCommand = kCommandWritePropgramBackgroundBus;
                        }
                        else if ((aName == gPresetSymbol) || (aName == gPreviewSymbol))
                        {
                            aCommand = kCommandWritePresetBackgroundBus;
                        }
                        else if ((aName == gKeySymbol) || (aName == gInsertSymbol) ||
                                 (aName == gKSymbol) || (aName == gISymbol))
                        {
                            aCommand = kCommandWriteKeyBus;
                        }
                        else
                        {
                            LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid bus '%s'", aName->s_name)
                            okSoFar = false;
                        }
                    }
                    break;

                default:
                    okSoFar = false;
                    LOG_ERROR_2(xx, OUTPUT_PREFIX "input of an unknown type (%d) seen",
                                static_cast<int>(argv[jj].a_type))
                    break;
                    
            }
        }
        if (okSoFar)
        {
            if (argc & 1)
            {
                /* the last element was a solitaire - do it! */
                dummy = static_cast<unsigned char>(number);
                gvpPerformWriteCommand(xx, 0, aCommand, 1, &dummy, kStateAwaitingByteCount1, true);
            }
        }
        else
        {
            outlet_bang(xx->fErrorBangOut);
        }
    }
} // cmd_SetCrosspoint
