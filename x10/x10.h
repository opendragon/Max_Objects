/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       x10.h                                                                   */
/*                                                                                      */
/*  Contains:   The data type declarations and forward references to routines.          */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 1996 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    1996/09/21                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(X10_H_))
 #define X10_H_ /* */
 
 /*#define USE_SYSLOG /* */

 #include "x10Base.h"

 #define OUTBUFF_SIZE        50
 #define MAX_POLL_RATE       1000
 #define OUR_NAME            "x10"
 #define OUR_RES_NUMB        17121
 #define OUTPUT_PREFIX       "x10: "
 #define SER_SAMPLE_RATE     100 /* default sampling rate for the serial port */
 #define COMMAND_PREFIX_SIZE 16
 #define ALL_ON              0x0FF
 #define CM11_POLL_CHAR      0x05a
 #define CM11_CLOCK_CHAR     0x0a5
 #define CM11_READY_CHAR     0x055
 #define CM11_POLL_ACK       0x0c3
 #define CM11_CHECKSUM_ACK   0x000
 #define CM11_CLOCK_ACK      0x09b
 #define MAX_RETRIES         4
 
enum X10CommandCode
{
  x10CP290DownloadBaseHouseCode        = 0x00,
  x10CP290Direct                       = 0x01,
  x10CP290SetClock                     = 0x02,
  x10CP290DownloadTimerEvent           = 0x03,
  x10CP290DownloadGraphicsData         = 0x03,
  x10CP290RequestClockAndBaseHouseCode = 0x04,
  x10CP290RequestTimerEvents           = 0x05,
  x10CP290RequestGraphicsData          = 0x06,
  x10CP290Diagnostic                   = 0x07
}; /* X10CommandCode */

enum X10FunctionCode
{
  x10FunctionUnknown      = -1,
  x10CM11FunctionUnitsOff = 0x00,
  x10CM11FunctionAllOn    = 0x01,
  x10CM11FunctionOn       = 0x02,
  x10CM11FunctionOff      = 0x03,
  x10CM11FunctionDim      = 0x04,
  x10CM11FunctionBright   = 0x05,
  x10CM11FunctionAllOff   = 0x06,
  x10CP290FunctionOn      = 0x02,
  x10CP290FunctionOff     = 0x03,
  x10CP290FunctionDimIn   = 0x04,
  x10CP290FunctionDimOut  = 0x05
}; /* X10FunctionCode */

enum X10KindCode
{
  X10KindUnknown,
  X10KindCM11,
  X10KindCP290
}; /* X10KindCode */

enum X10ModeCode
{
  x10CP290ModeNormal   = 0x08,
  x10CP290ModeSecurity = 0x09,
  x10CP290ModeToday    = 0x04,
  x10CP290ModeTomorrow = 0x02,
  x10CP290ModeClear    = 0x00
}; /* X10ModeCode */

enum X10MinorState
{
  x10CM11MinorIdle,
  x10CM11MinorAwaitingChecksum,
  x10CM11MinorAwaitingReady,
  x10CM11MinorAwaitingByteCount,
  x10CM11MinorGettingPollChars,
  x10CP290MinorIdle,
  x10CP290MinorSawFF,
  x10CP290MinorSawStatus,
  x10CP290MinorSawHouseCode,
  x10CP290MinorSawUnits1,
  x10CP290MinorSawUnits2,
  x10CP290MinorSawBaseHouseCode,
  x10CP290MinorSawMinutes,
  x10CP290MinorSawHours,
  x10CP290MinorSawDays,
  x10CP290MinorSawIcon1,
  x10CP290MinorSawIcon2,
  x10CP290MinorSawEvent1,
  x10CP290MinorSawEvent8
}; /* X10MinorState */

enum X10MajorState
{
  x10CM11MajorIdle,
  x10CM11MajorGotPollRequest,
  x10CM11MajorGotClockRequest,
  x10CM11MajorSendingMultiRequest,
  x10CM11MajorSendingSingleRequest,
  x10CM11MajorFailure,
  x10CP290MajorIdle,
  x10CP290MajorAwaitingACK,
  x10CP290MajorAwaitingACKAndUpload,
  x10CP290MajorAwaitingClockAndHouseCode,
  x10CP290MajorAwaitingGraphicsData,
  x10CP290MajorAwaitingTimerEvents,
  x10CP290MajorAwaitingUpload
}; /* X10MajorState */

struct X10Control
{
  Object        fObject;
  PClock        fPollClock;
  POutlet       fCommandComplete;
  POutlet       fCommandsOut;
  POutlet       fDeviceStatus;
  POutlet       fDayOut;
  POutlet       fErrorBangOut;
  POutlet       fHourOut;
  POutlet       fHouseCode;
  POutlet       fMinuteOut;
  POutlet       fPollerOut;
  PQelem        fPollQueue;
  Puchar        fOutBuffer;
  Puchar        fOutCmd;
  Puchar        fOutArea;
  X10KindCode   fKind;
  ushort        fOutCount;
  ushort        fChecksum;
  ushort        fDeviceMap;
  ushort		    fIndex;
  short         fPollRate;
  short         fAllOnCount;
  short         fEntryCount;
  short         fEventByteCount;
  short         fDimLevel;
  short         fHouseCodeChar;
  short         fFunctionChar;
  short         fRetries;
  X10MinorState fMinorState;
  X10MajorState fMajorState;
  bool          fCompletedWhenStatus;
  bool          fIgnoreChecksum;
  bool					fStopping;
}; /* X10Control */

typedef X10Control * X10ControlPtr;

 #define CP290_DATETIMEDAYTOBITS_DATA  \
{\
  0x7F, /* 0 = every day */\
  0x40, /* 1 = Sunday */\
  0x01, /* 2 = Monday */\
  0x02, /* 3 = Tuesday */\
  0x04, /* 4 = Wednesday */\
  0x08, /* 5 = Thursday */\
  0x10, /* 6 = Friday */\
  0x20  /* 7 = Saturday */\
}

mextern(uchar) kCP290DateTimeDayToBits[8] minit(CP290_DATETIMEDAYTOBITS_DATA);

 #define HOUSECODETOBITS_DATA    \
{\
  0x06, /* A */\
  0x0E, /* B */\
  0x02, /* C */\
  0x0A, /* D */\
  0x01, /* E */\
  0x09, /* F */\
  0x05, /* G */\
  0x0D, /* H */\
  0x07, /* I */\
  0x0F, /* J */\
  0x03, /* K */\
  0x0B, /* L */\
  0x00, /* M */\
  0x08, /* N */\
  0x04, /* O */\
  0x0C  /* P */\
}

mextern(uchar) kHouseCodeToBits[NUM_HOUSECODES] minit(HOUSECODETOBITS_DATA);

Pvoid cmd_ClearTimerEvent
  (X10ControlPtr xx,
   long          eventNumber);

Pvoid cmd_Dim
  (X10ControlPtr xx,
   PSymbol       code,
   long          map,
   long          level);

Pvoid cmd_GetHouseCode
  (X10ControlPtr xx);

Pvoid cmd_GetGraphicsData
  (X10ControlPtr xx);

Pvoid cmd_GetTimerEvents
(X10ControlPtr xx);

Pvoid cmd_Kind
  (X10ControlPtr xx,
   PSymbol       kind);

Pvoid cmd_Off
  (X10ControlPtr xx,
   PSymbol       code,
   long          map);

Pvoid cmd_On
  (X10ControlPtr xx,
   PSymbol       code,
   long          map);

Pvoid cmd_Reset
  (X10ControlPtr xx);

Pvoid cmd_SetClock
  (X10ControlPtr xx);

Pvoid cmd_SetEveryDayEvent
  (X10ControlPtr xx,
   PSymbol       code,
   long          map,
   long          eventNumber,
   long          hourMinute,
   PSymbol       function,
   long          level);

Pvoid cmd_SetGraphicsData
  (X10ControlPtr xx,
   long          objectNumber,
   long          objectData);

Pvoid cmd_SetHouseCode
  (X10ControlPtr xx,
   PSymbol       code);

Pvoid cmd_SetNormalEvent
  (X10ControlPtr xx,
   PSymbol       code,
   long          map,
   long          eventNumber,
   long          dayMap,
   long          hourMinute,
   PSymbol       function,
   long          level);

Pvoid cmd_SetSecurityEvent
  (X10ControlPtr xx,
   PSymbol       code,
   long          map,
   long          eventNumber,
   long          dayMap,
   long          hourMinute,
   PSymbol       function,
   long          level);

Pvoid cmd_SetTodayEvent
  (X10ControlPtr xx,
   PSymbol       code,
   long          map,
   long          eventNumber,
   long          hourMinute,
   PSymbol       function,
   long          level);

Pvoid cmd_SetTomorrowEvent
  (X10ControlPtr xx,
   PSymbol       code,
   long          map,
   long          eventNumber,
   long          hourMinute,
   PSymbol       function,
   long          level);

Pvoid cmd_XReset
  (X10ControlPtr xx);

void x10CM11DoDirect
  (X10ControlPtr					xx,
   const X10FunctionCode	funcCode,
   PSymbol         				code,
   const long							map,
   const long							level);

void x10CM11ResendBuffer
  (X10ControlPtr xx);

void x10CM11SendBuffer
  (X10ControlPtr	xx,
   const ushort		howMany);

void x10CM11SendClockInfo
  (X10ControlPtr	xx,
   const bool			doReset,
   const bool			reportCompletion);

void x10CM11SendDeviceAddress
  (X10ControlPtr xx);

void x10CM11SendFunctionCode
  (X10ControlPtr xx);

void x10CP290CalcSum
  (X10ControlPtr	xx,
   const ushort		offset,
   const ushort		howMany);

void x10CP290DoDirect
  (X10ControlPtr					xx,
   const X10FunctionCode	funcCode,
   PSymbol								code,
   const long							map,
   const long							level);

void x10CP290DoSingleton
  (X10ControlPtr				xx,
   const X10CommandCode	cmd,
   const X10MajorState	newState);

void x10CP290DoTimerEvent
  (X10ControlPtr			xx,
   const X10ModeCode	mode,
   PSymbol						code,
   const long					map,
   const long					eventNumber,
   const long					dayMap,
   const long					hourMinute,
   PSymbol						function,
   const long					level);

void x10CP290SendBuffer
  (X10ControlPtr	xx,
   const ushort		howMany);

ushort x10IdentifyHouseCode
  (PSymbol aSym);

StandardRoutineDeclarations(X10ControlPtr)

mextern(PSymbol) gCM11Symbol;                 /* Pointer to unique Symbol for 'cm11' */
mextern(PSymbol) gCP290Symbol;                /* Pointer to unique Symbol for 'cp290' */
mextern(PSymbol) gDimSymbol;                  /* Pointer to unique Symbol for 'dim' */
mextern(PSymbol) gEmptySymbol;                /* Pointer to unique Symbol for '' */
mextern(PSymbol) gHouseCodes[NUM_HOUSECODES]; /* Pointers to unique Symbols for 'A' .. 'P' */
mextern(PSymbol) gOffSymbol;                  /* Pointer to unique Symbol for 'off' */
mextern(PSymbol) gOnSymbol;                   /* Pointer to unique Symbol for 'on' */

#endif /* not X10_H_ */
