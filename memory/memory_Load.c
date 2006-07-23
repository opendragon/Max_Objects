/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       memory_Load.c                                                           */
/*                                                                                      */
/*  Contains:   The routine cmd_Load().                                                 */
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

/*------------------------------------ cmd_Load ---*/
Pvoid cmd_Load
  (MemoryPtr xx,
   PSymbol   fileName)
{
  EnterCallback();
  if (xx)
  {
    PBinbuf inBuffer;
    short   theVol, ifBinary;

    if (xx->fVerbose)
      LOG_POST_1(OUTPUT_PREFIX "reading in all symbols")
    memoryInitializeHashTable(xx);
    inBuffer = static_cast<PBinbuf>(binbuf_new());
    if (locatefile(fileName->s_name, &theVol, &ifBinary))
      /* Unable to locate file - use last volume accessed. */
      theVol = defvolume();
    if (binbuf_read(inBuffer, fileName->s_name, theVol, 0))
      LOG_ERROR_1(OUTPUT_PREFIX "problem reading in the symbol table")
    else
    {
      long          stuffOffset = 0, typeOffset = 0;
      SymbolLinkPtr theSymbol;
      Atom          tempAtom;
      PAtom         atomVector;
      bool          okSoFar;
      long          numSymbols;
      short         numAtoms;

      /* Get the number of symbols: */
      okSoFar = (! binbuf_getatom(inBuffer, &typeOffset, &stuffOffset, &tempAtom));
      if (okSoFar)
      {
        if (tempAtom.a_type == A_LONG)
          numSymbols = tempAtom.a_w.w_long;
        else
        {
          LOG_ERROR_1(OUTPUT_PREFIX "number of symbols is not numeric")
          okSoFar = false;
        }
      }
      for ( ; okSoFar && (numSymbols > 0); )
      {
        okSoFar = (! binbuf_getatom(inBuffer, &typeOffset, &stuffOffset, &tempAtom));
        if (okSoFar)
        {
          if (tempAtom.a_type == A_SYM)
          {
            theSymbol = memoryAddSymbol(xx, tempAtom.a_w.w_sym);
            if (theSymbol)
            {
              FREEBYTES(theSymbol->fOutput, theSymbol->fOutputCount)
              theSymbol->fOutputCount = 0;                
            }
            else
            {
              LOG_ERROR_1(OUTPUT_PREFIX "unable to create new symbol")
              break;

            }
          }
          else
          {
            LOG_ERROR_1(OUTPUT_PREFIX "expected a symbol")
            break;

          }      
        }
        okSoFar = (! binbuf_getatom(inBuffer, &typeOffset, &stuffOffset, &tempAtom));
        if (okSoFar)
        {
          if (tempAtom.a_type == A_LONG)
          {
            numAtoms = short(tempAtom.a_w.w_long);
            if (numAtoms < 0)
              numAtoms = 0;
          }
          else
          {
            LOG_ERROR_1(OUTPUT_PREFIX "expected a number following the symbol")
            break;

          }       
        }
        /* Collect the atoms */
        if (numAtoms > 0)
        {
          atomVector = GETBYTES(numAtoms, Atom);
          if (atomVector)
          {                       
            /* Prefill the atom vector, in case of early termination. */
            for (short ii = 0; ii < numAtoms; ++ii)
              SETLONG(atomVector + ii, 0);
            for (short ii = 0; okSoFar && (ii < numAtoms); ++ii)
            {       
              okSoFar = (! binbuf_getatom(inBuffer, &typeOffset, &stuffOffset, &tempAtom));
              if (okSoFar)
                atomVector[ii] = tempAtom;
            }
          }
        }
        else
          atomVector = NULL_PTR;
        theSymbol->fOutput = atomVector;
        theSymbol->fOutputCount = numAtoms;
        --numSymbols;
      }
    }
    freeobject(reinterpret_cast<PObject>(inBuffer));
  }
  ExitMaxMessageHandler()
} /* cmd_Load */
