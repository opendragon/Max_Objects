/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       gvp100.h                                                                */
/*                                                                                      */
/*  Contains:   The data type declarations and forward references to routines.          */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 1998 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    1998/06/14                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(GVP100_H_))
 #define GVP100_H_ /* */
 
 /*#define USE_SYSLOG /* */

 #include "MissingAndExtra.h"

 #define BE_VERBOSE           /* */
 #define REPORT_STATE_CHANGES /*!!*/
 #define DONT_RANGE_CHECK     /*!!*/

 #define MAX_MESSAGE_BYTES 13 /* 125 is the true limit: we are only sending very short sequences */

 #define MAX_POOL_SIZE   200
 #define POOL_SIZE       50  /* default message pool size */
 #define MAX_POLL_RATE   1000
 #define SER_SAMPLE_RATE 100 /* default sampling rate for the serial port */

 #define OUR_NAME      "gvp100"
 #define OUR_RES_NUMB  17124
 #define OUTPUT_PREFIX "gvp100: "

 #define GVP_ACK_CHAR  0x84
 #define GVP_NACK_CHAR 0x85

 #define GVP_STATUS_BUFFER_WARNING   0x20
 #define GVP_STATUS_PROTOCOL_ERROR   0x40
 #define GVP_STATUS_COMMAND_ACCEPTED 0x80

 #define MODE_KEY_SELECTED  0x01
 #define MODE_BKGD_SELECTED 0x04

 #if (! defined(BE_VERBOSE))
  #undef REPORT_STATE_CHANGES
 #endif /* not BE_VERBOSE */

typedef uchar GvpMessageBuffer[MAX_MESSAGE_BYTES + 3];

enum GvpCommandCode
{
  GVP_no_command                       = 0x0000,
  GVP_CMD_WRITE_PROGRAM_BACKGROUND_BUS = 0x00C1,
  GVP_CMD_WRITE_PRESET_BACKGROUND_BUS  = 0x00C2,
  GVP_CMD_WRITE_KEY_BUS                = 0x00C3,
  GVP_CMD_WRITE_ANALOG_CONTROL         = 0x00C5,
  GVP_CMD_SET_PB_L_ON                  = 0x00C6,
  GVP_CMD_SET_PB_L_OFF                 = 0x00C7,
  GVP_CMD_SET_WIPE_PATTERN             = 0x00C8,
  GVP_CMD_SET_TRANSITION_MODE          = 0x00CA,
  GVP_CMD_SET_AUTO_TRANS_RATE          = 0x00CC,
  GVP_CMD_SET_DSK_MIX_RATE             = 0x00CD,
  GVP_CMD_LEARN_STATUS_INTO_E_MEM      = 0x00DA,
  GVP_CMD_RECALL_E_MEM_TO_SWITCHER     = 0x00DB,
  GVP_CMD_PUSH_BUTTON                  = 0x00FB,
  GVP_CMD_SET_FADE_TO_BLACK_RATE       = 0x00FD,
  GVP_CMD_ALL_STOP                     = 0x00F2,
  GVP_end_sequence                     = 0x0100
}; /* GvpCommandCode */

enum GvpState
{
  GVP_IDLE,
  GVP_AWAITING_BREAK_DONE,
  GVP_AWAITING_ACK,
  GVP_SELECTED,
  GVP_AWAITING_BYTE_COUNT_1,
  GVP_AWAITING_BYTE_COUNT_n,
  GVP_AWAITING_STATUS_BYTE
}; /* GvpState */

struct GvpPacket
{
  GvpPacket *       fNext;
  GvpPacket *       fPrev;
  GvpMessageBuffer  fBuffer;
  GvpState          fState;
  bool              fSendCompletion;
  short             fSize;  /* >= 0 -> normal; < 0 -> endsequence packet */
}; /* GvpPacket */

typedef GvpPacket * GvpPacketPtr;

struct GvpControl
{
  Object       fObject;
  GvpPacketPtr fFirst;
  GvpPacketPtr fLast;
  GvpPacketPtr fPool;
  PClock       fPollClock;
  POutlet      fDataSendOut;
  POutlet      fBreakSendOut;
  POutlet      fErrorBangOut;
  POutlet      fSampleBangOut;
  POutlet      fCommandComplete;
  POutlet      fSequenceComplete;
  PQelem       fPollQueue;
  GvpState     fState;
  short        fPollRate;
  short        fSelectAddress;
  short        fPoolAvailable;
  short        fPoolSize;
  bool         fStopping;
  bool         fSendCompletion;
 #if defined(BE_VERBOSE)
  bool         fVerbose;
 #endif /* BE_VERBOSE */
}; /* GvpControl */

typedef GvpControl * GvpControlPtr;

Pvoid cmd_AllStop
  (GvpControlPtr xx);

Pvoid cmd_BreakDone
  (GvpControlPtr xx);

Pvoid cmd_EndSequence
  (GvpControlPtr xx);

Pvoid cmd_LearnEmem
  (GvpControlPtr xx,
   long          number);

Pvoid cmd_RecallEmem
  (GvpControlPtr xx,
   long          number);

Pvoid cmd_SetCrosspoint
  (GvpControlPtr xx,
   PSymbol       ss,
   short         argc,
   PAtom         argv);

Pvoid cmd_SetDskAnalogControl
  (GvpControlPtr xx,
   PSymbol       ss,
   short         argc,
   PAtom         argv);

Pvoid cmd_SetDskClipLevel
  (GvpControlPtr xx,
   double        pos);

Pvoid cmd_SetEffectsAnalogControl
  (GvpControlPtr xx,
   PSymbol       ss,
   short         argc,
   PAtom         argv);

Pvoid cmd_SetEffectsPosition
  (GvpControlPtr xx,
   double        pos);

Pvoid cmd_SetJoystick
  (GvpControlPtr xx,
   double        horiz,
   double        vert);

Pvoid cmd_SetPushbutton
  (GvpControlPtr xx,
   PSymbol       onOffPush,
   short         argc,
   PAtom         argv);

Pvoid cmd_SetTakePosition
  (GvpControlPtr xx,
   double        pos);

Pvoid cmd_SetTransitionMode
  (GvpControlPtr xx,
   PSymbol       keyOrBkgd1,
   PSymbol       keyOrBkgd2);

Pvoid cmd_SetTransitionRate
  (GvpControlPtr xx,
   PSymbol       kind,
   long          rate,
   PSymbol       keyOrBkgd1OrDoIt,
   PSymbol       keyOrBkgd2OrDoIt,
   PSymbol       keyOrBkgd3OrDoIt);

Pvoid cmd_SetWipePattern
  (GvpControlPtr xx,
   long          number);

Pvoid cmd_Verbose
  (GvpControlPtr xx,
   PSymbol       onOff);

Pvoid cmd_XReset
  (GvpControlPtr xx);

bool gvpCheckPoolSpace
  (GvpControlPtr xx,
   const short   numCommands);

void gvpClearPackets
  (GvpControlPtr xx);

GvpPacketPtr gvpGetFirstPacket
  (GvpControlPtr xx);

GvpPacketPtr gvpNewPacket
  (GvpControlPtr xx);

void gvpPerformWriteCommand
  (GvpControlPtr  			xx,
   const long     			effectsAddress,
   const GvpCommandCode commandCode,
   const uchar          numBytesToFollow,
   Puchar         			bytesToFollow,
   const GvpState       newState,
   const bool           lastCommand);

void gvpReleasePacket
  (GvpControlPtr xx,
   GvpPacketPtr  pp);

void gvpReportStateChange
  (GvpControlPtr xx);

StandardRoutineDeclarations(GvpControlPtr)

mextern(PSymbol) gASymbol;          /* Pointer to unique symbol for 'a' */
mextern(PSymbol) gAutoSymbol;       /* Pointer to unique symbol for 'auto' */
mextern(PSymbol) gBSymbol;          /* Pointer to unique symbol for 'b' */
mextern(PSymbol) gBackgroundSymbol; /* Pointer to unique symbol for 'background' */
mextern(PSymbol) gBlackSymbol;      /* Pointer to unique symbol for 'black' */
mextern(PSymbol) gDSymbol;          /* Pointer to unique symbol for 'd' */
mextern(PSymbol) gDoItSymbol;       /* Pointer to unique symbol for 'doit' */
mextern(PSymbol) gDskSymbol;        /* Pointer to unique symbol for 'dsk' */
mextern(PSymbol) gEmptySymbol;      /* Pointer to unique symbol for '' */
mextern(PSymbol) gFSymbol;          /* Pointer to unique symbol for 'f' */
mextern(PSymbol) gF2bSymbol;        /* Pointer to unique symbol for 'f2b' */
mextern(PSymbol) gISymbol;          /* Pointer to unique symbol for 'i' */
mextern(PSymbol) gInsertSymbol;     /* Pointer to unique symbol for 'insert' */
mextern(PSymbol) gKSymbol;          /* Pointer to unique symbol for 'k' */
mextern(PSymbol) gKeySymbol;        /* Pointer to unique symbol for 'key' */
mextern(PSymbol) gOffSymbol;        /* Pointer to unique symbol for 'off' */
mextern(PSymbol) gOnSymbol;         /* Pointer to unique symbol for 'on' */
mextern(PSymbol) gPSymbol;          /* Pointer to unique symbol for 'p' */
mextern(PSymbol) gPgmSymbol;        /* Pointer to unique symbol for 'pgm' */
mextern(PSymbol) gProgramSymbol;    /* Pointer to unique symbol for 'program' */
mextern(PSymbol) gPresetSymbol;     /* Pointer to unique symbol for 'preset' */
mextern(PSymbol) gPreviewSymbol;    /* Pointer to unique symbol for 'preview' */
mextern(PSymbol) gPushSymbol;       /* Pointer to unique symbol for 'push' */

#endif /* not GVP100_H_ */