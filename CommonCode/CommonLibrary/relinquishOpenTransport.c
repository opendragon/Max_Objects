/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       relinquishOpenTransport.c                                               */
/*                                                                                      */
/*  Contains:   The routine relinquishOpenTransport().                                  */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2004 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2004/02/09                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "acquireOpenTransport.h"

/*------------------------------------ relinquishOpenTransport ---*/
void relinquishOpenTransport
  (Qchar							name,
   AccessOTControlHdl reference)
{
#if defined(COMPILE_FOR_STUB)
 #pragma unused(name,reference)
#else /* not COMPILE_FOR_STUB */
 #if OPEN_TRANSPORT_SUPPORTED
  PSymbol            checkGlobal = gensym(OT_CONTROL_SYMBOL);
  bool               found = false;
  AccessOTControlPtr base = NULL_PTR;

  if (reference)
  {
    /* Determine if InitOpenTransport() has been called successfully in some other */
    /* object related to this one. */
    AccessOTControlPtr candidate = *reference;
    AccessOTControlPtr walker;

  #if SYSLOG_AVAILABLE
    Syslog(LOG_DEBUG, "%s: relinquishing 0x%lx", name, long(candidate));
  #endif /* SYSLOG_AVAILABLE */
    if (checkGlobal->s_thing)
    {
      walker = base = reinterpret_cast<AccessOTControlPtr>(checkGlobal->s_thing);
      /* locate the control block: */
      for ( ; walker; walker = walker->fNext)
      {
        if (walker == candidate)
        {
          found = true;
          break;

        }
      }
      if (found)
      {
  #if defined(COMPILE_FOR_CATS)
        OTClientContextPtr  context = candidate->fContext;
  #endif /* COMPILE_FOR_CATS */
 
        /* found it - release it */
        if (candidate->fNext)
          candidate->fNext->fPrevious = candidate->fPrevious;
        if (candidate->fPrevious)
          candidate->fPrevious->fNext = candidate->fNext;
        else
        {
          /* it was the first block in the chain */
          base = candidate->fNext;
          checkGlobal->s_thing = reinterpret_cast<PObject>(base);
        }
        FREEBYTES(candidate, 1)
        if (! --gObjectCount)
 #if defined(COMPILE_FOR_CATS)
          CloseOpenTransportInContext(context);
 #else /* not COMPILE_FOR_CATS */
          CloseOpenTransport();
 #endif /* not COMPILE_FOR_CATS */
      }
      else
        LOG_ERROR_2("%s: object not recognized as an Open Transport object", name)
    }
    else
      LOG_ERROR_2("%s: Open Transport not initialized", name)
    *reference = NULL_PTR;
    }
 #else /* not OPEN_TRANSPORT_SUPPORTED */
  #pragma unused(name, reference)
 #endif /* not OPEN_TRANSPORT_SUPPORTED */
#endif /* not COMPILE_FOR_STUB */
} /* relinquishOpenTransport */

#if defined(COMPILE_FOR_STUB)
 #pragma export list relinquishOpenTransport
#endif /* COMPILE_FOR_STUB */