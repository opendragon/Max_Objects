/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       fidget_Put.c                                                            */
/*                                                                                      */
/*  Contains:   The routine cmd_SetValue().                                             */
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
/*  Created:    2003/12/14                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "fidget.h"
#include "genericListOutput.h"

/*------------------------------------ performPut ---*/
static bool performPut
	(FidgetPtr				xx,
   HIDDeviceDataPtr	walkerHID,
   PSymbol					deviceType,
   const short			argc,
   PAtom						argv)
{
	bool					okSoFar = true;
 	OSErr					result;
	E_PhidgResult	action = kPhidgDoDefault;

	if (walkerHID->fClass)
	{
		PhidgetDescriptorPtr	kind = reinterpret_cast<PhidgetDescriptorPtr>(walkerHID->fClass);

#if defined(COMPILE_FOR_CATS)
		action = reinterpret_cast<FpDoPut>(kind->fDoPutFun)(OUR_NAME, deviceType, xx->fDataOut,
																												kind->fShared, walkerHID->fPrivate,
																												walkerHID, short(argc - 2),
																												argv + 2, &result);
#else /* not COMPILE_FOR_CATS */
		action = static_cast<E_PhidgResult>(CallUniversalProc(kind->fDoPutUpp, uppDoPutProcInfo,
																OUR_NAME, deviceType, xx->fDataOut, kind->fShared,
																walkerHID->fPrivate, walkerHID, short(argc - 2), argv + 2,
																&result));
#endif /* not COMPILE_FOR_CATS */
	}
	if (action == kPhidgDoDefault)
	{
		if (argc >= 4)
		{
		  long		element = 0, singleValue = 0;
			UInt32	extendedLength = 0;
			PAtom		extendedValue = NULL_PTR;

			if (argv[2].a_type == A_LONG)
				element = argv[2].a_w.w_long;
			else
			{
				okSoFar = false;
				LOG_ERROR_2(OUTPUT_PREFIX "element is not an integer for '%s:put'", deviceType->s_name)
			}
  		// Gather the data
  		if (okSoFar)
  		{
  			int	ii;
  			
  			for (ii = 3; ii < argc; ii++)
  			{
  				if (argv[ii].a_type != A_LONG)
  				{
  					okSoFar = false;
  					LOG_ERROR_2(OUTPUT_PREFIX "non-integer value for '%s:put'", deviceType->s_name)
  					break;
  					
  				}
  			}
  			if (okSoFar)
  			{
	  			if (argc == 4)
	  				singleValue = argv[3].a_w.w_long;
	  			else
	  			{
	  				extendedLength = static_cast<UInt32>(argc - 3);
	  				extendedValue = argv + 3;
	  			}
  			}
  		}
  		if (okSoFar)
  		{
		  	HIDElementDataPtr	anElement = NULL_PTR;
		  	
			  if (element)
			  {
			  	// Find the matching element:
			 		for (anElement = walkerHID->fFirstElement; anElement; anElement = anElement->fNext)
			 		{
			 			if (anElement->fCookie == reinterpret_cast<IOHIDElementCookie>(element))
			 				break;
			 				
			 		}
			  }
			  if (anElement)
			  {
 #if defined(COMPILE_FOR_CATS)
	 				IOReturn	result;
 #else /* not COMPILE_FOR_CATS */
	 				OSStatus	result;
 #endif /* not COMPILE_FOR_CATS */
	 				
	 				setHIDElementValue(OUR_NAME, *walkerHID, *anElement, extendedLength,
	 														extendedValue, singleValue, result);
  				if (result)
  					outlet_int(xx->fResultOut, result);
			  }
			  else
			  {
			  	okSoFar = false;
			  	LOG_ERROR_2(OUTPUT_PREFIX "element not found for '%s:put'", deviceType->s_name)
			  }
  		}
		}
		else if (argc == 2)
		{
			okSoFar = false;
			LOG_ERROR_2(OUTPUT_PREFIX "missing element for '%s:put'", deviceType->s_name)
		}
		else
		{
  		okSoFar = false;
  		LOG_ERROR_2(OUTPUT_PREFIX "missing value(s) for '%s:put'", deviceType->s_name)
  	}
	}
	else if (result)
		outlet_int(xx->fResultOut, result);
	return okSoFar;
} /* performPut */

/*------------------------------------ cmd_Put ---*/
Pvoid cmd_Put
  (FidgetPtr	xx,
   PSymbol		message,
   short			argc,
   PAtom			argv)  
{
#pragma unused(message)
	PSymbol	deviceType = NULL_PTR, serialNumber = NULL_PTR;

  EnterCallback();
  if (xx)
  {
  	bool	okSoFar = true;
  	
  	if (argc >= 2)
  	{
  		// Check the device type
  		if (argv[0].a_type == A_SYM)
  			deviceType = argv[0].a_w.w_sym;
  		else
  		{
  			okSoFar = false;
  			LOG_ERROR_1(OUTPUT_PREFIX "device type is not a symbol for 'put'")
  		}
  		// Check the serial number
  		if (okSoFar)
  		{
  			if (argv[1].a_type == A_SYM)
  				serialNumber = argv[1].a_w.w_sym;
  			else
  			{
  				okSoFar = false;
  				LOG_ERROR_2(OUTPUT_PREFIX "serial number is not a symbol for '%s:put'",
  										deviceType->s_name)
  			}
  		}
		}
		else if (argc == 1)
		{
			okSoFar = false;
  		LOG_ERROR_1(OUTPUT_PREFIX "missing serial number for 'put'")
		}
		else
		{
			okSoFar = false;
  		LOG_ERROR_1(OUTPUT_PREFIX "missing device type for 'put'")
		}
		if (okSoFar)
		{
			if (serialNumber == gAsteriskSymbol)
			{
				HIDDeviceDataPtr	walkerHID = fidgetGetFirstHIDData(xx, deviceType);
				
				for ( ; walkerHID; )
				{
					if (! performPut(xx, walkerHID, deviceType, argc, argv))
						break;
						
					walkerHID = fidgetGetNextHIDData(deviceType, walkerHID);
				}
			}
			else
			{
				// Identify the object
				HIDDeviceDataPtr	walkerHID = fidgetLocateHIDData(xx, deviceType, serialNumber);
				
				if (walkerHID)
					performPut(xx, walkerHID, deviceType, argc, argv);
				else
					LOG_ERROR_3(OUTPUT_PREFIX "no such device (%s:%s) is currently attached",
											deviceType->s_name, serialNumber->s_name)
			}
		}  		
  }
  ExitMaxMessageHandler()
} /* cmd_Put */
