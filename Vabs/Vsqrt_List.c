/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       Vsqrt_List.c                                                            */
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
/*  Created:    2001/04/23                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "Vsqrt.h"

/*------------------------------------ cmd_List ---*/
Pvoid cmd_List
  (VObjectPtr xx,
   PSymbol    message,
   short      argc,
   PAtom      argv)
{
#pragma unused(message)
  EnterCallback();
  if (xx)
  {
    PAtom newArg = NULL_PTR;
    bool  okSoFar = true;

    clearPrevious(xx);
    if (argc)
    {
      newArg = GETBYTES(argc, Atom);
      if (newArg)
      {
        PAtom newWalk = newArg, oldWalk = argv;

        for (short ii = 0; (ii < argc) && okSoFar; ii++)
        {
          newWalk->a_type = oldWalk->a_type;
          switch (oldWalk->a_type)
          {
            case A_FLOAT:
              if (oldWalk->a_w.w_float < 0)
              {
                LOG_ERROR_2(OUTPUT_PREFIX "Negative value (%g) in list",
                            double(oldWalk->a_w.w_float))
                okSoFar = false;
              }
              else
                newWalk->a_w.w_float = float(sqrt(oldWalk->a_w.w_float));
              break;

            case A_LONG:
              newWalk->a_type = A_FLOAT;
              if (oldWalk->a_w.w_long < 0)
              {
                LOG_ERROR_2(OUTPUT_PREFIX "Negative value (%ld) in list",
                            oldWalk->a_w.w_long)
                okSoFar = false;
              }
              else
                newWalk->a_w.w_float = float(sqrt(float(oldWalk->a_w.w_long)));
              break;

            default:
              okSoFar = false;
              break;

          }
          newWalk++;
          oldWalk++;
        }
      }
      else
        okSoFar = false;
    }
    if (okSoFar)
    {
      xx->fPreviousList = newArg;
      xx->fPreviousLength = argc;
      xx->fPreviousKind = A_GIMME;
      genericListOutput(xx->fResultOut, xx->fPreviousLength, xx->fPreviousList);
    }
    else
    {
      FREEBYTES(newArg, argc)
      xx->fPreviousKind = A_NOTHING;
      LOG_ERROR_1(OUTPUT_PREFIX "Non-numeric or invalid elements in list")
    }
  }
  ExitMaxMessageHandler()
} /* cmd_List */
