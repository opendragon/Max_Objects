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
# define QUEUE_H_ /* */

# include "missingAndExtra.h"
# include "genericListOutput.h"

# define OUR_NAME      "queue"
// # define OUR_RES_NUMB  17178
# define OUTPUT_PREFIX "queue: "

struct QueueEntry
{
    QueueEntry * fNext;
    t_atom *     fOutput;
    short        fOutputCount;
}; /* QueueEntry */

struct QueueData
{
    t_object     fObject;
    t_outlet *   fResultOut;
    t_outlet *   fDepthOut;
    t_outlet *   fErrorBangOut;
    QueueEntry * fFirstInQueue;
    QueueEntry * fLastInQueue;
    long         fDepth;
    long         fMaxDepth;
    bool         fVerbose;
}; // QueueData

void cmd_Add(QueueData * xx,
             t_symbol *  message,
             short       argc,
             t_atom *    argv);

void cmd_Clear(QueueData * xx);

void cmd_Depth(QueueData * xx);

void cmd_Fetch(QueueData * xx);

void cmd_Pull(QueueData * xx);

void cmd_SetDepth(QueueData * xx,
                  long        number);

void cmd_Trace(QueueData * xx,
               t_symbol *  onOff);

void queueClear(QueueData * xx);

StandardRoutineDeclarations(QueueData *);

mextern(t_symbol *) gEmptySymbol; /* Pointer to unique symbol for '' */
mextern(t_symbol *) gOffSymbol;   /* Pointer to unique symbol for 'off' */
mextern(t_symbol *) gOnSymbol;    /* Pointer to unique symbol for 'on' */

#endif /* not QUEUE_H_ */
