/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       map1d_Before.c                                                          */
/*                                                                                      */
/*  Contains:   The routine cmd_Before().                                               */
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
/*  Created:    2001/05/02                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "map1d.h"

/*------------------------------------ cmd_Before ---*/
Pvoid cmd_Before
  (Map1dPtr xx,
   PSymbol  message,
   short    argc,
   PAtom    argv)
{
#pragma unused(message)
  EnterCallback();
  if (xx)
  {
    short num;

    if (argc > 1)
    {
      switch (argv->a_type)
      {
        case A_LONG:
          num = short(argv->a_w.w_long);
          if ((num > 0) && (num <= xx->fRangeCount))
          {
            RangeDataPtr newRange = map1dConvertListToRange(1, argc, argv);

            if (newRange)
            {
              RangeDataPtr prev = NULL_PTR, walker = xx->fFirstRange;

              for ( ; walker && (--num); )
              {
                prev = walker;
                walker = walker->fNext;
              }
              newRange->fNext = walker;
              if (prev)
                prev->fNext = newRange;
              else
                xx->fFirstRange = newRange;
              xx->fRangeCount++;
            }
          }
          break;

        case A_FLOAT:
          LOG_ERROR_2("unexpected float (%g)", double(argv->a_w.w_float))
          break;

        case A_SYM:
          LOG_ERROR_2("unexpected symbol (%s)", argv->a_w.w_sym->s_name)
          break;

        default:
          LOG_ERROR_2(OUTPUT_PREFIX "input of an unknown type (%hd) seen", argv->a_type)
          break;

      }
    }
  }
  ExitMaxMessageHandler()
} /* cmd_Before */
