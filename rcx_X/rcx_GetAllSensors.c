/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       rcx_GetAllSensors.c                                                     */
/*                                                                                      */
/*  Contains:   The routine cmd_GetAllSensors().                                        */
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

/*------------------------------------ cmd_GetAllSensors ---*/
Pvoid
cmd_GetAllSensors(RcxControlPtr xx)
{
  EnterCallback();
  if (xx)
  {
#if FOR_MAC_PPC
    if (rcxSynchronize(xx))
    {
      bool  okSoFar = true;
      Atom  response[RCX_NUM_SENSORS * 3];

      for (short ii = 0, jj = 0; okSoFar && (ii < RCX_NUM_SENSORS); ++ii, jj += 3)
      {
        long value;

        if (rcxGetValue(xx, MAKE_RCX_VALUE(RCX_SENSOR_VALUE_TYPE, ii), value))
          SETLONG(response + jj, value);
        else
          okSoFar = false;
        if (okSoFar)
        {
          if (rcxGetValue(xx, MAKE_RCX_VALUE(RCX_SENSOR_TYPE_TYPE, ii), value))
          {
            switch (value)
            {
              case RCX_SENSOR_NONE:
                SETSYM(response + jj + 1, gNoSensorSymbol);
                break;

              case RCX_SENSOR_SWITCH:
                SETSYM(response + jj + 1, gSwitchSymbol);
                break;

              case RCX_SENSOR_TEMPERATURE:
                SETSYM(response + jj + 1, gTemperatureSymbol);
                break;

              case RCX_SENSOR_REFLECTION:
                SETSYM(response + jj + 1, gReflectionSymbol);
                break;

              case RCX_SENSOR_ANGLE:
                SETSYM(response + jj + 1, gAngleSymbol);
                break;

              default:
                SETSYM(response + jj + 1, gUnknownSymbol);
                break;

            }
          }
          else
            okSoFar = false;
        }
        if (okSoFar)
        {
          if (rcxGetValue(xx, MAKE_RCX_VALUE(RCX_SENSOR_MODE_TYPE, ii), value))
          {
            switch (value & RCX_MODE_MASK)
            {
              case RCX_SENSOR_RAW:
                SETSYM(response + jj + 2, gRawSymbol);
                break;

              case RCX_SENSOR_BOOLEAN:
                SETSYM(response + jj + 2, gBooleanSymbol);
                break;

              case RCX_SENSOR_TRANSITION_COUNT:
                SETSYM(response + jj + 2, gTransitionSymbol);
                break;

              case RCX_SENSOR_PERIOD_COUNTER:
                SETSYM(response + jj + 2, gPeriodCountSymbol);
                break;

              case RCX_SENSOR_PERCENT_FULL_SCALE:
                SETSYM(response + jj + 2, gPercentSymbol);
                break;

              case RCX_SENSOR_CELSIUS:
                SETSYM(response + jj + 2, gCelsiusSymbol);
                break;

              case RCX_SENSOR_FAHRENHEIT:
                SETSYM(response + jj + 2, gFahrenheitSymbol);
                break;

              case RCX_SENSOR_ANGLE_STEP:
                SETSYM(response + jj + 2, gAngleStepSymbol);
                break;

              default:
                SETSYM(response + jj + 2, gUnknownSymbol);
                break;

            }
          }
          else
            okSoFar = false;
        }
      }
      if (okSoFar)
      {
        outlet_bang(xx->fCommandComplete);
        outlet_anything(xx->fDataOut, gAllSensorsSymbol, 9, response);
      }
      else
        outlet_bang(xx->fErrorBangOut);
    }
    else
      outlet_bang(xx->fErrorBangOut);
#endif /* FOR_MAC_PPC */
  }
  ExitMaxMessageHandler()
} /* cmd_GetAllSensors */
