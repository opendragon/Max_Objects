/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       acquireSyslog.c                                                         */
/*                                                                                      */
/*  Contains:   The routine acquireSyslog().                                            */
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

#include "MissingAndExtra.h"
#include "loadOtherSegments.h"
#include "acquireSyslog.h"

#if SYSLOG_OK && (! defined(COMPILE_FOR_STUB))
long				gSyslogCount = 0;
 #if defined(COMPILE_FOR_OSX_4)
syslog_FP		gSyslogFunction = NULL_PTR;
CFBundleRef	gSyslogSysBundle = NULL_PTR;
closelog_FP	gSyslogDoCloselog = NULL_PTR;			
openlog_FP	gSyslogDoOpenlog = NULL_PTR;
 #endif /* COMPILE_FOR_OSX_4 */
#endif /* SYSLOG_OK and not COMPILE_FOR_STUB */

/*------------------------------------ acquireSyslog ---*/
void acquireSyslog
  (void)
{
#if (! defined(COMPILE_FOR_STUB))
 #if SYSLOG_OK
  PSymbol checkGlobal = gensym(SYSLOG_CONTROL_SYMBOL);

  if (! checkGlobal->s_thing)
  {
  #if defined(COMPILE_FOR_OSX_4)
 		OSStatus	err = noErr;
 		
		if (! gSyslogSysBundle)
			err = loadFrameworkBundle(CFSTR("CoreFoundation.framework"),
																			gSyslogSysBundle);
		if (err == noErr)
		{
			if (! gSyslogDoOpenlog)
				gSyslogDoOpenlog = getFrameworkFunction(openlog_FP, gSyslogSysBundle,
																							CFSTR("openlog"));
			if (! gSyslogFunction)
				gSyslogFunction = getFrameworkFunction(syslog_FP, gSyslogSysBundle,
																							CFSTR("syslog"));
			if (gSyslogDoOpenlog)
				gSyslogDoOpenlog("Max/MSP", 0, LOG_LOCAL0);
		}
  #endif /* COMPILE_FOR_OSX_4 */
  #if defined(COMPILE_FOR_OS9_4)
    ProcessSerialNumber	thisProcess;

    GetCurrentProcess(&thisProcess);
    Openlog(&thisProcess, LOG_LOCAL0);
 #endif /* COMPILE_FOR_OS9_4 */
    Syslog(SYSLOG_LEVEL, "------------------------------------------");
    checkGlobal->s_thing = reinterpret_cast<PObject>(checkGlobal);
  }
  ++gSyslogCount;
 #endif /* SYSLOG_OK */
#endif /* not COMPILE_FOR_STUB */
} /* acquireSyslog */

#if defined(COMPILE_FOR_STUB)
 #pragma export list acquireSyslog
#endif /* COMPILE_FOR_STUB */