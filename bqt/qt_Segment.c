/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       qt_Segment.c                                                            */
/*                                                                                      */
/*  Contains:   The routine cmd_Segment().                                              */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 1998 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    1998/10/28                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "qtCommon.h"

/*------------------------------------ deferred_Segment ---*/
static Pvoid
deferred_Segment(QtPtr   xx,
                 PSymbol ss,
                 short   argc,
                 PAtom   argv)
{
#pragma unused(argc, ss)
  EnterCallback();
  if (xx->fCurrentMovie)
  {
    TimeValue begin = argv->a_w.w_long;
    TimeValue end = (argv + 1)->a_w.w_long;

    if ((! begin) && (end <= 0))
    {
      /* Request to play the whole movie. */
      xx->fBeginSegment = -1;
      xx->fSegmentDuration = 0;
    }
    else
    {
      xx->fBeginSegment = begin;
      if (end < 0)
        xx->fSegmentDuration = xx->fCurrentDuration - begin;
      else
        xx->fSegmentDuration = end - begin;
    }
    SetMovieActiveSegment(xx->fCurrentMovie, xx->fBeginSegment, xx->fSegmentDuration);
    qtAdjustController(xx);
  }
  else
  {
    LOG_ERROR_1(OUTPUT_PREFIX "no movies loaded")
    SignalError(xx);
  }
  ExitMaxMessageHandler()
} /* deferred_Segment */

/*------------------------------------ cmd_Segment ---*/
Pvoid
cmd_Segment(QtPtr xx,
            long  begin,
            long  end)
{
  Atom avSegment[2];

  EnterCallback();
#if defined(TRACE_MESSAGES)
  LOG_POST_1("Segment")
#endif /* TRACE_MESSAGES */
  SETLONG(avSegment, begin);
  SETLONG(avSegment + 1, end);
  if (xx)
    defer(xx, reinterpret_cast<method>(deferred_Segment),
          NULL_PTR, 2, avSegment);
  ExitMaxMessageHandler()
} /* cmd_Segment */
