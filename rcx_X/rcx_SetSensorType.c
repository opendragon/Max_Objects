/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       rcx_SetSensorType.c                                                     */
/*                                                                                      */
/*  Contains:   The routine cmd_SetSensorType().                                        */
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

/*------------------------------------ cmd_SetSensorType ---*/
Pvoid
cmd_SetSensorType(RcxControlPtr xx,
                  long          slot,
                  PSymbol       newType)
{
#if (! FOR_MAC_PPC)
 #pragma unused(slot, newType)
#endif /* not FOR_MAC_PPC */
  EnterCallback();
  if (xx)
  {
#if FOR_MAC_PPC
    if (rcxSynchronize(xx))
    {
      if ((slot > 0) && (slot <= RCX_NUM_SENSORS))
      {
        uchar typeSetCommand[] = { RCX_SET_SENSOR_TYPE_CMD, 0, 0 };
        long  aType = -1;

        if (newType == gNoSensorSymbol)
          aType = RCX_SENSOR_NONE;
        else if (newType == gSwitchSymbol)
          aType = RCX_SENSOR_SWITCH;
        else if (newType == gTemperatureSymbol)
          aType = RCX_SENSOR_TEMPERATURE;
        else if (newType == gReflectionSymbol)
          aType = RCX_SENSOR_REFLECTION;
        else if (newType == gAngleSymbol)
          aType = RCX_SENSOR_ANGLE;
        else
        {
          LOG_ERROR_2(OUTPUT_PREFIX "bad sensor type (%s)", newType->s_name)
          outlet_bang(xx->fErrorBangOut);
        }
        if (aType != -1)
        {
          typeSetCommand[1] = static_cast<uchar>(slot - 1);
          typeSetCommand[2] = static_cast<uchar>(aType);
          if (rcxSendCommand(xx, typeSetCommand, sizeof(typeSetCommand),
                              RCX_SET_SENSOR_TYPE_REPLY, true))
            outlet_bang(xx->fCommandComplete);
          else
            outlet_bang(xx->fErrorBangOut);
        }
      }
      else
      {
        LOG_ERROR_2(OUTPUT_PREFIX "bad sensor index (%ld)", slot)
        outlet_bang(xx->fErrorBangOut);
      }
    }
    else
      outlet_bang(xx->fErrorBangOut);
#endif /* FOR_MAC_PPC */
  }
  ExitMaxMessageHandler()
} /* cmd_SetSensorType */
