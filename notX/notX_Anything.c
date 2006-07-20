/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       notX_Anything.c                                                         */
/*                                                                                      */
/*  Contains:   The routine cmd_Anything().                                             */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 1999 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    1999/12/05                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "notX.h"

/*------------------------------------ cmd_Anything ---*/
Pvoid cmd_Anything
  (NotXPtr xx,
   PSymbol message,
   short   argc,
   PAtom   argv)
{
  EnterCallback();
  if (xx)
  {
    PAtom newArg = GETBYTES(argc + 1, Atom);

    clearPrevious(xx);
    if (newArg)
    {
      short badArgs = 0;

      SETSYM(newArg, message);
      memcpy(newArg + 1, argv, argc * sizeof(Atom));
      for (short ii = 0; ii <= argc; ii++)
      {
        switch (newArg[ii].a_type)
        {
          case A_LONG:
            newArg[ii].a_w.w_long = (! newArg[ii].a_w.w_long);
            break;

          case A_SYM:
            badArgs++;
            break;

          case A_FLOAT:
            SETLONG(newArg + ii, long(newArg[ii].a_w.w_float));
            break;

          default:
            badArgs++;
            break;

        }
      }
      if (badArgs)
        LOG_ERROR_1(OUTPUT_PREFIX "Non-numeric elements in list")
      else
        LOG_ERROR_2(OUTPUT_PREFIX "Leading element of list (%s) is non-numeric", message->s_name)
      xx->fPreviousList = newArg;
      xx->fPreviousLength = short(argc + 1);
      genericListOutput(xx->fResultOut, xx->fPreviousLength, xx->fPreviousList);
    }
  }
  ExitMaxMessageHandler()
} /* cmd_Anything */
