/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       qt_Unload.c                                                             */
/*                                                                                      */
/*  Contains:   The routine cmd_Unload().                                               */
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
/*  Created:    1998/10/26                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "qtCommon.h"

/*------------------------------------ deferred_Unload ---*/
Pvoid deferred_Unload
  (QtPtr   xx,
   PSymbol name)
{
  EnterCallback();
  if (name == gEmptySymbol)
  {
    QtMovieHdl aMovie = xx->fFirstMovie, aNext;
    QtMoviePtr thisMovie;

    if (aMovie)
    {
      qtSetCurrentMovie(xx, NULL_HDL);
      for ( ; aMovie; aMovie = aNext)
      {
        HLock(reinterpret_cast<Handle>(aMovie));
        thisMovie = *aMovie;
        aNext = thisMovie->fNextMovie;
        qtDisposeOfMovie(thisMovie);            
        HUnlock(reinterpret_cast<Handle>(aMovie));
        disposhandle(reinterpret_cast<Handle>(aMovie));
      }
    }
    xx->fPendingMovie = NULL_PTR;
    xx->fFirstMovie = xx->fLastMovie = NULL_HDL;
    xx->fMovieCount = 0;
  }
  else
  {
    QtMovieHdl temp = qtSearchKnownMovies(xx, name);

    if (temp)
      qtRemoveMovie(xx, temp);
    else
    {
      LOG_ERROR_2(OUTPUT_PREFIX "movie '%s' not found", name->s_name)
      SignalError(xx);
    }
  }
  ExitMaxMessageHandler()
} /* deferred_Unload */

/*------------------------------------ cmd_Unload ---*/
Pvoid cmd_Unload
  (QtPtr   xx,
   PSymbol name)
{
  EnterCallback();
#if defined(TRACE_MESSAGES)
  LOG_POST_1("Unload %s", name->s_name)
#endif /* TRACE_MESSAGES */
  if (xx)
    defer_low(xx, reinterpret_cast<method>(deferred_Unload),
              name, 0, NULL_PTR);
  ExitMaxMessageHandler()
} /* cmd_Unload */
