/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       Common_IOKitX.h                                                         */
/*                                                                                      */
/*  Contains:   The data type declarations and forward references to routines.          */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2003 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2003/11/10                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(COMMON_IOKITX_H_))
 #define COMMON_IOKITX_H_ /* */

 #include "MissingAndExtra.h"

 #if defined(COMPILE_FOR_OSX_4)
  #include <mach/mach_types.h>
  #include <mach/error.h>
  #include <CFPlugInCOM.h>

// The following datatypes are copied from the relevant Framework headers, which aren't directly
// useable in CFM source:

  #define IOKIT /* */
typedef mach_port_t	io_object_t;

  #include <device/device_types.h>

typedef io_object_t	io_connect_t;
typedef io_object_t	io_iterator_t;
typedef io_object_t	io_registry_entry_t;
typedef io_object_t	io_service_t;
typedef io_object_t	io_enumerator_t;

typedef kern_return_t	IOReturn;
typedef UInt32				IOMessage;

  #ifndef sys_iokit
   #define sys_iokit                    err_system(0x38)
  #endif /* sys_iokit */

  #define sub_iokit_common							err_sub(0)

  #define iokit_common_msg(message)     (UInt32)(sys_iokit|sub_iokit_common|message)
	#define	iokit_common_err(return)			(sys_iokit|sub_iokit_common|return)

  #define kIOMessageServiceIsTerminated	iokit_common_msg(0x010)
	#define kIOReturnAborted							iokit_common_err(0x2eb) // operation aborted

typedef struct IONotificationPort * IONotificationPortRef; // from IOKitLib.h

typedef io_object_t (* IOIteratorNext_FP)
	(io_iterator_t iterator);

typedef kern_return_t (* IOObjectRelease_FP)
	(io_object_t object);

typedef void (* IOServiceInterestCallback) // from IOKitLib.h
	(Pvoid					refCon,
	 io_service_t		service,
	 natural_t			messageType,
	 Pvoid					messageArgument);
	 
typedef void (* IOServiceMatchingCallback) // from IOKitLib.h
	(Pvoid					refcon,
	 io_iterator_t	iterator);
	 
// IOService notification types from IOKitKeys.h
  #define kIOPublishNotification			"IOServicePublish"
  #define kIOFirstPublishNotification	"IOServiceFirstPublish"
  #define kIOMatchedNotification			"IOServiceMatched"
  #define kIOFirstMatchNotification		"IOServiceFirstMatch"
  #define kIOTerminatedNotification		"IOServiceTerminate"
  #define kIOGeneralInterest					"IOGeneralInterest"

typedef UInt32				IOOptionBits;
typedef	kern_return_t	IOReturn;

/* C244E858-109C-11D4-91D4-0050E4C6426F */
  #define kIOCFPlugInInterfaceID CFUUIDGetConstantUUIDWithBytes(NULL,	\
    0xC2, 0x44, 0xE8, 0x58, 0x10, 0x9C, 0x11, 0xD4,			\
    0x91, 0xD4, 0x00, 0x50, 0xE4, 0xC6, 0x42, 0x6F)

struct IOCFPlugInInterface;

// copied from CFRunLoop.h:
typedef struct __CFRunLoop * 				CFRunLoopRef;
typedef struct __CFRunLoopSource *	CFRunLoopSourceRef;

enum
{
	kCFRunLoopRunFinished = 1,
	kCFRunLoopRunStopped = 2,
	kCFRunLoopRunTimedOut = 3,
	kCFRunLoopRunHandledSource = 4
};

// End of copies ...

 #endif /* COMPILE_FOR_OSX_4 */

 #if defined(COMPILE_FOR_OSX_4)
typedef ULONG (* release_FP)
    (Pvoid	thisPointer);

struct IOKitContext
{
  bool											fActive;
  IOServiceMatchingCallback	fAddedCallback;
  io_iterator_t							fAddedIterator;
  CFBundleRef								fCoreBundle;  
	CFBundleRef								fIOKitBundle;
	mach_port_t								fMasterPort;
	CFMutableDictionaryRef		fMatchDictionary;
	IONotificationPortRef			fNotifier;
	IOServiceMatchingCallback	fRemovedCallback;
	io_iterator_t							fRemovedIterator;
}; /* IOKitContext */

 #endif /* COMPILE_FOR_OSX_4 */

 #if defined(COMPILE_FOR_OSX_4)

bool addInterestNotification
	(IOKitContext &							rec,
	 io_object_t								object,
	 const io_name_t						interestType,
	 IOServiceInterestCallback	callback,
	 Pvoid											refCon,
	 io_object_t &							notification,
   kern_return_t &						kernResult);
	 
bool createPlugInInterface
	(IOKitContext &							rec,
	 io_service_t								service,
   CFUUIDRef									pluginType,
   CFUUIDRef									interfaceType,
   IOCFPlugInInterface * * &	theInterface,
   SInt32 &										theScore,
   kern_return_t &						kernResult);
   
bool getNextServiceObject
	(IOKitContext &		rec,
	 io_iterator_t		walker,
	 io_service_t &		object);

bool getObjectName
	(IOKitContext &	rec,
	 io_service_t		object,
	 io_name_t			objName);

bool getObjectProperties
	(IOKitContext &						rec,
	 io_service_t							object,
	 CFMutableDictionaryRef &	dict,
	 kern_return_t						result);
	 
bool getObjectProperty
	(IOKitContext &	rec,
	 io_service_t		object,
	 CFStringRef		propName,
	 CFTypeRef &		result);

bool getRunLoopCurrent
	(IOKitContext &	rec,
	 CFRunLoopRef &	runLoop);
	 
bool getRunLoopDefaultMode
	(IOKitContext &	rec,
	 CFStringRef &	result);
	 
bool getServiceObjects
	(Pchar						name,
	 IOKitContext &		rec,
	 io_iterator_t &	walker);

HRESULT queryPlugInInterface
	(IOCFPlugInInterface * *	theInterface,
	 REFIID										iid,
	 LPVOID *									ppv);
	 
void releasePlugInInterface
	(IOCFPlugInInterface * *	theInterface,
	 ULONG &									result);
	 
bool releaseServiceObject
	(Pchar					name,
	 IOKitContext &	rec,
	 io_service_t &	object);

bool runLoopAddSource
	(IOKitContext &			rec,
	 CFRunLoopRef				runLoop,
	 CFRunLoopSourceRef	runLoopSource,
	 CFStringRef				runLoopMode);
	 	 
bool runLoopContainsSource
	(IOKitContext &			rec,
	 CFRunLoopRef				runLoop,
	 CFRunLoopSourceRef	runLoopSource,
	 CFStringRef				runLoopMode,
	 bool &							result);
	 
bool runLoopRemoveSource
	(IOKitContext &			rec,
	 CFRunLoopRef				runLoop,
	 CFRunLoopSourceRef	runLoopSource,
	 CFStringRef				runLoopMode);
	 	 
SInt32 runLoopRunInMode
	(IOKitContext &	rec,
	 CFStringRef		runLoopMode,
	 CFTimeInterval	seconds,
	 const bool			returnAfterSourceHandled);

bool setUpIOKit
	(Pvoid											obj,
	 Pchar											name,
	 IOKitContext &							rec,
	 Pchar											serviceClass,
	 IOServiceMatchingCallback	addFunction,
	 IOServiceMatchingCallback	removeFunction);

bool shutDownIOKit
	(IOKitContext &	rec);

void triggerIterators
	(Pvoid											obj,
	 IOKitContext &							rec,
	 IOServiceMatchingCallback	addFunction,
	 IOServiceMatchingCallback	removeFunction);
	  
 #endif /* COMPILE_FOR_OSX_4 */

#endif /* not COMMON_IOKITX_H_ */