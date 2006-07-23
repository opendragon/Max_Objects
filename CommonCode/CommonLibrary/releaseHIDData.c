/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       releaseHIDData.c                                                        */
/*                                                                                      */
/*  Contains:   The routine releaseHIDData().                                           */
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

#if (! defined(COMPILE_FOR_STUB))
/*------------------------------------ clearDeviceData ---*/
static void clearDeviceData
	(HIDDeviceDataStruct &	thisDevice)
{
	for (HIDElementDataPtr walker = thisDevice.fFirstElement, next; walker;
				walker = next)
	{
		next = walker->fNext;
		FREEBYTES(walker, 1);
	}
	thisDevice.fFirstElement = thisDevice.fLastElement = NULL_PTR;
	if (thisDevice.fPrevious)
		thisDevice.fPrevious->fNext = thisDevice.fNext;
	if (thisDevice.fNext)
		thisDevice.fNext->fPrevious = thisDevice.fPrevious;
	thisDevice.fPrevious = thisDevice.fNext = NULL_PTR;
 #if defined(COMPILE_FOR_OSX_4)
	thisDevice.fInterface = NULL_PTR;
	if (thisDevice.fInterestCallback)
		releaseCallback(thisDevice.fInterestCallback);
	if (thisDevice.fInputCallback)
		releaseCallback(thisDevice.fInputCallback);
 #endif /* COMPILE_FOR_OSX_4 */
} /* clearDeviceData */
#endif /* not COMPILE_FOR_STUB */

#if defined(COMPILE_FOR_OSX_4)
/*------------------------------------ releaseHIDData ---*/
void releaseHIDData
	(IOKitContext &			rec,
	 HIDDeviceDataPtr &	thisDevice)
{
 #if defined(COMPILE_FOR_STUB)
  #pragma unused(rec,thisDevice)
 #else /* not COMPILE_FOR_STUB */
	if (thisDevice)
	{
		if (thisDevice->fInputQueue)
		{
			IOReturn						result;
			TransferVector_rec	funkChunk;
			hidqDispose_FP			pFdispose;
			release_FP					pFrelease;
			hidqStop_FP					pFstop;

			if (thisDevice->fInputQueueRunloop)
			{
				CFStringRef		defaultRunLoopMode;
				CFRunLoopRef	runLoop;
				
				if (getRunLoopDefaultMode(rec, defaultRunLoopMode) &&
						getRunLoopCurrent(rec, runLoop))
				{
					bool	doesContain = false;
					
					if (runLoopContainsSource(rec, runLoop, thisDevice->fInputQueueRunloop,
																		defaultRunLoopMode, doesContain) && doesContain)
						runLoopRemoveSource(rec, runLoop, thisDevice->fInputQueueRunloop,
																defaultRunLoopMode);
				}
				CFRelease(thisDevice->fInputQueueRunloop);
				thisDevice->fInputQueueRunloop = NULL_PTR;
			}		
			pFstop = fillCallback(hidqStop_FP, funkChunk, (*thisDevice->fInputQueue)->stop);
			result = pFstop(thisDevice->fInputQueue);
			pFdispose = fillCallback(hidqDispose_FP, funkChunk,
																(*thisDevice->fInputQueue)->dispose);
			result = pFdispose(thisDevice->fInputQueue);
			pFrelease = fillCallback(release_FP, funkChunk,
																(*thisDevice->fInputQueue)->Release);
			ULONG	returnVal = pFrelease(thisDevice->fInputQueue);
			
			thisDevice->fInputQueue = NULL_PTR;
		}
		if (thisDevice->fInterface)
		{
			closeHIDDevice(thisDevice->fInterface);
			releaseHIDDevice(thisDevice->fInterface);
		}
		clearDeviceData(*thisDevice);
		FREEBYTES(thisDevice->fPrivate, thisDevice->fPrivateSize);
		FREEBYTES(thisDevice->fRefCon, thisDevice->fRefConSize);
		FREEBYTES(thisDevice, 1);
		thisDevice = NULL_PTR;
	}
 #endif /* not COMPILE_FOR_STUB */
} /* releaseHIDData */
#endif /* COMPILE_FOR_OSX_4 */

#if defined(COMPILE_FOR_OS9_4)
/*------------------------------------ releaseHIDData ---*/
void releaseHIDData
	(HIDDeviceDataPtr &	thisDevice)
{
 #if defined(COMPILE_FOR_STUB)
  #pragma unused(thisDevice)
 #else /* not COMPILE_FOR_STUB */
	if (thisDevice)
	{
		clearDeviceData(*thisDevice);
		if (thisDevice->fPrepReport)
			HIDCloseReportDescriptor(thisDevice->fPrepReport);
		if (thisDevice->fConnection)
			(*thisDevice->fDispatchTable->pHIDCloseDevice)(thisDevice->fConnection);
		FREEBYTES(thisDevice->fPrivate, thisDevice->fPrivateSize);
		FREEBYTES(thisDevice, 1);
		thisDevice = NULL_PTR;
	}
 #endif /* not COMPILE_FOR_STUB */
} /* releaseHIDData */
#endif /* COMPILE_FOR_OS9_4 */

#if defined(COMPILE_FOR_STUB)
 #pragma export list initHIDElementData,initHIDDeviceData
#endif /* COMPILE_FOR_STUB */