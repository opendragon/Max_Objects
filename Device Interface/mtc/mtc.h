/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       mtc.h                                                                   */
/*                                                                                      */
/*  Contains:   The data type declarations and forward references to routines.          */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 1999 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    1999/12/05                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(MTC_H_))
# define MTC_H_ /* */

# include "missingAndExtra.h"

# define OUR_NAME      "mtc"
// # define OUR_RES_NUMB  17135
# define OUTPUT_PREFIX "mtc: "

# define BE_VERBOSE           /* */
# define REPORT_STATE_CHANGES /*!!*/
# define MAX_MESSAGE_BYTES    260

# define MAX_DESCRIPTOR_LENGTH 80

# define MAX_SPOTS       10
# define MAX_POLL_RATE   1000
# define SER_SAMPLE_RATE 100 /* default sampling rate for the serial port */
# define DEFAULT_SPOTS   1

# if (! defined(BE_VERBOSE))
#  undef REPORT_STATE_CHANGES
# endif /* not BE_VERBOSE */

typedef unsigned char MtcMessageBuffer[MAX_MESSAGE_BYTES + 3];

# define MTC_MAX_COOKED_PRESSURE 2000
# define MTC_MAX_THRESHOLD       5000
# define MTC_DEFAULT_THRESHOLD   2000
# define MTC_MAX_RAW_PRESSURE    255
# define MTC_MIN_RAW_PRESSURE    0

enum MtcCommandCode
{
    kMtcNoCommand                = 0x0000,
    kMtcCommandEnd               = 0x000D,
    kMtcCommandBeginSendData     = 0x0062,
    kMtcCommandSetDiffThreshold  = 0x0068,
    kMtcCommandSetOffsetData     = 0x006F,
    kMtcCommandPingUnit          = 0x0070,
    kMtcCommandStopSendData      = 0x0073,
    kMtcCommandSetSampleInterval = 0x0074,
    kMtcCommandSendUnitDesc      = 0x0075
}; // MtcCommandCode

enum MtcState
{
    kMtcStateAwaitingEndOfCommand,
    kMtcStateAwaitingEndOfComp,
    kMtcStateAwaitingEndOfData,
    kMtcStateAwaitingEndOfDesc,
    kMtcStateAwaitingNumBytes,
    kMtcStateAwaitingNumPackets,
    kMtcStateAwaitingSampleNumber,
    kMtcStateIdle,
    kMtcStateLookingForEnd,
    kMtcStateSkipToAck,
    kMtcStateSkipToEnd,
    kMtcStateUnknown
}; // MtcState

enum MtcResponseCode
{
    kMtcResponseEndCommand = 0x000D,
    kMtcResponseAck        = 0x0061,
    kMtcResponseSendComp   = 0x0063,
    kMtcResponseSendData   = 0x0064,
    kMtcResponseUnitDesc   = 0x0075
}; // MtcResponseCode

struct MtcTaxelDesc
{
    float          fXCoord;
    float          fYCoord;
    double         fScale;
    double         fPressure;
    MtcTaxelDesc * fBottomTaxel;
    MtcTaxelDesc * fLeftTaxel;
    MtcTaxelDesc * fRightTaxel;
    MtcTaxelDesc * fTopTaxel;
 #if defined(MTC_USE_CORNERS)
    MtcTaxelDesc * fBottomLeftTaxel;
    MtcTaxelDesc * fBottomRightTaxel;
    MtcTaxelDesc * fTopLeftTaxel;
    MtcTaxelDesc * fTopRightTaxel;
 #endif /* MTC_USE_CORNERS */
    short          fTaxelNumber;
    short          fXIndex;
    short          fYIndex;
    short          fCookedData;
    short          fRawMax;
    short          fRawMin;
    short          fRawData;
}; // MtcTaxelDesc

struct MtcSpot
{
    t_atom fXCoord;
    t_atom fYCoord;
    t_atom fPressure;
}; // MtcSpot

#define AtomsPerSpot (sizeof(MtcSpot) / sizeof(t_atom))

enum MtcOrder
{
    kMtcOrderByPressure,
    kMtcOrderByX,
    kMtcOrderByY,
    kMtcOrderUnordered
}; // MtcOrder

struct MtcData
{
    t_object        fObject;
    t_clock *       fPollClock;
    void *          fProxy;
    t_outlet *      fChunkSendOut;
    t_outlet *      fCommandComplete;
    t_outlet *      fDataSendOut;
    t_outlet *      fErrorBangOut;
    t_outlet *      fSampleBangOut;
    t_outlet *      fDataStartStopOut;
    t_outlet *      fDataOut;
    t_qelem *       fPollQueue;
    MtcTaxelDesc ** fTaxelMapHandle;
    MtcTaxelDesc ** fTaxelMatrix;
    MtcTaxelDesc *  fSentinelTaxel;
    MtcSpot *       fSpots;
    t_atom *        fRawRow;
    char *          fMapFileName;
    char *          fNormalFileName;
    unsigned char * fDataBuffer;
    short *         fDataRecovery;
    long            fInletNumber;
    long            fLastSampleNumber;
    long            fSampleNumber;
    long            fSampleNumberBase;
    float           fXMaxActual;
    float           fXMinActual;
    float           fYMaxActual;
    float           fYMinActual;
    float           fXMax;
    float           fYMax;
    float           fRateMax;
    float           fRateCurrent;
    MtcState        fState;
    MtcResponseCode fResponse;
    MtcOrder        fSortOrder;
    char            fDescriptor[MAX_DESCRIPTOR_LENGTH + 1];
    short           fDescriptorLength;
    short           fPollRate;
    short           fPoolAvailable;
    short           fPoolSize;
    short           fNextTaxel;
    short           fNextByte;
    short           fLastByte;
    short           fNumBytes;
    short           fNumPackets;
    short           fNumTaxels;
    short           fMaxCol;
    short           fMaxRow;
    short           fThreshold;
    short           fNumSpots;
    short           fExpectedPackets;
    short           fMapFilePath;
    short           fNormalFilePath;
    bool            fChunkPulseSent;
    bool            fNormalizing;
    bool            fModeRaw;
    bool            fDataCompressed;
    bool            fIsPacketHeader;
    bool            fStarted;
    bool            fStopped;
    bool            fStopping;
    bool            fUseCompression;
 #if defined(BE_VERBOSE)
    bool            fVerbose;
 #endif /* BE_VERBOSE */
}; /* MtcData */

void cmd_Describe(MtcData * xx);

void cmd_Mode(MtcData *  xx,
              t_symbol * rawOrCooked);

void cmd_Order(MtcData *  xx,
               t_symbol * newOrder);

void cmd_Ping(MtcData * xx);

void cmd_Start(MtcData *  xx,
               t_symbol * kind);

void cmd_Stop(MtcData * xx);

void cmd_Threshold(MtcData * xx,
                   long      number);

void cmd_Train(MtcData *  xx,
               t_symbol * startStop);

VERBOSE_HEADER(MtcData);

void mtcPerformWriteCommand(MtcData *            xx,
                            const MtcCommandCode commandCode,
                            const unsigned char  numBytesToFollow,
                            unsigned char *      bytesToFollow);

void mtcDoStart(MtcData * xx);

void mtcDoStop(MtcData * xx);

void mtcMoveRawDataFromBuffer(MtcData * xx);

void mtcProcessResponse(MtcData * xx,
                        long      rr);

void mtcProcessTaxels(MtcData * xx);

bool mtcReadMapFile(MtcData * xx);

bool mtcReadNormalizationFile(MtcData * xx);

void mtcResetNormalization(MtcData * xx);

bool mtcSetKind(MtcData *  xx,
                t_symbol * kind);

bool mtcSetMode(MtcData *  xx,
                t_symbol * rawOrCooked);

bool mtcSetOrder(MtcData *  xx,
                 t_symbol * order);

void mtcSetupIndices(MtcData * xx);

void mtcSortTaxels(MtcData *   xx,
                   const short numSpots);

bool mtcWriteNormalizationFile(MtcData * xx);

StandardRoutineDeclarations(MtcData);

mextern(t_symbol *) gCompressedSymbol; /* Pointer to unique symbol for 'compressed' */
mextern(t_symbol *) gCookedSymbol;     /* Pointer to unique symbol for 'cooked' */
mextern(t_symbol *) gEmptySymbol;      /* Pointer to unique symbol for '' */
mextern(t_symbol *) gNormalSymbol;     /* Pointer to unique symbol for 'normal' */
mextern(t_symbol *) gOffSymbol;        /* Pointer to unique symbol for 'off' */
mextern(t_symbol *) gOnSymbol;         /* Pointer to unique symbol for 'on' */
mextern(t_symbol *) gPSymbol;          /* Pointer to unique symbol for 'p' */
mextern(t_symbol *) gRawSymbol;        /* Pointer to unique symbol for 'raw' */
mextern(t_symbol *) gStartSymbol;      /* Pointer to unique symbol for 'start' */
mextern(t_symbol *) gStopSymbol;       /* Pointer to unique symbol for 'stop' */
mextern(t_symbol *) gXSymbol;          /* Pointer to unique symbol for 'x' */
mextern(t_symbol *) gYSymbol;          /* Pointer to unique symbol for 'y' */

#endif /* not MTC_H_ */
