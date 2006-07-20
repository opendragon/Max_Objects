/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       TextLCDSupport.h                                                        */
/*                                                                                      */
/*  Contains:   The data type declarations that are needed to control the LCD phidgets. */
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
/*  Created:    2003/12/23                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(TEXTLCDSUPPORT_H))
 #define TEXTLCDSUPPORT_H	/* */

 #include "Common_HIDX.h" 

static const int	CHARS_PER_LINE = 20;
static const int	DATA_BUFFER_SIZE = 8;

 #if defined(COMPILE_FOR_CATS)
typedef IOReturn	ReturnType;
 #else /* not COMPILE_FOR_CATS */
typedef OSStatus	ReturnType;
 #endif /* not COMPILE_FOR_CATS */
 
ReturnType doTextLCDBacklightOff
	(Pchar									name,
	 HIDDeviceDataStruct &	thisDevice,	
	 HIDElementDataStruct &	outputElement);
	 
ReturnType doTextLCDBacklightOn
	(Pchar									name,
	 HIDDeviceDataStruct &	thisDevice,	
	 HIDElementDataStruct &	outputElement);
	 
ReturnType doTextLCDClear
	(Pchar									name,
	 HIDDeviceDataStruct &	thisDevice,	
	 HIDElementDataStruct &	outputElement);

ReturnType doTextLCDClearLine1
	(Pchar									name,
	 HIDDeviceDataStruct &	thisDevice,	
	 HIDElementDataStruct &	outputElement);

ReturnType doTextLCDClearLine2
	(Pchar									name,
	 HIDDeviceDataStruct &	thisDevice,	
	 HIDElementDataStruct &	outputElement);

ReturnType doTextLCDEntryMode
	(Pchar									name,
	 HIDDeviceDataStruct &	thisDevice,	
	 HIDElementDataStruct &	outputElement,
	 const bool							reversed,
	 const bool							shifted);

ReturnType doTextLCDGo
	(Pchar									name,
	 HIDDeviceDataStruct &	thisDevice,	
	 HIDElementDataStruct &	outputElement,
	 const int							row,
	 const int							column);

ReturnType doTextLCDOff
	(Pchar									name,
	 HIDDeviceDataStruct &	thisDevice,	
	 HIDElementDataStruct &	outputElement);
	 
ReturnType doTextLCDOn
	(Pchar									name,
	 HIDDeviceDataStruct &	thisDevice,	
	 HIDElementDataStruct &	outputElement,
	 const bool							cursorState,
	 const bool							blinkState);
	 
ReturnType doTextLCDShiftCursor
	(Pchar									name,
	 HIDDeviceDataStruct &	thisDevice,	
	 HIDElementDataStruct &	outputElement,
	 const bool							toRight);

ReturnType doTextLCDShiftDisplay
	(Pchar									name,
	 HIDDeviceDataStruct &	thisDevice,	
	 HIDElementDataStruct &	outputElement,
	 const bool							toRight);

ReturnType doTextLCDWrite
	(Pchar									name,
	 HIDDeviceDataStruct &	thisDevice,	
	 HIDElementDataStruct &	outputElement,
	 Qchar									input);
	 
ReturnType doTextLCDWriteLine1
	(Pchar									name,
	 HIDDeviceDataStruct &	thisDevice,	
	 HIDElementDataStruct &	outputElement,
	 Qchar									input);
	 
ReturnType doTextLCDWriteLine2
	(Pchar									name,
	 HIDDeviceDataStruct &	thisDevice,	
	 HIDElementDataStruct &	outputElement,
	 Qchar									input);
	 	 
#endif /* not TEXTLCDSUPPORT_H */
