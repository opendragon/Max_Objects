/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       spaceballlowlevel.cpp                                                   */
/*                                                                                      */
/*  Contains:   The low-level support routines for the spaceball module.                */
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
/*  Created:    2001/07/13                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "spaceball.h"
#include <math.h>

#define MERGE_BYTES(aa, bb) (static_cast<unsigned>((xx->fBuffer[aa] << 8) | \
                                                   xx->fBuffer[bb]))
#define SIGN_EXTEND(val)    ((int(val) << 16) >> 16)

/*------------------------------------ spaceballProcessPacket ---*/
void
spaceballProcessPacket(SpaceballData * xx)
{
    if (xx)
    {
        unsigned char firstChar = xx->fBuffer[0];
        unsigned char secondChar;

        switch (firstChar)
        {
            case kSpaceballResponseDispPacket:
                if (15 == xx->fBufferPos)
                {
//                    unsigned dt = MERGE_BYTES(1, 2);
                    unsigned tx = MERGE_BYTES(3, 4);
                    unsigned ty = MERGE_BYTES(5, 6);
                    unsigned tz = MERGE_BYTES(7, 8);
                    unsigned rx = MERGE_BYTES(9, 10);
                    unsigned ry = MERGE_BYTES(11, 12);
                    unsigned rz = MERGE_BYTES(13, 14);
                    t_atom   report[3];

                    if (xx->fModeDelta)
                    {
                        xx->fTrans[0] = SIGN_EXTEND(tx);
                        xx->fTrans[1] = SIGN_EXTEND(ty);
                        xx->fTrans[2] = SIGN_EXTEND(tz);
                        xx->fRot[0] = SIGN_EXTEND(rx);
                        xx->fRot[1] = SIGN_EXTEND(ry);
                        xx->fRot[2] = SIGN_EXTEND(rz);
                    }
                    else
                    {
                        xx->fTrans[0] += SIGN_EXTEND(tx);
                        xx->fTrans[1] += SIGN_EXTEND(ty);
                        xx->fTrans[2] += SIGN_EXTEND(tz);
                        xx->fRot[0] += SIGN_EXTEND(rx);
                        xx->fRot[1] += SIGN_EXTEND(ry);
                        xx->fRot[2] += SIGN_EXTEND(rz);
                    }
                    for (short ii = 0; 3 > ii; ++ii)
                    {
                        atom_setfloat(report + ii, xx->fTrans[ii]);
                    }
                    outlet_anything(xx->fDataOut, gTranslateSymbol, 3, report);
                    for (short ii = 0; 3 > ii; ++ii)
                    {
                        atom_setfloat(report + ii, xx->fRot[ii]);
                    }
                    outlet_anything(xx->fDataOut, gRotateSymbol, 3, report);
                }
                else
                {
                    LOG_ERROR_1(xx, OUTPUT_PREFIX "bad response packet seen")
                }
                break;

            case kSpaceballResponseKeyPacket:
                if (3 == xx->fBufferPos)
                {
                    long newButtons = (((xx->fBuffer[1] & 0x1f) << 4) | (xx->fBuffer[2] & 0x0f));
                    long buttonChanges = (xx->fButtons ^ newButtons);
                    long mask = 1;

                    for (long ii = 0; buttonChanges; ++ii)
                    {
                        if (buttonChanges & mask)
                        {
                            t_atom report[2];

                            atom_setlong(report, ii + 1);
                            if (newButtons & mask)
                            {
                                atom_setlong(report + 1, 1);
                            }
                            else
                            {
                                atom_setlong(report + 1, 0);
                            }
                            outlet_anything(xx->fDataOut, gButtonSymbol, 2, report);
                            buttonChanges &= ~ mask;
                        }
                        mask <<= 1;
                    }
                    xx->fButtons = newButtons;
                }
                else
                {
                    LOG_ERROR_1(xx, OUTPUT_PREFIX "bad key packet seen")
                }
                break;

            case kSpaceballResponseInfoPacket:
                secondChar = xx->fBuffer[1];
                xx->fBuffer[xx->fBufferPos] = 0;
                if (kSpaceballResponseInfo1 == secondChar)
                {
                    /* Spaceball status */
                    LOG_POST_2(xx, OUTPUT_PREFIX "%s", xx->fBuffer + 2)
                }
                else if (kSpaceballResponseInfo2 == secondChar)
                {
                    /* Spaceball firmware description */
                    LOG_POST_2(xx, OUTPUT_PREFIX "%s", xx->fBuffer + 2)
                }
                else
                {
                    LOG_ERROR_2(xx, OUTPUT_PREFIX "bad info/reset packet seen (0x%x)", secondChar)
                }
                break;

            default:
                break;
                
        }
    }
} // spaceballProcessPacket

/*------------------------------------ spaceballZeroValues ---*/
void
spaceballZeroValues(SpaceballData * xx)
{
    if (xx)
    {
        memset(xx->fTrans, 0, 3 * sizeof(xx->fTrans[0]));
        memset(xx->fRot, 0, 3 * sizeof(xx->fRot[0]));
    }
} // spaceballZeroValues
