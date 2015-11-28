/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       ldp1550.h                                                               */
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

#if (! defined(LDP1550_H_))
# define LDP1550_H_  /* */

# include "missingAndExtra.h"

# define INFO_SAMPLE_RATE   10      /* default poll rate multiplier */
# define MAX_CHAPTER_NUMBER 79
# define MAX_COMMAND_LENGTH 30
# define MAX_FRAME_NUMBER   54000
# define MAX_POLL_RATE      1000
# define MAX_POOL_SIZE      200
# define MAX_REPETITIONS    15
# define MAX_STEP_FACTOR    255
# define OUR_NAME           "ldp1550"
// # define OUR_RES_NUMB       17120
# define OUTPUT_PREFIX      "ldp1550: "
# define POOL_SIZE          30  /* default message pool size */
# define SER_SAMPLE_RATE    100 /* default sampling rate for the serial port */

enum LdpMode
{
    kLdpModeFrame,
    kLdpModeChapter,
    kLdpModeSegment
}; // LdpMode

enum LdpReturnCode
{
    kLdpReturnComplete   = 0x01,
    kLdpReturnError      = 0x02,
    kLdpReturnLidOpen    = 0x03,
    kLdpReturnProgramEnd = 0x04,
    kLdpReturnNotTarget  = 0x05,
    kLdpReturnNoFrame    = 0x06,
    kLdpReturnAck        = 0x0A,
    kLdpReturnNack       = 0x0B
}; // LdpReturnCode

enum LdpCommandCode
{
    kLdpNoCommand               = 0x00,
    kLdpCommandCanInterrupt     = 0x01,
    kLdpCommandSignalAccepted   = 0x02,
    kLdpCommandSignalDone       = 0x03,
    kLdpCommandPictureOff       = 0x26,
    kLdpCommandPictureOn        = 0x27,
    kLdpCommandPscEnable        = 0x28,
    kLdpCommandPscDisable       = 0x29,
    kLdpCommandStepStillForward = 0x2B,
    kLdpCommandStepStillReverse = 0x2C,
    kLdpCommandPlayForward      = 0x3A,
    kLdpCommandPlayFastForward  = 0x3B, /* plays as 90 fps */
    kLdpCommandPlaySlowForward  = 0x3C, /* plays as 30 fps */
    kLdpCommandStepForward      = 0x3D, /* needs SPEED */
    kLdpCommandScanForward      = 0x3E,
    kLdpCommandStop             = 0x3F,
    kLdpCommandEnter            = 0x40,
    kLdpCommandSearch           = 0x43, /* needs FRAME, returns COMPLETE */
    kLdpCommandRepeat           = 0x44, /* needs FRAME */
    kLdpCommandSound1On         = 0x46,
    kLdpCommandSound1Off        = 0x47,
    kLdpCommandSound2On         = 0x48,
    kLdpCommandSound2Off        = 0x49,
    kLdpCommandPlayReverse      = 0x4A,
    kLdpCommandPlayFastReverse  = 0x4B, /* plays as 90 fps reverse */
    kLdpCommandPlaySlowReverse  = 0x4C, /* plays as 30 fps reverse */
    kLdpCommandStepReverse      = 0x4D, /* needs SPEED */
    kLdpCommandScanReverse      = 0x4E,
    kLdpCommandStill            = 0x4F,
    kLdpCommandShowOn           = 0x50,
    kLdpCommandShowOff          = 0x51,
    kLdpCommandSetFrameMode     = 0x55,
    kLdpCommandReset            = 0x56,
    kLdpCommandMemory           = 0x5A,
    kLdpCommandMSearch          = 0x5B,
    kLdpCommandSkip             = 0x5C,
    kLdpCommandAddressInquiry   = 0x60,
    kLdpCommandContinue         = 0x61,
    kLdpCommandStatusInquiry    = 0x67,
    kLdpCommandSetChapterMode   = 0x69,
    kLdpCommandChapterInquiry   = 0x76
}; // LdpCommandCode

enum LdpState
{
    kLdpStateNotWaiting,
    kLdpStateAwaitingAck,
    kLdpStateAwaitingChapterByte1,
    kLdpStateAwaitingChapterByte2,
    kLdpStateAwaitingComplete,
    kLdpStateAwaitingFrameByte1,
    kLdpStateAwaitingFrameByte2,
    kLdpStateAwaitingFrameByte3,
    kLdpStateAwaitingFrameByte4,
    kLdpStateAwaitingFrameByte5,
    kLdpStateAwaitingStatusByte1,
    kLdpStateAwaitingStatusByte2,
    kLdpStateAwaitingStatusByte3,
    kLdpStateAwaitingStatusByte4,
    kLdpStateAwaitingStatusByte5
}; // LdpState

struct LdpPacket
{
    LdpCommandCode fCommand;
    LdpPacket *    fNext;
    LdpPacket *    fPrev;
    LdpState       fState;
}; // LdpPacket

struct LdpData
{
    t_object    fObject;
    bool        fInfoSamplingEnabled;
    bool        fStopping;
    t_clock *   fPollClock;
    LdpMode     fMode;
    LdpPacket * fFirst;
    LdpPacket * fInterruptPoint;
    LdpPacket * fLast;
    LdpPacket * fPool;
    long        fFrameNumber;
    t_outlet *  fChapterNumberOut;
    t_outlet *  fCommandAccepted;
    t_outlet *  fCommandComplete;
    t_outlet *  fCommandsOut;
    t_outlet *  fCommandStatus;
    t_outlet *  fErrorBangOut;
    t_outlet *  fFrameNumberOut;
    t_outlet *  fKeyModeStatus;
    t_outlet *  fPollerOut;
    t_outlet *  fProgramStopCodeOut;
    t_qelem *   fPollQueue;
    t_atom_long fChapterNumber;
    short       fInfoCount;
    short       fInfoRate;
    short       fPollRate;
    short       fPoolAvailable;
    short       fPoolSize;
}; // LdpData

# define ALLINFO_HEADER(type) \
    void cmd_AllInfo(type * xx)

# define CHAPTER_HEADER(type) \
    void cmd_Chapter(type * xx)

# define CONTINUE_HEADER(type) \
    void cmd_Continue(type * xx)

# define FRAME_HEADER(type) \
    void cmd_Frame(type * xx)

# define MEMORY_HEADER(type) \
    void cmd_Memory(type * xx)

# define MODE_HEADER(type) \
    void cmd_Mode(type *     xx,\
                  t_symbol * mode)

# define MSEARCH_HEADER(type) \
    void cmd_MSearch(type * xx)

# define PICTURE_HEADER(type) \
    void cmd_Picture(type *     xx,\
                     t_symbol * onOff)

# define PLAY_HEADER(type) \
    void cmd_Play(type *     xx,\
                  t_symbol * mode,\
                  const long stepRate)

# define PLAYTILL_HEADER(type) \
    void cmd_PlayTill(type *     xx,\
                      const long position,\
                      t_symbol * mode,\
                      const long stepFactor)

# define PSCENABLE_HEADER(type) \
    void cmd_PSCEnable(type *     xx,\
                       t_symbol * onOff)

# define REPEAT_HEADER(type) \
    void cmd_Repeat(type *     xx,\
                    const long position,\
                    t_symbol * mode,\
                    const long repeatCount,\
                    const long stepFactor)

# define SEARCH_HEADER(type) \
    void cmd_Search(type *     xx,\
                    const long position)

# define SHOW_HEADER(type) \
    void cmd_Show(type *     xx,\
                  t_symbol * onOff)

# define SOUND_HEADER(type) \
    void cmd_Sound(type *     xx,\
                   const long chan,\
                   t_symbol * onOff)

# define STATUS_HEADER(type) \
    void cmd_Status(type * xx)

# define STEPSTILL_HEADER(type) \
    void cmd_StepStill(type *     xx,\
                       t_symbol * mode)

# define STILL_HEADER(type) \
    void cmd_Still(type * xx)

ALLINFO_HEADER(LdpData);
CHAPTER_HEADER(LdpData);
CONTINUE_HEADER(LdpData);
FRAME_HEADER(LdpData);
MEMORY_HEADER(LdpData);
MODE_HEADER(LdpData);
MSEARCH_HEADER(LdpData);
PICTURE_HEADER(LdpData);
PLAY_HEADER(LdpData);
PLAYTILL_HEADER(LdpData);
PSCENABLE_HEADER(LdpData);
REPEAT_HEADER(LdpData);
RESET_HEADER(LdpData);
SEARCH_HEADER(LdpData);
SHOW_HEADER(LdpData);
SOUND_HEADER(LdpData);
STATUS_HEADER(LdpData);
STEPSTILL_HEADER(LdpData);
STILL_HEADER(LdpData);
STOP_HEADER(LdpData);
XRESET_HEADER(LdpData);

void ldpAddCommand(LdpData *            xx,
                   const LdpCommandCode cc,
                   const LdpState       rr);

bool ldpCheckPoolSpace(LdpData *   xx,
                       const short numCommands);

void ldpClearPackets(LdpData * xx);

LdpPacket * ldpGetFirstPacket(LdpData * xx);

void ldpInitCommands(LdpData * xx);

void ldpInsertCommand(LdpData *            xx,
                      LdpPacket *          before,
                      const LdpCommandCode cc,
                      const LdpState       rr);

LdpPacket * ldpNewPacket(LdpData * xx);

void ldpReleasePacket(LdpData *   xx,
                      LdpPacket * pp);

void ldpSendCommand(LdpData * xx);

StandardRoutineDeclarations(LdpData);

mextern(t_symbol *) gChapterSymbol; /* Pointer to unique symbol for 'chapter' */
mextern(t_symbol *) gEmptySymbol;   /* Pointer to unique symbol for '' */
mextern(t_symbol *) gFastSymbol;    /* Pointer to unique symbol for 'fast' */
mextern(t_symbol *) gFrameSymbol;   /* Pointer to unique symbol for 'frame' */
mextern(t_symbol *) gFwdSymbol;     /* Pointer to unique symbol for 'fwd' */
mextern(t_symbol *) gOffSymbol;     /* Pointer to unique symbol for 'off' */
mextern(t_symbol *) gOnSymbol;      /* Pointer to unique symbol for 'on' */
mextern(t_symbol *) gRevFastSymbol; /* Pointer to unique symbol for 'rev-fast' */
mextern(t_symbol *) gRevScanSymbol; /* Pointer to unique symbol for 'rev-scan' */
mextern(t_symbol *) gRevSlowSymbol; /* Pointer to unique symbol for 'rev-slow' */
mextern(t_symbol *) gRevStepSymbol; /* Pointer to unique symbol for 'rev-step' */
mextern(t_symbol *) gRevSymbol;     /* Pointer to unique symbol for 'rev' */
mextern(t_symbol *) gScanSymbol;    /* Pointer to unique symbol for 'scan' */
mextern(t_symbol *) gSlowSymbol;    /* Pointer to unique symbol for 'slow' */
mextern(t_symbol *) gStepSymbol;    /* Pointer to unique symbol for 'step' */

#endif /* not LDP1550_H_ */
