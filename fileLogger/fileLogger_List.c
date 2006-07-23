/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       fileLogger_List.c                                                       */
/*                                                                                      */
/*  Contains:   The routine cmd_List().                                                 */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2001 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2001/04/03                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "fileLogger.h"

/*------------------------------------ cmd_List ---*/
Pvoid cmd_List
  (FileLoggerPtr xx,
   PSymbol       message,
   short         argc,
   PAtom         argv)
{
#pragma unused(message)
  EnterCallback();
  if (xx)
  {
    bool        okSoFar = fileLoggerGetTheFile(xx);
    static char numBuffer[32];

    if (okSoFar)
    {
      for (short ii = 0; okSoFar && (ii < argc); ++ii)
      {
        Ptr toAdd = NULL_PTR;

        if (ii)
          okSoFar = fileLoggerWriteStringToTheFile(xx, " ");
        if (okSoFar)
        {
          switch (argv[ii].a_type)
          {
            case A_LONG:
              sprintf(numBuffer, "%ld", argv[ii].a_w.w_long);
              toAdd = numBuffer;
              break;

            case A_SYM:
              toAdd = argv[ii].a_w.w_sym->s_name;
              break;

            case A_FLOAT:
              sprintf(numBuffer, "%g", double(argv[ii].a_w.w_float));
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
            okSoFar = fileLoggerWriteStringToTheFile(xx, toAdd);
        }
      }
      if (okSoFar)
        fileLoggerWriteStringToTheFile(xx, "\r");
      fileLoggerReleaseTheFile(xx);
    }
  }
  ExitMaxMessageHandler()
} /* cmd_List */
