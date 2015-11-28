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
# define GVP100_H_ /* */

# include "missingAndExtra.h"

# define BE_VERBOSE           /* */
# define REPORT_STATE_CHANGES /*!!*/
# define DONT_RANGE_CHECK     /*!!*/

# define MAX_MESSAGE_BYTES 13 /* 125 is the true limit: we are only sending very short sequences */

# define MAX_POOL_SIZE   200
# define POOL_SIZE       50  /* default message pool size */
# define MAX_POLL_RATE   1000
# define SER_SAMPLE_RATE 100 /* default sampling rate for the serial port */

# define OUR_NAME      "gvp100"
// # define OUR_RES_NUMB  17124
# define OUTPUT_PREFIX "gvp100: "

# define GVP_ACK_CHAR  0x84
# define GVP_NACK_CHAR 0x85

# define GVP_STATUS_BUFFER_WARNING   0x20
# define GVP_STATUS_PROTOCOL_ERROR   0x40
# define GVP_STATUS_COMMAND_ACCEPTED 0x80

# define MODE_KEY_SELECTED  0x01
# define MODE_BKGD_SELECTED 0x04

# if (! defined(BE_VERBOSE))
#  undef REPORT_STATE_CHANGES
# endif /* not BE_VERBOSE */

typedef unsigned char GvpMessageBuffer[MAX_MESSAGE_BYTES + 3];

enum GvpCommandCode
{
    kNoCommand                         = 0x0000,
    kCommandWritePropgramBackgroundBus = 0x00C1,
    kCommandWritePresetBackgroundBus   = 0x00C2,
    kCommandWriteKeyBus                = 0x00C3,
    kCommandWriteAnalogControl         = 0x00C5,
    kCommandSetPushbuttonLampOn        = 0x00C6,
    kCommandSetPushbuttonLampOff       = 0x00C7,
    kCommandSetWipePattern             = 0x00C8,
    kCommandSetTransitionMode          = 0x00CA,
    kCommandSetAutoTransRate           = 0x00CC,
    kCommandSetDskMixRate              = 0x00CD,
    kCommandLearnStatusIntoEmem        = 0x00DA,
    kCommandRecallEmemToSwitcher       = 0x00DB,
    kCommandPushButton                 = 0x00FB,
    kCommandSetFadeToBlackRate         = 0x00FD,
    kCommandAllStop                    = 0x00F2,
    kCommandEndSequence                = 0x0100
}; // GvpCommandCode

enum GvpState
{
    kStateIdle,
    kStateAwaitingBreakDone,
    kStateAwaitingAck,
    kStateSelected,
    kStateAwaitingByteCount1,
    kStateAwaitingByteCountN,
    kStateAwaitingStatusByte
}; // GvpState

struct GvpPacket
{
    GvpPacket *      fNext;
    GvpPacket *      fPrev;
    GvpMessageBuffer fBuffer;
    GvpState         fState;
    bool             fSendCompletion;
    short            fSize;  /* >= 0 -> normal; < 0 -> endsequence packet */
}; // GvpPacket

struct GvpData
{
    t_object    fObject;
    GvpPacket * fFirst;
    GvpPacket * fLast;
    GvpPacket * fPool;
    t_clock *   fPollClock;
    t_outlet *  fDataSendOut;
    t_outlet *  fBreakSendOut;
    t_outlet *  fErrorBangOut;
    t_outlet *  fSampleBangOut;
    t_outlet *  fCommandComplete;
    t_outlet *  fSequenceComplete;
    t_qelem *   fPollQueue;
    GvpState    fState;
    short       fPollRate;
    t_atom_long fSelectAddress;
    short       fPoolAvailable;
    short       fPoolSize;
    bool        fStopping;
    bool        fSendCompletion;
# if defined(BE_VERBOSE)
    bool        fVerbose;
# endif /* BE_VERBOSE */
}; // GvpData

# define ALLSTOP_HEADER(type) \
    void cmd_AllStop(type * xx)

# define BREAKDONE_HEADER(type) \
    void cmd_BreakDone(type * xx)

# define ENDSEQUENCE_HEADER(type) \
    void cmd_EndSequence(type * xx)

# define LEARNEMEM_HEADER(type) \
    void cmd_LearnEmem(type *     xx,\
                       const long number)

# define RECALLEMEM_HEADER(type) \
    void cmd_RecallEmem(type *     xx,\
                        const long number)

# define SETCROSSPOINT_HEADER(type) \
    void cmd_SetCrosspoint(type *     xx,\
                           t_symbol * ss,\
                           const long argc,\
                           t_atom *   argv)

# define SETDSKANALOGCONTROL_HEADER(type) \
    void cmd_SetDskAnalogControl(type *     xx,\
                                 t_symbol * ss,\
                                 const long argc,\
                                 t_atom *   argv)

# define SETDSKCLIPLEVEL_HEADER(type) \
    void cmd_SetDskClipLevel(type *       xx,\
                             const double pos)

# define SETEFFECTSANALOGCONTROL_HEADER(type) \
    void cmd_SetEffectsAnalogControl(type *     xx,\
                                     t_symbol * ss,\
                                     const long argc,\
                                     t_atom *   argv)

# define SETEFFECTSPOSITION_HEADER(type) \
    void cmd_SetEffectsPosition(type *       xx,\
                                const double pos)

# define SETJOYSTICK_HEADER(type) \
    void cmd_SetJoystick(type *       xx,\
                         const double horiz,\
                         const double vert)

# define SETPUSHBUTTON_HEADER(type) \
    void cmd_SetPushbutton(type *     xx,\
                           t_symbol * onOffPush,\
                           const long argc,\
                           t_atom *   argv)

# define SETTAKEPOSITION_HEADER(type) \
    void cmd_SetTakePosition(type *       xx,\
                             const double pos)

# define SETTRANSITIONMODE_HEADER(type) \
    void cmd_SetTransitionMode(type *     xx,\
                               t_symbol * keyOrBkgd1,\
                               t_symbol * keyOrBkgd2)

# define SETTRANSITIONRATE_HEADER(type) \
    void cmd_SetTransitionRate(type *     xx,\
                               t_symbol * kind,\
                               const long rate,\
                               t_symbol * keyOrBkgdOrDoIt1,\
                               t_symbol * keyOrBkgdOrDoIt2,\
                               t_symbol * keyOrBkgdOrDoIt3)

# define SETWIPEPATTERN_HEADER(type) \
    void cmd_SetWipePattern(type *     xx,\
                            const long number)

ALLSTOP_HEADER(GvpData);
BREAKDONE_HEADER(GvpData);
ENDSEQUENCE_HEADER(GvpData);
LEARNEMEM_HEADER(GvpData);
RECALLEMEM_HEADER(GvpData);
SETCROSSPOINT_HEADER(GvpData);
SETDSKANALOGCONTROL_HEADER(GvpData);
SETDSKCLIPLEVEL_HEADER(GvpData);
SETEFFECTSANALOGCONTROL_HEADER(GvpData);
SETEFFECTSPOSITION_HEADER(GvpData);
SETJOYSTICK_HEADER(GvpData);
SETPUSHBUTTON_HEADER(GvpData);
SETTAKEPOSITION_HEADER(GvpData);
SETTRANSITIONMODE_HEADER(GvpData);
SETTRANSITIONRATE_HEADER(GvpData);
SETWIPEPATTERN_HEADER(GvpData);
VERBOSE_HEADER(GvpData);
XRESET_HEADER(GvpData);

bool gvpCheckPoolSpace(GvpData *   xx,
                       const short numCommands);

void gvpClearPackets(GvpData * xx);

GvpPacket * gvpGetFirstPacket(GvpData * xx);

GvpPacket * gvpNewPacket(GvpData * xx);

void gvpPerformWriteCommand(GvpData *            xx,
                            const long           effectsAddress,
                            const GvpCommandCode commandCode,
                            const unsigned char  numBytesToFollow,
                            unsigned char *      bytesToFollow,
                            const GvpState       newState,
                            const bool           lastCommand);

void gvpReleasePacket(GvpData *   xx,
                      GvpPacket * pp);

void gvpReportStateChange(GvpData * xx);

StandardRoutineDeclarations(GvpData);

mextern(t_symbol *) gASymbol;          /* Pointer to unique symbol for 'a' */
mextern(t_symbol *) gAutoSymbol;       /* Pointer to unique symbol for 'auto' */
mextern(t_symbol *) gBSymbol;          /* Pointer to unique symbol for 'b' */
mextern(t_symbol *) gBackgroundSymbol; /* Pointer to unique symbol for 'background' */
mextern(t_symbol *) gBlackSymbol;      /* Pointer to unique symbol for 'black' */
mextern(t_symbol *) gDSymbol;          /* Pointer to unique symbol for 'd' */
mextern(t_symbol *) gDoItSymbol;       /* Pointer to unique symbol for 'doit' */
mextern(t_symbol *) gDskSymbol;        /* Pointer to unique symbol for 'dsk' */
mextern(t_symbol *) gEmptySymbol;      /* Pointer to unique symbol for '' */
mextern(t_symbol *) gFSymbol;          /* Pointer to unique symbol for 'f' */
mextern(t_symbol *) gF2bSymbol;        /* Pointer to unique symbol for 'f2b' */
mextern(t_symbol *) gISymbol;          /* Pointer to unique symbol for 'i' */
mextern(t_symbol *) gInsertSymbol;     /* Pointer to unique symbol for 'insert' */
mextern(t_symbol *) gKSymbol;          /* Pointer to unique symbol for 'k' */
mextern(t_symbol *) gKeySymbol;        /* Pointer to unique symbol for 'key' */
mextern(t_symbol *) gOffSymbol;        /* Pointer to unique symbol for 'off' */
mextern(t_symbol *) gOnSymbol;         /* Pointer to unique symbol for 'on' */
mextern(t_symbol *) gPSymbol;          /* Pointer to unique symbol for 'p' */
mextern(t_symbol *) gPgmSymbol;        /* Pointer to unique symbol for 'pgm' */
mextern(t_symbol *) gProgramSymbol;    /* Pointer to unique symbol for 'program' */
mextern(t_symbol *) gPresetSymbol;     /* Pointer to unique symbol for 'preset' */
mextern(t_symbol *) gPreviewSymbol;    /* Pointer to unique symbol for 'preview' */
mextern(t_symbol *) gPushSymbol;       /* Pointer to unique symbol for 'push' */

#endif /* not GVP100_H_ */
