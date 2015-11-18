/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       gvp100_SetTransitionRate.cpp                                            */
/*                                                                                      */
/*  Contains:   The routine cmd_SetTransitionRate().                                    */
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
/*  Created:    1998/07/11                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "gvp100.h"

/*------------------------------------ cmd_SetTransitionRate ---*/
void cmd_SetTransitionRate(GvpData *  xx,
                           t_symbol * kind,
                           long       rate,
                           t_symbol * keyOrBkgdOrDoIt1,
                           t_symbol * keyOrBkgdOrDoIt2,
                           t_symbol * keyOrBkgdOrDoIt3)
{
    if (xx)
    {
        GvpCommandCode aCommand = kNoCommand;
        int            mode = 0;
        int            doIt = 0;

        if ((kind == gAutoSymbol) || (kind == gASymbol))
        {
            aCommand = kCommandSetAutoTransRate;
        }
        else if ((kind == gDskSymbol) || (kind == gDSymbol))
        {
            aCommand = kCommandSetDskMixRate;
        }
        else if ((kind == gF2bSymbol) || (kind == gFSymbol))
        {
            aCommand = kCommandSetFadeToBlackRate;
        }
        else
        {
            LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid transition '%s'", kind->s_name)
        }
        if (aCommand != kNoCommand)
        {
            if ((keyOrBkgdOrDoIt1 == gKeySymbol) || (keyOrBkgdOrDoIt1 == gKSymbol))
            {
                mode = MODE_KEY_SELECTED;
            }
            else if ((keyOrBkgdOrDoIt1 == gBackgroundSymbol) || (keyOrBkgdOrDoIt1 == gBSymbol))
            {
                mode = MODE_BKGD_SELECTED;
            }
            else if ((keyOrBkgdOrDoIt1 == gDoItSymbol) || (keyOrBkgdOrDoIt1 == gDSymbol))
            {
                doIt = 0x0080;
            }
            else if (keyOrBkgdOrDoIt1 != gEmptySymbol)
            {
                LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid mode '%s'", keyOrBkgdOrDoIt1->s_name)
                aCommand = kNoCommand;
            }
        }
        if (aCommand != kNoCommand)
        {
            if ((keyOrBkgdOrDoIt2 == gKeySymbol) || (keyOrBkgdOrDoIt2 == gKSymbol))
            {
                mode |= MODE_KEY_SELECTED;
            }
            else if ((keyOrBkgdOrDoIt2 == gBackgroundSymbol) || (keyOrBkgdOrDoIt2 == gBSymbol))
            {
                mode |= MODE_BKGD_SELECTED;
            }
            else if ((keyOrBkgdOrDoIt2 == gDoItSymbol) || (keyOrBkgdOrDoIt2 == gDSymbol))
            {
                doIt = 0x0080;
            }
            else if (keyOrBkgdOrDoIt2 != gEmptySymbol)
            {
                LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid mode '%s'", keyOrBkgdOrDoIt2->s_name)
                aCommand = kNoCommand;
            }
        }
        if (aCommand != kNoCommand)
        {
            if ((keyOrBkgdOrDoIt3 == gKeySymbol) || (keyOrBkgdOrDoIt3 == gKSymbol))
            {
                mode |= MODE_KEY_SELECTED;
            }
            else if ((keyOrBkgdOrDoIt3 == gBackgroundSymbol) || (keyOrBkgdOrDoIt3 == gBSymbol))
            {
                mode |= MODE_BKGD_SELECTED;
            }
            else if ((keyOrBkgdOrDoIt3 == gDoItSymbol) || (keyOrBkgdOrDoIt3 == gDSymbol))
            {
                doIt = 0x0080;
            }
            else if (keyOrBkgdOrDoIt3 != gEmptySymbol)
            {
                LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid mode '%s'", keyOrBkgdOrDoIt3->s_name)
                aCommand = kNoCommand;
            }
        }
        if (aCommand != kNoCommand)
        {
            if ((rate < 0) || (rate >= 1000))
            {
                LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid rate (%ld)", rate)
                aCommand = kNoCommand;
            }
        }
        if (aCommand == kNoCommand)
        {
            outlet_bang(xx->fErrorBangOut);
        }
        else
        {
            unsigned char dummy[3];

            dummy[0] = static_cast<unsigned char>(doIt | (mode << 4) | ((rate / 100) % 10));
            dummy[1] = static_cast<unsigned char>((rate / 10) % 10);
            dummy[2] = static_cast<unsigned char>(rate % 10);
            gvpPerformWriteCommand(xx, 0, aCommand, 3, dummy, kStateAwaitingByteCount1, true);
        }
    }
} // cmd_SetTransitionRate
