/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       Vmean.h                                                                 */
/*                                                                                      */
/*  Contains:   The data type declarations and forward references to routines.          */
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
/*  Created:    2001/04/06                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(VMEAN_H_))
# define VMEAN_H_ /* */

# include "missingAndExtra.h"
# include <Math.h>

# define OUR_NAME      "Vmean"
// # define OUR_RES_NUMB  17162
# define OUTPUT_PREFIX "Vmean: "

enum OpCode
{
    OP_unknown = 0,
    OP_Arithmetic,
    OP_Geometric,
    OP_Harmonic
}; // OpCode

struct VmeanData
{
    t_object   fObject;
    t_outlet * fResultOut;
    OpCode     fOperation;
    double     fPreviousFloat;
}; // VmeanData

StandardRoutineDeclarations(VmeanData *);

mextern(t_symbol *) gASymbol;     /* Pointer to unique symbol for 'a' */
mextern(t_symbol *) gArithSymbol; /* Pointer to unique symbol for 'arith' */
mextern(t_symbol *) gGSymbol;     /* Pointer to unique symbol for 'g' */
mextern(t_symbol *) gGeomSymbol;  /* Pointer to unique symbol for 'geom' */
mextern(t_symbol *) gHSymbol;     /* Pointer to unique symbol for 'h' */
mextern(t_symbol *) gHarmSymbol;  /* Pointer to unique symbol for 'harm' */

#endif /* not VMEAN_H_ */
