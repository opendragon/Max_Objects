/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       displayAboutBox.c                                                       */
/*                                                                                      */
/*  Contains:   The routine Common_DisplayAboutBox().                                   */
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
/*  Created:    2000/06/14                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "displayAboutBox.h"
#include "reportVersion.h"

#if defined(COMPILE_FOR_CATS)
 #define  NEWLINE_ "\n"
#else /* not COMPILE_FOR_CATS */
 #define  NEWLINE_  "\r"
#endif /* not COMPILE_FOR_CATS */ 

static const int TEXT_INSET_LR = 5;
static const int TEXT_INSET_TB = 1;

#if defined(COMPILE_FOR_CATS)
static ControlUserPaneDrawUPP lTPDrawProc = NULL_PTR;
#endif /* COMPILE_FOR_CATS */  

#if defined(COMPILE_FOR_CATS)
struct STPTextPaneVars
{
		/* OS records referenced */
	TXNObject  fTXNRec;             /* the txn record */
	TXNFrameID fTXNFrame;           /* the txn frame ID */
	CGrafPtr   fDrawingEnvironment; /* grafport where control is drawn */
		/* calculated locations */
	Rect       fRTextArea;          /* area where the text is drawn */
	Rect       fRTextOutline;       /* rectangle used to draw the border */
	RgnHandle  fTextBackgroundRgn;  /* background region for the text, erased before calling TEUpdate */
}; /* STPTextPaneVars */
#endif /* COMPILE_FOR_CATS */  

#if defined(COMPILE_FOR_CATS) && (! defined(COMPILE_FOR_STUB))
/*------------------------------------ TPPaneDrawProc ---*/
static pascal void TPPaneDrawProc
	(ControlRef      theControl,
	 ControlPartCode thePart)
{
 #pragma unused(thePart)
	STPTextPaneVars * * tpvars = reinterpret_cast<STPTextPaneVars**>(GetControlReference(theControl));

	if (tpvars)
	{
		Rect bounds;
		char state = HGetState(reinterpret_cast<Handle>(tpvars));
		
		HLock(reinterpret_cast<Handle>(tpvars));
		STPTextPaneVars *   varsp = *tpvars;
			
			/* save the drawing state */
		SetPort(varsp->fDrawingEnvironment);
			/* verify our boundary */
		GetControlBounds(theControl, &bounds);
		if (! EqualRect(&bounds, &varsp->fRTextArea))
		{
			SetRect(&varsp->fRTextOutline, bounds.left, bounds.top, bounds.right, bounds.bottom);
			SetRect(&varsp->fRTextArea, bounds.left, bounds.top, bounds.right, bounds.bottom);
			RectRgn(varsp->fTextBackgroundRgn, &varsp->fRTextOutline);
			TXNSetFrameBounds(varsp->fTXNRec, bounds.top, bounds.left, bounds.bottom, bounds.right,
												varsp->fTXNFrame);
		}

			/* update the text region */
		EraseRgn(varsp->fTextBackgroundRgn);
		TXNDraw(varsp->fTXNRec, NULL_PTR);
		DrawThemeEditTextFrame(&varsp->fRTextOutline,	kThemeStateActive);
			/* release our globals */
		HSetState(reinterpret_cast<Handle>(tpvars), state);
	}
} /* TPPaneDrawProc */
#endif /* COMPILE_FOR_CATS and not COMPILE_FOR_STUB */  

#if (! defined(COMPILE_FOR_STUB))
 #if defined(COMPILE_FOR_CATS)
/*------------------------------------ setStyle ---*/
static OSStatus setStyle
	(TXNObject				textObject,
	 ConstStr255Param	fontName,
	 SInt16						fontSize,
	 RGBColor &				fontColor,
	 Style						fontStyle)
{
	TXNTypeAttributes	typeAttr[4];
	Str255						tempString;
	
	typeAttr[0].tag = kTXNQDFontStyleAttribute;
	typeAttr[0].size = kTXNQDFontStyleAttributeSize;
	typeAttr[0].data.dataValue = fontStyle;
	typeAttr[1].tag = kTXNQDFontColorAttribute;
	typeAttr[1].size = kTXNQDFontColorAttributeSize;
	typeAttr[1].data.dataPtr = &fontColor;
	typeAttr[2].tag = kTXNQDFontSizeAttribute;
	typeAttr[2].size = kTXNFontSizeAttributeSize;
	typeAttr[2].data.dataValue = static_cast<UInt32>(fontSize << 16);
	typeAttr[3].tag = kTXNQDFontNameAttribute;
	typeAttr[3].size = kTXNQDFontNameAttributeSize;
	memcpy(tempString, fontName, strlen(reinterpret_cast<Qchar>(fontName)));
	typeAttr[3].data.dataPtr = tempString;
	return TXNSetTypeAttributes(textObject, 4, typeAttr, kTXNUseCurrentSelection,
															kTXNEndOffset);
} /* setStyle */
 #else /* not COMPILE_FOR_CATS */
/*------------------------------------ setStyle ---*/
static void setStyle
	(TEHandle 				textHandle,
	 ConstStr255Param	fontName,
	 SInt16						fontSize,
	 RGBColor &				fontColor,
	 Style						fontStyle)
{
  short     fontID;
  TextStyle newStyle;

  GetFNum(fontName, &fontID);
  newStyle.tsFace = fontStyle;
  newStyle.tsColor = fontColor;
  newStyle.tsSize = fontSize;
  newStyle.tsFont = fontID;
  TESetStyle(doFont + doFace + doSize + doColor, &newStyle, FALSE, textHandle);
} /* setStyle */
 #endif /* not COMPILE_FOR_CATS */
#endif /* not COMPILE_FOR_STUB */  

/*------------------------------------ displayAboutBox ---*/
void displayAboutBox
  (Qchar				name,
   const short	dialogId,
   const ulong	version)
{
#if defined(COMPILE_FOR_STUB)
 #pragma unused(dialogId,name,version)
#else /* not COMPILE_FOR_STUB */
  DialogItemType      itemType;
  Handle              item;
  Rect                itemBox;
  PenState            ps;
  NumVersionVariant 	variant;
  char              	versionString[30];
  short             	itemHit, major, minor, bugRev;
  Qchar								stage = NULL_PTR;
  GrafPtr             oldPort;
  Str255							description;
 #if defined(COMPILE_FOR_CATS)
  ControlHandle       aControl;
	STPTextPaneVars * * tpvars;
	STPTextPaneVars *   varsp;
	Rect                bounds;
  OSStatus            resultCode = noErr;
  TXNObject           textObject = NULL_PTR;
  TXNFrameID          frameID = 0;
 #else /* not COMPILE_FOR_CATS */
  TEHandle            textHandle;
 #endif /* not COMPILE_FOR_CATS */
	static RGBColor   	myBlack = { 0x0000, 0x0000, 0x0000 };
	static RGBColor   	myBlue = { 0x0000, 0x0000, 0xFFFF };
	static RGBColor   	myRed = { 0xFFFF, 0x0000, 0x0000 };
	static RGBColor   	myPurple = { 0xFFFF, 0, 0xFFFF };	
	static Qchar				lKindOfObject = "a Max object by" NEWLINE_;
	static Qchar				lAuthorName = "Norman Jaffe";
	static Qchar				lVersionPrefix = NEWLINE_ NEWLINE_ "Version ";
	static Qchar				lWrittenString = " Written with Metrowerks "
																				"CodeWarrior for the Max "
																				"visual programming environment.";
	static Qchar				lProductionPart1 = NEWLINE_ NEWLINE_ "An ";
	static Qchar				lProductionPart2 = "Open Dragon";
	static Qchar				lProductionPart3 = " Production.";
  
  // Build the version string text:
  variant.whole = version;
  major = variant.parts.majorRev;
  minor = short(variant.parts.minorAndBugRev >> 4);
  bugRev = short(variant.parts.minorAndBugRev & 0x0F);
  switch (variant.parts.stage)
  {
    case developStage:
      stage = "d";
      break;

    case alphaStage:
      stage = "a";
      break;

    case betaStage:
      stage = "b";
      break;

    default:
      stage = "a";
      break;

  }
  if (variant.parts.stage == finalStage)
    sprintf(versionString, "%hd.%hd.%hd", major, minor, bugRev);
  else
    sprintf(versionString, "%hd.%hd.%hd%s%hd", major, minor, bugRev, stage,
            short(variant.parts.nonRelRev));
            
	// Get the descriptive text:
  GetIndString(description, dialogId, 1);
 #if defined(COMPILE_FOR_CATS)
  TXNInitTextension(NULL_PTR,  0, kTXNWantGraphicsMask);
 #endif /* COMPILE_FOR_CATS */
  GetPort(&oldPort);
  DialogPtr dp = GetNewDialog(OUR_RES_NUMB_BASE, 0L, reinterpret_cast<WindowPtr>(-1L));
  WindowPtr	aWindow = GetDialogWindow(dp);

  /* Highlight the OK button */   
  SetPortDialogPort(dp);
  GetDialogItem(dp, OUR_OK_BUTTON_ID, &itemType, &item, &itemBox);
  GetPenState(&ps);
  PenNormal();
  PenSize(3, 3);
  InsetRect(&itemBox, -4, -4);
  FrameRoundRect(&itemBox, 16, 16);
  SetPenState(&ps);

  /* Set up the text area */  
  GetDialogItem(dp, OUR_TEXT_AREA_ID, &itemType, &item, &itemBox);
  InsetRect(&itemBox, TEXT_INSET_LR, TEXT_INSET_TB);  /* text margins */
 #if defined(COMPILE_FOR_CATS)
	GetDialogItemAsControl(dp, OUR_TEXT_AREA_ID, &aControl);
  lTPDrawProc = NewControlUserPaneDrawUPP(TPPaneDrawProc);
  tpvars = reinterpret_cast<STPTextPaneVars**>(NewHandleClear(sizeof(STPTextPaneVars)));
	SetControlReference(aControl, long(tpvars));
	HLock(reinterpret_cast<Handle>(tpvars));
	varsp = *tpvars;
		/* set the initial settings for our private data */
	varsp->fDrawingEnvironment = GetWindowPort(aWindow);
	SetControlData(aControl, kControlEntireControl, kControlUserPaneDrawProcTag, sizeof(lTPDrawProc),
									&lTPDrawProc);
		/* calculate the rectangles used by the control */
	GetControlBounds(aControl, &bounds);
	SetRect(&varsp->fRTextOutline, bounds.left, bounds.top, bounds.right, bounds.bottom);
	SetRect(&varsp->fRTextArea, bounds.left, bounds.top, bounds.right, bounds.bottom);
		/* calculate the background region for the text.  In this case, it's kindof
		and irregular region because we're setting the scroll bar a little ways inside
		of the text area. */
	varsp->fTextBackgroundRgn = NewRgn();
	RectRgn(varsp->fTextBackgroundRgn, &varsp->fRTextOutline);

		/* set up the drawing environment */
	SetPort(varsp->fDrawingEnvironment);

		/* create the new edit field */
	resultCode = TXNNewObject(NULL_PTR, aWindow, &varsp->fRTextArea,
														kTXNNoKeyboardSyncMask | kTXNNoSelectionMask |
														kTXNAlwaysWrapAtViewEdgeMask | kTXNDisableDragAndDropMask,
														kTXNTextEditStyleFrameType, kTXNTextFile,
														kTXNSystemDefaultEncoding, &textObject, &frameID,
														static_cast<TXNObjectRefcon>(tpvars));
	if (! resultCode)
	{
		varsp->fTXNRec = textObject;
		varsp->fTXNFrame = frameID;	
	}
		/* unlock our storage */
	HUnlock(reinterpret_cast<Handle>(tpvars));
 #endif /* COMPILE_FOR_CATS */  
 #if defined(COMPILE_FOR_CATS)
	resultCode = setStyle(textObject, "\pCharcoal", 24, myBlack, bold); 
 	if (! resultCode)
 		resultCode = TXNSetData(textObject, kTXNTextData, name, strlen(name),
 														kTXNEndOffset, kTXNEndOffset);
 	if (! resultCode)
 		resultCode = TXNSetData(textObject, kTXNTextData, NEWLINE_, 1, kTXNEndOffset,
 														kTXNEndOffset);
 	if (! resultCode)
 		resultCode = setStyle(textObject, "\pMonaco", 9, myBlue, normal);
 	if (! resultCode)
 		resultCode = TXNSetData(textObject, kTXNTextData, lKindOfObject,
 														strlen(lKindOfObject), kTXNEndOffset, kTXNEndOffset);
 	if (! resultCode)
 		resultCode = setStyle(textObject, "\pTimes", 12, myRed, normal);
 	if (! resultCode)
 		resultCode = TXNSetData(textObject, kTXNTextData, lAuthorName,
 														strlen(lAuthorName), kTXNEndOffset, kTXNEndOffset);
 	if (! resultCode)
 		resultCode = setStyle(textObject, "\pTimes", 10, myBlack, normal); 
 	if (! resultCode)
 		resultCode = TXNSetData(textObject, kTXNTextData, lVersionPrefix,
 														strlen(lVersionPrefix), kTXNEndOffset, kTXNEndOffset);
 	if (! resultCode)
 		resultCode = TXNSetData(textObject, kTXNTextData, versionString,
 														strlen(versionString), kTXNEndOffset, kTXNEndOffset);
 	if (! resultCode)
 		resultCode = TXNSetData(textObject, kTXNTextData, NEWLINE_, 1, kTXNEndOffset,
 														kTXNEndOffset);
 	if (! resultCode)
 	{
		if (strlen(gCopyrightString))
 		{
 			resultCode = TXNSetData(textObject, kTXNTextData, gCopyrightString,
 														strlen(gCopyrightString), kTXNEndOffset, kTXNEndOffset);
		 	if (! resultCode)
		 		resultCode = TXNSetData(textObject, kTXNTextData, NEWLINE_, 1, kTXNEndOffset,
		 														kTXNEndOffset);
 		}
	}
 	if (! resultCode)
 		resultCode = TXNSetData(textObject, kTXNTextData, NEWLINE_, 1, kTXNEndOffset,
 														kTXNEndOffset);
	CopyPascalStringToC(description, reinterpret_cast<Pchar>(description));
 	if (! resultCode)
 		resultCode = setStyle(textObject, "\pTimes", 12, myBlack, normal); 
 	if (! resultCode)
 		resultCode = TXNSetData(textObject, kTXNTextData, description,
 														strlen(reinterpret_cast<Pchar>(description)),
 														kTXNEndOffset, kTXNEndOffset);
 	if (! resultCode)
 		resultCode = TXNSetData(textObject, kTXNTextData, lWrittenString,
 														strlen(lWrittenString), kTXNEndOffset, kTXNEndOffset);
 	if (! resultCode)
 		resultCode = TXNSetData(textObject, kTXNTextData, lProductionPart1,
 														strlen(lProductionPart1), kTXNEndOffset, kTXNEndOffset);
 	if (! resultCode)
 		resultCode = setStyle(textObject, "\pTimes", 12, myPurple, bold + extend);
 	if (! resultCode)
 		resultCode = TXNSetData(textObject, kTXNTextData, lProductionPart2,
 														strlen(lProductionPart2), kTXNEndOffset, kTXNEndOffset);
 	if (! resultCode)
 		resultCode = setStyle(textObject, "\pTimes", 12, myBlack, normal); 
 	if (! resultCode)
 		resultCode = TXNSetData(textObject, kTXNTextData, lProductionPart3,
 														strlen(lProductionPart3), kTXNEndOffset, kTXNEndOffset);
  #else /* not COMPILE_FOR_CATS */
 	textHandle = TEStyleNew(&itemBox, &itemBox);
	setStyle(textHandle, "\pCharcoal", 24, myBlack, bold); 
 	TEInsert(name, long(strlen(name)), textHandle);
	TEInsert(NEWLINE_, 1, textHandle);
 	setStyle(textHandle, "\pMonaco", 9, myBlue, normal);
 	TEInsert(lKindOfObject, long(strlen(lKindOfObject)), textHandle);
	setStyle(textHandle, "\pTimes", 12, myRed, normal);
 	TEInsert(lAuthorName, long(strlen(lAuthorName)), textHandle);
 	setStyle(textHandle, "\pTimes", 10, myBlack, normal); 
 	TEInsert(lVersionPrefix, long(strlen(lVersionPrefix)), textHandle);
 	TEInsert(versionString, long(strlen(versionString)), textHandle);
 	TEInsert(NEWLINE_, 1, textHandle);
	if (strlen(reinterpret_cast<Pchar>(gCopyrightString)))
	{
		TEInsert(gCopyrightString, long(strlen(reinterpret_cast<Pchar>(gCopyrightString))), textHandle);
		TEInsert(NEWLINE_, 1, textHandle);
	}
	TEInsert(NEWLINE_, 1, textHandle);
  P2CStr(reinterpret_cast<Puchar>(description));
 	setStyle(textHandle, "\pTimes", 12, myBlack, normal); 
 	TEInsert(description,
 					long(strlen(reinterpret_cast<Pchar>(description))), textHandle);
 	TEInsert(lWrittenString, long(strlen(lWrittenString)), textHandle);
 	TEInsert(lProductionPart1, long(strlen(lProductionPart1)), textHandle);
 	setStyle(textHandle, "\pTimes", 12, myPurple, bold + extend);
 	TEInsert(lProductionPart2, long(strlen(lProductionPart2)), textHandle);
 	setStyle(textHandle, "\pTimes", 12, myBlack, normal); 
 	TEInsert(lProductionPart3, long(strlen(lProductionPart3)), textHandle);
	TESetAlignment(teJustCenter, textHandle);
 #endif /* not COMPILE_FOR_CATS */
 #if defined(COMPILE_FOR_CATS)
  if (! resultCode)
  {
    TXNControlTag   tags[] = { kTXNJustificationTag };
    TXNControlData  data[] = { kTXNCenter };
    
    resultCode = TXNSetTXNObjectControls(textObject, false, sizeof(tags) / sizeof(tags[0]),
                                          tags, data); 
  }
 #endif /* COMPILE_FOR_CATS */
  ShowWindow(aWindow);
 #if (! defined(COMPILE_FOR_CATS))
  TEUpdate(&itemBox, textHandle);
 #endif /* not COMPILE_FOR_CATS */
  InsetRect(&itemBox, -TEXT_INSET_LR, -TEXT_INSET_TB);
  FrameRect(&itemBox);

  /* Wait for the press of the OK button */
  for ( ; ; )
  {
    ModalDialog(NULL_PTR, &itemHit);
    if (itemHit == OUR_OK_BUTTON_ID)
      break;

  }
 #if defined(COMPILE_FOR_CATS)
  TXNDeleteObject(textObject);
 #else /* not COMPILE_FOR_CATS */
  TEDispose(textHandle);
 #endif /* not COMPILE_FOR_CATS */
  DisposeDialog(dp);
  SetPort(oldPort);
 #if defined(COMPILE_FOR_CATS)
	DisposeHandle(reinterpret_cast<Handle>(tpvars));	
 #endif /* COMPILE_FOR_CATS */
#endif /* not COMPILE_FOR_STUB */
} /* displayAboutBox */

#if defined(COMPILE_FOR_STUB)
 #pragma export list displayAboutBox
#endif /* COMPILE_FOR_STUB */