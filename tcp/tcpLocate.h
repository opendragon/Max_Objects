/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       tcpLocate.h                                                             */
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
/*  Created:    2000/11/06                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(TCPLOCATE_H_))
 #define TCPLOCATE_H_ /* */
 
 /*#define USE_SYSLOG /* */

 #include "MissingAndExtra.h" 
 #include "tcpMapping.h"
 
 #define OUR_NAME      "tcpLocate"
 #define OUR_RES_NUMB  17147
 #define OUTPUT_PREFIX "tcpLocate: "

struct TcpLocateData
{
  Object             fObject;
  POutlet            fErrorBangOut;
  POutlet            fResultOut;
  AccessOTControlPtr fAccessControl;
  InetSvcRef         fService;
  TcpObjectState     fState;
  bool               fVerbose;
}; /* TcpLocateData */

typedef TcpLocateData * TcpLocatePtr;

Pvoid cmd_Verbose
  (TcpLocatePtr xx,
   PSymbol      onOff);

void doNameLookup
  (TcpLocatePtr xx,
   Ptr          name);

bool initObject
  (TcpLocatePtr xx);
    
StandardRoutineDeclarations(TcpLocatePtr)

#endif /* not TCPLOCATE_H_ */
