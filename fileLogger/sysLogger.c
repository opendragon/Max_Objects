/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       sysLogger.c                                                             */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the sysLogger module.     */
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

#define IS_MAIN /* */

#define USE_SYSLOG  /* must be set in order to pull in the definitions that we need! */

#include "sysLogger.h"
#include "displayAboutBox.h"
#include "loadAboutBoxData.h"
#include "loadOtherSegments.h"
#include "reportVersion.h"

/* Forward references: */
Pvoid sysLoggerCreate
  (PSymbol level);

Pvoid sysLoggerFree
  (SysLoggerPtr xx);

/*------------------------------------ main ---*/
void main
  (Pfptr ff)
{
  EnterCodeResource();
  PrepareCallback();
  FNS = ff;   /* Connect up the function macros. */
  /* Allocate class memory and set up class. */
  setup(reinterpret_cast<t_messlist**>(&gClass), reinterpret_cast<method>(sysLoggerCreate),
        reinterpret_cast<method>(sysLoggerFree), short(sizeof(SysLoggerData)),
        reinterpret_cast<method>(0L), A_DEFSYM, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Anything), "anything", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Assist), "assist", A_CANT, A_NOTHING);
  addfloat(reinterpret_cast<method>(cmd_Float));
  addmess(reinterpret_cast<method>(cmd_Info), "info", A_CANT, A_NOTHING);
  addint(reinterpret_cast<method>(cmd_Int));
  addmess(reinterpret_cast<method>(cmd_List), "list", A_GIMME, A_NOTHING);

  /* Add object class to New Object List */
  finder_addclass("Messages", OUR_NAME);
  /* Make sure that our string list resource is present */
  rescopy('STR#', OUR_RES_NUMB);
  loadAboutBoxData();
  gAlertSymbol = gensym("alert");
  gCriticalSymbol = gensym("critical");
  gDebugSymbol = gensym("debug");
  gEmergencySymbol = gensym("emergency");
  gEmptySymbol = gensym("");
  gErrorSymbol = gensym("error");
  gInfoSymbol = gensym("info");
  gNoticeSymbol = gensym("notice");
  gWarningSymbol = gensym("warning");
  gVersionNumber = reportVersion(OUR_NAME);
  loadOtherSegments();
#if (! SYSLOG_OK)
  error(OUTPUT_PREFIX "not supported on Max/68K");
#endif /* not SYSLOG_OK */
  ExitCodeResource();
} /* main */

/*------------------------------------ sysLoggerCreate ---*/
Pvoid sysLoggerCreate
  (PSymbol level)
{
#if (! SYSLOG_OK)
 #pragma unused(level)
#endif /* not SYSLOG_OK */
  SysLoggerPtr xx;

  EnterCallback();
  LOG_ENTER()
  xx = static_cast<SysLoggerPtr>(newobject(gClass));
  if (xx)
  {
#if SYSLOG_OK
    xx->fLevel = LOG_INFO;
    if (level == gAlertSymbol)
      xx->fLevel = LOG_ALERT;
    else if (level == gCriticalSymbol)
      xx->fLevel = LOG_CRIT;
    else if (level == gDebugSymbol)
      xx->fLevel = LOG_DEBUG;
    else if (level == gEmergencySymbol)
      xx->fLevel = LOG_EMERG;
    else if ((level == gEmptySymbol) || (level == gInfoSymbol))
      xx->fLevel = LOG_INFO;
    else if (level == gErrorSymbol)
      xx->fLevel = LOG_ERR;
    else if (level == gNoticeSymbol)
      xx->fLevel = LOG_NOTICE;
    else if (level == gWarningSymbol)
      xx->fLevel = LOG_WARNING;
    else
      LOG_ERROR_1(OUTPUT_PREFIX "unrecognized logging level for object")
    xx->fBuffer = GETBYTES(MAX_BUFFER_SIZE, char);
    if (xx->fBuffer)
      acquireSyslog();
    else
    {
      LOG_ERROR_1(OUTPUT_PREFIX "unable to allocate memory for object")
      freeobject(reinterpret_cast<PObject>(xx));
      xx = NULL_PTR;
    }
#endif /* SYSLOG_OK */
  }
  ExitCodeResource();
  return xx;
} /* sysLoggerCreate */

/*------------------------------------ sysLoggerFree ---*/
Pvoid sysLoggerFree
  (SysLoggerPtr xx)
{
  EnterCallback();
  if (xx)
  {
#if SYSLOG_OK
    FREEBYTES(xx->fBuffer, MAX_BUFFER_SIZE)
    relinquishSyslog();
#endif /* SYSLOG_OK */
  }
  LOG_EXIT()
  ExitMaxMessageHandler()
} /* sysLoggerFree */

StandardInfoRoutine(SysLoggerPtr)