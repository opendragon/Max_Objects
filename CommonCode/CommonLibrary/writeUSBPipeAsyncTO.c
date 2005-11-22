/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       writeUSBPipeAsyncTO.c                                                   */
/*                                                                                      */
/*  Contains:   The routine writeUSBPipeAsyncTO().                                      */
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
/*  Created:    2005/11/20                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "Common_USBX.h"
#include "Common_USBXData.h"
#include "loadOtherSegments.h"

#if defined(COMPILE_FOR_CATS)
/*------------------------------------ writeUSBPipeAsyncTO ---*/
IOReturn writeUSBPipeAsyncTO
	(IOUSBInterfaceInterface * *	theInterface,
	 const UInt8									pipeRef,
   Pvoid												buffer,
   const UInt32									buffSize,
	 const UInt32									noDataTimeout,
	 const UInt32									completionTimeout,
	 IOAsyncCallback1							callback,
	 Pvoid												refCon)
{
 #if defined(COMPILE_FOR_STUB)
  #pragma unused(theInterface,pipeRef,buffer,buffSize,noDataTimeout,completionTimeout,callback,refCon)
 	return KERN_SUCCESS;
 #else /* not COMPILE_FOR_STUB */
	TransferVector_rec	funkChunk;
	usbWritePipeAsyncTO_FP		pFusbWritePipeAsyncTO = fillCallback(usbWritePipeAsyncTO_FP, funkChunk,
																																(*theInterface)->WritePipeAsyncTO);
																																					
	return pFusbWritePipeAsyncTO(theInterface, pipeRef, buffer, buffSize, noDataTimeout,
																completionTimeout, callback, refCon);
 #endif /* not COMPILE_FOR_STUB */
} /* writeUSBPipeAsyncTO */
#endif /* COMPILE_FOR_CATS */

#if defined(COMPILE_FOR_CATS) and defined(COMPILE_FOR_STUB)
 #pragma export list writeUSBPipeAsyncTO
#endif /* COMPILE_FOR_CATS and COMPILE_FOR_STUB */