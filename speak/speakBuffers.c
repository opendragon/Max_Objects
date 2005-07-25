/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       speakBuffers.c                                                          */
/*                                                                                      */
/*  Contains:   The speech buffer management data structures and routines.              */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2001 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2001/04/03                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "speakBuffers.h"

/*------------------------------------ addAtomToBuffer ---*/
bool addAtomToBuffer
  (Qchar					name,
   DataBufferPtr	aBuffer,
   PAtom					aValue)
{
  switch (aValue->a_type)
  {
    case A_LONG:
      return addLongToBuffer(name, aBuffer, aValue->a_w.w_long);

    case A_FLOAT:
      return addFloatToBuffer(name, aBuffer, aValue->a_w.w_float);

    case A_SYM:
      if (aValue->a_w.w_sym == gDollarSymbol)
        return addSymToBuffer(name, aBuffer, gDollarNameSymbol);

      return addSymToBuffer(name, aBuffer, aValue->a_w.w_sym);

    case A_SEMI:
      return addSymToBuffer(name, aBuffer, gSemicolonNameSymbol);

    case A_COMMA:
      return addSymToBuffer(name, aBuffer, gCommaNameSymbol);

    case A_DOLLAR:
      return addSymToBuffer(name, aBuffer, gDollarNameSymbol);

  }
  LOG_ERROR_3("%s: unknown atom type (%hd) seen", name, aValue->a_type)
  return false;
} /* addAtomToBuffer */

/*------------------------------------ addFloatToBuffer ---*/
bool addFloatToBuffer
  (Qchar					name,
   DataBufferPtr	aBuffer,
   const float		aValue)
{
  static char numBuff[30]; /* should be big enough! */
  short       actLength;

  sprintf(numBuff, "%g ", double(aValue));
  /* Check if there's room for the float plus a blank: */
  actLength = short(strlen(numBuff));
  if ((aBuffer->fNumBytesInUse + actLength) > MAX_BUFFER_TO_SEND)
  {
    LOG_ERROR_2("%s: send buffer is full and cannot be added to", name)
    return false;        

  }
  memcpy(aBuffer->fNextByteToUse, numBuff, actLength);
  aBuffer->fNextByteToUse += actLength;
  aBuffer->fNumBytesInUse += actLength;
  return true;
} /* addFloatToBuffer */

/*------------------------------------ addLongToBuffer ---*/
bool addLongToBuffer
  (Qchar					name,
   DataBufferPtr	aBuffer,
   const long			aValue)
{
  static char numBuff[30]; /* should be big enough! */
  short       actLength;

  sprintf(numBuff, "%ld ", aValue);
  /* Check if there's room for the long plus a blank: */
  actLength = short(strlen(numBuff));
  if ((aBuffer->fNumBytesInUse + actLength) > MAX_BUFFER_TO_SEND)
  {
    LOG_ERROR_2("%s: send buffer is full and cannot be added to", name)
    return false;        

  }
  memcpy(aBuffer->fNextByteToUse, numBuff, actLength);
  aBuffer->fNextByteToUse += actLength;
  aBuffer->fNumBytesInUse += actLength;
  return true;
} /* addLongToBuffer */

/*------------------------------------ addSymToBuffer ---*/
bool addSymToBuffer
  (Qchar					name,
   DataBufferPtr	aBuffer,
   PSymbol				aValue)
{
  short actLength = short(strlen(aValue->s_name));

  /* Check if there's room for the string: */
  if ((aBuffer->fNumBytesInUse + actLength + 1) > MAX_BUFFER_TO_SEND)
  {
    LOG_ERROR_2("%s: send buffer is full and cannot be added to", name)
    return false;        

  }
  memcpy(aBuffer->fNextByteToUse, aValue->s_name, actLength);
  memcpy(aBuffer->fNextByteToUse + actLength, " ", 1);
  aBuffer->fNextByteToUse += actLength + 1;
  aBuffer->fNumBytesInUse += short(actLength + 1);
  return true;
} /* addSymToBuffer */

/*------------------------------------ clearDataBuffer ---*/
void clearDataBuffer
  (DataBufferPtr aBuffer)
{
  aBuffer->fNumBytesInUse = 0;
  aBuffer->fNextByteToUse = aBuffer->fElements;
} /* clearDataBuffer */
