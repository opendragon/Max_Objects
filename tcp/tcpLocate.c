/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       tcpLocate.c                                                             */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the tcpLocate module.     */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2000 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2000/11/06                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#define IS_MAIN /* */
#include "tcpLocate.h"
#include "displayAboutBox.h"
#include "loadAboutBoxData.h"
#include "loadOtherSegments.h"
#include "reportVersion.h"

/* Forward references: */
Pvoid tcpLocateCreate
  (void);

Pvoid tcpLocateFree
  (TcpLocatePtr xx);

/*------------------------------------ main ---*/
void main
  (Pfptr ff)
{
  EnterCodeResource();
  PrepareCallback();
  FNS = ff;   /* Connect up the function macros. */
  if (isOpenTransportAvailable(OUR_NAME))
  {
    /* Allocate class memory and set up class. */
    setup(reinterpret_cast<t_messlist**>(&gClass),
          reinterpret_cast<method>(tcpLocateCreate),
          reinterpret_cast<method>(tcpLocateFree), short(sizeof(TcpLocateData)),
          reinterpret_cast<method>(0L), A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Anything), "anything", A_GIMME, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Assist), "assist", A_CANT, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Info), "info", A_NOTHING);
#if defined(BE_VERBOSE)
    addmess(reinterpret_cast<method>(cmd_Verbose), "verbose", A_DEFSYM, A_NOTHING);
#endif /* BE_VERBOSE */
    /* Add object class to New Object List */
    finder_addclass("Devices", OUR_NAME);
    /* Make sure that our string list resource is present */
    rescopy('STR#', OUR_RES_NUMB);
    loadAboutBoxData();
    gEmptySymbol = gensym("");
    gOffSymbol = gensym("off");
    gOnSymbol = gensym("on");
    setUpStateSymbols();
    gVersionNumber = reportVersion(OUR_NAME);
    loadOtherSegments();
  }
#if (! OPEN_TRANSPORT_SUPPORTED)
  error(OUTPUT_PREFIX "not supported on Max/68K");
#endif /* not OPEN_TRANSPORT_SUPPORTED */
  ExitCodeResource();
} /* main */

/*------------------------------------ tcpLocateCreate ---*/
Pvoid tcpLocateCreate
  (void)
{
  TcpLocatePtr xx;

  EnterCallback();
  LOG_ENTER()
  xx = static_cast<TcpLocatePtr>(newobject(gClass));
  if (xx)
  {
    bool      okSoFar = true;
#if OPEN_TRANSPORT_SUPPORTED
    OSStatus  result;
#endif /* OPEN_TRANSPORT_SUPPORTED */

#if defined(BE_VERBOSE)
    xx->fVerbose = false;
#endif /* BE_VERBOSE */
    xx->fErrorBangOut = xx->fResultOut = NULL_PTR;
    if (okSoFar)
      okSoFar = initObject(xx);
#if OPEN_TRANSPORT_SUPPORTED
    if (okSoFar)
    {
      xx->fAccessControl = acquireOpenTransport(OUR_NAME, 0, false);
      if (! xx->fAccessControl)
        okSoFar = false;
    }
    /* Set up the endpoint */
    if (okSoFar)
    {
 #if defined(COMPILE_FOR_CATS)
      xx->fService = OTOpenInternetServicesInContext(kDefaultInternetServicesPath, 0,
                                                      &result,
                                                      xx->fAccessControl->fContext);
 #else /* not COMPILE_FOR_CATS */
      xx->fService = OTOpenInternetServices(kDefaultInternetServicesPath, 0, &result);
 #endif /* not COMPILE_FOR_CATS */
      if (result != kOTNoError)
      {
        REPORT_ERROR(OUTPUT_PREFIX "unable to obtain an endpoint (%ld = %s)", result)
        okSoFar = false;
      }
    }
#endif /* OPEN_TRANSPORT_SUPPORTED */
    if (! okSoFar)
    {
      freeobject(reinterpret_cast<PObject>(xx));
      xx = NULL_PTR;
    }
  }
  ExitCodeResource();
  return xx;
} /* tcpLocateCreate */

/*------------------------------------ tcpLocateFree ---*/
Pvoid tcpLocateFree
  (TcpLocatePtr xx)
{
  EnterCallback();
  if (xx)
  {
#if OPEN_TRANSPORT_SUPPORTED
    OSStatus result;

    WRAP_OT_CALL(xx, result, "OTCloseProvider", OTCloseProvider(xx->fService))
    relinquishOpenTransport(OUR_NAME, &xx->fAccessControl);
#endif /* OPEN_TRANSPORT_SUPPORTED */
  }
  LOG_EXIT()
  ExitMaxMessageHandler()
} /* tcpLocateFree */

StandardInfoRoutine(TcpLocatePtr)