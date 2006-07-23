/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       acquireOpenTransport.h                                                  */
/*                                                                                      */
/*  Contains:   The data type declarations and forward references to routines.          */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2000 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2000/11/04                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(ACQUIREOPENTRANSPORT_H_))
 #define ACQUIREOPENTRANSPORT_H_ /* */

 #include "MissingAndExtra.h"
 #include <OpenTptInternet.h>

 #if FOR_MAC_PPC
  #define OPEN_TRANSPORT_SUPPORTED 1
 #else /* not FOR_MAC_PPC */
  #define OPEN_TRANSPORT_SUPPORTED 0
 #endif /* not FOR_MAC_PPC */
 #define OT_CONTROL_SYMBOL "##otcontrol##"

enum
{
  kOTVersionUsed = 0x01110000 /*  0x01200000 */
};

struct AccessOTControlBlock
{
  AccessOTControlBlock * fNext;
  AccessOTControlBlock * fPrevious;
 #if defined(COMPILE_FOR_OSX_4)
  OTClientContextPtr     fContext;
 #endif /* COMPILE_FOR_OSX_4 */
  ushort                 fPort;
  bool                   fIsServer;
}; /* AccessOTControlBlock */

typedef AccessOTControlBlock * AccessOTControlPtr;
typedef AccessOTControlPtr *   AccessOTControlHdl;

AccessOTControlPtr acquireOpenTransport
  (Qchar				name,
   const ushort	port,
   const bool		isServer);

bool isOpenTransportAvailable
  (Qchar	name);

bool isPortInUse
  (const ushort	port);

Ptr mapErrorCodeToString
  (const OSStatus errCode);

void relinquishOpenTransport
  (Qchar							name,
   AccessOTControlHdl reference);
 
extern long gObjectCount;
extern long	gOTVersion;

#endif /* not ACQUIREOPENTRANSPORT_H_ */
