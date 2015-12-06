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
# define MAP1D_H_ /* */

# include "mapCommon.h"

# define OUR_DESC    "Map coordinate to value"
# define OUR_NAME    "map1d"
# define OUR_VERSION "1.0.6"
# define OUR_YEAR    "2000"

# define BE_VERBOSE /* */

struct RangeData
{
    RangeData *  fNext;
    t_atom *     fOutput;
    long         fOutputCount;
    RangeElement fLower;
    RangeElement fUpper;
    long         fDollarsPresent;
    long         fDoubleDollarsPresent;
    bool         fLowerUpperDontCare;
}; // RangeData

struct Map1dData
{
    t_object       fObject;
    t_outlet *     fResultOut;
    t_binbuf *     fBuffer;
    RangeData *    fFirstRange;
    RangeData *    fLastRange;
    RangeData *    fPreviousResult;
    long           fBufferTypeOffset;
    long           fBufferStuffOffset;
    FloatOrInteger fPreviousInput;
    long           fRangeCount;
    bool           fVerbose;
}; // Map1dData

MapRoutineDeclarations(Map1dData);

void map1dClearRangeList(Map1dData * xx);

RangeData * map1dConvertListToRange(Map1dData * xx,
                                    const long  offset,
                                    const long  numAtoms,
                                    t_atom *    inList);

bool map1dLoadRangeList(Map1dData * xx,
                        t_symbol *  fileName);

void map1dOutputResult(Map1dData * xx,
                       RangeData * result);

void map1dProcessData(Map1dData *            xx,
                      const FloatOrInteger & input);

StandardRoutineDeclarations(Map1dData);

mextern(t_symbol *) gLowerSymbol; /* Pointer to unique symbol for 'lower' */
mextern(t_symbol *) gUpperSymbol; /* Pointer to unique symbol for 'upper' */

#endif /* not MAP1D_H_ */
