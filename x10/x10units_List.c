/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       x10units_List.c                                                         */
/*                                                                                      */
/*  Contains:   The routine cmd_List().                                                 */
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

#include "x10units.h"

/*------------------------------------ cmd_List ---*/
Pvoid cmd_List
  (X10UnitsPtr xx,
   PSymbol     message,
   short       argc,
   PAtom       argv)
{
#pragma unused(message)
  long   num;
  ushort sum = 0;
  bool   okSoFar = true;

  EnterCallback();
  for (short ii = 0; okSoFar && (ii < argc); ii++)
  {
    switch (argv[ii].a_type)
    {
      case A_LONG:
        num = argv[ii].a_w.w_long;
        if ((num > 0) && (num <= NUM_HOUSECODES))
          sum |= kUnitCodeToBits[num - 1];
        else
        {
          LOG_ERROR_3(OUTPUT_PREFIX "argument %hd is out of range (%ld)", ii, num)
          okSoFar = false;
        }
        break;

      case A_SYM:
        LOG_ERROR_3(OUTPUT_PREFIX "argument %hd is a symbol (%s)", ii,
                    argv[ii].a_w.w_sym->s_name)
        okSoFar = false;
        break;

      case A_FLOAT:
        LOG_ERROR_3(OUTPUT_PREFIX "argument %hd is a float (%g)", ii,
                    double(argv[ii].a_w.w_float))
        okSoFar = false;
        break;

      default:
        LOG_ERROR_2(OUTPUT_PREFIX "input of an unknown type (%hd) seen", argv[ii].a_type)
        okSoFar = false;
        break;

    }
  }
  if (okSoFar)
  {
    xx->fPreviousResult = sum;
    outlet_int(xx->fResultOut, long(xx->fPreviousResult));
  }
  ExitMaxMessageHandler()
} /* cmd_List */
