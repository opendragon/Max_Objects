/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       stack_TopPop.c                                                          */
/*                                                                                      */
/*  Contains:   The routine cmd_TopPop().                                               */
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
/*  Created:    2000/07/09                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "stack.h"

/*------------------------------------ cmd_TopPop ---*/
Pvoid cmd_TopPop
  (StackPtr xx)
{
  EnterCallback();
  if (xx)
  {
    StackDescriptorPtr descriptor = xx->fStack;

    if (descriptor)
    {
      StackEntryPtr top = descriptor->fTopOfStack;

      if (xx->fVerbose)
        LOG_POST_1(OUTPUT_PREFIX "top element of stack being fetched and popped")
      if (top)
      {
        genericListOutput(xx->fResultOut, top->fOutputCount, top->fOutput);
        descriptor->fTopOfStack = top->fNext;
        FREEBYTES(top->fOutput, top->fOutputCount)
        FREEBYTES(top, 1)
        descriptor->fDepth--;
      }
      else
        LOG_ERROR_1(OUTPUT_PREFIX "attempt to pop empty stack")
    }
  }
  ExitMaxMessageHandler()
} /* cmd_TopPop */