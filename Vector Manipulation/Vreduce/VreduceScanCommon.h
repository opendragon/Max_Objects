/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       VreduceScanCommon.h                                                     */
/*                                                                                      */
/*  Contains:   The data type declarations and forward references to routines.          */
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
/*  Created:    2000/11/29                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(VREDUCESCANCOMMON_H_))
# define VREDUCESCANCOMMON_H_ /* */

# include "missingAndExtra.h"
# include "genericListOutput.h"

enum OpCode
{
    OP_unknown = 0,
    OP_ADD,
    OP_AND,
    OP_BITAND,
    OP_BITOR,
    OP_DIVIDE,
    OP_EXOR,
    OP_MAX,
    OP_MIN,
    OP_MODULUS,
    OP_MULTIPLY,
    OP_OR,
    OP_SUBTRACT
}; // OpCode

enum InputRestriction
{
    IR_NONE = 0,
    IR_INTEGER = 1,
    IR_NONZERO = 2,
    IR_INTEGER_NONZERO = 3
}; // InputRestriction

bool checkInput(void *                 xx,
                const char *           name,
                const short            argc,
                t_atom *               argv,
                const InputRestriction check);

OpCode identifySymbol(t_symbol *         name,
                      InputRestriction * check);

void setUpCommonSymbols(void);

mextern(t_symbol *) gAddSymbol;      /* Pointer to unique symbol for '+' */
mextern(t_symbol *) gAndSymbol;      /* Pointer to unique symbol for '&&' */
mextern(t_symbol *) gBitAndSymbol;   /* Pointer to unique symbol for '&' */
mextern(t_symbol *) gBitOrSymbol;    /* Pointer to unique symbol for '|' */
mextern(t_symbol *) gDivideSymbol;   /* Pointer to unique symbol for '/' */
mextern(t_symbol *) gExorSymbol;     /* Pointer to unique symbol for '^' */
mextern(t_symbol *) gMaxSymbol;      /* Pointer to unique symbol for 'max' */
mextern(t_symbol *) gMinSymbol;      /* Pointer to unique symbol for 'min' */
mextern(t_symbol *) gModulusSymbol;  /* Pointer to unique symbol for '%' */
mextern(t_symbol *) gMultiplySymbol; /* Pointer to unique symbol for '*' */
mextern(t_symbol *) gOrSymbol;       /* Pointer to unique symbol for '||' */
mextern(t_symbol *) gSubtractSymbol; /* Pointer to unique symbol for '-' */

#endif /* not VREDUCESCANCOMMON_H_ */
