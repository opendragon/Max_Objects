/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       getHIDElementValue.c                                                    */
/*                                                                                      */
/*  Contains:   The routine getHIDElementValue().                                       */
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

#if defined(COMPILE_FOR_CATS)
/*------------------------------------ getHIDElementValue ---*/
long getHIDElementValue
	(Pchar									name,
	 HIDDeviceDataStruct &	thisDevice,
	 HIDElementDataStruct &	theElement,
	 UInt32 &								extendedValueLength,
	 Pvoid &								extendedValue,
	 IOReturn &							result)
{
 #if defined(COMPILE_FOR_STUB)
  #pragma unused(name,thisDevice,theElement,extendedValueLength,extendedValue,result)
 	return 0;
 #else /* not COMPILE_FOR_STUB */
  #pragma unused(name)
	IOHIDEventStruct				anEvent;
	TransferVector_rec			funkChunk;
	hidGetElementValue_FP		pFgetElementValue;
	hidQueryElementValue_FP	pFqueryElementValue;
	
	switch (theElement.fType)
	{
		case kIOHIDElementTypeFeature:
			pFqueryElementValue = fillCallback(hidQueryElementValue_FP, funkChunk,
																					(*thisDevice.fInterface)->queryElementValue);
			result = pFqueryElementValue(thisDevice.fInterface, theElement.fCookie, &anEvent, 0,
																		NULL_PTR, NULL_PTR, NULL_PTR); 
			if (result == KERN_SUCCESS)
			{
				extendedValueLength = anEvent.longValueSize;
				extendedValue = anEvent.longValue;
				return anEvent.value;
				
			}
			// fall into the 'input' mechanism

		case kIOHIDElementTypeInput_Misc:
		case kIOHIDElementTypeInput_Button:
		case kIOHIDElementTypeInput_Axis:
		case kIOHIDElementTypeInput_ScanCodes:
			pFgetElementValue = fillCallback(hidGetElementValue_FP, funkChunk,
																				(*thisDevice.fInterface)->getElementValue);
			result = pFgetElementValue(thisDevice.fInterface, theElement.fCookie, &anEvent);
			if (result == KERN_SUCCESS)
			{
				extendedValueLength = anEvent.longValueSize;
				extendedValue = anEvent.longValue;
				return anEvent.value;
				
			}
					
	}
	extendedValueLength = 0;
	extendedValue = NULL_PTR;
	return 12345;	
 #endif /* not COMPILE_FOR_STUB */
} /* getHIDElementValue */
#endif /* COMPILE_FOR_CATS */

#if defined(COMPILE_FOR_CATS) && defined(COMPILE_FOR_STUB)
 #pragma export list getHIDElementValue
#endif /* COMPILE_FOR_CATS and COMPILE_FOR_STUB */