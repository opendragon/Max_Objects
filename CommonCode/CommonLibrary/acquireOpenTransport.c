/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       acquireOpenTransport.c                                                  */
/*                                                                                      */
/*  Contains:   The routine acquireOpenTransport().                                     */
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

#if (! defined(COMPILE_FOR_STUB))
long gObjectCount = 0;
long gOTVersion;
#endif /* not COMPILE_FOR_STUB */

/*------------------------------------ acquireOpenTransport ---*/
AccessOTControlPtr acquireOpenTransport
  (Qchar				name,
   const ushort	port,
   const bool		isServer)
{
#if defined(COMPILE_FOR_STUB)
 #pragma unused(name,port,isServer)
	return NULL_PTR;
#else /* not COMPILE_FOR_STUB */
 #if OPEN_TRANSPORT_SUPPORTED
  bool               found = false;
  PSymbol            checkGlobal = gensym(OT_CONTROL_SYMBOL);
  AccessOTControlPtr result = NULL_PTR;
  AccessOTControlPtr base = NULL_PTR;

  /* Determine if InitOpenTransport() has been called successfully in some other */
  /* object related to this one. */

  LOG_ENTER()
  if (checkGlobal->s_thing)
  {
    /* already set up: look for a match */
    base = reinterpret_cast<AccessOTControlPtr>(checkGlobal->s_thing);
  #if SYSLOG_AVAILABLE
    Syslog(LOG_DEBUG, "%s: OT already inited, base = 0x%lx", name, long(base));
  #endif /* SYSLOG_AVAILABLE */
    if (isServer)
    {
      for (AccessOTControlPtr walker = base; walker; walker = walker->fNext)
      {
        if (walker->fIsServer && (walker->fPort == port))
        {
          found = true;
          break;

        }
      }
    }
  }
  if (found)
    LOG_ERROR_2("%s: port already in use", name)
  else
  {
    /* no match: add the new control block */
    result = GETBYTES(1, AccessOTControlBlock);
  #if SYSLOG_AVAILABLE
    Syslog(LOG_DEBUG, "%s: port available for use, new block = 0x%lx", name, asLong(result));
  #endif /* SYSLOG_AVAILABLE */
    if (result)
    {
      result->fPort = port;
      result->fIsServer = isServer;
      result->fPrevious = NULL_PTR;
      result->fNext = base;
      if (base)
      {
  #if SYSLOG_AVAILABLE
        Syslog(LOG_DEBUG, "%s: putting new block on head of chain", name);
  #endif /* SYSLOG_AVAILABLE */
        base->fPrevious = result;
        checkGlobal->s_thing = reinterpret_cast<PObject>(result);
      }
      if (! gObjectCount++)
      {
        /* first call: set it up */
  #if defined(COMPILE_FOR_CATS)
        OSStatus err = InitOpenTransportInContext(kInitOTForExtensionMask,
                                                  &result->fContext);
  #else /* not COMPILE_FOR_CATS */
        OSStatus err = InitOpenTransport();
  #endif /* not COMPILE_FOR_CATS */

        if (err == kOTNoError)
        {
  #if SYSLOG_AVAILABLE
          Syslog(LOG_DEBUG, "%s: OT successfully initialized", name);
  #endif /* SYSLOG_AVAILABLE */
          checkGlobal->s_thing = reinterpret_cast<PObject>(result);        
        }
        else
        {
          LOG_ERROR_4("%s: unable to initialize Open Transport (%ld = %s)", name, err,
                      mapErrorCodeToString(err))
          FREEBYTES(result, 1)
        }
      }   
    }
    else
      LOG_ERROR_2("%s: unable to allocate memory for Open Transport Control Block", name)
  }
  #if SYSLOG_AVAILABLE
  Syslog(LOG_DEBUG, "%s: returning with result = 0x%lx", name, result);
  #endif /* SYSLOG_AVAILABLE */
  return result;
 #else /* not OPEN_TRANSPORT_SUPPORTED */
  #pragma unused(name, port, isServer)
  return NULL_PTR;
 #endif /* not OPEN_TRANSPORT_SUPPORTED */
#endif /* not COMPILE_FOR_STUB */
} /* acquireOpenTransport */

#if defined(COMPILE_FOR_STUB)
 #pragma export list acquireOpenTransport
#endif /* COMPILE_FOR_STUB */
