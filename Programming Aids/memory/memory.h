/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       memory.h                                                                */
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
/*  Created:    2000/06/21                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(MEMORY_H_))
# define MEMORY_H_ /* */

# include "missingAndExtra.h"
# include "genericListOutput.h"

# define OUR_DESC    "Remember arbitrary data"
# define OUR_NAME    "memory"
# define OUR_VERSION "1.0.2"
# define OUR_YEAR    "2000"

# define HASH_TABLE_SIZE 53 /* a small prime that's not too small */

struct SymbolLink
{
    SymbolLink * fNext;
    t_symbol *   fSymbol;
    t_atom *     fOutput;
    long         fOutputCount;
}; // SymbolLink

struct MemoryDescriptor
{
    SymbolLink **      fSymbolTable;
    MemoryDescriptor * fPrevious;
    MemoryDescriptor * fNext;
    t_symbol *         fTag;
    long               fReferenceCount;
}; // MemoryDescriptor

struct MemoryData
{
    t_object           fObject;
    t_outlet *         fResultOut;
    t_outlet *         fErrorBangOut;
    MemoryDescriptor * fSymbols;
    long               fSymbolCount;
    bool               fVerbose;
}; // MemoryData

# define FORGET_HEADER(type) \
    void cmd_Forget(type *     xx,\
                    t_symbol * name)

# define RECALL_HEADER(type) \
    void cmd_Recall(type *     xx,\
                    t_symbol * name)

# define REMEMBER_HEADER(type) \
    void cmd_Remember(type *     xx,\
                      t_symbol * message,\
                      const long argc,\
                      t_atom *   argv)

# define STORE_HEADER(type) \
    void cmd_Store(type *     xx,\
                   t_symbol * fileName)

CLEAR_HEADER(MemoryData);
FORGET_HEADER(MemoryData);
LOAD_HEADER(MemoryData);
RECALL_HEADER(MemoryData);
REMEMBER_HEADER(MemoryData);
STORE_HEADER(MemoryData);
TRACE_HEADER(MemoryData);

SymbolLink * memoryAddSymbol(MemoryData * xx,
                             t_symbol *   name);

void memoryClearHashTable(MemoryData * xx);

void memoryInitializeHashTable(MemoryData * xx);

SymbolLink * memoryLookupSymbol(MemoryData * xx,
                                t_symbol *   name);

void memoryRemoveSymbol(MemoryData * xx,
                        t_symbol *   name);

StandardRoutineDeclarations(MemoryData);

mextern(t_symbol *)         gEmptySymbol;  /* Pointer to unique symbol for '' */
mextern(t_symbol *)         gOffSymbol;    /* Pointer to unique symbol for 'off' */
mextern(t_symbol *)         gOnSymbol;     /* Pointer to unique symbol for 'on' */
mextern(MemoryDescriptor *) gMemoryAnchor; /* First memory descriptor */

#endif /* not MEMORY_H_ */
