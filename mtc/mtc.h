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
 #define MTC_H_ /* */
 
 /*#define USE_SYSLOG /* */

 #include "MissingAndExtra.h"

 #define OUR_NAME      "mtc"
 #define OUR_RES_NUMB  17135
 #define OUTPUT_PREFIX "mtc: "

 #define BE_VERBOSE           /* */
 #define REPORT_STATE_CHANGES /*!!*/
 #define MAX_MESSAGE_BYTES    260

 #define MAX_DESCRIPTOR_LENGTH 80

 #define MAX_SPOTS       10
 #define MAX_POLL_RATE   1000
 #define SER_SAMPLE_RATE 100 /* default sampling rate for the serial port */
 #define DEFAULT_SPOTS   1

 #if (! defined(BE_VERBOSE))
  #undef REPORT_STATE_CHANGES
 #endif /* not BE_VERBOSE */

typedef uchar MtcMessageBuffer[MAX_MESSAGE_BYTES + 3];

 #define MTC_MAX_COOKED_PRESSURE 2000
 #define MTC_MAX_THRESHOLD       5000
 #define MTC_DEFAULT_THRESHOLD   2000
 #define MTC_MAX_RAW_PRESSURE    255
 #define MTC_MIN_RAW_PRESSURE    0

enum MtcCommandCode
{
  MTC_no_command              = 0x0000,
  MTC_CMD_END_COMMAND         = 0x000D,
  MTC_CMD_BEGIN_SEND_DATA     = 0x0062,
  MTC_CMD_SET_DIFF_THRESH     = 0x0068,
  MTC_CMD_SET_OFFSET_DATA     = 0x006F,
  MTC_CMD_PING_UNIT           = 0x0070,
  MTC_CMD_STOP_SEND_DATA      = 0x0073,
  MTC_CMD_SET_SAMPLE_INTERVAL = 0x0074,
  MTC_CMD_SEND_UNIT_DESC      = 0x0075
}; /* MtcCommandCode */

enum MtcState
{
  MTC_AWAITING_END_OF_COMMAND,
  MTC_AWAITING_END_OF_COMP,
  MTC_AWAITING_END_OF_DATA,
  MTC_AWAITING_END_OF_DESC,
  MTC_AWAITING_NUM_BYTES,
  MTC_AWAITING_NUM_PACKETS,
  MTC_AWAITING_SAMPLE_NUMBER,
  MTC_IDLE,
  MTC_LOOKING_FOR_END,
  MTC_SKIP_TO_ACK,
  MTC_SKIP_TO_END,
  MTC_state_unknown
}; /* MtcState */

enum MtcResponseCode
{
  MTC_RESPONSE_END_CMD   = 0x000D,
  MTC_RESPONSE_ACK       = 0x0061,
  MTC_RESPONSE_SEND_COMP = 0x0063,
  MTC_RESPONSE_SEND_DATA = 0x0064,
  MTC_RESPONSE_UNIT_DESC = 0x0075
}; /* MtcResponseCode */

struct MtcTaxelDesc
{
  float           fXCoord;
  float           fYCoord;
  double          fScale;
  double          fPressure;
  MtcTaxelDesc *  fBottomTaxel;
  MtcTaxelDesc *  fLeftTaxel;
  MtcTaxelDesc *  fRightTaxel;
  MtcTaxelDesc *  fTopTaxel;
 #if defined(MTC_USE_CORNERS)
  MtcTaxelDesc *  fBottomLeftTaxel;
  MtcTaxelDesc *  fBottomRightTaxel;
  MtcTaxelDesc *  fTopLeftTaxel;
  MtcTaxelDesc *  fTopRightTaxel;
 #endif /* MTC_USE_CORNERS */
  short           fTaxelNumber;
  short           fXIndex;
  short           fYIndex;
  short           fCookedData;
  short           fRawMax;
  short           fRawMin;
  short           fRawData;
}; /* MtcTaxelDesc */

typedef MtcTaxelDesc * MtcTaxelPtr;

typedef MtcTaxelPtr * MtcTaxelHdl;

struct MtcSpot
{
  Atom fXCoord;
  Atom fYCoord;
  Atom fPressure;
}; /* MtcSpot */

typedef MtcSpot * MtcSpotPtr;

#define AtomsPerSpot (sizeof(MtcSpot) / sizeof(Atom))

enum MtcOrder
{
  MTC_BY_PRESSURE,
  MTC_BY_X,
  MTC_BY_Y,
  MTC_UNORDERED
}; /* MtcOrder */

struct MtcControl
{
  Object          fObject;
  PClock          fPollClock;
  Pvoid           fProxy;
  POutlet         fChunkSendOut;
  POutlet         fCommandComplete;
  POutlet         fDataSendOut;
  POutlet         fErrorBangOut;
  POutlet         fSampleBangOut;
  POutlet         fDataStartStopOut;
  POutlet         fDataOut;
  PQelem          fPollQueue;
  MtcTaxelHdl     fTaxelMapHandle;
  MtcTaxelHdl     fTaxelMatrix;
  MtcTaxelPtr     fSentinelTaxel;
  MtcSpotPtr      fSpots;
  PAtom           fRawRow;
  Pchar           fMapFileName;
  Pchar           fNormalFileName;
  Puchar          fDataBuffer;
  Pshort          fDataRecovery;
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
 #if defined(COMPILE_FOR_CATS)
  short           fMapFilePath;
  short						fNormalFilePath;
 #else /* not COMPILE_FOR_CATS */
  short           fMapFileVolume;
 #endif /* not COMPILE_FOR_CATS */
  bool            fChunkPulseSent;
  bool            fNormalizing;
  bool            fModeRaw;
  bool            fDataCompressed;
  bool            fIsPacketHeader;
  bool            fStarted;
  bool            fStopped;
  bool						fStopping;
  bool            fUseCompression;
 #if defined(BE_VERBOSE)
  bool            fVerbose;
 #endif /* BE_VERBOSE */
}; /* MtcControl */

typedef MtcControl * MtcControlPtr;

Pvoid cmd_Describe
  (MtcControlPtr xx);

Pvoid cmd_Mode
  (MtcControlPtr xx,
   PSymbol       rawOrCooked);

Pvoid cmd_Order
  (MtcControlPtr xx,
   PSymbol       newOrder);

Pvoid cmd_Ping
  (MtcControlPtr xx);

Pvoid cmd_Start
  (MtcControlPtr xx,
   PSymbol       kind);

Pvoid cmd_Stop
  (MtcControlPtr xx);

Pvoid cmd_Threshold
  (MtcControlPtr xx,
   long          number);

Pvoid cmd_Train
  (MtcControlPtr xx,
   PSymbol       startStop);

Pvoid cmd_Verbose
  (MtcControlPtr xx,
   PSymbol       onOff);

void mtcPerformWriteCommand
  (MtcControlPtr  			xx,
   const MtcCommandCode	commandCode,
   const uchar          numBytesToFollow,
   Puchar         			bytesToFollow);

void mtcDoStart
  (MtcControlPtr xx);

void mtcDoStop
  (MtcControlPtr xx);

void mtcMoveRawDataFromBuffer
  (MtcControlPtr xx);

Pvoid mtcProcessResponse
  (MtcControlPtr xx,
   long          rr);

void mtcProcessTaxels
  (MtcControlPtr xx);

bool mtcReadMapFile
  (MtcControlPtr xx);

bool mtcReadNormalizationFile
  (MtcControlPtr xx);

void mtcResetNormalization
  (MtcControlPtr xx);

bool mtcSetKind
  (MtcControlPtr xx,
   PSymbol       kind);

bool mtcSetMode
  (MtcControlPtr xx,
   PSymbol       rawOrCooked);

bool mtcSetOrder
  (MtcControlPtr xx,
   PSymbol       order);

void mtcSetupIndices
  (MtcControlPtr xx);

void mtcSortTaxels
  (MtcControlPtr	xx,
   const short		numSpots);

bool mtcWriteNormalizationFile
  (MtcControlPtr xx);

StandardRoutineDeclarations(MtcControlPtr)

mextern(PSymbol) gCompressedSymbol; /* Pointer to unique symbol for 'compressed' */
mextern(PSymbol) gCookedSymbol;     /* Pointer to unique symbol for 'cooked' */
mextern(PSymbol) gEmptySymbol;      /* Pointer to unique symbol for '' */
mextern(PSymbol) gNormalSymbol;     /* Pointer to unique symbol for 'normal' */
mextern(PSymbol) gOffSymbol;        /* Pointer to unique symbol for 'off' */
mextern(PSymbol) gOnSymbol;         /* Pointer to unique symbol for 'on' */
mextern(PSymbol) gPSymbol;          /* Pointer to unique symbol for 'p' */
mextern(PSymbol) gRawSymbol;        /* Pointer to unique symbol for 'raw' */
mextern(PSymbol) gStartSymbol;      /* Pointer to unique symbol for 'start' */
mextern(PSymbol) gStopSymbol;       /* Pointer to unique symbol for 'stop' */
mextern(PSymbol) gXSymbol;          /* Pointer to unique symbol for 'x' */
mextern(PSymbol) gYSymbol;          /* Pointer to unique symbol for 'y' */

#endif /* not MTC_H_ */
