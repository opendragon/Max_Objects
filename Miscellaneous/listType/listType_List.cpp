/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       listType_List.cpp                                                       */
/*                                                                                      */
/*  Contains:   The routine cmd_List().                                                 */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2000 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2000/11/14                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "listType.h"

/*------------------------------------ cmd_List ---*/
LIST_HEADER(ListTypeData)
{
#pragma unused(message)
    if (xx)
    {
        if (argc)
        {
            bool sawInteger = false;
            bool sawFloat = false;
            bool sawSymbol = false;
            bool sawUnknown = false;

            for (long ii = 0; (ii < argc) && (! sawUnknown); ++ii)
            {
                switch (argv[ii].a_type)
                {
                    case A_LONG:
                        sawInteger = true;
                        break;

                    case A_SYM:
                        sawSymbol = true;
                        break;

                    case A_FLOAT:
                        sawFloat = true;
                        break;

                    case A_SEMI:
                    case A_COMMA:
                    case A_DOLLAR:
                    case A_DOLLSYM:
                        sawSymbol = true;
                        break;

                    default:
                        sawUnknown = true;
                        break;
                        
                }
            }
            if (sawUnknown)
            {
                outlet_int(xx->fResultOut, TO_INT(TYPE_LIST_WITH_UNKNOWNS));
            }
            else if (sawSymbol)
            {
                outlet_int(xx->fResultOut, TO_INT((sawFloat || sawInteger) ? TYPE_MIXED_LIST :
                                                  TYPE_SYMBOL_LIST));
            }
            else if (sawInteger)
            {
                outlet_int(xx->fResultOut, TO_INT(sawFloat ? TYPE_NUMERIC_LIST :
                                                  TYPE_INTEGER_LIST));
            }
            else
            {
                outlet_int(xx->fResultOut, TO_INT(sawFloat ? TYPE_FLOAT_LIST : TYPE_unknown));
            }
        }
        else
        {
            outlet_int(xx->fResultOut, TO_INT(TYPE_EMPTY_LIST));
        }
    }
} // cmd_List
