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
# define MAP3D_H_ /* */

# include "mapCommon.h"

# define OUR_DESC    "Map coordinate triple to value"
# define OUR_NAME    "map3d"
# define OUR_VERSION "1.0.6"
# define OUR_YEAR    "2001"

# define BE_VERBOSE /* */

struct RangeData
{
    RangeData *  fNext;
    t_atom *     fOutput;
    long         fOutputCount;
    RangeElement fBottom;
    RangeElement fLeft;
    RangeElement fForward;
    RangeElement fRight;
    RangeElement fTop;
    RangeElement fBack;
    long         fDollarsPresent;
    long         fDoubleDollarsPresent;
    long         fSingleDollarsPresent;
    bool         fBottomTopDontCare;
    bool         fLeftRightDontCare;
    bool         fForwardBackDontCare;
}; // RangeData

struct Map3dData
{
    t_object       fObject;
    t_outlet *     fResultOut;
    t_binbuf *     fBuffer;
    RangeData *    fFirstRange;
    RangeData *    fLastRange;
    RangeData *    fPreviousResult;
    long           fBufferTypeOffset;
    long           fBufferStuffOffset;
    FloatOrInteger fPreviousInput[3];
    short          fRangeCount;
    bool           fVerbose;
}; // Map3dData

MapRoutineDeclarations(Map3dData);

void map3dClearRangeList(Map3dData * xx);

RangeData * map3dConvertListToRange(Map3dData * xx,
                                    const long  offset,
                                    const long  numAtoms,
                                    t_atom *    inList);

bool map3dLoadRangeList(Map3dData * xx,
                        t_symbol *  fileName);

void map3dOutputResult(Map3dData * xx,
                       RangeData * result);

void map3dProcessData(Map3dData *            xx,
                      const FloatOrInteger & xCoord,
                      const FloatOrInteger & yCoord,
                      const FloatOrInteger & zCoord);

StandardRoutineDeclarations(Map3dData);

mextern(t_symbol *) gBackSymbol;    /* Pointer to unique symbol for 'back' */
mextern(t_symbol *) gBottomSymbol;  /* Pointer to unique symbol for 'bottom' */
mextern(t_symbol *) gForwardSymbol; /* Pointer to unique symbol for 'forward' */
mextern(t_symbol *) gLeftSymbol;    /* Pointer to unique symbol for 'left' */
mextern(t_symbol *) gRightSymbol;   /* Pointer to unique symbol for 'right' */
mextern(t_symbol *) gTopSymbol;     /* Pointer to unique symbol for 'top' */

#endif /* not MAP3D_H_ */
