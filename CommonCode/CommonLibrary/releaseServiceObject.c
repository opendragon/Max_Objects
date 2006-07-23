/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       releaseServiceObject.c                                                  */
/*                                                                                      */
/*  Contains:   The routine releaseServiceObject().                                     */
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
/*  Created:    2004/02/29                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "Common_IOKitX.h"
#include "Common_IOKitXData.h"
#include "loadOtherSegments.h"

#if defined(COMPILE_FOR_OSX_4)
/*------------------------------------ releaseServiceObject ---*/
bool releaseServiceObject
	(Pchar					name,
	 IOKitContext &	rec,
	 io_service_t &	object)
{
 #if defined(COMPILE_FOR_STUB)
  #pragma unused(name,rec,object)
 	return false;
 #else /* not COMPILE_FOR_STUB */
	static IOObjectRelease_FP	pFobjectRelease = NULL_PTR;
	
	if (! pFobjectRelease)
		pFobjectRelease = getFrameworkFunction(IOObjectRelease_FP, rec.fIOKitBundle,
																						CFSTR("IOObjectRelease"));														
	if (pFobjectRelease)
	{
		kern_return_t	kernResult = pFobjectRelease(object);
		
		if (kernResult == KERN_SUCCESS)
		{
			object = NULL_PTR;
			return true;
		
		}
		LOG_ERROR_4("%s: unable to release an object, error = %d/0x%x", name,
								kernResult, kernResult)
	}
	return false;
 #endif /* not COMPILE_FOR_STUB */
} /* releaseServiceObject */
#endif /* COMPILE_FOR_OSX_4 */
	 
#if defined(COMPILE_FOR_OSX_4) && defined(COMPILE_FOR_STUB)
 #pragma export list releaseServiceObject
#endif /* COMPILE_FOR_OSX_4 and COMPILE_FOR_STUB */