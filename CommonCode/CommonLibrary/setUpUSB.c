/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       setUpUSB.c                                                              */
/*                                                                                      */
/*  Contains:   The routine setUpUSB().                                                 */
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

#include "Common_USBX.h"
#include "loadOtherSegments.h"

#if (! defined(COMPILE_FOR_CATS))
bool setUpUSB
	(Pvoid																obj,
	 Pchar																name,
	 USBContext &													rec,
	 USBDeviceNotificationCallbackProcPtr	notificationFunction)
{
 #if defined(COMPILE_FOR_STUB)
  #pragma unused(obj,name,rec,notificationFunction)
 	return false;
 #else /* not COMPILE_FOR_STUB */
	USBDeviceNotificationParameterBlock	dnpb;
	
	rec.fNotificationDeviceTokenValid = false;
	rec.fOwner = obj;
	rec.fActive = true;

	// Attach the callback:
	dnpb.pbLength = sizeof(dnpb);
	dnpb.pbVersion = 0;
	dnpb.usbDeviceNotification = kNotifyAnyEvent;
	dnpb.usbDeviceRef = 0;
	dnpb.usbClass = kUSBAnyClass;
	dnpb.usbSubClass = kUSBAnySubClass;
	dnpb.usbProtocol = kUSBAnyProtocol;
	dnpb.usbVendor = kUSBAnyVendor;
	dnpb.usbProduct = kUSBAnyProduct;
	dnpb.result = noErr;
	dnpb.token = 0;
	dnpb.refcon = reinterpret_cast<UInt32>(&rec);
	dnpb.callback = notificationFunction;
	if (notificationFunction)
	{
		USBInstallDeviceNotification(&dnpb);
		if (dnpb.result == noErr)
		{
			rec.fNotificationDeviceToken = dnpb.token;
			rec.fNotificationDeviceTokenValid = true;
		}
		else
		{
      LOG_ERROR_4("%s: unable to install notification for object: %d(0x%x)", name, dnpb.result,
      						dnpb.result)
      rec.fActive = false;
		}
	}
	return rec.fActive;
 #endif /* not COMPILE_FOR_STUB */
} /* setUpUSB */
#endif /* not COMPILE_FOR_CATS */

#if (! defined(COMPILE_FOR_CATS)) && defined(COMPILE_FOR_STUB)
 #pragma export list setUpUSB
#endif /* not COMPILE_FOR_CATS and COMPILE_FOR_STUB */