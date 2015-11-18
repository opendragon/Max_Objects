/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       senseX.cpp                                                              */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the senseX module.        */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2006 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2006/01/12                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#define IS_MAIN /* */
#include "senseX.h"
#include "reportVersion.h"

/* Forward references: */
void * senseXCreate(long onDelay,
                    long offDelay);

void senseXFree(SenseXData * xx);

void senseXProcessClock(SenseXData * xx);

void senseXProcessQueue(SenseXData * xx);

/*------------------------------------ main ---*/
int main(void)
{
    /* Allocate class memory and set up class. */
    t_class * temp = class_new(OUR_NAME, reinterpret_cast<method>(senseXCreate), reinterpret_cast<method>(senseXFree),
                               sizeof(SenseXData), reinterpret_cast<method>(0L), A_LONG, A_LONG, 0);

    if (temp)
    {
        class_addmethod(temp, reinterpret_cast<method>(cmd_Anything), MESSAGE_ANYTHING, A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Assist), MESSAGE_ASSIST, A_CANT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Bang), MESSAGE_BANG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Float), MESSAGE_FLOAT, A_FLOAT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Int), MESSAGE_INT, A_LONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Off), "off", A_LONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_On), "on", A_LONG, 0);
        class_register(CLASS_BOX, temp);
    }
    gClass = temp;
    gEmptySymbol = gensym("");
    reportVersion(OUR_NAME);
    return 0;
} // main
/*------------------------------------ senseXCreate ---*/
void * senseXCreate(long onDelay,
                    long offDelay)
{
    SenseXData * xx = static_cast<SenseXData *>(object_alloc(gClass));

    if (xx)
    {
        if (offDelay < 0)
        {
            LOG_ERROR_1(xx, OUTPUT_PREFIX "off delay is not valid")
            offDelay = 0;
        }
        if (onDelay < 0)
        {
            LOG_ERROR_1(xx, OUTPUT_PREFIX "on delay is not valid")
            onDelay = 0;
        }
        xx->fOffDelay = offDelay;
        xx->fOnDelay = onDelay;
        xx->fStopping = false;
        xx->fPollClock = static_cast<t_clock *>(clock_new(xx, reinterpret_cast<method>(senseXProcessClock)));
        xx->fPollQueue = static_cast<t_qelem *>(qelem_new(xx, reinterpret_cast<method>(senseXProcessQueue)));
        xx->fResultOut = static_cast<t_outlet *>(intout(xx));
        xx->fProxy = proxy_new(xx, 1L, &xx->fInletNumber);
        xx->fState = kAwaitingFirstMessage;
        if (! (xx->fResultOut || xx->fProxy))
        {
            LOG_ERROR_1(xx, OUTPUT_PREFIX "unable to create port for object")
            freeobject(reinterpret_cast<t_object *>(xx));
            xx = NULL_PTR;
        }
    }
    return xx;
} // senseXCreate
/*------------------------------------ senseXDoMessage ---*/
void senseXDoMessage(SenseXData * xx)
{
    if (! xx->fStopping)
    {
        short prevLock;

        switch (xx->fState)
        {
            case kAwaitingFirstMessage:
                prevLock = lockout_set(1);
                if (xx->fOnDelay > 0)
                {
                    clock_delay(xx->fPollClock, xx->fOnDelay);
                    xx->fState = kBetweenMessages;
                }
                else
                {
                    outlet_int(xx->fResultOut, 1);
                    if (xx->fOffDelay > 0)
                    {
                        clock_delay(xx->fPollClock, xx->fOffDelay);
                        xx->fState = kAfterSecondMessage;
                    }
                    else
                    {
                        outlet_int(xx->fResultOut, 0);
                        xx->fState = kAwaitingFirstMessage;
                    }
                }
                lockout_set(prevLock);
                break;

            case kBetweenMessages:
                prevLock = lockout_set(1);
                outlet_int(xx->fResultOut, 1);
                clock_unset(xx->fPollClock);
                if (xx->fOffDelay > 0)
                {
                    clock_delay(xx->fPollClock, xx->fOffDelay);
                    xx->fState = kAfterSecondMessage;
                }
                else
                {
                    outlet_int(xx->fResultOut, 0);
                    xx->fState = kAwaitingFirstMessage;
                }
                lockout_set(prevLock);
                break;

            default:
                break;
        }
    }
} // senseXDoMessage
/*------------------------------------ senseXFree ---*/
void senseXFree(SenseXData * xx)
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
    }
} // senseXFree
/*------------------------------------ senseXProcessClock ---*/
void senseXProcessClock(SenseXData * xx)
{
    if (xx && (! xx->fStopping))
    {
        qelem_set(xx->fPollQueue);
    }
} // senseXProcessClock
/*------------------------------------ senseXProcessQueue ---*/
void senseXProcessQueue(SenseXData * xx)
{
    if (xx && (! xx->fStopping))
    {
        short prevLock;

        switch (xx->fState)
        {
            case kBetweenMessages:
                prevLock = lockout_set(1);
                xx->fState = kAwaitingFirstMessage;
                lockout_set(prevLock);
                break;

            case kAfterSecondMessage:
                prevLock = lockout_set(1);
                outlet_int(xx->fResultOut, 0);
                xx->fState = kAwaitingFirstMessage;
                lockout_set(prevLock);
                break;

            default:
                break;
        }
        evnum_incr();
    }
} // senseXProcessQueue
