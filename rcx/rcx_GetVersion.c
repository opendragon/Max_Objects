/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       rcx_GetVersion.c                                                        */
/*                                                                                      */
/*  Contains:   The routine cmd_GetVersion().                                           */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2002 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2002/03/31                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "rcx.h"

/*------------------------------------ cmd_GetVersion ---*/
Pvoid cmd_GetVersion
  (RcxControlPtr xx)
{
  EnterCallback();
  if (xx)
  {
#if __powerc
    if (rcxSynchronize(xx))
    {
      static uchar unlockCommand[] = { RCX_UNLOCK_CMD, 1, 3, 5, 7, 11 };

      if (rcxSendCommand(xx, unlockCommand, sizeof(unlockCommand), RCX_UNLOCK_REPLY,
                          true))
      {
        uchar versionBuffer[8];
        Atom  response[2];
        long  romVersion, firmwareVersion;

        rcxCopyFromReply(xx, versionBuffer, sizeof(versionBuffer));
        outlet_bang(xx->fCommandComplete);
        romVersion = long((versionBuffer[0] << 12) | (versionBuffer[1] << 8) |
                          (versionBuffer[2] << 4) | versionBuffer[3]);
        firmwareVersion = long((versionBuffer[4] << 12) | (versionBuffer[5] << 8) |
                              (versionBuffer[6] << 4) | versionBuffer[7]);
        SETLONG(response, romVersion);
        SETLONG(response + 1, firmwareVersion);
        outlet_anything(xx->fDataOut, gVersionSymbol, 2, response);
      }
      else
        outlet_bang(xx->fErrorBangOut);
    }
    else
      outlet_bang(xx->fErrorBangOut);
#endif /* __powerc */
  }
  ExitMaxMessageHandler()
} /* cmd_GetVersion */