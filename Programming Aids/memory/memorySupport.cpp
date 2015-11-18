/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       memorySupport.cpp                                                       */
/*                                                                                      */
/*  Contains:   The low-level support routines for the memory module.                   */
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
/*  Created:    2000/06/25                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "memory.h"

/*------------------------------------ memoryAddSymbol ---*/
SymbolLink * memoryAddSymbol(MemoryData * xx,
                             t_symbol *   name)
{
    MemoryDescriptor * descriptor = xx->fSymbols;
    SymbolLink *       slot = NULL_PTR;

    if (descriptor)
    {
        short        ii = static_cast<short>(reinterpret_cast<long>(name) % HASH_TABLE_SIZE);
        SymbolLink * prev = NULL_PTR;

        for (slot = *(descriptor->fSymbolTable + ii); slot; slot = slot->fNext)
        {
            if (slot->fSymbol == name)
            {
                break;
            }

            prev = slot;
        }
        if (! slot)
        {
            /* If this is a new Symbol, link it in. */
            slot = GETBYTES(1, SymbolLink);
            slot->fNext = NULL_PTR;
            slot->fSymbol = name;
            slot->fOutput = NULL_PTR;
            slot->fOutputCount = 0;
            if (prev)
            {
                prev->fNext = slot;
            }
            else
            {
                *(descriptor->fSymbolTable + ii) = slot;
            }
            ++xx->fSymbolCount;
        }
    }
    return slot;
} // memoryAddSymbol
/*------------------------------------ memoryClearHashTable ---*/
void memoryClearHashTable(MemoryData * xx)
{
    MemoryDescriptor * descriptor = xx->fSymbols;

    if (descriptor)
    {
        if (descriptor->fSymbolTable)
        {
            SymbolLink * slot;
            SymbolLink * next;

            for (short ii = 0; ii < HASH_TABLE_SIZE; ++ii)
            {
                for (slot = *(descriptor->fSymbolTable + ii); slot; slot = next)
                {
                    next = slot->fNext;
                    FREEBYTES(slot->fOutput, slot->fOutputCount);
                    FREEBYTES(slot, 1);
                }
            }
            FREEBYTES(descriptor->fSymbolTable, HASH_TABLE_SIZE);
        }
    }
    xx->fSymbolCount = 0;
} // memoryClearHashTable
/*------------------------------------ memoryInitializeHashTable ---*/
void memoryInitializeHashTable(MemoryData * xx)
{
    MemoryDescriptor * descriptor = xx->fSymbols;

    memoryClearHashTable(xx);
    if (descriptor)
    {
        descriptor->fSymbolTable = GETBYTES(HASH_TABLE_SIZE, SymbolLink *);
        if (descriptor->fSymbolTable)
        {
            for (short ii = 0; ii < HASH_TABLE_SIZE; ++ii)
            {
                *(descriptor->fSymbolTable + ii) = NULL_PTR;
            }
        }
    }
} // memoryInitializeHashTable
/*------------------------------------ memoryLookupSymbol ---*/
SymbolLink * memoryLookupSymbol(MemoryData * xx,
                                t_symbol *   name)
{
    MemoryDescriptor * descriptor = xx->fSymbols;
    SymbolLink *       slot = NULL_PTR;

    if (descriptor)
    {
        short ii = static_cast<short>(reinterpret_cast<long>(name) % HASH_TABLE_SIZE);

        for (slot = *(descriptor->fSymbolTable + ii); slot; slot = slot->fNext)
        {
            if (slot->fSymbol == name)
            {
                break;
            }
        }
    }
    return slot;
} // memoryLookupSymbol
/*------------------------------------ memoryRemoveSymbol ---*/
void memoryRemoveSymbol(MemoryData * xx,
                        t_symbol *   name)
{
    MemoryDescriptor * descriptor = xx->fSymbols;

    if (descriptor)
    {
        short        ii = static_cast<short>(reinterpret_cast<long>(name) % HASH_TABLE_SIZE);
        SymbolLink * prev = NULL_PTR;
        SymbolLink * slot = *(descriptor->fSymbolTable + ii);

        for ( ; slot; slot = slot->fNext)
        {
            if (slot->fSymbol == name)
            {
                break;
            }

            prev = slot;
        }
        if (slot)
        {
            if (prev)
            {
                prev->fNext = slot->fNext;
            }
            else
            {
                *(descriptor->fSymbolTable + ii) = slot->fNext;
            }
            FREEBYTES(slot->fOutput, slot->fOutputCount);
            FREEBYTES(slot, 1);
            --xx->fSymbolCount;
        }
    }
} // memoryRemoveSymbol
