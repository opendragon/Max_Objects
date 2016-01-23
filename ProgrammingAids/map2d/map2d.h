/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       map2d.h                                                                 */
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
/*  Created:    2000/11/15                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(MAP2D_H_))
# define MAP2D_H_ /* */

# include "mapCommon.h"

# define OUR_DESC    "Map coordinate pair to value"
# define OUR_NAME    "map2d"
# define OUR_VERSION "1.0.6"
# define OUR_YEAR    "2000"

# define BE_VERBOSE /* */

struct RangeData
{
    RangeData *  fNext;
    t_atom *     fOutput;
    long         fOutputCount;
    RangeElement fBottom;
    RangeElement fLeft;
    RangeElement fRight;
    RangeElement fTop;
    long         fDollarsPresent;
    long         fDoubleDollarsPresent;
    long         fSingleDollarsPresent;
    bool         fBottomTopDontCare;
    bool         fLeftRightDontCare;
}; // RangeData


struct Map2dData
{
    t_object       fObject;
    t_outlet *     fResultOut;
    t_binbuf *     fBuffer;
    RangeData *    fFirstRange;
    RangeData *    fLastRange;
    RangeData *    fPreviousResult;
    long           fBufferTypeOffset;
    long           fBufferStuffOffset;
    FloatOrInteger fPreviousInput[2];
    long           fRangeCount;
    bool           fVerbose;
}; // Map2dData

MapRoutineDeclarations(Map2dData);

void
map2dClearRangeList(Map2dData * xx);

RangeData *
map2dConvertListToRange(Map2dData * xx,
                        const long  offset,
                        const long  numAtoms,
                        t_atom *    inList);

bool
map2dLoadRangeList(Map2dData * xx,
                   t_symbol *  fileName);

void
map2dOutputResult(Map2dData * xx,
                  RangeData * result);

void
map2dProcessData(Map2dData *            xx,
                 const FloatOrInteger & xCoord,
                 const FloatOrInteger & yCoord);

StandardRoutineDeclarations(Map2dData);

mextern(t_symbol *) gBottomSymbol; /* Pointer to unique symbol for 'bottom' */
mextern(t_symbol *) gLeftSymbol;   /* Pointer to unique symbol for 'left' */
mextern(t_symbol *) gRightSymbol;  /* Pointer to unique symbol for 'right' */
mextern(t_symbol *) gTopSymbol;    /* Pointer to unique symbol for 'top' */

#endif /* not MAP2D_H_ */
