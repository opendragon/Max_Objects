/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       mtc.cpp                                                                 */
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
#include "reportVersion.h"

/* Forward references: */
void mtcChangeState(MtcData *      xx,
                    const MtcState newState);

void * mtcCreate(t_symbol * ss,
                 short      argc,
                 t_atom *   argv);

void mtcFree(MtcData * xx);

void mtcProcessClock(MtcData * xx);

void mtcProcessQueue(MtcData * xx);

/*------------------------------------ main ---*/
int main(void)
{
    /* Allocate class memory and set up class. */
    t_class * temp = class_new(OUR_NAME, reinterpret_cast<method>(mtcCreate), reinterpret_cast<method>(mtcFree),
                               sizeof(MtcData), reinterpret_cast<method>(0L), A_GIMME, 0);

    if (temp)
    {
        class_addmethod(temp, reinterpret_cast<method>(cmd_Anything), MESSAGE_ANYTHING, A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Assist), MESSAGE_ASSIST, A_CANT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Describe), "describe", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Int), MESSAGE_INT, A_LONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_List), MESSAGE_LIST, A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Mode), "mode", A_SYM, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Order), "order", A_SYM, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Ping), "ping", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Start), "start", A_DEFSYM, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Stop), "stop", 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Threshold), "threshold", A_LONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Train), "train", A_DEFSYM, 0);
#if defined(BE_VERBOSE)
        class_addmethod(temp, reinterpret_cast<method>(cmd_Verbose), "verbose", A_DEFSYM, 0);
#endif /* BE_VERBOSE */
        class_register(CLASS_BOX, temp);
    }
    gClass = temp;
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
    reportVersion(OUR_NAME);
    return 0;
} // main
/*------------------------------------ mtcChangeState ---*/
void mtcChangeState(MtcData *      xx,
                    const MtcState newState)
{
#if defined(REPORT_STATE_CHANGES)
    const char * oldString;
    static char  numBuffer[NUM_BUFF_SIZE];
#endif /* REPORT_STATE_CHANGES */

    if (xx)
    {
#if defined(REPORT_STATE_CHANGES)
        if (xx->fVerbose)
        {
            switch (xx->fState)
            {
                case kMtcStateAwaitingEndOfCommand:
                    oldString = "kMtcStateAwaitingEndOfCommand";
                    break;

                case kMtcStateAwaitingEndOfComp:
                    oldString = "kMtcStateAwaitingEndOfComp";
                    break;

                case kMtcStateAwaitingEndOfData:
                    oldString = "kMtcStateAwaitingEndOfData";
                    break;

                case kMtcStateAwaitingEndOfDesc:
                    oldString = "kMtcStateAwaitingEndOfDesc";
                    break;

                case kMtcStateAwaitingNumBytes:
                    oldString = "kMtcStateAwaitingNumBytes";
                    break;

                case kMtcStateAwaitingNumPackets:
                    oldString = "kMtcStateAwaitingNumPackets";
                    break;

                case kMtcStateAwaitingSampleNumber:
                    oldString = "kMtcStateAwaitingSampleNumber";
                    break;

                case kMtcStateIdle:
                    oldString = "kMtcStateIdle";
                    break;

                case kMtcStateLookingForEnd:
                    oldString = "kMtcStateLookingForEnd";
                    break;

                case kMtcStateSkipToAck:
                    oldString = "kMtcStateSkipToAck";
                    break;

                case kMtcStateSkipToEnd:
                    oldString = "kMtcStateSkipToEnd";
                    break;

                default:
                    snprintf(numBuffer, sizeof(numBuffer), "unrecognized state %ld", static_cast<long>(xx->fState));
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
                case kMtcStateAwaitingEndOfComp:
                    LOG_POST_2(xx, OUTPUT_PREFIX "%s -> kMtcStateAwaitingEndOfComp", oldString)
                    break;

                case kMtcStateAwaitingEndOfData:
                    LOG_POST_2(xx, OUTPUT_PREFIX "%s -> kMtcStateAwaitingEndOfData", oldString)
                    break;

                case kMtcStateAwaitingEndOfDesc:
                    LOG_POST_2(xx, OUTPUT_PREFIX "%s -> kMtcStateAwaitingEndOfDesc", oldString)
                    break;

                case kMtcStateAwaitingEndOfCommand:
                    LOG_POST_2(xx, OUTPUT_PREFIX "%s -> kMtcStateAwaitingEndOfCommand", oldString)
                    break;

                case kMtcStateAwaitingNumBytes:
                    LOG_POST_2(xx, OUTPUT_PREFIX "%s -> kMtcStateAwaitingNumBytes", oldString)
                    break;

                case kMtcStateAwaitingNumPackets:
                    LOG_POST_2(xx, OUTPUT_PREFIX "%s -> kMtcStateAwaitingNumPackets", oldString)
                    break;

                case kMtcStateAwaitingSampleNumber:
                    LOG_POST_2(xx, OUTPUT_PREFIX "%s -> kMtcStateAwaitingSampleNumber", oldString)
                    break;

                case kMtcStateIdle:
                    LOG_POST_2(xx, OUTPUT_PREFIX "%s -> kMtcStateIdle", oldString)
                    break;

                case kMtcStateLookingForEnd:
                    LOG_POST_2(xx, OUTPUT_PREFIX "%s -> kMtcStateLookingForEnd", oldString)
                    break;

                case kMtcStateSkipToAck:
                    LOG_POST_2(xx, OUTPUT_PREFIX "%s -> kMtcStateSkipToAck", oldString)
                    break;

                case kMtcStateSkipToEnd:
                    LOG_POST_2(xx, OUTPUT_PREFIX "%s -> kMtcStateSkipToEnd", oldString);
                    break;

                default:
                    LOG_POST_3(xx, OUTPUT_PREFIX "%s -> unrecognized state (%ld)", oldString, xx->fState)
                    break;
            }
        }
#endif /* REPORT_STATE_CHANGES */
    }
} // mtcChangeState
/*------------------------------------ mtcCreate ---*/
void * mtcCreate(t_symbol * ss,
                 short      argc,
                 t_atom *   argv)
{
#pragma unused(ss)
    MtcData * xx = static_cast<MtcData *>(object_alloc(gClass));

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
        xx->fChunkPulseSent = xx->fStarted = xx->fStopped = xx->fNormalizing = xx->fModeRaw = false;
        xx->fDataCompressed = xx->fIsPacketHeader = xx->fUseCompression = xx->fStopping = false;
        xx->fDescriptorLength = xx->fNumTaxels = xx->fExpectedPackets = 0;
        xx->fThreshold = MTC_DEFAULT_THRESHOLD;
        xx->fLastSampleNumber = xx->fSampleNumber = xx->fSampleNumberBase = 0;
        xx->fSortOrder = kMtcOrderUnordered;
        if (argc < 3)
        {
            LOG_ERROR_1(xx, OUTPUT_PREFIX "missing argments")
            okSoFar = false;
        }
        else
        {
            long numSpots = DEFAULT_SPOTS;

            /* Check the number of spots */
            if (A_LONG == argv[0].a_type)
            {
                numSpots = argv[0].a_w.w_long;
            }
            else if (A_FLOAT == argv[0].a_type)
            {
                numSpots = static_cast<long>(argv[0].a_w.w_float);
            }
            else
            {
                LOG_ERROR_1(xx, OUTPUT_PREFIX "number of spots is not numeric")
            }
            if (okSoFar)
            {
                if ((numSpots <= 0) || (numSpots > MAX_SPOTS))
                {
                    LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid number of spots (%ld) for device", numSpots)
                    numSpots = DEFAULT_SPOTS;
                }
                xx->fNumSpots = static_cast<short>(numSpots);
            }
            if (okSoFar)
            {
                /* Check the mapping file */
                if (A_SYM == argv[1].a_type)
                {
                    xx->fMapFileName = GETBYTES(strlen(argv[1].a_w.w_sym->s_name) + 1, char);
                    if (xx->fMapFileName)
                    {
                        strcpy(xx->fMapFileName, argv[1].a_w.w_sym->s_name);
                    }
                }
                else
                {
                    LOG_ERROR_1(xx, OUTPUT_PREFIX "mapping file name is invalid")
                    okSoFar = false;
                }
            }
            if (okSoFar)
            {
                /* Check the normalization file */
                if (A_SYM == argv[2].a_type)
                {
                    xx->fNormalFileName = GETBYTES(strlen(argv[2].a_w.w_sym->s_name) + 1, char);
                    if (xx->fNormalFileName)
                    {
                        strcpy(xx->fNormalFileName, argv[2].a_w.w_sym->s_name);
                    }
                }
                else
                {
                    LOG_ERROR_1(xx, OUTPUT_PREFIX "normalization file name is invalid")
                    okSoFar = false;
                }
            }
        }
        if (okSoFar && (argc > 3))
        {
            /* Check the mode */
            if (A_SYM == argv[3].a_type)
            {
                okSoFar = mtcSetMode(xx, argv[3].a_w.w_sym);
            }
            else
            {
                LOG_ERROR_1(xx, OUTPUT_PREFIX "mode is invalid")
                okSoFar = false;
            }
        }
        if (okSoFar && (argc > 4))
        {
            /* Check the sort order */
            if (A_SYM == argv[4].a_type)
            {
                okSoFar = mtcSetOrder(xx, argv[4].a_w.w_sym);
            }
            else
            {
                LOG_ERROR_1(xx, OUTPUT_PREFIX "sort order is invalid")
                okSoFar = false;
            }
        }
        if (okSoFar && (argc > 5))
        {
            /* Check the polling rate */
            long pollRate = SER_SAMPLE_RATE;

            if (A_LONG == argv[5].a_type)
            {
                pollRate = argv[5].a_w.w_long;
            }
            else if (A_FLOAT == argv[5].a_type)
            {
                pollRate = static_cast<long>(argv[5].a_w.w_float);
            }
            else
            {
                LOG_ERROR_1(xx, OUTPUT_PREFIX "polling rate is not numeric")
                okSoFar = false;
            }
            if (okSoFar)
            {
                if ((pollRate <= 0) || (pollRate > MAX_POLL_RATE))
                {
                    LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid polling rate (%ld) for device", pollRate)
                    okSoFar = false;
                }
                else
                {
                    xx->fPollRate = static_cast<short>(pollRate);
                }
            }
        }
        if (okSoFar)
        {
            xx->fSpots = GETBYTES(xx->fNumSpots, MtcSpot);
            xx->fDataRecovery = GETBYTES(256, short);
            xx->fDataBuffer = GETBYTES(MAX_MESSAGE_BYTES, unsigned char);
            /* Set up our connections and private data */
            xx->fProxy = proxy_new(xx, 1L, &xx->fInletNumber);
            xx->fErrorBangOut = static_cast<t_outlet *>(bangout(xx));
            xx->fChunkSendOut = static_cast<t_outlet *>(bangout(xx));
            xx->fDataSendOut = static_cast<t_outlet *>(outlet_new(xx, 0L));  /* list, int */
            xx->fSampleBangOut = static_cast<t_outlet *>(bangout(xx));
            xx->fCommandComplete = static_cast<t_outlet *>(bangout(xx));
            xx->fDataStartStopOut = static_cast<t_outlet *>(intout(xx));
            xx->fDataOut = static_cast<t_outlet *>(outlet_new(xx, 0L));   /* normally just a list */
            xx->fPollClock = static_cast<t_clock *>(clock_new(xx, reinterpret_cast<method>(mtcProcessClock)));
            xx->fPollQueue = static_cast<t_qelem *>(qelem_new(xx, reinterpret_cast<method>(mtcProcessQueue)));
            if (xx->fProxy && xx->fErrorBangOut && xx->fDataSendOut && xx->fChunkSendOut && xx->fSampleBangOut &&
                xx->fDataOut && xx->fDataStartStopOut && xx->fCommandComplete && xx->fPollClock && xx->fPollQueue &&
                xx->fMapFileName && xx->fNormalFileName && xx->fSpots && xx->fDataRecovery)
            {
                if (mtcReadMapFile(xx))
                {
                    mtcSetupIndices(xx);
                    mtcResetNormalization(xx);
                    mtcReadNormalizationFile(xx);
                    xx->fRawRow = GETBYTES(xx->fMaxCol * xx->fMaxRow, t_atom);
                    if (xx->fRawRow)
                    {
                        clock_delay(xx->fPollClock, xx->fPollRate);
#if defined(BE_VERBOSE)
                        xx->fVerbose = false;
#endif /* BE_VERBOSE */
                        mtcChangeState(xx, kMtcStateIdle);
                    }
                    else
                    {
                        LOG_ERROR_1(xx, OUTPUT_PREFIX "unable to create raw buffer for device")
                        okSoFar = false;
                    }
                }
                else
                {
                    LOG_ERROR_1(xx, OUTPUT_PREFIX "unable to open map file for device")
                    okSoFar = false;
                }
            }
            else
            {
                LOG_ERROR_1(xx, OUTPUT_PREFIX "unable to create port or clock for device")
                okSoFar = false;
            }
        }
        if (! okSoFar)
        {
            freeobject(reinterpret_cast<t_object *>(xx));
            xx = NULL_PTR;
        }
    }
    return xx;
} // mtcCreate
/*------------------------------------ mtcDoStart ---*/
void mtcDoStart(MtcData * xx)
{
    unsigned char compress = 't';
    unsigned char no_compress = 'n';

    if (! xx->fStarted)
    {
        mtcPerformWriteCommand(xx, kMtcCommandBeginSendData, 1, xx->fUseCompression ? &compress : &no_compress);
        xx->fStarted = true;
    }
} // mtcDoStart
/*------------------------------------ mtcDoStop ---*/
void mtcDoStop(MtcData * xx)
{
    mtcPerformWriteCommand(xx, kMtcCommandStopSendData, 0, NULL_PTR);
    xx->fStarted = false;
    xx->fStopped = true;
} // mtcDoStop
/*------------------------------------ mtcFree ---*/
void mtcFree(MtcData * xx)
{
    if (xx)
    {
        xx->fStopping = true;
        if (xx->fPollClock)
        {
            clock_unset(xx->fPollClock);
            clock_free(reinterpret_cast<t_object *>(xx->fPollClock));
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
            freeobject(reinterpret_cast<t_object *>(xx->fProxy));
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
            sysmem_lockhandle(reinterpret_cast<t_handle>(xx->fTaxelMapHandle), 0);
            sysmem_freehandle(reinterpret_cast<t_handle>(xx->fTaxelMapHandle));
            xx->fTaxelMapHandle = NULL_HDL;
        }
    }
} // mtcFree
/*------------------------------------ mtcPerformWriteCommand ---*/
void mtcPerformWriteCommand(MtcData *            xx,
                            const MtcCommandCode commandCode,
                            const unsigned char  numBytesToFollow,
                            unsigned char *      bytesToFollow)
{
    unsigned char dataValue;
    static t_atom dataList[MAX_MESSAGE_BYTES + 3];

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
        for (short ii = 0; ii < numBytesToFollow; ++ii)
        {
            dataValue = *bytesToFollow++;
            SETLONG(dataList + ii + 1, dataValue);
        }
        SETLONG(dataList + numBytesToFollow + 1, kMtcCommandEnd);
        outlet_list(xx->fDataSendOut, 0L, static_cast<short>(numBytesToFollow + 2), dataList);
        lockout_set(prevLock);
    }
} // mtcPerformWriteCommand
/*------------------------------------ mtcProcessClock ---*/
void mtcProcessClock(MtcData * xx)
{
    if (xx && (! xx->fStopping))
    {
        qelem_set(xx->fPollQueue);
    }
} // mtcProcessClock
/*------------------------------------ mtcProcessQueue ---*/
void mtcProcessQueue(MtcData * xx)
{
    if (xx && (! xx->fStopping))
    {
        short prevLock = lockout_set(1);

        outlet_bang(xx->fSampleBangOut);
        clock_delay(xx->fPollClock, xx->fPollRate);
        lockout_set(prevLock);
        evnum_incr();
    }
} // mtcProcessQueue
/*------------------------------------ mtcProcessResponse ---*/
void mtcProcessResponse(MtcData * xx,
                        long      rr)
{
    /* We've received a byte. Check if it matches what we are expecting. */
    if (xx)
    {
        short prevLock = lockout_set(1);
        long  incoming = (rr & 0x00ff);

#if defined(BE_VERBOSE)
        if (xx->fVerbose)
        {
            if ((xx->fState != kMtcStateAwaitingEndOfComp) && (xx->fState != kMtcStateAwaitingEndOfData) &&
                (xx->fState != kMtcStateAwaitingEndOfDesc))
            {
                LOG_POST_2(xx, OUTPUT_PREFIX "saw byte: 0x%lx", incoming)
            }
        }
#endif /* BE_VERBOSE */
        switch (xx->fState)
        {
            case kMtcStateAwaitingEndOfCommand:
                if (xx->fStopped)
                {
                    mtcChangeState(xx, (kMtcResponseAck == incoming) ? kMtcStateLookingForEnd : kMtcStateSkipToAck);
                }
                else if (kMtcResponseEndCommand == incoming)
                {
                    mtcChangeState(xx, kMtcStateIdle);
                    if (kMtcResponseAck == xx->fResponse)
                    {
                        outlet_bang(xx->fCommandComplete);
                    }
                    else if ((kMtcResponseSendData == xx->fResponse) || (kMtcResponseSendComp == xx->fResponse))
                    {
                        mtcProcessTaxels(xx);
                    }
                }
                else
                {
                    LOG_ERROR_2(xx, OUTPUT_PREFIX "EOC/unexpected input character (0x%lx) seen", incoming)
                    outlet_bang(xx->fErrorBangOut);
                    mtcChangeState(xx, kMtcStateSkipToEnd);
                }
                break;

            case kMtcStateAwaitingEndOfComp:
                if (xx->fStopped)
                {
                    mtcChangeState(xx, (kMtcResponseAck == incoming) ? kMtcStateLookingForEnd : kMtcStateSkipToAck);
                }
                else
                {
                    /* Copy incoming into the buffer. */
                    *(xx->fDataBuffer + xx->fNextByte) = static_cast<unsigned char>(incoming);
                    ++xx->fNextByte;
                    if (xx->fIsPacketHeader)
                    {
                        /* Analyze the map byte */
                        short count = static_cast<short>(BITCOUNT(incoming));

                        if (count > 3)
                        {
                            count = 3;
                        }
                        xx->fNumBytes = count;
                        if (count)
                        {
                            xx->fIsPacketHeader = false;
                        }
                        else
                        {
                            /* The packet is empty, so count it off */
                            if (! --xx->fNumPackets)
                            {
                                xx->fLastByte = xx->fNextByte;
                                mtcChangeState(xx, kMtcStateAwaitingEndOfCommand);
                            }
                        }
                    }
                    else if (! --xx->fNumBytes)
                    {
                        /* We've received the last byte of the packet */
                        if (--xx->fNumPackets)
                        {
                            xx->fIsPacketHeader = true;
                        }
                        else
                        {
                            xx->fLastByte = xx->fNextByte;
                            mtcChangeState(xx, kMtcStateAwaitingEndOfCommand);
                        }
                    }
                }
                break;

            case kMtcStateAwaitingEndOfData:
                if (xx->fStopped)
                {
                    mtcChangeState(xx, (kMtcResponseAck == incoming) ? kMtcStateLookingForEnd : kMtcStateSkipToAck);
                }
                else
                {
                    /* Copy incoming into the buffer. */
                    *(xx->fDataBuffer + xx->fNextTaxel) = static_cast<unsigned char>(incoming);
                    if (++xx->fNextTaxel >= xx->fNumBytes)
                    {
                        mtcChangeState(xx, kMtcStateAwaitingEndOfCommand);
                    }
                }
                break;

            case kMtcStateAwaitingEndOfDesc:
                if (xx->fStopped)
                {
                    mtcChangeState(xx, (kMtcResponseAck == incoming) ? kMtcStateLookingForEnd : kMtcStateSkipToAck);
                }
                else if (kMtcResponseEndCommand == incoming)
                {
                    xx->fDescriptor[xx->fDescriptorLength] = '\0';
                    LOG_POST_2(xx, OUTPUT_PREFIX "descriptor: '%s'", xx->fDescriptor)
                    mtcChangeState(xx, kMtcStateIdle);
                    outlet_bang(xx->fCommandComplete);
                }
                else
                {
                    xx->fDescriptor[xx->fDescriptorLength] = static_cast<char>(incoming);
                    if (xx->fDescriptorLength < MAX_DESCRIPTOR_LENGTH)
                    {
                        ++xx->fDescriptorLength;
                    }
                    else
                    {
                        LOG_ERROR_1(xx, OUTPUT_PREFIX "overlong descriptor truncated")
                        outlet_bang(xx->fErrorBangOut);
                        mtcChangeState(xx, kMtcStateSkipToEnd);
                    }
                }
                break;

            case kMtcStateAwaitingNumBytes:
                if (xx->fStopped)
                {
                    mtcChangeState(xx, (kMtcResponseAck == incoming) ? kMtcStateLookingForEnd : kMtcStateSkipToAck);
                }
                else
                {
                    xx->fNumBytes = static_cast<short>(incoming);
                    xx->fNextTaxel = 0;
                    mtcChangeState(xx, kMtcStateAwaitingEndOfData);
                }
                break;

            case kMtcStateAwaitingNumPackets:
                if (xx->fStopped)
                {
                    mtcChangeState(xx, (kMtcResponseAck == incoming) ? kMtcStateLookingForEnd : kMtcStateSkipToAck);
                }
                else if (xx->fExpectedPackets == incoming)
                {
                    xx->fNumPackets = static_cast<short>(incoming);
                    xx->fNextByte = xx->fLastByte = 0;
                    xx->fIsPacketHeader = true;
                    mtcChangeState(xx, kMtcStateAwaitingEndOfComp);
                }
                else
                {
                    LOG_ERROR_2(xx, OUTPUT_PREFIX "Bad number of packets (%ld) seen", incoming)
                    outlet_bang(xx->fErrorBangOut);
                    mtcChangeState(xx, kMtcStateSkipToEnd);
                }
                break;

            case kMtcStateAwaitingSampleNumber:
                if (xx->fStopped)
                {
                    mtcChangeState(xx, (kMtcResponseAck == incoming) ? kMtcStateLookingForEnd : kMtcStateSkipToAck);
                }
                else
                {
                    if ((incoming + xx->fSampleNumberBase) < xx->fSampleNumber)
                    {
                        xx->fSampleNumberBase += 256;   /* wraparound? */
                    }
                    xx->fSampleNumber = incoming + xx->fSampleNumberBase;
                    mtcChangeState(xx, xx->fDataCompressed ? kMtcStateAwaitingNumPackets : kMtcStateAwaitingNumBytes);
                }
                break;

            case kMtcStateIdle:
                /* Process received data here. */
                xx->fStopped = false;
                xx->fResponse = static_cast<MtcResponseCode>(incoming);
                if (kMtcResponseSendData == incoming)
                {
                    if (xx->fUseCompression)
                    {
                        LOG_ERROR_1(xx, OUTPUT_PREFIX "IDLE/unexpected uncompressed data seen")
                        outlet_bang(xx->fErrorBangOut);
                        mtcChangeState(xx, kMtcStateSkipToEnd);
                    }
                    else
                    {
                        mtcChangeState(xx, kMtcStateAwaitingSampleNumber);
                        xx->fDataCompressed = false;
                    }
                }
                else if (kMtcResponseSendComp == incoming)
                {
                    if (xx->fUseCompression)
                    {
                        mtcChangeState(xx, kMtcStateAwaitingSampleNumber);
                        xx->fDataCompressed = true;
                    }
                    else
                    {
                        LOG_ERROR_1(xx, OUTPUT_PREFIX "IDLE/unexpected compressed data seen")
                        outlet_bang(xx->fErrorBangOut);
                        mtcChangeState(xx, kMtcStateSkipToEnd);
                    }
                }
                else if (kMtcResponseAck == incoming)
                {
                    mtcChangeState(xx, kMtcStateAwaitingEndOfCommand);
                }
                else if (kMtcResponseUnitDesc == incoming)
                {
                    xx->fDescriptorLength = 0;
                    mtcChangeState(xx, kMtcStateAwaitingEndOfDesc);
                }
                else
                {
                    LOG_ERROR_2(xx, OUTPUT_PREFIX "IDLE/unexpected input character (0x%lx) seen", incoming)
                    outlet_bang(xx->fErrorBangOut);
                    mtcChangeState(xx, kMtcStateSkipToEnd);
                }
                break;

            case kMtcStateLookingForEnd:
                if (kMtcResponseEndCommand == incoming)
                {
                    outlet_bang(xx->fCommandComplete);
                    mtcChangeState(xx, kMtcStateIdle);
                }
                else if (incoming != kMtcResponseAck)
                {
                    mtcChangeState(xx, kMtcStateSkipToAck);
                }
                break;

            case kMtcStateSkipToAck:
                xx->fStopped = false;
                if (kMtcResponseAck == incoming)
                {
                    mtcChangeState(xx, kMtcStateLookingForEnd);
                }
                break;

            case kMtcStateSkipToEnd:
                if (kMtcResponseEndCommand == incoming)
                {
                    mtcChangeState(xx, kMtcStateIdle);
                }
                break;

            default:
                break;
        }
        lockout_set(prevLock);
    }
} // mtcProcessResponse
/*------------------------------------ mtcSetKind ---*/
bool mtcSetKind(MtcData *  xx,
                t_symbol * kind)
{
    bool okSoFar = true;

    if (kind == gCompressedSymbol)
    {
        xx->fUseCompression = true;
    }
    else if ((kind == gNormalSymbol) || (kind == gEmptySymbol))
    {
        xx->fUseCompression = false;
    }
    else
    {
        LOG_ERROR_2(xx, OUTPUT_PREFIX "unrecognized compression (%s)", kind->s_name)
        okSoFar = false;
    }
    return okSoFar;
} // mtcSetKind
/*------------------------------------ mtcSetMode ---*/
bool mtcSetMode(MtcData *  xx,
                t_symbol * rawOrCooked)
{
    bool okSoFar = true;

    if (rawOrCooked == gCookedSymbol)
    {
        xx->fModeRaw = false;
    }
    else if (rawOrCooked == gRawSymbol)
    {
        xx->fModeRaw = true;
    }
    else
    {
        LOG_ERROR_2(xx, OUTPUT_PREFIX "unrecognized processing mode (%s)", rawOrCooked->s_name)
        okSoFar = false;
    }
    return okSoFar;
} // mtcSetMode
/*------------------------------------ mtcSetOrder ---*/
bool mtcSetOrder(MtcData *  xx,
                 t_symbol * order)
{
    bool okSoFar = true;

    if (order == gPSymbol)
    {
        xx->fSortOrder = kMtcOrderByPressure;
    }
    else if (order == gXSymbol)
    {
        xx->fSortOrder = kMtcOrderByX;
    }
    else if (order == gYSymbol)
    {
        xx->fSortOrder = kMtcOrderByY;
    }
    else
    {
        LOG_ERROR_2(xx, OUTPUT_PREFIX "unrecognized sort order (%s)", order->s_name)
        okSoFar = false;
    }
    return okSoFar;
} // mtcSetOrder
