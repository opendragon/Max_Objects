/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       gvp100_SetEffectsControl.cpp                                            */
/*                                                                                      */
/*  Contains:   The routine cmd_SetEffectsAnalogControl().                              */
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
/*  Created:    1998/07/07                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "gvp100.h"
#include "fillBufferWithDouble.h"

/*------------------------------------ cmd_SetEffectsAnalogControl ---*/
SETEFFECTSANALOGCONTROL_HEADER(GvpData)
{
#pragma unused(ss)
    if (xx)
    {
        bool               okSoFar = true;
        t_atom_long        number;
        t_atom_long        temp;
        t_atom_long        value;
        unsigned char      dummy[3];
        int                ii = 0;
        static t_atom_long validControl[] =
        {
            0x0000, 0x000A, 0x000B, 0x0011, 0x0012, 0x0014, 0x0015, 0x0017,
            0x0018, 0x0019, 0x001A, 0x001B, 0x001C, 0x001D, 0x001E, 0x001F
        };
        static const int   kNumValidEffectsControls = (sizeof(validControl) /
                                                       sizeof(*validControl));
        static int         shiftAmount[] =
        {
            4,      4,      4,      4,      4,      8,      4,      8,
            8,      8,      8,      8,      4,      8,      4,      8
        };

        for (long jj = 0; okSoFar && (jj  < argc); ++jj)
        {
            switch (argv[jj].a_type)
            {
                case A_FLOAT:
                    if (jj & 1)
                    {
                        /* odd index - do it! */
                        value = TO_INT(argv[jj].a_w.w_float);
                        temp = (value << shiftAmount[ii]); // from previous element
                        dummy[0] = static_cast<unsigned char>(number);
                        dummy[1] = static_cast<unsigned char>(temp & 0x00ff);
                        dummy[2] = static_cast<unsigned char>((temp >> 8) & 0x00ff);
                        gvpPerformWriteCommand(xx, 1, kCommandWriteAnalogControl, 3, dummy,
                                               kStateAwaitingByteCount1, jj == (argc - 1));
                    }
                    else
                    {
                        /* even index - validate and remember it! */
                        number = TO_INT(argv[jj].a_w.w_float);
                        value = 0;
                        for (ii = 0; kNumValidEffectsControls > ii; ++ii)
                        {
                            if (number == validControl[ii])
                            {
                                break;
                            }
                            
                        }
                        if (kNumValidEffectsControls == ii)
                        {
                            LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid analog control (%g)",
                                        static_cast<double>(argv[jj].a_w.w_float))
                            okSoFar = false;
                        }
                    }
                    break;

                case A_LONG:
                    if (jj & 1)
                    {
                        /* odd index - do it! */
                        value = argv[jj].a_w.w_long;
                        temp = (value << shiftAmount[ii]); // from previous element
                        dummy[0] = static_cast<unsigned char>(number);
                        dummy[1] = static_cast<unsigned char>(temp & 0x00ff);
                        dummy[2] = static_cast<unsigned char>((temp >> 8) & 0x00ff);
                        gvpPerformWriteCommand(xx, 1, kCommandWriteAnalogControl, 3, dummy,
                                               kStateAwaitingByteCount1, jj == (argc - 1));
                    }
                    else
                    {
                        /* even index - validate and remember it! */
                        number = argv[jj].a_w.w_long;
                        value = 0;
                        for (ii = 0; kNumValidEffectsControls > ii; ++ii)
                        {
                            if (number == validControl[ii])
                            {
                                break;
                            }
                            
                        }
                        if (kNumValidEffectsControls == ii)
                        {
                            LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid analog control (" LONG_FORMAT
                                        ")", number)
                            okSoFar = false;
                        }
                    }
                    break;

                case A_SYM:
                    LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid analog control '%s'",
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
        if (okSoFar)
        {
            if (argc & 1)
            {
                /* the last number was a solitaire */
                temp = (value << shiftAmount[ii]); // from previous element
                dummy[0] = static_cast<unsigned char>(number);
                dummy[1] = static_cast<unsigned char>(temp & 0x00ff);
                dummy[2] = static_cast<unsigned char>((temp >> 8) & 0x00ff);
                gvpPerformWriteCommand(xx, 1, kCommandWriteAnalogControl, 3, dummy,
                                       kStateAwaitingByteCount1, true);
            }
        }
        else
        {
            outlet_bang(xx->fErrorBangOut);
        }
    }
} // cmd_SetEffectsAnalogControl
