/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       memory_Store.c                                                          */
/*                                                                                      */
/*  Contains:   The routine cmd_Store().                                                */
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

/*------------------------------------ cmd_Store ---*/
Pvoid cmd_Store
  (MemoryPtr xx,
   PSymbol   fileName)
{
  EnterCallback();
  if (xx)
  {
    PBinbuf             outBuffer;
    Atom                tempAtom[2];
    MemoryDescriptorPtr descriptor = xx->fSymbols;
    short               theVol, ifBinary;

    if (xx->fVerbose)
      LOG_POST_1(OUTPUT_PREFIX "writing out all symbols")
    outBuffer = static_cast<PBinbuf>(binbuf_new());
    /* Output the number of symbols: */
    SETLONG(tempAtom, xx->fSymbolCount);
    binbuf_append(outBuffer, NULL_PTR, 1, tempAtom);
    /* For each symbol, output it's value: */
    if (descriptor)
    {
      if (descriptor->fSymbolTable)
      {
        SymbolLinkPtr slot;

        for (short ii = 0; ii < HASH_TABLE_SIZE; ii++)
        {
          slot = *(descriptor->fSymbolTable + ii);
          while (slot)
          {
            SETSYM(tempAtom, slot->fSymbol);
            SETLONG(tempAtom + 1, long(slot->fOutputCount));
            binbuf_append(outBuffer, NULL_PTR, 2, tempAtom);
            if (slot->fOutput && slot->fOutputCount)
              binbuf_append(outBuffer, NULL_PTR, slot->fOutputCount, slot->fOutput);
            slot = slot->fNext;
          }
        }
      }
    }
    /* Now we can write out the collected symbols: */
    if (locatefile(fileName->s_name, &theVol, &ifBinary))
      /* Unable to locate file - use last volume accessed. */
      theVol = defvolume();
    if (binbuf_write(outBuffer, fileName->s_name, theVol, 0))
      LOG_ERROR_1(OUTPUT_PREFIX "problem writing out the symbol table")
    freeobject(reinterpret_cast<PObject>(outBuffer));
  }
  ExitMaxMessageHandler()
} /* cmd_Store */
