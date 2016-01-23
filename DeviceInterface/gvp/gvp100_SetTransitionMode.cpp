/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       gvp100_SetTransitionMode.cpp                                            */
/*                                                                                      */
/*  Contains:   The routine cmd_SetTransitionMode().                                    */
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

/*------------------------------------ cmd_SetTransitionMode ---*/
SETTRANSITIONMODE_HEADER(GvpData)
{
    if (xx)
    {
        bool okSoFar = true;
        int  mode = 0;

        if ((keyOrBkgd1 == gKeySymbol) || (keyOrBkgd1 == gKSymbol))
        {
            mode = MODE_KEY_SELECTED;
        }
        else if ((keyOrBkgd1 == gBackgroundSymbol) || (keyOrBkgd1 == gBSymbol))
        {
            mode = MODE_BKGD_SELECTED;
        }
        else
        {
            LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid mode '%s'", keyOrBkgd1->s_name)
            okSoFar = false;
        }
        if (okSoFar)
        {
            if ((keyOrBkgd2 == gKeySymbol) || (keyOrBkgd2 == gKSymbol))
            {
                mode |= MODE_KEY_SELECTED;
            }
            else if ((keyOrBkgd2 == gBackgroundSymbol) || (keyOrBkgd2 == gBSymbol))
            {
                mode |= MODE_BKGD_SELECTED;
            }
            else if (keyOrBkgd2 != gEmptySymbol)
            {
                LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid mode '%s'", keyOrBkgd2->s_name)
                okSoFar = false;
            }
        }
        if (okSoFar)
        {
            if (mode)
            {
                unsigned char dummy = static_cast<unsigned char>(mode);

                gvpPerformWriteCommand(xx, 0, kCommandSetTransitionMode, 1, &dummy,
                                       kStateAwaitingByteCount1, true);
            }
        }
        else
        {
            outlet_bang(xx->fErrorBangOut);
        }
    }
} // cmd_SetTransitionMode
