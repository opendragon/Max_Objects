/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       map1d_Replace.cpp                                                       */
/*                                                                                      */
/*  Contains:   The routine cmd_Replace().                                              */
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
/*  Created:    2001/05/02                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "map1d.h"

/*------------------------------------ cmd_Replace ---*/
REPLACE_HEADER(Map1dData)
{
#pragma unused(message)
    if (xx && (1 < argc))
    {
        t_atom_long num;
        
        switch (argv->a_type)
        {
            case A_LONG:
                num = argv->a_w.w_long;
                if ((0 < num) && (num <= xx->fRangeCount))
                {
                    RangeData * newRange = map1dConvertListToRange(xx, 1, argc, argv);

                    if (newRange)
                    {
                        RangeData * prev = NULL;
                        RangeData * walker = xx->fFirstRange;

                        for ( ; walker && (--num); )
                        {
                            prev = walker;
                            walker = walker->fNext;
                        }
                        if (walker)
                        {
                            newRange->fNext = walker->fNext;
                        }
                        if (prev)
                        {
                            prev->fNext = newRange;
                        }
                        else
                        {
                            xx->fFirstRange = newRange;
                        }
                        if (xx->fLastRange == walker)
                        {
                            xx->fLastRange = newRange;
                        }
                        FREE_BYTES(walker->fOutput);
                        FREE_BYTES(walker);
                    }
                }
                break;

            case A_FLOAT:
                LOG_ERROR_2(xx, OUTPUT_PREFIX "unexpected float (%g)", TO_DBL(argv->a_w.w_float))
                break;

            case A_SYM:
                LOG_ERROR_2(xx, OUTPUT_PREFIX "unexpected symbol (%s)", argv->a_w.w_sym->s_name)
                break;

            default:
                LOG_ERROR_2(xx, OUTPUT_PREFIX "input of an unknown type (%d) seen",
                            static_cast<int>(argv->a_type))
                break;
                
        }
    }
} // cmd_Replace
