/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       x10units_List.cpp                                                       */
/*                                                                                      */
/*  Contains:   The routine cmd_List().                                                 */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 1996 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    1996/09/21                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "x10units.h"

/*------------------------------------ cmd_List ---*/
LIST_HEADER(X10UnitsData)
{
#pragma unused(message)
    t_atom_long num;
    t_atom_long sum = 0;
    bool        okSoFar = true;

    for (long ii = 0; okSoFar && (ii < argc); ++ii)
    {
        switch (argv[ii].a_type)
        {
            case A_LONG:
                num = argv[ii].a_w.w_long;
                if ((0 < num) && (NUM_HOUSECODES >= num))
                {
                    sum |= kUnitCodeToBits[num - 1];
                }
                else
                {
                    LOG_ERROR_3(xx, OUTPUT_PREFIX "argument %ld is out of range (" LONG_FORMAT ")",
                                ii, num)
                    okSoFar = false;
                }
                break;

            case A_SYM:
                LOG_ERROR_3(xx, OUTPUT_PREFIX "argument %ld is a symbol (%s)", ii,
                            argv[ii].a_w.w_sym->s_name)
                okSoFar = false;
                break;

            case A_FLOAT:
                LOG_ERROR_3(xx, OUTPUT_PREFIX "argument %ld is a float (%g)", ii,
                            TO_DBL(argv[ii].a_w.w_float))
                okSoFar = false;
                break;

            default:
                LOG_ERROR_2(xx, OUTPUT_PREFIX "input of an unknown type (%d) seen",
                            static_cast<int>(argv[ii].a_type))
                okSoFar = false;
                break;
                
        }
    }
    if (okSoFar)
    {
        xx->fPreviousResult = sum;
        outlet_int(xx->fResultOut, xx->fPreviousResult);
    }
} // cmd_List
