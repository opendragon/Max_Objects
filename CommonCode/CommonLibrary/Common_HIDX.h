/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       Common_HIDX.h                                                           */
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
/*  Created:    2003/12/07                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(COMMON_HIDX_H_))
 #define COMMON_HIDX_H_ /* */

 #if defined(COMPILE_FOR_CATS)
  #include "Common_IOKitX.h"

 // The following datatypes and defines are copied from the relevant Framework headers,
 // which aren't directly useable in CFM source:
  #define kIOHIDDeviceKey					"IOHIDDevice"
  #define kIOHIDProductIDKey			"ProductID"
  #define kIOHIDSerialNumberKey		"SerialNumber"
  #define kIOHIDVendorIDKey				"VendorID"
  #define kIOHIDVersionNumberKey	"VersionNumber"

/* FA12FA38-6F1A-11D4-BA0C-0005028F18D5 */
  #define kIOHIDDeviceUserClientTypeID CFUUIDGetConstantUUIDWithBytes(NULL, \
    0xFA, 0x12, 0xFA, 0x38, 0x6F, 0x1A, 0x11, 0xD4,			\
    0xBA, 0x0C, 0x00, 0x05, 0x02, 0x8F, 0x18, 0xD5)
/* 78BD420C-6F14-11D4-9474-0005028F18D5 */
/*! @defined kIOHIDDeviceInterfaceID
    @discussion Interface ID for the IOHIDDeviceInterface. Corresponds to an
                 available HID device. */
  #define kIOHIDDeviceInterfaceID CFUUIDGetConstantUUIDWithBytes(NULL, 	\
    0x78, 0xBD, 0x42, 0x0C, 0x6F, 0x14, 0x11, 0xD4,			\
    0x94, 0x74, 0x00, 0x05, 0x02, 0x8F, 0x18, 0xD5)

typedef Pvoid IOHIDElementCookie;

  #define kIOHIDElementCookieKey			"ElementCookie"
  #define kIOHIDElementKey						"Elements"
  #define kIOHIDElementMaxKey				"Max"
  #define kIOHIDElementMinKey				"Min"
  #define kIOHIDElementScaledMaxKey	"ScaledMax"
  #define kIOHIDElementScaledMinKey	"ScaledMin"
  #define kIOHIDElementSizeKey				"Size"
  #define kIOHIDElementTypeKey				"Type"
  #define kIOHIDElementUsageKey			"Usage"
  #define kIOHIDElementUsagePageKey	"UsagePage"

enum IOHIDReportType
{
	kIOHIDReportTypeInput = 0,
	kIOHIDReportTypeOutput,
	kIOHIDReportTypeFeature,
	kIOHIDReportTypeCount
}; /* IOHIDReportType */

enum IOHIDElementType
{
	kIOHIDElementTypeInput_Misc        = 1,
	kIOHIDElementTypeInput_Button      = 2,
	kIOHIDElementTypeInput_Axis        = 3,
	kIOHIDElementTypeInput_ScanCodes   = 4,
	kIOHIDElementTypeOutput            = 129,
	kIOHIDElementTypeFeature           = 257,
	kIOHIDElementTypeCollection        = 513
}; /* IOHIDElementType */

struct IOHIDDeviceInterface;

struct IOHIDQueueInterface;

/* Usage Pages */
enum
{
	kHIDPage_Undefined	= 0x00,
	kHIDPage_GenericDesktop	= 0x01,
	kHIDPage_Simulation	= 0x02,
	kHIDPage_VR	= 0x03,
	kHIDPage_Sport	= 0x04,
	kHIDPage_Game	= 0x05,
	/* Reserved 0x06 */
	kHIDPage_KeyboardOrKeypad	= 0x07,	/* USB Device Class Definition for Human Interface Devices (HID). Note: the usage type for all key codes is Selector (Sel). */
	kHIDPage_LEDs	= 0x08,
	kHIDPage_Button	= 0x09,
	kHIDPage_Ordinal	= 0x0A,
	kHIDPage_Telephony	= 0x0B,
	kHIDPage_Consumer	= 0x0C,
	kHIDPage_Digitizer	= 0x0D,
	/* Reserved 0x0E */
	kHIDPage_PID	= 0x0F,	/* USB Physical Interface Device definitions for force feedback and related devices. */
	kHIDPage_Unicode	= 0x10,
	/* Reserved 0x11 - 0x13 */
	kHIDPage_AlphanumericDisplay	= 0x14,
	/* Reserved 0x15 - 0x7F */
	/* Monitor 0x80 - 0x83	 USB Device Class Definition for Monitor Devices */
	/* Power 0x84 - 0x87	 USB Device Class Definition for Power Devices */
	kHIDPage_PowerDevice = 0x84, 				/* Power Device Page */
	kHIDPage_BatterySystem = 0x85, 				/* Battery System Page */
	/* Reserved 0x88 - 0x8B */
	kHIDPage_BarCodeScanner	= 0x8C,	/* (Point of Sale) USB Device Class Definition for Bar Code Scanner Devices */
	kHIDPage_Scale	= 0x8D,	/* (Point of Sale) USB Device Class Definition for Scale Devices */
	/* ReservedPointofSalepages 0x8E - 0x8F */
	kHIDPage_CameraControl	= 0x90,	/* USB Device Class Definition for Image Class Devices */
	kHIDPage_Arcade	= 0x91,	/* OAAF Definitions for arcade and coinop related Devices */
	/* Reserved 0x92 - 0xFEFF */
	/* VendorDefined 0xFF00 - 0xFFFF */
	kHIDPage_VendorDefinedStart	= 0xFF00
};

/* GenericDesktop Page (0x01) */
enum
{
	kHIDUsage_GD_Pointer	= 0x01,	/* Physical Collection */
	kHIDUsage_GD_Mouse	= 0x02,	/* Application Collection */
	/* 0x03 Reserved */
	kHIDUsage_GD_Joystick	= 0x04,	/* Application Collection */
	kHIDUsage_GD_GamePad	= 0x05,	/* Application Collection */
	kHIDUsage_GD_Keyboard	= 0x06,	/* Application Collection */
	kHIDUsage_GD_Keypad	= 0x07,	/* Application Collection */
	kHIDUsage_GD_MultiAxisController	= 0x08,	/* Application Collection */
	/* 0x09 - 0x2F Reserved */
	kHIDUsage_GD_X	= 0x30,	/* Dynamic Value */
	kHIDUsage_GD_Y	= 0x31,	/* Dynamic Value */
	kHIDUsage_GD_Z	= 0x32,	/* Dynamic Value */
	kHIDUsage_GD_Rx	= 0x33,	/* Dynamic Value */
	kHIDUsage_GD_Ry	= 0x34,	/* Dynamic Value */
	kHIDUsage_GD_Rz	= 0x35,	/* Dynamic Value */
	kHIDUsage_GD_Slider	= 0x36,	/* Dynamic Value */
	kHIDUsage_GD_Dial	= 0x37,	/* Dynamic Value */
	kHIDUsage_GD_Wheel	= 0x38,	/* Dynamic Value */
	kHIDUsage_GD_Hatswitch	= 0x39,	/* Dynamic Value */
	kHIDUsage_GD_CountedBuffer	= 0x3A,	/* Logical Collection */
	kHIDUsage_GD_ByteCount	= 0x3B,	/* Dynamic Value */
	kHIDUsage_GD_MotionWakeup	= 0x3C,	/* One-Shot Control */
	kHIDUsage_GD_Start	= 0x3D,	/* On/Off Control */
	kHIDUsage_GD_Select	= 0x3E,	/* On/Off Control */
	/* 0x3F Reserved */
	kHIDUsage_GD_Vx	= 0x40,	/* Dynamic Value */
	kHIDUsage_GD_Vy	= 0x41,	/* Dynamic Value */
	kHIDUsage_GD_Vz	= 0x42,	/* Dynamic Value */
	kHIDUsage_GD_Vbrx	= 0x43,	/* Dynamic Value */
	kHIDUsage_GD_Vbry	= 0x44,	/* Dynamic Value */
	kHIDUsage_GD_Vbrz	= 0x45,	/* Dynamic Value */
	kHIDUsage_GD_Vno	= 0x46,	/* Dynamic Value */
	/* 0x47 - 0x7F Reserved */
	kHIDUsage_GD_SystemControl	= 0x80,	/* Application Collection */
	kHIDUsage_GD_SystemPowerDown	= 0x81,	/* One-Shot Control */
	kHIDUsage_GD_SystemSleep	= 0x82,	/* One-Shot Control */
	kHIDUsage_GD_SystemWakeUp	= 0x83,	/* One-Shot Control */
	kHIDUsage_GD_SystemContextMenu	= 0x84,	/* One-Shot Control */
	kHIDUsage_GD_SystemMainMenu	= 0x85,	/* One-Shot Control */
	kHIDUsage_GD_SystemAppMenu	= 0x86,	/* One-Shot Control */
	kHIDUsage_GD_SystemMenuHelp	= 0x87,	/* One-Shot Control */
	kHIDUsage_GD_SystemMenuExit	= 0x88,	/* One-Shot Control */
	kHIDUsage_GD_SystemMenu	= 0x89,	/* Selector */
	kHIDUsage_GD_SystemMenuRight	= 0x8A,	/* Re-Trigger Control */
	kHIDUsage_GD_SystemMenuLeft	= 0x8B,	/* Re-Trigger Control */
	kHIDUsage_GD_SystemMenuUp	= 0x8C,	/* Re-Trigger Control */
	kHIDUsage_GD_SystemMenuDown	= 0x8D,	/* Re-Trigger Control */
	/* 0x8E - 0x8F Reserved */
	kHIDUsage_GD_DPadUp	= 0x90,	/* On/Off Control */
	kHIDUsage_GD_DPadDown	= 0x91,	/* On/Off Control */
	kHIDUsage_GD_DPadRight	= 0x92,	/* On/Off Control */
	kHIDUsage_GD_DPadLeft	= 0x93,	/* On/Off Control */
	/* 0x94 - 0xFFFF Reserved */
	kHIDUsage_GD_Reserved = 0xFFFF
};

/*! @typedef IOHIDCallbackFunction
    @discussion Type and arguments of callout C function that is used when a
                completion routine is called, see
                IOHIDLib.h:setRemovalCallback().
    @param target void * pointer to your data, often a pointer to an object.
    @param result completion result of desired operation
    @param refcon void * pointer to more data.
    @param sender interface instance sending the completion routine.
*/
typedef void (*IOHIDCallbackFunction)
              (void * target, IOReturn result, void * refcon, void * sender);

// End of copies ...
 #else /* not COMPILE_FOR_CATS */
  #include "Common_USBX.h"
  #include "hid.h"

typedef Pvoid IOHIDElementCookie;

enum IOHIDElementType
{
	kIOHIDElementTypeInput_Misc        = 1,
	kIOHIDElementTypeInput_Button      = 2,
	kIOHIDElementTypeInput_Axis        = 3,
	kIOHIDElementTypeInput_ScanCodes   = 4,
	kIOHIDElementTypeOutput            = 129,
	kIOHIDElementTypeFeature           = 257,
	kIOHIDElementTypeCollection        = 513
}; /* IOHIDElementType */
	
 #endif /* not COMPILE_FOR_CATS */

 #if defined(COMPILE_FOR_CATS)
  #define STANDARD_HID_ARGS_INPUTEVENTHANDLER	\
		Pvoid								refCon,\
		IOHIDElementCookie	elementCookie,\
		SInt32							value,\
		UInt32							longValueSize,\
		Pvoid								longValue

typedef void (* InputEventHandler)
	(STANDARD_HID_ARGS_INPUTEVENTHANDLER);
 #endif /* COMPILE_FOR_CATS */
	
struct HIDDeviceDataStruct;

typedef HIDDeviceDataStruct *	HIDDeviceDataPtr;

struct HIDElementDataStruct;

typedef HIDElementDataStruct *	HIDElementDataPtr;

struct HIDElementDataStruct
{
	HIDElementDataPtr		fChild;
	IOHIDElementCookie	fCookie;
	long								fDepth;
	long								fMax;
	long								fMin;
	HIDElementDataPtr		fNext;
	HIDElementDataPtr		fNextSibling;
	HIDElementDataPtr		fPrevious;
	HIDElementDataPtr		fPreviousSibling;
	long								fScaledMax;
	long								fScaledMin;
	long								fSize;
  IOHIDElementType		fType;
  long								fUsage;	
  long								fUsagePage;
 #if (! defined(COMPILE_FOR_CATS))
 	UInt32							fReportID;
 #endif /* not COMPILE_FOR_CATS */
}; /* ElementDataStruct */

struct HIDDeviceDataStruct
{
	long											fAxisCount;
	long											fButtonCount;
	Pvoid											fClass;
	long											fCollectionCount;
	long											fDialCount;
	long											fElementCount;
	long											fFeatureCount;
	HIDElementDataPtr					fFirstElement;
	long											fHatCount;
	long											fInputCount;
	HIDElementDataPtr					fLastElement;
	HIDDeviceDataPtr					fNext;
	Pvoid											fOwner;
	HIDDeviceDataPtr					fPrevious;
	Ptr												fPrivate;
	long											fPrivateSize;
	long											fOutputCount;
	long											fProductID;
	PSymbol										fSerialNumber;
	long											fSliderCount;
	HIDElementDataPtr					fTopElement;
	long											fVendorID;
	long											fVersionNumber;
	long											fWheelCount;
 #if defined(COMPILE_FOR_CATS)
	IOHIDCallbackFunction			fInputCallback;
	InputEventHandler					fInputFunction;
	IOHIDQueueInterface * *		fInputQueue;
	CFRunLoopSourceRef				fInputQueueRunloop;
	IOServiceInterestCallback	fInterestCallback;
	IOHIDDeviceInterface * *	fInterface;
	io_object_t								fNotification;
  InputEventHandler					fQueueHandler;
	Ptr												fRefCon;
	long											fRefConSize;
 #else /* not COMPILE_FOR_CATS */ 
	HIDDeviceConnectionRef		fConnection;
	USBDeviceRef							fDevice;
	HIDDeviceDispatchTablePtr	fDispatchTable;
	HIDPreparsedDataRef				fPrepReport;
 #endif /* not COMPILE_FOR_CATS */
}; /* HIDDeviceDataStruct */

void initHIDElementData
	(HIDElementDataStruct &	thisElement);

 #if defined(COMPILE_FOR_CATS)
IOReturn closeHIDDevice
	(IOHIDDeviceInterface * *	theInterface);

long getHIDElementValue
	(Pchar									name,
	 HIDDeviceDataStruct &	thisDevice,
	 HIDElementDataStruct &	theElement,
	 UInt32 &								extendedValueLength,
	 Pvoid &								extendedValue,
	 IOReturn &							result);
		 
bool initHIDDeviceData
	(Pchar										name,
	 IOKitContext &						rec,
	 io_service_t							object,
	 HIDDeviceDataStruct &		thisDevice,
	 Pvoid										owner,
	 IOHIDDeviceInterface * *	interface);

IOReturn openHIDDevice
	(IOHIDDeviceInterface * *	theInterface,
   UInt32										flags);

void releaseHIDData
	(IOKitContext &			rec,
	 HIDDeviceDataPtr &	thisDevice);
	
ULONG releaseHIDDevice
	(IOHIDDeviceInterface * *	theInterface);

void setHIDElementValue
	(Pchar									name,
	 HIDDeviceDataStruct &	thisDevice,
	 HIDElementDataStruct &	theElement,
	 UInt32									extendedValueLength,
	 PAtom									extendedValue,
	 long										singleValue,
	 IOReturn &							result);

void setUpHIDInputQueue
	(Pchar									name,
	 IOKitContext &					rec,
	 HIDDeviceDataStruct &	thisDevice,
	 InputEventHandler			aFun,
	 IOHIDElementCookie *		cookies,
	 int										numCookies);
  
 #else /* not COMPILE_FOR_CATS */	 
bool initHIDDeviceData
	(Pchar											name,
	 HIDDeviceDataStruct &			thisDevice,
	 USBDeviceRef								aDevice,
	 HIDDeviceDispatchTablePtr	aTable,
	 Pvoid											owner);

void releaseHIDData
	(HIDDeviceDataPtr &	thisDevice);
	
void setHIDElementValue
	(Pchar									name,
	 HIDDeviceDataStruct &	thisDevice,
	 HIDElementDataStruct &	theElement,
	 UInt32									extendedValueLength,
	 PAtom									extendedValue,
	 long										singleValue,
	 OSStatus &							result);

 #endif /* not COMPILE_FOR_CATS */
 
#endif /* not COMMON_HIDX_H_ */
