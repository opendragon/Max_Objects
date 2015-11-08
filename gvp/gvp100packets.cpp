/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       gvp100packets.cpp                                                       */
/*                                                                                      */
/*  Contains:   Packet support routines for the gvp100 module.                          */
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
/*  Created:    1998/07/04                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "gvp100.h"

/*------------------------------------ gvpCheckPoolSpace ---*/
bool gvpCheckPoolSpace(GvpData *   xx,
                       const short numCommands)
{
    bool result;

    if (xx && (xx->fPoolSize >= numCommands))
    {
        result = true;
    }
    else
    {
        result = false;
    }
    return result;
} // gvpCheckPoolSpace
/*------------------------------------ gvpClearPackets ---*/
void gvpClearPackets(GvpData * xx)
{
    if (xx)
    {
        GvpPacket * first = gvpGetFirstPacket(xx);

        for ( ; first; first = gvpGetFirstPacket(xx))
        {
            gvpReleasePacket(xx, first);
        }
    }
} // gvpClearPackets
/*------------------------------------ gvpGetFirstPacket ---*/
GvpPacket * gvpGetFirstPacket(GvpData * xx)
{
    if (xx)
    {
        GvpPacket * aPacket = xx->fFirst;

        if (aPacket)
        {
            xx->fFirst = aPacket->fNext;
            aPacket->fNext = aPacket->fPrev = NULL_PTR;
            if (xx->fFirst)
            {
                xx->fFirst->fPrev = NULL_PTR;
            }
            else
            {
                xx->fLast = NULL_PTR;
            }
        }
        return aPacket;
    }
    return NULL_PTR;
} // gvpGetFirstPacket
/*------------------------------------ gvpNewPacket ---*/
GvpPacket * gvpNewPacket(GvpData * xx)
{
    GvpPacket * newPacket = xx->fPool;

    xx->fPool = newPacket->fNext;
    newPacket->fPrev = newPacket->fNext = NULL_PTR;
    xx->fSendCompletion = false;
    --xx->fPoolAvailable;
    return newPacket;
} // gvpNewPacket
/*------------------------------------ gvpReleasePacket ---*/
void gvpReleasePacket(GvpData *   xx,
                      GvpPacket * pp)
{
    if (xx && pp)
    {
        pp->fPrev = NULL_PTR;
        pp->fNext = xx->fPool;
        xx->fPool = pp;
        ++xx->fPoolAvailable;
    }
} // gvpReleasePacket
