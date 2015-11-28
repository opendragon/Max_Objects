/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       getListKind.cpp                                                         */
/*                                                                                      */
/*  Contains:   The routine getListKind().                                              */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2004 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2004/02/09                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "getListKind.h"

/* Pointer to unique symbol for '$' */
static t_symbol * lDollarSymbol = NULL;

/*------------------------------------ getListKind ---*/
GETLISTKIND_HEADER
{
    short result = A_NOTHING;

    if (! lDollarSymbol)
    {
        lDollarSymbol = gensym("$");
    }
    for (long ii = 0; ii < argc; ++ii)
    {
        short aType = argv[ii].a_type;

        if ((A_LONG != aType) && (A_FLOAT != aType) && (A_SYM != aType) &&
            (A_SEMI != aType) && (A_COMMA != aType) && (A_DOLLAR != aType) &&
            (A_DOLLSYM != aType))
        {
            result = A_ERROR; /* we can't work with this value! */
            break;
        }
        
        if (A_NOTHING == result)
        {
            if ((A_SYM == aType) && (argv[ii].a_w.w_sym == lDollarSymbol))
            {
                result = A_DOLLAR;
            }
            else
            {
                result = argv[ii].a_type;
            }
        }
        else if (result != argv[ii].a_type)
        {
            if ((A_SYM == aType) && (argv[ii].a_w.w_sym == lDollarSymbol))
            {
                if (A_DOLLAR == result)
                {
                    continue;
                }
                
            }
            result = A_GIMME;
            break;
        }
        
    }
    return static_cast<unsigned char>(result);
} // getListKind
