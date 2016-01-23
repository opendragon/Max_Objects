/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       memory_Load.cpp                                                         */
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

/*------------------------------------ deferred_Load ---*/
static void
deferred_Load(MemoryData * xx,
              t_symbol *   fileName,
              const long   argc,
              t_atom *     argv)
{
    t_filehandle fileRef;

    if (xx->fVerbose)
    {
        LOG_POST_1(xx, OUTPUT_PREFIX "reading in all symbols")
    }
    memoryInitializeHashTable(xx);
    if (path_opensysfile(fileName->s_name, path_getdefault(), &fileRef, PATH_READ_PERM))
    {
        LOG_ERROR_2(xx, OUTPUT_PREFIX "problem opening file '%s'", fileName->s_name)
    }
    else
    {
        t_handle   fileContents = sysmem_newhandle(0);
        t_binbuf * inBuffer = static_cast<t_binbuf *>(binbuf_new());

        if (sysfile_readtextfile(fileRef, fileContents, 0, TEXT_LB_NATIVE))
        {
            LOG_ERROR_2(xx, OUTPUT_PREFIX "problem reading file '%s'", fileName->s_name)
        }
        else if (binbuf_text(inBuffer, fileContents, sysmem_handlesize(fileContents)))
        {
            post("problem processing");
            LOG_ERROR_2(xx, OUTPUT_PREFIX "problem converting file '%s'", fileName->s_name)
        }
        else
        {
            long         stuffOffset = 0;
            long         typeOffset = 0;
            SymbolLink * theSymbol;
            t_atom       tempAtom;
            t_atom *     atomVector;
            bool         okSoFar;
            long         numSymbols;
            long         numAtoms;

            /* Get the number of symbols: */
            okSoFar = (! binbuf_getatom(inBuffer, &typeOffset, &stuffOffset, &tempAtom));
            if (okSoFar)
            {
                if (A_LONG == tempAtom.a_type)
                {
                    numSymbols = tempAtom.a_w.w_long;
                }
                else
                {
                    LOG_ERROR_1(xx, OUTPUT_PREFIX "number of symbols is not numeric")
                    okSoFar = false;
                }
            }
            for ( ; okSoFar && (0 < numSymbols); )
            {
                okSoFar = (! binbuf_getatom(inBuffer, &typeOffset, &stuffOffset, &tempAtom));
                if (okSoFar)
                {
                    if (A_SYM == tempAtom.a_type)
                    {
                        theSymbol = memoryAddSymbol(xx, tempAtom.a_w.w_sym);
                        if (theSymbol)
                        {
                            FREE_BYTES(theSymbol->fOutput);
                            theSymbol->fOutputCount = 0;
                        }
                        else
                        {
                            LOG_ERROR_1(xx, OUTPUT_PREFIX "unable to create new symbol")
                            break;
                        }
                        
                    }
                    else
                    {
                        LOG_ERROR_1(xx, OUTPUT_PREFIX "expected a symbol")
                        break;
                    }
                    
                }
                okSoFar = (! binbuf_getatom(inBuffer, &typeOffset, &stuffOffset, &tempAtom));
                if (okSoFar)
                {
                    if (A_LONG == tempAtom.a_type)
                    {
                        numAtoms = tempAtom.a_w.w_long;
                        if (0 > numAtoms)
                        {
                            numAtoms = 0;
                        }
                    }
                    else
                    {
                        LOG_ERROR_1(xx, OUTPUT_PREFIX "expected a number following the symbol")
                        break;
                    }
                    
                }
                /* Collect the atoms */
                if (0 < numAtoms)
                {
                    atomVector = GET_BYTES(numAtoms, t_atom);
                    if (atomVector)
                    {
                        /* Prefill the atom vector, in case of early termination. */
                        for (long ii = 0; ii < numAtoms; ++ii)
                        {
                            atom_setlong(atomVector + ii, 0);
                        }
                        for (long ii = 0; okSoFar && (ii < numAtoms); ++ii)
                        {
                            okSoFar = (! binbuf_getatom(inBuffer, &typeOffset, &stuffOffset,
                                                        &tempAtom));
                            if (okSoFar)
                            {
                                atomVector[ii] = tempAtom;
                            }
                        }
                    }
                }
                else
                {
                    atomVector = NULL;
                }
                theSymbol->fOutput = atomVector;
                theSymbol->fOutputCount = numAtoms;
                --numSymbols;
            }
        }
        sysfile_close(fileRef);
        sysmem_freehandle(fileContents);
        freeobject(reinterpret_cast<t_object *>(inBuffer));
    }
} // deferred_Load

/*------------------------------------ cmd_Load ---*/
LOAD_HEADER(MemoryData)
{
    if (xx)
    {
        defer(xx, reinterpret_cast<method>(deferred_Load), fileName, 0, NULL);
    }
} // cmd_Load
