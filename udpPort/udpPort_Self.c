/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       udpPort_Self.c                                                          */
/*                                                                                      */
/*  Contains:   The routine cmd_Self().                                                 */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2005 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2005/07/19                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "udpPort.h"

/*------------------------------------ cmd_Self ---*/
Pvoid cmd_Self
  (UdpObjectPtr xx)
{
  EnterCallback();
  REPORT_MAX_MESSAGE("self");
  if (xx)
  {
#if OPEN_TRANSPORT_SUPPORTED
    Atom response[1];

    if (! xx->fSelfName)
    {
      OSStatus          result;
      InetInterfaceInfo info;

      WRAP_OT_CALL(xx, result, "OTInetGetInterfaceInfo", OTInetGetInterfaceInfo(&info, 0))
      if (result == kOTNoError)
      {
        char temp_name[32];

        OTInetHostToString(info.fAddress, temp_name);
        xx->fSelfName = gensym(temp_name);
      }
      else
        REPORT_ERROR(OUTPUT_PREFIX "OTInetGetInterfaceInfo failed (%ld = %s)", result)
    }
    if (xx->fSelfName)
    {
      SETSYM(response, xx->fSelfName);
      outlet_anything(xx->fResultOut, gSelfSymbol, 1, response);  
    }
#endif /* OPEN_TRANSPORT_SUPPORTED */
  }
  ExitMaxMessageHandler()
} /* cmd_Self */
