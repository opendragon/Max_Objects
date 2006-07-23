/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       Common_USBX.h                                                           */
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
/*  Created:    2003/11/11                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(COMMON_USBX_H_))
 #define COMMON_USBX_H_ /* */

 #include "Common_IOKitX.h"

 #if defined(COMPILE_FOR_OSX_4)
// The following datatypes and defines are copied from the relevant Framework headers,
// which aren't directly useable in CFM source:
  #define kIOUSBDeviceClassName					"IOUSBDevice"
  #define kIOUSBInterfaceClassName			"IOUSBInterface"
  #define kUSBVendorID									"idVendor"
  #define kUSBProductID									"idProduct"
  #define kUSBDevicePropertyAddress			"USB Address"
  #define kUSBDevicePropertyLocationID	"locationID"
  #define kUSBDeviceNumConfigs					"bNumConfigurations"
  #define kUSBNumEndpoints							"bNumEndpoints"

// 5c8187d0-9ef3-11d4-8b45-000a27052861
  #define kIOUSBDeviceInterfaceID CFUUIDGetConstantUUIDWithBytes(NULL,	\
    0x5c, 0x81, 0x87, 0xd0, 0x9e, 0xf3, 0x11, 0xD4,			\
    0x8b, 0x45, 0x00, 0x0a, 0x27, 0x05, 0x28, 0x61)
// 9dc7b780-9ec0-11d4-a54f-000a27052861
  #define kIOUSBDeviceUserClientTypeID CFUUIDGetConstantUUIDWithBytes(NULL,	\
    0x9d, 0xc7, 0xb7, 0x80, 0x9e, 0xc0, 0x11, 0xD4,			\
    0xa5, 0x4f, 0x00, 0x0a, 0x27, 0x05, 0x28, 0x61)
// 73c97ae8-9ef3-11d4-b1d0-000a27052861
  #define kIOUSBInterfaceInterfaceID CFUUIDGetConstantUUIDWithBytes(NULL,	\
    0x73, 0xc9, 0x7a, 0xe8, 0x9e, 0xf3, 0x11, 0xD4,			\
    0xb1, 0xd0, 0x00, 0x0a, 0x27, 0x05, 0x28, 0x61)
// 2d9786c6-9ef3-11d4-ad51-000a27052861
  #define kIOUSBInterfaceUserClientTypeID CFUUIDGetConstantUUIDWithBytes(NULL,	\
    0x2d, 0x97, 0x86, 0xc6, 0x9e, 0xf3, 0x11, 0xD4,			\
    0xad, 0x51, 0x00, 0x0a, 0x27, 0x05, 0x28, 0x61)

typedef void (* IOAsyncCallback1)
	(Pvoid		refcon,
	 IOReturn	result,
	 Pvoid		arg0);
	 
struct IOUSBFindInterfaceRequest
{
	UInt16	bInterfaceClass;		// requested class
	UInt16 	bInterfaceSubClass;		// requested subclass
	UInt16 	bInterfaceProtocol;		// requested protocol
	UInt16	bAlternateSetting;		// requested alt setting
}; /* IOUSBFindInterfaceRequest */

enum
{
	kIOUSBFindInterfaceDontCare	= 0xFFFF
};

enum
{
	kUSBHIDClass	= 3
};

struct IOUSBDevRequest
{
	UInt8		bmRequestType;
	UInt8		bRequest;
	UInt16	wValue;
	UInt16	wIndex;
	UInt16	wLength;
	Pvoid		pData;
	UInt32	wLenDone;
}; /* IOUSBDevRequest */

struct IOUSBDevRequestTO
{
	UInt8		bmRequestType;
	UInt8		bRequest;
	UInt16	wValue;
	UInt16	wIndex;
	UInt16	wLength;
	Pvoid		pData;
	UInt32	wLenDone;
	UInt32	noDataTimeout;
	UInt32	completionTimeout;
}; /* IOUSBDevRequestTO */

enum
{
	kUSBControl     = 0,
	kUSBIsoc        = 1,
	kUSBBulk        = 2,
	kUSBInterrupt   = 3,
	kUSBAnyType     = 0xFF
};

enum
{
	kUSBOut         = 0,
	kUSBIn          = 1,
	kUSBNone        = 2,
	kUSBAnyDirn     = 3
};

enum
{
	kUSBStandard    = 0,
	kUSBClass       = 1,
	kUSBVendor      = 2
};

enum
{
	kUSBDevice      = 0,
	kUSBInterface   = 1,
	kUSBEndpoint    = 2,
	kUSBOther       = 3
};

enum
{
	kUSBRqGetStatus     = 0,
	kUSBRqClearFeature  = 1,
	kUSBRqGetState      = 2,
	kUSBRqSetFeature    = 3,
	kUSBRqReserved2     = 4,
	kUSBRqSetAddress    = 5,
	kUSBRqGetDescriptor = 6,
	kUSBRqSetDescriptor = 7,
	kUSBRqGetConfig     = 8,
	kUSBRqSetConfig     = 9,
	kUSBRqGetInterface  = 10,
	kUSBRqSetInterface  = 11,
	kUSBRqSyncFrame     = 12
};

struct IOUSBDeviceInterface;

struct IOUSBInterfaceInterface;

 // End of copies ...

  #define kUSBSerialNumber	"USB Serial Number"
 #endif /* COMPILE_FOR_OSX_4 */
 #if defined(COMPILE_FOR_OS9_4)
  #include "usb.h"
  
struct USBContext
{
  bool		fActive;
 	UInt32	fNotificationDeviceToken;
 	bool		fNotificationDeviceTokenValid;
 	Pvoid		fOwner;
}; /* USBContext */ 

 #endif /* COMPILE_FOR_OS9_4 */
 
 #if defined(COMPILE_FOR_OSX_4)
IOReturn abortUSBPipe
	(IOUSBInterfaceInterface * *	theInterface,
	 const UInt8									pipeRef);

IOReturn closeUSBDevice
	(IOUSBDeviceInterface * *	theInterface);

IOReturn closeUSBInterface
	(IOUSBInterfaceInterface * *	theInterface);

IOReturn configureUSBDevice
	(IOUSBDeviceInterface * *	theInterface,
	 const UInt8							configIndex);

IOReturn createUSBInterfaceAsyncEventSource
	(IOUSBInterfaceInterface * *	theInterface,
	 CFRunLoopSourceRef &					source);
	 
IOReturn createUSBInterfaceIterator
	(IOUSBDeviceInterface * *			theInterface,
	 IOUSBFindInterfaceRequest &	request,
	 io_iterator_t &							iter);

IOReturn getUSBDeviceProduct
	(IOUSBDeviceInterface * *	theInterface,
   UInt16 &									devProduct);

IOReturn getUSBDeviceReleaseNumber
	(IOUSBDeviceInterface * *	theInterface,
   UInt16 &									devRelNum);

IOReturn getUSBDeviceVendor
	(IOUSBDeviceInterface * *	theInterface,
   UInt16 &									devVendor);
   
CFRunLoopSourceRef getUSBInterfaceAsyncEventSource
	(IOUSBInterfaceInterface * *	theInterface);

IOReturn getUSBInterfaceClass
	(IOUSBInterfaceInterface * *	theInterface,
   UInt8 &											intfClass);
	
IOReturn getUSBInterfaceSubClass
	(IOUSBInterfaceInterface * *	theInterface,
   UInt8 &											intfSubClass);

IOReturn getUSBNumberOfConfigurations
	(IOUSBDeviceInterface * *	theInterface,
	 UInt8 &									numConfigs);
									
IOReturn openUSBDevice
	(IOUSBDeviceInterface * *	theInterface);

IOReturn openUSBInterface
	(IOUSBInterfaceInterface * *	theInterface);

IOReturn readUSBPipe
	(IOUSBInterfaceInterface * *	theInterface,
	 const UInt8									pipeRef,
   Pvoid												buffer,
   UInt32 &											buffSize);

IOReturn readUSBPipeAsync
	(IOUSBInterfaceInterface * *	theInterface,
	 const UInt8									pipeRef,
	 Pvoid												buf,
	 const UInt32									size,
	 IOAsyncCallback1							callback,
	 Pvoid												refCon);
	 
IOReturn readUSBPipeAsyncTO
	(IOUSBInterfaceInterface * *	theInterface,
	 const UInt8									pipeRef,
	 Pvoid												buf,
	 const UInt32									size,
	 const UInt32									noDataTimeout,
	 const UInt32									completionTimeout,
	 IOAsyncCallback1							callback,
	 Pvoid												refCon);

IOReturn readUSBPipeTO
	(IOUSBInterfaceInterface * *	theInterface,
	 const UInt8									pipeRef,
   Pvoid												buffer,
   UInt32 &											buffSize,
   const UInt32									noDataTimeout,
   const UInt32									completionTimeout);

ULONG releaseUSBDevice
	(IOUSBDeviceInterface * *	theInterface);

ULONG releaseUSBInterface
	(IOUSBInterfaceInterface * *	theInterface);

IOReturn resetUSBPipe
	(IOUSBInterfaceInterface * *	theInterface,
	 const UInt8									pipeRef);

IOReturn sendUSBControlRequest
	(IOUSBInterfaceInterface * *	theInterface,
	 const UInt8									pipeRef,
   IOUSBDevRequest &						request);
 
IOReturn sendUSBControlRequestAsync
	(IOUSBInterfaceInterface * *	theInterface,
	 const UInt8									pipeRef,
	 IOUSBDevRequest &						req,
	 IOAsyncCallback1							callback,
	 Pvoid												refCon);
	 
IOReturn sendUSBControlRequestAsyncTO
	(IOUSBInterfaceInterface * *	theInterface,
	 const UInt8									pipeRef,
	 IOUSBDevRequestTO &					req,
	 IOAsyncCallback1							callback,
	 Pvoid												refCon);

IOReturn sendUSBControlRequestTO
	(IOUSBInterfaceInterface * *	theInterface,
	 const UInt8									pipeRef,
   IOUSBDevRequestTO &					request);

IOReturn writeUSBPipe
	(IOUSBInterfaceInterface * *	theInterface,
	 const UInt8									pipeRef,
   Pvoid												buffer,
   const UInt32									buffSize);

IOReturn writeUSBPipeAsync
	(IOUSBInterfaceInterface * *	theInterface,
	 const UInt8									pipeRef,
	 Pvoid												buf,
	 const UInt32									size,
	 IOAsyncCallback1							callback,
	 Pvoid												refCon);

IOReturn writeUSBPipeAsyncTO
	(IOUSBInterfaceInterface * *	theInterface,
	 const UInt8									pipeRef,
	 Pvoid												buf,
	 const UInt32									size,
	 const UInt32									noDataTimeout,
	 const UInt32									completionTimeout,
	 IOAsyncCallback1							callback,
	 Pvoid												refCon);
	 
IOReturn writeUSBPipeTO
	(IOUSBInterfaceInterface * *	theInterface,
	 const UInt8									pipeRef,
   Pvoid												buffer,
   const UInt32									buffSize,
   const UInt32									noDataTimeout,
   const UInt32									completionTimeout);
 #endif /* COMPILE_FOR_OSX_4 */

 #if defined(COMPILE_FOR_OS9_4)
bool setUpUSB
	(Pvoid																obj,
	 Pchar																name,
	 USBContext &													rec,
	 USBDeviceNotificationCallbackProcPtr	notificationFunction);

void shutDownUSB
	(USBContext &	rec);
 #endif /* COMPILE_FOR_OS9_4 */

#endif /* not COMMON_USBX_H_ */