/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       reportVersion.cpp                                                       */
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
#include <CoreFoundation/CoreFoundation.h>

// char gCopyrightString[256];

/*------------------------------------ reportVersion ---*/
REPORTVERSION_HEADER
{
    char maxExternalIdentifier[256];

    strcpy(maxExternalIdentifier, "com.opendragon.");
    strcat(maxExternalIdentifier, name);
    CFStringRef nameAsString = CFStringCreateWithCString(kCFAllocatorDefault, maxExternalIdentifier,
                                                         kCFStringEncodingASCII);
    CFBundleRef ourBundle = CFBundleGetBundleWithIdentifier(nameAsString);

    if (ourBundle)
    {
        CFTypeRef versionValue = CFBundleGetValueForInfoDictionaryKey(ourBundle,
                                                              CFSTR("CFBundleLongVersionString"));

        if (versionValue && (CFGetTypeID(versionValue) == CFStringGetTypeID()))
        {
            CFStringRef asString = (CFStringRef) versionValue;
            CFIndex     numChars = CFStringGetLength(asString);
            CFIndex     maxLen = CFStringGetMaximumSizeForEncoding(numChars,
                                                                   kCFStringEncodingASCII);
            char *      buffer = sysmem_newptr(maxLen + 10);

            if (CFStringGetCString(asString, buffer, maxLen + 8, kCFStringEncodingASCII))
            {
                post(buffer);
            }
            sysmem_freeptr(buffer);
        }
    }
    CFRelease(nameAsString);
} // reportVersion

#if 0
Info.plist contents -

<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
    <dict>
        <key>BuildMachineOSBuild</key>
        <string>15B42</string>
        <key>CFBundleDevelopmentRegion</key>
        <string>English</string>
        <key>CFBundleExecutable</key>
        <string>caseShift</string>
        <key>CFBundleInfoDictionaryVersion</key>
        <string>1.0.2</string>
        <key>CFBundleLongVersionString</key>
        <string>caseShift 1.0.2, Copyright 2014 Cycling '74</string>
        <key>CFBundlePackageType</key>
        <string>iLaX</string>
        <key>CFBundleShortVersionString</key>
        <string>1.0.2</string>
        <key>CFBundleSignature</key>
        <string>max2</string>
        <key>CFBundleSupportedPlatforms</key>
        <array>
            <string>MacOSX</string>
        </array>
        <key>CFBundleVersion</key>
        <string>1.0.2</string>
        <key>CSResourcesFileMapped</key>
        <true/>
        <key>DTCompiler</key>
        <string>com.apple.compilers.llvm.clang.1_0</string>
        <key>DTPlatformBuild</key>
        <string>7B1005</string>
        <key>DTPlatformVersion</key>
        <string>GM</string>
        <key>DTSDKBuild</key>
        <string>15A278</string>
        <key>DTSDKName</key>
        <string>macosx10.11</string>
        <key>DTXcode</key>
        <string>0711</string>
        <key>DTXcodeBuild</key>
        <string>7B1005</string>
    </dict>
</plist>

#endif//0

