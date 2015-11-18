/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       caseShift.h                                                             */
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
/*  Created:    2000/10/27                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(CASESHIFT_H_))
# define CASESHIFT_H_ /* */

# include "missingAndExtra.h"
# include "genericListOutput.h"

# define OUR_NAME      "caseShift"
// # define OUR_RES_NUMB  17146
# define OUTPUT_PREFIX "caseShift: "

struct CaseShiftData
{
    t_object   fObject;
    t_outlet * fResultOut;
    double     fPreviousFloat;
    long       fPreviousLong;
    short      fPreviousLength;
    t_atom *   fPreviousList;
    bool       fDown;
    short      fPreviousKind;
}; // CaseShiftData

void clearPrevious(CaseShiftData * xx);

void shiftAString(CaseShiftData * xx,
                  char *          newWord,
                  const char *    oldWord);

StandardRoutineDeclarations(CaseShiftData *);

mextern(t_symbol *) gDownSymbol;  /* Pointer to unique symbol for 'up' */
mextern(t_symbol *) gEmptySymbol; /* Pointer to unique symbol for '' */
mextern(t_symbol *) gUPSymbol;    /* Pointer to unique symbol for 'down' */

#endif /* not CASESHIFT_H_ */
