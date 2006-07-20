/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       TextLCD088.c                                                            */
/*                                                                                      */
/*  Contains:   The main entry points for this phidget.                                 */
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
/*  Created:    2004/01/07                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

//#define USE_SYSLOG /* */
//#define USE_DEFAULT /* */

#include "TextLCD088.h"
#include "TextLCDSupport.h"
#include "genericListOutput.h"
#include "HD44780.h"

static const int	MAX_INDEX = 7;
static const int	DIGITAL_REPORT_SIZE = MAX_INDEX + 5;
static const int	EXPECTED_INPUT_SIZE = 16;
static const int	EXPECTED_OUTPUT_SIZE = 64;

static PSymbol	lBacklightSymbol = NULL_PTR;
static PSymbol	lBlinkSymbol = NULL_PTR;
static PSymbol	lClearSymbol = NULL_PTR;
static PSymbol	lCursorSymbol = NULL_PTR;
static PSymbol	lDiSymbol = NULL_PTR;
static PSymbol	lDisplaySymbol = NULL_PTR;
static PSymbol	lDoSymbol = NULL_PTR;
static PSymbol	lEntryModeSymbol = NULL_PTR;
static PSymbol	lFlipSymbol = NULL_PTR;
static PSymbol	lGoSymbol = NULL_PTR;
static PSymbol	lLeftSymbol = NULL_PTR;
static PSymbol	lOffSymbol = NULL_PTR;
static PSymbol	lOnSymbol = NULL_PTR;
static PSymbol	lReverseSymbol = NULL_PTR;
static PSymbol	lRightSymbol = NULL_PTR;
static PSymbol	lShiftSymbol = NULL_PTR;
static PSymbol	lToSymbol = NULL_PTR;
static PSymbol	lTriggerSymbol = NULL_PTR;
static PSymbol	lWriteSymbol = NULL_PTR;

#if defined(COMPILE_FOR_CATS)
/*------------------------------------ inputCallback ---*/
static void inputCallback
	(STANDARD_HID_ARGS_INPUTEVENTHANDLER)
{
 #pragma unused(elementCookie,longValue,longValueSize)
	PhidgRefConPtr	data = reinterpret_cast<PhidgRefConPtr>(refCon);
	PrivatePtr			privateData = reinterpret_cast<PrivatePtr>(data->fPrivateStorage);
	
	if (privateData->fDigitalInputTriggerEnabled)
	{
		Atom	reportList[DIGITAL_REPORT_SIZE];

		// Extract the output bit vector and concatenate with the raw bits
		SETSYM(reportList, data->fDeviceType);
		SETSYM(reportList + 1, data->fThisDevice->fSerialNumber);
		SETSYM(reportList + 2, lDiSymbol);
		SETLONG(reportList + 3, value);
		for (UInt32 ii = 0; ii <= MAX_INDEX; ii++)
			SETLONG(reportList + ii + 4, (value & (1 << ii)) ? 1 : 0);
		genericListOutput(data->fOutlet, DIGITAL_REPORT_SIZE, reportList);
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
 #pragma unused(name,deviceType,outlet,sharedStorage,privateStorage,thisDevice,argc,argv)
	*result = noErr;
	return kPhidgDoDefault;
#else /* not USE_DEFAULT */
 #pragma unused(outlet)
 #if defined(COMPILE_FOR_CATS)
	IOReturn		result2 = KERN_SUCCESS;
 #else /* not COMPILE_FOR_CATS */
	OSStatus		result2 = noErr;
 #endif /* not COMPILE_FOR_CATS */
	PrivatePtr	privateData = reinterpret_cast<PrivatePtr>(privateStorage);
	SharedPtr		sharedData = reinterpret_cast<SharedPtr>(sharedStorage);
	
	if (argc >= 2)
	{
		bool							okSoFar = true, doFlip = false, doOff = false, doText = false;
		bool							doTrigger = false;
  	HIDElementDataPtr	anElement = NULL_PTR;
  	
  	// Find the matching element:
 		for (anElement = thisDevice->fFirstElement; anElement; anElement = anElement->fNext)
 		{
 			if (anElement->fCookie == sharedData->fOutputCookie)
 				break;
 				
 		}
		if (! anElement)
		{
			LOG_ERROR_3("%s: element not found for '%s:do'", name, deviceType->s_name)
			okSoFar = false;
		}
		if (okSoFar)
		{
			if (argv[0].a_type == A_SYM)
			{
				PSymbol	check = argv[0].a_w.w_sym;
				
				if (check == lTriggerSymbol)
					doTrigger = true;
				else if (check == lToSymbol)
					doText = true;
				else if (check == lFlipSymbol)
					doFlip = true;
				else if (check == lOffSymbol)
					doOff = true;	
				else if (check != lOnSymbol)
				{
					okSoFar = false;
					LOG_ERROR_4("%s: unknown operation ('%s') for '%s:do'", name,
											check->s_name, deviceType->s_name)
				}
			}
			else
			{
				okSoFar = false;
				LOG_ERROR_3("%s: operation is not a symbol for '%s:do'", name, deviceType->s_name)
			}
		}
		if (doTrigger)
		{
			if (argv[1].a_type == A_SYM)
			{
				PSymbol	check = argv[1].a_w.w_sym;
				
				if (check == lOffSymbol)
				{
					if (argc == 2)
						privateData->fDigitalInputTriggerEnabled = false;
					else
						LOG_ERROR_3("%s: extra arguments for '%s:do'", name, deviceType->s_name)
				}
				else if (check == lOnSymbol)
				{
					if (argc == 2)
						privateData->fDigitalInputTriggerEnabled = true;
					else
						LOG_ERROR_3("%s: extra arguments for '%s:do'", name, deviceType->s_name)
				}
				else
					LOG_ERROR_4("%s: unknown trigger operation ('%s') for '%s:do'", name,
											check->s_name, deviceType->s_name)				
			}
			else
				LOG_ERROR_3("%s: trigger operation is not a symbol for '%s:do'", name,
										deviceType->s_name)
		}
		else if (doText)
		{
			// Check text operations for validity and perform them:
			if (argv[1].a_type == A_SYM)
			{
				bool		overCount = false, notNumber = false, notText = false;
				bool		unknownSymbol = false, badNumber = false;
				PSymbol	check = argv[1].a_w.w_sym;
				
				if (check == lOffSymbol)
				{
					if (argc == 2)
						result2 = doTextLCDOff(name, *thisDevice, *anElement);
					else
						overCount = true;
				}
				else if (check == lOnSymbol)
				{
						PSymbol	param;
						bool		sawCursor = false, sawBlink = false;
						
						if (argc >= 3)
						{
							if (argv[2].a_type == A_SYM)
							{
								param = argv[2].a_w.w_sym;
								if (param == lCursorSymbol)
									sawCursor = true;
								else if (param == lBlinkSymbol)
									sawBlink = true;
								else
									unknownSymbol = true;
							}
							else
								notText = true;
						}
						if (argc == 4)
						{
							if (argv[3].a_type == A_SYM)
							{
								param = argv[3].a_w.w_sym;
								if (param == lCursorSymbol)
									sawCursor = true;
								else if (param == lBlinkSymbol)
									sawBlink = true;
								else
									unknownSymbol = true;
							}
							else
								notText = true;
						}
						if ((! notText) && (! unknownSymbol))
							result2 = doTextLCDOn(name, *thisDevice, *anElement, sawCursor, sawBlink);
					}
				else if (check == lBacklightSymbol)
				{
					if (argc == 3)
					{
						if (argv[2].a_type == A_SYM)
						{
							PSymbol	param = argv[2].a_w.w_sym;
							
							if (param == lOffSymbol)
								result2 = doTextLCDBacklightOff(name, *thisDevice, *anElement);								
							else if (param == lOnSymbol)
								result2 = doTextLCDBacklightOn(name, *thisDevice, *anElement);
							else
								unknownSymbol = true;
						}
						else
							notText = true;	
					}
					else
						overCount = true;
				}
				else if (check == lEntryModeSymbol)
				{
					if (argc > 4)
						overCount = true;
					else
					{
						PSymbol	param;
						bool		sawReverse = false, sawShift = false;
						
						if (argc >= 3)
						{
							if (argv[2].a_type == A_SYM)
							{
								param = argv[2].a_w.w_sym;
								if (param == lReverseSymbol)
									sawReverse = true;
								else if (param == lShiftSymbol)
									sawShift = true;
								else
									unknownSymbol = true;
							}
							else
								notText = true;
						}
						if (argc == 4)
						{
							if (argv[3].a_type == A_SYM)
							{
								param = argv[3].a_w.w_sym;
								if (param == lReverseSymbol)
									sawReverse = true;
								else if (param == lShiftSymbol)
									sawShift = true;
								else
									unknownSymbol = true;
							}
							else
								notText = true;
						}
						if ((! notText) && (! unknownSymbol))
							result2 = doTextLCDEntryMode(name, *thisDevice, *anElement,
																						sawReverse, sawShift);
					}
				}
				else if (check == lDisplaySymbol)
				{
					if (argc == 3)
					{
						if (argv[2].a_type == A_SYM)
						{
							PSymbol	param = argv[2].a_w.w_sym;
							
							if (param == lLeftSymbol)
								result2 = doTextLCDShiftDisplay(name, *thisDevice, *anElement, false);
							else if (param == lRightSymbol)
								result2 = doTextLCDShiftDisplay(name, *thisDevice, *anElement, true);
							else
								unknownSymbol = true;							
						}
						else
							notText = true;	
					}
					else
						overCount = true;
				}
				else if (check == lCursorSymbol)
				{
					if (argc == 3)
					{
						if (argv[2].a_type == A_SYM)
						{
							PSymbol	param = argv[2].a_w.w_sym;
							
							if (param == lLeftSymbol)
								result2 = doTextLCDShiftCursor(name, *thisDevice, *anElement, false);
							else if (param == lRightSymbol)
								result2 = doTextLCDShiftCursor(name, *thisDevice, *anElement, true);
							else
								unknownSymbol = true;							
						}
						else
							notText = true;	
					}
					else
						overCount = true;
				}
				else if (check == lClearSymbol)
				{
					if (argc == 3)
					{
						if (argv[2].a_type == A_LONG)
						{
							long	row = argv[2].a_w.w_long;
							
							switch (row)
							{
								case 1:
									result2 = doTextLCDClearLine1(name, *thisDevice, *anElement);									
									break;
								
								case 2:
									result2 = doTextLCDClearLine2(name, *thisDevice, *anElement);									
									break;
								
								default:
									badNumber = true;
									break;
									
							}
						}
						else
							notNumber = true;	
					}
					else if (argc == 2)
						result2 = doTextLCDClear(name, *thisDevice, *anElement);
					else
						overCount = true;
				}
				else if (check == lGoSymbol)
				{
					long	row = 1;
					long	column = 1;
						
					if (argc == 4)
					{
						if (argv[2].a_type == A_LONG)
							row = argv[2].a_w.w_long;
						else
							notNumber = true;
						if (argv[3].a_type == A_LONG)
							column = argv[3].a_w.w_long;
						else
							notNumber = true;
					}
					else if (argc == 3)
					{
						if (argv[2].a_type == A_LONG)
							row = argv[2].a_w.w_long;
						else
							notNumber = true;
					}
					else if (argc != 2)
						overCount = true;
					if ((! overCount) && (! notNumber))
						result2 = doTextLCDGo(name, *thisDevice, *anElement, row, column);
				}
				else if (check == lWriteSymbol)
				{
					if (argc < 3)
						LOG_ERROR_3("%s: missing text parameters for '%s:do'", name, deviceType->s_name)
					else
					{
						// Convert the atoms into a text buffer... Note that the most that we can
						// write is CHARS_PER_LINE in any case.
						char		buffer[CHARS_PER_LINE + 1];
						char		numBuffer[10];
						size_t	lastChar = 0;
						Ptr			toAdd = NULL_PTR;
						
						memset(buffer, 0, sizeof(buffer));
						for (short ii = 2; ii < argc; ii++)
						{
							switch (argv[ii].a_type)
							{
		            case A_LONG:
		              sprintf(numBuffer, "%ld", argv[ii].a_w.w_long);
		              toAdd = numBuffer;
		              break;

		            case A_SYM:
		              toAdd = argv[ii].a_w.w_sym->s_name;
		              break;

		            case A_FLOAT:
		              sprintf(numBuffer, "%g", double(argv[ii].a_w.w_float));
		              toAdd = numBuffer;
		              break;

		            case A_SEMI:
		              toAdd = ";";
		              break;

		            case A_COMMA:
		              toAdd = ",";
		              break;

		            case A_DOLLAR:
		              toAdd = "$";
		              break;

		          }
		          if (toAdd)
		          {
		          	size_t	len = strlen(toAdd);
		          	
		          	if ((len + lastChar) > CHARS_PER_LINE)
		          		len = CHARS_PER_LINE - lastChar;
		          	memcpy(buffer + lastChar, toAdd, len);
		          	lastChar += len;
		          	if (lastChar >= CHARS_PER_LINE)
		          		break;
		          	
		          	if (ii < (argc - 1))
		          	{
		          		*(buffer + lastChar) = ' ';
		          		lastChar++;
		          		if (lastChar >= CHARS_PER_LINE)
		          			break;
		          			
		          	}
		          }
						}
						if (lastChar)
							result2 = doTextLCDWrite(name, *thisDevice, *anElement, buffer);	
					}
				}
				else
					LOG_ERROR_4("%s: unknown text operation ('%s') for '%s:do'", name,
											check->s_name, deviceType->s_name)
				if (overCount)
					LOG_ERROR_3("%s: extra arguments for '%s:do'", name, deviceType->s_name)
				else if (notText)
					LOG_ERROR_3("%s: text parameter is not a symbol for '%s:do'", name, deviceType->s_name)						
				else if (notNumber)
					LOG_ERROR_3("%s: text parameter is not a number for '%s:do'", name, deviceType->s_name)						
				else if (unknownSymbol)
					LOG_ERROR_3("%s: text parameter is not recognized for '%s:do'", name, deviceType->s_name)						
				else if (badNumber)
					LOG_ERROR_3("%s: text parameter is out of range for '%s:do'", name, deviceType->s_name)						
			}
			else
				LOG_ERROR_3("%s: text operation is not a symbol for '%s:do'", name, deviceType->s_name)
		}
		else if (okSoFar)
		{
			if (argc > 2)
				LOG_ERROR_3("%s: extra arguments for '%s:do'", name, deviceType->s_name)
			else if (argv[1].a_type == A_LONG)
			{
				long	index = argv[1].a_w.w_long - 1;
		  	long	singleValue = privateData->fDigitalOutputs;
				
				if (index < 0)
					index = 0;
				else if (index > MAX_INDEX)
					index = MAX_INDEX;
				if (doFlip)
					singleValue ^= (1 << index);
				else if (doOff)
					singleValue &= ~ (1 << index);
				else
					singleValue |= (1 << index);
				Atom			buffer[DATA_BUFFER_SIZE];
				
				SETLONG(buffer, singleValue);
				for (int ii = 1; ii < (DATA_BUFFER_SIZE - 1); ii++)
					SETLONG(buffer + ii, 0);
				SETLONG(buffer + DATA_BUFFER_SIZE - 1, 0x10); // request digital output
				setHIDElementValue(name, *thisDevice, *anElement, DATA_BUFFER_SIZE, buffer, 0, result2);	
				privateData->fDigitalOutputs = singleValue;
			}
			else
				LOG_ERROR_3("%s: non-integer value for '%s:do'", name, deviceType->s_name)
		}
	}
	else if (argc == 1)
		LOG_ERROR_3("%s: missing value for '%s:do'", name, deviceType->s_name)
	else
		LOG_ERROR_3("%s: missing operation and value for '%s:do'", name, deviceType->s_name)
	*result = static_cast<OSErr>(result2);
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
	Atom				reportList[DIGITAL_REPORT_SIZE];
	PrivatePtr	privateData = reinterpret_cast<PrivatePtr>(privateStorage);
	SharedPtr		sharedData = reinterpret_cast<SharedPtr>(sharedStorage);
 
	if (argc == 1)
	{
		bool	okSoFar = true, doInput = false;
		
		if (argv[0].a_type == A_SYM)
		{
			if (argv[0].a_w.w_sym == lDiSymbol)
				doInput = true;	
			else if (argv[0].a_w.w_sym != lDoSymbol)
			{
				okSoFar = false;
				LOG_ERROR_4("%s: unknown type ('%s') for '%s:get'", name, argv[0].a_w.w_sym->s_name,
										deviceType->s_name)
			}
		}
		else
		{
			okSoFar = false;
			LOG_ERROR_3("%s: type is not a symbol for '%s:get'", name, deviceType->s_name)
		}
		if (okSoFar)
		{
			if (doInput)
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
						// Extract the output bit vector and concatenate with the raw bits
						SETSYM(reportList, deviceType);
						SETSYM(reportList + 1, thisDevice->fSerialNumber);
						SETSYM(reportList + 2, lDiSymbol);
						SETLONG(reportList + 3, value);
						for (UInt32 ii = 0; ii <= MAX_INDEX; ii++)
							SETLONG(reportList + ii + 4, (value & (1 << ii)) ? 1 : 0);
						genericListOutput(outlet, DIGITAL_REPORT_SIZE, reportList);
						*result = static_cast<OSErr>(result2);
					}							
			  }
			  else
			  	LOG_ERROR_3("%s: element not found for '%s:get'", name, deviceType->s_name)	
#else /* not COMPILE_FOR_CATS */
				// Extract the output bit vector and concatenate with the raw bits
				SETSYM(reportList, deviceType);
				SETSYM(reportList + 1, thisDevice->fSerialNumber);
				SETSYM(reportList + 2, lDiSymbol);
				SETLONG(reportList + 3, privateData->fDigitalInput);
				for (UInt32 ii = 0; ii <= MAX_INDEX; ii++)
					SETLONG(reportList + ii + 4, (privateData->fDigitalInput & (1 << ii)) ? 1 : 0);
				genericListOutput(outlet, DIGITAL_REPORT_SIZE, reportList);
				*result = noErr;
#endif /* not COMPILE_FOR_CATS */
			}
			else
			{
				// report the state of the digital output
				SETSYM(reportList, deviceType);
				SETSYM(reportList + 1, thisDevice->fSerialNumber);
				SETSYM(reportList + 2, lDoSymbol);
				SETLONG(reportList + 3, privateData->fDigitalOutputs);
				for (UInt32 ii = 0; ii <= MAX_INDEX; ii++)
					SETLONG(reportList + ii + 4, (privateData->fDigitalOutputs & (1 << ii)) ? 1 : 0);
				genericListOutput(outlet, DIGITAL_REPORT_SIZE, reportList);
			}
		}
	}
	else if (argc)
		LOG_ERROR_3("%s: extra arguments for '%s:get'", name, deviceType->s_name)
	else
		LOG_ERROR_3("%s: missing type (di or do) for '%s:get'", name, deviceType->s_name)
	*result = noErr;
	return kPhidgSuccess;
#endif /* not USE_DEFAULT */
} /* doGet */

/*------------------------------------ doPut ---*/
E_PhidgResult doPut
  (STANDARD_PHID_ARGS_DO)
{
#if defined(USE_DEFAULT)
 #pragma unused(name,deviceType,outlet,sharedStorage,privateStorage,thisDevice,argc,argv)
	*result = noErr;
	return kPhidgDoDefault;
#else /* not USE_DEFAULT */
 #pragma unused(outlet)
	PrivatePtr	privateData = reinterpret_cast<PrivatePtr>(privateStorage);
	SharedPtr		sharedData = reinterpret_cast<SharedPtr>(sharedStorage);
	
	*result = noErr;
	if (argc == 1)
	{
		bool	okSoFar = true;
	  long	singleValue = 0;

		if (argv[0].a_type == A_LONG)
		{
			singleValue = argv[0].a_w.w_long;
			if (singleValue < 0)
				singleValue = 0;
			else if (singleValue > 0x00FF)
				singleValue = 0x00FF;
		}
		else
		{
			okSoFar = false;
			LOG_ERROR_3("%s: non-integer value for '%s:put'", name, deviceType->s_name)
		}
		if (okSoFar)
		{
	  	HIDElementDataPtr	anElement = NULL_PTR;
	  	
	  	// Find the matching element:
	 		for (anElement = thisDevice->fFirstElement; anElement; anElement = anElement->fNext)
	 		{
	 			if (anElement->fCookie == sharedData->fOutputCookie)
	 				break;
	 				
	 		}
		  if (anElement)
		  {
 #if defined(COMPILE_FOR_CATS)
				IOReturn	result2;
 #else /* not COMPILE_FOR_CATS */
				OSStatus	result2;
 #endif /* not COMPILE_FOR_CATS */
				Atom			buffer[DATA_BUFFER_SIZE];
				
				SETLONG(buffer, singleValue);
				for (int ii = 1; ii < (DATA_BUFFER_SIZE - 1); ii++)
					SETLONG(buffer + ii, 0);
				SETLONG(buffer + DATA_BUFFER_SIZE - 1, 0x10); // request digital output
				setHIDElementValue(name, *thisDevice, *anElement, DATA_BUFFER_SIZE, buffer, 0, result2);	
				privateData->fDigitalOutputs = singleValue;
				*result = static_cast<OSErr>(result2);
		  }
		  else
		  	LOG_ERROR_3("%s: element not found for '%s:put'", name, deviceType->s_name)
		}
	}
	else if (argc)
		LOG_ERROR_3("%s: extra values for '%s:put'", name, deviceType->s_name)
	else
		LOG_ERROR_3("%s: missing value for '%s:put'", name, deviceType->s_name)
	return kPhidgSuccess;
#endif /* not USE_DEFAULT */
} /* doPut */

/*------------------------------------ identify ---*/
OSErr identify
  (STANDARD_PHID_ARGS_IDENTIFY)
{
#pragma unused(name)
	*productID = 0x53;
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
	lBacklightSymbol = gensym("backlight");
	lBlinkSymbol = gensym("blink");
	lClearSymbol = gensym("clear");
	lCursorSymbol = gensym("cursor");
	lDiSymbol = gensym("di");
	lDisplaySymbol = gensym("display");
	lDoSymbol = gensym("do");
	lEntryModeSymbol = gensym("entrymode");
	lFlipSymbol = gensym("flip");
	lGoSymbol = gensym("go");
	lLeftSymbol = gensym("left");
	lOffSymbol = gensym("off");
	lOnSymbol = gensym("on");
	lReverseSymbol = gensym("reverse");
	lRightSymbol = gensym("right");
	lShiftSymbol = gensym("shift");
	lToSymbol = gensym("to");
	lTriggerSymbol = gensym("trigger");
	lWriteSymbol = gensym("write");
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
		privateData->fDigitalInputTriggerEnabled = false;
		privateData->fDigitalOutputs = privateData->fDigitalInput = 0;
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
#if defined(USE_DEFAULT)
 #pragma unused(name,deviceType,sharedStorage,privateStorage,thisDevice)
	*result = noErr;
	return kPhidgDoDefault;
#else /* not USE_DEFAULT */
 #pragma unused(name,deviceType,sharedStorage,privateStorage,thisDevice)
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
  #pragma unused(name,deviceType,sharedStorage,privateStorage,thisDevice,outlet,inHIDReport,inHIDReportLength)
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
		OSStatus	result = HIDGetUsageValue(kind, static_cast<UInt32>(anElement->fUsagePage), 0,
																					static_cast<UInt32>(anElement->fUsage),
																					&privateData->fDigitalInput, thisDevice->fPrepReport,
																			  	inHIDReport, inHIDReportLength);
  	
  	if (result == noErr)
  	{
			if (privateData->fDigitalInputTriggerEnabled)
			{
				Atom	reportList[DIGITAL_REPORT_SIZE];

				// Extract the output bit vector and concatenate with the raw bits
				SETSYM(reportList, deviceType);
				SETSYM(reportList + 1, thisDevice->fSerialNumber);
				SETSYM(reportList + 2, lDiSymbol);
				SETLONG(reportList + 3, privateData->fDigitalInput);
				for (UInt32 ii = 0; ii <= MAX_INDEX; ii++)
					SETLONG(reportList + ii + 4, (privateData->fDigitalInput & (1 << ii)) ? 1 : 0);
				genericListOutput(outlet, DIGITAL_REPORT_SIZE, reportList);
			}		
  	}
  	else
  		LOG_ERROR_4("%s: error getting usage value (%d/0x%x)", name, result, result) 
	}
 #endif /* not USE_DEFAULT */
} /* reportHandler */
#endif /* not COMPILE_FOR_CATS */
