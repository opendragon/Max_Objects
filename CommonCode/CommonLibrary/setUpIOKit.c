/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       setUpIOKit.c                                                            */
/*                                                                                      */
/*  Contains:   The routine setUpIOKit().                                               */
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

#include "Common_IOKitX.h"
#include "Common_IOKitXData.h"
#include "loadOtherSegments.h"

#if defined(COMPILE_FOR_CATS) && (! defined(COMPILE_FOR_STUB))
/*------------------------------------ addNotification ---*/
static bool addNotification
	(IOKitContext &							rec,
	 const io_name_t						notificationType,
	 IOServiceMatchingCallback	theCallback,
	 Pvoid											refCon,
 	 io_iterator_t &						notification)
{
	// Set up the add/remove notifications:
	static matchingNotification_FP	pFaddNotification = NULL_PTR;
	
	if (! pFaddNotification)
		pFaddNotification = getFrameworkFunction(matchingNotification_FP, rec.fIOKitBundle,
																							CFSTR("IOServiceAddMatchingNotification"));
	if (pFaddNotification)
	{
  	kern_return_t	kernResult = pFaddNotification(rec.fNotifier, notificationType,
																									rec.fMatchDictionary, theCallback,
																									refCon, &notification);
																								
		return (kernResult == KERN_SUCCESS);
		
	}
	return false;
} /* addNotification */	 
#endif /* COMPILE_FOR_CATS and not COMPILE_FOR_STUB */

#if defined(COMPILE_FOR_CATS) && (! defined(COMPILE_FOR_STUB))
/*------------------------------------ addNotifierToRunLoop ---*/
static bool addNotifierToRunLoop
	(IOKitContext &	rec)
{
	CFStringRef									defaultRunLoopMode;
	CFRunLoopRef								runLoop;
	static getRunLoopSource_FP	pFgetRunLoopSource = NULL_PTR;
																									
	if (! pFgetRunLoopSource)
		pFgetRunLoopSource = getFrameworkFunction(getRunLoopSource_FP, rec.fIOKitBundle,
																							CFSTR("IONotificationPortGetRunLoopSource"));
	if (pFgetRunLoopSource &&
			getRunLoopDefaultMode(rec, defaultRunLoopMode) &&
			getRunLoopCurrent(rec, runLoop))
	{
		if (runLoop)
		{
			// Get the run loop source:
			CFRunLoopSourceRef	runLoopSource = pFgetRunLoopSource(rec.fNotifier);
			
			// Attach the notifier to the run loop:
			runLoopAddSource(rec, runLoop, runLoopSource, defaultRunLoopMode);
			return true;
				
		}
	}
	return false;
} /* addNotifierToRunLoop */
#endif /* COMPILE_FOR_CATS and not COMPILE_FOR_STUB */

#if defined(COMPILE_FOR_CATS) && (! defined(COMPILE_FOR_STUB))
/*------------------------------------ createMatchingDictionary ---*/
static bool createMatchingDictionary
	(IOKitContext &	rec,
	 Pchar					name)
{
	// Set up the match dictionary:
	static serviceMatching_FP	pFserviceMatching = NULL_PTR;
	
	if (! pFserviceMatching)
		pFserviceMatching = getFrameworkFunction(serviceMatching_FP, rec.fIOKitBundle,
																							CFSTR("IOServiceMatching"));																													
	if (pFserviceMatching)
	{
		rec.fMatchDictionary = pFserviceMatching(name);
		return (rec.fMatchDictionary != NULL_PTR);
		
	}
	rec.fMatchDictionary = NULL_PTR;
	return false;
} /* createMatchingDictionary */
#endif /* COMPILE_FOR_CATS and not COMPILE_FOR_STUB */
	 	
#if defined(COMPILE_FOR_CATS) && (! defined(COMPILE_FOR_STUB))
/*------------------------------------ createNotificationPort ---*/
static bool createNotificationPort
	(IOKitContext &	rec)
{
	// Create a notification port:
	static createNotification_FP	pFcreateNotification = NULL_PTR;
	
	if (! pFcreateNotification)
		pFcreateNotification = getFrameworkFunction(createNotification_FP, rec.fIOKitBundle,
																								CFSTR("IONotificationPortCreate"));																																	
	if (pFcreateNotification)
	{
		rec.fNotifier = pFcreateNotification(rec.fMasterPort);
		return (rec.fNotifier != NULL_PTR);
		
	}
	rec.fNotifier = NULL_PTR;
	return false;
} /* createNotificationPort */
#endif /* COMPILE_FOR_CATS and not COMPILE_FOR_STUB */

#if defined(COMPILE_FOR_CATS) && (! defined(COMPILE_FOR_STUB))
/*------------------------------------ getMasterPort ---*/
static bool getMasterPort
	(IOKitContext &	rec)
{
	static ioMasterPort_FP	pFgetMasterPort = NULL_PTR;
	
	if (! pFgetMasterPort)
		pFgetMasterPort = getFrameworkFunction(ioMasterPort_FP, rec.fIOKitBundle,
																						CFSTR("IOMasterPort"));																												
	if (pFgetMasterPort)
	{
		kern_return_t	kernResult = pFgetMasterPort(MACH_PORT_NULL, &rec.fMasterPort);
		
		if (kernResult == KERN_SUCCESS)
			return true;

	}
	rec.fMasterPort = MACH_PORT_NULL;
	return false; 
} /* getMasterPort */
#endif /* COMPILE_FOR_CATS and not COMPILE_FOR_STUB */

#if defined(COMPILE_FOR_CATS)
/*------------------------------------ setUpIOKit ---*/
bool setUpIOKit
	(Pvoid											obj,
	 Pchar											name,
	 IOKitContext &							rec,
	 Pchar											serviceClass,
	 IOServiceMatchingCallback	addFunction,
	 IOServiceMatchingCallback	removeFunction)
{
 #if defined(COMPILE_FOR_STUB)
  #pragma unused(obj,name,rec,serviceClass,addFunction,removeFunction)
 	return false;
 #else /* not COMPILE_FOR_STUB */
	rec.fAddedCallback = rec.fRemovedCallback = NULL_PTR;
	rec.fAddedIterator = rec.fRemovedIterator = NULL_PTR;
	rec.fCoreBundle = rec.fIOKitBundle = NULL_PTR;
	rec.fMasterPort = NULL_PTR;
	rec.fMatchDictionary = NULL_PTR;
	rec.fNotifier = NULL_PTR;
	bool			okSoFar = true;
	OSStatus	err = loadFrameworkBundle(CFSTR("CoreFoundation.framework"),
																			rec.fCoreBundle);

	if (err == noErr)
	{
		err = loadFrameworkBundle(CFSTR("IOKit.framework"), rec.fIOKitBundle);
		if (err == noErr)
			okSoFar = getMasterPort(rec);
		else
		{
	    LOG_ERROR_4("%s: unable to create I/O framework bundle for object, error = %d/0x%x",
	    						name, err, err)
			okSoFar = false;
		}
	}
	else
	{
		LOG_ERROR_4("%s: unable to create core framework bundler for object, error = %d/0x%x",
								name, err, err)
		okSoFar = false;
	}
	if (okSoFar)
	{
		okSoFar = createNotificationPort(rec);
		if (! okSoFar)
			LOG_ERROR_2("%s: unable to create notification port for object", name);
	}
	if (okSoFar)
		okSoFar = addNotifierToRunLoop(rec);
	if (okSoFar)
	{
		okSoFar = createMatchingDictionary(rec, serviceClass);
		if (okSoFar)
		{
			// Add a reference, as we consume one with each call to 'IOServiceAddMatchingNotification'.
			if (addFunction)
				CFRetain(rec.fMatchDictionary);
			if (removeFunction)
				CFRetain(rec.fMatchDictionary);
		}
		else
			LOG_ERROR_2("%s: unable to create matching dictionary for object", name);
	}
	if (okSoFar && addFunction)
	{
		rec.fAddedCallback = createSpringboard(IOServiceMatchingCallback, addFunction);
 		okSoFar = addNotification(rec, kIOFirstPublishNotification, rec.fAddedCallback, obj,
															rec.fAddedIterator);
		if (! okSoFar)
			LOG_ERROR_2("%s: unable to install 'add' notification for object", name);
	}
	if (okSoFar && removeFunction)
	{
		rec.fRemovedCallback = createSpringboard(IOServiceMatchingCallback, removeFunction);
		okSoFar = addNotification(rec, kIOTerminatedNotification, rec.fRemovedCallback, obj,
															rec.fRemovedIterator);
		if (! okSoFar)
			LOG_ERROR_2("%s: unable to install 'remove' notification for object", name);
	}
	rec.fActive = okSoFar;
	return okSoFar;
 #endif /* not COMPILE_FOR_STUB */
} /* setUpIOKit */
#endif /* COMPILE_FOR_CATS */
	 
#if defined(COMPILE_FOR_CATS) && defined(COMPILE_FOR_STUB)
 #pragma export list setUpIOKit
#endif /* COMPILE_FOR_CATS and COMPILE_FOR_STUB */