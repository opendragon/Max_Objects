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
# define X10_H_ /* */

# include "x10Base.h"

# define OUTBUFF_SIZE        50
# define MAX_POLL_RATE       1000
# define OUR_NAME            "x10"
// # define OUR_RES_NUMB        17121
# define OUTPUT_PREFIX       "x10: "
# define SER_SAMPLE_RATE     100 /* default sampling rate for the serial port */
# define COMMAND_PREFIX_SIZE 16
# define ALL_ON              0x0FF
# define CM11_POLL_CHAR      0x05a
# define CM11_CLOCK_CHAR     0x0a5
# define CM11_READY_CHAR     0x055
# define CM11_POLL_ACK       0x0c3
# define CM11_CHECKSUM_ACK   0x000
# define CM11_CLOCK_ACK      0x09b
# define MAX_RETRIES         4

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
}; // X10CommandCode

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
}; // X10FunctionCode

enum X10KindCode
{
    X10KindUnknown,
    X10KindCM11,
    X10KindCP290
}; // X10KindCode

enum X10ModeCode
{
    x10CP290ModeNormal   = 0x08,
    x10CP290ModeSecurity = 0x09,
    x10CP290ModeToday    = 0x04,
    x10CP290ModeTomorrow = 0x02,
    x10CP290ModeClear    = 0x00
}; // X10ModeCode

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
}; // X10MinorState

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
}; // X10MajorState

struct X10ControlData
{
    t_object        fObject;
    t_clock *       fPollClock;
    t_outlet *      fCommandComplete;
    t_outlet *      fCommandsOut;
    t_outlet *      fDeviceStatus;
    t_outlet *      fDayOut;
    t_outlet *      fErrorBangOut;
    t_outlet *      fHourOut;
    t_outlet *      fHouseCode;
    t_outlet *      fMinuteOut;
    t_outlet *      fPollerOut;
    t_qelem *       fPollQueue;
    unsigned char * fOutBuffer;
    unsigned char * fOutCmd;
    unsigned char * fOutArea;
    X10KindCode     fKind;
    unsigned short  fOutCount;
    unsigned short  fChecksum;
    unsigned short  fDeviceMap;
    unsigned short  fIndex;
    short           fPollRate;
    short           fAllOnCount;
    short           fEntryCount;
    short           fEventByteCount;
    short           fDimLevel;
    short           fHouseCodeChar;
    short           fFunctionChar;
    short           fRetries;
    X10MinorState   fMinorState;
    X10MajorState   fMajorState;
    bool            fCompletedWhenStatus;
    bool            fIgnoreChecksum;
    bool            fStopping;
}; // X10ControlData

#define CP290_DATETIMEDAYTOBITS_DATA  \
    { \
        0x7F, /* 0 = every day */ \
        0x40, /* 1 = Sunday */ \
        0x01, /* 2 = Monday */ \
        0x02, /* 3 = Tuesday */ \
        0x04, /* 4 = Wednesday */ \
        0x08, /* 5 = Thursday */ \
        0x10, /* 6 = Friday */ \
        0x20  /* 7 = Saturday */ \
    }

mextern(unsigned char) kCP290DateTimeDayToBits[8] minit(CP290_DATETIMEDAYTOBITS_DATA);

#define HOUSECODETOBITS_DATA    \
    { \
        0x06, /* A */ \
        0x0E, /* B */ \
        0x02, /* C */ \
        0x0A, /* D */ \
        0x01, /* E */ \
        0x09, /* F */ \
        0x05, /* G */ \
        0x0D, /* H */ \
        0x07, /* I */ \
        0x0F, /* J */ \
        0x03, /* K */ \
        0x0B, /* L */ \
        0x00, /* M */ \
        0x08, /* N */ \
        0x04, /* O */ \
        0x0C  /* P */ \
    }

mextern(unsigned char) kHouseCodeToBits[NUM_HOUSECODES] minit(HOUSECODETOBITS_DATA);

# define CLEARTIMEREVENT_HEADER(type) \
    void cmd_ClearTimerEvent(type *     xx,\
                             const long eventNumber)

# define DIM_HEADER(type) \
    void cmd_Dim(type *     xx,\
                 t_symbol * code,\
                 const long map,\
                 const long level)

# define GETGRAPHICSDATA_HEADER(type) \
    void cmd_GetGraphicsData(type * xx)

# define GETHOUSECODE_HEADER(type) \
    void cmd_GetHouseCode(type * xx)

# define GETTIMEREVENTS_HEADER(type) \
    void cmd_GetTimerEvents(type * xx)

# define KIND_HEADER(type) \
    void cmd_Kind(type *     xx,\
                  t_symbol * kind)

# define OFF_HEADER(type) \
    void cmd_Off(type *     xx,\
                 t_symbol * code,\
                 const long map)

# define ON_HEADER(type) \
    void cmd_On(type *     xx,\
                t_symbol * code,\
                const long map)

# define SETCLOCK_HEADER(type) \
    void cmd_SetClock(type * xx)

# define SETEVERYDAYEVENT_HEADER(type) \
    void cmd_SetEveryDayEvent(type *     xx,\
                              t_symbol * code,\
                              const long map,\
                              const long eventNumber,\
                              const long hourMinute,\
                              t_symbol * function,\
                              const long level)

# define SETGRAPHICSDATA_HEADER(type) \
    void cmd_SetGraphicsData(type *     xx,\
                             const long objectNumber,\
                             const long objectData)

# define SETHOUSECODE_HEADER(type) \
    void cmd_SetHouseCode(type *     xx,\
                          t_symbol * code)

# define SETNORMALEVENT_HEADER(type) \
    void cmd_SetNormalEvent(type *     xx,\
                            t_symbol * code,\
                            const long map,\
                            const long eventNumber,\
                            const long dayMap,\
                            const long hourMinute,\
                            t_symbol * function,\
                            const long level)

# define SETSECURITYEVENT_HEADER(type) \
    void cmd_SetSecurityEvent(type *     xx,\
                              t_symbol * code,\
                              const long map,\
                              const long eventNumber,\
                              const long dayMap,\
                              const long hourMinute,\
                              t_symbol * function,\
                              const long level)

# define SETTODAYEVENT_HEADER(type) \
    void cmd_SetTodayEvent(type *     xx,\
                           t_symbol * code,\
                           const long map,\
                           const long eventNumber,\
                           const long hourMinute,\
                           t_symbol * function,\
                           const long level)

# define SETTOMORROWEVENT_HEADER(type) \
    void cmd_SetTomorrowEvent(type *     xx,\
                              t_symbol * code,\
                              const long map,\
                              const long eventNumber,\
                              const long hourMinute,\
                              t_symbol * function,\
                              const long level)

CLEARTIMEREVENT_HEADER(X10ControlData);
DIM_HEADER(X10ControlData);
GETGRAPHICSDATA_HEADER(X10ControlData);
GETHOUSECODE_HEADER(X10ControlData);
GETTIMEREVENTS_HEADER(X10ControlData);
KIND_HEADER(X10ControlData);
OFF_HEADER(X10ControlData);
ON_HEADER(X10ControlData);
RESET_HEADER(X10ControlData);
SETCLOCK_HEADER(X10ControlData);
SETEVERYDAYEVENT_HEADER(X10ControlData);
SETGRAPHICSDATA_HEADER(X10ControlData);
SETHOUSECODE_HEADER(X10ControlData);
SETNORMALEVENT_HEADER(X10ControlData);
SETSECURITYEVENT_HEADER(X10ControlData);
SETTODAYEVENT_HEADER(X10ControlData);
SETTOMORROWEVENT_HEADER(X10ControlData);
XRESET_HEADER(X10ControlData);

void x10CM11DoDirect(X10ControlData *      xx,
                     const X10FunctionCode funcCode,
                     t_symbol *            code,
                     const long            map,
                     const long            level);

void x10CM11ResendBuffer(X10ControlData * xx);

void x10CM11SendBuffer(X10ControlData *     xx,
                       const unsigned short howMany);

void x10CM11SendClockInfo(X10ControlData * xx,
                          const bool       doReset,
                          const bool       reportCompletion);

void x10CM11SendDeviceAddress(X10ControlData * xx);

void x10CM11SendFunctionCode(X10ControlData * xx);

void x10CP290CalcSum(X10ControlData *     xx,
                     const unsigned short offset,
                     const unsigned short howMany);

void x10CP290DoDirect(X10ControlData *      xx,
                      const X10FunctionCode funcCode,
                      t_symbol *            code,
                      const long            map,
                      const long            level);

void x10CP290DoSingleton(X10ControlData *     xx,
                         const X10CommandCode cmd,
                         const X10MajorState  newState);

void x10CP290DoTimerEvent(X10ControlData *  xx,
                          const X10ModeCode mode,
                          t_symbol *        code,
                          const long        map,
                          const long        eventNumber,
                          const long        dayMap,
                          const long        hourMinute,
                          t_symbol *        function,
                          const long        level);

void x10CP290SendBuffer(X10ControlData *     xx,
                        const unsigned short howMany);

unsigned short x10IdentifyHouseCode(t_symbol * aSym);

StandardRoutineDeclarations(X10ControlData);

mextern(t_symbol *) gCM11Symbol;                 /* Pointer to unique symbol for 'cm11' */
mextern(t_symbol *) gCP290Symbol;                /* Pointer to unique symbol for 'cp290' */
mextern(t_symbol *) gDimSymbol;                  /* Pointer to unique symbol for 'dim' */
mextern(t_symbol *) gEmptySymbol;                /* Pointer to unique symbol for '' */
mextern(t_symbol *) gHouseCodes[NUM_HOUSECODES]; /* Pointers to unique symbols for 'A' .. 'P' */
mextern(t_symbol *) gOffSymbol;                  /* Pointer to unique symbol for 'off' */
mextern(t_symbol *) gOnSymbol;                   /* Pointer to unique symbol for 'on' */

#endif /* not X10_H_ */
