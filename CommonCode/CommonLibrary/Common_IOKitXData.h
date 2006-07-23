/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       Common_IOKitXData.h                                                     */
/*                                                                                      */
/*  Contains:   The data type declarations and forward references to routines.          */
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

#if (! defined(COMMON_IOKITXDATA_H_))
 #define COMMON_IOKITXDATA_H_ /* */

 #if defined(COMPILE_FOR_OSX_4)
 // The following datatypes are copied from the relevant Framework headers, which aren't directly
 // useable in CFM source:
 
  #define IOCFPLUGINBASE							\
    UInt16	version;						\
    UInt16	revision;						\
    IOReturn (*Probe)(void *thisPointer, CFDictionaryRef propertyTable,	\
                    io_service_t service, SInt32 * order);		\
    IOReturn (*Start)(void *thisPointer, CFDictionaryRef propertyTable,	\
                      io_service_t service);				\
    IOReturn (*Stop)(void *thisPointer)

struct IOCFPlugInInterface
{
	IUNKNOWN_C_GUTS;
	IOCFPLUGINBASE;
}; /* IOCFPlugInInterface */
// End of copies ...

typedef kern_return_t (* addInterestNotification_FP)
	(IONotificationPortRef			notifyPort,
   io_service_t								service,
	 const io_name_t						interestType,
   IOServiceInterestCallback	callback,
   Pvoid											refCon,
   io_object_t *							notification);

typedef void (* addRemoveLoopSource_FP)
	(CFRunLoopRef				rl,
	 CFRunLoopSourceRef	source,
	 CFStringRef				mode);

typedef IONotificationPortRef (* createNotification_FP)
	(mach_port_t	masterPort); 

typedef kern_return_t (* createPlugInInterface_FP)
	(io_service_t								service,
   CFUUIDRef									pluginType,
   CFUUIDRef									interfaceType,
   IOCFPlugInInterface * * *	theInterface,
   SInt32 *										theScore);

typedef void (* destroyNotification_FP)
	(IONotificationPortRef	notify); 

typedef kern_return_t (* getMatchingServices_FP)
	(mach_port_t			masterPort,
	 CFDictionaryRef	matching,
	 io_iterator_t *	existing);

typedef CFRunLoopRef (* getRunLoop_FP)
	(void);

typedef CFRunLoopSourceRef (* getRunLoopSource_FP)
	(IONotificationPortRef	notify); 

typedef kern_return_t (* ioMasterPort_FP)
	(mach_port_t		bootstrapPort,
	 mach_port_t *	masterPort);

typedef kern_return_t (* matchingNotification_FP)
	(IONotificationPortRef			notifyPort,
 	 const io_name_t						notificationType,
 	 CFDictionaryRef						matching,
 	 IOServiceMatchingCallback	callback,
 	 Pvoid											refCon,
 	 io_iterator_t *						notification); 
	 
typedef HRESULT (* queryInterface_FP)
	(Pvoid *	thisPointer,
	 REFIID		iid,
	 LPVOID *	ppv);

typedef kern_return_t (* registryEntryCreateCFProperties_FP)
	(io_registry_entry_t			entry,
	 CFMutableDictionaryRef * properties,
	 CFAllocatorRef						allocator,
	 IOOptionBits							options);
	 
typedef CFTypeRef (* registryEntryCreateCFProperty_FP)
	(io_registry_entry_t	entry,
	 CFStringRef					key,
   CFAllocatorRef				allocator,
	 IOOptionBits					options);
	 
typedef kern_return_t (* registryEntryGetName_FP)
	(io_registry_entry_t	entry,
	 io_name_t						name );
	 
typedef Boolean (* runLoopContainsSource_FP)
	(CFRunLoopRef				rl,
	 CFRunLoopSourceRef	source,
	 CFStringRef				mode);

typedef SInt32 (* runLoopRunInMode_FP)
	(CFStringRef		mode,
	 CFTimeInterval	seconds,
	 Boolean				returnAfterSourceHandled);

typedef CFMutableDictionaryRef (* serviceMatching_FP)
	(Qchar	name); 

 #endif /* COMPILE_FOR_OSX_4 */

#endif /* not COMMON_IOKITXDATA_H_ */
