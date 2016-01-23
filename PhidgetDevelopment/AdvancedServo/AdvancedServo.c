/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       AdvancedServo.c                                                         */
/*                                                                                      */
/*  Contains:   The main entry points for this phidget.                                 */
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
/*  Created:    2003/12/20                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

//#define USE_SYSLOG /* */
#define USE_DEFAULT /* */

#include "AdvancedServo.h"
#include "genericListOutput.h"

#if defined(COMPILE_FOR_OSX_4)
/*------------------------------------ defineCallback ---*/
E_PhidgResult
defineCallback(STANDARD_PHID_ARGS_DEFINECALLBACK)
{
 #pragma unused(name,sharedStorage,privateStorage,thisDevice,aFun,cookies,numCookies)
    *result = noErr;
    return kPhidgSuccess;
} /* defineCallback */
#endif /* COMPILE_FOR_OSX_4 */
    
/*------------------------------------ doCustom ---*/
E_PhidgResult
doCustom(STANDARD_PHID_ARGS_DO)
{
#if defined(USE_DEFAULT)
 #pragma unused(name,deviceType,outlet,sharedStorage,privateStorage,thisDevice,argc,argv)
    *result = noErr;
    return kPhidgDoDefault;
#else /* not USE_DEFAULT */
 #pragma unused(name,deviceType,outlet,sharedStorage,privateStorage,thisDevice,argc,argv)
    *result = noErr;
    return kPhidgSuccess;
#endif /* not USE_DEFAULT */
} /* doCustom */

/*------------------------------------ doGet ---*/
E_PhidgResult
doGet(STANDARD_PHID_ARGS_DO)
{
#if defined(USE_DEFAULT)
 #pragma unused(name,deviceType,outlet,sharedStorage,privateStorage,thisDevice,argc,argv)
    *result = noErr;
    return kPhidgDoDefault;
#else /* not USE_DEFAULT */
 #pragma unused(name,deviceType,outlet,sharedStorage,privateStorage,thisDevice,argc,argv)
    *result = noErr;
    return kPhidgSuccess;
#endif /* not USE_DEFAULT */
} /* doGet */

/*------------------------------------ doPut ---*/
E_PhidgResult
doPut(STANDARD_PHID_ARGS_DO)
{
#if defined(USE_DEFAULT)
 #pragma unused(name,deviceType,outlet,sharedStorage,privateStorage,thisDevice,argc,argv)
    *result = noErr;
    return kPhidgDoDefault;
#else /* not USE_DEFAULT */
 #pragma unused(name,deviceType,outlet,sharedStorage,privateStorage,thisDevice,argc,argv)
    *result = noErr;
    return kPhidgSuccess;
#endif /* not USE_DEFAULT */
} /* doPut */

/*------------------------------------ identify ---*/
OSErr
identify(STANDARD_PHID_ARGS_IDENTIFY)
{
#if defined(COMPILE_FOR_OSX_4)
 #pragma unused(name,privateSize,sharedSize,isAsynchronous)
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
 #pragma unused(name,privateSize,sharedSize)
#endif /* COMPILE_FOR_OS9_4 */
    *productID = 0x03B;
  return noErr;
} /* identify */

/*------------------------------------ main ---*/
OSErr
main(STANDARD_PHID_ARGS_MAIN)
{
#if defined(COMPILE_FOR_OSX_4)
 #pragma unused(name,sharedStorage)
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
 #pragma unused(name,sharedStorage,environment)
#endif /* COMPILE_FOR_OS9_4 */
    STANDARD_MAIN_CODE;
  return noErr;
} /* main */

/*------------------------------------ niam ---*/
OSErr
niam(STANDARD_PHID_ARGS_NIAM)
{
#pragma unused(name,sharedStorage)
  return noErr;
} /* niam */

/*------------------------------------ onAttach ---*/
E_PhidgResult
onAttach(STANDARD_PHID_ARGS_ATTACH)
{
#if defined(USE_DEFAULT)
 #pragma unused(name,deviceType,sharedStorage,privateStorage,thisDevice)
    *result = noErr;
    return kPhidgDoDefault;
#else /* not USE_DEFAULT */
 #pragma unused(name,deviceType,sharedStorage,privateStorage,thisDevice)
    *result = noErr;
    return kPhidgSuccess;
#endif /* not USE_DEFAULT */
} /* onAttach */

/*------------------------------------ onDetach ---*/
E_PhidgResult
onDetach(STANDARD_PHID_ARGS_ATTACH)
{
#if defined(USE_DEFAULT)
 #pragma unused(name,deviceType,sharedStorage,privateStorage,thisDevice)
    *result = noErr;
    return kPhidgDoDefault;
#else /* not USE_DEFAULT */
 #pragma unused(name,deviceType,sharedStorage,privateStorage,thisDevice)
    *result = noErr;
    return kPhidgSuccess;
#endif /* not USE_DEFAULT */
} /* onDetach */

#if defined(COMPILE_FOR_OS9_4)
/*------------------------------------ reportHandler ---*/
void
reportHandler(STANDARD_PHID_ARGS_REPORTHANDLER)
{
 #pragma unused(name,deviceType,sharedStorage,privateStorage,thisDevice,outlet,inHIDReport,inHIDReportLength)
} /* reportHandler */
#endif /* COMPILE_FOR_OS9_4 */
