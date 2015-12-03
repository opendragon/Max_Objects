/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       Vexp_List.cpp                                                           */
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
/*  Created:    2001/05/21                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "Vexp.h"
#include <cfloat>

/*------------------------------------ cmd_List ---*/
LIST_HEADER(VObjectData)
{
#pragma unused(message)
    if (xx)
    {
        t_atom * newArg = NULL;
        bool     okSoFar = true;

        clearPrevious(xx);
        if (argc)
        {
            newArg = GET_BYTES(argc, t_atom);
            if (newArg)
            {
                t_atom * newWalk = newArg;
                t_atom * oldWalk = argv;
                double   newValue;

                for (long ii = 0; (ii < argc) && okSoFar; ++ii, ++newWalk, ++oldWalk)
                {
                    newWalk->a_type = oldWalk->a_type;
                    switch (oldWalk->a_type)
                    {
                        case A_FLOAT:
                            if (oldWalk->a_w.w_float < gMinInput)
                            {
                                LOG_ERROR_2(xx, OUTPUT_PREFIX "Underflow due to small value (%g) "
                                            "in input", static_cast<double>(oldWalk->a_w.w_float))
                                newValue = 0;
                            }
                            else if (oldWalk->a_w.w_float > gMaxInput)
                            {
                                LOG_ERROR_2(xx, OUTPUT_PREFIX "Overflow due to large value (%g) in "
                                            "input", static_cast<double>(oldWalk->a_w.w_float))
                                newValue = INFINITY;
                            }
                            else
                            {
                                newValue = exp(oldWalk->a_w.w_float);
                            }
                            break;

                        case A_LONG:
                            if (oldWalk->a_w.w_long < gMinInput)
                            {
                                LOG_ERROR_2(xx, OUTPUT_PREFIX "Underflow due to small value ("
                                            LONG_FORMAT ") in input", oldWalk->a_w.w_long)
                                newValue = 0;
                            }
                            else if (oldWalk->a_w.w_long > gMaxInput)
                            {
                                LOG_ERROR_2(xx, OUTPUT_PREFIX "Overflow due to large value ("
                                            LONG_FORMAT ") in input", oldWalk->a_w.w_long)
                                newValue = INFINITY;
                            }
                            else
                            {
                                newValue = exp(TO_DBL(oldWalk->a_w.w_long));
                            }
                            break;

                        default:
                            okSoFar = false;
                            break;
                            
                    }
                    if (okSoFar)
                    {
                        newWalk->a_type = A_FLOAT;
                        newWalk->a_w.w_float = TO_DBL(newValue);
                    }
                }
            }
            else
            {
                okSoFar = false;
            }
        }
        if (okSoFar)
        {
            xx->fPreviousList = newArg;
            xx->fPreviousLength = argc;
            xx->fPreviousKind = A_GIMME;
            genericListOutput(xx->fResultOut, xx->fPreviousLength, xx->fPreviousList);
        }
        else
        {
            FREE_BYTES(newArg);
            xx->fPreviousKind = A_NOTHING;
            LOG_ERROR_1(xx, OUTPUT_PREFIX "Non-numeric or invalid elements in list")
        }
    }
} // cmd_List
