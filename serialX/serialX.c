/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       serialX.c                                                               */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the serialX module.       */
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

#define IS_MAIN /* */
#include "serialX.h"
#include "displayAboutBox.h"
#include "loadAboutBoxData.h"
#include "loadOtherSegments.h"
#include "reportAnything.h"
#include "reportVersion.h"

/* Forward references: */
Pvoid serialXClearBreak
  (SerialXControlPtr xx);

Pvoid serialXCreate
  (PSymbol ss,
   short   argc,
   PAtom   argv);

Pvoid serialXFree
  (SerialXControlPtr xx);

void serialXInitRoutineDescriptors
  (void);

Pvoid serialXResetBreak
  (SerialXControlPtr xx);

/*------------------------------------ main ---*/
void main
  (Pfptr ff)
{
  EnterCodeResource();
  PrepareCallback();
  FNS = ff;   /* Connect up the function macros. */
  /* Allocate class memory and set up class. */
  setup(reinterpret_cast<t_messlist**>(&gClass), reinterpret_cast<method>(serialXCreate),
        reinterpret_cast<method>(serialXFree), short(sizeof(SerialXControl)),
        reinterpret_cast<method>(0L), A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Anything), "anything", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Assist), "assist", A_CANT, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Baud), "baud", A_GIMME, A_NOTHING);
  addbang(reinterpret_cast<method>(cmd_Bang));
  addmess(reinterpret_cast<method>(cmd_Break), "break", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Chunk), "chunk", A_DEFSYM, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_DTR), "dtr", A_DEFSYM, A_NOTHING);
  addfloat(reinterpret_cast<method>(cmd_Float));
  addmess(reinterpret_cast<method>(cmd_Info), "info", A_CANT, A_NOTHING);
  addint(reinterpret_cast<method>(cmd_Int));
  addmess(reinterpret_cast<method>(cmd_List), "list", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Text), "text", A_GIMME, A_NOTHING);
  /* Add object class to New Object List */
  finder_addclass("Devices", OUR_NAME);
  /* Make sure that our string list resource is present */
  rescopy('STR#', OUR_RES_NUMB);
  loadAboutBoxData();
  gAssertSymbol = gensym("assert");
  gBaud_3KSymbol = gensym(".3k");
  gBaud_6KSymbol = gensym(".6k");
  gBaud0_3KSymbol = gensym("0.3k");
  gBaud0_6KSymbol = gensym("0.6k");
  gBaud1_2KSymbol = gensym("1.2k");
  gBaud1_8KSymbol = gensym("1.8k");
  gBaud2_4KSymbol = gensym("2.4k");
  gBaud3_6KSymbol = gensym("3.6k");
  gBaud4_8KSymbol = gensym("4.8k");
  gBaud7_2KSymbol = gensym("7.2k");
  gBaud9_6KSymbol = gensym("9.6k");
  gBaud14_4KSymbol = gensym("14.4k");
  gBaud19_2KSymbol = gensym("19.2k");
  gBaud28_8KSymbol = gensym("28.8k");
  gBaud38_4KSymbol = gensym("38.4k");
  gBaud57_6KSymbol = gensym("57.6k");
  gBaud115_2KSymbol = gensym("115.2k");
  gBaud230_4KSymbol = gensym("230.4k");
  gBaudMIDI_1Symbol = gensym("MIDI_1");
  gBaudMIDI_16Symbol = gensym("MIDI_16");
  gBaudMIDI_32Symbol = gensym("MIDI_32");
  gBaudMIDI_64Symbol = gensym("MIDI_64");
  gEmptySymbol = gensym("");
  gOffSymbol = gensym("off");
  gOnSymbol = gensym("on");
  gNegateSymbol = gensym("negate");
  gNoSymbol = gensym("no");
  gNoneSymbol = gensym("none");
  gEvenSymbol = gensym("even");
  gOddSymbol = gensym("odd");
  gPrinterSymbol = gensym("printer");
  gModemSymbol = gensym("modem");
  serialXInitRoutineDescriptors();
  gVersionNumber = reportVersion(OUR_NAME);
  loadOtherSegments();
  ExitCodeResource();
} /* main */

#if defined(COMPILE_FOR_OS9_4)
/*------------------------------------ CloseSerialDrivers ---*/
static OSErr CloseSerialDrivers
  (const SInt16	inRefNum,
   const SInt16	outRefNum)
{
  OSErr err = noErr;

  TRACE_POST_1("entering CloseSerialDrivers")
  if (inRefNum)
  {
    KillIO(inRefNum);
    SerSetBuf(inRefNum, NULL_PTR, 0);
    err = CloseDriver(inRefNum);
  }
  if (err == noErr)
  {
    if (outRefNum)
    {
      KillIO(outRefNum);
      err = CloseDriver(outRefNum);
    }
  }
  TRACE_POST_2("exiting CloseSerialDrivers %d", err)
  return err;
} /* CloseSerialDrivers */
#endif /* COMPILE_FOR_OS9_4 */

#if defined(COMPILE_FOR_OS9_4)
/*------------------------------------ CommunicationsResourceManagerExists ---*/
static bool CommunicationsResourceManagerExists
  (void)
{
  long response;

  return ((Gestalt(gestaltCRMAttr, &response) == noErr) &&
            ((response & (1 << gestaltCRMPresent) != 0)));
} /* CommunicationsResourceManagerExists */
#endif /* COMPILE_FOR_OS9_4 */

#if defined(COMPILE_FOR_OS9_4)
/*------------------------------------ DriverIsOpen ---*/
static bool DriverIsOpen
  (ConstStr255Param driverName)
{
  Boolean    found = false, isOpen = false;
  short      unit = 0;
  DCtlHandle dceHandle;
  StringPtr  namePtr;

  TRACE_POST_1("entering DriverIsOpen")
  for ( ; (! found) && (unit < LMGetUnitTableEntryCount()); )
  {
    dceHandle = GetDCtlEntry(short(~unit));
    if (dceHandle && (*dceHandle)->dCtlDriver)
    {
      if (((*dceHandle)->dCtlFlags & dRAMBasedMask))
        namePtr =
              &(**(reinterpret_cast<DRVRHeaderHandle>((*dceHandle)->dCtlDriver))).drvrName[0];
      else
        namePtr = &(*(reinterpret_cast<DRVRHeaderPtr>((*dceHandle)->dCtlDriver))).drvrName[0];
      if (EqualString(driverName, namePtr, false, true))
      {
        found = true;
        isOpen = (((*dceHandle)->dCtlFlags & dOpenedMask) != 0);
      }
    }
    ++unit;
  }
  TRACE_POST_2("exiting DriverIsOpen %s", isOpen ? "true" : "false")
  return isOpen;
} /* DriverIsOpen */
#endif /* COMPILE_FOR_OS9_4 */

#if defined(COMPILE_FOR_OS9_4)
/*------------------------------------ SerialArbitrationExists ---*/
static bool SerialArbitrationExists
  (void)
{
  long response;

  return ((Gestalt(gestaltArbitorAttr, &response) == noErr) &&
            ((response & (1 << gestaltSerialArbitrationExists) != 0)));
} /* SerialArbitrationExists */
#endif /* COMPILE_FOR_OS9_4 */

#if defined(COMPILE_FOR_OS9_4)
/*------------------------------------ OpenOneSerialDriver ---*/
static OSErr OpenOneSerialDriver
  (ConstStr255Param driverName,
   SInt16 &         refNum)
{
  OSErr err = noErr;

  TRACE_POST_1("entering OpenOneSerialDriver")
  TRACE_POST_3("name is: {%d}%s", int(*driverName), driverName + 1)
  if (SerialArbitrationExists())
    err = OpenDriver(driverName, &refNum);
  else if (DriverIsOpen(driverName))
    err = portInUse;
  else
    err = OpenDriver(driverName, &refNum);
  TRACE_POST_2("exiting OpenOneSerialDriver %d", err)
  return err;
} /* OpenOneSerialDriver */
#endif /* COMPILE_FOR_OS9_4 */

#if defined(COMPILE_FOR_OS9_4)
/*------------------------------------ OpenSerialDrivers ---*/
static OSErr OpenSerialDrivers
  (ConstStr255Param inName,
   ConstStr255Param outName,
   SInt16 &         inRefNum,
   SInt16 &         outRefNum)
{
  OSErr err = OpenOneSerialDriver(outName, outRefNum);

  TRACE_POST_1("entering OpenSerialDrivers")
  if (err == noErr)
  {
    err = OpenOneSerialDriver(inName, inRefNum);
    if (err != noErr)
      CloseDriver(outRefNum);
  }
  if (err != noErr)
    inRefNum = outRefNum = 0;
  TRACE_POST_2("exiting OpenSerialDrivers %d", err)
  return err;
} /* OpenSerialDrivers */
#endif /* COMPILE_FOR_OS9_4 */

/*------------------------------------ AttemptToOpenSerialPorts ---*/
static bool AttemptToOpenSerialPorts
  (SerialXControlPtr xx)
{
  bool okSoFar = true;

  TRACE_POST_1("entering AttemptToOpenSerialPorts")
#if defined(COMPILE_FOR_OSX_4)
  xx->fFileDescriptor = open(xx->fPortName, O_RDWR | O_NOCTTY | O_NONBLOCK);
  if (xx->fFileDescriptor == -1)
    LOG_ERROR_1(OUTPUT_PREFIX "unable to open device")
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
  if (CommunicationsResourceManagerExists())
  {
    CRMRec       commRecord;
    CRMRecPtr    thisCommRecord;
    CRMSerialPtr serialPtr;
    uchar        lookForInName[] = "\p.?In";

    InitCRM();
    lookForInName[2] = static_cast<uchar>(xx->fPortCharacter);
    commRecord.crmDeviceType = crmSerialDevice;
    commRecord.crmDeviceID = 0;
    thisCommRecord = &commRecord;
    do
    {
      thisCommRecord =
                static_cast<CRMRecPtr>(CRMSearch(static_cast<CRMRecPtr>(thisCommRecord)));
      if (thisCommRecord)
      {
        /* Look for a matching driver name: */
        serialPtr = reinterpret_cast<CRMSerialPtr>(thisCommRecord->crmAttributes);
        TRACE_POST_2("driver version: %d", int(serialPtr->version))
        TRACE_POST_3("name is: {%d}%s", int(**(serialPtr->name)), *(serialPtr->name) + 1)
        TRACE_POST_3("driver input name is: {%d}%s", int(**(serialPtr->inputDriverName)),
                      *(serialPtr->inputDriverName) + 1)
        TRACE_POST_3("driver output name is: {%d}%s", int(**(serialPtr->outputDriverName)),
                      *(serialPtr->outputDriverName) + 1)
        if (EqualString(*(serialPtr->inputDriverName), lookForInName, false, true))
        {
          TRACE_POST_1("candidate selected")
          if (xx->fBaudRate > serialPtr->maxSpeed)
            LOG_ERROR_1(OUTPUT_PREFIX "baud rate too high for this device")
          else
          {
            if (OpenSerialDrivers(*(serialPtr->inputDriverName),
                                  *(serialPtr->outputDriverName), xx->fInRefNum,
                                  xx->fOutRefNum) != noErr)
              LOG_ERROR_1(OUTPUT_PREFIX "unable to open drivers to device")
            else
              okSoFar = true;
          }
          break;

        }
        commRecord.crmDeviceID = thisCommRecord->crmDeviceID;
      }
    }
    while (thisCommRecord);
    if (! thisCommRecord)
      LOG_ERROR_1(OUTPUT_PREFIX "no such device")
  }
  else
  {
    /* Synthesize the driver name */
    uchar inName[] = "\p.?In";
    uchar outName[] = "\p.?Out";

    inName[2] = outName[2] = static_cast<uchar>(xx->fPortCharacter);
    TRACE_POST_3("input name is: {%d}%s", int(*inName), inName + 1)
    TRACE_POST_3("output name is: {%d}%s", int(*outName), outName + 1)
    if (OpenSerialDrivers(inName, outName, xx->fInRefNum, xx->fOutRefNum) != noErr)
      LOG_ERROR_1(OUTPUT_PREFIX "unable to open drivers to device")
    else
      okSoFar = true;
  }
#endif /* COMPILE_FOR_OS9_4 */
  TRACE_POST_2("exiting AttemptToOpenSerialPorts %s", okSoFar ? "true" : "false")
  return okSoFar;
} /* AttemptToOpenSerialPorts */

#if defined(COMPILE_FOR_OS9_4)
/*------------------------------------ CheckPortName ---*/
static char CheckPortName
  (PSymbol portName)
{
  char candidate = 0;

  TRACE_POST_1("entering CheckPortName")
  if (portName == gModemSymbol)
    candidate = 'A';
  else if (portName == gPrinterSymbol)
    candidate = 'B';
  else
  {
    Qchar	walker = portName->s_name;

    if (walker)
    {
      candidate = *walker++;
      if (*walker)
        candidate = '\0';
      else
      {
        if (isalpha(candidate))
          candidate = char(toupper(candidate));
        else
          candidate = '\0';
      }
    }
  }
  TRACE_POST_2("exiting CheckPortName %c", candidate)
  return candidate;
} /* CheckPortname */
#endif /* COMPILE_FOR_OS9_4 */

/*------------------------------------ CheckDataBits ---*/
static long CheckDataBits
  (const long	dataBits)
{
  long result;

  TRACE_POST_1("entering CheckDataBits")
#if defined(COMPILE_FOR_OSX_4)
  switch (dataBits)
  {
    case 5:
      result = CS5;
      break;

    case 6:
      result = CS6;
      break;

    case 7:
      result = CS7;
      break;

    case 8:
      result = CS8;
      break;

    default:
      result = -1;
      break;

  }
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
  switch (dataBits)
  {
    case 5:
      result = data5;
      break;

    case 6:
      result = data6;
      break;

    case 7:
      result = data7;
      break;

    case 8:
      result = data8;
      break;

    default:
      result = -1;
      break;

  }
#endif /* COMPILE_FOR_OS9_4 */
  TRACE_POST_2("exiting CheckDataBits %ld", result)
  return result;
} /* CheckDataBits */

#define FLOAT_TOLERANCE 0.01
/*------------------------------------ CheckStopBits ---*/
static long CheckStopBits
  (const double	stopBits)
{
  long result = -1;

  TRACE_POST_1("entering CheckStopBits")
#if defined(COMPILE_FOR_OSX_4)
  if (stopBits >= 0)
  {
    if (fabs(stopBits - 1.0) < FLOAT_TOLERANCE)
      result = 0; // 1 stop bit
    else if (fabs(stopBits - 2.0) < FLOAT_TOLERANCE)
      result = CSTOPB; // 2 stop bits
  }
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
  if (stopBits >= 0)
  {
    if (fabs(stopBits - 1.0) < FLOAT_TOLERANCE)
      result = stop10;
    else if (fabs(stopBits - 2.0) < FLOAT_TOLERANCE)
      result = stop20;
    else if (fabs(stopBits - 1.5) < FLOAT_TOLERANCE)
      result = stop15;
  }
#endif /* COMPILE_FOR_OS9_4 */
  TRACE_POST_2("exiting CheckStopBits %ld", result)
  return result;
} /* CheckStopBits */

/*------------------------------------ CheckParityState ---*/
static long CheckParityState
  (PSymbol parityState)
{
  long result = -1;

  TRACE_POST_1("entering CheckParityState")
#if defined(COMPILE_FOR_OSX_4)
  if ((parityState == gOffSymbol) || (parityState == gNoSymbol) ||
      (parityState == gNoneSymbol))
    result = 0;
  else if (parityState == gEvenSymbol)
    result = PARENB;
  else if (parityState == gOddSymbol)
    result = (PARENB | PARODD);
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
  if ((parityState == gOffSymbol) || (parityState == gNoSymbol) ||
      (parityState == gNoneSymbol))
    result = noParity;
  else if (parityState == gEvenSymbol)
    result = evenParity;
  else if (parityState == gOddSymbol)
    result = oddParity;
#endif /* COMPILE_FOR_OS9_4 */
  TRACE_POST_2("exiting CheckParityState %ld", result)
  return result;
} /* CheckParityState */

/*------------------------------------ SetInputBuffering ---*/
static bool SetInputBuffering
  (SerialXControlPtr xx)
{
  bool   okSoFar = false;
  Handle aBuff = NewHandle(INBUFFER_SIZE);

  TRACE_POST_1("entering SetInputBuffering")
  if (aBuff)
  {
    HLock(aBuff);
    xx->fInBuffer = aBuff;
    memset(*aBuff, 0, INBUFFER_SIZE);
#if defined(COMPILE_FOR_OSX_4)
//TBD
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
    SerSetBuf(xx->fInRefNum, *aBuff, INBUFFER_SIZE);
#endif /* COMPILE_FOR_OS9_4 */
    okSoFar = true;
  }
  TRACE_POST_2("exiting SetInputBuffering %s", okSoFar ? "true" : "false")
  return okSoFar;
} /* SetInputBuffering */

/*------------------------------------ serialXClearBreak ---*/
Pvoid serialXClearBreak
  (SerialXControlPtr xx)
{
  EnterCallback();
  TRACE_POST_1("entering serialXClearBreak")
  if (xx)
  {
    long delayTime;

#if defined(COMPILE_FOR_OSX_4)
    ioctl(xx->fFileDescriptor, TIOCCBRK);
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
    SerClrBrk(xx->fOutRefNum);
#endif /* COMPILE_FOR_OS9_4 */
    delayTime = (2000L + xx->fBaudRate) / xx->fBaudRate;
    clock_delay(xx->fClearClock, delayTime);
  }
  TRACE_POST_1("exiting serialXClearBreak")
  ExitMaxMessageHandler()
} /* serialXClearBreak */

/*------------------------------------ serialXCreate ---*/
Pvoid serialXCreate
  (PSymbol ss,
   short   argc,
   PAtom   argv)
{
#pragma unused(ss)
  SerialXControlPtr xx;

  EnterCallback();
  LOG_ENTER()
  xx = static_cast<SerialXControlPtr>(newobject(gClass));
  if (xx)
  {
    bool skipCheck = false;

 #if defined(COMPILE_FOR_OSX_4)
    xx->fFileDescriptor = -1;
    xx->fPortName = NULL_PTR;
    xx->fPortNameLength = 0;
    xx->fBaudRate = B4800;
    xx->fBaudRateBits = B4800;
    xx->fDataBits = CS8;
    xx->fStopBits = 0;
    xx->fParityState = 0;
 #endif /* COMPILE_FOR_OSX_4 */
 #if defined(COMPILE_FOR_OS9_4)
    xx->fInRefNum = xx->fOutRefNum = 0;
    xx->fPortCharacter = 'A';
    xx->fBaudRate = 4800;
    xx->fBaudRateBits = baud4800;
    xx->fDataBits = data8;
    xx->fStopBits = stop10;
    xx->fParityState = noParity;
 #endif /* COMPILE_FOR_OS9_4 */
    xx->fBreakClock = xx->fClearClock = NULL_PTR;
    xx->fInBuffer = NULL_HDL;
    xx->fChunked = false;
    xx->fDTRHandshakeActive = true;
    xx->fLocalBuffer = NULL_PTR;
    xx->fChunkSize = DEFAULT_CHUNK;
    xx->fChunkData = NULL_PTR;
    if (! argc)
      LOG_ERROR_1(OUTPUT_PREFIX "missing port name")
    else if (argv[0].a_type != A_SYM)
      LOG_ERROR_1(OUTPUT_PREFIX "port name is not a symbol")
    else
    {
#if defined(COMPILE_FOR_OSX_4)
      Pchar aName = argv[0].a_w.w_sym->s_name;
      
      xx->fPortNameLength = strlen(aName);
      xx->fPortName = GETBYTES(xx->fPortNameLength + 1, char);
      strcpy(xx->fPortName, aName);
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
      xx->fPortCharacter = CheckPortName(argv[0].a_w.w_sym);
      if (! xx->fPortCharacter)
      {
        LOG_ERROR_2(OUTPUT_PREFIX "bad port name (%s) specified",
                    argv[0].a_w.w_sym->s_name)
        xx->fPortCharacter = 'A';
      }
#endif /* COMPILE_FOR_OS9_4 */
    }
    if (argc > 1)
    {
      /* Check baud rate */
      if (argv[1].a_type == A_LONG)
        xx->fBaudRate = argv[1].a_w.w_long;
      else if (argv[1].a_type == A_FLOAT)
        xx->fBaudRate = long(argv[1].a_w.w_float);
      else if (argv[1].a_type == A_SYM)
      {
        xx->fBaudRateBits = serialXCheckSymbolBaudRate(argv[1].a_w.w_sym, xx->fBaudRate);
        skipCheck = true;
      }
      else
        LOG_ERROR_1(OUTPUT_PREFIX "baud rate is not numeric")
      if (! skipCheck)
        xx->fBaudRateBits = serialXCheckLongBaudRate(xx->fBaudRate);
#if defined(COMPILE_FOR_OSX_4)
      if (xx->fBaudRateBits == -1)
      {
        LOG_ERROR_2(OUTPUT_PREFIX "bad baud rate (%ld) specified", xx->fBaudRate)
        xx->fBaudRateBits = B4800;
      }
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
      if (xx->fBaudRateBits == -1)
      {
        LOG_ERROR_2(OUTPUT_PREFIX "bad baud rate (%ld) specified", xx->fBaudRate)
        xx->fBaudRateBits = baud4800;
      }
      else if ((xx->fBaudRateBits == baud115K) || (xx->fBaudRateBits == baud230K))
        xx->fBaudRate = baud57600;
#endif /* COMPILE_FOR_OS9_4 */
    }
    if (argc > 2)
    {
      long dataBits = 8;

      /* Check data bits */
      if (argv[2].a_type == A_LONG)
        dataBits = argv[2].a_w.w_long;
      else if (argv[2].a_type == A_FLOAT)
        dataBits = long(argv[2].a_w.w_float);
      else
        LOG_ERROR_1(OUTPUT_PREFIX "data bits is not numeric")
      xx->fDataBits = CheckDataBits(dataBits);
      if (xx->fDataBits == -1)
      {
        LOG_ERROR_2(OUTPUT_PREFIX "bad data bits (%ld) specified", dataBits)
#if defined(COMPILE_FOR_OSX_4)
        xx->fDataBits = CS8;
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
        xx->fDataBits = data8;
#endif /* COMPILE_FOR_OS9_4 */
      }
    }
    if (argc > 3)
    {
      /* Check stop bits */
      double stopBits = 1.0;

      /* Check data bits */
      if (argv[3].a_type == A_LONG)
        stopBits = double(argv[3].a_w.w_long);
      else if (argv[3].a_type == A_FLOAT)
        stopBits = argv[3].a_w.w_float;
      else
        LOG_ERROR_1(OUTPUT_PREFIX "stop bits is not numeric")
      xx->fStopBits = CheckStopBits(stopBits);
      if (xx->fStopBits == -1)
      {
        LOG_ERROR_2(OUTPUT_PREFIX "bad stop bits (%g) specified", stopBits)
#if defined(COMPILE_FOR_OSX_4)
        xx->fStopBits = 0;
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
        xx->fStopBits = stop10;
#endif /* COMPILE_FOR_OS9_4 */
      }
    }
    if (argc > 4)
    {
      if (argv[4].a_type != A_SYM)
        LOG_ERROR_1(OUTPUT_PREFIX "parity is not a symbol")
      else
      {
        /* Check parity */
        xx->fParityState = CheckParityState(argv[4].a_w.w_sym);
        if (xx->fParityState == -1)
        {
          LOG_ERROR_2(OUTPUT_PREFIX "bad parity (%s) specified", argv[4].a_w.w_sym->s_name)
#if defined(COMPILE_FOR_OSX_4)
          xx->fParityState = 0;
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
          xx->fParityState = noParity;
#endif /* COMPILE_FOR_OS9_4 */
        }
      }
    }
    if (argc > 5)
    {
      if (argv[5].a_type != A_LONG)
        LOG_ERROR_1(OUTPUT_PREFIX "chunk size is not an integer")
      else
      {
        long temp = argv[5].a_w.w_long;

        if ((temp <= 0) || (temp > MAX_CHUNK_SIZE))
        {
          LOG_ERROR_2(OUTPUT_PREFIX "bad chunk size (%ld) specified", temp)
          xx->fChunkSize = DEFAULT_CHUNK;
        }
        else
          xx->fChunkSize = short(temp);
      }
    }
    if (argc > 6)
      LOG_ERROR_1(OUTPUT_PREFIX "too many parameters for device")
    /* Set up our connections and private data */
    xx->fBreakDone = static_cast<POutlet>(bangout(xx));
    xx->fBreakSeen = static_cast<POutlet>(bangout(xx));
    xx->fCharOut = static_cast<POutlet>(outlet_new(xx, NULL_PTR));
    xx->fBreakClock = static_cast<PClock>(clock_new(xx,
                                            reinterpret_cast<method>(serialXClearBreak)));
    xx->fClearClock = static_cast<PClock>(clock_new(xx,
                                            reinterpret_cast<method>(serialXResetBreak)));
    xx->fLocalBuffer = GETBYTES(LOCBUFFER_SIZE, char);
    xx->fChunkData = GETBYTES(xx->fChunkSize, Atom);
    if (xx->fCharOut && xx->fBreakClock && xx->fClearClock && xx->fLocalBuffer &&
        xx->fChunkData)
    {
      if (! (AttemptToOpenSerialPorts(xx) && serialXSetPortCharacteristics(xx) &&
            SetInputBuffering(xx)))
      {
        LOG_ERROR_1(OUTPUT_PREFIX "unable to open port for device")
        freeobject(reinterpret_cast<PObject>(xx));
        xx = NULL_PTR;
      }
    }
    else
    {
      LOG_ERROR_1(OUTPUT_PREFIX "unable to create port or clock for device")
      freeobject(reinterpret_cast<PObject>(xx));
      xx = NULL_PTR;
    }
  }
  ExitCodeResource();
  return xx;
} /* serialXCreate */

/*------------------------------------ serialXFree ---*/
Pvoid serialXFree
  (SerialXControlPtr xx)
{
  EnterCallback();
  if (xx)
  {
#if defined(COMPILE_FOR_OSX_4)
    if (xx->fFileDescriptor != -1)
    {
      tcdrain(xx->fFileDescriptor);
      tcsetattr(xx->fFileDescriptor, TCSANOW, &xx->fOriginalAttrs);
      close(xx->fFileDescriptor);
      xx->fFileDescriptor = -1;
    }
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
    if (xx->fInRefNum || xx->fOutRefNum)
    {
      CloseSerialDrivers(xx->fInRefNum, xx->fOutRefNum);
      xx->fInRefNum = xx->fOutRefNum = 0;
    }
#endif /* COMPILE_FOR_OS9_4 */
    if (xx->fInBuffer)
    {
      HUnlock(xx->fInBuffer);
      DisposeHandle(xx->fInBuffer);
      xx->fInBuffer = NULL_PTR;
    }
    FREEBYTES(xx->fLocalBuffer, LOCBUFFER_SIZE)
    FREEBYTES(xx->fChunkData, xx->fChunkSize)
    if (xx->fBreakClock)
    {
      clock_unset(xx->fBreakClock);
      freeobject(reinterpret_cast<PObject>(xx->fBreakClock));
      xx->fBreakClock = NULL_PTR;
    }
    if (xx->fClearClock)
    {
      clock_unset(xx->fClearClock);
      freeobject(reinterpret_cast<PObject>(xx->fClearClock));
      xx->fClearClock = NULL_PTR;
    }
#if defined(COMPILE_FOR_OSX_4)
    FREEBYTES(xx->fPortName, xx->fPortNameLength + 1);
#endif /* COMPILE_FOR_OSX_4 */
  }
  LOG_EXIT()
  ExitMaxMessageHandler()
} /* serialXFree */

/*------------------------------------ serialXInitRoutineDescriptors ---*/
void serialXInitRoutineDescriptors
  (void)
{
} /* serialXInitRoutineDescriptors */

/*------------------------------------ serialXResetBreak ---*/
Pvoid serialXResetBreak
  (SerialXControlPtr xx)
{
  EnterCallback();
  TRACE_POST_1("entering serialXResetBreak")
  if (xx)
    outlet_bang(xx->fBreakDone);
  TRACE_POST_1("exiting serialXResetBreak")
  ExitMaxMessageHandler()
} /* serialXResetBreak */

StandardAnythingRoutine(SerialXControlPtr)
StandardInfoRoutine(SerialXControlPtr)