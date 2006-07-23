/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       initHIDDeviceData.c                                                     */
/*                                                                                      */
/*  Contains:   The routine initHIDDeviceData().                                        */
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
#include "loadOtherSegments.h"

#if defined(COMPILE_FOR_OS9_4) && (! defined(COMPILE_FOR_STUB))
/*------------------------------------ addButtons ---*/
static void addButtons
	(HIDDeviceDataStruct &	thisDevice,
	 HIDButtonCapsPtr				allCaps,
	 const UInt32						capCount,
	 const IOHIDElementType	kind)
{
	HIDButtonCapsPtr	capsWalker = allCaps;
	
	for (UInt32	ii = 0; ii < capCount; ++ii, ++capsWalker)
	{
		if (capsWalker->isRange)
			continue;
			
		// Create a new element:
		HIDElementDataPtr	newElement = GETBYTES(1, HIDElementDataStruct);
		
		if (newElement)
		{
			initHIDElementData(*newElement);
			newElement->fType = kind;
			newElement->fUsage = long(capsWalker->u.notRange.usage);
			newElement->fUsagePage = long(capsWalker->usagePage);
			newElement->fReportID = capsWalker->reportID;
			newElement->fCookie = reinterpret_cast<IOHIDElementCookie>(++thisDevice.fElementCount);
			switch (newElement->fUsagePage)
			{
				case kHIDPage_GenericDesktop:
					{
						switch (newElement->fUsage) // look at usage to determine function
						{
							case kHIDUsage_GD_X:
							case kHIDUsage_GD_Y:
							case kHIDUsage_GD_Z:
							case kHIDUsage_GD_Rx:
							case kHIDUsage_GD_Ry:
							case kHIDUsage_GD_Rz:
								++thisDevice.fAxisCount;
								break;
							
							case kHIDUsage_GD_Slider:
								++thisDevice.fSliderCount;
								break;
							
							case kHIDUsage_GD_Dial:
								++thisDevice.fDialCount;
								break;
							
							case kHIDUsage_GD_Wheel:
								++thisDevice.fWheelCount;
								break;
								
							case kHIDUsage_GD_Hatswitch:
								++thisDevice.fHatCount;
								break;
							
						}
					}
					break;

				case kHIDPage_Button:
					++thisDevice.fButtonCount;
					break;
					
			}
			switch (kind)
			{
				case kIOHIDElementTypeInput_Misc:
				case kIOHIDElementTypeInput_Button:
				case kIOHIDElementTypeInput_Axis:
				case kIOHIDElementTypeInput_ScanCodes:
					++thisDevice.fInputCount;
					break;

				case kIOHIDElementTypeOutput:
					++thisDevice.fOutputCount;
					break;

				case kIOHIDElementTypeFeature:
					++thisDevice.fFeatureCount;
					break;

			}
			// Locate parent:
			HIDElementDataPtr	parent = thisDevice.fFirstElement;
			
			for ( ; parent; parent = parent->fNext)
			{
				if ((parent->fUsage == capsWalker->collectionUsage) &&
						(parent->fUsagePage == capsWalker->collectionUsagePage))
					break;
				
			}
			if (parent)
			{
				// Locate last child and attach as the last sibling:
				if (parent->fChild)
				{
					HIDElementDataPtr	lastChild = parent->fChild;
					
					for ( ; lastChild->fNextSibling; )
						lastChild = lastChild->fNextSibling;
					lastChild->fNextSibling = newElement;
					newElement->fPreviousSibling = lastChild;
				}
				else
					parent->fChild = newElement;
				newElement->fDepth = parent->fDepth + 1;
			}
			if (thisDevice.fLastElement)
			{
				newElement->fPrevious = thisDevice.fLastElement;
				thisDevice.fLastElement->fNext = newElement;
			}
			else
				thisDevice.fFirstElement = newElement;
			thisDevice.fLastElement = newElement;
		}
	}
} /* addButtons */
#endif /* COMPILE_FOR_OS9_4 and not COMPILE_FOR_STUB */

#if defined(COMPILE_FOR_OS9_4) && (! defined(COMPILE_FOR_STUB))
/*------------------------------------ addValueElements ---*/
static void addValueElements
	(HIDDeviceDataStruct &	thisDevice,
	 HIDValueCapsPtr				allCaps,
	 const UInt32						capCount,
	 const IOHIDElementType	kind)
{
	HIDValueCapsPtr	capsWalker = allCaps;
	
	for (UInt32	ii = 0; ii < capCount; ++ii, ++capsWalker)
	{
		if (capsWalker->isRange)
			continue;
			
		// Create a new element:
		HIDElementDataPtr	newElement = GETBYTES(1, HIDElementDataStruct);
		
		if (newElement)
		{
			initHIDElementData(*newElement);
			newElement->fType = kind;
			newElement->fUsage = long(capsWalker->u.notRange.usage);
			newElement->fUsagePage = long(capsWalker->usagePage);
			newElement->fSize = long(capsWalker->bitSize * capsWalker->reportCount);
			newElement->fReportID = capsWalker->reportID;
			newElement->fCookie = reinterpret_cast<IOHIDElementCookie>(++thisDevice.fElementCount);
			switch (newElement->fUsagePage)
			{
				case kHIDPage_GenericDesktop:
					{
						switch (newElement->fUsage) // look at usage to determine function
						{
							case kHIDUsage_GD_X:
							case kHIDUsage_GD_Y:
							case kHIDUsage_GD_Z:
							case kHIDUsage_GD_Rx:
							case kHIDUsage_GD_Ry:
							case kHIDUsage_GD_Rz:
								++thisDevice.fAxisCount;
								break;
							
							case kHIDUsage_GD_Slider:
								++thisDevice.fSliderCount;
								break;
							
							case kHIDUsage_GD_Dial:
								++thisDevice.fDialCount;
								break;
							
							case kHIDUsage_GD_Wheel:
								++thisDevice.fWheelCount;
								break;
								
							case kHIDUsage_GD_Hatswitch:
								++thisDevice.fHatCount;
								break;
							
						}
					}
					break;

				case kHIDPage_Button:
					++thisDevice.fButtonCount;
					break;
					
			}
			switch (kind)
			{
				case kIOHIDElementTypeInput_Misc:
					++thisDevice.fInputCount;
					break;

				case kIOHIDElementTypeOutput:
					++thisDevice.fOutputCount;
					break;

				case kIOHIDElementTypeFeature:
					++thisDevice.fFeatureCount;
					break;

			}
			// Locate parent:
			HIDElementDataPtr	parent = thisDevice.fFirstElement;
			
			for ( ; parent; parent = parent->fNext)
			{
				if ((parent->fUsage == capsWalker->collectionUsage) &&
						(parent->fUsagePage == capsWalker->collectionUsagePage))
					break;
				
			}
			if (parent)
			{
				// Locate last child and attach as the last sibling:
				if (parent->fChild)
				{
					HIDElementDataPtr	lastChild = parent->fChild;
					
					for ( ; lastChild->fNextSibling; )
						lastChild = lastChild->fNextSibling;
					lastChild->fNextSibling = newElement;
					newElement->fPreviousSibling = lastChild;
				}
				else
					parent->fChild = newElement;
				newElement->fDepth = parent->fDepth + 1;
			}
			if (thisDevice.fLastElement)
			{
				newElement->fPrevious = thisDevice.fLastElement;
				thisDevice.fLastElement->fNext = newElement;
			}
			else
				thisDevice.fFirstElement = newElement;
			thisDevice.fLastElement = newElement;
		}
	}
} /* addValueElements */
#endif /* COMPILE_FOR_OS9_4 and not COMPILE_FOR_STUB */

#if defined(COMPILE_FOR_OS9_4) && (! defined(COMPILE_FOR_STUB))
/*------------------------------------ assembleSerialNumber ---*/
static void assembleSerialNumber
	(Pvoid		inHIDReport,
	 UInt32		inHIDReportLength,
	 UInt32		inRefcon,
	 OSStatus	inStatus)
{
	HIDDeviceDataPtr	thisDevice = reinterpret_cast<HIDDeviceDataPtr>(inRefcon);	
	
	if (inStatus == kHIDSuccess)
	{
		// Extract ASCII characters from the UNICODE string.
		UInt32	shortLength = (inHIDReportLength / 2) + 2;
		Pchar		buffer = GETBYTES(shortLength, char);
		Pchar		walker = reinterpret_cast<Pchar>(inHIDReport);

		*buffer = '_';
		*(buffer + shortLength - 1) = 0;
		for (UInt32 ii = 1, jj = 1; ii < inHIDReportLength; ++jj, ii += 2)
			// ignore the first byte of each pair
			*(buffer + jj) = *(walker + ii);
		thisDevice->fSerialNumber = gensym(buffer);
		FREEBYTES(buffer, shortLength);
	}
} /* assembleSerialNumber */
#endif /* COMPILE_FOR_OS9_4 and not COMPILE_FOR_STUB */

#if defined(COMPILE_FOR_OSX_4) && (! defined(COMPILE_FOR_STUB))
/*------------------------------------ gatherHIDElementInfo ---*/
static void gatherHIDElementInfo
	(CFDictionaryRef				refElement,
	 HIDElementDataStruct &	thisElement)
{
	CFTypeRef	refType = CFDictionaryGetValue(refElement, CFSTR(kIOHIDElementCookieKey));
	
	if (refType)
		CFNumberGetValue(reinterpret_cast<CFNumberRef>(refType), kCFNumberLongType,
											&thisElement.fCookie);
	refType = CFDictionaryGetValue(refElement, CFSTR(kIOHIDElementMaxKey));
	if (refType)
		CFNumberGetValue(reinterpret_cast<CFNumberRef>(refType), kCFNumberLongType,
												&thisElement.fMax);
	refType = CFDictionaryGetValue(refElement, CFSTR(kIOHIDElementMinKey));
	if (refType)
		CFNumberGetValue(reinterpret_cast<CFNumberRef>(refType), kCFNumberLongType,
											&thisElement.fMin);
	refType = CFDictionaryGetValue(refElement, CFSTR(kIOHIDElementScaledMaxKey));
	if (refType)
		CFNumberGetValue(reinterpret_cast<CFNumberRef>(refType), kCFNumberLongType,
											&thisElement.fScaledMax);
	refType = CFDictionaryGetValue(refElement, CFSTR(kIOHIDElementScaledMinKey));
	if (refType)
		CFNumberGetValue(reinterpret_cast<CFNumberRef>(refType), kCFNumberLongType,
											&thisElement.fScaledMin);
	refType = CFDictionaryGetValue(refElement, CFSTR(kIOHIDElementSizeKey));
	if (refType)
		CFNumberGetValue(reinterpret_cast<CFNumberRef>(refType), kCFNumberLongType,
											&thisElement.fSize);
} /* gatherHIDElementInfo */
#endif /* COMPILE_FOR_OSX_4 and not COMPILE_FOR_STUB */

#if defined(COMPILE_FOR_OSX_4) && (! defined(COMPILE_FOR_STUB))
/*------------------------------------ collectHIDElements ---*/
static void collectHIDElements
	(CFTypeRef							thisValue,
	 const long							depth,
	 HIDDeviceDataStruct &	thisDevice,
	 HIDElementDataPtr &		thisElement)
{
	if (CFGetTypeID(thisValue) == CFArrayGetTypeID()) // if element is an array
	{
		CFArrayRef				thisArray = reinterpret_cast<CFArrayRef>(thisValue);
		CFIndex						upperBound = CFArrayGetCount(thisArray);
		HIDElementDataPtr	prevElement = NULL_PTR;
		
		for (CFIndex item = 0; item < upperBound; ++item)
		{
			const void *	element = CFArrayGetValueAtIndex(thisArray, item);
			
			if (element && (CFGetTypeID(element) == CFDictionaryGetTypeID()))
			{
				CFDictionaryRef	refElement = reinterpret_cast<CFDictionaryRef>(element);
				long						elementType, usagePage, usage;
				CFTypeRef				refElementType = CFDictionaryGetValue(refElement, CFSTR(kIOHIDElementTypeKey));
				CFTypeRef				refUsagePage = CFDictionaryGetValue(refElement, CFSTR(kIOHIDElementUsagePageKey));
				CFTypeRef				refUsage = CFDictionaryGetValue(refElement, CFSTR(kIOHIDElementUsageKey));

				if (refElementType)
					CFNumberGetValue(reinterpret_cast<CFNumberRef>(refElementType), kCFNumberLongType, &elementType);
				if (refUsagePage)
					CFNumberGetValue(reinterpret_cast<CFNumberRef>(refUsagePage), kCFNumberLongType, &usagePage);
				if (refUsage)
					CFNumberGetValue(reinterpret_cast<CFNumberRef>(refUsage), kCFNumberLongType, &usage);
				if (elementType == kIOHIDElementTypeCollection)
				{
					// a collection...	
				}
				else if (elementType)
				{
					if (usagePage && usage)
					{
						// valid usage and page
						switch (usagePage)
						{
							case kHIDPage_GenericDesktop:
								{
									switch (usage) // look at usage to determine function
									{
										case kHIDUsage_GD_X:
										case kHIDUsage_GD_Y:
										case kHIDUsage_GD_Z:
										case kHIDUsage_GD_Rx:
										case kHIDUsage_GD_Ry:
										case kHIDUsage_GD_Rz:
											++thisDevice.fAxisCount;
											break;
										
										case kHIDUsage_GD_Slider:
											++thisDevice.fSliderCount;
											break;
										
										case kHIDUsage_GD_Dial:
											++thisDevice.fDialCount;
											break;
										
										case kHIDUsage_GD_Wheel:
											++thisDevice.fWheelCount;
											break;
											
										case kHIDUsage_GD_Hatswitch:
											++thisDevice.fHatCount;
											break;
										
									}
								}
								break;

							case kHIDPage_Button:
								++thisDevice.fButtonCount;
								break;
								
						}
					}
				}
				HIDElementDataPtr	newElement = GETBYTES(1, HIDElementDataStruct);
				
				if (newElement)
				{
					initHIDElementData(*newElement);
					newElement->fType = static_cast<IOHIDElementType>(elementType);
					newElement->fUsage = usage;
					newElement->fUsagePage = usagePage;
					newElement->fDepth = depth;
					gatherHIDElementInfo(refElement, *newElement);
					++thisDevice.fElementCount;
					switch (elementType)
					{
						case kIOHIDElementTypeInput_Misc:
						case kIOHIDElementTypeInput_Button:
						case kIOHIDElementTypeInput_Axis:
						case kIOHIDElementTypeInput_ScanCodes:
							++thisDevice.fInputCount;
							break;

						case kIOHIDElementTypeOutput:
							++thisDevice.fOutputCount;
							break;

						case kIOHIDElementTypeFeature:
							++thisDevice.fFeatureCount;
							break;

						case kIOHIDElementTypeCollection:
							++thisDevice.fCollectionCount;
							break;

					}
					if (prevElement)
					{
						// Add to left sibling
						prevElement->fNextSibling = newElement;
						newElement->fPreviousSibling = prevElement;
					}
					else
						// First child
						thisElement = newElement;
					// Add to flattened chain
					if (thisDevice.fLastElement)
						thisDevice.fLastElement->fNext = newElement;
					if (! thisDevice.fFirstElement)
						thisDevice.fFirstElement = newElement;
					thisDevice.fLastElement = newElement;
					if (elementType == kIOHIDElementTypeCollection) // if this element is a collection of other elements
						collectHIDElements(CFDictionaryGetValue(refElement, CFSTR(kIOHIDElementKey)), depth + 1,
																thisDevice, newElement->fChild); 
					prevElement = newElement;
				}
	    }
		}
	}
} /* collectHIDElements */
#endif /* COMPILE_FOR_OSX_4 and not COMPILE_FOR_STUB */
	
#if defined(COMPILE_FOR_OSX_4)
/*------------------------------------ initHIDDeviceData ---*/
bool initHIDDeviceData
	(Pchar										name,
	 IOKitContext &						rec,
	 io_service_t							object,
	 HIDDeviceDataStruct &		thisDevice,
	 Pvoid										owner,
	 IOHIDDeviceInterface * *	interface)
{
 #if defined(COMPILE_FOR_STUB)
  #pragma unused(name,rec,object,thisDevice,owner,interface)
 	return false;
 #else /* not COMPILE_FOR_STUB */
	thisDevice.fCollectionCount = thisDevice.fFeatureCount =
		thisDevice.fInputCount = thisDevice.fOutputCount =
		thisDevice.fElementCount = thisDevice.fAxisCount =
		thisDevice.fButtonCount = thisDevice.fHatCount =
		thisDevice.fSliderCount = thisDevice.fWheelCount =
		thisDevice.fProductID = thisDevice.fVendorID =
		thisDevice.fPrivateSize = thisDevice.fRefConSize =
		thisDevice.fVersionNumber = 0;
	thisDevice.fTopElement = thisDevice.fFirstElement =
		thisDevice.fLastElement = NULL_PTR;
	thisDevice.fPrevious = thisDevice.fNext = NULL_PTR;
	thisDevice.fSerialNumber = NULL_PTR;
	thisDevice.fInputCallback = NULL_PTR;
	thisDevice.fInputQueue = NULL_PTR;
	thisDevice.fInterface = interface;
	thisDevice.fOwner = owner;
	thisDevice.fPrivate = NULL_PTR;
	thisDevice.fClass = NULL_PTR;
	thisDevice.fRefCon = NULL_PTR;
	
	// Build up the element collection for this device.
  CFMutableDictionaryRef	deviceProperties;
	kern_return_t						kernResult = 0;
	bool										okSoFar = true;

	if (getObjectProperties(rec, object, deviceProperties, kernResult))
	{
		// Get serial number
		CFTypeRef	serialNumberRef = CFDictionaryGetValue(deviceProperties, CFSTR(kIOHIDSerialNumberKey));
		
		if (serialNumberRef)
		{
			CFIndex	bufferSize = CFStringGetLength(reinterpret_cast<CFStringRef>(serialNumberRef)) + 1;
			Pchar		buffer = GETBYTES(bufferSize + 1, char);
			
			if (buffer)
			{
				*buffer = '_'; // We want to be sure that the string is non-numeric!
				if (CFStringGetCString(reinterpret_cast<CFStringRef>(serialNumberRef),
																buffer + 1, bufferSize, CFStringGetSystemEncoding()))
					thisDevice.fSerialNumber = gensym(buffer);
				FREEBYTES(buffer, bufferSize);
			}
		}
		else
			okSoFar = false;
		
		// Get product identifier
		CFTypeRef	productIDRef = CFDictionaryGetValue(deviceProperties, CFSTR(kIOHIDProductIDKey));
		
		if (productIDRef)
			CFNumberGetValue(reinterpret_cast<CFNumberRef>(productIDRef), kCFNumberLongType,
											&thisDevice.fProductID);
		else
			okSoFar = false;

		// Get vendor identifier		
		CFTypeRef	vendorIDRef = CFDictionaryGetValue(deviceProperties, CFSTR(kIOHIDVendorIDKey));
		
		if (vendorIDRef)
			CFNumberGetValue(reinterpret_cast<CFNumberRef>(vendorIDRef), kCFNumberLongType,
											&thisDevice.fVendorID);
		else
			okSoFar = false;

		// Get version number		
		CFTypeRef	versionNumberRef = CFDictionaryGetValue(deviceProperties, CFSTR(kIOHIDVersionNumberKey));
		
		if (versionNumberRef)
			CFNumberGetValue(reinterpret_cast<CFNumberRef>(versionNumberRef), kCFNumberLongType,
											&thisDevice.fVersionNumber);
		else
			okSoFar = false;
		if (okSoFar)
		{
			// Build up the element collection for this device.
			CFTypeRef	topmost = CFDictionaryGetValue(deviceProperties, CFSTR(kIOHIDElementKey));

			if (topmost)
				collectHIDElements(topmost, 0, thisDevice, thisDevice.fTopElement);
			else
			{
				okSoFar = false;
				LOG_POST_2("%s: could not create topmost element of device tree", name);
			}
		}
		CFRelease(deviceProperties);
	}
	return okSoFar;	
 #endif /* not COMPILE_FOR_STUB */
} /* initHIDDeviceData */
#endif /* COMPILE_FOR_OSX_4 */

#if defined(COMPILE_FOR_OS9_4)	 
/*------------------------------------ initHIDDeviceData ---*/
bool initHIDDeviceData
	(Pchar											name,
	 HIDDeviceDataStruct &			thisDevice,
	 USBDeviceRef								aDevice,
	 HIDDeviceDispatchTablePtr	aTable,
	 Pvoid											owner)
{
 #if defined(COMPILE_FOR_STUB)
  #pragma unused(name,thisDevice,aDevice,aTable,owner)
 	return false;
 #else /* not COMPILE_FOR_STUB */
	bool	okSoFar = false;
	
	thisDevice.fCollectionCount = thisDevice.fFeatureCount =
		thisDevice.fInputCount = thisDevice.fOutputCount =
		thisDevice.fElementCount = thisDevice.fAxisCount =
		thisDevice.fButtonCount = thisDevice.fHatCount =
		thisDevice.fSliderCount = thisDevice.fWheelCount =
		thisDevice.fProductID = thisDevice.fVendorID =
		thisDevice.fPrivateSize = thisDevice.fVersionNumber = 0;
	thisDevice.fTopElement = thisDevice.fFirstElement =
		thisDevice.fLastElement = NULL_PTR;
	thisDevice.fPrevious = thisDevice.fNext = NULL_PTR;
	thisDevice.fSerialNumber = NULL_PTR;
	thisDevice.fOwner = owner;
	thisDevice.fPrivate = NULL_PTR;
	thisDevice.fClass = NULL_PTR;
	thisDevice.fPrepReport = NULL_PTR;
	thisDevice.fConnection = NULL_PTR;
	thisDevice.fDevice = aDevice;
	thisDevice.fDispatchTable = aTable;
	// Get the raw descriptor:
	UInt32		reportDescLength = 0;
	OSStatus	result = (*aTable->pHIDGetHIDDescriptor)(kUSBReportDesc, 0, NULL_PTR,
																											&reportDescLength);

	if ((result == kHIDSuccess) && reportDescLength)
	{
		UInt8 *	reportDescPtr = GETBYTES(reportDescLength, UInt8);
		
		result = (*aTable->pHIDGetHIDDescriptor)(kUSBReportDesc, 0, reportDescPtr,
																						&reportDescLength);
		if (result == kHIDSuccess)
		{
			HIDPreparsedDataRef	prepReport;
			
			result = HIDOpenReportDescriptor(reportDescPtr, reportDescLength,
																				&prepReport, kHIDFlag_StrictErrorChecking);
			if (result == kHIDSuccess)
			{
				thisDevice.fPrepReport = prepReport;
				// Open the device
				result = (*aTable->pHIDOpenDevice)(&thisDevice.fConnection, kHIDPerm_ReadWriteShared, 0);
				okSoFar = (result == kHIDSuccess);
			}
			if (okSoFar)
			{
				UInt32	ioSize;

				// Get serial number
				result = (*aTable->pHIDGetIndexedString)(thisDevice.fConnection, 3, 0, assembleSerialNumber,
																									reinterpret_cast<UInt32>(&thisDevice));
				// Get product identifier
				ioSize = sizeof(thisDevice.fProductID);
				result = (*aTable->pHIDGetDeviceInfo)(kHIDGetInfo_ProductID, &thisDevice.fProductID, &ioSize);
				if (result != kHIDSuccess)
					okSoFar = false;
				// Get vendor identifier
				ioSize = sizeof(thisDevice.fVendorID);		
				result = (*aTable->pHIDGetDeviceInfo)(kHIDGetInfo_VendorID, &thisDevice.fVendorID, &ioSize);
				if (result != kHIDSuccess)
					okSoFar = false;
				// Get version number		
				ioSize = sizeof(thisDevice.fVersionNumber);		
				result = (*aTable->pHIDGetDeviceInfo)(kHIDGetInfo_VersionNumber, &thisDevice.fVersionNumber,
																							&ioSize);
				if (result != kHIDSuccess)
					okSoFar = false;
			}
			if (okSoFar)
			{
				HIDCaps	caps;
				
				result = HIDGetCaps(prepReport, &caps);
				if (result == kHIDSuccess)
				{
					HIDCollectionNodePtr	collection = GETBYTES(caps.numberCollectionNodes, HIDCollectionNode);
					UInt32								numCollNodes = caps.numberCollectionNodes;
					
					result = HIDGetCollectionNodes(collection, &numCollNodes, prepReport);
					if (result == kHIDSuccess)
					{
						HIDCollectionNodePtr	collWalker = collection;
						
						for (UInt32 ii = 1; ii <= numCollNodes; ++ii, ++collWalker)
						{
							// Check for a valid collection:
							if (collWalker->collectionUsage && collWalker->collectionUsagePage)
							{
								// Create new element:
								HIDElementDataPtr	newElement = GETBYTES(1, HIDElementDataStruct);
								
								if (newElement)
								{
									initHIDElementData(*newElement);
									newElement->fType = kIOHIDElementTypeCollection;
									newElement->fUsage = long(collWalker->collectionUsage);
									newElement->fUsagePage = long(collWalker->collectionUsagePage);
									newElement->fCookie = reinterpret_cast<IOHIDElementCookie>(++thisDevice.fElementCount);
									// Locate parent:
									HIDElementDataPtr	parent = thisDevice.fFirstElement;
									
									for ( ; parent; parent = parent->fNext)
									{
										if (parent->fCookie == reinterpret_cast<IOHIDElementCookie>(collWalker->parent))
											break;
										
									}
									if (parent)
									{
										// Locate last child and attach as the last sibling:
										if (parent->fChild)
										{
											HIDElementDataPtr	lastChild = parent->fChild;
											
											for ( ; lastChild->fNextSibling; )
												lastChild = lastChild->fNextSibling;
											lastChild->fNextSibling = newElement;
											newElement->fPreviousSibling = lastChild;
										}
										else
											parent->fChild = newElement;
										newElement->fDepth = parent->fDepth + 1;
									}
									else
									{
										newElement->fDepth = 0;
										thisDevice.fTopElement = newElement;
									}
									if (thisDevice.fLastElement)
									{
										newElement->fPrevious = thisDevice.fLastElement;
										thisDevice.fLastElement->fNext = newElement;
									}
									else
										thisDevice.fFirstElement = newElement;
									thisDevice.fLastElement = newElement;
								}
							}
						}
					}
					FREEBYTES(collection, caps.numberCollectionNodes);
					UInt32	numValueCaps = caps.numberInputValueCaps;
					
					if (numValueCaps < caps.numberOutputValueCaps)
						numValueCaps = caps.numberOutputValueCaps;
					if (numValueCaps < caps.numberFeatureValueCaps)
						numValueCaps = caps.numberFeatureValueCaps;
					if (numValueCaps)
					{
						UInt32					capCount;																				
						HIDValueCapsPtr	allCaps = GETBYTES(numValueCaps, HIDValueCaps);
						
						capCount = caps.numberInputValueCaps;
						if (capCount)
						{
							result = HIDGetValueCaps(kHIDInputReport, allCaps, &capCount, prepReport);
							if (result == kHIDSuccess)
								addValueElements(thisDevice, allCaps, capCount, kIOHIDElementTypeInput_Misc);
						}
						capCount = caps.numberOutputValueCaps;
						if (capCount)
						{
							result = HIDGetValueCaps(kHIDOutputReport, allCaps,	&capCount, prepReport);
							if (result == kHIDSuccess)
								addValueElements(thisDevice, allCaps, capCount, kIOHIDElementTypeOutput);
						}
						capCount = caps.numberFeatureValueCaps;
						if (capCount)
						{
							result = HIDGetValueCaps(kHIDFeatureReport, allCaps, &capCount, prepReport);
							if (result == kHIDSuccess)
								addValueElements(thisDevice, allCaps, capCount, kIOHIDElementTypeFeature);
						}
						FREEBYTES(allCaps, numValueCaps);
					}
					UInt32	numButtonCaps = caps.numberInputButtonCaps;
					
					if (numButtonCaps < caps.numberOutputButtonCaps)
						numButtonCaps = caps.numberOutputButtonCaps;
					if (numButtonCaps < caps.numberFeatureValueCaps)
						numButtonCaps = caps.numberFeatureValueCaps;					
					if (numButtonCaps)
					{
						UInt32						capCount;																				
						HIDButtonCapsPtr	allButtons = GETBYTES(numButtonCaps, HIDButtonCaps);
						
						capCount = caps.numberInputButtonCaps;
						if (capCount)
						{
							result = HIDGetButtonCaps(kHIDInputReport, allButtons, &capCount, prepReport);
							if (result == kHIDSuccess)
								addButtons(thisDevice, allButtons, capCount, kIOHIDElementTypeInput_Button);
						}
						capCount = caps.numberOutputButtonCaps;
						if (capCount)
						{
							result = HIDGetButtonCaps(kHIDOutputReport, allButtons, &capCount, prepReport);
							if (result == kHIDSuccess)
								addButtons(thisDevice, allButtons, capCount, kIOHIDElementTypeOutput);
						}
						capCount = caps.numberFeatureButtonCaps;
						if (capCount)
						{
							result = HIDGetButtonCaps(kHIDFeatureReport, allButtons, &capCount, prepReport);
							if (result == kHIDSuccess)
								addButtons(thisDevice, allButtons, capCount, kIOHIDElementTypeFeature);
						}
#if 0
						// do something...
LOG_POST_1("buttons:")//!!
						for (UInt32 ii = 0; ii < numButtonCaps; ++ii)
						{
							HIDButtonCapsPtr	thisButton = allButtons + ii;
							
LOG_POST_2("  button: %d", ii + 1)//!!
if (! thisButton->isRange)
	LOG_POST_2("    usage: %d", thisButton->u.notRange.usage)//!!
LOG_POST_2("    usagePage: %d", thisButton->usagePage)//!!
LOG_POST_2("    reportID: %d", thisButton->reportID)//!!
LOG_POST_2("    bitField: %d", thisButton->bitField)//!!
LOG_POST_2("    collection: %d", thisButton->collection)//!!
LOG_POST_2("    collectionUsage: %d", thisButton->collectionUsage)//!!
LOG_POST_2("    collectionUsagePage: %d", thisButton->collectionUsagePage)//!!
						}
#endif//0
						FREEBYTES(allButtons, numButtonCaps);
					}
				}
			}
		}																						
		FREEBYTES(reportDescPtr, reportDescLength);																							
	}
	else
		LOG_ERROR_4("%s: could not get report descriptor (%d/0x%x)", name, result, result)
	return okSoFar;
 #endif /* not COMPILE_FOR_STUB */
} /* initHIDDeviceData */
#endif /* COMPILE_FOR_OS9_4 */

#if defined(COMPILE_FOR_STUB)
 #pragma export list initHIDDeviceData
#endif /* COMPILE_FOR_STUB */