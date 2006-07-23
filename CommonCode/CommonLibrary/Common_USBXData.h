/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       Common_USBXData.h                                                       */
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

#if (! defined(COMMON_USBXDATA_H_))
 #define COMMON_USBXDATA_H_ /* */

 #include "Common_USBX.h"
 
 #if defined(COMPILE_FOR_OSX_4)
// The following datatypes and defines are copied from the relevant Framework headers,
// which aren't directly useable in CFM source:

enum
{
	kUSBRqDirnShift = 7,
	kUSBRqDirnMask = 1,

	kUSBRqTypeShift = 5,
	kUSBRqTypeMask = 3,

	kUSBRqRecipientMask = 0x1F
};

enum
{
	kUSBDefaultControlNoDataTimeoutMS = 5000,
	kUSBDefaultControlCompletionTimeoutMS = 0
};

typedef UInt16 USBDeviceAddress;

struct IOUSBIsocFrame;

struct IOUSBConfigurationDescriptor
{
	UInt8		bLength;
	UInt8		bDescriptorType;
	UInt16	wTotalLength;
	UInt8		bNumInterfaces;
	UInt8		bConfigurationValue;
	UInt8		iConfiguration;
	UInt8		bmAttributes;
	UInt8		MaxPower;
}; /* IOUSBConfigurationDescriptor */

typedef IOUSBConfigurationDescriptor *		IOUSBConfigurationDescriptorPtr;

//
// DeviceInterface Functions available in version 1.8 (10.0) and 1.8.1 (10.0.1) of Mac OS X
//
 #define IOUSBDEVICE_FUNCS_180	\
    IOReturn (*CreateDeviceAsyncEventSource)(void *self, CFRunLoopSourceRef *source); \
    CFRunLoopSourceRef (*GetDeviceAsyncEventSource)(void *self); \
    IOReturn (*CreateDeviceAsyncPort)(void *self, mach_port_t *port); \
    mach_port_t (*GetDeviceAsyncPort)(void *self); \
    IOReturn (*USBDeviceOpen)(void *self); \
    IOReturn (*USBDeviceClose)(void *self); \
    IOReturn (*GetDeviceClass)(void *self, UInt8 *devClass); \
    IOReturn (*GetDeviceSubClass)(void *self, UInt8 *devSubClass); \
    IOReturn (*GetDeviceProtocol)(void *self, UInt8 *devProtocol); \
    IOReturn (*GetDeviceVendor)(void *self, UInt16 *devVendor); \
    IOReturn (*GetDeviceProduct)(void *self, UInt16 *devProduct); \
    IOReturn (*GetDeviceReleaseNumber)(void *self, UInt16 *devRelNum); \
    IOReturn (*GetDeviceAddress)(void *self, USBDeviceAddress *addr); \
    IOReturn (*GetDeviceBusPowerAvailable)(void *self, UInt32 *powerAvailable); \
    IOReturn (*GetDeviceSpeed)(void *self, UInt8 *devSpeed); \
    IOReturn (*GetNumberOfConfigurations)(void *self, UInt8 *numConfig); \
    IOReturn (*GetLocationID)(void *self, UInt32 *locationID); \
    IOReturn (*GetConfigurationDescriptorPtr)(void *self, UInt8 configIndex, IOUSBConfigurationDescriptorPtr *desc); \
    IOReturn (*GetConfiguration)(void *self, UInt8 *configNum); \
    IOReturn (*SetConfiguration)(void *self, UInt8 configNum); \
    IOReturn (*GetBusFrameNumber)(void *self, UInt64 *frame, AbsoluteTime *atTime); \
    IOReturn (*ResetDevice)(void *self); \
    IOReturn (*DeviceRequest)(void *self, IOUSBDevRequest *req); \
    IOReturn (*DeviceRequestAsync)(void *self, IOUSBDevRequest *req, IOAsyncCallback1 callback, void *refCon); \
    IOReturn (*CreateInterfaceIterator)(void *self, IOUSBFindInterfaceRequest *req, io_iterator_t *iter)
//-----------------------------------------------------------------------------------------
// END OF DeviceInterface Functions available in version 1.8 (10.0) and 1.8.1 (10.0.1) of Mac OS X
//-----------------------------------------------------------------------------------------
//
// DeviceInterface Functions available in version 1.8.2 Mac OS X
//
#define IOUSBDEVICE_FUNCS_182 \
    IOReturn (*USBDeviceOpenSeize)(void *self); \
    IOReturn (*DeviceRequestTO)(void *self, IOUSBDevRequestTO *req); \
    IOReturn (*DeviceRequestAsyncTO)(void *self, IOUSBDevRequestTO *req, IOAsyncCallback1 callback, void *refCon); \
    IOReturn (*USBDeviceSuspend)(void *self, Boolean suspend); \
    IOReturn (*USBDeviceAbortPipeZero)(void *self); \
    IOReturn (*USBGetManufacturerStringIndex)(void *self, UInt8 *msi); \
    IOReturn (*USBGetProductStringIndex)(void *self, UInt8 *psi); \
    IOReturn (*USBGetSerialNumberStringIndex)(void *self, UInt8 *snsi)

//-----------------------------------------------------------------------------------------
// END OF DeviceInterface Functions available in version 1.8.2 (10.1) of Mac OS X
//-----------------------------------------------------------------------------------------
//
// InterfaceInterface Functions available in version 1.8 (10.0) and 1.8.1 (10.0.1) of Mac OS X
//
  #define IOUSBINTERFACE_FUNCS_180 \
    IOReturn (*CreateInterfaceAsyncEventSource)(void *self, CFRunLoopSourceRef *source); \
    CFRunLoopSourceRef (*GetInterfaceAsyncEventSource)(void *self); \
    IOReturn (*CreateInterfaceAsyncPort)(void *self, mach_port_t *port); \
    mach_port_t (*GetInterfaceAsyncPort)(void *self); \
    IOReturn (*USBInterfaceOpen)(void *self); \
    IOReturn (*USBInterfaceClose)(void *self); \
    IOReturn (*GetInterfaceClass)(void *self, UInt8 *intfClass); \
    IOReturn (*GetInterfaceSubClass)(void *self, UInt8 *intfSubClass); \
    IOReturn (*GetInterfaceProtocol)(void *self, UInt8 *intfProtocol); \
    IOReturn (*GetDeviceVendor)(void *self, UInt16 *devVendor); \
    IOReturn (*GetDeviceProduct)(void *self, UInt16 *devProduct); \
    IOReturn (*GetDeviceReleaseNumber)(void *self, UInt16 *devRelNum); \
    IOReturn (*GetConfigurationValue)(void *self, UInt8 *configVal); \
    IOReturn (*GetInterfaceNumber)(void *self, UInt8 *intfNumber); \
    IOReturn (*GetAlternateSetting)(void *self, UInt8 *intfAltSetting); \
    IOReturn (*GetNumEndpoints)(void *self, UInt8 *intfNumEndpoints); \
    IOReturn (*GetLocationID)(void *self, UInt32 *locationID); \
    IOReturn (*GetDevice)(void *self, io_service_t *device); \
    IOReturn (*SetAlternateInterface)(void *self, UInt8 alternateSetting); \
    IOReturn (*GetBusFrameNumber)(void *self, UInt64 *frame, AbsoluteTime *atTime); \
    IOReturn (*ControlRequest)(void *self, UInt8 pipeRef, IOUSBDevRequest *req); \
    IOReturn (*ControlRequestAsync)(void *self, UInt8 pipeRef, IOUSBDevRequest *req, IOAsyncCallback1 callback, void *refCon); \
    IOReturn (*GetPipeProperties)(void *self, UInt8 pipeRef, UInt8 *direction, UInt8 *number, UInt8 *transferType, UInt16 *maxPacketSize, UInt8 *interval); \
    IOReturn (*GetPipeStatus)(void *self, UInt8 pipeRef); \
    IOReturn (*AbortPipe)(void *self, UInt8 pipeRef); \
    IOReturn (*ResetPipe)(void *self, UInt8 pipeRef); \
    IOReturn (*ClearPipeStall)(void *self, UInt8 pipeRef); \
    IOReturn (*ReadPipe)(void *self, UInt8 pipeRef, void *buf, UInt32 *size); \
    IOReturn (*WritePipe)(void *self, UInt8 pipeRef, void *buf, UInt32 size); \
    IOReturn (*ReadPipeAsync)(void *self, UInt8 pipeRef, void *buf, UInt32 size, IOAsyncCallback1 callback, void *refcon); \
    IOReturn (*WritePipeAsync)(void *self, UInt8 pipeRef, void *buf, UInt32 size, IOAsyncCallback1 callback, void *refcon); \
    IOReturn (*ReadIsochPipeAsync)(void *self, UInt8 pipeRef, void *buf, UInt64 frameStart, UInt32 numFrames, IOUSBIsocFrame *frameList, \
                                  IOAsyncCallback1 callback, void *refcon); \
    IOReturn (*WriteIsochPipeAsync)(void *self, UInt8 pipeRef, void *buf, UInt64 frameStart, UInt32 numFrames, IOUSBIsocFrame *frameList, \
                                  IOAsyncCallback1 callback, void *refcon)

//-----------------------------------------------------------------------------------------
// END OF InterfaceInterface Functions available in version 1.8 (10.0) and 1.8.1 (10.0.1) of Mac OS X
//-----------------------------------------------------------------------------------------
//
// InterfaceInterface Functions available in version 1.8.2 Mac OS X
//
#define IOUSBINTERFACE_FUNCS_182 \
    IOReturn (*ControlRequestTO)(void *self, UInt8 pipeRef, IOUSBDevRequestTO *req); \
    IOReturn (*ControlRequestAsyncTO)(void *self, UInt8 pipeRef, IOUSBDevRequestTO *req, IOAsyncCallback1 callback, void *refCon); \
    IOReturn (*ReadPipeTO)(void *self, UInt8 pipeRef, void *buf, UInt32 *size, UInt32 noDataTimeout, UInt32 completionTimeout); \
    IOReturn (*WritePipeTO)(void *self, UInt8 pipeRef, void *buf, UInt32 size, UInt32 noDataTimeout, UInt32 completionTimeout); \
    IOReturn (*ReadPipeAsyncTO)(void *self, UInt8 pipeRef, void *buf, UInt32 size, UInt32 noDataTimeout, UInt32 completionTimeout, IOAsyncCallback1 callback, void *refcon); \
    IOReturn (*WritePipeAsyncTO)(void *self, UInt8 pipeRef, void *buf, UInt32 size, UInt32 noDataTimeout, UInt32 completionTimeout, IOAsyncCallback1 callback, void *refcon); \
    IOReturn (*USBInterfaceGetStringIndex)(void *self, UInt8 *si)
//-----------------------------------------------------------------------------------------
// END OF InterfaceInterface Functions available in version 1.8.2 Mac OS X
//-----------------------------------------------------------------------------------------

struct IOUSBDeviceInterface
{
	IUNKNOWN_C_GUTS;
	IOUSBDEVICE_FUNCS_180;
	IOUSBDEVICE_FUNCS_182;
}; /* IOUSBDeviceInterface */

struct IOUSBInterfaceInterface
{
	IUNKNOWN_C_GUTS;
	IOUSBINTERFACE_FUNCS_180;
	IOUSBINTERFACE_FUNCS_182;
}; /* IOUSBInterfaceInterface */

// End of copies ...

typedef ULONG (* release_FP)
    (Pvoid	thisPointer);

typedef IOReturn (* usbAbortPipe_FP)
	(Pvoid	self,
	 UInt8	pipeRef);
	 
typedef IOReturn (* usbControlRequest_FP)
	(Pvoid							self,
	 UInt8							pipeRef,
	 IOUSBDevRequest *	req);

typedef IOReturn (* usbControlRequestAsync_FP)
	(Pvoid							self,
	 UInt8							pipeRef,
	 IOUSBDevRequest *	req,
	 IOAsyncCallback1		callback,
	 Pvoid							refCon);
	 
typedef IOReturn (* usbControlRequestAsyncTO_FP)
	(Pvoid								self,
	 UInt8								pipeRef,
	 IOUSBDevRequestTO *	req,
	 IOAsyncCallback1			callback,
	 Pvoid								refCon);

typedef IOReturn (* usbControlRequestTO_FP)
	(Pvoid								self,
	 UInt8								pipeRef,
	 IOUSBDevRequestTO *	req);

typedef IOReturn (* usbCreateInterfaceAsyncEventSource_FP)
	(Pvoid								self,
	 CFRunLoopSourceRef *	source);
	 
typedef IOReturn (* usbCreateInterfaceIterator_FP)
	(Pvoid												self,
	 IOUSBFindInterfaceRequest *	req,
	 io_iterator_t *							iter);
	 
typedef IOReturn (* usbDeviceClose_FP)
	(Pvoid	self);

typedef IOReturn (* usbDeviceOpen_FP)
	(Pvoid	self);

typedef IOReturn (* usbGetConfigurationDescriptorPtr_FP)
	(Pvoid															self,
	 UInt8															configIndex,
	 IOUSBConfigurationDescriptorPtr *	desc);
	 
typedef IOReturn (* usbGetDeviceVendor_FP)
	(Pvoid		self,
	 UInt16 *	devVendor);
	 
typedef IOReturn (* usbGetDeviceProduct_FP)
	(Pvoid		self,
	 UInt16 *	devProduct);
	 
typedef IOReturn (* usbGetDeviceReleaseNumber_FP)
	(Pvoid		self,
	 UInt16 *	devRelNum);

typedef CFRunLoopSourceRef (* usbGetInterfaceAsyncEventSource_FP)
	(Pvoid	self);

typedef IOReturn (* usbGetInterfaceClass_FP)
	(Pvoid		self,
	 UInt8 *	intfClass);
	 
typedef IOReturn (* usbGetInterfaceSubClass_FP)
	(Pvoid		self,
	 UInt8 *	intfSubClass);
	 
typedef IOReturn (* usbGetNumberOfConfigurations_FP)
	(Pvoid		self,
	 UInt8 *	numConfig);

typedef IOReturn (* usbInterfaceClose_FP)
	(Pvoid	self);

typedef IOReturn (* usbInterfaceOpen_FP)
	(Pvoid	self);

typedef IOReturn (* usbReadPipe_FP)
	(Pvoid		self,
	 UInt8		pipeRef,
	 Pvoid		buf,
	 UInt32 *	size);
	 
typedef IOReturn (* usbReadPipeAsync_FP)
	(Pvoid						self,
	 UInt8						pipeRef,
	 Pvoid						buf,
	 UInt32						size,
	 IOAsyncCallback1	callback,
	 Pvoid						refCon);
	 
typedef IOReturn (* usbReadPipeAsyncTO_FP)
	(Pvoid						self,
	 UInt8						pipeRef,
	 Pvoid						buf,
	 UInt32						size,
	 UInt32						noDataTimeout,
	 UInt32						completionTimeout,
	 IOAsyncCallback1	callback,
	 Pvoid						refCon);

typedef IOReturn (* usbReadPipeTO_FP)
	(Pvoid		self,
	 UInt8		pipeRef,
	 Pvoid		buf,
	 UInt32 *	size,
	 UInt32		noDataTimeout,
	 UInt32		completionTimeout);
	 
typedef IOReturn (* usbResetPipe_FP)
	(Pvoid	self,
	 UInt8	pipeRef);
	 
typedef IOReturn (* usbSetConfiguration_FP)
	(Pvoid	self,
	 UInt8	configNum);

typedef IOReturn (* usbWritePipe_FP)
	(Pvoid	self,
	 UInt8	pipeRef,
	 Pvoid	buf,
	 UInt32	size);
	  
typedef IOReturn (* usbWritePipeAsync_FP)
	(Pvoid						self,
	 UInt8						pipeRef,
	 Pvoid						buf,
	 UInt32						size,
	 IOAsyncCallback1	callback,
	 Pvoid						refCon);

typedef IOReturn (* usbWritePipeAsyncTO_FP)
	(Pvoid						self,
	 UInt8						pipeRef,
	 Pvoid						buf,
	 UInt32						size,
	 UInt32						noDataTimeout,
	 UInt32						completionTimeout,
	 IOAsyncCallback1	callback,
	 Pvoid						refCon);
	 
typedef IOReturn (* usbWritePipeTO_FP)
	(Pvoid	self,
	 UInt8	pipeRef,
	 Pvoid	buf,
	 UInt32	size,
	 UInt32	noDataTimeout,
	 UInt32	completionTimeout);
	 
 #endif /* COMPILE_FOR_OSX_4 */

#endif /* not COMMON_USBXDATA_H_ */
