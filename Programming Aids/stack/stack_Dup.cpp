/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       stack_Dup.cpp                                                           */
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
DUP_HEADER(StackData)
{
    if (xx)
    {
        StackDescriptor * descriptor = xx->fStack;

        if (xx->fVerbose)
        {
            LOG_POST_1(xx, OUTPUT_PREFIX "top value being duplicated on stack")
        }
        if (descriptor)
        {
            StackEntry * top = descriptor->fTopOfStack;

            if (top)
            {
                StackEntry * newTop = GET_BYTES(1, StackEntry);

                if (newTop)
                {
                    t_atom * temp = NULL;

                    if (0 < top->fOutputCount)
                    {
                        temp = GET_BYTES(top->fOutputCount, t_atom);
                        if (temp)
                        {
                            t_atom * walker = top->fOutput;

                            newTop->fOutput = temp;
                            for (long ii = 0; ii < top->fOutputCount; ++ii, ++temp, ++walker)
                            {
                                *temp = *walker;
                            }
                        }
                        else
                        {
                            newTop->fOutput = NULL;
                            LOG_ERROR_1(xx, OUTPUT_PREFIX "problem allocating memory for stack "
                                        "element")
                        }
                    }
                    else
                    {
                        newTop->fOutput = NULL;
                    }
                    newTop->fNext = top;
                    newTop->fOutputCount = top->fOutputCount;
                    descriptor->fTopOfStack = newTop;
                    ++descriptor->fDepth;
                }
                else
                {
                    LOG_ERROR_1(xx, OUTPUT_PREFIX "problem allocating space for stack element")
                }
            }
            else
            {
                LOG_ERROR_1(xx, OUTPUT_PREFIX "attempt to dup top element of empty stack")
            }
        }
    }
} // cmd_Dup
