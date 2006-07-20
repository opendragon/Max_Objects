/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       Vltrim_Assist.c                                                         */
/*                                                                                      */
/*  Contains:   The routine cmd_Assist().                                               */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2003 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2003/06/16                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "Vltrim.h"

/*------------------------------------ cmd_Assist ---*/
Pvoid cmd_Assist
  (VObjectPtr xx,
   Pvoid      bb,
   long       msg,
   long       arg,
   Ptr        dstString)
{
#pragma unused(xx, bb, arg)
  EnterCallback();
#if defined(USE_ASSIST_STRING)
  assist_string(OUR_RES_NUMB, msg, arg,
                OUR_STR_OFFSET + 1, /* INLET messages are 1 */
                OUR_STR_OFFSET + 2, /* OUTLET messages are 2, 3 */
                dstString);
#else /* not USE_ASSIST_STRING */
  if (msg == ASSIST_INLET)
  	strcpy(dstString, "List input");
  else
  {
    switch (arg)
    {
      case 0:
        strcpy(dstString, "List output");
        break;

      case 1:
        strcpy(dstString, "Empty output");
        break;

    }
  }
#endif /* not USE_ASSIST_STRING */
  ExitMaxMessageHandler()
} /* cmd_Assist */
