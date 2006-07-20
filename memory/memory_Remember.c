/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       memory_Remember.c                                                       */
/*                                                                                      */
/*  Contains:   The routine cmd_Remember().                                             */
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

#include "memory.h"

/*------------------------------------ cmd_Remember ---*/
Pvoid cmd_Remember
  (MemoryPtr xx,
   PSymbol   message,
   short     argc,
   PAtom     argv)
{
#pragma unused(message)
  EnterCallback();
  if (xx)
  {
    SymbolLinkPtr which;

    if (argc)
    {
      if (argv[0].a_type == A_SYM)
      {
        if (xx->fVerbose)
          LOG_POST_2(OUTPUT_PREFIX "adding symbol '%s'", argv[0].a_w.w_sym->s_name)
        which = memoryAddSymbol(xx, argv[0].a_w.w_sym);
        if (which)
        {
          FREEBYTES(which->fOutput, which->fOutputCount)
          which->fOutputCount = short(argc - 1);
          if (argc > 1)
          {
            PAtom temp = GETBYTES(argc - 1, Atom);

            if (temp)
            {
              which->fOutput = temp;
              for (short ii = 1; ii < argc; ii++)
                *temp++ = argv[ii];
            }
          }
        }
      }
      else
        LOG_ERROR_1(OUTPUT_PREFIX "invalid symbol for remember")
    }
    else
      LOG_ERROR_1(OUTPUT_PREFIX "missing symbol for remember")
  }
  ExitMaxMessageHandler()
} /* cmd_Remember */
