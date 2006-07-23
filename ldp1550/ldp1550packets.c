/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       ldp1550packets.c                                                        */
/*                                                                                      */
/*  Contains:   The packet support routines for the gvp100 module.                      */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 1996 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    1996/09/21                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "ldp1550.h"

/*------------------------------------ ldpAddCommand ---*/
void ldpAddCommand
  (LdpControlPtr  			xx,
   const LdpCommandCode cc,
   const LdpState       rr)
{
  if (xx)
  {
    LdpPacketPtr newPacket = ldpNewPacket(xx);

    newPacket->fPrev = xx->fLast;
    newPacket->fCommand = cc;
    newPacket->fState = rr;
    if (xx->fLast)
      xx->fLast->fNext = newPacket;
    else
      xx->fFirst = newPacket;
    xx->fLast = newPacket;
  }
} /* ldpAddCommand */

/*------------------------------------ ldpCheckPoolSpace ---*/
bool ldpCheckPoolSpace
  (LdpControlPtr xx,
   const short   numCommands)
{
  if (xx && (xx->fPoolSize >= numCommands))
    return true;

  return false;
} /* ldpCheckPoolSpace */

/*------------------------------------ ldpClearPackets ---*/
void ldpClearPackets
  (LdpControlPtr xx)
{
  if (xx)
  {
    for (LdpPacketPtr first = ldpGetFirstPacket(xx); first; first = ldpGetFirstPacket(xx))
      ldpReleasePacket(xx, first);
  }
} /* ldpClearPackets */

/*------------------------------------ ldpGetFirstPacket ---*/
LdpPacketPtr ldpGetFirstPacket
  (LdpControlPtr xx)
{
  if (xx)
  {
    LdpPacketPtr aPacket = xx->fFirst;

    if (aPacket)
    {
      xx->fFirst = aPacket->fNext;
      aPacket->fNext = aPacket->fPrev = NULL_PTR;
      if (xx->fFirst)
        xx->fFirst->fPrev = NULL_PTR;
      else
        xx->fLast = NULL_PTR;
    }
    return aPacket;

  }
  return NULL_PTR;
} /* ldpGetFirstPacket */

/*------------------------------------ ldpInitCommands ---*/
void ldpInitCommands
  (LdpControlPtr xx)
{
  if (xx)
  {
    xx->fInterruptPoint = NULL_PTR;
    ldpClearPackets(xx);
  }
} /* ldpInitCommands */

/*------------------------------------ ldpInsertCommand ---*/
void ldpInsertCommand
  (LdpControlPtr  			xx,
   LdpPacketPtr   			before,
   const LdpCommandCode cc,
   const LdpState       rr)
{
  if (xx)
  {
    LdpPacketPtr newPacket = ldpNewPacket(xx);

    newPacket->fCommand = cc;
    newPacket->fState = rr;
    if (before)
    {
      if (before->fPrev)
        before->fPrev->fNext = newPacket;
      else
        xx->fFirst = newPacket;
      newPacket->fPrev = before->fPrev;
      before->fPrev = newPacket;
    }
    else
      xx->fFirst = xx->fLast = newPacket;
    newPacket->fNext = before;
  }
} /* ldpInsertCommand */

/*------------------------------------ ldpNewPacket ---*/
LdpPacketPtr ldpNewPacket
  (LdpControlPtr xx)
{
  LdpPacketPtr newPacket = xx->fPool;

  xx->fPool = newPacket->fNext;
  newPacket->fPrev = newPacket->fNext = NULL_PTR;
  --xx->fPoolAvailable;
  return newPacket;
} /* ldpNewPacket */

/*------------------------------------ ldpReleasePacket ---*/
void ldpReleasePacket
  (LdpControlPtr xx,
   LdpPacketPtr  pp)
{
  if (xx && pp)
  {
    pp->fPrev = NULL_PTR;
    pp->fNext = xx->fPool;
    xx->fPool = pp;
    ++xx->fPoolAvailable;
  }
} /* ldpReleasePacket */

/*------------------------------------ ldpSendCommand ---*/
void ldpSendCommand
  (LdpControlPtr xx)
{
  if (xx && xx->fFirst)
    outlet_int(xx->fCommandsOut, long(xx->fFirst->fCommand));
} /* ldpSendCommand */

