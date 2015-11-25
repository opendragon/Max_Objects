/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       tcpLocateLowLevel.cpp                                                   */
/*                                                                                      */
/*  Contains:   The common utility routines.                                            */
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
/*  Created:    2000/11/06                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "tcpLocate.h"
#include <netdb.h>

static void stopResolution(TcpLocateData * xx)
{
    CFHostSetClient(xx->fHost, NULL, NULL);
    CFHostUnscheduleFromRunLoop(xx->fHost, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
    CFHostCancelInfoResolution(xx->fHost, kCFHostAddresses);
} // stopResolution

static void deferred_ErrorBang(TcpLocateData * xx)
{
    if (xx)
    {
        outlet_bang(xx->fErrorBangOut);
    }
} // deferred_ErrorBang

static void deferred_OutputAddresses(TcpLocateData * xx)
{
    if (xx)
    {
        Boolean    hasBeenResolved;
        CFArrayRef results = CFHostGetAddressing(xx->fHost, &hasBeenResolved);

        if (results && hasBeenResolved)
        {
            CFIndex count = CFArrayGetCount(results);

            for (CFIndex ii = 0; ii < count; ++ii)
            {
                CFDataRef addressData = static_cast<CFDataRef>(CFArrayGetValueAtIndex(results, ii));
                char      addressString[NI_MAXHOST];
                int       err = getnameinfo((const sockaddr *) CFDataGetBytePtr(addressData),
                                            (socklen_t) CFDataGetLength(addressData), addressString,
                                            sizeof(addressString), NULL, 0, NI_NUMERICHOST);

                if (err)
                {
                    break;
                }

                outlet_anything(xx->fResultOut, gensym(addressString), 0, NULL);
            }
        }
    }
} // deferred_OutputAddresses

static void hostResolutionCallback(CFHostRef             theHost,
                                   CFHostInfoType        typeInfo,
                                   const CFStreamError * error,
                                   void *                info)
{
    TcpLocateData * xx = reinterpret_cast<TcpLocateData *>(info);

    if (xx)
    {
        stopResolution(xx);
        if (error && (error->domain || error->error))
        {
            defer(xx, reinterpret_cast<method>(deferred_ErrorBang), NULL, 0, NULL);
        }
        else
        {
            defer(xx, reinterpret_cast<method>(deferred_OutputAddresses), NULL, 0, NULL);
        }
    }
} // hostResolutionCallback

/*------------------------------------ doNameLookup ---*/
void doNameLookup(TcpLocateData * xx,
                  const char *    name)
{
    if (xx)
    {
        CFStringRef         asString = CFStringCreateWithCString(kCFAllocatorDefault, name,
                                                                 kCFStringEncodingASCII);
        CFHostClientContext context = { 0, xx, NULL, NULL, NULL };

        if (xx->fHost)
        {
            CFRelease(xx->fHost);
        }
        xx->fHost = CFHostCreateWithName(kCFAllocatorDefault, asString);
        if (asString)
        {
            CFRelease(asString);
        }
        if (CFHostSetClient(xx->fHost, hostResolutionCallback, &context))
        {
            CFStreamError streamError;

            CFHostScheduleWithRunLoop(xx->fHost, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
            if (! CFHostStartInfoResolution(xx->fHost, kCFHostAddresses, &streamError))
            {
                stopResolution(xx);
                outlet_bang(xx->fErrorBangOut);
            }
        }
        else
        {
            outlet_bang(xx->fErrorBangOut);
        }
    }
} // doNameLookup

/*------------------------------------ initObject ---*/
bool initObject(TcpLocateData * xx)
{
    bool okSoFar = true;

    if (xx)
    {
        xx->fErrorBangOut = static_cast<t_outlet *>(bangout(xx));
        xx->fResultOut = static_cast<t_outlet *>(outlet_new(xx, NULL));
#if defined(BE_VERBOSE)
        xx->fVerbose = false;
#endif /* BE_VERBOSE */
        if (! (xx->fResultOut && xx->fErrorBangOut))
        {
            LOG_ERROR_1(xx, OUTPUT_PREFIX "unable to create port or buffer for object")
            okSoFar = false;
        }
    }
    return okSoFar;
} // initObject
