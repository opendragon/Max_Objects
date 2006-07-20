/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       serialX.h                                                               */
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
/*  Created:    1998/06/17                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(SERIALX_H_))
 #define SERIALX_H_  /* */
 
 //#define USE_SYSLOG /* */
 //#define ALLOW_TRACE /* */

 #include "MissingAndExtra.h"

 #include <math.h>
 #if defined(COMPILE_FOR_CATS)
  #include <fcntl.h>
  #include <sys/ioctl.h>
  #include <termios.h>
 #else /* not COMPILE_FOR_CATS */
  #include <Serial.h>
  #include <CommResources.h>
  #include <CRMSerialDevices.h>
 #endif /* not COMPILE_FOR_CATS */

 #define __cstdio__  /* suppress in ctype.h! */
 #pragma push
 #pragma warn_implicitconv off
 #include <ctype.h>
 #pragma pop

 #define OUR_NAME      "serialX"
 #define OUR_RES_NUMB  17125
 #define OUTPUT_PREFIX "serialX: "

 #define INBUFFER_SIZE  25000
 #define LOCBUFFER_SIZE 4000

 #define DEFAULT_CHUNK  10
 #define MAX_CHUNK_SIZE 80

 #define baud115K 115000  /* value not currently used */
 #define baud230K 230000  /* value not currently used */

struct SerialXControl
{
  Object         fObject;
  long           fBaudRate;
  long           fBaudRateBits;
  long           fDataBits;
  long           fStopBits;
  long           fParityState;
  PClock         fBreakClock;
  PClock         fClearClock;
  POutlet        fCharOut;
  POutlet        fBreakDone;
  POutlet        fBreakSeen;
  Handle         fInBuffer;
  Ptr            fLocalBuffer;
  PAtom          fChunkData;
  short          fChunkSize;
  bool           fChunked;
  bool           fDTRHandshakeActive;
  bool           fExternalClock;
#if defined(COMPILE_FOR_CATS)
  struct termios fOriginalAttrs;
  int            fFileDescriptor;
  Pchar          fPortName;
  ulong          fPortNameLength;
#else /* not COMPILE_FOR_CATS */
  SInt16         fInRefNum;
  SInt16         fOutRefNum;
  char           fPortCharacter;
#endif /* not COMPILE_FOR_CATS */
}; /* SerialXControl */

typedef SerialXControl * SerialXControlPtr;

Pvoid cmd_Baud
  (SerialXControlPtr xx,
   PSymbol           message,
   short             argc,
   PAtom             argv);

Pvoid cmd_Break
  (SerialXControlPtr xx);

Pvoid cmd_Chunk
  (SerialXControlPtr xx,
   PSymbol           onOff);

Pvoid cmd_DTR
  (SerialXControlPtr xx,
   PSymbol           onOff);

Pvoid cmd_Text
  (SerialXControlPtr xx,
   PSymbol           message,
   short             argc,
   PAtom             argv);

long serialXCheckLongBaudRate
  (const long	baudRate);

long serialXCheckSymbolBaudRate
  (PSymbol	baudSymbol,
   long &		baudRate);

OSErr serialXForceDTRState
  (SerialXControlPtr	xx,
   const bool					isAsserted);

OSErr serialXSetDTRHandshake
  (SerialXControlPtr xx);

bool serialXSetPortCharacteristics
  (SerialXControlPtr xx);

OSErr serialXWriteACharacter
  (SerialXControlPtr	xx,
   const long					msg);

OSErr serialXWriteASymbol
  (SerialXControlPtr xx,
   Ptr               msg);

StandardRoutineDeclarations(SerialXControlPtr)

mextern(PSymbol) gAssertSymbol;      /* Pointer to unique Symbol for 'assert' */
mextern(PSymbol) gBaud_3KSymbol;     /* Pointer to unique Symbol for '.3k' */
mextern(PSymbol) gBaud_6KSymbol;     /* Pointer to unique Symbol for '.6k' */
mextern(PSymbol) gBaud0_3KSymbol;    /* Pointer to unique Symbol for '0.3k' */
mextern(PSymbol) gBaud0_6KSymbol;    /* Pointer to unique Symbol for '0.6k' */
mextern(PSymbol) gBaud1_2KSymbol;    /* Pointer to unique Symbol for '1.2k' */
mextern(PSymbol) gBaud1_8KSymbol;    /* Pointer to unique Symbol for '1.8k' */
mextern(PSymbol) gBaud2_4KSymbol;    /* Pointer to unique Symbol for '2.4k' */
mextern(PSymbol) gBaud3_6KSymbol;    /* Pointer to unique Symbol for '3.6k' */
mextern(PSymbol) gBaud4_8KSymbol;    /* Pointer to unique Symbol for '4.8k' */
mextern(PSymbol) gBaud7_2KSymbol;    /* Pointer to unique Symbol for '7.2k' */
mextern(PSymbol) gBaud9_6KSymbol;    /* Pointer to unique Symbol for '9.6k' */
mextern(PSymbol) gBaud14_4KSymbol;   /* Pointer to unique Symbol for '14.4k' */
mextern(PSymbol) gBaud19_2KSymbol;   /* Pointer to unique Symbol for '19.2k' */
mextern(PSymbol) gBaud28_8KSymbol;   /* Pointer to unique Symbol for '28.8k' */
mextern(PSymbol) gBaud38_4KSymbol;   /* Pointer to unique Symbol for '38.4k' */
mextern(PSymbol) gBaud57_6KSymbol;   /* Pointer to unique Symbol for '57.6k' */
mextern(PSymbol) gBaud115_2KSymbol;  /* Pointer to unique Symbol for '115.2k' */
mextern(PSymbol) gBaud230_4KSymbol;  /* Pointer to unique Symbol for '230.4k' */
mextern(PSymbol) gBaudMIDI_1Symbol;  /* Pointer to unique Symbol for 'MIDI_1' */
mextern(PSymbol) gBaudMIDI_16Symbol; /* Pointer to unique Symbol for 'MIDI_16' */
mextern(PSymbol) gBaudMIDI_32Symbol; /* Pointer to unique Symbol for 'MIDI_32' */
mextern(PSymbol) gBaudMIDI_64Symbol; /* Pointer to unique Symbol for 'MIDI_64' */
mextern(PSymbol) gEmptySymbol;       /* Pointer to unique Symbol for '' */
mextern(PSymbol) gEvenSymbol;        /* Pointer to unique Symbol for 'even' */
mextern(PSymbol) gModemSymbol;       /* Pointer to unique Symbol for 'modem' */
mextern(PSymbol) gNegateSymbol;      /* Pointer to unique Symbol for 'negate' */
mextern(PSymbol) gNoSymbol;          /* Pointer to unique Symbol for 'no' */
mextern(PSymbol) gNoneSymbol;        /* Pointer to unique Symbol for 'none' */
mextern(PSymbol) gOddSymbol;         /* Pointer to unique Symbol for 'odd' */
mextern(PSymbol) gOffSymbol;         /* Pointer to unique Symbol for 'off' */
mextern(PSymbol) gOnSymbol;          /* Pointer to unique Symbol for 'on' */
mextern(PSymbol) gPrinterSymbol;     /* Pointer to unique Symbol for 'printer' */

#endif /* not SERIALX_H_ */
