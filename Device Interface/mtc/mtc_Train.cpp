/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       mtc_Train.cpp                                                           */
/*                                                                                      */
/*  Contains:   The routine cmd_Train().                                                */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 1999 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    1999/12/20                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "mtc.h"

/*------------------------------------ cmd_Train ---*/
TRAIN_HEADER(MtcData)
{
    if (xx)
    {
        bool oldValue = xx->fNormalizing;

        switch (xx->fInletNumber)
        {
            case 0:
                /* Normal command, normal inlet */
                if (startStop == gStopSymbol)
                {
                    xx->fNormalizing = false;
                }
                else if (startStop == gStartSymbol)
                {
                    xx->fNormalizing = true;
                }
                else
                {
                    LOG_ERROR_1(xx, OUTPUT_PREFIX "invalid argument to 'train' command.")
                    outlet_bang(xx->fErrorBangOut);
                    break;
                }
                if (xx->fNormalizing != oldValue)
                {
                    if (xx->fNormalizing)
                    {
                        LOG_POST_1(xx, OUTPUT_PREFIX "training started.")
                        mtcResetNormalization(xx);
                        if (xx->fStarted)
                        {
                            outlet_bang(xx->fCommandComplete);
                        }
                        else
                        {
                            mtcDoStart(xx);
                        }
                    }
                    else
                    {
                        LOG_POST_1(xx, OUTPUT_PREFIX "training stopped.")
                        mtcDoStop(xx);
                        if (mtcWriteNormalizationFile(xx))
                        {
                            outlet_bang(xx->fCommandComplete);
                        }
                        else
                        {
                            outlet_bang(xx->fErrorBangOut);
                        }
                    }
                }
                break;

            case 1:
                /* Normal command, wrong inlet */
                LOG_ERROR_1(xx, OUTPUT_PREFIX "unexpected command 'train' in the right inlet.")
                outlet_bang(xx->fErrorBangOut);
                break;

            default:
                LOG_ERROR_2(xx, OUTPUT_PREFIX "unexpected port (%ld) seen", xx->fInletNumber)
                break;
                
        }
    }
} // cmd_Train
