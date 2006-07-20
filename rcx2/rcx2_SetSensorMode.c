/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       rcx2_SetSensorMode.c                                                    */
/*                                                                                      */
/*  Contains:   The routine cmd_SetSensorMode().                                        */
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
/*  Created:    2005/11/19                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "rcx2.h"

/*------------------------------------ cmd_SetSensorMode ---*/
Pvoid cmd_SetSensorMode
  (Rcx2ControlPtr xx,
   long          slot,
   PSymbol       newMode)
{
  EnterCallback();
  if (xx)
  {
    if (rcx2Synchronize(xx))
    {
      if ((slot > 0) && (slot <= RCX_NUM_SENSORS))
      {
        uchar modeSetCommand[] = { RCX_SET_SENSOR_MODE_CMD, 0, 0 };
        long  aMode = -1;

        if (newMode == gRawSymbol)
          aMode = RCX_SENSOR_RAW;
        else if (newMode == gBooleanSymbol)
          aMode = RCX_SENSOR_BOOLEAN;
        else if (newMode == gTransitionSymbol)
          aMode = RCX_SENSOR_TRANSITION_COUNT;
        else if (newMode == gPeriodCountSymbol)
          aMode = RCX_SENSOR_PERIOD_COUNTER;
        else if (newMode == gPercentSymbol)
          aMode = RCX_SENSOR_PERCENT_FULL_SCALE;
        else if (newMode == gCelsiusSymbol)
          aMode = RCX_SENSOR_CELSIUS;
        else if (newMode == gFahrenheitSymbol)
          aMode = RCX_SENSOR_FAHRENHEIT;
        else if (newMode == gAngleStepSymbol)
          aMode = RCX_SENSOR_ANGLE_STEP;
        else
        {
          LOG_ERROR_2(OUTPUT_PREFIX "bad sensor mode (%s)", newMode->s_name)
          outlet_bang(xx->fErrorBangOut);
        }
        if (aMode != -1)
        {
          modeSetCommand[1] = static_cast<uchar>(slot - 1);
          modeSetCommand[2] = static_cast<uchar>(aMode);
          if (rcx2SendCommand(xx, modeSetCommand, sizeof(modeSetCommand),
                              RCX_SET_SENSOR_MODE_REPLY, true))
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
  }
  ExitMaxMessageHandler()
} /* cmd_SetSensorMode */
