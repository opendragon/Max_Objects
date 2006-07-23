/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       setHIDElementValue.c                                                    */
/*                                                                                      */
/*  Contains:   The routine setHIDElementValue().                                       */
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

#if defined(COMPILE_FOR_OSX_4)
/*------------------------------------ setHIDElementValue ---*/
void setHIDElementValue
	(Pchar									name,
	 HIDDeviceDataStruct &	thisDevice,
	 HIDElementDataStruct &	theElement,
	 UInt32									extendedValueLength,
	 PAtom									extendedValue,
	 long										singleValue,
	 IOReturn &							result)
{
 #if defined(COMPILE_FOR_STUB)
  #pragma unused(name,thisDevice,theElement,extendedValueLength,extendedValue,singleValue,result)
 #else /* not COMPILE_FOR_STUB */
	IOHIDEventStruct				anEvent;
	TransferVector_rec			funkChunk;
	hidSetElementValue_FP		pFsetElementValue;
	Pchar										buffer = NULL_PTR;
	
	anEvent.type = theElement.fType;
	anEvent.elementCookie = theElement.fCookie;
	anEvent.value = singleValue;
	if (extendedValueLength && extendedValue)
	{
		buffer = GETBYTES(extendedValueLength, char);
		for (UInt32 ii = 0; ii < extendedValueLength; ++ii)
			*(buffer + ii) = char((extendedValue + ii)->a_w.w_long);
		anEvent.longValueSize = extendedValueLength;
	}
	else
		anEvent.longValueSize = 0;
	anEvent.longValue = buffer;
	pFsetElementValue = fillCallback(hidSetElementValue_FP, funkChunk,
																		(*thisDevice.fInterface)->setElementValue);
	result = pFsetElementValue(thisDevice.fInterface, theElement.fCookie, &anEvent, 0,
															NULL_PTR, NULL_PTR, NULL_PTR);
	if (result != noErr)
		LOG_ERROR_4("%s: problem setting value (%d/0x%x)", name, result, result)
	if (buffer)
		FREEBYTES(buffer, extendedValueLength) 
 #endif /* not COMPILE_FOR_STUB */
} /* setHIDElementValue */
#endif /* COMPILE_FOR_OSX_4 */

#if defined(COMPILE_FOR_OS9_4)	 	 
/*------------------------------------ setHIDElementValue ---*/
void setHIDElementValue
	(Pchar									name,
	 HIDDeviceDataStruct &	thisDevice,
	 HIDElementDataStruct &	theElement,
	 UInt32									extendedValueLength,
	 PAtom									extendedValue,
	 long										singleValue,
	 OSStatus &							result)
{
 #if defined(COMPILE_FOR_STUB)
  #pragma unused(name,thisDevice,theElement,extendedValueLength,extendedValue,singleValue,result)
 #else /* not COMPILE_FOR_STUB */
	Byte *				buffer = NULL_PTR;
	HIDReportType	kind;
	UInt8 *				report = NULL_PTR;
	ByteCount			reportLength = 0;

	switch (theElement.fType)
	{
		case kIOHIDElementTypeOutput:
			kind = kHIDOutputReport;
			break;
			
		case kIOHIDElementTypeFeature:
			kind = kHIDFeatureReport;
			break;
			
		default:
			kind = kHIDInputReport;
			break;
			
	}
	result = HIDGetReportLength(kind, static_cast<UInt8>(theElement.fReportID),
															&reportLength, thisDevice.fPrepReport);
	if (result == kHIDSuccess)
	{
		report = GETBYTES(reportLength, UInt8);
		*report = static_cast<UInt8>(theElement.fReportID);
		if (extendedValueLength && extendedValue)
		{
			buffer = GETBYTES(extendedValueLength, Byte);
			for (UInt32 ii = 0; ii < extendedValueLength; ++ii)
				*(buffer + ii) = static_cast<Byte>((extendedValue + ii)->a_w.w_long);
			result = HIDSetUsageValueArray(kind, static_cast<HIDUsage>(theElement.fUsagePage),
																			0/*collection*/, static_cast<HIDUsage>(theElement.fUsage),
																			buffer, extendedValueLength, thisDevice.fPrepReport,
																			report, reportLength);
		}
		else
			result = HIDSetUsageValue(kind, static_cast<HIDUsage>(theElement.fUsagePage),
																0/*collection*/, static_cast<HIDUsage>(theElement.fUsage),
																singleValue, thisDevice.fPrepReport, report, reportLength);
		if (result == kHIDSuccess)
		{
			result = (*thisDevice.fDispatchTable->pHIDSetReport)(thisDevice.fConnection, 
																													kind, theElement.fReportID,
																													report, reportLength);
			if (result != kHIDSuccess)
				LOG_ERROR_4("%s: problem sending report (%d/0x%x)", name, result, result)
		}
		else
			LOG_ERROR_4("%s: problem setting value (%d/0x%x)", name, result, result)
	}
	else
		LOG_ERROR_4("%s: problem getting report length (%d/0x%x)", name, result, result)
	if (buffer)
		FREEBYTES(buffer, extendedValueLength);
	if (report)
		FREEBYTES(report, reportLength);
 #endif /* not COMPILE_FOR_STUB */
} /* setHIDElementValue */
#endif /* COMPILE_FOR_OS9_4 */

#if defined(COMPILE_FOR_STUB)
 #pragma export list setHIDElementValue
#endif /* COMPILE_FOR_STUB */