/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       doPlugin_Plugin.c                                                       */
/*                                                                                      */
/*  Contains:   The routine cmd_Plugin().                                               */
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

/*------------------------------------ cmd_Plugin ---*/
Pvoid cmd_Plugin
  (DoPluginPtr xx,
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
      if (argc)
      {
        if (argv[0].a_type == A_SYM)
        {
          PSymbol which = argv[0].a_w.w_sym;

          if (which == gReloadSymbol)
          {
            refreshDescriptors(xx);
            LOG_POST_1(OUTPUT_PREFIX "plŸgin reloaded")
          }
          else if (which == gListSymbol)
          {
            LOG_POST_1(OUTPUT_PREFIX "List of plŸgins:")
            for (descriptor = gPluginAnchor; descriptor; descriptor = descriptor->fNext)
            {
              LOG_POST_2("  Name: %s", descriptor->fName->s_name)
              descriptor = descriptor->fNext;
            }
          }
          else if (which == gRescanSymbol)
          {
            rescanPlugins();
            LOG_POST_1(OUTPUT_PREFIX "plŸgins rescanned")
          }
          else
            LOG_ERROR_2(OUTPUT_PREFIX "unknown modifier '%s' for plŸgin command", which->s_name)
        }
        else
          LOG_ERROR_1(OUTPUT_PREFIX "unexpected modifier for plŸgin command")
      }
      else
        LOG_POST_4("Name: %s, inlets: %hd, outlets: %hd", descriptor->fName->s_name,
                    xx->fNumInlets, xx->fNumOutlets)
    }
#endif /* FOR_MAC_PPC */
  }
  ExitMaxMessageHandler()
} /* cmd_Plugin */
