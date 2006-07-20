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
 #define VREDUCESCANCOMMON_H_ /* */
 
 #include "MissingAndExtra.h"
 #include "genericListOutput.h"

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
}; /* OpCode */

enum InputRestriction
{
  IR_NONE = 0,
  IR_INTEGER = 1,
  IR_NONZERO = 2,
  IR_INTEGER_NONZERO = 3
}; /* InputRestriction */

typedef InputRestriction * InputRestrictPtr;

bool CheckInput
  (Qchar									name,
   const short						argc,
   PAtom									argv,
   const InputRestriction	check);

OpCode IdentifySymbol
  (PSymbol          name,
   InputRestrictPtr check);

void SetUpCommonSymbols
  (void);

mextern(PSymbol) gAddSymbol;      /* Pointer to unique Symbol for '+' */
mextern(PSymbol) gAndSymbol;      /* Pointer to unique Symbol for '&&' */
mextern(PSymbol) gBitAndSymbol;   /* Pointer to unique Symbol for '&' */
mextern(PSymbol) gBitOrSymbol;    /* Pointer to unique Symbol for '|' */
mextern(PSymbol) gDivideSymbol;   /* Pointer to unique Symbol for '/' */
mextern(PSymbol) gExorSymbol;     /* Pointer to unique Symbol for '^' */
mextern(PSymbol) gMaxSymbol;      /* Pointer to unique Symbol for 'max' */
mextern(PSymbol) gMinSymbol;      /* Pointer to unique Symbol for 'min' */
mextern(PSymbol) gModulusSymbol;  /* Pointer to unique Symbol for '%' */
mextern(PSymbol) gMultiplySymbol; /* Pointer to unique Symbol for '*' */
mextern(PSymbol) gOrSymbol;       /* Pointer to unique Symbol for '||' */
mextern(PSymbol) gSubtractSymbol; /* Pointer to unique Symbol for '-' */

#endif /* not VREDUCESCANCOMMON_H_ */
