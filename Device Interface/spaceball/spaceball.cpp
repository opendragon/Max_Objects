/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       spaceball.cpp                                                           */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the spaceball module.     */
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

#define IS_MAIN /* */
#include "spaceball.h"
#include "reportVersion.h"

/*------------------------------------ spaceballProcessClock ---*/
static void spaceballProcessClock(SpaceballData * xx)
{
    if (xx && (! xx->fStopping))
    {
        qelem_set(xx->fPollQueue);
    }
} // spaceballProcessClock

/*------------------------------------ spaceballProcessQueue ---*/
static void spaceballProcessQueue(SpaceballData * xx)
{
    if (xx && (! xx->fStopping))
    {
        short prevLock = lockout_set(1);
        
        if (! xx->fReset)
        {
            static unsigned char resetString[] = "@RESET\015\015";
            
            if (! xx->fDelayCounter)
            {
                spaceballPerformWriteCommand(xx, sizeof(resetString) - 1, resetString);
                ++xx->fDelayCounter;
            }
            else if (xx->fDelayCounter++ >= xx->fResetDuration)
            {
                xx->fReset = true;
                xx->fDelayCounter = 0;
            }
        }
        else if (! xx->fInited)
        {
            static unsigned char initString[] =
                                            "CB\015NT\015FR?\015P@r@r\015MSSV\015Z\015BcCcCc\015";
            
            if (! xx->fDelayCounter)
            {
                spaceballPerformWriteCommand(xx, sizeof(initString) - 1, initString);
                spaceballZeroValues(xx);
                ++xx->fDelayCounter;
            }
            else if (xx->fDelayCounter++ >= xx->fInitDuration)
            {
                xx->fInited = true;
                xx->fDelayCounter = 0;
            }
        }
        outlet_bang(xx->fSampleBangOut);
        clock_delay(xx->fPollClock, xx->fPollRate);
        lockout_set(prevLock);
#if USE_EVNUM
        evnum_incr();
#endif /* USE_EVNUM */
    }
} // spaceballProcessQueue

/*------------------------------------ spaceballCreate ---*/
static void * spaceballCreate(t_symbol * addOrDelta,
                              const long pollRate)
{
    SpaceballData * xx = static_cast<SpaceballData *>(object_alloc(gClass));
    
    if (xx)
    {
        xx->fPollClock = NULL;
        xx->fPollQueue = NULL;
        xx->fBufferPos = xx->fDelayCounter = 0;
        xx->fButtons = 0;
        xx->fChunkPulseSent = xx->fInited = xx->fNextCharEscaped = xx->fOutputBlocked = false;
        xx->fReset = xx->fSkipping = xx->fModeDelta = xx->fStopping = false;
        if ((pollRate < 0) || (pollRate > MAX_POLL_RATE))
        {
            LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid polling rate (%ld) for device", pollRate)
            xx->fPollRate = SER_SAMPLE_RATE;
        }
        else
        {
            xx->fPollRate = static_cast<short>(pollRate ? pollRate : SER_SAMPLE_RATE);
        }
        /* Set up our connections and private data */
        xx->fProxy = proxy_new(xx, 1L, &xx->fInletNumber);
        xx->fErrorBangOut = static_cast<t_outlet *>(bangout(xx));
        xx->fChunkSendOut = static_cast<t_outlet *>(bangout(xx));
        xx->fDataSendOut = static_cast<t_outlet *>(outlet_new(xx, 0L));  /* list, int */
        xx->fSampleBangOut = static_cast<t_outlet *>(bangout(xx));
        xx->fDataOut = static_cast<t_outlet *>(outlet_new(xx, 0L));   /* normally just a list */
        xx->fPollClock = MAKE_CLOCK(xx, spaceballProcessClock);
        xx->fPollQueue = MAKE_QELEM(xx, spaceballProcessQueue);
        if (! (xx->fProxy && xx->fErrorBangOut && xx->fDataSendOut && xx->fChunkSendOut &&
               xx->fSampleBangOut && xx->fDataOut && xx->fPollClock && xx->fPollQueue))
        {
            LOG_ERROR_1(xx, OUTPUT_PREFIX "unable to create port or clock for device")
            freeobject(reinterpret_cast<t_object *>(xx));
            xx = NULL;
        }
        else
        {
            xx->fResetDuration = static_cast<short>((2000 + xx->fPollRate - 1) / xx->fPollRate);
            xx->fInitDuration = static_cast<short>((1000 + xx->fPollRate - 1) / xx->fPollRate);
            if (addOrDelta != gEmptySymbol)
            {
                spaceballSetMode(xx, addOrDelta);
            }
            clock_delay(xx->fPollClock, xx->fPollRate);
        }
    }
    return xx;
} // spaceballCreate

/*------------------------------------ spaceballFree ---*/
static void spaceballFree(SpaceballData * xx)
{
    if (xx)
    {
        xx->fStopping = true;
        if (xx->fPollClock)
        {
            clock_unset(xx->fPollClock);
            clock_free(reinterpret_cast<t_object *>(xx->fPollClock));
            xx->fPollClock = NULL;
        }
        if (xx->fPollQueue)
        {
            qelem_unset(xx->fPollQueue);
            qelem_free(xx->fPollQueue);
            xx->fPollQueue = NULL;
        }
        if (xx->fProxy)
        {
            freeobject(reinterpret_cast<t_object *>(xx->fProxy));
            xx->fProxy = NULL;
        }
    }
} // spaceballFree

/*------------------------------------ main ---*/
int main(void)
{
    /* Allocate class memory and set up class. */
    t_class * temp = class_new(OUR_NAME, reinterpret_cast<method>(spaceballCreate),
                               reinterpret_cast<method>(spaceballFree), sizeof(SpaceballData),
                               reinterpret_cast<method>(0L), A_DEFSYM, A_DEFLONG, 0);

    if (temp)
    {
        class_addmethod(temp, reinterpret_cast<method>(cmd_Anything), MESSAGE_ANYTHING, A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Assist), MESSAGE_ASSIST, A_CANT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Int), MESSAGE_INT, A_LONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_List), MESSAGE_LIST, A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Mode), "mode", A_SYM, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Reset), "reset", 0);
#if defined(BE_VERBOSE)
        class_addmethod(temp, reinterpret_cast<method>(cmd_Verbose), "verbose", A_DEFSYM, 0);
#endif /* BE_VERBOSE */
        class_addmethod(temp, reinterpret_cast<method>(cmd_Zero), "zero", 0);
        class_register(CLASS_BOX, temp);
    }
    gClass = temp;
    gAddSymbol = gensym("add");
    gButtonSymbol = gensym("button");
    gDeltaSymbol = gensym("delta");
    gEmptySymbol = gensym("");
    gOffSymbol = gensym("off");
    gOnSymbol = gensym("on");
    gRotateSymbol = gensym("rotate");
    gTranslateSymbol = gensym("translate");
    reportVersion(OUR_NAME);
    return 0;
} // main

/*------------------------------------ spaceballPerformWriteCommand ---*/
void spaceballPerformWriteCommand(SpaceballData * xx,
                                  const short     numBytesToSend,
                                  unsigned char * bytesToFollow)
{
    unsigned char dataValue;
    static t_atom dataList[OUT_BUFFER_SIZE];

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
        for (short ii = 0; ii < numBytesToSend; ++ii, ++bytesToFollow)
        {
            dataValue = *bytesToFollow;
            A_SETLONG(dataList + ii, dataValue);
        }
        outlet_list(xx->fDataSendOut, 0L, numBytesToSend, dataList);
        lockout_set(prevLock);
    }
} // spaceballPerformWriteCommand

/*------------------------------------ spaceballProcessResponse ---*/
void spaceballProcessResponse(SpaceballData * xx,
                              long            rr)
{
    if (xx)
    {
        short         prevLock = lockout_set(1);
        unsigned char incoming = static_cast<unsigned char>(rr & 0x00ff);
        short         slot = xx->fBufferPos;

        if (xx->fNextCharEscaped)
        {
            xx->fNextCharEscaped = false;
            if (incoming != kSpaceballResponseEscape)
            {
                incoming &= 0x001f;
            }
            if (slot < IN_BUFFER_SIZE)
            {
                xx->fBuffer[slot] = incoming;
                ++xx->fBufferPos;
            }
            else
            {
                xx->fBufferPos = 0;
                xx->fSkipping = true;
            }
        }
        else if (kSpaceballResponseEndCommand == incoming)
        {
            if (xx->fSkipping)
            {
                xx->fSkipping = false;
            }
            else if (xx->fBufferPos > 0)
            {
                spaceballProcessPacket(xx);
            }
            xx->fBufferPos = 0;
        }
        else if (kSpaceballResponseNewLine == incoming)
        {
            if (! xx->fSkipping)
            {
                if (slot)
                {
                    if (slot < IN_BUFFER_SIZE)
                    {
                        xx->fBuffer[slot] = incoming;
                        ++xx->fBufferPos;
                    }
                    else
                    {
                        xx->fBufferPos = 0;
                        xx->fSkipping = true;
                    }
                }
            }
        }
        else if (kSpaceballResponseXoff == incoming)
        {
            xx->fOutputBlocked = true;
        }
        else if (kSpaceballResponseXon == incoming)
        {
            xx->fOutputBlocked = false;
        }
        else if (kSpaceballResponseEscape == incoming)
        {
            if (! xx->fSkipping)
            {
                xx->fNextCharEscaped = true;
            }
        }
        else if (! xx->fSkipping)
        {
            if (slot < IN_BUFFER_SIZE)
            {
                xx->fBuffer[slot] = incoming;
                ++xx->fBufferPos;
            }
            else
            {
                xx->fBufferPos = 0;
                xx->fSkipping = true;
            }
        }
        lockout_set(prevLock);
    }
} // spaceballProcessResponse

/*------------------------------------ spaceballSetMode ---*/
bool spaceballSetMode(SpaceballData * xx,
                      t_symbol *      addOrDelta)
{
    bool okSoFar = true;

    if (addOrDelta == gAddSymbol)
    {
        xx->fModeDelta = false;
    }
    else if (addOrDelta == gDeltaSymbol)
    {
        xx->fModeDelta = true;
    }
    else
    {
        LOG_ERROR_2(xx, OUTPUT_PREFIX "unrecognized processing mode (%s)", addOrDelta->s_name)
        okSoFar = false;
    }
    return okSoFar;
} // spaceballSetMode
