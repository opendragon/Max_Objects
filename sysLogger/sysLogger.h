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
# define SYSLOGGER_H_ /* */

# define USE_ASL_INSTEAD_OF_SYSLOG  TRUE
# include "missingAndExtra.h"
# if USE_ASL_INSTEAD_OF_SYSLOG
#  include <asl.h>
# else // not USE_ASL_INSTEAD_OF_SYSLOG
#  include <syslog.h>
# endif // not USE_ASL_INSTEAD_OF_SYSLOG

# define OUR_NAME      "sysLogger"
// # define OUR_RES_NUMB  17187
# define OUTPUT_PREFIX "sysLogger: "

# define MAX_BUFFER_SIZE            1024

struct SysLoggerData
{
    t_object  fObject;
# if USE_ASL_INSTEAD_OF_SYSLOG
    aslclient fAslClient;
# endif // not USE_ASL_INSTEAD_OF_SYSLOG
    char *    fBuffer;
    int       fLevel;
}; // SysLoggerData

StandardRoutineDeclarations(SysLoggerData *);

mextern(t_symbol *) gAlertSymbol;     /* Pointer to unique symbol for 'alert' */
mextern(t_symbol *) gCriticalSymbol;  /* Pointer to unique symbol for 'critical' */
mextern(t_symbol *) gDebugSymbol;     /* Pointer to unique symbol for 'debug' */
mextern(t_symbol *) gEmergencySymbol; /* Pointer to unique symbol for 'emergency' */
mextern(t_symbol *) gEmptySymbol;     /* Pointer to unique symbol for '' */
mextern(t_symbol *) gErrorSymbol;     /* Pointer to unique symbol for 'error' */
mextern(t_symbol *) gInfoSymbol;      /* Pointer to unique symbol for 'info' */
mextern(t_symbol *) gNoticeSymbol;    /* Pointer to unique symbol for 'notice' */
mextern(t_symbol *) gWarningSymbol;   /* Pointer to unique symbol for 'warning' */

#endif /* not SYSLOGGER_H_ */
