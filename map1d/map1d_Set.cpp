/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       map1d_Set.cpp                                                           */
/*                                                                                      */
/*  Contains:   The routine cmd_Set().                                                  */
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
/*  Created:    2001/05/08                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "map1d.h"

/*------------------------------------ cmd_Set ---*/
void cmd_Set(Map1dData * xx,
             t_symbol *  message,
             short       argc,
             t_atom *    argv)
{
#pragma unused(message)
    if (xx)
    {
        if (3 == argc)
        {
            t_atom *       next = argv;
            long           slot;
            bool           okSoFar = false;
            bool           infPositive = true;
            t_symbol *     which = NULL_PTR;
            RangeElement * data = NULL_PTR;

            switch (next->a_type)
            {
                case A_LONG:
                    slot = next->a_w.w_long;
                    okSoFar = true;
                    break;

                case A_FLOAT:
                    LOG_ERROR_2(xx, OUTPUT_PREFIX "unexpected float (%g)", double(next->a_w.w_float))
                    break;

                case A_SYM:
                    LOG_ERROR_2(xx, OUTPUT_PREFIX "unexpected symbol (%s)", next->a_w.w_sym->s_name)
                    break;

                default:
                    LOG_ERROR_2(xx, OUTPUT_PREFIX "input of an unknown type (%d) seen", static_cast<int>(next->a_type))
                    break;
            }
            if (okSoFar)
            {
                ++next;
                switch (next->a_type)
                {
                    case A_LONG:
                        LOG_ERROR_2(xx, OUTPUT_PREFIX "unexpected long (%ld)", next->a_w.w_long)
                        okSoFar = false;
                        break;

                    case A_FLOAT:
                        LOG_ERROR_2(xx, OUTPUT_PREFIX "unexpected float (%g)", double(next->a_w.w_float))
                        okSoFar = false;
                        break;

                    case A_SYM:
                        which = next->a_w.w_sym;
                        break;

                    default:
                        LOG_ERROR_2(xx, OUTPUT_PREFIX "input of an unknown type (%d) seen",
                                    static_cast<int>(next->a_type))
                        okSoFar = false;
                        break;
                }
                ++next;
                if (which && (slot > 0) && (slot <= xx->fRangeCount))
                {
                    RangeData * walker = xx->fFirstRange;

                    for (long ii = 1; walker && (ii < slot); ++ii)
                    {
                        walker = walker->fNext;
                    }
                    if (walker)
                    {
                        if (walker->fLowerUpperDontCare)
                        {
                            LOG_ERROR_1(xx, OUTPUT_PREFIX "can't change don't care entry")
                        }
                        else if (which == gLowerSymbol)
                        {
                            infPositive = false;
                            data = &walker->fLower;
                        }
                        else if (which == gUpperSymbol)
                        {
                            data = &walker->fUpper;
                        }
                        else
                        {
                            LOG_ERROR_2(xx, OUTPUT_PREFIX "unknown point (%s)", which->s_name)
                        }
                    }
                }
                if (data)
                {
                    switch (next->a_type)
                    {
                        case A_LONG:
                            data->fKind = MatchInteger;
                            setFOI2Integer(data->fValue, next->a_w.w_long);
                            break;

                        case A_FLOAT:
                            data->fKind = MatchFloat;
                            setFOI2Float(data->fValue, next->a_w.w_float);
                            break;

                        case A_SYM:
                            which = next->a_w.w_sym;
                            if (infPositive)
                            {
                                if ((which == gPosInfSymbol1) || (which == gPosInfSymbol2) ||
                                    (which == gPosInfSymbol3) || (which == gPosInfSymbol4))
                                {
                                    data->fKind = MatchInfinity;
                                }
                                else
                                {
                                    LOG_ERROR_2(xx, OUTPUT_PREFIX "unexpected symbol (%s)", which->s_name)
                                }
                            }
                            else if ((which == gNegInfSymbol1) || (which == gNegInfSymbol2))
                            {
                                data->fKind = MatchInfinity;
                            }
                            else
                            {
                                LOG_ERROR_2(xx, OUTPUT_PREFIX "unexpected symbol (%s)", which->s_name)
                            }
                            break;

                        default:
                            LOG_ERROR_2(xx, OUTPUT_PREFIX "input of an unknown type (%d) seen",
                                        static_cast<int>(next->a_type))
                            break;
                    }
                }
            }
        }
        else
        {
            LOG_ERROR_1(xx, OUTPUT_PREFIX "incorrectly formatted request")
        }
    }
} // cmd_Set
