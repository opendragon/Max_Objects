/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       x10_Assist.c                                                            */
/*                                                                                      */
/*  Contains:   The routine cmd_Assist().                                               */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 1996 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    1996/09/21                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "x10.h"

/*------------------------------------ cmd_Assist ---*/
Pvoid cmd_Assist
  (X10ControlPtr xx,
   Pvoid         bb,
   long          msg,
   long          arg,
   Ptr           dstString)
{
#pragma unused(xx, bb)
  EnterCallback();
#if defined(USE_ASSIST_STRING)
  assist_string(OUR_RES_NUMB, msg, arg,
                OUR_STR_OFFSET + 1, /* INLET messages are 1, 2 */
                OUR_STR_OFFSET + 3, /* OUTLET messages are 3, 4, 5, 6, 7, 8, 9, 10, 11 */
                dstString);
#else /* not USE_ASSIST_STRING */
  if (msg == ASSIST_INLET)
  {
    switch (arg)
    {
      case 0:
        strcpy(dstString, "Command channel");
        break;

      case 1:
        strcpy(dstString, "Serial device feedback");
        break;

    }
  }
  else if (msg == ASSIST_OUTLET)
  {
    switch (arg)
    {
      case 0:
        strcpy(dstString, "Commands to serial device");
        break;

      case 1:
        strcpy(dstString, "Poller output to serial device");
        break;

      case 2:
        strcpy(dstString, "Base housecode output");
        break;

      case 3:
        strcpy(dstString, "Command complete output");
        break;

      case 4:
        strcpy(dstString, "Device status output");
        break;

      case 5:
        strcpy(dstString, "Day number output");
        break;

      case 6:
        strcpy(dstString, "Hour number output");
        break;

      case 7:
        strcpy(dstString, "Minute number output");
        break;

      case 8:
        strcpy(dstString, "Error detected output");
        break;

    }
  }
#endif /* not USE_ASSIST_STRING */
  ExitMaxMessageHandler()
} /* cmd_Assist */
