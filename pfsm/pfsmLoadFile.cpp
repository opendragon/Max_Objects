/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       pfsmLoadFile.cpp                                                        */
/*                                                                                      */
/*  Contains:   The low-level support routines for the pfsm module.                     */
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
/*  Created:    2000/06/05                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "pfsm.h"

/*------------------------------------ pfsmClearHashTable ---*/
void pfsmClearHashTable(PfsmData * xx)
{
    if (xx && xx->fStateSymbols)
    {
        for (short ii = 0; ii < HASH_TABLE_SIZE; ++ii)
        {
            SymbolLink * slot = *(xx->fStateSymbols + ii);

            for (SymbolLink * next; slot; slot = next)
            {
                TransitionData * walker = slot->fTransitions;

                next = slot->fNext;
                for (TransitionData * trans; walker; walker = trans)
                {
                    trans = walker->fNext;
                    FREEBYTES(walker->fOutput, walker->fOutputCount);
                    FREEBYTES(walker, 1);
                }
                FREEBYTES(slot, 1);
            }
        }
        FREEBYTES(xx->fStateSymbols, HASH_TABLE_SIZE);
    }
} // pfsmClearHashTable
/*------------------------------------ pfsmReportHashTable ---*/
void pfsmReportHashTable(PfsmData * xx)
{
    if (xx && xx->fStateSymbols)
    {
        const char * stateDescriptor;
        const char * outputTag;

        for (short ii = 0; ii < HASH_TABLE_SIZE; ++ii)
        {
            for (SymbolLink * slot = *(xx->fStateSymbols + ii); slot; slot = slot->fNext)
            {
                if (slot->fIsError)
                {
                    stateDescriptor = OUTPUT_PREFIX " error state %s ";
                }
                else if (slot->fIsStop)
                {
                    stateDescriptor = OUTPUT_PREFIX " stop state %s ";
                }
                else if (slot == xx->fStartState)
                {
                    stateDescriptor = OUTPUT_PREFIX " start state %s ";
                }
                else
                {
                    stateDescriptor = OUTPUT_PREFIX " state %s ";
                }
                LOG_POST_2(xx, stateDescriptor, slot->fSymbol->s_name)
                for (TransitionData * trans = slot->fTransitions; trans; trans = trans->fNext)
                {
                    char * nextName = trans->fNextState->fSymbol->s_name;

                    outputTag = (trans->fOutputCount ? "with output:" : "");
                    switch (trans->fKind)
                    {
                        case MatchAnyNumber:
                            LOG_POST_3(xx, OUTPUT_PREFIX "  any_number -> %s %s", nextName, outputTag)
                            break;

                        case MatchAnySymbol:
                            LOG_POST_3(xx, OUTPUT_PREFIX "  any_string -> %s %s", nextName, outputTag)
                            break;

                        case MatchNumber:
                            LOG_POST_4(xx, OUTPUT_PREFIX "  %ld -> %s %s", trans->fMatch.fAsNumber, nextName, outputTag)
                            break;

                        case MatchNumberRange:
                            if (trans->fMatch.fAsRange.fLowerIsOpen)
                            {
                                if (trans->fMatch.fAsRange.fUpperIsOpen)
                                {
                                    LOG_POST_3(xx, OUTPUT_PREFIX "  range any_number ... any_number -> %s %s", nextName,
                                               outputTag)
                                }
                                else
                                {
                                    LOG_POST_4(xx, OUTPUT_PREFIX "  range any_number ... %ld -> %s %s",
                                               trans->fMatch.fAsRange.fUpper, nextName, outputTag)
                                }
                            }
                            else if (trans->fMatch.fAsRange.fUpperIsOpen)
                            {
                                LOG_POST_4(xx, OUTPUT_PREFIX "  range %ld ... any_number -> %s %s",
                                           trans->fMatch.fAsRange.fLower, nextName, outputTag)
                            }
                            else
                            {
                                LOG_POST_5(xx, OUTPUT_PREFIX "  range %ld ... %ld -> %s %s",
                                           trans->fMatch.fAsRange.fLower, trans->fMatch.fAsRange.fUpper,
                                           nextName, outputTag)
                            }
                            break;

                        case MatchSymbol:
                            LOG_POST_4(xx, OUTPUT_PREFIX "  %s -> %s %s", trans->fMatch.fAsSymbol->s_name, nextName,
                                       outputTag)
                            break;

                        case MatchSpecial:
                            LOG_POST_4(xx, OUTPUT_PREFIX "  %c -> %s %s", trans->fMatch.fAsSpecial, nextName, outputTag)
                            break;

                        default:
                            break;
                    }
                    if (trans->fOutputCount)
                    {
                        for (short jj = 0; jj < trans->fOutputCount; ++jj)
                        {
                            postatom(trans->fOutput + jj);
                        }
                    }
                    if (trans->fIsRandom)
                    {
                        LOG_POST_2(xx, OUTPUT_PREFIX "    with probability %g", trans->fProbability)
                    }
                }
            }
        }
    }
} // pfsmReportHashTable
/*------------------------------------ pfsmInitializeHashTable ---*/
static void pfsmInitializeHashTable(PfsmData * xx)
{
    pfsmClearHashTable(xx);
    xx->fStateSymbols = GETBYTES(HASH_TABLE_SIZE, SymbolLink *);
    if (xx->fStateSymbols)
    {
        for (short ii = 0; ii < HASH_TABLE_SIZE; ++ii)
        {
            *(xx->fStateSymbols + ii) = NULL_PTR;
        }
    }
} // pfsmInitializeHashTable
/*------------------------------------ pfsmAddStateSymbol ---*/
static SymbolLink * pfsmAddStateSymbol(PfsmData * xx,
                                       t_symbol * name)
{
    short        ii = static_cast<short>(reinterpret_cast<long>(name) % HASH_TABLE_SIZE);
    SymbolLink * prev = NULL_PTR;
    SymbolLink * slot = *(xx->fStateSymbols + ii);

    for ( ; slot; slot = slot->fNext)
    {
        if (slot->fSymbol == name)
        {
            break;
        }

        prev = slot;
    }
    /* Note that we should never have duplicates, so we just use this as */
    /* a safety valve */
    if (! slot)
    {
        slot = GETBYTES(1, SymbolLink);
        slot->fNext = NULL_PTR;
        slot->fSymbol = name;
        slot->fTransitions = NULL_PTR;
        slot->fIsError = false;
        slot->fIsStop = false;
        if (prev)
        {
            prev->fNext = slot;
        }
        else
        {
            *(xx->fStateSymbols + ii) = slot;
        }
    }
    return slot;
} // pfsmAddStateSymbol
/*------------------------------------ pfsmLookupStateSymbol ---*/
SymbolLink * pfsmLookupStateSymbol(PfsmData * xx,
                                   t_symbol * name)
{
    short        ii = static_cast<short>(reinterpret_cast<long>(name) % HASH_TABLE_SIZE);
    SymbolLink * slot = *(xx->fStateSymbols + ii);

    for ( ; slot; slot = slot->fNext)
    {
        if (slot->fSymbol == name)
        {
            break;
        }
    }
    return slot;
} // pfsmLookupStateSymbol
/*------------------------------------ pfsmGetNextAtom ---*/
static bool pfsmGetNextAtom(PfsmData * xx,
                            t_atom &   value)
{
    t_atom skipper;
    bool   result = true;

    for ( ; result; )
    {
        result = (! binbuf_getatom(xx->fBuffer, &xx->fBufferTypeOffset, &xx->fBufferStuffOffset, &value));
        if (result && (A_SYM == value.a_type))
        {
            if (kCommentCharacter == value.a_w.w_sym->s_name[0])
            {
                /* skip a comment */
                for ( ; result; )
                {
                    result = (! binbuf_getatom(xx->fBuffer, &xx->fBufferTypeOffset, &xx->fBufferStuffOffset, &skipper));
                    if (A_SEMI == skipper.a_type)
                    {
                        break;
                    }
                }
                /* At this point, we've read the trailing semicolon, so we need to get the next atom. */
            }
            else
            {
                /* We've seen a symbol other than '#' */
                break;
            }
        }
        else
        {
            /* We've seen something other than a symbol, so we can leave... */
            break;
        }
    }
    return result;
} // pfsmGetNextAtom
/*------------------------------------ pfsmReportUnexpected ---*/
static void pfsmReportUnexpected(PfsmData * xx,
                                 t_atom &   what)
{
    switch (what.a_type)
    {
        case A_FLOAT:
            LOG_ERROR_2(xx, OUTPUT_PREFIX "unexpected floating point (%g)", static_cast<double>(what.a_w.w_float))
            break;

        case A_LONG:
            LOG_ERROR_2(xx, OUTPUT_PREFIX "unexpected long (%ld)", what.a_w.w_long)
            break;

        case A_SYM:
            LOG_ERROR_2(xx, OUTPUT_PREFIX "unexpected symbol (%s)", what.a_w.w_sym->s_name)
            break;

        case A_SEMI:
            LOG_ERROR_1(xx, OUTPUT_PREFIX "unexpected semicolon")
            break;

        case A_COMMA:
            LOG_ERROR_1(xx, OUTPUT_PREFIX "unexpected comma")
            break;

        case A_DOLLAR:
            LOG_ERROR_1(xx, OUTPUT_PREFIX "unexpected dollar")
            break;

        default:
            LOG_ERROR_2(xx, OUTPUT_PREFIX "unexpected atom, type=%d", static_cast<int>(what.a_type))
            break;
    }
} // pfsmReportUnexpected
/*------------------------------------ pfsmGetStateSymbols ---*/
static bool pfsmGetStateSymbols(PfsmData * xx)
{
    t_atom holder;
    bool   result = pfsmGetNextAtom(xx, holder);

    if (result)
    {
        if ((A_SYM == holder.a_type) && (holder.a_w.w_sym == gOpenSquareSymbol))
        {
            result = pfsmGetNextAtom(xx, holder);
        }
        else
        {
            pfsmReportUnexpected(xx, holder);
            result = false;
        }
    }
    for ( ; result; )
    {
        if (A_SYM == holder.a_type)
        {
            if (holder.a_w.w_sym == gCloseSquareSymbol)
            {
                /* last symbol seen, complete processing. */
                if (xx->fVerbose)
                {
                    LOG_POST_1(xx, OUTPUT_PREFIX "read state symbols")
                }
                break;
            }
            /* Add this symbol to the set of known symbols. */
            if (pfsmLookupStateSymbol(xx, holder.a_w.w_sym))
            {
                LOG_ERROR_2(xx, OUTPUT_PREFIX "state symbol '%s' already present", holder.a_w.w_sym->s_name)
                result = false;
            }
            else
            {
                pfsmAddStateSymbol(xx, holder.a_w.w_sym);
                result = pfsmGetNextAtom(xx, holder);
            }
        }
        else
        {
            pfsmReportUnexpected(xx, holder);
            result = false;
        }
    }
    return result;
} // pfsmGetStateSymbols
/*------------------------------------ pfsmGetErrorSymbols ---*/
static bool pfsmGetErrorSymbols(PfsmData * xx)
{
    t_atom       holder;
    bool         result = pfsmGetNextAtom(xx, holder);
    SymbolLink * symbol;

    if (result)
    {
        if ((A_SYM == holder.a_type) && (holder.a_w.w_sym == gOpenSquareSymbol))
        {
            result = pfsmGetNextAtom(xx, holder);
        }
        else
        {
            pfsmReportUnexpected(xx, holder);
            result = false;
        }
    }
    for ( ; result; )
    {
        if (A_SYM == holder.a_type)
        {
            if (holder.a_w.w_sym == gCloseSquareSymbol)
            {
                /* last symbol seen, complete processing. */
                if (xx->fVerbose)
                {
                    LOG_POST_1(xx, OUTPUT_PREFIX "read error states")
                }
                break;
            }
            /* Locate the symbol and update it's information. */
            symbol = pfsmLookupStateSymbol(xx, holder.a_w.w_sym);
            if (symbol)
            {
                symbol->fIsError = true;
                result = pfsmGetNextAtom(xx, holder);
            }
            else
            {
                LOG_ERROR_2(xx, OUTPUT_PREFIX "unknown error state symbol '%s'", holder.a_w.w_sym->s_name)
                result = false;
            }
        }
        else
        {
            pfsmReportUnexpected(xx, holder);
            result = false;
        }
    }
    return result;
} // pfsmGetErrorSymbols
/*------------------------------------ pfsmGetStartSymbol ---*/
static bool pfsmGetStartSymbol(PfsmData * xx)
{
    t_atom       holder;
    bool         result = pfsmGetNextAtom(xx, holder);
    SymbolLink * symbol;

    if (result)
    {
        if (A_SYM == holder.a_type)
        {
            symbol = pfsmLookupStateSymbol(xx, holder.a_w.w_sym);
            if (symbol)
            {
                xx->fStartState = symbol;
                if (xx->fVerbose)
                {
                    LOG_POST_1(xx, OUTPUT_PREFIX "read start state")
                }
            }
            else
            {
                LOG_ERROR_2(xx, OUTPUT_PREFIX "start state symbol '%s' not known", holder.a_w.w_sym->s_name)
                result = false;
            }
        }
        else
        {
            pfsmReportUnexpected(xx, holder);
            result = false;
        }
    }
    return result;
} // pfsmGetStartSymbol
/*------------------------------------ pfsmGetStopSymbols ---*/
static bool pfsmGetStopSymbols(PfsmData * xx)
{
    t_atom       holder;
    bool         result = pfsmGetNextAtom(xx, holder);
    SymbolLink * symbol;

    if (result)
    {
        if ((A_SYM == holder.a_type) && (holder.a_w.w_sym == gOpenSquareSymbol))
        {
            result = pfsmGetNextAtom(xx, holder);
        }
        else
        {
            pfsmReportUnexpected(xx, holder);
            result = false;
        }
    }
    for ( ; result; )
    {
        if (A_SYM == holder.a_type)
        {
            if (holder.a_w.w_sym == gCloseSquareSymbol)
            {
                /* last symbol seen, complete processing. */
                if (xx->fVerbose)
                {
                    LOG_POST_1(xx, OUTPUT_PREFIX "read stop states")
                }
                break;
            }
            /* Locate the symbol and update it's information. */
            symbol = pfsmLookupStateSymbol(xx, holder.a_w.w_sym);
            if (symbol)
            {
                symbol->fIsStop = true;
                result = pfsmGetNextAtom(xx, holder);
            }
            else
            {
                LOG_ERROR_2(xx, OUTPUT_PREFIX "unknown stop state symbol '%s'", holder.a_w.w_sym->s_name)
                result = false;
            }
        }
        else
        {
            pfsmReportUnexpected(xx, holder);
            result = false;
        }
    }
    return result;
} // pfsmGetStopSymbols
/*------------------------------------ pfsmGetNextNumber ---*/
static bool pfsmGetNextNumber(PfsmData * xx,
                              long &     value,
                              bool &     isOpenEnded)
{
    t_atom holder;
    bool   result = pfsmGetNextAtom(xx, holder);

    if (result)
    {
        if (A_LONG == holder.a_type)
        {
            value = holder.a_w.w_long;
            isOpenEnded = false;
        }
        else if (A_SYM == holder.a_type)
        {
            if (holder.a_w.w_sym == gAnyNumberSymbol)
            {
                value = 0;
                isOpenEnded = true;
            }
            else
            {
                /* Check for syntactic sugar: 'x is the same as the ASCII for the given */
                /* character. */
                const char * matcher = holder.a_w.w_sym->s_name;

                if (kSugarCharacter == *matcher)
                {
                    if (matcher[1] && (! matcher[2]))
                    {
                        value = matcher[1];
                        isOpenEnded = false;
                    }
                    else
                    {
                        result = false;
                    }
                }
                else
                {
                    result = false;
                }
            }
        }
        else
        {
            result = false;
        }
        if (! result)
        {
            pfsmReportUnexpected(xx, holder);
        }
    }
    return result;
} // pfsmGetNextNumber
/*------------------------------------ pfsmCollectATransition ---*/
static bool pfsmCollectATransition(PfsmData * xx)
{
    t_atom       holder;
    SymbolLink * inputSymbol = NULL_PTR;
    SymbolLink * outputSymbol = NULL_PTR;
    float        probability;
    bool         isRandom = false;
    bool         openLow;
    bool         openHigh;
    Criteria     whatKind = MatchUnknown;
    t_symbol *   whatSymbol = NULL_PTR;
    char         whatChar = '\0';
    long         whatValue = 0;
    long         lowValue;
    long         highValue;
    bool         result = pfsmGetNextAtom(xx, holder);

    if (result)
    {
        if (A_SYM == holder.a_type)
        {
            inputSymbol = pfsmLookupStateSymbol(xx, holder.a_w.w_sym);
            if (inputSymbol)
            {
                result = pfsmGetNextAtom(xx, holder);
            }
            else
            {
                LOG_ERROR_2(xx, OUTPUT_PREFIX "input state symbol '%s' not known", holder.a_w.w_sym->s_name)
                result = false;
            }
        }
        else
        {
            pfsmReportUnexpected(xx, holder);
            result = false;
        }
    }
    if (result)
    {
        /* Remember input data: */
        switch (holder.a_type)
        {
            case A_SYM:
                if (! strcmp(holder.a_w.w_sym->s_name, "$"))
                {
                    whatKind = MatchSpecial;
                    whatChar = '$';
                }
                else if (holder.a_w.w_sym == gAnyNumberSymbol)
                {
                    whatKind = MatchAnyNumber;
                }
                else if (holder.a_w.w_sym == gAnySymbolSymbol)
                {
                    whatKind = MatchAnySymbol;
                }
                else if (holder.a_w.w_sym == gRangeSymbol)
                {
                    if (pfsmGetNextNumber(xx, lowValue, openLow) &&
                        pfsmGetNextNumber(xx, highValue, openHigh))
                    {
                        whatKind = MatchNumberRange;
                    }
                    else
                    {
                        LOG_ERROR_1(xx, OUTPUT_PREFIX "invalid range specification seen")
                        result = false;
                    }
                }
                else
                {
                    /* Check for syntactic sugar: 'x is the same as the ASCII for the given character. */
                    const char * matcher = holder.a_w.w_sym->s_name;

                    whatKind = MatchSymbol;
                    whatSymbol = holder.a_w.w_sym;
                    if (kSugarCharacter == *matcher)
                    {
                        if (matcher[1] && (! matcher[2]))
                        {
                            whatKind = MatchNumber;
                            whatValue = matcher[1];
                        }
                    }
                }
                break;

            case A_LONG:
                whatKind = MatchNumber;
                whatValue = holder.a_w.w_long;
                break;

            case A_SEMI:
                whatKind = MatchSpecial;
                whatChar = ';';
                break;

            case A_COMMA:
                whatKind = MatchSpecial;
                whatChar = ',';
                break;

            case A_DOLLAR:
                whatKind = MatchSpecial;
                whatChar = '$';
                break;

            default:
                pfsmReportUnexpected(xx, holder);
                result = false;
                break;
        }
        if (result)
        {
            result = pfsmGetNextAtom(xx, holder);
        }
    }
    if (result)
    {
        if (A_SYM == holder.a_type)
        {
            if ((holder.a_w.w_sym != gTransitionSymbol) && (holder.a_w.w_sym != gRandomMoveSymbol))
            {
                pfsmReportUnexpected(xx, holder);
                result = false;
            }
            if (result && (holder.a_w.w_sym == gRandomMoveSymbol))
            {
                isRandom = true;
                result = pfsmGetNextAtom(xx, holder);
                if (A_FLOAT == holder.a_type)
                {
                    probability = holder.a_w.w_float;
                    if ((probability <= 0) || (probability >= 1))
                    {
                        pfsmReportUnexpected(xx, holder);
                        result = false;
                    }
                }
                else
                {
                    pfsmReportUnexpected(xx, holder);
                    result = false;
                }
            }
            if (result)
            {
                result = pfsmGetNextAtom(xx, holder);
            }
        }
        else
        {
            pfsmReportUnexpected(xx, holder);
            result = false;
        }
    }
    if (result)
    {
        if (A_SYM == holder.a_type)
        {
            outputSymbol = pfsmLookupStateSymbol(xx, holder.a_w.w_sym);
            if (outputSymbol)
            {
                result = pfsmGetNextAtom(xx, holder);
            }
            else
            {
                LOG_ERROR_2(xx, OUTPUT_PREFIX "output state symbol '%s' not known", holder.a_w.w_sym->s_name)
                result = false;
            }
        }
        else
        {
            pfsmReportUnexpected(xx, holder);
            result = false;
        }
    }
    if (result)
    {
        TransitionData * prevTrans = inputSymbol->fTransitions;
        TransitionData * newTrans = prevTrans;

        for ( ; newTrans; newTrans = newTrans->fNext)
        {
            if (newTrans)
            {
                prevTrans = newTrans;
            }
        }
        newTrans = GETBYTES(1, TransitionData);
        if (newTrans)
        {
            t_binbuf * collector = static_cast<t_binbuf *>(binbuf_new());

            newTrans->fNext = NULL_PTR;
            newTrans->fNextState = outputSymbol;
            newTrans->fIsRandom = isRandom;
            newTrans->fKind = whatKind;
            switch (whatKind)
            {
                case MatchNumber:
                    newTrans->fMatch.fAsNumber = whatValue;
                    break;

                case MatchNumberRange:
                    newTrans->fMatch.fAsRange.fLower = lowValue;
                    newTrans->fMatch.fAsRange.fUpper = highValue;
                    newTrans->fMatch.fAsRange.fLowerIsOpen = openLow;
                    newTrans->fMatch.fAsRange.fUpperIsOpen = openHigh;
                    break;

                case MatchSymbol:
                    newTrans->fMatch.fAsSymbol = whatSymbol;
                    break;

                case MatchSpecial:
                    newTrans->fMatch.fAsSpecial = whatChar;
                    break;

                default:
                    break;
            }
            newTrans->fProbability = probability;
            newTrans->fOutputCount = newTrans->fDollarCount = newTrans->fDoubleDollarCount = 0;
            newTrans->fDollarStarCount = 0;
            /* Collect the output values, terminated by a semicolon: */
            for ( ; holder.a_type != A_SEMI; )
            {
                /* We have a value; attach it. */
                binbuf_append(collector, NULL_PTR, 1, &holder);
                ++newTrans->fOutputCount;
                /* Note that the '$' symbols are handled in the transition engine. */
                result = pfsmGetNextAtom(xx, holder);
                if (! result)
                {
                    break;
                }
            }
            /* Assemble the pieces into a rule to be added to the transition tables. */
            if (newTrans->fOutputCount)
            {
                long     tyOffset = 0;
                long     stOffset = 0;
                t_atom * vector = GETBYTES(newTrans->fOutputCount, t_atom);

                newTrans->fOutput = vector;
                for (short ii = 0; ii < newTrans->fOutputCount; ++ii, ++vector)
                {
                    if (binbuf_getatom(collector, &tyOffset, &stOffset, vector))
                    {
                        break;
                    }

                    if (A_SYM == vector->a_type)
                    {
                        if (vector->a_w.w_sym == gDollarSymbol)
                        {
                            ++newTrans->fDollarCount;
                        }
                        else if (vector->a_w.w_sym == gDoubleDollarSymbol)
                        {
                            ++newTrans->fDoubleDollarCount;
                        }
                        else if (vector->a_w.w_sym == gDollarStarSymbol)
                        {
                            ++newTrans->fDollarStarCount;
                        }
                    }
                    else if (A_DOLLAR == vector->a_type)
                    {
                        ++newTrans->fDollarCount;
                    }
                }
            }
            else
            {
                newTrans->fOutput = NULL_PTR;
            }
            freeobject(reinterpret_cast<t_object *>(collector));
            /* Add the new transition to the input symbol. */
            if (prevTrans)
            {
                prevTrans->fNext = newTrans;
            }
            else
            {
                inputSymbol->fTransitions = newTrans;
            }
        }
    }
    return result;
} // pfsmCollectATransition
/*------------------------------------ pfsmLoadTables ---*/
bool pfsmLoadTables(PfsmData * xx,
                    t_symbol * fileName)
{
    bool         result = false;
    t_filehandle fileRef;

    if (path_opensysfile(fileName->s_name, path_getdefault(), &fileRef, PATH_READ_PERM))
    {
        LOG_ERROR_2(xx, OUTPUT_PREFIX "problem opening file '%s'", fileName->s_name)
    }
    else
    {
        t_handle fileContents = sysmem_newhandle(0);

        if (sysfile_readtextfile(fileRef, fileContents, 0, TEXT_LB_NATIVE))
        {
            LOG_ERROR_2(xx, OUTPUT_PREFIX "problem reading file '%s'", fileName->s_name)
        }
        else if (binbuf_text(xx->fBuffer, fileContents, sysmem_handlesize(fileContents)))
        {
            post("problem processing");
            LOG_ERROR_2(xx, OUTPUT_PREFIX "problem converting file '%s'", fileName->s_name)
        }
        else
        {
            pfsmInitializeHashTable(xx);
            xx->fBufferTypeOffset = 0;
            xx->fBufferStuffOffset = 0;
            if (pfsmGetStateSymbols(xx) && pfsmGetStartSymbol(xx) && pfsmGetStopSymbols(xx) && pfsmGetErrorSymbols(xx))
            {
                for ( ; pfsmCollectATransition(xx); )
                {
                    if (xx->fVerbose)
                    {
                        LOG_POST_1(xx, OUTPUT_PREFIX "read state transition")
                    }
                }
                result = true;
            }
        }
        sysfile_close(fileRef);
        sysmem_freehandle(fileContents);
    }
    return result;
} // pfsmLoadTables
