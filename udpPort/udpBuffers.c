/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       udpBuffers.c                                                            */
/*                                                                                      */
/*  Contains:   The TCP buffer management data structures and routines.                 */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2005 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2005/07/19                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "udpPort.h"

/*------------------------------------ addSymToBuffer ---*/
static bool addSymToBuffer
  (DataBufferPtr	aBuffer,
   const short    maxSize,
   PSymbol				aValue,
   const bool     rawMode)
{
  if (rawMode)
  {
    short actLength = short(strlen(aValue->s_name));
    
    if ((aBuffer->fNumBytesInUse + actLength) > maxSize)
    {
	    LOG_ERROR_1(OUTPUT_PREFIX "send buffer is full and cannot be added to")
	    return false;        

    }
    memcpy(aBuffer->fNextByteToUse, aValue->s_name, actLength);
    aBuffer->fNextByteToUse += actLength;
    aBuffer->fNumBytesInUse += actLength;
  }
  else
  {
	  uchar tag = A_SYM;     
	  short actLength = short(strlen(aValue->s_name) + 1); /* actual length, with null */
	  short dummy = htons(actLength);

	  /* Check if there's room for the string plus the type tag: */
	  if ((aBuffer->fNumBytesInUse + sizeof(tag) + sizeof(dummy) + actLength) >
	        maxSize)
	  {
	    LOG_ERROR_1(OUTPUT_PREFIX "send buffer is full and cannot be added to")
	    return false;        

	  }
	  if (aBuffer->fDataType != A_SYM)
	  {
	    /* A mixed vector, tag needed */
	    *aBuffer->fNextByteToUse = tag;
	    ++aBuffer->fNextByteToUse;
	    ++aBuffer->fNumBytesInUse;
	  }
	  memcpy(aBuffer->fNextByteToUse, &dummy, sizeof(dummy));
	  memcpy(aBuffer->fNextByteToUse + sizeof(dummy), aValue->s_name, actLength);
	  aBuffer->fNextByteToUse += sizeof(dummy) + actLength;
	  aBuffer->fNumBytesInUse += short(sizeof(dummy) + actLength);
	  ++aBuffer->fNumElements;
  }
  return true;
} /* addSymToBuffer */

/*------------------------------------ addSpecialToBuffer ---*/
static bool addSpecialToBuffer
  (DataBufferPtr	aBuffer,
   const short    maxSize,
   const short		aValue)
{
  uchar tag = static_cast<uchar>(aValue);     

  /* Check if there's room for the type tag: */
  if ((aBuffer->fNumBytesInUse + sizeof(tag)) > maxSize)
  {
    LOG_ERROR_1(OUTPUT_PREFIX "send buffer is full and cannot be added to")
    return false;        

  }
  if (aBuffer->fDataType != aValue)
  {
    /* A mixed vector, tag needed */
    *aBuffer->fNextByteToUse = tag;
    ++aBuffer->fNextByteToUse;
    ++aBuffer->fNumBytesInUse;
  }
  ++aBuffer->fNumElements;
  return true;
} /* addSpecialToBuffer */

/*------------------------------------ addAtomToBuffer ---*/
bool addAtomToBuffer
  (DataBufferPtr	aBuffer,
   const short    maxSize,
   PAtom					aValue,
   const bool     rawMode)
{
  switch (aValue->a_type)
  {
    case A_LONG:
      return addLongToBuffer(aBuffer, maxSize, aValue->a_w.w_long, rawMode);

    case A_FLOAT:
      return addFloatToBuffer(aBuffer, maxSize, aValue->a_w.w_float, rawMode);

    case A_SYM:
      if ((! rawMode) && (aValue->a_w.w_sym == gDollarSymbol))
        return addSpecialToBuffer(aBuffer, maxSize, A_DOLLAR);

      return addSymToBuffer(aBuffer, maxSize, aValue->a_w.w_sym, rawMode);

    case A_SEMI:
      return (rawMode ? addLongToBuffer(aBuffer, maxSize, ';', true) :
              addSpecialToBuffer(aBuffer, maxSize, A_SEMI));

    case A_COMMA:
      return (rawMode ? addLongToBuffer(aBuffer, maxSize, ',', true) :
              addSpecialToBuffer(aBuffer, maxSize, A_COMMA));

    case A_DOLLAR:
      return (rawMode ? addLongToBuffer(aBuffer, maxSize, '$', true) :
              addSpecialToBuffer(aBuffer, maxSize, A_DOLLAR));

  }
  LOG_ERROR_2(OUTPUT_PREFIX "unknown atom type (%hd) seen", aValue->a_type)
  return false;
} /* addAtomToBuffer */

/*------------------------------------ addFloatToBuffer ---*/
bool addFloatToBuffer
  (DataBufferPtr	aBuffer,
   const short    maxSize,
   const float		aValue,
   const bool     rawMode)
{
  if (rawMode)
  {
    if ((aBuffer->fNumBytesInUse + 1) > maxSize)
    {
	    LOG_ERROR_1(OUTPUT_PREFIX "send buffer is full and cannot be added to")
	    return false;        

    }
    *aBuffer->fNextByteToUse = static_cast<uchar>(short(aValue) & 0x00FF);
    ++aBuffer->fNextByteToUse;
    ++aBuffer->fNumBytesInUse;
  }
  else
  {
	  uchar tag = A_FLOAT;
	  float	dummyF = aValue;
	  long  dummy;

	  /* Check if there's room for the float plus the type tag: */
	  if ((aBuffer->fNumBytesInUse + sizeof(tag) + sizeof(dummy)) > maxSize)
	  {
	    LOG_ERROR_1(OUTPUT_PREFIX "send buffer is full and cannot be added to")
	    return false;        

	  }
	  if (aBuffer->fDataType != A_FLOAT)
	  {
	    /* A mixed vector, tag needed */
	    *aBuffer->fNextByteToUse = tag;
	    ++aBuffer->fNextByteToUse;
	    ++aBuffer->fNumBytesInUse;
	  }
	  dummy = htonl(*reinterpret_cast<Plong>(&dummyF));
	  memcpy(aBuffer->fNextByteToUse, &dummy, sizeof(dummy));
	  aBuffer->fNextByteToUse += sizeof(dummy);
	  aBuffer->fNumBytesInUse += sizeof(dummy);
	  ++aBuffer->fNumElements;
  }
  return true;
} /* addFloatToBuffer */

/*------------------------------------ addLongToBuffer ---*/
bool addLongToBuffer
  (DataBufferPtr	aBuffer,
   const short    maxSize,
   const long			aValue,
   const bool     rawMode)
{
  if (rawMode)
  {
    if ((aBuffer->fNumBytesInUse + 1) > maxSize)
    {
	    LOG_ERROR_1(OUTPUT_PREFIX "send buffer is full and cannot be added to")
	    return false;        

    }
    *aBuffer->fNextByteToUse = static_cast<uchar>(aValue & 0x00FF);
    ++aBuffer->fNextByteToUse;
    ++aBuffer->fNumBytesInUse;
  }
  else
  {
	  uchar tag = A_LONG;
	  long  dummy = htonl(aValue);

	  /* Check if there's room for the long plus the type tag: */
	  if ((aBuffer->fNumBytesInUse + sizeof(tag) + sizeof(dummy)) > maxSize)
	  {
	    LOG_ERROR_1(OUTPUT_PREFIX "send buffer is full and cannot be added to")
	    return false;        

	  }
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
  }
  return true;
} /* addLongToBuffer */

/*------------------------------------ clearDataBuffer ---*/
void clearDataBuffer
  (DataBufferPtr aBuffer)
{
  aBuffer->fNumElements = aBuffer->fNumBytesInUse = 0;
  aBuffer->fNextByteToUse = aBuffer->fElements;
  aBuffer->fDataType = A_NOTHING;
} /* clearDataBuffer */

/*------------------------------------ convertBufferToAtoms ---*/
PAtom convertBufferToAtoms
  (Handle				aBuffer,
   InetAddress	sender,
   short &			numAtoms,
   const short  numBytes,
   const bool   rawMode)
{
  short  outputSize;
  PAtom  result = NULL_PTR;
  Puchar walker = reinterpret_cast<Puchar>(*aBuffer);
  
  if (rawMode)
  {
    outputSize = short(numBytes + 2);
    result = GETBYTES(outputSize, Atom);
    if (result)
    {
      PAtom  thisAtom = result;
      char   hostAddr[255];

			// copy the sender IP address and port number to the first two Atoms:
			OTInetHostToString(sender.fHost, hostAddr);
			SETSYM(thisAtom, gensym(hostAddr));
			++thisAtom;
			SETLONG(thisAtom, sender.fPort);
			++thisAtom;
      for (short ii = 0; ii < numBytes; ++ii, ++thisAtom, ++walker)
      	SETLONG(thisAtom, long(*walker));
    }
  }
  else
  {
	  short  numElements, sanityCheck, calcBytes;
	  bool   okSoFar = true;

	  memcpy(&numElements, walker, sizeof(numElements)); 
	  numElements = ntohs(numElements);
	  memcpy(&sanityCheck, walker + sizeof(numElements), sizeof(sanityCheck));
	  sanityCheck = ntohs(sanityCheck);
	  calcBytes = short(- (numElements + sanityCheck));
	  walker += (sizeof(numElements) + sizeof(sanityCheck));
	  outputSize = short(numElements + 2);
    result = GETBYTES(outputSize, Atom);
    if (result)
    {
      PAtom  thisAtom = result;
      Puchar lastByteInBuffer = walker + calcBytes;
      short  chunkSize;
      uchar  baseElementType = *walker;//!!!
      uchar  elementType = baseElementType;
      long   dummy;
      char   hostAddr[255];

			// copy the sender IP address and port number to the first two Atoms:
			OTInetHostToString(sender.fHost, hostAddr);
			SETSYM(thisAtom, gensym(hostAddr));
			++thisAtom;
			SETLONG(thisAtom, sender.fPort);
			++thisAtom;
      ++walker;
      for (short element_index = 0; okSoFar && (element_index < numElements);
            ++element_index, ++thisAtom)
      {
        if (walker > lastByteInBuffer)
        {
          /* We should never get here, but just in case: */
          LOG_ERROR_1(OUTPUT_PREFIX "ran off end of receive buffer!")
          okSoFar = false;
        }
        if (baseElementType == A_GIMME)
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
              LOG_ERROR_1(OUTPUT_PREFIX "ran off end of receive buffer!")
              okSoFar = false;
            }
            else
            {
              memcpy(&dummy, walker, sizeof(dummy));
              SETFLOAT(thisAtom, ntohl(*reinterpret_cast<Pfloat>(&dummy)));
              walker += sizeof(float);
            }
            break;

          case A_LONG:
            if ((walker + sizeof(long)) > lastByteInBuffer)
            {
              LOG_ERROR_1(OUTPUT_PREFIX "ran off end of receive buffer!")
              okSoFar = false;
            }
            else
            {
              memcpy(&dummy, walker, sizeof(long));
              SETLONG(thisAtom, ntohl(dummy));
              walker += sizeof(long);
            }
            break;

          case A_SYM:
            if ((walker + sizeof(chunkSize)) > lastByteInBuffer)
            {
              LOG_ERROR_1(OUTPUT_PREFIX "ran off end of receive buffer!")
              okSoFar = false;
            }
            else
            {
              memcpy(&chunkSize, walker, sizeof(chunkSize));
              chunkSize = ntohs(chunkSize);
              walker += sizeof(chunkSize);
              if ((walker + chunkSize) > lastByteInBuffer)
              {
                LOG_ERROR_1(OUTPUT_PREFIX "ran off end of receive buffer!")
                okSoFar = false;
              }
              else
              {
                SETSYM(thisAtom, gensym(reinterpret_cast<Ptr>(walker)));
                walker += chunkSize;
              }
            }
            break;

          case A_SEMI:
          case A_COMMA:
          case A_DOLLAR:
            thisAtom->a_type = elementType;
            break;

          default:
            LOG_ERROR_2(OUTPUT_PREFIX "unexpected atom type seen - atom %hd", element_index)
            okSoFar = false;
            break;

        }
      }
      if (! okSoFar)
      {
        FREEBYTES(result, outputSize)
        outputSize = 0;
      }
    }
	  *aBuffer = reinterpret_cast<Pchar>(walker);
  }
  numAtoms = outputSize;
  return result;
} /* convertBufferToAtoms */

/*------------------------------------ copyDataBuffer ---*/
void copyDataBuffer
  (DataBufferPtr       outBuffer,
   const DataBufferPtr inBuffer)
{
  if (inBuffer->fNextByteToUse)
    outBuffer->fNextByteToUse = outBuffer->fElements +
                                        (inBuffer->fNextByteToUse - inBuffer->fElements);
  else
    outBuffer->fNextByteToUse = NULL_PTR;
  outBuffer->fNumBytesInUse = inBuffer->fNumBytesInUse;
  outBuffer->fNumElements = inBuffer->fNumElements;
  outBuffer->fSanityCheck = inBuffer->fSanityCheck;
  outBuffer->fDataType = inBuffer->fDataType;
  if (inBuffer->fNumBytesInUse > 0)
    memmove(outBuffer->fElements, inBuffer->fElements, inBuffer->fNumBytesInUse);
} /* copyDataBuffer */

/*------------------------------------ validateBuffer ---*/
short validateBuffer
  (DataBufferPtr	aBuffer,
   const bool     rawMode)
{
  if (rawMode)
  	return 1;

  short  countMessages = 0, totalBytes = aBuffer->fNumBytesInUse;
  Puchar walker = reinterpret_cast<Puchar>(&aBuffer->fNumElements);
  short  sanityCheck, numElements, calcBytes, countBytes = 0;

  if (totalBytes < SIZEOF_DATABUFFER_HDR)
  {
    LOG_ERROR_2(OUTPUT_PREFIX "received message is too short (%hd)", totalBytes)
    countMessages = -1;
  }
  else
  {
    for ( ; ; )
    {
      memcpy(&numElements, walker, sizeof(numElements)); 
      numElements = ntohs(numElements);
      memcpy(&sanityCheck, walker + sizeof(numElements), sizeof(sanityCheck));
      sanityCheck = ntohs(sanityCheck);
      calcBytes = short(- (numElements + sanityCheck));
      if ((numElements < 0) || (numElements > MAX_ATOMS_EXPECTED))
      {
        LOG_ERROR_2(OUTPUT_PREFIX "bad number of Atoms received (%hd)", numElements)
        countMessages = -1;
        break;

      }
      /* Check if we would go too far: */
      countBytes += calcBytes;
      if (countBytes > totalBytes)
      {
        LOG_ERROR_1(OUTPUT_PREFIX "received message is not recognizable")
        countMessages = -1;
        break;

      }            
      ++countMessages;
      if (countBytes == totalBytes)
        break;

      // reposition walker
      walker += calcBytes;
    }
  }
  return countMessages; /* for now */
} /* validateBuffer */
