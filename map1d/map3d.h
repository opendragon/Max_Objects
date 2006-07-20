/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       map3d.h                                                                 */
/*                                                                                      */
/*  Contains:   The data type declarations and forward references to routines.          */
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
/*  Created:    2001/04/07                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(MAP3D_H_))
 #define MAP3D_H_ /* */

/*#define USE_SYSLOG /* */

 #include "mapCommon.h"

 #define BE_VERBOSE    /* */
 #define OUR_NAME      "map3d"
 #define OUR_RES_NUMB  17164
 #define OUTPUT_PREFIX "map3d: "

struct RangeData
{
  RangeData *   fNext;
  PAtom         fOutput;
  short         fOutputCount;
  RangeElement  fBottom;
  RangeElement  fLeft;
  RangeElement  fForward;
  RangeElement  fRight;
  RangeElement  fTop;
  RangeElement  fBack;
  short         fDollarsPresent;
  short         fDoubleDollarsPresent;
  short         fSingleDollarsPresent;
  bool					fBottomTopDontCare;
  bool					fLeftRightDontCare;
  bool					fForwardBackDontCare;
}; /* RangeData */

typedef RangeData * RangeDataPtr;

struct Map3dData
{
  Object         fObject;
  POutlet        fResultOut;
  PBinbuf        fBuffer;
  RangeDataPtr   fFirstRange;
  RangeDataPtr   fLastRange;
  RangeDataPtr   fPreviousResult;
  long           fBufferTypeOffset;
  long           fBufferStuffOffset;
  FloatOrInteger fPreviousInput[3];
  short          fRangeCount;
  bool           fVerbose;
}; /* Map3dData */

typedef Map3dData * Map3dPtr;

MapRoutineDeclarations(Map3dPtr)

void map3dClearRangeList
  (Map3dPtr xx);

RangeDataPtr map3dConvertListToRange
  (const short	offset,
   const short	numAtoms,
   PAtom 				inList);

bool map3dLoadRangeList
  (Map3dPtr xx,
   PSymbol  filename);

void map3dOutputResult
  (Map3dPtr     xx,
   RangeDataPtr result);

void map3dProcessData
  (Map3dPtr       				xx,
   const FloatOrInteger &	xCoord,
   const FloatOrInteger &	yCoord,
   const FloatOrInteger &	zCoord);

StandardRoutineDeclarations(Map3dPtr)

mextern(PSymbol) gBackSymbol;    /* Pointer to unique Symbol for 'back' */
mextern(PSymbol) gBottomSymbol;  /* Pointer to unique Symbol for 'bottom' */
mextern(PSymbol) gForwardSymbol; /* Pointer to unique Symbol for 'forward' */
mextern(PSymbol) gLeftSymbol;    /* Pointer to unique Symbol for 'left' */
mextern(PSymbol) gRightSymbol;   /* Pointer to unique Symbol for 'right' */
mextern(PSymbol) gTopSymbol;     /* Pointer to unique Symbol for 'top' */

#endif /* not MAP3D_H_ */
