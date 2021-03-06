/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       spaceball_List.cpp                                                      */
/*                                                                                      */
/*  Contains:   The routine cmd_List().                                                 */
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
/*  Created:    2001/07/13                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "spaceball.h"

/*------------------------------------ cmd_List ---*/
LIST_HEADER(SpaceballData)
{
#pragma unused(message)
    if (xx)
    {
        bool okSoFar = true;

        switch (xx->fInletNumber)
        {
            case 0:
                /* Normal command, wrong inlet */
                LOG_ERROR_1(xx, OUTPUT_PREFIX "unexpected list in the left inlet.")
                outlet_bang(xx->fErrorBangOut);
                break;

            case 1:
                /* Normal command, normal inlet */
                for (long ii = 0; okSoFar && (ii < argc); ++ii)
                {
                    switch (argv[ii].a_type)
                    {
                        case A_LONG:
                            spaceballProcessResponse(xx, argv[ii].a_w.w_long);
                            break;

                        default:
                            LOG_ERROR_1(xx, OUTPUT_PREFIX "bad list in the right inlet.")
                            outlet_bang(xx->fErrorBangOut);
                            okSoFar = false;
                            break;
                            
                    }
                }
                if (! okSoFar)
                {
                    /* Reset state - TBD */
                }
                break;

            default:
                LOG_ERROR_2(xx, OUTPUT_PREFIX "unexpected port (%ld) seen", xx->fInletNumber)
                break;
                
        }
    }
} // cmd_List
