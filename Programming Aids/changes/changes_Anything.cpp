/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       changes_Anything.cpp                                                    */
/*                                                                                      */
/*  Contains:   The routine cmd_Anything().                                             */
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
/*  Created:    1998/10/01                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "changes.h"

/*------------------------------------ cmd_Anything ---*/
ANYTHING_HEADER(ChangesData)
{
    if (xx)
    {
        bool  passThrough = false;
        short anIndex;
        short thisIndex;

        for (short ii = 0; (! passThrough) && (ii <= xx->fLastIndex); ++ii)
        {
            anIndex = xx->fWhichIndex[ii];
            thisIndex = static_cast<short>((anIndex > 0) ? (anIndex - 1) : (argc + anIndex));
            if (anIndex)
            {
                /* We are looking at something other than the message selector */
                if ((0 <= thisIndex) && (thisIndex < argc))
                {
                    if (argv[thisIndex].a_type == xx->fPreviousValue[ii].a_type)
                    {
                        switch (argv[thisIndex].a_type)
                        {
                            case A_LONG:
                                passThrough = (argv[thisIndex].a_w.w_long !=
                                               xx->fPreviousValue[ii].a_w.w_long);
                                break;

                            case A_FLOAT:
                                passThrough = (argv[thisIndex].a_w.w_float !=
                                               xx->fPreviousValue[ii].a_w.w_float);
                                break;

                            case A_SYM:
                                passThrough = (argv[thisIndex].a_w.w_sym !=
                                               xx->fPreviousValue[ii].a_w.w_sym);
                                break;
                                
                        }
                    }
                    else
                    {
                        passThrough = true;
                    }
                }
                else
                {
                    /* We are looking at a non-existent element of the input */
                    passThrough = (A_NOTHING != xx->fPreviousValue[ii].a_type);
                }
            }
            else
            {
                /* We are looking at the message selector */
                if (A_SYM == xx->fPreviousValue[ii].a_type)
                {
                    passThrough = (xx->fPreviousValue[ii].a_w.w_sym != message);
                }
                else
                {
                    passThrough = true;
                }
            }
        }
        if (passThrough)
        {
            outlet_anything(xx->fResultOut, message, argc, argv);
            /* Remember the changed value(s) */
            for (short ii = 0; ii <= xx->fLastIndex; ++ii)
            {
                anIndex = xx->fWhichIndex[ii];
                thisIndex = static_cast<short>((anIndex > 0) ? (anIndex - 1) : (argc + anIndex));
                if (anIndex)
                {
                    /* We are looking at something other than the message selector */
                    if ((0 <= thisIndex) && (thisIndex < argc))
                    {
                        xx->fPreviousValue[ii] = argv[thisIndex];
                    }
                    else
                    {
                        /* We are looking at a non-existent element of the input */
                        xx->fPreviousValue[ii].a_type = A_NOTHING;
                    }
                }
                else
                {
                    /* We are looking at the message selector */
                    A_SETSYM(xx->fPreviousValue + ii, message);
                }
            }
        }
    }
} // cmd_Anything
