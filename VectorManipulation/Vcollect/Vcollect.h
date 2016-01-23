/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       Vcollect.h                                                              */
/*                                                                                      */
/*  Contains:   The data type declarations and forward references to routines.          */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2002 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2002/08/27                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(VCOLLECT_H_))
# define VCOLLECT_H_ /* */

# include "missingAndExtra.h"
# include "genericListOutput.h"

# define OUR_DESC    "Collect atoms into a list"
# define OUR_NAME    "Vcollect"
# define OUR_VERSION "1.0.1"
# define OUR_YEAR    "2002"

# define CHUNK_SIZE 30

struct CollectChunk
{
    CollectChunk * fNext;
    t_atom         fAtoms[CHUNK_SIZE];
}; // CollectChunk

struct VcollectData
{
    t_object       fObject;
    t_outlet *     fLengthOut;
    t_outlet *     fResultOut;
    void *         fProxy;
    CollectChunk * fFirstChunk;
    CollectChunk * fFreeChunks;
    CollectChunk * fLastChunk;
    long           fInletNumber;
    short          fAtomsInLastChunk;
    short          fTotalAtoms;
    bool           fCollecting;
}; // VcollectData

# define START_HEADER(type) \
    void\
    cmd_Start(type * xx)

CLEAR_HEADER(VcollectData);
COUNT_HEADER(VcollectData);
START_HEADER(VcollectData);
STOP_HEADER(VcollectData);

void
collectAddAtoms(VcollectData * xx,
                t_atom *       atoms,
                const long     count);

StandardRoutineDeclarations(VcollectData);

mextern(t_symbol *)  gClearSymbol; /* Pointer to unique symbol for 'clear' */
mextern(t_symbol *)  gCountSymbol; /* Pointer to unique symbol for 'count' */
mextern(t_symbol *)  gStartSymbol; /* Pointer to unique symbol for 'start' */
mextern(t_symbol *)  gStopSymbol;  /* Pointer to unique symbol for 'stop' */

#endif /* not VCOLLECT_H_ */
