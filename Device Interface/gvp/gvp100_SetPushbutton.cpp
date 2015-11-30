/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       gvp100_SetPushbutton.cpp                                                */
/*                                                                                      */
/*  Contains:   The routine cmd_SetPushbutton().                                        */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 1998 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    1998/07/11                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "gvp100.h"

/*------------------------------------ cmd_SetPushbutton ---*/
SETPUSHBUTTON_HEADER(GvpData)
{
    if (xx)
    {
        GvpCommandCode   aCommand = kNoCommand;
        bool             okSoFar = true;
        t_atom_long      number;
        unsigned char    dummy;
#if (! defined(DONT_RANGE_CHECK))
        static long      validControlOnOff[] =
        {
            0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007,
            0x0008, 0x0009,         0x000B, 0x000C, 0x000D, 0x000E, 0x000F,
            0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017,
            0x0018, 0x0019,         0x001B, 0x001C, 0x001D, 0x001E, 0x001F,
            0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027,
            0x0028, 0x0029,         0x002B, 0x002C, 0x002D, 0x002E, 0x002F,
            0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037,
            0x0038, 0x0039,
            0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047,
            0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E,
            0x0052
        };
        static const int kNumValidOnOffControls = (sizeof(validControlOnOff) /
                                                   sizeof(*validControlOnOff));
        static long      validControlPush[] =
        {
            0x0000, 0x0001, 0x0002, 0x0003, 0x0004, 0x0005, 0x0006, 0x0007,
            0x0008, 0x0009,         0x000B, 0x000C, 0x000D, 0x000E, 0x000F,
            0x0010, 0x0011, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0017,
            0x0018, 0x0019,         0x001B, 0x001C, 0x001D, 0x001E, 0x001F,
            0x0020, 0x0021, 0x0022, 0x0023, 0x0024, 0x0025, 0x0026, 0x0027,
            0x0028, 0x0029,         0x002B, 0x002C, 0x002D, 0x002E, 0x002F,
            0x0030, 0x0031, 0x0032, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037,
            0x0038, 0x0039,         0x003B, 0x003C, 0x003D, 0x003E, 0x003F,
            0x0040, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047,
            0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F
        };
        static const int kNumValidPushControls = (sizeof(validControlPush) /
                                                  sizeof(*validControlPush));
#endif /* not DONT_RANGE_CHECK */

        if (onOffPush == gOffSymbol)
        {
            aCommand = kCommandSetPushbuttonLampOff;
        }
        else if (onOffPush == gOnSymbol)
        {
            aCommand = kCommandSetPushbuttonLampOn;
        }
        else if ((onOffPush == gPushSymbol) || (onOffPush == gPSymbol))
        {
            aCommand = kCommandPushButton;
        }
        else
        {
            LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid request '%s'", onOffPush->s_name)
            okSoFar = false;
        }
        for (long jj = 0; okSoFar && (jj  < argc); ++jj)
        {
            switch (argv[jj].a_type)
            {
                case A_FLOAT:
                    number = TO_INT(argv[jj].a_w.w_float);
#if (! defined(DONT_RANGE_CHECK))
                    if ((onOffPush == gPushSymbol) || (onOffPush == gPSymbol))
                    {
                        int ii = 0;

                        for ( ; kNumValidPushControls > ii; ++ii)
                        {
                            if (number == validControlPush[ii])
                            {
                                break;
                            }
                            
                        }
                        if (kNumValidPushControls == ii)
                        {
                            LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid pushbutton or lamp (%g)",
                                        static_cast<double>(argv[jj].a_w.w_float))
                            okSoFar = false;
                        }
                    }
                    else if ((onOffPush == gOffSymbol) || (onOffPush == gOnSymbol))
                    {
                        int ii = 0;

                        for ( ; kNumValidOnOffControls > ii; ++ii)
                        {
                            if (number == validControlOnOff[ii])
                            {
                                break;
                            }
                            
                        }
                        if (kNumValidOnOffControls == ii)
                        {
                            LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid pushbutton or lamp (%g)",
                                        static_cast<double>(argv[jj].a_w.w_float))
                            okSoFar = false;
                        }
                    }
#endif /* not DONT_RANGE_CHECK */
                    if (okSoFar)
                    {
                        dummy = static_cast<unsigned char>(number);
                        gvpPerformWriteCommand(xx, 0, aCommand, 1, &dummy, kStateAwaitingByteCount1,
                                               jj == (argc - 1));
                    }
                    break;

                case A_LONG:
                    number = argv[jj].a_w.w_long;
#if (! defined(DONT_RANGE_CHECK))
                    if ((onOffPush == gPushSymbol) || (onOffPush == gPSymbol))
                    {
                        int ii = 0;

                        for ( ; kNumValidPushControls > ii; ++ii)
                        {
                            if (number == validControlPush[ii])
                            {
                                break;
                            }
                            
                        }
                        if (kNumValidPushControls == ii)
                        {
                            LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid pushbutton or lamp (" LONG_FORMAT
                                        ")", number)
                            okSoFar = false;
                        }
                    }
                    else if ((onOffPush == gOffSymbol) || (onOffPush == gOnSymbol))
                    {
                        int ii = 0;

                        for ( ; kNumValidOnOffControls > ii; ++ii)
                        {
                            if (number == validControlOnOff[ii])
                            {
                                break;
                            }
                            
                        }
                        if (kNumValidOnOffControls == ii)
                        {
                            LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid pushbutton or lamp (" LONG_FORMAT
                                        ")", number)
                            okSoFar = false;
                        }
                    }
#endif /* not DONT_RANGE_CHECK */
                    if (okSoFar)
                    {
                        dummy = static_cast<unsigned char>(number);
                        gvpPerformWriteCommand(xx, 0, aCommand, 1, &dummy, kStateAwaitingByteCount1,
                                               jj == (argc - 1));
                    }
                    break;

                case A_SYM:
                    LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid pushbutton or lamp '%s'",
                                argv[jj].a_w.w_sym->s_name)
                    okSoFar = false;
                    break;

                default:
                    LOG_ERROR_2(xx, OUTPUT_PREFIX "input of an unknown type (%d) seen",
                                static_cast<int>(argv[jj].a_type))
                    okSoFar = false;
                    break;
                    
            }
        }
        if (! okSoFar)
        {
            outlet_bang(xx->fErrorBangOut);
        }
    }
} // cmd_SetPushbutton
