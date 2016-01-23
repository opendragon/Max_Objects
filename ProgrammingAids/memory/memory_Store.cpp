/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       memory_Store.cpp                                                        */
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

/*------------------------------------ deferred_Store ---*/
static void
deferred_Store(MemoryData * xx,
               t_symbol *   fileName,
               const long   argc,
               t_atom *     argv)
{
    bool               okSoFar = false;
    t_binbuf *         outBuffer = static_cast<t_binbuf *>(binbuf_new());
    t_atom             tempAtom[2];
    MemoryDescriptor * descriptor = xx->fSymbols;
    t_filehandle       fileRef;

    if (xx->fVerbose)
    {
        LOG_POST_1(xx, OUTPUT_PREFIX "writing out all symbols")
    }
    /* Output the number of symbols: */
    atom_setlong(tempAtom, xx->fSymbolCount);
    binbuf_append(outBuffer, NULL, 1, tempAtom);
    /* For each symbol, output it's value: */
    if (descriptor && descriptor->fSymbolTable)
    {
        SymbolLink * slot;

        for (long ii = 0; HASH_TABLE_SIZE > ii; ++ii)
        {
            for (slot = *(descriptor->fSymbolTable + ii); slot; slot = slot->fNext)
            {
                atom_setsym(tempAtom, slot->fSymbol);
                atom_setlong(tempAtom + 1, TO_INT(slot->fOutputCount));
                binbuf_append(outBuffer, NULL, 2, tempAtom);
                if (slot->fOutput && slot->fOutputCount)
                {
                    binbuf_append(outBuffer, NULL, slot->fOutputCount, slot->fOutput);
                }
            }
        }
    }
    /* Now we can write out the collected symbols: */
    if (path_opensysfile(fileName->s_name, path_getdefault(), &fileRef, PATH_WRITE_PERM))
    {
        // If unsuccessful, attempt to create the file.
        okSoFar = (! path_createsysfile(fileName->s_name, path_getdefault(), FILETYPE_TEXT,
                                        &fileRef));
    }
    else
    {
        // Successful.
        okSoFar = true;
    }
    if (okSoFar)
    {
        t_handle   contents = sysmem_newhandle(0);
        t_ptr_size outSize = 0;

        if (binbuf_totext(outBuffer, contents, &outSize))
        {
            LOG_ERROR_1(xx, OUTPUT_PREFIX "problem converting the symbol table to text")
        }
        else if (outSize)
        {
            if (sysfile_writetextfile(fileRef, contents, TEXT_LB_NATIVE))
            {
                LOG_ERROR_1(xx, OUTPUT_PREFIX "problem writing out the symbol table")
            }
        }
        sysmem_freehandle(contents);
        sysfile_close(fileRef);
    }
    freeobject(reinterpret_cast<t_object *>(outBuffer));
} // deferred_Store

/*------------------------------------ cmd_Store ---*/
STORE_HEADER(MemoryData)
{
    if (xx)
    {
        defer(xx, reinterpret_cast<method>(deferred_Store), fileName, 0, NULL);
    }
} // cmd_Store
