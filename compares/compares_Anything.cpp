/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       compares_Anything.cpp                                                   */
/*                                                                                      */
/*  Contains:   The routine cmd_Anything().                                             */
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
/*  Created:    1999/04/09                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "compares.h"

/*------------------------------------ cmd_Anything ---*/
void cmd_Anything(ComparesData * xx,
                  t_symbol *     message,
                  short          argc,
                  t_atom *       argv)
{
#pragma unused(argv)
    if (xx)
    {
        if (argc)
        {
            LOG_ERROR_1(xx, OUTPUT_PREFIX "only symbols expected")
        }
        else
        {
            long result = 0;

            switch (xx->fInletNumber)
            {
                case 0:
                    /* Compare the right and left inputs: */
                    if (message != xx->fRightInput)
                    {
                        if (message)
                        {
                            if (xx->fResultOut)
                            {
                                /* left and right are non-null: */
                                result = strcmp(message->s_name, xx->fRightInput->s_name);
                                if (result > 0)
                                {
                                    result = 1;
                                }
                                else if (result < 0)
                                {
                                    result = -1;
                                }
                            }
                            else
                            {
                                /* left is non-null and right is null: */
                                result = 1;
                            }
                        }
                        else
                        {
                            /* left is null and right is non-null: */
                            result = -1;
                        }
                    }
                    outlet_int(xx->fResultOut, result);
                    break;

                case 1:
                    /* Remember the input: */
                    xx->fRightInput = message;
                    break;

                default:
                    LOG_ERROR_2(xx, OUTPUT_PREFIX "unexpected port (%ld) seen", xx->fInletNumber)
                    break;
            }
        }
    }
} // cmd_Anything
