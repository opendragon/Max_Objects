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
 #define MEMORY_H_ /* */
 
 /*#define USE_SYSLOG /* */

 #include "MissingAndExtra.h"
 #include "genericListOutput.h"

 #define OUR_NAME      "memory"
 #define OUR_RES_NUMB  17137
 #define OUTPUT_PREFIX "memory: "
 
 #define HASH_TABLE_SIZE 53 /* a small prime that's not too small */

struct SymbolLink
{
  SymbolLink *  fNext;
  PSymbol       fSymbol;
  PAtom         fOutput;
  short         fOutputCount;
}; /* SymbolLink */

typedef SymbolLink * SymbolLinkPtr;

typedef SymbolLinkPtr * SymbolLinkHdl;

struct MemoryDescriptor
{
  SymbolLinkHdl       fSymbolTable;
  MemoryDescriptor *  fPrevious;
  MemoryDescriptor *  fNext;
  PSymbol             fTag;
  short               fReferenceCount;
}; /* MemoryDescriptor */

typedef MemoryDescriptor * MemoryDescriptorPtr;

struct MemoryData
{
  Object              fObject;
  POutlet             fResultOut;
  POutlet             fErrorBangOut;
  MemoryDescriptorPtr fSymbols;
  long                fSymbolCount;
  bool                fVerbose;
}; /* MemoryData */

typedef MemoryData * MemoryPtr;

Pvoid cmd_Clear
  (MemoryPtr xx);

Pvoid cmd_Forget
  (MemoryPtr xx,
   PSymbol   name);

Pvoid cmd_Load
  (MemoryPtr xx,
   PSymbol   fileName);

Pvoid cmd_Recall
  (MemoryPtr xx,
   PSymbol   name);

Pvoid cmd_Remember
  (MemoryPtr xx,
   PSymbol   message,
   short     argc,
   PAtom     argv);

Pvoid cmd_Store
  (MemoryPtr xx,
   PSymbol   fileName);

Pvoid cmd_Trace
  (MemoryPtr xx,
   PSymbol   onOff);

SymbolLinkPtr memoryAddSymbol
  (MemoryPtr xx,
   PSymbol   name);

void memoryClearHashTable
  (MemoryPtr xx);

void memoryInitializeHashTable
  (MemoryPtr xx);

SymbolLinkPtr memoryLookupSymbol
  (MemoryPtr xx,
   PSymbol   name);

void memoryRemoveSymbol
  (MemoryPtr xx,
   PSymbol   name);

StandardRoutineDeclarations(MemoryPtr)

mextern(PSymbol)             gEmptySymbol;  /* Pointer to unique Symbol for '' */
mextern(PSymbol)             gOffSymbol;    /* Pointer to unique Symbol for 'off' */
mextern(PSymbol)             gOnSymbol;     /* Pointer to unique Symbol for 'on' */
mextern(MemoryDescriptorPtr) gMemoryAnchor; /* First memory descriptor */

#endif /* not MEMORY_H_ */
