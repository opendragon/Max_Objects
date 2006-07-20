/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       mtc.c                                                                   */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the mtc module.           */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 1999 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    1999/12/05                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#define IS_MAIN /* */
#include "mtc.h"
#include "displayAboutBox.h"
#include "loadAboutBoxData.h"
#include "loadOtherSegments.h"
#include "reportVersion.h"

/* Forward references: */
void mtcChangeState
  (MtcControlPtr	xx,
   const MtcState	newState);

Pvoid mtcCreate
  (PSymbol ss,
   short   argc,
   PAtom   argv);

Pvoid mtcFree
  (MtcControlPtr xx);

Pvoid mtcProcessClock
  (MtcControlPtr xx);

Pvoid mtcProcessQueue
  (MtcControlPtr xx);

/*------------------------------------ main ---*/
void main
  (Pfptr ff)
{
  EnterCodeResource();
  PrepareCallback();
  FNS = ff;   /* Connect up the function macros. */
  /* Allocate class memory and set up class. */
  setup(reinterpret_cast<t_messlist**>(&gClass), reinterpret_cast<method>(mtcCreate), 
        reinterpret_cast<method>(mtcFree), short(sizeof(MtcControl)),
        reinterpret_cast<method>(0L), A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Anything), "anything", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Assist), "assist", A_CANT, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Describe), "describe", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Info), "info", A_CANT, A_NOTHING);
  addint(reinterpret_cast<method>(cmd_Int));
  addmess(reinterpret_cast<method>(cmd_List), "list", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Mode), "mode", A_SYM, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Order), "order", A_SYM, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Ping), "ping", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Start), "start", A_DEFSYM, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Stop), "stop", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Threshold), "threshold", A_LONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Train), "train", A_DEFSYM, A_NOTHING);
#if defined(BE_VERBOSE)
  addmess(reinterpret_cast<method>(cmd_Verbose), "verbose", A_DEFSYM, A_NOTHING);
#endif /* BE_VERBOSE */
  /* Add object class to New Object List */
  finder_addclass("Devices", OUR_NAME);
  /* Make sure that our string list resource is present */
  rescopy('STR#', OUR_RES_NUMB);
  loadAboutBoxData();
  gCompressedSymbol = gensym("compressed");
  gCookedSymbol = gensym("cooked");
  gEmptySymbol = gensym("");
  gNormalSymbol = gensym("normal");
  gOffSymbol = gensym("off");
  gOnSymbol = gensym("on");
  gPSymbol = gensym("p");
  gRawSymbol = gensym("raw");
  gStartSymbol = gensym("start");
  gStopSymbol = gensym("stop");
  gXSymbol = gensym("x");
  gYSymbol = gensym("y");
  gVersionNumber = reportVersion(OUR_NAME);
  loadOtherSegments();
  ExitCodeResource();
} /* main */

/*------------------------------------ mtcChangeState ---*/
void mtcChangeState
  (MtcControlPtr	xx,
   const MtcState	newState)
{
#if defined(REPORT_STATE_CHANGES)
  Qchar				oldString;
  static char	numBuffer[30];
#endif /* REPORT_STATE_CHANGES */

  if (xx)
  {
#if defined(REPORT_STATE_CHANGES)
    if (xx->fVerbose)
    {
      switch (xx->fState)
      {
          case MTC_AWAITING_END_OF_COMMAND:
            oldString = "MTC_AWAITING_END_OF_COMMAND";
            break;

          case MTC_AWAITING_END_OF_COMP:
            oldString = "MTC_AWAITING_END_OF_COMP";
            break;

          case MTC_AWAITING_END_OF_DATA:
            oldString = "MTC_AWAITING_END_OF_DATA";
            break;

          case MTC_AWAITING_END_OF_DESC:
            oldString = "MTC_AWAITING_END_OF_DESC";
            break;

          case MTC_AWAITING_NUM_BYTES:
            oldString = "MTC_AWAITING_NUM_BYTES";
            break;

          case MTC_AWAITING_NUM_PACKETS:
            oldString = "MTC_AWAITING_NUM_PACKETS";
            break;

          case MTC_AWAITING_SAMPLE_NUMBER:
            oldString = "MTC_AWAITING_SAMPLE_NUMBER";
            break;

          case MTC_IDLE:
            oldString = "MTC_IDLE";
            break;

          case MTC_LOOKING_FOR_END:
            oldString = "MTC_LOOKING_FOR_END";
            break;

          case MTC_SKIP_TO_ACK:
            oldString = "MTC_SKIP_TO_ACK";
            break;

          case MTC_SKIP_TO_END:
            oldString = "MTC_SKIP_TO_END";
            break;

          default:
            sprintf(numBuffer, "unrecognized state %ld", xx->fState);
            oldString = numBuffer;
            break;

      }
    }
#endif /* REPORT_STATE_CHANGES */
    xx->fState = newState;
#if defined(REPORT_STATE_CHANGES)
    if (xx->fVerbose)
    {
      switch (xx->fState)
      {
        case MTC_AWAITING_END_OF_COMP:
          LOG_POST_2(OUTPUT_PREFIX "%s -> MTC_AWAITING_END_OF_COMP", oldString)
          break;

        case MTC_AWAITING_END_OF_DATA:
          LOG_POST_2(OUTPUT_PREFIX "%s -> MTC_AWAITING_END_OF_DATA", oldString)
          break;

        case MTC_AWAITING_END_OF_DESC:
          LOG_POST_2(OUTPUT_PREFIX "%s -> MTC_AWAITING_END_OF_DESC", oldString)
          break;

        case MTC_AWAITING_END_OF_COMMAND:
          LOG_POST_2(OUTPUT_PREFIX "%s -> MTC_AWAITING_END_OF_COMMAND", oldString)
          break;

        case MTC_AWAITING_NUM_BYTES:
          LOG_POST_2(OUTPUT_PREFIX "%s -> MTC_AWAITING_NUM_BYTES", oldString)
          break;

        case MTC_AWAITING_NUM_PACKETS:
          LOG_POST_2(OUTPUT_PREFIX "%s -> MTC_AWAITING_NUM_PACKETS", oldString)
          break;

        case MTC_AWAITING_SAMPLE_NUMBER:
          LOG_POST_2(OUTPUT_PREFIX "%s -> MTC_AWAITING_SAMPLE_NUMBER", oldString)
          break;

        case MTC_IDLE:
          LOG_POST_2(OUTPUT_PREFIX "%s -> MTC_IDLE", oldString)
          break;

        case MTC_LOOKING_FOR_END:
          LOG_POST_2(OUTPUT_PREFIX "%s -> MTC_LOOKING_FOR_END", oldString)
          break;

        case MTC_SKIP_TO_ACK:
          LOG_POST_2(OUTPUT_PREFIX "%s -> MTC_SKIP_TO_ACK", oldString)
          break;

        case MTC_SKIP_TO_END:
          LOG_POST_2(OUTPUT_PREFIX "%s -> MTC_SKIP_TO_END", oldString);
          break;

        default:
          LOG_POST_3(OUTPUT_PREFIX "%s -> unrecognized state (%ld)", oldString,
                      xx->fState)
          break;

      }
    }
#endif /* REPORT_STATE_CHANGES */
  }
} /* mtcChangeState */

/*------------------------------------ mtcCreate ---*/
Pvoid mtcCreate
  (PSymbol ss,
   short   argc,
   PAtom   argv)
{
#pragma unused(ss)
  MtcControlPtr xx;

  EnterCallback();
  LOG_ENTER()
  xx = static_cast<MtcControlPtr>(newobject(gClass));
  if (xx)
  {
    bool okSoFar = true;

    xx->fProxy = NULL_PTR;
    xx->fPollClock = NULL_PTR;
    xx->fPollQueue = NULL_PTR;
    xx->fMapFileName = NULL_PTR;
    xx->fNormalFileName = NULL_PTR;
    xx->fSpots = NULL_PTR;
    xx->fRawRow = NULL_PTR;
    xx->fDataRecovery = NULL_PTR;
    xx->fDataBuffer = NULL_PTR;
    xx->fSentinelTaxel = NULL_PTR;
    xx->fTaxelMapHandle = xx->fTaxelMatrix = NULL_HDL;
    xx->fChunkPulseSent = xx->fStarted = xx->fStopped = xx->fNormalizing = xx->fModeRaw =
        xx->fDataCompressed = xx->fIsPacketHeader = xx->fUseCompression =
        xx->fStopping = false;
    xx->fDescriptorLength = xx->fNumTaxels = xx->fExpectedPackets = 0;
    xx->fThreshold = MTC_DEFAULT_THRESHOLD;
    xx->fLastSampleNumber = xx->fSampleNumber = xx->fSampleNumberBase = 0;
    xx->fSortOrder = MTC_UNORDERED;
    if (argc < 3)
    {
      LOG_ERROR_1(OUTPUT_PREFIX "missing argments")
      okSoFar = false;
    }
    else
    {
      long numSpots = DEFAULT_SPOTS;

      /* Check the number of spots */
      if (argv[0].a_type == A_LONG)
        numSpots = argv[0].a_w.w_long;
      else if (argv[0].a_type == A_FLOAT)
        numSpots = long(argv[0].a_w.w_float);
      else
        LOG_ERROR_1(OUTPUT_PREFIX "number of spots is not numeric")
      if (okSoFar)
      {
        if ((numSpots <= 0) || (numSpots > MAX_SPOTS))
        {
          LOG_ERROR_2(OUTPUT_PREFIX "invalid number of spots (%ld) for device", numSpots)
          numSpots = DEFAULT_SPOTS;
        }
        xx->fNumSpots = short(numSpots);
      }
      if (okSoFar)
      {
        /* Check the mapping file */
        if (argv[1].a_type == A_SYM)
        {
          xx->fMapFileName = GETBYTES(strlen(argv[1].a_w.w_sym->s_name) + 1, char);
          if (xx->fMapFileName)
            strcpy(xx->fMapFileName, argv[1].a_w.w_sym->s_name);
        }
        else
        {
          LOG_ERROR_1(OUTPUT_PREFIX "mapping file name is invalid")
          okSoFar = false;
        }
      }
      if (okSoFar)
      {
        /* Check the normalization file */
        if (argv[2].a_type == A_SYM)
        {
          xx->fNormalFileName = GETBYTES(strlen(argv[2].a_w.w_sym->s_name) + 1, char);
          if (xx->fNormalFileName)
            strcpy(xx->fNormalFileName, argv[2].a_w.w_sym->s_name);
        }
        else
        {
          LOG_ERROR_1(OUTPUT_PREFIX "normalization file name is invalid")
          okSoFar = false;
        }
      }
    }
    if (okSoFar && (argc > 3))
    {
      /* Check the mode */
      if (argv[3].a_type == A_SYM)
        okSoFar = mtcSetMode(xx, argv[3].a_w.w_sym);
      else
      {
        LOG_ERROR_1(OUTPUT_PREFIX "mode is invalid")
        okSoFar = false;
      }
    }
    if (okSoFar && (argc > 4))
    {
      /* Check the sort order */
      if (argv[4].a_type == A_SYM)
        okSoFar = mtcSetOrder(xx, argv[4].a_w.w_sym);
      else
      {
        LOG_ERROR_1(OUTPUT_PREFIX "sort order is invalid")
        okSoFar = false;
      }
    }
    if (okSoFar && (argc > 5))
    {
      /* Check the polling rate */
      long pollRate = SER_SAMPLE_RATE;

      if (argv[5].a_type == A_LONG)
        pollRate = argv[5].a_w.w_long;
      else if (argv[5].a_type == A_FLOAT)
        pollRate = long(argv[5].a_w.w_float);
      else
      {
        LOG_ERROR_1(OUTPUT_PREFIX "polling rate is not numeric")
        okSoFar = false;
      }
      if (okSoFar)
      {
        if ((pollRate <= 0) || (pollRate > MAX_POLL_RATE))
        {
          LOG_ERROR_2(OUTPUT_PREFIX "invalid polling rate (%ld) for device", pollRate)
          okSoFar = false;
        }
        else
        xx->fPollRate = short(pollRate);
      }
    }
    if (okSoFar)
    {
      xx->fSpots = GETBYTES(xx->fNumSpots, MtcSpot);
      xx->fDataRecovery = GETBYTES(256, short);
      xx->fDataBuffer = GETBYTES(MAX_MESSAGE_BYTES, uchar);
      /* Set up our connections and private data */
      xx->fProxy = proxy_new(xx, 1L, &xx->fInletNumber);
      xx->fErrorBangOut = static_cast<POutlet>(bangout(xx));
      xx->fChunkSendOut = static_cast<POutlet>(bangout(xx));
      xx->fDataSendOut = static_cast<POutlet>(outlet_new(xx, 0L));  /* list, int */
      xx->fSampleBangOut = static_cast<POutlet>(bangout(xx));
      xx->fCommandComplete = static_cast<POutlet>(bangout(xx));
      xx->fDataStartStopOut = static_cast<POutlet>(intout(xx));
      xx->fDataOut = static_cast<POutlet>(outlet_new(xx, 0L));   /* normally just a list */
      xx->fPollClock = static_cast<PClock>(clock_new(xx,
                                            reinterpret_cast<method>(mtcProcessClock)));
      xx->fPollQueue = static_cast<PQelem>(qelem_new(xx,
                                            reinterpret_cast<method>(mtcProcessQueue)));
      if (xx->fProxy && xx->fErrorBangOut && xx->fDataSendOut && xx->fChunkSendOut &&
          xx->fSampleBangOut && xx->fDataOut && xx->fDataStartStopOut &&
          xx->fCommandComplete && xx->fPollClock && xx->fPollQueue && xx->fMapFileName &&
          xx->fNormalFileName && xx->fSpots && xx->fDataRecovery)
      {
        if (mtcReadMapFile(xx))
        {
          mtcSetupIndices(xx);
          mtcResetNormalization(xx);
          mtcReadNormalizationFile(xx);
          xx->fRawRow = GETBYTES(xx->fMaxCol * xx->fMaxRow, Atom);
          if (xx->fRawRow)
          {
            clock_delay(xx->fPollClock, xx->fPollRate);
#if defined(BE_VERBOSE)
            xx->fVerbose = false;
#endif /* BE_VERBOSE */
            mtcChangeState(xx, MTC_IDLE);
          }
          else
          {
            LOG_ERROR_1(OUTPUT_PREFIX "unable to create raw buffer for device")
            okSoFar = false;
          }
        }
        else
        {
          LOG_ERROR_1(OUTPUT_PREFIX "unable to open map file for device")
          okSoFar = false;
        }
      }
      else
      {
        LOG_ERROR_1(OUTPUT_PREFIX "unable to create port or clock for device")
        okSoFar = false;
      }
    }
    if (! okSoFar)
    {
      freeobject(reinterpret_cast<PObject>(xx));
      xx = NULL_PTR;
    }
  }
  ExitCodeResource();
  return xx;
} /* mtcCreate */

/*------------------------------------ mtcDoStart ---*/
void mtcDoStart
  (MtcControlPtr xx)
{
  uchar compress = 't', no_compress = 'n';

  if (! xx->fStarted)
  {
    mtcPerformWriteCommand(xx, MTC_CMD_BEGIN_SEND_DATA, 1,
                            xx->fUseCompression ? &compress : &no_compress);
    xx->fStarted = true;
  }
} /* mtcDoStart */

/*------------------------------------ mtcDoStop ---*/
void mtcDoStop
  (MtcControlPtr xx)
{
  mtcPerformWriteCommand(xx, MTC_CMD_STOP_SEND_DATA, 0, NULL_PTR);
  xx->fStarted = false;
  xx->fStopped = true;
} /* mtcDoStop */

/*------------------------------------ mtcFree ---*/
Pvoid mtcFree
  (MtcControlPtr xx)
{
  EnterCallback();
  if (xx)
  {
  	xx->fStopping = true;
    if (xx->fPollClock)
    {
      clock_unset(xx->fPollClock);
      clock_free(reinterpret_cast<PObject>(xx->fPollClock));
      xx->fPollClock = NULL_PTR;
    }
    if (xx->fPollQueue)
    {
      qelem_unset(xx->fPollQueue);
      qelem_free(xx->fPollQueue);
      xx->fPollQueue = NULL_PTR;
    }
    if (xx->fProxy)
    {
      freeobject(reinterpret_cast<PObject>(xx->fProxy));
      xx->fProxy = NULL_PTR;
    }
    FREEBYTES(xx->fMapFileName, strlen(xx->fMapFileName) + 1)
    FREEBYTES(xx->fNormalFileName, strlen(xx->fNormalFileName) + 1)
    FREEBYTES(xx->fSpots, xx->fNumBytes)
    FREEBYTES(xx->fDataRecovery, 256)
    FREEBYTES(xx->fDataBuffer, MAX_MESSAGE_BYTES)
    FREEBYTES(xx->fRawRow, xx->fMaxCol * xx->fMaxRow)
    FREEBYTES(xx->fTaxelMatrix, xx->fMaxCol * xx->fMaxRow)
    if (xx->fTaxelMapHandle)
    {
      HUnlock(reinterpret_cast<Handle>(xx->fTaxelMapHandle));
      DisposeHandle(reinterpret_cast<Handle>(xx->fTaxelMapHandle));
      xx->fTaxelMapHandle = NULL_HDL;
    }
  }
  LOG_EXIT()
  ExitMaxMessageHandler()
} /* mtcFree */

/*------------------------------------ mtcPerformWriteCommand ---*/
void mtcPerformWriteCommand
  (MtcControlPtr				xx,
   const MtcCommandCode commandCode,
   const uchar          numBytesToFollow,
   Puchar         			bytesToFollow)
{
  uchar				dataValue;
  static Atom	dataList[MAX_MESSAGE_BYTES + 3];

  if (xx)
  {
    short prevLock = lockout_set(1);

    /* Make sure serialX is in 'chunk' mode. */
    if (! xx->fChunkPulseSent)
    {
      outlet_bang(xx->fChunkSendOut);
      xx->fChunkPulseSent = true;
    }
    /* Send data. */
    SETLONG(dataList, commandCode);
    for (short ii = 0; ii < numBytesToFollow; ii++)
    {
      dataValue = *bytesToFollow++;
      SETLONG(dataList + ii + 1, dataValue);
    }
    SETLONG(dataList + numBytesToFollow + 1, MTC_CMD_END_COMMAND);
    outlet_list(xx->fDataSendOut, 0L, short(numBytesToFollow + 2), dataList);
    lockout_set(prevLock);
  }
} /* mtcPerformWriteCommand */

/*------------------------------------ mtcProcessClock ---*/
Pvoid mtcProcessClock
  (MtcControlPtr xx)
{
  EnterCallback();
  if (xx && (! xx->fStopping))
    qelem_set(xx->fPollQueue);
  ExitMaxMessageHandler()
} /* mtcProcessClock */

/*------------------------------------ mtcProcessQueue ---*/
Pvoid mtcProcessQueue
  (MtcControlPtr xx)
{
  EnterCallback();
  if (xx && (! xx->fStopping))
  {
    short prevLock = lockout_set(1);

    outlet_bang(xx->fSampleBangOut);
    clock_delay(xx->fPollClock, xx->fPollRate);
    lockout_set(prevLock);
    evnum_incr();
  }
  ExitMaxMessageHandler()
} /* mtcProcessQueue */

/*------------------------------------ mtcProcessResponse ---*/
Pvoid mtcProcessResponse
  (MtcControlPtr xx,
   long          rr)
{
  EnterCallback();
  /* We've received a byte. Check if it matches what we are expecting. */
  if (xx)
  {
    short prevLock = lockout_set(1);
    long  incoming = (rr & 0x00ff);

#if defined(BE_VERBOSE)
    if (xx->fVerbose)
    {
      if ((xx->fState != MTC_AWAITING_END_OF_COMP) &&
          (xx->fState != MTC_AWAITING_END_OF_DATA) &&
          (xx->fState != MTC_AWAITING_END_OF_DESC))
        LOG_POST_2(OUTPUT_PREFIX "saw byte: 0x%lx", incoming)
    }
#endif /* BE_VERBOSE */
    switch (xx->fState)
    {
      case MTC_AWAITING_END_OF_COMMAND:
        if (xx->fStopped)
          mtcChangeState(xx, (incoming == MTC_RESPONSE_ACK) ? MTC_LOOKING_FOR_END :
                                                              MTC_SKIP_TO_ACK);
        else if (incoming == MTC_RESPONSE_END_CMD)
        {
          mtcChangeState(xx, MTC_IDLE);
          if (xx->fResponse == MTC_RESPONSE_ACK)
            outlet_bang(xx->fCommandComplete);
          else if ((xx->fResponse == MTC_RESPONSE_SEND_DATA) ||
                  (xx->fResponse == MTC_RESPONSE_SEND_COMP))
            mtcProcessTaxels(xx);
        }
        else
        {
          LOG_ERROR_2(OUTPUT_PREFIX "EOC/unexpected input character (0x%lx) seen",
                      incoming)
          outlet_bang(xx->fErrorBangOut);
          mtcChangeState(xx, MTC_SKIP_TO_END);
        }
        break;

      case MTC_AWAITING_END_OF_COMP:
        if (xx->fStopped)
          mtcChangeState(xx, (incoming == MTC_RESPONSE_ACK) ? MTC_LOOKING_FOR_END :
                                                              MTC_SKIP_TO_ACK);
        else
        {
          /* Copy incoming into the buffer. */
          *(xx->fDataBuffer + xx->fNextByte) = static_cast<uchar>(incoming);
          xx->fNextByte++;
          if (xx->fIsPacketHeader)
          {
            /* Analyze the map byte */
            short count = short(BITCOUNT(incoming));

            if (count > 3)
              count = 3;
            xx->fNumBytes = count;
            if (count)
              xx->fIsPacketHeader = false;
            else
            {
              /* The packet is empty, so count it off */
              if (! --xx->fNumPackets)
              {
                xx->fLastByte = xx->fNextByte;
                mtcChangeState(xx, MTC_AWAITING_END_OF_COMMAND);
              }
            }
          }
          else
          {
            if (! --xx->fNumBytes)
            {
              /* We've received the last byte of the packet */
              if (--xx->fNumPackets)
                xx->fIsPacketHeader = true;
              else
              {
                xx->fLastByte = xx->fNextByte;
                mtcChangeState(xx, MTC_AWAITING_END_OF_COMMAND);
              }
            }
          }
        }
        break;

      case MTC_AWAITING_END_OF_DATA:
        if (xx->fStopped)
          mtcChangeState(xx, (incoming == MTC_RESPONSE_ACK) ? MTC_LOOKING_FOR_END :
                                                              MTC_SKIP_TO_ACK);
        else
        {
          /* Copy incoming into the buffer. */
          *(xx->fDataBuffer + xx->fNextTaxel) = static_cast<uchar>(incoming);
          if (++xx->fNextTaxel >= xx->fNumBytes)
            mtcChangeState(xx, MTC_AWAITING_END_OF_COMMAND);
        }
        break;

      case MTC_AWAITING_END_OF_DESC:
        if (xx->fStopped)
          mtcChangeState(xx, (incoming == MTC_RESPONSE_ACK) ? MTC_LOOKING_FOR_END :
                                                              MTC_SKIP_TO_ACK);
        else if (incoming == MTC_RESPONSE_END_CMD)
        {
          xx->fDescriptor[xx->fDescriptorLength] = '\0';
          LOG_POST_2(OUTPUT_PREFIX "descriptor: '%s'", xx->fDescriptor)
          mtcChangeState(xx, MTC_IDLE);
          outlet_bang(xx->fCommandComplete);
        }
        else
        {
          xx->fDescriptor[xx->fDescriptorLength] = char(incoming);
          if (xx->fDescriptorLength < MAX_DESCRIPTOR_LENGTH)
            xx->fDescriptorLength++;
          else
          {
            LOG_ERROR_1(OUTPUT_PREFIX "overlong descriptor truncated")
            outlet_bang(xx->fErrorBangOut);
            mtcChangeState(xx, MTC_SKIP_TO_END);
          }
        }
        break;

      case MTC_AWAITING_NUM_BYTES:
        if (xx->fStopped)
          mtcChangeState(xx, (incoming == MTC_RESPONSE_ACK) ? MTC_LOOKING_FOR_END :
                                                              MTC_SKIP_TO_ACK);
        else
        {
          xx->fNumBytes = short(incoming);
          xx->fNextTaxel = 0;
          mtcChangeState(xx, MTC_AWAITING_END_OF_DATA);
        }
        break;

      case MTC_AWAITING_NUM_PACKETS:
        if (xx->fStopped)
          mtcChangeState(xx, (incoming == MTC_RESPONSE_ACK) ? MTC_LOOKING_FOR_END :
                                                              MTC_SKIP_TO_ACK);
        else
        {
          if (incoming == xx->fExpectedPackets)
          {
            xx->fNumPackets = short(incoming);
            xx->fNextByte = xx->fLastByte = 0;
            xx->fIsPacketHeader = true;
            mtcChangeState(xx, MTC_AWAITING_END_OF_COMP);
          }
          else
          {
            LOG_ERROR_2(OUTPUT_PREFIX "Bad number of packets (%ld) seen", incoming)
            outlet_bang(xx->fErrorBangOut);
            mtcChangeState(xx, MTC_SKIP_TO_END);
          }
        }
        break;

      case MTC_AWAITING_SAMPLE_NUMBER:
        if (xx->fStopped)
          mtcChangeState(xx, (incoming == MTC_RESPONSE_ACK) ? MTC_LOOKING_FOR_END :
                                                              MTC_SKIP_TO_ACK);
        else
        {
          if ((incoming + xx->fSampleNumberBase) < xx->fSampleNumber)
            xx->fSampleNumberBase += 256;   /* wraparound? */
          xx->fSampleNumber = incoming + xx->fSampleNumberBase;
          mtcChangeState(xx, xx->fDataCompressed ? MTC_AWAITING_NUM_PACKETS :
                                                    MTC_AWAITING_NUM_BYTES);
        }
        break;

      case MTC_IDLE:
        /* Process received data here. */
        xx->fStopped = false;
        xx->fResponse = static_cast<MtcResponseCode>(incoming);
        if (incoming == MTC_RESPONSE_SEND_DATA)
        {
          if (xx->fUseCompression)
          {
            LOG_ERROR_1(OUTPUT_PREFIX "IDLE/unexpected uncompressed data seen")
            outlet_bang(xx->fErrorBangOut);
            mtcChangeState(xx, MTC_SKIP_TO_END);
          }
          else
          {
            mtcChangeState(xx, MTC_AWAITING_SAMPLE_NUMBER);
            xx->fDataCompressed = false;
          }
        }
        else if (incoming == MTC_RESPONSE_SEND_COMP)
        {
          if (xx->fUseCompression)
          {
            mtcChangeState(xx, MTC_AWAITING_SAMPLE_NUMBER);
            xx->fDataCompressed = true;
          }
          else
          {
            LOG_ERROR_1(OUTPUT_PREFIX "IDLE/unexpected compressed data seen")
            outlet_bang(xx->fErrorBangOut);
            mtcChangeState(xx, MTC_SKIP_TO_END);
          }
        }
        else if (incoming == MTC_RESPONSE_ACK)
          mtcChangeState(xx, MTC_AWAITING_END_OF_COMMAND);
        else if (incoming == MTC_RESPONSE_UNIT_DESC)
        {
          xx->fDescriptorLength = 0;
          mtcChangeState(xx, MTC_AWAITING_END_OF_DESC);
        }
        else
        {
          LOG_ERROR_2(OUTPUT_PREFIX "IDLE/unexpected input character (0x%lx) seen",
                      incoming)
          outlet_bang(xx->fErrorBangOut);
          mtcChangeState(xx, MTC_SKIP_TO_END);
        }
        break;

      case MTC_LOOKING_FOR_END:
        if (incoming == MTC_RESPONSE_END_CMD)
        {
          outlet_bang(xx->fCommandComplete);
          mtcChangeState(xx, MTC_IDLE);
        }
        else if (incoming != MTC_RESPONSE_ACK)
          mtcChangeState(xx, MTC_SKIP_TO_ACK);
        break;

      case MTC_SKIP_TO_ACK:
        xx->fStopped = false;
        if (incoming == MTC_RESPONSE_ACK)
          mtcChangeState(xx, MTC_LOOKING_FOR_END);
        break;

      case MTC_SKIP_TO_END:
        if (incoming == MTC_RESPONSE_END_CMD)
          mtcChangeState(xx, MTC_IDLE);
        break;

    }
    lockout_set(prevLock);      
  }
  ExitMaxMessageHandler()
} /* mtcProcessResponse */

/*------------------------------------ mtcSetKind ---*/
bool mtcSetKind
  (MtcControlPtr xx,
   PSymbol       kind)
{
  bool okSoFar = true;

  if (kind == gCompressedSymbol)
    xx->fUseCompression = true;
  else if ((kind == gNormalSymbol) || (kind == gEmptySymbol))
    xx->fUseCompression = false;
  else
  {
    LOG_ERROR_2(OUTPUT_PREFIX "unrecognized compression (%s)", kind->s_name)
    okSoFar = false;
  }
  return okSoFar;
} /* mtcSetKind */

/*------------------------------------ mtcSetMode ---*/
bool mtcSetMode
  (MtcControlPtr xx,
   PSymbol       rawOrCooked)
{
  bool okSoFar = true;

  if (rawOrCooked == gCookedSymbol)
    xx->fModeRaw = false;
  else if (rawOrCooked == gRawSymbol)
    xx->fModeRaw = true;
  else
  {
    LOG_ERROR_2(OUTPUT_PREFIX "unrecognized processing mode (%s)", rawOrCooked->s_name)
    okSoFar = false;
  }
  return okSoFar;
} /* mtcSetMode */

/*------------------------------------ mtcSetOrder ---*/
bool mtcSetOrder
  (MtcControlPtr xx,
   PSymbol       order)
{
  bool okSoFar = true;

  if (order == gPSymbol)
    xx->fSortOrder = MTC_BY_PRESSURE;
  else if (order == gXSymbol)
    xx->fSortOrder = MTC_BY_X;
  else if (order == gYSymbol)
    xx->fSortOrder = MTC_BY_Y;
  else
  {
    LOG_ERROR_2(OUTPUT_PREFIX "unrecognized sort order (%s)", order->s_name)
    okSoFar = false;
  }
  return okSoFar;
} /* mtcSetOrder */

StandardInfoRoutine(MtcControlPtr)