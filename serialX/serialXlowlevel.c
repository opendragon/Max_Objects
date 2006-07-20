/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       serialXlowlevel.c                                                       */
/*                                                                                      */
/*  Contains:   The low-level support routines for the serialX module.                  */
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

#include "serialX.h"

/*------------------------------------ serialXCheckLongBaudRate ---*/
long serialXCheckLongBaudRate
  (const long	baudRate)
{
  long result;

  TRACE_POST_1("entering serialXCheckLongBaudRate")
#if defined(COMPILE_FOR_CATS)
  switch (baudRate)
  {
    case 150:
      result = B150;
      break;

    case 300:
      result = B300;
      break;

    case 600:
      result = B600;
      break;

    case 1200:
      result = B1200;
      break;

    case 1800:
      result = B1800;
      break;

    case 2400:
      result = B2400;
      break;

    case 4800:
      result = B4800;
      break;

    case 7200:
      result = B7200;
      break;

    case 9600:
      result = B9600;
      break;

    case 14400:
      result = B14400;
      break;

    case 19200:
      result = B19200;
      break;

    case 28800:
      result = B28800;
      break;

    case 38400:
      result = B38400;
      break;

    case 57600:
      result = B57600;
      break;

    case 115200:
      result = B115200;
      break;

    case 230400:
      result = B230400;
      break;

    default:
      result = -1;
      break;

  }
#else /* not COMPILE_FOR_CATS */
  switch (baudRate)
  {
    case 150:
      result = baud150;
      break;

    case 300:
      result = baud300;
      break;

    case 600:
      result = baud600;
      break;

    case 1200:
      result = baud1200;
      break;

    case 1800:
      result = baud1800;
      break;

    case 2400:
      result = baud2400;
      break;

    case 3600:
      result = baud3600;
      break;

    case 4800:
      result = baud4800;
      break;

    case 7200:
      result = baud7200;
      break;

    case 9600:
      result = baud9600;
      break;

    case 14400:
      result = baud14400;
      break;

    case 19200:
      result = baud19200;
      break;

    case 28800:
      result = baud28800;
      break;

    case 38400:
      result = baud38400;
      break;

    case 57600:
      result = baud57600;
      break;

    case 115200:
      result = baud115K;
      break;

    case 230400:
      result = baud230K;
      break;

    default:
      result = -1;
      break;

  }
#endif /* not COMPILE_FOR_CATS */
  TRACE_POST_2("exiting serialXCheckLongBaudRate %ld", result)
  return result;
} /* serialXCheckLongBaudRate */

/*------------------------------------ serialXCheckSymbolBaudRate ---*/
long serialXCheckSymbolBaudRate
  (PSymbol	baudSymbol,
   long &		baudRate)
{
  long result;

  TRACE_POST_1("entering serialXCheckSymbolBaudRate")
#if defined(COMPILE_FOR_CATS)
  if ((baudSymbol == gBaud_3KSymbol) || (baudSymbol == gBaud0_3KSymbol))
  {
    baudRate = 300;
    result = B300;
  }
  else if ((baudSymbol == gBaud_6KSymbol) || (baudSymbol == gBaud0_6KSymbol))
  {
   	baudRate = 600;
    result = B600;
  }
  else if (baudSymbol == gBaud1_2KSymbol)
  {
    baudRate = 1200;
    result = B1200;
  }
  else if (baudSymbol == gBaud1_8KSymbol)
  {
    baudRate = 1800;
    result = B1800;
  }
  else if (baudSymbol == gBaud2_4KSymbol)
  {
    baudRate = 2400;
    result = B2400;
  }
  else if (baudSymbol == gBaud4_8KSymbol)
  {
    baudRate = 4800;
    result = B4800;
  }
  else if (baudSymbol == gBaud7_2KSymbol)
  {
    baudRate = 7200;
    result = B7200;
  }
  else if (baudSymbol == gBaud9_6KSymbol)
  {
    baudRate = 9600;
    result = B9600;
  }
  else if (baudSymbol == gBaud14_4KSymbol)
  {
    baudRate = 14400;
    result = B14400;
  }
  else if (baudSymbol == gBaud19_2KSymbol)
  {
    baudRate = 19200;
    result = B19200;
  }
  else if (baudSymbol == gBaud28_8KSymbol)
  {
    baudRate = 28800;
    result = B28800;
  }
  else if (baudSymbol == gBaud38_4KSymbol)
  {
    baudRate = 38400;
    result = B38400;
  }
  else if (baudSymbol == gBaud57_6KSymbol)
  {
    baudRate = 57600;
    result = B57600;
  }
  else if (baudSymbol == gBaud115_2KSymbol)
  {
    baudRate = 115200;
    result = B115200;
  }
  else if (baudSymbol == gBaud230_4KSymbol)
  {
    baudRate = 230400;
    result = B230400;
  }
  else
  {
    baudRate = 0;
    result = -1;
  }
#else /* COMPILE_FOR_CATS */
  if ((baudSymbol == gBaud_3KSymbol) || (baudSymbol == gBaud0_3KSymbol))
  {
    baudRate = 300;
    result = baud300;
  }
  else if ((baudSymbol == gBaud_6KSymbol) || (baudSymbol == gBaud0_6KSymbol))
  {
    baudRate = 600;
    result = baud600;
  }
  else if (baudSymbol == gBaud1_2KSymbol)
  {
    baudRate = 1200;
    result = baud1200;
  }
  else if (baudSymbol == gBaud1_8KSymbol)
  {
    baudRate = 1800;
    result = baud1800;
  }
  else if (baudSymbol == gBaud2_4KSymbol)
  {
    baudRate = 2400;
    result = baud2400;
  }
  else if (baudSymbol == gBaud3_6KSymbol)
  {
    baudRate = 3600;
    result = baud3600;
  }
  else if (baudSymbol == gBaud4_8KSymbol)
  {
    baudRate = 4800;
    result = baud4800;
  }
  else if (baudSymbol == gBaud7_2KSymbol)
  {
    baudRate = 7200;
    result = baud7200;
  }
  else if (baudSymbol == gBaud9_6KSymbol)
  {
    baudRate = 9600;
    result = baud9600;
  }
  else if (baudSymbol == gBaud14_4KSymbol)
  {
    baudRate = 14400;
    result = baud14400;
  }
  else if (baudSymbol == gBaud19_2KSymbol)
  {
    baudRate = 19200;
    result = baud19200;
  }
  else if (baudSymbol == gBaud28_8KSymbol)
  {
    baudRate = 28800;
    result = baud28800;
  }
  else if (baudSymbol == gBaud38_4KSymbol)
  {
    baudRate = 38400;
    result = baud38400;
  }
  else if (baudSymbol == gBaud57_6KSymbol)
  {
    baudRate = 57600;
    result = baud57600;
  }
  else if (baudSymbol == gBaud115_2KSymbol)
  {
    baudRate = 115200;
    result = baud115K;
  }
  else if (baudSymbol == gBaud230_4KSymbol)
  {
    baudRate = 230400;
    result = baud230K;
  }
  else if (baudSymbol == gBaudMIDI_1Symbol)
  {
    /* Store the MIDI multiplier in the baud rate */
    baudRate = 0x0000;
    result = -2;
  }
  else if (baudSymbol == gBaudMIDI_16Symbol)
  {
    /* Store the MIDI multiplier in the baud rate */
    baudRate = 0x0040;
    result = -2;
  }
  else if (baudSymbol == gBaudMIDI_32Symbol)
  {
    /* Store the MIDI multiplier in the baud rate */
    baudRate = 0x0080;
    result = -2;
  }
  else if (baudSymbol == gBaudMIDI_64Symbol)
  {
    /* Store the MIDI multiplier in the baud rate */
    baudRate = 0x00C0;
    result = -2;
  }
  else
  {
    baudRate = 0;
    result = -1;
  }
#endif /* not COMPILE_FOR_CATS */
  TRACE_POST_2("exiting serialXCheckSymbolBaudRate %ld", result)
  return result;
} /* serialXCheckSymbolBaudRate */

/*------------------------------------ serialXForceDTRState ---*/
OSErr serialXForceDTRState
  (SerialXControlPtr  xx,
   const bool					ifAsserted)
{
#if defined(COMPILE_FOR_CATS)
  if (ioctl(xx->fFileDescriptor, ifAsserted ? TIOCSDTR : TIOCCDTR) == -1)
    return (noErr + 1);
    
  return noErr;
#else /* not COMPILE_FOR_CATS */
  return Control(xx->fOutRefNum, ifAsserted ? kSERDAssertDTR : kSERDNegateDTR, NULL_PTR);
#endif /* not COMPILE_FOR_CATS */
} /* serialXForceDTRState */

/*------------------------------------ serialXSetDTRHandshake ---*/
OSErr serialXSetDTRHandshake
  (SerialXControlPtr xx)
{
#if defined(COMPILE_FOR_CATS)
  return noErr;
#else /* not COMPILE_FOR_CATS */
  SerShk ss; /* serial handshake record */

  ss.fXOn = 0;
  ss.fCTS = 0;
  /* ss.xOn  = */
  /* ss.xOff = */
  ss.errs = 0;
  ss.evts = 0;
  ss.fInX = 0;
  ss.fDTR = static_cast<uchar>(xx->fDTRHandshakeActive ? 1 : 0);
  return Control(xx->fOutRefNum, kSERDHandshake, &ss);
#endif /* not COMPILE_FOR_CATS */
} /* serialXSetDTRHandshake */

/*------------------------------------ serialXSetPortCharacteristics ---*/
bool serialXSetPortCharacteristics
  (SerialXControlPtr xx)
{
  OSErr           err = noErr;
#if defined(COMPILE_FOR_CATS)
  struct termios  options;
#else /* not COMPILE_FOR_CATS */
  long            effectiveBaudBits = xx->fBaudRateBits;
#endif /* not COMPILE_FOR_CATS */

  TRACE_POST_1("entering serialXSetPortCharacteristics")
#if defined(COMPILE_FOR_CATS)
  if (tcgetattr(xx->fFileDescriptor, &xx->fOriginalAttrs) == -1)
    err = noErr + 1;
  else
  {
    options = xx->fOriginalAttrs;
    cfmakeraw(&options);
    cfsetspeed(&options, xx->fBaudRateBits);
    options.c_cflag &= ~(CSIZE | PARENB | CSTOPB | PARODD);
    options.c_cflag |= (xx->fDataBits | xx->fStopBits | xx->fParityState);
    if (tcsetattr(xx->fFileDescriptor, TCSANOW, &options) == -1)
      err = noErr + 1;
    else
      err = serialXSetDTRHandshake(xx);
  }
  TRACE_POST_2("exiting serialXSetPortCharacteristics %s",
                (err == noErr) ? "true" : "false")
  return (err == noErr);
#else /* not COMPILE_FOR_CATS */
  if (effectiveBaudBits == -2)
  {
    /* MIDI rate */
    xx->fDataBits = data8;
    xx->fStopBits = stop10;
    err = SerReset(xx->fOutRefNum, short(baud57600 | data8 | stop10 | xx->fParityState));
    if (err == noErr)
    {
      char csParam = char(xx->fBaudRate);

      /* We've stored the MIDI multiplier in the baud rate */ 
      err = Control(xx->fOutRefNum, kSERDClockMIDI, &csParam);
    }
  }
  else
  {
    if ((effectiveBaudBits == baud115K) || (effectiveBaudBits == baud230K))
      effectiveBaudBits = baud57600;
    err = SerReset(xx->fOutRefNum, short(effectiveBaudBits | xx->fDataBits |
                                          xx->fStopBits | xx->fParityState));
    if (err == noErr)
    {
      if (xx->fBaudRateBits == baud115K)
        err = Control(xx->fOutRefNum, kSERD115KBaud, NULL_PTR);
      else if (xx->fBaudRateBits == baud230K)
        err = Control(xx->fOutRefNum, kSERD230KBaud, NULL_PTR);
    }
  }
  if (err == noErr)
    err = serialXSetDTRHandshake(xx);
  TRACE_POST_2("exiting serialXSetPortCharacteristics %s",
                (err == noErr) ? "true" : "false")
  return (err == noErr);
#endif /* not COMPILE_FOR_CATS */
} /* serialXSetPortCharacteristics */

/*------------------------------------ serialXWriteACharacter ---*/
OSErr serialXWriteACharacter
  (SerialXControlPtr	xx,
   const long					msg)
{
  OSErr         err;
  char          sendChar = char(msg);
#if defined(COMPILE_FOR_CATS)
  int           numBytes;

  TRACE_POST_1("entering serialXWriteACharacter")
  numBytes = write(xx->fFileDescriptor, &sendChar, 1);
  if (numBytes == -1)
    LOG_ERROR_1(OUTPUT_PREFIX "write error on serial port")
  err = static_cast<OSErr>((numBytes == -1) ? (noErr + 1) : noErr);
#else /* not COMPILE_FOR_CATS */
  ParamBlockRec sendParamBlock;

  TRACE_POST_1("entering serialXWriteACharacter")
  sendParamBlock.ioParam.ioRefNum = xx->fOutRefNum;
  sendParamBlock.ioParam.ioBuffer = &sendChar;
  sendParamBlock.ioParam.ioReqCount = 1;
  sendParamBlock.ioParam.ioCompletion = NULL_PTR;
  sendParamBlock.ioParam.ioVRefNum = 0;
  sendParamBlock.ioParam.ioPosMode = 0;
  err = PBWrite(&sendParamBlock, false);
  if (err != noErr)
    LOG_ERROR_1(OUTPUT_PREFIX "write error on serial port")
#endif /* not COMPILE_FOR_CATS */
  TRACE_POST_2("exiting serialXWriteACharacter %d", err)
  return err;
} /* serialXWriteACharacter */


/*------------------------------------ serialXWriteASymbol ---*/
OSErr serialXWriteASymbol
  (SerialXControlPtr xx,
   Ptr               msg)
{
  OSErr         err;
#if defined(COMPILE_FOR_CATS)
  int           numBytes;

  TRACE_POST_1("entering serialXWriteASymbol")
  numBytes = write(xx->fFileDescriptor, msg, strlen(msg));
  if (numBytes == -1)
    LOG_ERROR_1(OUTPUT_PREFIX "write error on serial port")
  err = static_cast<OSErr>((numBytes == -1) ? (noErr + 1) : noErr);
#else /* not COMPILE_FOR_CATS */
  ParamBlockRec sendParamBlock;

  sendParamBlock.ioParam.ioRefNum = xx->fOutRefNum;
  sendParamBlock.ioParam.ioBuffer = msg;
  sendParamBlock.ioParam.ioReqCount = long(strlen(msg));
  sendParamBlock.ioParam.ioCompletion = NULL_PTR;
  sendParamBlock.ioParam.ioVRefNum = 0;
  sendParamBlock.ioParam.ioPosMode = 0;
  err = PBWrite(&sendParamBlock, false);
  if (err != noErr)
    LOG_ERROR_1(OUTPUT_PREFIX "write error on serial port")
#endif /* not COMPILE_FOR_CATS */
  TRACE_POST_2("exiting serialXWriteASymbol %d", err)
  return err;
} /* serialXWriteASymbol */
