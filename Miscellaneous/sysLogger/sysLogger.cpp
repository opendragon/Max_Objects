/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       sysLogger.cpp                                                           */
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
#include "sysLogger.h"
#include "reportVersion.h"

/*------------------------------------ sysLoggerCreate ---*/
static void *
sysLoggerCreate(t_symbol * level)
{
    SysLoggerData * xx = static_cast<SysLoggerData *>(object_alloc(gClass));
    
    if (xx)
    {
        if (level == gAlertSymbol)
        {
#if USE_ASL_INSTEAD_OF_SYSLOG
            xx->fLevel = ASL_LEVEL_ALERT;
#else // not USE_ASL_INSTEAD_OF_SYSLOG
            xx->fLevel = LOG_ALERT;
#endif // not USE_ASL_INSTEAD_OF_SYSLOG
        }
        else if (level == gCriticalSymbol)
        {
#if USE_ASL_INSTEAD_OF_SYSLOG
            xx->fLevel = ASL_LEVEL_CRIT;
#else // not USE_ASL_INSTEAD_OF_SYSLOG
            xx->fLevel = LOG_CRIT;
#endif // not USE_ASL_INSTEAD_OF_SYSLOG
        }
        else if (level == gDebugSymbol)
        {
#if USE_ASL_INSTEAD_OF_SYSLOG
            xx->fLevel = ASL_LEVEL_DEBUG;
#else // not USE_ASL_INSTEAD_OF_SYSLOG
            xx->fLevel = LOG_DEBUG;
#endif // not USE_ASL_INSTEAD_OF_SYSLOG
        }
        else if (level == gEmergencySymbol)
        {
#if USE_ASL_INSTEAD_OF_SYSLOG
            xx->fLevel = ASL_LEVEL_EMERG;
#else // not USE_ASL_INSTEAD_OF_SYSLOG
            xx->fLevel = LOG_EMERG;
#endif // not USE_ASL_INSTEAD_OF_SYSLOG
        }
        else if ((level == gEmptySymbol) || (level == gInfoSymbol))
        {
#if USE_ASL_INSTEAD_OF_SYSLOG
            xx->fLevel = ASL_LEVEL_INFO;
#else // not USE_ASL_INSTEAD_OF_SYSLOG
            xx->fLevel = LOG_INFO;
#endif // not USE_ASL_INSTEAD_OF_SYSLOG
        }
        else if (level == gErrorSymbol)
        {
#if USE_ASL_INSTEAD_OF_SYSLOG
            xx->fLevel = ASL_LEVEL_ERR;
#else // not USE_ASL_INSTEAD_OF_SYSLOG
            xx->fLevel = LOG_ERR;
#endif // not USE_ASL_INSTEAD_OF_SYSLOG
        }
        else if (level == gNoticeSymbol)
        {
#if USE_ASL_INSTEAD_OF_SYSLOG
            xx->fLevel = ASL_LEVEL_NOTICE;
#else // not USE_ASL_INSTEAD_OF_SYSLOG
            xx->fLevel = LOG_NOTICE;
#endif // not USE_ASL_INSTEAD_OF_SYSLOG
        }
        else if (level == gWarningSymbol)
        {
#if USE_ASL_INSTEAD_OF_SYSLOG
            xx->fLevel = ASL_LEVEL_WARNING;
#else // not USE_ASL_INSTEAD_OF_SYSLOG
            xx->fLevel = LOG_WARNING;
#endif // not USE_ASL_INSTEAD_OF_SYSLOG
        }
        else
        {
            LOG_ERROR_1(xx, OUTPUT_PREFIX "unrecognized logging level for object")
#if USE_ASL_INSTEAD_OF_SYSLOG
            xx->fLevel = ASL_LEVEL_INFO;
#else // not USE_ASL_INSTEAD_OF_SYSLOG
            xx->fLevel = LOG_INFO;
#endif // not USE_ASL_INSTEAD_OF_SYSLOG
        }
        xx->fBuffer = GET_BYTES(MAX_BUFFER_SIZE, char);
        if (xx->fBuffer)
        {
#if USE_ASL_INSTEAD_OF_SYSLOG
            xx->fAslClient = asl_open(OUR_NAME, "Max/MSP ASL Facility", ASL_OPT_NO_DELAY);
            if (! xx->fAslClient)
            {
                LOG_ERROR_1(xx, OUTPUT_PREFIX "unable to open logging facility for object")
                freeobject(reinterpret_cast<t_object *>(xx));
                xx = NULL;
            }
#else // not USE_ASL_INSTEAD_OF_SYSLOG
            openlog(OUR_NAME, LOG_CONS | LOG_PERROR, LOG_LOCAL0);
#endif // not USE_ASL_INSTEAD_OF_SYSLOG
        }
        else
        {
            LOG_ERROR_1(xx, OUTPUT_PREFIX "unable to allocate memory for object")
            freeobject(reinterpret_cast<t_object *>(xx));
            xx = NULL;
        }
    }
    return xx;
} // sysLoggerCreate

/*------------------------------------ sysLoggerFree ---*/
static void
sysLoggerFree(SysLoggerData * xx)
{
    if (xx)
    {
#if USE_ASL_INSTEAD_OF_SYSLOG
        asl_close(xx->fAslClient);
        xx->fAslClient = NULL;
#else // not USE_ASL_INSTEAD_OF_SYSLOG
        closelog();
#endif // not USE_ASL_INSTEAD_OF_SYSLOG
        FREE_BYTES(xx->fBuffer);
    }
} // sysLoggerFree

/*------------------------------------ main ---*/
int C74_EXPORT
main(void)
{
    /* Allocate class memory and set up class. */
    t_class * temp = class_new(OUR_NAME, reinterpret_cast<method>(sysLoggerCreate),
                               reinterpret_cast<method>(sysLoggerFree),
                               static_cast<long>(sizeof(SysLoggerData)),
                               reinterpret_cast<method>(NULL), A_DEFSYM, 0);

    if (temp)
    {
        class_addmethod(temp, reinterpret_cast<method>(cmd_Anything), MESSAGE_ANYTHING, A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Assist), MESSAGE_ASSIST, A_CANT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Float), MESSAGE_FLOAT, A_FLOAT, 0);
        class_addmethod(temp, reinterpret_cast<method>(stdinletinfo), MESSAGE_INLETINFO, A_CANT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Int), MESSAGE_INT, A_LONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_List), MESSAGE_LIST, A_GIMME, 0);
        class_register(CLASS_BOX, temp);
    }
    gClass = temp;
    gAlertSymbol = gensym("alert");
    gCriticalSymbol = gensym("critical");
    gDebugSymbol = gensym("debug");
    gEmergencySymbol = gensym("emergency");
    gEmptySymbol = gensym("");
    gErrorSymbol = gensym("error");
    gInfoSymbol = gensym("info");
    gNoticeSymbol = gensym("notice");
    gWarningSymbol = gensym("warning");
    reportVersion();
    return 0;
} // main
