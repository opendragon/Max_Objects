/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       HD44780.h                                                               */
/*                                                                                      */
/*  Contains:   The data type declarations that are needed for controlling an LCD.      */
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

#if (! defined(HD44780_H_))
 #define HD44780_H_	/* */
 
static const char	HD44780_ClearDisplay = 0x01;

static const char	HD44780_ReturnHome = 0x02;

static const char	HD44780_EntryModeSet = 0x04;
static const char	HD44780_EntryMode_Decrement = 0x00;
static const char	HD44780_EntryMode_Increment = 0x02;
static const char	HD44780_EntryMode_NoShift = 0x00;
static const char	HD44780_EntryMode_WithShift = 0x01;

static const char	HD44780_DisplayOnOff = 0x08;
static const char	HD44780_Display_Off = 0x00;
static const char	HD44780_Display_On = 0x04;
static const char	HD44780_Display_NoCursor = 0x00;
static const char	HD44780_Display_WithCursor = 0x02;
static const char	HD44780_Display_NoBlink = 0x00;
static const char	HD44780_Display_WithBlink = 0x01;

static const char	HD44780_CursorDisplayShift = 0x10;
static const char	HD44780_CursorDisplay_ChangeCursor = 0x00;
static const char	HD44780_CursorDisplay_ChangeDisplay = 0x08;
static const char	HD44780_CursorDisplay_GoLeft = 0x00;
static const char	HD44780_CursorDisplay_GoRight = 0x04;

static const char	HD44780_FunctionSet = 0x20;
static const char	HD44780_Function_4Bit = 0x00;
static const char	HD44780_Function_8Bit = 0x10;
static const char	HD44780_Function_1x5x7 = 0x00;
static const char	HD44780_Function_1x5x10 = 0x04;
static const char	HD44780_Function_2x5x7 = 0x08;

static const char	HD44780_CGRamSet = 0x40; // low bits are the address

static const char	HD44780_DDRamSet = 0x80; // low bits are the address
static const char	HD44780_DDRam_MaxCell = 0x27;
static const char	HD44780_DDRam_Line2 = 0x40;

static const char	HD44780_Backlight_Off_0 = 0x00;
static const char	HD44780_Backlight_Off_n = 0x11;
static const char	HD44780_Backlight_On_0 = 0x01;
static const char	HD44780_Backlight_On_n = 0x11;

// Protocol commands:
static const char	TextLCD_Escape = 0x00;
static const char	TextLCD_ControlMode = 0x01;
static const char	TextLCD_DataMode = 0x02;

#endif /* not HD44780_H_ */
