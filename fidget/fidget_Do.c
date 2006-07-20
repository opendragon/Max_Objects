/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       fidget_Do.c                                                             */
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
/*  Created:    2003/12/19                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "fidget.h"
#include "genericListOutput.h"

/*------------------------------------ performDo ---*/
static bool performDo
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
		action = reinterpret_cast<FpDoCustom>(kind->fDoCustomFun)(OUR_NAME, deviceType,
																															xx->fResultOut,
																															kind->fShared, walkerHID->fPrivate,
																															walkerHID, short(argc - 2),
																															argv + 2, &result);
#else /* not COMPILE_FOR_CATS */
		action = static_cast<E_PhidgResult>(CallUniversalProc(kind->fDoCustomUpp, uppDoCustomProcInfo,
																OUR_NAME, deviceType, xx->fResultOut, kind->fShared,
																walkerHID->fPrivate, walkerHID, short(argc - 2),
																argv + 2, &result));
#endif /* not COMPILE_FOR_CATS */
	}
	if (action == kPhidgDoDefault)
	{
		okSoFar = false;
		LOG_POST_2(OUTPUT_PREFIX "do %s", deviceType->s_name)
	}
	else if (result)
		outlet_int(xx->fResultOut, result);
	return okSoFar;
} /* performDo */

/*------------------------------------ cmd_Do ---*/
Pvoid cmd_Do
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
  			LOG_ERROR_1(OUTPUT_PREFIX "device type is not a symbol for 'do'")
  		}
  		// Check the serial number
  		if (okSoFar)
  		{
  			if (argv[1].a_type == A_SYM)
  				serialNumber = argv[1].a_w.w_sym;
  			else
  			{
  				okSoFar = false;
  				LOG_ERROR_2(OUTPUT_PREFIX "serial number is not a symbol for '%s:do'",
  										deviceType->s_name)
  			}
  		}
		}
		else if (argc == 1)
		{
			okSoFar = false;
  		LOG_ERROR_1(OUTPUT_PREFIX "missing serial number for 'do'")
		}
		else
		{
			okSoFar = false;
  		LOG_ERROR_1(OUTPUT_PREFIX "missing device type for 'do'")
		}
		if (okSoFar)
		{
			if (serialNumber == gAsteriskSymbol)
			{
				HIDDeviceDataPtr	walkerHID = fidgetGetFirstHIDData(xx, deviceType);
				
				for ( ; walkerHID; )
				{
					if (! performDo(xx, walkerHID, deviceType, argc, argv))
						break;
						
					walkerHID = fidgetGetNextHIDData(deviceType, walkerHID);
				}
			}
			else
			{
				// Identify the object
	  		HIDDeviceDataPtr	walkerHID = fidgetLocateHIDData(xx, deviceType, serialNumber);
	  		
				if (walkerHID)
					performDo(xx, walkerHID, deviceType, argc, argv);
				else
					LOG_ERROR_3(OUTPUT_PREFIX "no such device (%s:%s) is currently attached",
											deviceType->s_name, serialNumber->s_name)
			}
		}  		
  }
  ExitMaxMessageHandler()
} /* cmd_Do */
