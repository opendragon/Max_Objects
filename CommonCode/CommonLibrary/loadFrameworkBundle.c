/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       loadFrameworkBundle.c                                                   */
/*                                                                                      */
/*  Contains:   The routine loadFrameworkBundle().                                      */
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

#include "loadOtherSegments.h"

#if defined(COMPILE_FOR_OSX_4)
/*------------------------------------ loadFrameworkBundle ---*/
OSStatus loadFrameworkBundle
	(CFStringRef		framework,
	 CFBundleRef &	bundleRef)
{
 #if defined(COMPILE_FOR_STUB)
  #pragma unused(framework,bundleRef)
 	return noErr;
 #else /* not COMPILE_FOR_STUB */
	OSStatus 	err;
	FSRef 		frameworksFolderRef;
	CFURLRef	baseURL = NULL_PTR;
	CFURLRef	bundleURL = NULL_PTR;
	
	bundleRef = NULL_PTR;
	
	// Find the frameworks folder and create a URL for it.
	err = FSFindFolder(kOnAppropriateDisk, kFrameworksFolderType, true,
											&frameworksFolderRef);
	if (err == noErr)
	{
		baseURL = CFURLCreateFromFSRef(kCFAllocatorSystemDefault,
																		&frameworksFolderRef);
		if (! baseURL)
			err = coreFoundationUnknownErr;
	}
	
	// Append the name of the framework to the URL.	
	if (err == noErr)
	{
		bundleURL = CFURLCreateCopyAppendingPathComponent(kCFAllocatorSystemDefault,
																											baseURL, framework, false);
		if (! bundleURL)
			err = coreFoundationUnknownErr;
	}
	
	// Create a bundle based on that URL and load the bundle into memory.
	if (err == noErr)
	{
		bundleRef = CFBundleCreate(kCFAllocatorSystemDefault, bundleURL);
		if (! bundleRef)
			err = coreFoundationUnknownErr;
	}
	if (err == noErr)
	{
		if (! CFBundleLoadExecutable(bundleRef))
			err = coreFoundationUnknownErr;
	}

	// Clean up.
	if ((err != noErr) && bundleRef)
	{
		CFRelease(bundleRef);
		bundleRef = NULL_PTR;
	}
	if (bundleURL)
		CFRelease(bundleURL);
	if (baseURL)
		CFRelease(baseURL);	
	return err;
 #endif /* not COMPILE_FOR_STUB */
} /* loadFrameworkBundle */
#endif /* COMPILE_FOR_OSX_4 */

#if defined(COMPILE_FOR_OSX_4) && defined(COMPILE_FOR_STUB)
 #pragma export list loadFrameworkBundle
#endif /* COMPILE_FOR_OSX_4 and COMPILE_FOR_STUB */