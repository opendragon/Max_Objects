/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       setUpHIDInputQueue.c                                                    */
/*                                                                                      */
/*  Contains:   The routine setUpHIDInputQueue().                                       */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2004 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2004/02/09                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "Common_HIDX.h"
#include "Common_HIDXData.h"
#include "loadOtherSegments.h"

#if defined(COMPILE_FOR_OSX_4) && (! defined(COMPILE_FOR_STUB))
/*------------------------------------ doInputEventHandler ---*/
static void doInputEventHandler
	(Pvoid		target,
	 IOReturn	result,
	 Pvoid		refcon,
	 Pvoid		sender)
{
 #pragma unused(result,sender)
LOG_POST_1("doInputEventHandler")//!!
	TransferVector_rec	funkChunk;
	HIDDeviceDataPtr		thisDevice = reinterpret_cast<HIDDeviceDataPtr>(target);
	IOHIDEventStruct		event;
	AbsoluteTime				maxTime = { 0, 0 };
	hidqGetNextEvent_FP	pFgetNextEvent;
	
	pFgetNextEvent = fillCallback(hidqGetNextEvent_FP, funkChunk,
																(*thisDevice->fInputQueue)->getNextEvent);
	pFgetNextEvent(thisDevice->fInputQueue, &event, maxTime, 0);
	if (thisDevice->fInputFunction)
		thisDevice->fInputFunction(refcon, event.elementCookie, event.value,
																event.longValueSize, event.longValue);
} /* doInputEventHandler */
#endif /* COMPILE_FOR_OSX_4 and not COMPILE_FOR_STUB */

#if defined(COMPILE_FOR_OSX_4)
/*------------------------------------ setUpHIDInputQueue ---*/
void setUpHIDInputQueue
	(Pchar									name,
	 IOKitContext &					rec,
	 HIDDeviceDataStruct &	thisDevice,
	 InputEventHandler			aFun,
	 IOHIDElementCookie *		cookies,
	 int										numCookies)
{
 #if defined(COMPILE_FOR_STUB)
  #pragma unused(name,rec,thisDevice,aFun,cookies,numCookies)
 #else /* not COMPILE_FOR_STUB */
	IOReturn						result;
	TransferVector_rec	funkChunk;
	hidAllocQueue_FP		pFallocQueue;
	
	thisDevice.fInputFunction = aFun;
	pFallocQueue = fillCallback(hidAllocQueue_FP, funkChunk, (*thisDevice.fInterface)->allocQueue);
	thisDevice.fInputQueue = pFallocQueue(thisDevice.fInterface);
	if (thisDevice.fInputQueue)
	{
		hidqCreate_FP	pFcreate;
		
		pFcreate = fillCallback(hidqCreate_FP, funkChunk, (*thisDevice.fInputQueue)->create);
		result = pFcreate(thisDevice.fInputQueue, 0, MAX_QUEUE_DEPTH);
		if (result == KERN_SUCCESS)
		{
			TransferVector_rec	funkChunk2;
			hidqAddElement_FP		pFaddElement;
			hidqHasElement_FP		pFhasElement;
			
			pFaddElement = fillCallback(hidqAddElement_FP, funkChunk,
																	(*thisDevice.fInputQueue)->addElement);
			pFhasElement = fillCallback(hidqHasElement_FP, funkChunk2,
																	(*thisDevice.fInputQueue)->hasElement);														
			for (int	ii = 0; ii < numCookies; ++ii)
			{
LOG_POST_3("cookie %d: %d", ii, long(*(cookies + ii)))//!!
				if (! pFhasElement(thisDevice.fInputQueue, *(cookies + ii)))
				{
					result = pFaddElement(thisDevice.fInputQueue, *(cookies + ii), 0);
					if (result != KERN_SUCCESS)
					{
						LOG_POST_4("%s: problem adding element to input queue (%d/0x%x)", name, result, result)
						break;
						
					}
				}
			}
			if (result == KERN_SUCCESS)
			{
				if (! thisDevice.fInputQueueRunloop)
				{
					hidqCreateAsyncEventSource_FP	pFcreateAsyncEventSource;
					
					pFcreateAsyncEventSource = fillCallback(hidqCreateAsyncEventSource_FP, funkChunk,
																									(*thisDevice.fInputQueue)->createAsyncEventSource);
					result = pFcreateAsyncEventSource(thisDevice.fInputQueue, &thisDevice.fInputQueueRunloop);
					if (result != KERN_SUCCESS)
						LOG_POST_4("%s: problem creating async event source for input queue (%d/0x%x)", name, result, result)
				}
			}
			// Check if the queue runloop source is already attached to our runloop
			if (result == KERN_SUCCESS)
			{
				CFStringRef		defaultRunLoopMode;
				CFRunLoopRef	runLoop;
				
				if (getRunLoopDefaultMode(rec, defaultRunLoopMode) &&
						getRunLoopCurrent(rec, runLoop))
				{
					bool	doesContain = false;
					
					if (runLoopContainsSource(rec, runLoop, thisDevice.fInputQueueRunloop,
																			defaultRunLoopMode, doesContain) && (! doesContain))
						runLoopAddSource(rec, runLoop, thisDevice.fInputQueueRunloop, defaultRunLoopMode);
				}
			}
			// Install the callback
			if (result == KERN_SUCCESS)
			{
				hidqSetEventCallout_FP	pFsetEventCallout;
				
				pFsetEventCallout = fillCallback(hidqSetEventCallout_FP, funkChunk,
																					(*thisDevice.fInputQueue)->setEventCallout);
				thisDevice.fInputCallback = createSpringboard(IOHIDCallbackFunction,
																											doInputEventHandler);																							
				result = pFsetEventCallout(thisDevice.fInputQueue, thisDevice.fInputCallback,
																		&thisDevice, thisDevice.fRefCon);
				if (result != KERN_SUCCESS)
					LOG_POST_4("%s: problem setting event handler for input queue (%d/0x%x)", name, result, result)
			}
			if (result == KERN_SUCCESS)
			{
				hidqStart_FP	pFstart;
				
				pFstart = fillCallback(hidqStart_FP, funkChunk, (*thisDevice.fInputQueue)->start);
				result = pFstart(thisDevice.fInputQueue);
				if (result != KERN_SUCCESS)
					LOG_POST_4("%s: problem starting input queue (%d/0x%x)", name, result, result)
			}
		}
		else
			LOG_POST_4("%s: problem creating input queue (%d/0x%x)", name, result, result)
	}
	else
		LOG_POST_2("%s: could not allocate input queue", name)
 #endif /* not COMPILE_FOR_STUB */
} /* setUpHIDInputQueue */
#endif /* COMPILE_FOR_OSX_4 */

#if defined(COMPILE_FOR_OSX_4) && defined(COMPILE_FOR_STUB)
 #pragma export list setUpHIDInputQueue
#endif /* COMPILE_FOR_OSX_4 and COMPILE_FOR_STUB */