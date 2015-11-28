/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       tcpBuffers.cpp                                                          */
/*                                                                                      */
/*  Contains:   The TCP buffer management data structures and routines.                 */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 1998 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    1998/09/27                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "tcpCommon.h"

/*------------------------------------ addSymToBuffer ---*/
static bool addSymToBuffer(void *       xx,
                           const char * name,
                           DataBuffer * aBuffer,
                           t_symbol *   aValue,
                           const bool   rawMode)
{
    bool result;

    if (rawMode)
    {
        long actLength = strlen(aValue->s_name);

        if (MAX_BUFFER_TO_SEND < (aBuffer->fNumBytesInUse + actLength))
        {
            LOG_ERROR_2(xx, "%s: send buffer is full and cannot be added to", name)
            result = false;
        }
        else
        {
            memcpy(aBuffer->fNextByteToUse, aValue->s_name, actLength);
            aBuffer->fNextByteToUse += actLength;
            aBuffer->fNumBytesInUse += actLength;
            result = true;
        }
    }
    else
    {
        unsigned char tag = A_SYM;
        long          actLength = strlen(aValue->s_name) + 1; /* actual length, with null */
        short         dummy = htons(actLength);

        /* Check if there's room for the string plus the type tag: */
        if (MAX_BUFFER_TO_SEND < (aBuffer->fNumBytesInUse + sizeof(tag) + sizeof(dummy) +
                                  actLength))
        {
            LOG_ERROR_2(xx, "%s: send buffer is full and cannot be added to", name)
            result = false;
        }
        else
        {
            result = true;
            if (A_SYM != aBuffer->fDataType)
            {
                /* A mixed vector, tag needed */
                *aBuffer->fNextByteToUse = tag;
                ++aBuffer->fNextByteToUse;
                ++aBuffer->fNumBytesInUse;
            }
            memcpy(aBuffer->fNextByteToUse, &dummy, sizeof(dummy));
            memcpy(aBuffer->fNextByteToUse + sizeof(dummy), aValue->s_name, actLength);
            aBuffer->fNextByteToUse += sizeof(dummy) + actLength;
            aBuffer->fNumBytesInUse += sizeof(dummy) + actLength;
            ++aBuffer->fNumElements;
        }
    }
    return result;
} // addSymToBuffer

/*------------------------------------ addSpecialToBuffer ---*/
static bool addSpecialToBuffer(void *       xx,
                               const char * name,
                               DataBuffer * aBuffer,
                               const short  aValue)
{
    bool          result;
    unsigned char tag = static_cast<unsigned char>(aValue);

    /* Check if there's room for the type tag: */
    if (MAX_BUFFER_TO_SEND < (aBuffer->fNumBytesInUse + sizeof(tag)))
    {
        LOG_ERROR_2(xx, "%s: send buffer is full and cannot be added to", name)
        result = false;
    }
    else
    {
        if (aBuffer->fDataType != aValue)
        {
            /* A mixed vector, tag needed */
            *aBuffer->fNextByteToUse = tag;
            ++aBuffer->fNextByteToUse;
            ++aBuffer->fNumBytesInUse;
        }
        ++aBuffer->fNumElements;
        result = true;
    }
    return result;
} // addSpecialToBuffer

/*------------------------------------ addAtomToBuffer ---*/
bool addAtomToBuffer(void *       xx,
                     const char * name,
                     DataBuffer * aBuffer,
                     t_atom *     aValue,
                     const bool   rawMode)
{
    bool result;

    switch (aValue->a_type)
    {
        case A_LONG:
            result = addLongToBuffer(xx, name, aBuffer, aValue->a_w.w_long, rawMode);
            break;

        case A_FLOAT:
            result = addFloatToBuffer(xx, name, aBuffer, aValue->a_w.w_float, rawMode);
            break;

        case A_SYM:
            if ((! rawMode) && (aValue->a_w.w_sym == gDollarSymbol))
            {
                result = addSpecialToBuffer(xx, name, aBuffer, A_DOLLAR);
            }
            else
            {
                result = addSymToBuffer(xx, name, aBuffer, aValue->a_w.w_sym, rawMode);
            }
            break;

        case A_SEMI:
            result = (rawMode ? addLongToBuffer(xx, name, aBuffer, ';', true) :
                      addSpecialToBuffer(xx, name, aBuffer, A_SEMI));
            break;

        case A_COMMA:
            result = (rawMode ? addLongToBuffer(xx, name, aBuffer, ',', true) :
                      addSpecialToBuffer(xx, name, aBuffer, A_COMMA));
            break;

        case A_DOLLAR:
        case A_DOLLSYM:
            result = (rawMode ? addLongToBuffer(xx, name, aBuffer, '$', true) :
                      addSpecialToBuffer(xx, name, aBuffer, A_DOLLAR));
            break;

        default:
            LOG_ERROR_3(xx, "%s: unknown atom type (%d) seen", name,
                        static_cast<int>(aValue->a_type))
            result = false;
            break;

    }
    return result;
} // addAtomToBuffer

/*------------------------------------ addFloatToBuffer ---*/
bool addFloatToBuffer(void *       xx,
                      const char * name,
                      DataBuffer * aBuffer,
                      const float  aValue,
                      const bool   rawMode)
{
    bool result;

    if (rawMode)
    {
        if (MAX_BUFFER_TO_SEND < (aBuffer->fNumBytesInUse + 1))
        {
            LOG_ERROR_2(xx, "%s: send buffer is full and cannot be added to", name)
            result = false;
        }
        else
        {
            *aBuffer->fNextByteToUse = static_cast<unsigned char>(static_cast<short>(aValue) &
                                                                  0x00FF);
            ++aBuffer->fNextByteToUse;
            ++aBuffer->fNumBytesInUse;
            result = true;
        }
    }
    else
    {
        unsigned char tag = A_FLOAT;
        float         dummyF = aValue;
        long          dummy;

        /* Check if there's room for the float plus the type tag: */
        if (MAX_BUFFER_TO_SEND < (aBuffer->fNumBytesInUse + sizeof(tag) + sizeof(dummy)))
        {
            LOG_ERROR_2(xx, "%s: send buffer is full and cannot be added to", name)
            result = false;
        }
        else
        {
            if (aBuffer->fDataType != A_FLOAT)
            {
                /* A mixed vector, tag needed */
                *aBuffer->fNextByteToUse = tag;
                ++aBuffer->fNextByteToUse;
                ++aBuffer->fNumBytesInUse;
            }
            dummy = htonl(*reinterpret_cast<long *>(&dummyF));
            memcpy(aBuffer->fNextByteToUse, &dummy, sizeof(dummy));
            aBuffer->fNextByteToUse += sizeof(dummy);
            aBuffer->fNumBytesInUse += sizeof(dummy);
            ++aBuffer->fNumElements;
            result = true;
        }
    }
    return result;
} // addFloatToBuffer

/*------------------------------------ addLongToBuffer ---*/
bool addLongToBuffer(void *       xx,
                     const char * name,
                     DataBuffer * aBuffer,
                     const long   aValue,
                     const bool   rawMode)
{
    bool result;

    if (rawMode)
    {
        if (MAX_BUFFER_TO_SEND < (aBuffer->fNumBytesInUse + 1))
        {
            LOG_ERROR_2(xx, "%s: send buffer is full and cannot be added to", name)
            result = false;
        }
        else
        {
            *aBuffer->fNextByteToUse = static_cast<unsigned char>(aValue & 0x00FF);
            ++aBuffer->fNextByteToUse;
            ++aBuffer->fNumBytesInUse;
            result = true;
        }
    }
    else
    {
        unsigned char tag = A_LONG;
        long          dummy = htonl(aValue);

        /* Check if there's room for the long plus the type tag: */
        if (MAX_BUFFER_TO_SEND < (aBuffer->fNumBytesInUse + sizeof(tag) + sizeof(dummy)))
        {
            LOG_ERROR_2(xx, "%s: send buffer is full and cannot be added to", name)
            result = false;
        }
        else
        {
            if (aBuffer->fDataType != A_LONG)
            {
                /* A mixed vector, tag needed */
                *aBuffer->fNextByteToUse = tag;
                ++aBuffer->fNextByteToUse;
                ++aBuffer->fNumBytesInUse;
            }
            memcpy(aBuffer->fNextByteToUse, &dummy, sizeof(dummy));
            aBuffer->fNextByteToUse += sizeof(dummy);
            aBuffer->fNumBytesInUse += sizeof(dummy);
            ++aBuffer->fNumElements;
            result = true;
        }
    }
    return result;
} // addLongToBuffer

/*------------------------------------ clearDataBuffer ---*/
void clearDataBuffer(DataBuffer * aBuffer)
{
    aBuffer->fNumElements = aBuffer->fNumBytesInUse = 0;
    aBuffer->fNextByteToUse = aBuffer->fElements;
    aBuffer->fDataType = A_NOTHING;
} // clearDataBuffer

/*------------------------------------ convertBufferToAtoms ---*/
t_atom * convertBufferToAtoms(void *               xx,
                              const char *         name,
                              t_handle             aBuffer,
                              long &               numAtoms,
                              const unsigned short indexToAdd,
                              const long           numBytes,
                              const bool           rawMode)
{
    long            outputSize;
    t_atom *        result = NULL;
    unsigned char * walker = reinterpret_cast<unsigned char *>(*aBuffer);

    if (rawMode)
    {
        outputSize = numBytes + (indexToAdd ? 1 : 0);
        if (0 < outputSize)
        {
            result = GET_BYTES(outputSize, t_atom);
            if (result)
            {
                t_atom * thisAtom = result;

                if (indexToAdd)
                {
                    atom_setlong(thisAtom, TO_INT(indexToAdd));
                    ++thisAtom;
                }
                for (short ii = 0; ii < numBytes; ++ii, ++thisAtom, ++walker)
                {
                    atom_setlong(thisAtom, TO_INT(*walker));
                }
            }
        }
    }
    else
    {
        short numElements;
        short sanityCheck;
        long  calcBytes;
        bool  okSoFar = true;

        memcpy(&numElements, walker, sizeof(numElements));
        numElements = ntohs(numElements);
        memcpy(&sanityCheck, walker + sizeof(numElements), sizeof(sanityCheck));
        sanityCheck = ntohs(sanityCheck);
        calcBytes = -(numElements + sanityCheck);
        walker += (sizeof(numElements) + sizeof(sanityCheck));
        outputSize = numElements + (indexToAdd ? 1 : 0);
        if (0 < outputSize)
        {
            result = GET_BYTES(outputSize, t_atom);
            if (result)
            {
                t_atom *        thisAtom = result;
                unsigned char * lastByteInBuffer = walker + calcBytes;
                short           chunkSize;
                unsigned char   baseElementType = *walker; // !!!
                unsigned char   elementType = baseElementType;
                long            dummy;

                if (indexToAdd)
                {
                    atom_setlong(thisAtom, TO_INT(indexToAdd));
                    ++thisAtom;
                }
                ++walker;
                for (short element_index = 0; okSoFar && (element_index < numElements);
                     ++element_index, ++thisAtom)
                {
                    if (walker > lastByteInBuffer)
                    {
                        /* We should never get here, but just in case: */
                        LOG_ERROR_2(xx, "%s: ran off end of receive buffer!", name)
                        okSoFar = false;
                    }
                    if (A_GIMME == baseElementType)
                    {
                        /* A mixed vector: get the type */
                        elementType = *walker;
                        ++walker;
                    }
                    switch (elementType)
                    {
                        case A_FLOAT:
                            if ((walker + sizeof(float)) > lastByteInBuffer)
                            {
                                LOG_ERROR_2(xx, "%s: ran off end of receive buffer!", name)
                                okSoFar = false;
                            }
                            else
                            {
                                memcpy(&dummy, walker, sizeof(dummy));
                                atom_setfloat(thisAtom, ntohl(*reinterpret_cast<float *>(&dummy)));
                                walker += sizeof(float);
                            }
                            break;

                        case A_LONG:
                            if ((walker + sizeof(long)) > lastByteInBuffer)
                            {
                                LOG_ERROR_2(xx, "%s: ran off end of receive buffer!", name)
                                okSoFar = false;
                            }
                            else
                            {
                                memcpy(&dummy, walker, sizeof(long));
                                atom_setlong(thisAtom, ntohl(dummy));
                                walker += sizeof(long);
                            }
                            break;

                        case A_SYM:
                            if ((walker + sizeof(chunkSize)) > lastByteInBuffer)
                            {
                                LOG_ERROR_2(xx, "%s: ran off end of receive buffer!", name)
                                okSoFar = false;
                            }
                            else
                            {
                                memcpy(&chunkSize, walker, sizeof(chunkSize));
                                chunkSize = ntohs(chunkSize);
                                walker += sizeof(chunkSize);
                                if ((walker + chunkSize) > lastByteInBuffer)
                                {
                                    LOG_ERROR_2(xx, "%s: ran off end of receive buffer!", name)
                                    okSoFar = false;
                                }
                                else
                                {
                                    atom_setsym(thisAtom, gensym(reinterpret_cast<char *>(walker)));
                                    walker += chunkSize;
                                }
                            }
                            break;

                        case A_SEMI:
                        case A_COMMA:
                        case A_DOLLAR:
                        case A_DOLLSYM:
                            thisAtom->a_type = elementType;
                            break;

                        default:
                            LOG_ERROR_3(xx, "%s: unexpected atom type seen - atom %d", name,
                                        static_cast<int>(element_index))
                            okSoFar = false;
                            break;
                            
                    }
                }
                if (! okSoFar)
                {
                    FREE_BYTES(result);
                    outputSize = 0;
                }
            }
        }
        *aBuffer = reinterpret_cast<char *>(walker);
    }
    numAtoms = outputSize;
    return result;
} // convertBufferToAtoms

/*------------------------------------ copyDataBuffer ---*/
void copyDataBuffer(DataBuffer *       outBuffer,
                    const DataBuffer * inBuffer)
{
    if (inBuffer->fNextByteToUse)
    {
        outBuffer->fNextByteToUse = outBuffer->fElements +
                                    (inBuffer->fNextByteToUse - inBuffer->fElements);
    }
    else
    {
        outBuffer->fNextByteToUse = NULL;
    }
    outBuffer->fNumBytesInUse = inBuffer->fNumBytesInUse;
    outBuffer->fNumElements = inBuffer->fNumElements;
    outBuffer->fSanityCheck = inBuffer->fSanityCheck;
    outBuffer->fDataType = inBuffer->fDataType;
    if (0 < inBuffer->fNumBytesInUse)
    {
        memmove(outBuffer->fElements, inBuffer->fElements, inBuffer->fNumBytesInUse);
    }
} // copyDataBuffer

/*------------------------------------ validateBuffer ---*/
long validateBuffer(void *       xx,
                    const char * name,
                    DataBuffer * aBuffer,
                    const bool   rawMode)
{
    long countMessages;

    if (rawMode)
    {
        countMessages = 1;
    }
    else
    {
        long            totalBytes = aBuffer->fNumBytesInUse;
        unsigned char * walker = reinterpret_cast<unsigned char *>(&aBuffer->fNumElements);
        short           sanityCheck;
        short           numElements;
        long            calcBytes;
        long            countBytes = 0;

        if (SIZEOF_DATABUFFER_HDR > totalBytes)
        {
            LOG_ERROR_3(xx, "%s: received message is too short (%d)", name,
                        static_cast<int>(totalBytes))
            countMessages = -1;
        }
        else
        {
            countMessages = 0;
            for ( ; ; )
            {
                memcpy(&numElements, walker, sizeof(numElements));
                numElements = ntohs(numElements);
                memcpy(&sanityCheck, walker + sizeof(numElements), sizeof(sanityCheck));
                sanityCheck = ntohs(sanityCheck);
                calcBytes = -(numElements + sanityCheck);
                if ((0 > numElements) || (MAX_ATOMS_EXPECTED < numElements))
                {
                    LOG_ERROR_3(xx, "%s: bad number of Atoms received (%d)", name,
                                static_cast<int>(numElements))
                    countMessages = -1;
                    break;
                }
                /* Check if we would go too far: */
                countBytes += calcBytes;
                if (countBytes > totalBytes)
                {
                    LOG_ERROR_2(xx, "%s: received message is not recognizable", name)
                    countMessages = -1;
                    break;
                }
                ++countMessages;
                if (countBytes == totalBytes)
                {
                    break;
                }
                
                // reposition walker
                walker += calcBytes;
            }
        }
    }
    return countMessages; /* for now */
} // validateBuffer
