/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       loadOtherSegments.h                                                     */
/*                                                                                      */
/*  Contains:   The data type declarations and forward references to routines.          */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2000 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2000/11/13                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(LOADOTHERSEGMENTS_H_))
 #define LOADOTHERSEGMENTS_H_ /* */

 #include "MissingAndExtra.h"

 #if defined(COMPILE_FOR_OSX_4)
struct TransferVector_rec
{
	ProcPtr	fProcPtr;
	UInt32	fTOC;
}; /* TransferVector_rec */

typedef TransferVector_rec *	TransferVector_ptr;

  #define createCallback(funcType, inFunc)	\
  	reinterpret_cast<funcType>(createMachCallback(inFunc))
  	
Pvoid createMachCallback
	(Pvoid	aFunction);
	
Pvoid createMachSpringboard
	(Pvoid	aFunction);
	
Pvoid fillMachCallback
	(TransferVector_rec &	funkChunk,
	 Pvoid								aFunction);
	
  #define createSpringboard(funcType, inFunc)	\
  	reinterpret_cast<funcType>(createMachSpringboard(inFunc))
  	
  #define fillCallback(funcType, funkChunk, inFunc)	\
  	reinterpret_cast<funcType>(fillMachCallback(funkChunk, inFunc))
  	
  #define getFrameworkData(dataType, bundle, name)	\
  	reinterpret_cast<dataType>(CFBundleGetDataPointerForName(bundle, name)) 
  	
  #define getFrameworkFunction(funcType, bundle, name)	\
		reinterpret_cast<funcType>(CFBundleGetFunctionPointerForName(bundle, name))

OSStatus loadFrameworkBundle
	(CFStringRef		framework,
	 CFBundleRef &	bundleRef);

  #define releaseCallback(inFunc)	\
		releaseMachCallback(*reinterpret_cast<Ptr*>(&inFunc))
		
void releaseMachCallback
	(Ptr &	aFunction);

void releaseMachSpringboard
	(Ptr &	aFunction);

  #define releaseSpringboard(inFunc)	\
		releaseMachSpringboard(*reinterpret_cast<Ptr*>(&inFunc))
  	
void unloadFrameworkBundle
	(CFBundleRef &	bundleRef);
 #endif /* COMPILE_FOR_OSX_4 */

void loadOtherSegments
  (void);

#endif /* not LOADOTHERSEGMENTS_H_ */
