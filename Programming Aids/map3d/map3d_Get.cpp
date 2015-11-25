/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       map3d_Get.cpp                                                           */
/*                                                                                      */
/*  Contains:   The routine cmd_Get().                                                  */
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
/*  Created:    2001/05/08                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "map3d.h"

/*------------------------------------ cmd_Get ---*/
GET_HEADER(Map3dData)
{
    if (xx && (0 < num) && (num <= xx->fRangeCount))
    {
        RangeData * walker = xx->fFirstRange;

        for (long ii = 1; walker && (ii < num); ++ii)
        {
            walker = walker->fNext;
        }
        if (walker)
        {
            RangeElement * data = NULL;
            bool           infPositive = true;
            bool           leftRight = false;
            bool           bottomTop = false;
            bool           forwardBack = false;

            if (which == gLeftSymbol)
            {
                infPositive = false;
                data = &walker->fLeft;
                leftRight = true;
            }
            else if (which == gRightSymbol)
            {
                data = &walker->fRight;
                leftRight = true;
            }
            else if (which == gBottomSymbol)
            {
                infPositive = false;
                data = &walker->fBottom;
                bottomTop = true;
            }
            else if (which == gTopSymbol)
            {
                data = &walker->fTop;
                bottomTop = true;
            }
            else if (which == gBackSymbol)
            {
                data = &walker->fBack;
                forwardBack = true;
            }
            else if (which == gForwardSymbol)
            {
                infPositive = false;
                data = &walker->fForward;
                forwardBack = true;
            }
            else
            {
                LOG_ERROR_2(xx, OUTPUT_PREFIX "unknown point (%s)", which->s_name)
            }
            if (data)
            {
                t_atom result[1];

                if ((leftRight && walker->fLeftRightDontCare) ||
                    (bottomTop && walker->fBottomTopDontCare) ||
                    (forwardBack && walker->fForwardBackDontCare))
                {
                    A_SETSYM(result, gAsteriskSymbol);
                }
                else if (MatchInfinity == data->fKind)
                {
                    A_SETSYM(result, infPositive ? gPosInfSymbol1 : gNegInfSymbol1);
                }
                else if (MatchFloat == data->fKind)
                {
                    A_SETFLOAT(result, getFOIFloat(data->fValue));
                }
                else
                {
                    A_SETLONG(result, getFOILong(data->fValue));
                }
                outlet_anything(xx->fResultOut, gValueSymbol, 1, result);
            }
        }
    }
} // cmd_Get
