/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       mtcTrack.h                                                              */
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
/*  Created:    2001/04/18                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(MTCTRACK_H_))
# define MTCTRACK_H_  /* */

# include "missingAndExtra.h"
# include "genericListOutput.h"

# define OUR_DESC    "Generates point tracks from mtc data"
# define OUR_NAME    "mtcTrack"
# define OUR_VERSION "1.0.2"
# define OUR_YEAR    "2001"

# define MAX_POINTS 10

struct MtcSampleData
{
    double   fThisP;
    double   fThisX;
    double   fThisY;
    double * fActDistance;
    bool     fAvailable;
}; // MtcSampleData

struct MtcRetainedData
{
    double fLastP;
    double fLastX;
    double fLastY;
    double fDeltaX;
    double fDeltaY;
    double fNewP;
    double fNewX;
    double fNewY;
    double fVelocity;
    double fForce;
    bool   fFresh;
    bool   fValid;
}; // MtcRetainedData

struct MtcTrackData
{
    t_object          fObject;
    t_outlet *        fResultOut;
    t_outlet *        fPointCountOut;
    t_outlet *        fErrorBangOut;
    t_outlet *        fLinesCompleteOut;
    MtcRetainedData * fRetainedData;
    MtcSampleData *   fSamples;
    double *          fActDistances;
    double            fThreshold;
    t_atom_long       fBatchNumber;
    long              fHowMany;
    long              fMaxSamples;
    long              fRetainedCount;
    long              fSampleCount;
    bool              fAddBatchNumber;
    bool              fAddIndex;
}; // MtcTrackData

# define BATCH_HEADER(type) \
    void cmd_Batch(type *     xx,\
                   t_symbol * onOff)

# define INDEX_HEADER(type) \
    void cmd_Index(type *     xx,\
                   t_symbol * onOff)

# define THRESHOLD_HEADER(type) \
    void cmd_Threshold(type *       xx,\
                       const double num)

BATCH_HEADER(MtcTrackData);
CLEAR_HEADER(MtcTrackData);
INDEX_HEADER(MtcTrackData);
THRESHOLD_HEADER(MtcTrackData);

StandardRoutineDeclarations(MtcTrackData);

mextern(t_symbol *) gBatchSymbol; /* Pointer to unique symbol for 'batch' */
mextern(t_symbol *) gEmptySymbol; /* Pointer to unique symbol for '' */
mextern(t_symbol *) gIndexSymbol; /* Pointer to unique symbol for 'index' */
mextern(t_symbol *) gOffSymbol;   /* Pointer to unique symbol for 'off' */
mextern(t_symbol *) gOnSymbol;    /* Pointer to unique symbol for 'on' */

#endif /* not MTCTRACK_H_ */
