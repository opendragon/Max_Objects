/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       spaceball.h                                                             */
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
/*  Created:    2001/07/13                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(SPACEBALL_H_))
# define SPACEBALL_H_ /* */

# include "missingAndExtra.h"

# define OUR_NAME      "spaceball"
// # define OUR_RES_NUMB  17161
# define OUTPUT_PREFIX "spaceball: "

# define BE_VERBOSE           /* */
# define REPORT_STATE_CHANGES /*!!*/

# define IN_BUFFER_SIZE  1000
# define OUT_BUFFER_SIZE 120
# define MAX_POLL_RATE   1000
# define SER_SAMPLE_RATE 100 /* default sampling rate for the serial port */

# if (! defined(BE_VERBOSE))
#  undef REPORT_STATE_CHANGES
# endif /* not BE_VERBOSE */

enum SpaceballResponseCode
{
    kSpaceballResponseEndCommand = 0x000D, /* '\r' */
    kSpaceballResponseNewLine    = 0x000A, /* '\n' */
    kSpaceballResponseDispPacket = 0x0044, /* 'D' */
    kSpaceballResponseKeyPacket  = 0x004b, /* 'K' */
    kSpaceballResponseInfoPacket = 0x0040, /* '@' */
    kSpaceballResponseInfo1      = 0x0031, /* '1' */
    kSpaceballResponseInfo2      = 0x0032, /* '2' */
    kSpaceballResponseEscape     = 0x005e, /* '^' */
    kSpaceballResponseXoff       = 0x0013, /* XOFF */
    kSpaceballResponseXon        = 0x0011  /* XON */
}; // SpaceballResponseCode

struct SpaceballData
{
    t_object      fObject;
    t_clock *     fPollClock;
    void *        fProxy;
    t_outlet *    fChunkSendOut;
    t_outlet *    fDataSendOut;
    t_outlet *    fErrorBangOut;
    t_outlet *    fSampleBangOut;
    t_outlet *    fDataOut;
    t_qelem *     fPollQueue;
    long          fInletNumber;
    unsigned char fBuffer[IN_BUFFER_SIZE];
    float         fTrans[3];
    float         fRot[3];
    short         fBufferPos;
    short         fPollRate;
    short         fResetDuration;
    short         fInitDuration;
    short         fDelayCounter;
    long          fButtons;
    bool          fChunkPulseSent;
    bool          fInited;
    bool          fNextCharEscaped;
    bool          fOutputBlocked;
    bool          fModeDelta;
    bool          fReset;
    bool          fSkipping;
    bool          fStopping;
# if defined(BE_VERBOSE)
    bool          fVerbose;
# endif /* BE_VERBOSE */
}; // SpaceballData

# define MODE_HEADER(type) \
    void cmd_Mode(type *     xx,\
                  t_symbol * addOrDelta)

# define ZERO_HEADER(type) \
    void cmd_Zero(type * xx)

MODE_HEADER(SpaceballData);
RESET_HEADER(SpaceballData);
VERBOSE_HEADER(SpaceballData);
ZERO_HEADER(SpaceballData);

void spaceballPerformWriteCommand(SpaceballData * xx,
                                  const short     numBytesToSend,
                                  unsigned char * bytesToFollow);

void spaceballProcessPacket(SpaceballData * xx);

void spaceballProcessResponse(SpaceballData * xx,
                              long            rr);

bool spaceballSetMode(SpaceballData * xx,
                      t_symbol *      addOrDelta);

void spaceballZeroValues(SpaceballData * xx);

StandardRoutineDeclarations(SpaceballData);

mextern(t_symbol *) gAddSymbol;       /* Pointer to unique symbol for 'add' */
mextern(t_symbol *) gButtonSymbol;    /* Pointer to unique symbol for 'button' */
mextern(t_symbol *) gDeltaSymbol;     /* Pointer to unique symbol for 'delta' */
mextern(t_symbol *) gEmptySymbol;     /* Pointer to unique symbol for '' */
mextern(t_symbol *) gOffSymbol;       /* Pointer to unique symbol for 'off' */
mextern(t_symbol *) gOnSymbol;        /* Pointer to unique symbol for 'on' */
mextern(t_symbol *) gRotateSymbol;    /* Pointer to unique symbol for 'rotate' */
mextern(t_symbol *) gTranslateSymbol; /* Pointer to unique symbol for 'translate' */

#endif /* not SPACEBALL_H_ */
