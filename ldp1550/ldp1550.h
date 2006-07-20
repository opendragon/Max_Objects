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
 #define LDP1550_H_  /* */
 
 /*#define USE_SYSLOG /* */

 #include "MissingAndExtra.h"

 #define INFO_SAMPLE_RATE   10      /* default poll rate multiplier */
 #define MAX_CHAPTER_NUMBER 79
 #define MAX_COMMAND_LENGTH 30
 #define MAX_FRAME_NUMBER   54000
 #define MAX_POLL_RATE      1000
 #define MAX_POOL_SIZE      200
 #define MAX_REPETITIONS    15
 #define MAX_STEP_FACTOR    255
 #define OUR_NAME           "ldp1550"
 #define OUR_RES_NUMB       17120
 #define OUTPUT_PREFIX      "ldp1550: "
 #define POOL_SIZE          30  /* default message pool size */
 #define SER_SAMPLE_RATE    100 /* default sampling rate for the serial port */

enum LdpMode
{
  LDP_FRAME_MODE,
  LDP_CHAPTER_MODE,
  LDP_SEGMENT_MODE
}; /* LdpMode */

enum LdpReturnCode
{
  LDP_COMPLETE   = 0x01,
  LDP_ERROR      = 0x02,
  LDP_LID_OPEN   = 0x03,
  LDP_PGM_END    = 0x04,
  LDP_NOT_TARGET = 0x05,
  LDP_NO_FRAME   = 0x06,
  LDP_ACK        = 0x0A,
  LDP_NACK       = 0x0B
}; /* LdpReturnCode */

enum LdpCommandCode
{
  LDP_no_command       = 0x00,
  LDP_can_interrupt    = 0x01,
  LDP_signal_accepted  = 0x02,
  LDP_signal_done      = 0x03,
  LDP_PICTURE_OFF      = 0x26,
  LDP_PICTURE_ON       = 0x27,
  LDP_PSC_ENABLE       = 0x28,
  LDP_PSC_DISABLE      = 0x29,
  LDP_STEP_STILL_FWD   = 0x2B,
  LDP_STEP_STILL_REV   = 0x2C,
  LDP_PLAY_FWD         = 0x3A,
  LDP_PLAY_F_FWD       = 0x3B, /* plays as 90 fps */
  LDP_PLAY_S_FWD       = 0x3C, /* plays as 30 fps */
  LDP_STEP_FWD         = 0x3D, /* needs SPEED */
  LDP_SCAN_FWD         = 0x3E,
  LDP_STOP             = 0x3F,
  LDP_ENTER            = 0x40,
  LDP_SEARCH           = 0x43, /* needs FRAME, returns COMPLETE */
  LDP_REPEAT           = 0x44, /* needs FRAME */
  LDP_SOUND_1_ON       = 0x46,
  LDP_SOUND_1_OFF      = 0x47,
  LDP_SOUND_2_ON       = 0x48,
  LDP_SOUND_2_OFF      = 0x49,
  LDP_PLAY_REV         = 0x4A,
  LDP_PLAY_F_REV       = 0x4B, /* plays as 90 fps reverse */
  LDP_PLAY_S_REV       = 0x4C, /* plays as 30 fps reverse */
  LDP_STEP_REV         = 0x4D, /* needs SPEED */
  LDP_SCAN_REV         = 0x4E,
  LDP_STILL            = 0x4F,
  LDP_SHOW_ON          = 0x50,
  LDP_SHOW_OFF         = 0x51,
  LDP_SET_FRAME_MODE   = 0x55,
  LDP_RESET            = 0x56,
  LDP_MEMORY           = 0x5A,
  LDP_MSEARCH          = 0x5B,
  LDP_SKIP             = 0x5C,
  LDP_ADDR_INQ         = 0x60,
  LDP_CONTINUE         = 0x61,
  LDP_STATUS_INQ       = 0x67,
  LDP_SET_CHAPTER_MODE = 0x69,
  LDP_CHAPTER_INQ      = 0x76
}; /* LdpCommandCode */

enum LdpState
{
  LDP_not_waiting,
  LDP_AWAITING_ACK,
  LDP_AWAITING_CHAPTER_BYTE_1,
  LDP_AWAITING_CHAPTER_BYTE_2,
  LDP_AWAITING_COMPLETE,
  LDP_AWAITING_FRAME_BYTE_1,
  LDP_AWAITING_FRAME_BYTE_2,
  LDP_AWAITING_FRAME_BYTE_3,
  LDP_AWAITING_FRAME_BYTE_4,
  LDP_AWAITING_FRAME_BYTE_5,
  LDP_AWAITING_STATUS_BYTE_1,
  LDP_AWAITING_STATUS_BYTE_2,
  LDP_AWAITING_STATUS_BYTE_3,
  LDP_AWAITING_STATUS_BYTE_4,
  LDP_AWAITING_STATUS_BYTE_5
}; /* LdpState */

struct LdpPacket
{
  LdpCommandCode  fCommand;
  LdpPacket *     fNext;
  LdpPacket *     fPrev;
  LdpState        fState;
}; /* LdpPacket */

typedef LdpPacket * LdpPacketPtr;

struct LdpControl
{
  Object       fObject;
  bool         fInfoSamplingEnabled;
  bool         fStopping;
  PClock       fPollClock;
  LdpMode      fMode;
  LdpPacketPtr fFirst;
  LdpPacketPtr fInterruptPoint;
  LdpPacketPtr fLast;
  LdpPacketPtr fPool;
  long         fFrameNumber;
  POutlet      fChapterNumberOut;
  POutlet      fCommandAccepted;
  POutlet      fCommandComplete;
  POutlet      fCommandsOut;
  POutlet      fCommandStatus;
  POutlet      fErrorBangOut;
  POutlet      fFrameNumberOut;
  POutlet      fKeyModeStatus;
  POutlet      fPollerOut;
  POutlet      fProgramStopCodeOut;
  PQelem       fPollQueue;
  short        fChapterNumber;
  short        fInfoCount;
  short        fInfoRate;
  short        fPollRate;
  short        fPoolAvailable;
  short        fPoolSize;
}; /* LdpControl */

typedef LdpControl * LdpControlPtr;

Pvoid cmd_AllInfo
  (LdpControlPtr xx);

Pvoid cmd_Chapter
  (LdpControlPtr xx);

Pvoid cmd_Continue
  (LdpControlPtr xx);

Pvoid cmd_Frame
  (LdpControlPtr xx);

Pvoid cmd_Memory
  (LdpControlPtr xx);

Pvoid cmd_Mode
  (LdpControlPtr xx,
   PSymbol       mode);

Pvoid cmd_MSearch
  (LdpControlPtr xx);

Pvoid cmd_Picture
  (LdpControlPtr xx,
   PSymbol       onOff);

Pvoid cmd_Play
  (LdpControlPtr xx,
   PSymbol       mode,
   long          stepRate);

Pvoid cmd_PlayTill
  (LdpControlPtr xx,
   long          position,
   PSymbol       mode,
   long          stepFactor);

Pvoid cmd_PSCEnable
  (LdpControlPtr xx,
   PSymbol       onOff);

Pvoid cmd_Repeat
  (LdpControlPtr xx,
   long          position,
   PSymbol       mode,
   long          repeatCount,
   long          stepFactor);

Pvoid cmd_Reset
  (LdpControlPtr xx);

Pvoid cmd_Search
  (LdpControlPtr xx,
   long          position);

Pvoid cmd_Show
  (LdpControlPtr xx,
   PSymbol       onOff);

Pvoid cmd_Sound
  (LdpControlPtr xx,
   long          chan,
   PSymbol       onOff);

Pvoid cmd_Status
  (LdpControlPtr xx);

Pvoid cmd_StepStill
  (LdpControlPtr xx,
   PSymbol       mode);

Pvoid cmd_Still
  (LdpControlPtr xx);

Pvoid cmd_Stop
  (LdpControlPtr xx);

Pvoid cmd_XReset
  (LdpControlPtr xx);

void ldpAddCommand
  (LdpControlPtr  			xx,
   const LdpCommandCode cc,
   const LdpState       rr);

bool ldpCheckPoolSpace
  (LdpControlPtr xx,
   const short   numCommands);

void ldpClearPackets
  (LdpControlPtr xx);

LdpPacketPtr ldpGetFirstPacket
  (LdpControlPtr xx);

void ldpInitCommands
  (LdpControlPtr xx);

void ldpInsertCommand
  (LdpControlPtr  			xx,
   LdpPacketPtr   			before,
   const LdpCommandCode cc,
   const LdpState       rr);

LdpPacketPtr ldpNewPacket
  (LdpControlPtr xx);

void ldpReleasePacket
  (LdpControlPtr xx,
   LdpPacketPtr  pp);

void ldpSendCommand
  (LdpControlPtr xx);

StandardRoutineDeclarations(LdpControlPtr)

mextern(PSymbol) gChapterSymbol; /* Pointer to unique Symbol for 'chapter' */
mextern(PSymbol) gEmptySymbol;   /* Pointer to unique Symbol for '' */
mextern(PSymbol) gFastSymbol;    /* Pointer to unique Symbol for 'fast' */
mextern(PSymbol) gFrameSymbol;   /* Pointer to unique Symbol for 'frame' */
mextern(PSymbol) gFwdSymbol;     /* Pointer to unique Symbol for 'fwd' */
mextern(PSymbol) gOffSymbol;     /* Pointer to unique Symbol for 'off' */
mextern(PSymbol) gOnSymbol;      /* Pointer to unique Symbol for 'on' */
mextern(PSymbol) gRevFastSymbol; /* Pointer to unique Symbol for 'rev-fast' */
mextern(PSymbol) gRevScanSymbol; /* Pointer to unique Symbol for 'rev-scan' */
mextern(PSymbol) gRevSlowSymbol; /* Pointer to unique Symbol for 'rev-slow' */
mextern(PSymbol) gRevStepSymbol; /* Pointer to unique Symbol for 'rev-step' */
mextern(PSymbol) gRevSymbol;     /* Pointer to unique Symbol for 'rev' */
mextern(PSymbol) gScanSymbol;    /* Pointer to unique Symbol for 'scan' */
mextern(PSymbol) gSlowSymbol;    /* Pointer to unique Symbol for 'slow' */
mextern(PSymbol) gStepSymbol;    /* Pointer to unique Symbol for 'step' */

#endif /* not LDP1550_H_ */