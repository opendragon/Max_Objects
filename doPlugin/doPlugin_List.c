/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       doPlugin_List.c                                                         */
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
/*  Created:    2001/08/19                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "doPlugin.h"

/*------------------------------------ cmd_List ---*/
Pvoid
cmd_List(DoPluginPtr xx,
         PSymbol     message,
         short       argc,
         PAtom       argv)
{
#if FOR_MAC_PPC
 #pragma unused(message)
#else /* not FOR_MAC_PPC */
 #pragma unused(message, argc, argv)
#endif /* not FOR_MAC_PPC */
  EnterCallback();
  if (xx)
  {
#if FOR_MAC_PPC
    PluginDescriptorPtr descriptor = xx->fActivePlugin;

    if (descriptor)
    {
 #if defined(COMPILE_FOR_OSX_4)
      OSErr myErr = reinterpret_cast<FpDoList>(descriptor->fDoListFun)
                          (PLUGIN_FIXED_ARGUMENTS, argc, argv);
 #endif /* COMPILE_FOR_OSX_4 */
 #if defined(COMPILE_FOR_OS9_4)
      OSErr myErr = static_cast<OSErr>(CallUniversalProc(descriptor->fDoListUpp,
                                            uppDoListProcInfo, PLUGIN_FIXED_ARGUMENTS,
                                            argc, argv));
 #endif /* COMPILE_FOR_OS9_4 */

      if (myErr != noErr)
      {
      }
    }
#endif /* FOR_MAC_PPC */
  }
  ExitMaxMessageHandler()
} /* cmd_List */
