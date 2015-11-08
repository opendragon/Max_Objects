/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       tcpBuffers.h                                                            */
/*                                                                                      */
/*  Contains:   The TCP buffer management data structures.                              */
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

#if (! defined(TCPBUFFERS_H_))
# define TCPBUFFERS_H_ /* */

# include "getListKind.h"
# include <CoreServices/CoreServices.h>
# include <sys/socket.h>
# include <netinet/in.h>

struct DataBuffer
{
    unsigned char * fNextByteToUse;
    short           fNumBytesInUse;
    /* send/receive from this element: */
    short           fNumElements;
    short           fSanityCheck;
    unsigned char   fDataType;
    unsigned char   fElements[1]; /* dummy field to construct/deconstruct Atoms */
}; // DataBuffer

# define MAX_BUFFER_TO_SEND       24000
# define SIZEOF_DATABUFFER_HDR    (sizeof(short) + sizeof(short) + sizeof(unsigned char))
# define SIZEOF_DATABUFFER_PREFIX (sizeof(char *) + sizeof(short))
# define SIZEOF_OVERHEAD          (SIZEOF_DATABUFFER_HDR + SIZEOF_DATABUFFER_PREFIX)
# define MAX_BUFFER_TO_RECEIVE    (MAX_BUFFER_TO_SEND + SIZEOF_DATABUFFER_HDR)
# define BUFF_MEMORY_TO_ALLOC     (MAX_BUFFER_TO_SEND + SIZEOF_OVERHEAD)
# define MAX_ATOMS_EXPECTED       (MAX_BUFFER_TO_SEND / sizeof(long))

bool addAtomToBuffer(void *       xx,
                     const char * name,
                     DataBuffer * aBuffer,
                     t_atom *     aValue,
                     const bool   rawMode);

bool addFloatToBuffer(void *       xx,
                      const char * name,
                      DataBuffer * aBuffer,
                      const float  aValue,
                      const bool   rawMode);

bool addLongToBuffer(void *       xx,
                     const char * name,
                     DataBuffer * aBuffer,
                     const long   aValue,
                     const bool   rawMode);

void clearDataBuffer(DataBuffer * aBuffer);

t_atom * convertBufferToAtoms(void *               xx,
                              const char *         name,
                              t_handle             aBuffer,
                              short &              numAtoms,
                              const unsigned short indexToAdd,
                              const short          numBytes,
                              const bool           rawMode);

void copyDataBuffer(DataBuffer *       outBuffer,
                    const DataBuffer * inBuffer);

short validateBuffer(void *       xx,
                     const char * name,
                     DataBuffer * aBuffer,
                     const bool   rawMode);

mextern(t_symbol *) gDollarSymbol; /* Pointer to unique symbol for '$' */

#endif /* not TCPBUFFERS_H_ */
