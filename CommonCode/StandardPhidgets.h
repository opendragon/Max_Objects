/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       StandardPhidgets.h                                                      */
/*                                                                                      */
/*  Contains:   The data type declarations that are needed for standard phidgets.       */
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
/*  Created:    2003/12/19                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(STANDARDPHIDGETS_H_))
 #define STANDARDPHIDGETS_H_ /* */

 #include "MissingAndExtra.h"
 #include "Common_HIDX.h"
 #include "PhidgetEntryPoints.h"

 #if defined(__cplusplus)
extern "C"
{
 #endif /* __cplusplus */

 #if defined(COMPILE_FOR_CATS)
E_PhidgResult defineCallback
	(STANDARD_PHID_ARGS_DEFINECALLBACK);
 #endif /* COMPILE_FOR_CATS */
 
E_PhidgResult doCustom
  (STANDARD_PHID_ARGS_DO);

E_PhidgResult doGet
  (STANDARD_PHID_ARGS_DO);

E_PhidgResult doPut
  (STANDARD_PHID_ARGS_DO);

OSErr identify
	(STANDARD_PHID_ARGS_IDENTIFY);
	
OSErr main
  (STANDARD_PHID_ARGS_MAIN);

OSErr niam
  (STANDARD_PHID_ARGS_NIAM);

E_PhidgResult onAttach
  (STANDARD_PHID_ARGS_ATTACH);

E_PhidgResult onDetach
  (STANDARD_PHID_ARGS_ATTACH);

 #if (! defined(COMPILE_FOR_CATS))
 void reportHandler
	(STANDARD_PHID_ARGS_REPORTHANDLER);
 #endif /* not COMPILE_FOR_CATS */
 
 #if defined(COMPILE_FOR_CATS)
  #pragma export list defineCallback
 #else /* not COMPILE_FOR_CATS */
  #pragma export list reportHandler
 #endif /* not COMPILE_FOR_CATS */
 
 #pragma export list doCustom,doGet,doPut,identify,main,niam,onAttach,onDetach

 #if defined(COMPILE_FOR_CATS)
  #define STANDARD_MAIN_CODE	\
gSyslogFunction = environment->fSyslogFunction
 #else /* not COMPILE_FOR_CATS */
  #define STANDARD_MAIN_CODE	/* */
 #endif /* not COMPILE_FOR_CATS */

 #if defined(__cplusplus)
}
 #endif /* __cplusplus */

#endif /* not STANDARDPHIDGETS_H_ */
