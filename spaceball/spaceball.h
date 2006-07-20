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
 #define SPACEBALL_H_ /* */
 
 /*#define USE_SYSLOG /* */

 #include "MissingAndExtra.h"

 #define OUR_NAME      "spaceball"
 #define OUR_RES_NUMB  17161
 #define OUTPUT_PREFIX "spaceball: "

 #define BE_VERBOSE           /* */
 #define REPORT_STATE_CHANGES /*!!*/

 #define IN_BUFFER_SIZE  1000
 #define OUT_BUFFER_SIZE 120
 #define MAX_POLL_RATE   1000
 #define SER_SAMPLE_RATE 100 /* default sampling rate for the serial port */

 #if (! defined(BE_VERBOSE))
  #undef REPORT_STATE_CHANGES
 #endif /* not BE_VERBOSE */

enum SpaceballResponseCode
{
  SPACEBALL_RESPONSE_END_CMD     = 0x000D, /* '\r' */
  SPACEBALL_RESPONSE_NEW_LINE    = 0x000A, /* '\n' */
  SPACEBALL_RESPONSE_DISP_PACKET = 0x0044, /* 'D' */
  SPACEBALL_RESPONSE_KEY_PACKET  = 0x004b, /* 'K' */
  SPACEBALL_RESPONSE_INFO_PACKET = 0x0040, /* '@' */
  SPACEBALL_RESPONSE_INFO_1      = 0x0031, /* '1' */
  SPACEBALL_RESPONSE_INFO_2      = 0x0032, /* '2' */
  SPACEBALL_RESPONSE_ESCAPE      = 0x005e, /* '^' */
  SPACEBALL_RESPONSE_XOFF        = 0x0013, /* XOFF */
  SPACEBALL_RESPONSE_XON         = 0x0011  /* XON */
}; /* SpaceballResponseCode */

struct SpaceballControl
{
  Object  fObject;
  PClock  fPollClock;
  Pvoid   fProxy;
  POutlet fChunkSendOut;
  POutlet fDataSendOut;
  POutlet fErrorBangOut;
  POutlet fSampleBangOut;
  POutlet fDataOut;
  PQelem  fPollQueue;
  long    fInletNumber;
  uchar   fBuffer[IN_BUFFER_SIZE];
  float   fTrans[3];
  float   fRot[3];
  short   fBufferPos;
  short   fPollRate;
  short   fResetDuration;
  short   fInitDuration;
  short   fDelayCounter;
  long    fButtons;
  bool    fChunkPulseSent;
  bool    fInited;
  bool    fNextCharEscaped;
  bool    fOutputBlocked;
  bool    fModeDelta;
  bool    fReset;
  bool    fSkipping;
  bool		fStopping;
 #if defined(BE_VERBOSE)
  bool    fVerbose;
 #endif /* BE_VERBOSE */
}; /* SpaceballControl */

typedef SpaceballControl * SpaceballControlPtr;

Pvoid cmd_Mode
  (SpaceballControlPtr xx,
   PSymbol             addOrDelta);

Pvoid cmd_Reset
  (SpaceballControlPtr xx);

Pvoid cmd_Verbose
  (SpaceballControlPtr xx,
   PSymbol             onOff);

Pvoid cmd_Zero
  (SpaceballControlPtr xx);

void spaceballPerformWriteCommand
  (SpaceballControlPtr xx,
   const short         numBytesToSend,
   Puchar              bytesToFollow);

void spaceballProcessPacket
  (SpaceballControlPtr xx);

Pvoid spaceballProcessResponse
  (SpaceballControlPtr xx,
   long                rr);

bool spaceballSetMode
  (SpaceballControlPtr xx,
   PSymbol             addOrDelta);

void spaceballZeroValues
  (SpaceballControlPtr xx);

StandardRoutineDeclarations(SpaceballControlPtr)

mextern(PSymbol) gAddSymbol;       /* Pointer to unique symbol for 'add' */
mextern(PSymbol) gButtonSymbol;    /* Pointer to unique symbol for 'button' */
mextern(PSymbol) gDeltaSymbol;     /* Pointer to unique symbol for 'delta' */
mextern(PSymbol) gEmptySymbol;     /* Pointer to unique symbol for '' */
mextern(PSymbol) gOffSymbol;       /* Pointer to unique symbol for 'off' */
mextern(PSymbol) gOnSymbol;        /* Pointer to unique symbol for 'on' */
mextern(PSymbol) gRotateSymbol;    /* Pointer to unique symbol for 'rotate' */
mextern(PSymbol) gTranslateSymbol; /* Pointer to unique symbol for 'translate' */

#endif /* not SPACEBALL_H_ */