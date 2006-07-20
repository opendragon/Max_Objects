/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       queue.h                                                                 */
/*                                                                                      */
/*  Contains:   The data type declarations and forward references to routines.          */
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
/*  Created:    2001/04/24                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(QUEUE_H_))
 #define QUEUE_H_ /* */
 
 /*#define USE_SYSLOG /* */

 #include "MissingAndExtra.h"
 #include "genericListOutput.h"

 #define OUR_NAME      "queue"
 #define OUR_RES_NUMB  17178
 #define OUTPUT_PREFIX "queue: "
 
struct QueueEntry
{
  QueueEntry *  fNext;
  PAtom         fOutput;
  short         fOutputCount;
}; /* QueueEntry */

typedef QueueEntry * QueueEntryPtr;

struct QueueData
{
  Object        fObject;
  POutlet       fResultOut;
  POutlet       fDepthOut;
  POutlet       fErrorBangOut;
  QueueEntryPtr fFirstInQueue;
  QueueEntryPtr fLastInQueue;
  long          fDepth;
  long          fMaxDepth;
  bool          fVerbose;
}; /* QueueData */

typedef QueueData * QueuePtr;

Pvoid cmd_Add
  (QueuePtr xx,
   PSymbol  message,
   short    argc,
   PAtom    argv);

Pvoid cmd_Clear
  (QueuePtr xx);

Pvoid cmd_Depth
  (QueuePtr xx);

Pvoid cmd_Fetch
  (QueuePtr xx);

Pvoid cmd_Pull
  (QueuePtr xx);

Pvoid cmd_SetDepth
  (QueuePtr xx,
   long     number);

Pvoid cmd_Trace
  (QueuePtr xx,
   PSymbol  onOff);

void queueClear
  (QueuePtr xx);

StandardRoutineDeclarations(QueuePtr)

mextern(PSymbol) gEmptySymbol; /* Pointer to unique Symbol for '' */
mextern(PSymbol) gOffSymbol;   /* Pointer to unique Symbol for 'off' */
mextern(PSymbol) gOnSymbol;    /* Pointer to unique Symbol for 'on' */

#endif /* not QUEUE_H_ */
