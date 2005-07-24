/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       sysLogger.h                                                             */
/*                                                                                      */
/*  Contains:   The data type declarations and forward references to routines.          */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2002 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2002/03/17                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(SYSLOGGER_H_))
 #define SYSLOGGER_H_ /* */

 #define USE_SYSLOG  /* must be set in order to pull in the definitions that we need! */
 
 #include "MissingAndExtra.h"
 
 #define OUR_NAME      "sysLogger"
 #define OUR_RES_NUMB  17187
 #define OUTPUT_PREFIX "sysLogger: "
 
 #define MAX_BUFFER_SIZE 1024

struct SysLoggerData
{
  Object fObject;
  Pchar  fBuffer;
  uchar  fLevel;
}; /* SysLoggerData */

typedef SysLoggerData * SysLoggerPtr;

StandardRoutineDeclarations(SysLoggerPtr)

mextern(PSymbol) gAlertSymbol;     /* Pointer to unique Symbol for 'alert' */
mextern(PSymbol) gCriticalSymbol;  /* Pointer to unique Symbol for 'critical' */
mextern(PSymbol) gDebugSymbol;     /* Pointer to unique Symbol for 'debug' */
mextern(PSymbol) gEmergencySymbol; /* Pointer to unique Symbol for 'emergency' */
mextern(PSymbol) gEmptySymbol;     /* Pointer to unique Symbol for '' */
mextern(PSymbol) gErrorSymbol;     /* Pointer to unique Symbol for 'error' */
mextern(PSymbol) gInfoSymbol;      /* Pointer to unique Symbol for 'info' */
mextern(PSymbol) gNoticeSymbol;    /* Pointer to unique Symbol for 'notice' */
mextern(PSymbol) gWarningSymbol;   /* Pointer to unique Symbol for 'warning' */

#endif /* not SYSLOGGER_H_ */
