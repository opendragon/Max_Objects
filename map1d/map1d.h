/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       map1d.h                                                                 */
/*                                                                                      */
/*  Contains:   The data type declarations and forward references to routines.          */
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
/*  Created:    2000/11/14                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(MAP1D_H_))
 #define MAP1D_H_ /* */

/*#define USE_SYSLOG /* */

 #include "mapCommon.h"

 #define BE_VERBOSE    /* */
 #define OUR_NAME      "map1d"
 #define OUR_RES_NUMB  17153
 #define OUTPUT_PREFIX "map1d: "

struct RangeData
{
  RangeData *   fNext;
  PAtom         fOutput;
  short         fOutputCount;
  RangeElement  fLower;
  RangeElement  fUpper;
  short         fDollarsPresent;
  short         fDoubleDollarsPresent;
  bool					fLowerUpperDontCare;
}; /* RangeData */

typedef RangeData * RangeDataPtr;

struct Map1dData
{
  Object         fObject;
  POutlet        fResultOut;
  PBinbuf        fBuffer;
  RangeDataPtr   fFirstRange;
  RangeDataPtr   fLastRange;
  RangeDataPtr   fPreviousResult;
  long           fBufferTypeOffset;
  long           fBufferStuffOffset;
  FloatOrInteger fPreviousInput;
  short          fRangeCount;
  bool           fVerbose;
}; /* Map1dData */

typedef Map1dData * Map1dPtr;

MapRoutineDeclarations(Map1dPtr)

void map1dClearRangeList
  (Map1dPtr xx);

RangeDataPtr map1dConvertListToRange
  (const short	offset,
   const short	numAtoms,
   PAtom				inList);

bool map1dLoadRangeList
  (Map1dPtr xx,
   PSymbol  filename);

void map1dOutputResult
  (Map1dPtr     xx,
   RangeDataPtr result);

void map1dProcessData
  (Map1dPtr       				xx,
   const FloatOrInteger & input);

StandardRoutineDeclarations(Map1dPtr)

mextern(PSymbol) gLowerSymbol; /* Pointer to unique Symbol for 'lower' */
mextern(PSymbol) gUpperSymbol; /* Pointer to unique Symbol for 'upper' */

#endif /* not MAP1D_H_ */
