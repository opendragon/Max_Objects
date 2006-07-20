/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       VObjectCommon.h                                                         */
/*                                                                                      */
/*  Contains:   The data type declarations and forward references to routines.          */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2003 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2003/06/16                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(VOBJECTCOMMON_H_))
 #define VOBJECTCOMMON_H_ /* */
 
 #include "MissingAndExtra.h"
 #include "genericListOutput.h"

 #define MAX_SEP_CHARS	5
 #define CHUNK_SIZE     20

struct ChunkData
{
  ChunkData * fNext;
  long        fData[CHUNK_SIZE];
  short       fLastEntry;
}; /* ChunkData */

typedef ChunkData * ChunkPtr;

struct VObjectData
{
  Object    fObject;
  POutlet   fResultOut;
  POutlet	  fBangOut;
  short     fHowMany;
  long		  fSeparators[MAX_SEP_CHARS];
  short     fPreviousLength;
  PAtom     fPreviousList;
  ChunkPtr  fChunkList;
  ChunkPtr  fLastChunk;
  bool      fAssembled;
}; /* VObjectData */

typedef VObjectData * VObjectPtr;

bool checkInput
  (Qchar				name,
   const short	argc,
   PAtom  			argv);
   
void clearPrevious
  (VObjectPtr xx);

bool isSeparator
	(VObjectPtr	xx,
	 const long	value);

bool setupSeparators
	(VObjectPtr xx,
	 const long separator1,
   const long separator2,
   const long separator3,
   const long separator4,
   const long separator5);
   
StandardRoutineDeclarations(VObjectPtr)

#endif /* not VOBJECTCOMMON_H_ */
