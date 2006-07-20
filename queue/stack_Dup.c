/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       stack_Dup.c                                                             */
/*                                                                                      */
/*  Contains:   The routine cmd_Dup().                                                  */
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
/*  Created:    2000/07/08                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "stack.h"

/*------------------------------------ cmd_Dup ---*/
Pvoid cmd_Dup
  (StackPtr xx)
{
  EnterCallback();
  if (xx)
  {
    StackDescriptorPtr descriptor = xx->fStack;

    if (xx->fVerbose)
      LOG_POST_1(OUTPUT_PREFIX "top value being duplicated on stack")
    if (descriptor)
    {
      StackEntryPtr top = descriptor->fTopOfStack;

      if (top)
      {
        StackEntryPtr newTop = GETBYTES(1, StackEntry);

        if (newTop)
        {
          PAtom temp = NULL_PTR;

          if (top->fOutputCount > 0)
          {
            temp = GETBYTES(top->fOutputCount, Atom);
            if (temp)
            {
              PAtom walker = top->fOutput;

              newTop->fOutput = temp;
              for (short ii = 0; ii < top->fOutputCount; ii++)
                *temp++ = *walker++;
            }
            else
            {
              newTop->fOutput = NULL_PTR;
              LOG_ERROR_1(OUTPUT_PREFIX "problem allocating memory for stack element")
            }
          }
          else
            newTop->fOutput = NULL_PTR;
          newTop->fNext = top;
          newTop->fOutputCount = top->fOutputCount;
          descriptor->fTopOfStack = newTop;
          descriptor->fDepth++;
        }
        else
          LOG_ERROR_1(OUTPUT_PREFIX "problem allocating space for stack element")
      }
      else
        LOG_ERROR_1(OUTPUT_PREFIX "attempt to dup top element of empty stack")
    }
  }
  ExitMaxMessageHandler()
} /* cmd_Dup */
