/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       RFID2.c                                                                 */
/*                                                                                      */
/*  Contains:   The main entry points for this phidget.                                 */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2005 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2005/07/29                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

//#define USE_SYSLOG /* */
//#define USE_DEFAULT /* */

#include "RFID2.h"
#include "genericListOutput.h"

static const int	RFID2_TAG_SIZE = 7;
static const int	EXPECTED_INPUT_SIZE = 56;
static const int	EXPECTED_OUTPUT_SIZE = 32;

static PSymbol	lOffSymbol = NULL_PTR;
static PSymbol	lOnSymbol = NULL_PTR;

static char lHexDigits[] =
	{ '0', '1', '2', '3', '4', '5', '6', '7',
		'8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

/*------------------------------------ makeSymbolFromValues ---*/
static PSymbol makeSymbolFromValues
	(Ptr	values)
{
	char	buffer[12];

	buffer[0] = '_';
	for (int ii = 1, jj = 1; ii <= 5; ii++, jj += 2)
	{
		int	nextByte = *(values + ii) & 0x00FF;

		buffer[jj] = lHexDigits[nextByte / 16];
		buffer[jj + 1] = lHexDigits[nextByte % 16];
	}
	buffer[11] = 0;
	return gensym(buffer);
} /* makeSymbolFromValues */

#if defined(COMPILE_FOR_CATS)
/*------------------------------------ inputCallback ---*/
static void inputCallback
	(STANDARD_HID_ARGS_INPUTEVENTHANDLER)
{
 #pragma unused(elementCookie,value)
	PhidgRefConPtr	data = reinterpret_cast<PhidgRefConPtr>(refCon);
	PrivatePtr			privateData = reinterpret_cast<PrivatePtr>(data->fPrivateStorage);
	
	if (privateData->fActive)
	{
		// Construct the symbol from the seven bytes:
		if (longValueSize == RFID2_TAG_SIZE)
		{
			Atom	reportList[3];
			
			privateData->fTagSeen = makeSymbolFromValues(reinterpret_cast<Ptr>(longValue));
			SETSYM(reportList, data->fDeviceType);
			SETSYM(reportList + 1, data->fThisDevice->fSerialNumber);
			SETSYM(reportList + 2, privateData->fTagSeen);
			genericListOutput(data->fOutlet, 3, reportList);
		}
	}		
} /* inputCallback */
#endif /* COMPILE_FOR_CATS */

#if defined(COMPILE_FOR_CATS)
/*------------------------------------ defineCallback ---*/
E_PhidgResult defineCallback
	(STANDARD_PHID_ARGS_DEFINECALLBACK)
{
 #pragma unused(name,privateStorage,thisDevice)
	SharedPtr	sharedData = reinterpret_cast<SharedPtr>(sharedStorage);

	*aFun = inputCallback;
	*cookies = &sharedData->fInputCookie;
	*numCookies = 1;
	*result = noErr;
	return kPhidgSuccess;
} /* defineCallback */
#endif /* COMPILE_FOR_CATS */

/*------------------------------------ doCustom ---*/
E_PhidgResult doCustom
  (STANDARD_PHID_ARGS_DO)
{
#if defined(USE_DEFAULT)
 #pragma unused(name,deviceType,sharedStorage,outlet,privateStorage,thisDevice,argc,argv)
	*result = noErr;
	return kPhidgDoDefault;
#else /* not USE_DEFAULT */
 #pragma unused(outlet)
	PrivatePtr	privateData = reinterpret_cast<PrivatePtr>(privateStorage);
	SharedPtr		sharedData = reinterpret_cast<SharedPtr>(sharedStorage);
	
	*result = noErr;
	if (argc == 1)
	{
		bool	okSoFar = true, doOff = false;
		
		if (argv[0].a_type == A_SYM)
		{
			if (argv[0].a_w.w_sym == lOffSymbol)
				doOff = true;	
			else if (argv[0].a_w.w_sym != lOnSymbol)
			{
				okSoFar = false;
				LOG_ERROR_4("%s: unknown operation ('%s') for '%s:do'", name, argv[0].a_w.w_sym->s_name,
										deviceType->s_name)
			}
		}
		else
		{
			okSoFar = false;
			LOG_ERROR_3("%s: operation is not a symbol for '%s:do'", name, deviceType->s_name)
		}
		if (okSoFar)
		{
			if (doOff == privateData->fActive)
			{
				if (doOff)
					// Deactivate
					privateData->fActive = false;
				else
				{
					// Activate
			  	HIDElementDataPtr	anElement = NULL_PTR;
			  	
			  	// Find the matching element:
			 		for (anElement = thisDevice->fFirstElement; anElement; anElement = anElement->fNext)
			 		{
			 			if (anElement->fCookie == sharedData->fOutputCookie)
			 				break;
			 				
			 		}
				  if (anElement)
				  {
				  	long			singleValue = -1; // this value seems to work...
 #if defined(COMPILE_FOR_CATS)
						IOReturn	result2;
 #else /* not COMPILE_FOR_CATS */
						OSStatus	result2;
 #endif /* not COMPILE_FOR_CATS */
						
						setHIDElementValue(name, *thisDevice, *anElement, 0, NULL_PTR, singleValue, result2);
						privateData->fActive = true;
						*result = static_cast<OSErr>(result2);
				  }
				  else
				  	LOG_ERROR_3("%s: element not found for '%s:do'", name, deviceType->s_name)					
				}
			}
		}
	}
	else if (argc)
		LOG_ERROR_3("%s: extra values for '%s:do'", name, deviceType->s_name)
	else
		LOG_ERROR_3("%s: missing operation for '%s:do'", name, deviceType->s_name)
	return kPhidgSuccess;
#endif /* not USE_DEFAULT */
} /* doCustom */

/*------------------------------------ doGet ---*/
E_PhidgResult doGet
  (STANDARD_PHID_ARGS_DO)
{
#if defined(USE_DEFAULT)
 #pragma unused(name,deviceType,outlet,sharedStorage,privateStorage,thisDevice,argc,argv)
	*result = noErr;
	return kPhidgDoDefault;
#else /* not USE_DEFAULT */
 #pragma unused(argv)
	PrivatePtr	privateData = reinterpret_cast<PrivatePtr>(privateStorage);
	SharedPtr		sharedData = reinterpret_cast<SharedPtr>(sharedStorage);
 
	*result = noErr;
	if (argc)
		LOG_ERROR_3("%s: extra arguments for '%s:get'", name, deviceType->s_name)
	else
	{
 #if defined(COMPILE_FOR_CATS)
		// Get the raw data
  	HIDElementDataPtr	anElement = NULL_PTR;
  	
  	// Find the matching element:
 		for (anElement = thisDevice->fFirstElement; anElement; anElement = anElement->fNext)
 		{
 			if (anElement->fCookie == sharedData->fInputCookie)
 				break;
 				
 		}
	  if (anElement)
	  {
			UInt32		extendedLength;
			Pvoid			extendedValue;
			IOReturn	result2;
			long			value = getHIDElementValue(name, *thisDevice, *anElement, extendedLength,
 																					extendedValue, result2);
			
			if (result2 == KERN_SUCCESS)
			{
				if (extendedLength == RFID2_TAG_SIZE)
				{
					Atom	reportList[3];
					
					privateData->fTagSeen = makeSymbolFromValues(reinterpret_cast<Ptr>(extendedValue));
					SETSYM(reportList, deviceType);
					SETSYM(reportList + 1, thisDevice->fSerialNumber);
					SETSYM(reportList + 2, privateData->fTagSeen);
					genericListOutput(outlet, 3, reportList);
				}
				else
					LOG_ERROR_4("%s: bad value length (%d) for '%s:get'", name, extendedLength,
											deviceType->s_name)
			}
			else
				*result = static_cast<OSErr>(result2);
	  }
	  else
	  	LOG_ERROR_3("%s: element not found for '%s:get'", name, deviceType->s_name)
 #else /* not COMPILE_FOR_CATS */
		if (privateData->fTagSeen)
		{
			Atom	reportList[3];

			SETSYM(reportList, deviceType);
			SETSYM(reportList + 1, thisDevice->fSerialNumber);
			SETSYM(reportList + 2, privateData->fTagSeen);
			genericListOutput(outlet, 3, reportList);
		}
		*result = noErr;
 #endif /* not COMPILE_FOR_CATS */
	}
	return kPhidgSuccess;
#endif /* not USE_DEFAULT */
} /* doGet */

/*------------------------------------ doPut ---*/
E_PhidgResult doPut
  (STANDARD_PHID_ARGS_DO)
{
#pragma unused(name,deviceType,outlet,sharedStorage,privateStorage,thisDevice,argc,argv)
#if defined(USE_DEFAULT)
	*result = noErr;
	return kPhidgDoDefault;
#else /* not USE_DEFAULT */
	*result = noErr;
	return kPhidgSuccess;
#endif /* not USE_DEFAULT */
} /* doPut */

/*------------------------------------ identify ---*/
OSErr identify
  (STANDARD_PHID_ARGS_IDENTIFY)
{
#pragma unused(name)
	*productID = 0x031;
	*privateSize = sizeof(PrivateData);
	*sharedSize = sizeof(SharedData);
#if defined(COMPILE_FOR_CATS)
	*isAsynchronous = true;
#endif /* COMPILE_FOR_CATS */
  return noErr;
} /* identify */

/*------------------------------------ main ---*/
OSErr main
  (STANDARD_PHID_ARGS_MAIN)
{
#if defined(COMPILE_FOR_CATS)
#pragma unused(name)
#else /* not COMPILE_FOR_CATS */
 #pragma unused(name,environment)
#endif /* not COMPILE_FOR_CATS */
	STANDARD_MAIN_CODE;
	SharedPtr	sharedData = reinterpret_cast<SharedPtr>(sharedStorage);
	
	if (sharedData)
		sharedData->fCookiesValid = false;
	lOffSymbol = gensym("off");
	lOnSymbol = gensym("on");
  return noErr;
} /* main */

/*------------------------------------ niam ---*/
OSErr niam
  (STANDARD_PHID_ARGS_NIAM)
{
#pragma unused(name,sharedStorage)
  return noErr;
} /* niam */

/*------------------------------------ onAttach ---*/
E_PhidgResult onAttach
  (STANDARD_PHID_ARGS_ATTACH)
{
#if defined(USE_DEFAULT)
 #pragma unused(name,deviceType,sharedStorage,privateStorage,thisDevice)
	*result = noErr;
	return kPhidgDoDefault;
#else /* not USE_DEFAULT */
 #pragma unused(name,deviceType)
	PrivatePtr	privateData = reinterpret_cast<PrivatePtr>(privateStorage);
	SharedPtr		sharedData = reinterpret_cast<SharedPtr>(sharedStorage);

	if (privateData)
	{
		privateData->fTagSeen = NULL_PTR;
		privateData->fActive = false;
	}
	if (sharedData)
	{
		if (! sharedData->fCookiesValid)
		{
			bool	foundIn = false, foundOut = false;
			
			// Locate the input and output elements
			for (HIDElementDataPtr walker = thisDevice->fFirstElement; walker;
						walker = walker->fNext)
			{
				switch (walker->fType)
				{
					case kIOHIDElementTypeInput_Misc:
					case kIOHIDElementTypeInput_Button:
					case kIOHIDElementTypeInput_Axis:
					case kIOHIDElementTypeInput_ScanCodes:
						if (walker->fSize == EXPECTED_INPUT_SIZE)
						{
							foundIn = true;
							sharedData->fInputCookie = walker->fCookie;
						}
						break;

					case kIOHIDElementTypeOutput:
						if (walker->fSize == EXPECTED_OUTPUT_SIZE)
						{
							foundOut = true;
							sharedData->fOutputCookie = walker->fCookie;
						}
						break;

				}		
			}
			sharedData->fCookiesValid = (foundIn && foundOut);
		}
	}
	*result = noErr;
	return kPhidgSuccess;
#endif /* not USE_DEFAULT */
} /* onAttach */

/*------------------------------------ onDetach ---*/
E_PhidgResult onDetach
  (STANDARD_PHID_ARGS_ATTACH)
{
#pragma unused(name,deviceType,sharedStorage,privateStorage,thisDevice)
#if defined(USE_DEFAULT)
	*result = noErr;
	return kPhidgDoDefault;
#else /* not USE_DEFAULT */
	*result = noErr;
	return kPhidgSuccess;
#endif /* not USE_DEFAULT */
} /* onDetach */

#if (! defined(COMPILE_FOR_CATS))
/*------------------------------------ reportHandler ---*/
void reportHandler
	(STANDARD_PHID_ARGS_REPORTHANDLER)
{
 #if defined(USE_DEFAULT)
  #pragma unused(name,deviceType,sharedStorage,privateStorage,outlet,thisDevice,outlet,inHIDReport,inHIDReportLength)
 #else /* not USE_DEFAULT */
	// Find the element descriptor that we want:
	PrivatePtr				privateData = reinterpret_cast<PrivatePtr>(privateStorage);
	SharedPtr					sharedData = reinterpret_cast<SharedPtr>(sharedStorage);
	HIDElementDataPtr	anElement = NULL_PTR;
	
	// Find the matching element:
	for (anElement = thisDevice->fFirstElement; anElement; anElement = anElement->fNext)
	{
		if (anElement->fCookie == sharedData->fInputCookie)
			break;
			
	}
  if (anElement)
  {
		HIDReportType	kind;

		switch (anElement->fType)
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
		Byte			valueBuffer[RFID2_TAG_SIZE];
		OSStatus	result = HIDGetUsageValueArray(kind, static_cast<UInt32>(anElement->fUsagePage),
																							0, static_cast<UInt32>(anElement->fUsage),
																							valueBuffer, sizeof(valueBuffer),
																							thisDevice->fPrepReport, inHIDReport, inHIDReportLength);  
  	
  	if (result == noErr)
  	{
			if (privateData->fActive && privateData->fTagSeen)
			{
				Atom	reportList[3];

				privateData->fTagSeen = makeSymbolFromValues(reinterpret_cast<Ptr>(valueBuffer));
				SETSYM(reportList, deviceType);
				SETSYM(reportList + 1, thisDevice->fSerialNumber);
				SETSYM(reportList + 2, privateData->fTagSeen);
				genericListOutput(outlet, 3, reportList);
			}
  	}
  	else
  		LOG_ERROR_4("%s: error getting usage value (%d/0x%x)", name, result, result) 
	}
 #endif /* not USE_DEFAULT */
} /* reportHandler */
#endif /* not COMPILE_FOR_CATS */
