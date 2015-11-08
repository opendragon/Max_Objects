/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       fileLogger_Anything.cpp                                                 */
/*                                                                                      */
/*  Contains:   The routine cmd_Anything().                                             */
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

#include "fileLogger.h"

/*------------------------------------ deferred_Anything ---*/
static void deferred_Anything(FileLoggerData * xx,
                              t_symbol *       message,
                              short            argc,
                              t_atom *         argv)
{
    bool        okSoFar = fileLoggerGetTheFile(xx);
    static char numBuffer[NUM_BUFF_SIZE];

    if (okSoFar)
    {
        okSoFar = fileLoggerWriteStringToTheFile(xx, message->s_name);
        for (short ii = 0; okSoFar && (ii < argc); ++ii)
        {
            const char * toAdd = NULL_PTR;

            okSoFar = fileLoggerWriteStringToTheFile(xx, " ");
            if (okSoFar)
            {
                switch (argv[ii].a_type)
                {
                    case A_LONG:
                        snprintf(numBuffer, sizeof(numBuffer), "%ld", argv[ii].a_w.w_long);
                        toAdd = numBuffer;
                        break;

                    case A_SYM:
                        toAdd = argv[ii].a_w.w_sym->s_name;
                        break;

                    case A_FLOAT:
                        snprintf(numBuffer, sizeof(numBuffer), "%g", static_cast<double>(argv[ii].a_w.w_float));
                        toAdd = numBuffer;
                        break;

                    case A_SEMI:
                        toAdd = ";";
                        break;

                    case A_COMMA:
                        toAdd = ",";
                        break;

                    case A_DOLLAR:
                        toAdd = "$";
                        break;

                    default:
                        toAdd = "<unknown type>";
                        break;
                }
                if (toAdd)
                {
                    okSoFar = fileLoggerWriteStringToTheFile(xx, toAdd);
                }
            }
        }
        if (okSoFar)
        {
            fileLoggerWriteStringToTheFile(xx, "\n");
        }
        fileLoggerReleaseTheFile(xx);
    }
} // deferred_Anything

/*------------------------------------ cmd_Anything ---*/
void cmd_Anything(FileLoggerData * xx,
                  t_symbol *       message,
                  short            argc,
                  t_atom *         argv)
{
    if (xx)
    {
        defer(xx, reinterpret_cast<method>(deferred_Anything), message, argc, argv);
    }
} // cmd_Anything