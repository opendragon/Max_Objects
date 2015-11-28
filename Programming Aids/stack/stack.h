/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       stack.h                                                                 */
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
/*  Created:    2000/06/22                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(STACK_H_))
# define STACK_H_ /* */

# include "missingAndExtra.h"
# include "genericListOutput.h"

# define OUR_NAME      "stack"
// # define OUR_RES_NUMB  17138
# define OUTPUT_PREFIX "stack: "

struct StackEntry
{
    StackEntry * fNext;
    t_atom *     fOutput;
    long         fOutputCount;
}; // StackEntry

struct StackDescriptor
{
    StackEntry *      fTopOfStack;
    StackDescriptor * fPrevious;
    StackDescriptor * fNext;
    t_symbol *        fTag;
    long              fDepth;
    long              fReferenceCount;
}; // StackDescriptor

struct StackData
{
    t_object          fObject;
    t_outlet *        fResultOut;
    t_outlet *        fDepthOut;
    t_outlet *        fErrorBangOut;
    StackDescriptor * fStack;
    bool              fVerbose;
}; // StackData

# define DEPTH_HEADER(type) \
    void cmd_Depth(type * xx)

# define DUP_HEADER(type) \
    void cmd_Dup(type * xx)

# define POP_HEADER(type) \
    void cmd_Pop(type * xx)

# define PUSH_HEADER(type) \
    void cmd_Push(type *     xx,\
                  t_symbol * message,\
                  const long argc,\
                  t_atom *   argv)

# define SWAP_HEADER(type) \
    void cmd_Swap(type * xx)

# define TOP_HEADER(type) \
    void cmd_Top(type * xx)

# define TOPPOP_HEADER(type) \
    void cmd_TopPop(type * xx)

CLEAR_HEADER(StackData);
DEPTH_HEADER(StackData);
DUP_HEADER(StackData);
POP_HEADER(StackData);
PUSH_HEADER(StackData);
SWAP_HEADER(StackData);
TOP_HEADER(StackData);
TOPPOP_HEADER(StackData);
TRACE_HEADER(StackData);

void stackClear(StackData * xx);

StandardRoutineDeclarations(StackData);

mextern(t_symbol *)        gEmptySymbol; /* Pointer to unique symbol for '' */
mextern(t_symbol *)        gOffSymbol;   /* Pointer to unique symbol for 'off' */
mextern(t_symbol *)        gOnSymbol;    /* Pointer to unique symbol for 'on' */
mextern(StackDescriptor *) gStackAnchor; /* First stack descriptor */

#endif /* not STACK_H_ */
