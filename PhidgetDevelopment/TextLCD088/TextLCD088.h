/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       TextLCD088.h                                                            */
/*                                                                                      */
/*  Contains:   The data type declarations that are needed for this phidget.            */
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

#if (! defined(TEXTLCD088_H_))
 #define TEXTLCD088_H_ /* */
 
 #include "StandardPhidgets.h"
 
struct PrivateData
{
	bool	fDigitalInputTriggerEnabled;
	long	fDigitalOutputs;
 #if (! defined(COMPILE_FOR_CARBON))
	long	fDigitalInput;
 #endif /* not COMPILE_FOR_CARBON */
}; /* PrivateData */
 
struct SharedData
{
	IOHIDElementCookie	fInputCookie;
	IOHIDElementCookie	fOutputCookie;
	bool								fCookiesValid;	
}; /* SharedData */

typedef PrivateData *	PrivatePtr;
typedef SharedData *	SharedPtr;

#endif /* not TEXTLCD088_H_ */