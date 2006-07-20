/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       QuadServo.c                                                             */
/*                                                                                      */
/*  Contains:   The main entry points for this phidget.                                 */
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
/*  Created:    2003/12/19                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

//#define USE_SYSLOG /* */
//#define USE_DEFAULT /* */

#include "QuadServo.h"
#include "genericListOutput.h"

static const int	SERVO_REQUEST_SIZE = 6;
static const int	EXPECTED_INPUT_SIZE = 16;
static const int	EXPECTED_OUTPUT_SIZE = 48;

static PSymbol	lAllSymbol = NULL_PTR;
static PSymbol	lSiSymbol = NULL_PTR;
static PSymbol	lSoSymbol = NULL_PTR;

#if defined(COMPILE_FOR_CATS)
/*------------------------------------ defineCallback ---*/
E_PhidgResult defineCallback
	(STANDARD_PHID_ARGS_DEFINECALLBACK)
{
 #pragma unused(name,sharedStorage,privateStorage,thisDevice,aFun,cookies,numCookies)
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
 #pragma unused(name,deviceType,outlet,sharedStorage,privateStorage,thisDevice,argc,argv)
	*result = noErr;
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
 #pragma unused(sharedStorage)
	bool				okSoFar = false, doAll = false;
	long				index = 0;
	PrivatePtr	privateData = reinterpret_cast<PrivatePtr>(privateStorage);

	if (argc == 2)
	{
		if (argv[0].a_type == A_SYM)
		{
			if (argv[0].a_w.w_sym == lSiSymbol)
			{
				if (argv[1].a_type == A_LONG)
				{
					index = argv[1].a_w.w_long;
					okSoFar = ((index >= 1) && (index <= NUM_SERVOS));
					if (! okSoFar)
						LOG_ERROR_3("%s: servo is out of range for '%s:get'", name, deviceType->s_name)
				}
				else
					LOG_ERROR_3("%s: servo is not an integer for '%s:get'", name, deviceType->s_name)
			}
			else
				LOG_ERROR_4("%s: invalid selection ('%s') for '%s:get'", name,
										argv[0].a_w.w_sym->s_name, deviceType->s_name)
		}
		else
			LOG_ERROR_3("%s: selection is not a symbol for '%s:get'", name, deviceType->s_name)
		okSoFar = true;
	}
	else if (argc == 1)
	{
		if (argv[0].a_type == A_SYM)
		{
			if (argv[0].a_w.w_sym == lAllSymbol)
				doAll = okSoFar = true;
			else
				LOG_ERROR_4("%s: invalid selection ('%s') for '%s:get'", name,
										argv[0].a_w.w_sym->s_name, deviceType->s_name)
		}
		else
			LOG_ERROR_3("%s: selection is not a symbol for '%s:get'", name, deviceType->s_name)
	}
	else if (argc)
		LOG_ERROR_3("%s: extra arguments for '%s:get'", name, deviceType->s_name)
	else
		LOG_ERROR_3("%s: missing type (all or si) for '%s:get'", name, deviceType->s_name)
	if (okSoFar)
	{
		if (doAll)
		{
			Atom	reportList[NUM_SERVOS + 3];

			SETSYM(reportList, deviceType);
			SETSYM(reportList + 1, thisDevice->fSerialNumber);
			SETSYM(reportList + 2, lAllSymbol);
			for (index = 0; index < NUM_SERVOS; index++)
				SETFLOAT(reportList + index + 3, privateData->fServo[index]);
			genericListOutput(outlet, NUM_SERVOS + 3, reportList);
		}
		else
		{
			Atom	reportList[4];
			
			SETSYM(reportList, deviceType);
			SETSYM(reportList + 1, thisDevice->fSerialNumber);
			SETSYM(reportList + 2, lSiSymbol);
			SETFLOAT(reportList + 3, privateData->fServo[index - 1]);
			genericListOutput(outlet, 4, reportList);
		}
	}
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
	bool				okSoFar = false, doAll = false;
	float				positions[NUM_SERVOS];
	long				index;
	PrivatePtr	privateData = reinterpret_cast<PrivatePtr>(privateStorage);
	SharedPtr		sharedData = reinterpret_cast<SharedPtr>(sharedStorage);
	
	*result = noErr;
	for (index = 0; index < NUM_SERVOS; index++)
		positions[index] = privateData->fServo[index];
	if (argc == 3)
	{
		if (argv[0].a_type == A_SYM)
		{
			if (argv[0].a_w.w_sym == lSoSymbol)
			{
				if (argv[1].a_type == A_LONG)
				{
					index = argv[1].a_w.w_long;
					okSoFar = ((index >= 1) && (index <= NUM_SERVOS));
					if (okSoFar)
					{
						float	percentage;
						
						if (argv[2].a_type == A_LONG)
							percentage = argv[2].a_w.w_long;
						else if (argv[2].a_type == A_FLOAT)
							percentage = argv[2].a_w.w_float;
						else
						{
							LOG_ERROR_3("%s: percentage is not a number for '%s:get'", name, deviceType->s_name)
							okSoFar = false;
						}
						if (percentage < 0)
							percentage = 0;
						else if (percentage > 100)
							percentage = 100;
						positions[index - 1] = percentage;
					}
					else
						LOG_ERROR_3("%s: servo is out of range for '%s:get'", name, deviceType->s_name)
				}
				else
					LOG_ERROR_3("%s: servo is not an integer for '%s:get'", name, deviceType->s_name)
			}
			else
				LOG_ERROR_4("%s: invalid selection ('%s') for '%s:get'", name,
										argv[0].a_w.w_sym->s_name, deviceType->s_name)
		}
		else
			LOG_ERROR_3("%s: selection is not a symbol for '%s:get'", name, deviceType->s_name)
		okSoFar = true;
	}
	else if (argc == (NUM_SERVOS + 1))
	{
		if (argv[0].a_type == A_SYM)
		{
			if (argv[0].a_w.w_sym == lAllSymbol)
				doAll = true;
			else
				LOG_ERROR_4("%s: invalid selection ('%s') for '%s:get'", name,
										argv[0].a_w.w_sym->s_name, deviceType->s_name)
			if (doAll)
			{
				// Check the rest of the arguments - either float or integer...
				okSoFar = true;
				for (index = 1; (index < argc) && okSoFar; index++)
				{
					float	percentage;
					
					if (argv[index].a_type == A_LONG)
						percentage = argv[index].a_w.w_long;
					else if (argv[index].a_type == A_FLOAT)
						percentage = argv[index].a_w.w_float;
					else
					{
						LOG_ERROR_3("%s: one or more percentages are not a number for '%s:get'",
												name, deviceType->s_name)
						okSoFar = false;
					}
					if (okSoFar)
					{
						if (percentage < 0)
							percentage = 0;
						else if (percentage > 100)
							percentage = 100;
						positions[index - 1] = percentage;
					}
				}
			}
		}
		else
			LOG_ERROR_3("%s: selection is not a symbol for '%s:get'", name, deviceType->s_name)
	}
	else if (argc)
		LOG_ERROR_3("%s: missing or extra arguments for '%s:get'", name, deviceType->s_name)
	else
		LOG_ERROR_3("%s: missing type (all or si) for '%s:get'", name, deviceType->s_name)
	if (okSoFar)
	{
		int		pulse1 = int((positions[0] * 180 * 10.6f / 100) + 230); 
		int		pulse2 = int((positions[1] * 180 * 10.6f / 100) + 230); 
		int		pulse3 = int((positions[2] * 180 * 10.6f / 100) + 230); 
		int		pulse4 = int((positions[3] * 180 * 10.6f / 100) + 230); 
		Atom	servoVector[SERVO_REQUEST_SIZE];
		
		SETLONG(servoVector, pulse1 % 256);
		SETLONG(servoVector + 1, (pulse1 / 256) | ((pulse2 / 256) * 16));
		SETLONG(servoVector + 2, pulse2 % 256);
		SETLONG(servoVector + 3, pulse3 % 256);
		SETLONG(servoVector + 4, (pulse3 / 256) | ((pulse4 / 256) * 16));
		SETLONG(servoVector + 5, pulse4 % 256);
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

				setHIDElementValue(name, *thisDevice, *anElement, SERVO_REQUEST_SIZE, servoVector,
														0, result2);
				for (index = 0; index < NUM_SERVOS; index++)
					privateData->fServo[index] = positions[index];
				*result = static_cast<OSErr>(result2);
		  }
		  else
		  	LOG_ERROR_3("%s: element not found for '%s:put'", name, deviceType->s_name)
		}
	}
	return kPhidgSuccess;
#endif /* not USE_DEFAULT */
} /* doPut */

/*------------------------------------ identify ---*/
OSErr identify
  (STANDARD_PHID_ARGS_IDENTIFY)
{
#if defined(COMPILE_FOR_CATS)
 #pragma unused(name,isAsynchronous)
#else /* not COMPILE_FOR_CATS */
 #pragma unused(name)
#endif /* not COMPILE_FOR_CATS */
	*productID = 0x038;
	*privateSize = sizeof(PrivateData);
	*sharedSize = sizeof(SharedData);
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
	lAllSymbol = gensym("all");
	lSiSymbol = gensym("si");
	lSoSymbol = gensym("so");
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
 #pragma unused(name,deviceType,privateStorage)
  PrivatePtr	privateData = reinterpret_cast<PrivatePtr>(privateStorage);
	SharedPtr		sharedData = reinterpret_cast<SharedPtr>(sharedStorage);
	
	if (privateData)
	{
		for (long index = 0; index < NUM_SERVOS; index++)
			privateData->fServo[index] = 0;
	}
	if (sharedData)
	{
		if (! sharedData->fCookiesValid)
		{
			bool	foundOut = false;
			
			// Locate the input and output elements
			for (HIDElementDataPtr walker = thisDevice->fFirstElement; walker;
						walker = walker->fNext)
			{
				switch (walker->fType)
				{
					case kIOHIDElementTypeOutput:
						if (walker->fSize == EXPECTED_OUTPUT_SIZE)
						{
							foundOut = true;
							sharedData->fOutputCookie = walker->fCookie;
						}
						break;

				}		
			}
			sharedData->fCookiesValid = foundOut;
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
 #pragma unused(name,deviceType,sharedStorage,privateStorage,thisDevice,outlet,inHIDReport,inHIDReportLength)
} /* reportHandler */
#endif /* not COMPILE_FOR_CATS */
