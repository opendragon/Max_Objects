/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       reportVersion.c                                                         */
/*                                                                                      */
/*  Contains:   The routine reportVersion().                                            */
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
/*  Created:    2000/06/25                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "reportVersion.h"

#if defined(COMPILE_FOR_CATS)
char		gCopyrightString[256];
#else /* not COMPILE_FOR_CATS */
Str255	gCopyrightString;
#endif /* not COMPILE_FOR_CATS */

/*------------------------------------ reportVersion ---*/
ulong reportVersion
  (Qchar	name)
{
#if defined(COMPILE_FOR_STUB)
 #pragma unused(name)
	return 0;
#else /* not COMPILE_FOR_STUB */
  short             maxVers = Count1Resources('vers');
  ulong             versNumber;
  NumVersionVariant variant;
	VersRecHndl				theVers = NULL_HDL;
	int								shortLength, longLength;
	Ptr								longVersion;
 
 #if defined(COMPILE_FOR_CATS)
	gCopyrightString[0] = '\0';
 #else /* not COMPILE_FOR_CATS */
 	memcpy(gCopyrightString, "", 1);
    P2CStr(reinterpret_cast<Puchar>(&gCopyrightString));
 #endif /* not COMPILE_FOR_CATS */
	if (maxVers > 0)
		theVers = reinterpret_cast<VersRecHndl>(Get1Resource('vers', 1));
  if (theVers && (! ResError()) && (! MemError()))
  {
 #if defined(COMPILE_FOR_CATS)
		char		tempString[256];
 #else /* not COMPILE_FOR_CATS */
		Str255	tempString;
 #endif /* not COMPILE_FOR_CATS */

    shortLength = StrLength((*theVers)->shortVersion);
    longVersion = reinterpret_cast<Ptr>(&(*theVers)->shortVersion) + shortLength + 1;
    longLength = StrLength(reinterpret_cast<Puchar>(longVersion));
    variant.parts = (*theVers)->numericVersion;
    versNumber = variant.whole;
 #if defined(COMPILE_FOR_CATS)
    CopyPascalStringToC(reinterpret_cast<ConstStr255Param>(longVersion), tempString);
 #else /* not COMPILE_FOR_CATS */
    memcpy(tempString, longVersion, static_cast<ulong>(longLength + 1));
    P2CStr(reinterpret_cast<Puchar>(&tempString));
 #endif /* not COMPILE_FOR_CATS */
    short	major = variant.parts.majorRev;
    short	minorAndBug = variant.parts.minorAndBugRev;
    short	minor = short(minorAndBug >> 4);
    short	bug = short(minorAndBug & 0x0F);
    short	nRR = variant.parts.nonRelRev;
    Qchar	stage = NULL_PTR;

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
      post("%s %hd.%hd.%hd %s", name, major, minor, bug, tempString);
    else
      post("%s %hd.%hd.%hd%s%hd %s", name, major, minor, bug, stage, nRR, tempString);
 #if (! defined(COMPILE_FOR_CATS))
    C2PStr(reinterpret_cast<Ptr>(&tempString));
 #endif /* not COMPILE_FOR_CATS */
    ReleaseResource(reinterpret_cast<Handle>(theVers));
    theVers = NULL_HDL;
  }
	if (maxVers > 1)
		theVers = reinterpret_cast<VersRecHndl>(Get1Resource('vers', 2));
  if (theVers && (! ResError()) && (! MemError()))
  {
    shortLength = StrLength((*theVers)->shortVersion);
    longVersion = reinterpret_cast<Ptr>(&(*theVers)->shortVersion) + shortLength + 1;
    longLength = StrLength(reinterpret_cast<Puchar>(longVersion));
 #if defined(COMPILE_FOR_CATS)
    CopyPascalStringToC(reinterpret_cast<ConstStr255Param>(longVersion), gCopyrightString);
 #else /* not COMPILE_FOR_CATS */
    memcpy(gCopyrightString, longVersion, static_cast<ulong>(longLength + 1));
    P2CStr(reinterpret_cast<Puchar>(&gCopyrightString));
 #endif /* not COMPILE_FOR_CATS */
    post("%s", gCopyrightString);
    ReleaseResource(reinterpret_cast<Handle>(theVers));
  }
 return versNumber;
#endif /* not COMPILE_FOR_STUB */
} /* reportVersion */

#if defined(COMPILE_FOR_STUB)
 #pragma export list reportVersion
#endif /* COMPILE_FOR_STUB */