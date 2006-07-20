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
 #define STACK_H_ /* */
 
 /*#define USE_SYSLOG /* */

 #include "MissingAndExtra.h"
 #include "genericListOutput.h"

 #define OUR_NAME      "stack"
 #define OUR_RES_NUMB  17138
 #define OUTPUT_PREFIX "stack: "
 
struct StackEntry
{
  StackEntry *  fNext;
  PAtom         fOutput;
  short         fOutputCount;
}; /* StackEntry */

typedef StackEntry * StackEntryPtr;

struct StackDescriptor
{
  StackEntryPtr     fTopOfStack;
  StackDescriptor * fPrevious;
  StackDescriptor * fNext;
  PSymbol           fTag;
  long              fDepth;
  short             fReferenceCount;
}; /* StackDescriptor */

typedef StackDescriptor * StackDescriptorPtr;

struct StackData
{
  Object             fObject;
  POutlet            fResultOut;
  POutlet            fDepthOut;
  POutlet            fErrorBangOut;
  StackDescriptorPtr fStack;
  bool               fVerbose;
}; /* StackData */

typedef StackData * StackPtr;

Pvoid cmd_Clear
  (StackPtr xx);

Pvoid cmd_Depth
  (StackPtr xx);

Pvoid cmd_Dup
  (StackPtr xx);

Pvoid cmd_Pop
  (StackPtr xx);

Pvoid cmd_Push
  (StackPtr xx,
   PSymbol  message,
   short    argc,
   PAtom    argv);

Pvoid cmd_Swap
  (StackPtr xx);

Pvoid cmd_Top
  (StackPtr xx);

Pvoid cmd_TopPop
  (StackPtr xx);

Pvoid cmd_Trace
  (StackPtr xx,
   PSymbol  onOff);

void stackClear
  (StackPtr xx);

StandardRoutineDeclarations(StackPtr)

mextern(PSymbol)            gEmptySymbol; /* Pointer to unique Symbol for '' */
mextern(PSymbol)            gOffSymbol;   /* Pointer to unique Symbol for 'off' */
mextern(PSymbol)            gOnSymbol;    /* Pointer to unique Symbol for 'on' */
mextern(StackDescriptorPtr) gStackAnchor; /* First stack descriptor */

#endif /* not STACK_H_ */