/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       Vcollect.c                                                              */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the Vcollect module.       */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2002 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2002/08/27                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#define IS_MAIN /* */
#include "Vcollect.h"
#include "displayAboutBox.h"
#include "loadAboutBoxData.h"
#include "loadOtherSegments.h"
#include "reportVersion.h"

/* Forward references: */
Pvoid VcollectCreate
  (void);

Pvoid VcollectFree
  (VcollectPtr xx);

/*------------------------------------ main ---*/
void main
  (Pfptr ff)
{
  EnterCodeResource();
  PrepareCallback();
  FNS = ff;   /* Connect up the function macros. */
  /* Allocate class memory and set up class. */
  setup(reinterpret_cast<t_messlist**>(&gClass), reinterpret_cast<method>(VcollectCreate),
        reinterpret_cast<method>(VcollectFree), short(sizeof(VcollectData)),
        reinterpret_cast<method>(0L), A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Anything), "anything", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Assist), "assist", A_CANT, A_NOTHING);
  addbang(reinterpret_cast<method>(cmd_Bang));
  addmess(reinterpret_cast<method>(cmd_Clear), "clear", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Count), "count", A_NOTHING);
  addfloat(reinterpret_cast<method>(cmd_Float));
  addmess(reinterpret_cast<method>(cmd_Info), "info", A_CANT, A_NOTHING);
  addint(reinterpret_cast<method>(cmd_Int));
  addmess(reinterpret_cast<method>(cmd_List), "list", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Start), "start", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Stop), "stop", A_NOTHING);
  /* Add object class to New Object List */
  finder_addclass("Lists", OUR_NAME);
  /* Make sure that our string list resource is present */
  rescopy('STR#', OUR_RES_NUMB);
  loadAboutBoxData();
  gClearSymbol = gensym("clear");
  gCountSymbol = gensym("count");
  gStartSymbol = gensym("start");
  gStopSymbol = gensym("stop");
  gVersionNumber = reportVersion(OUR_NAME);
  loadOtherSegments();
  ExitCodeResource();
} /* main */

/*------------------------------------ VcollectCreate ---*/
Pvoid VcollectCreate
  (void)
{
  VcollectPtr xx;

  EnterCallback();
  LOG_ENTER()
  xx = static_cast<VcollectPtr>(newobject(gClass));
  if (xx)
  {
    xx->fLengthOut = static_cast<POutlet>(intout(xx));
    xx->fResultOut = static_cast<POutlet>(outlet_new(xx, NULL_PTR));
    xx->fProxy = proxy_new(xx, 1L, &xx->fInletNumber);
    xx->fFirstChunk = xx->fFreeChunks = xx->fLastChunk = NULL_PTR;
    xx->fAtomsInLastChunk = xx->fTotalAtoms = 0;
    xx->fCollecting = true;
    if (! (xx->fLengthOut && xx->fResultOut && xx->fProxy))
    {
      LOG_ERROR_1(OUTPUT_PREFIX "unable to create port for object")
      freeobject(reinterpret_cast<PObject>(xx));
      xx = NULL_PTR;
    }
  }
  ExitCodeResource();
  return xx;
} /* VcollectCreate */

/*------------------------------------ VcollectFree ---*/
Pvoid VcollectFree
  (VcollectPtr xx)
{
  EnterCallback();
  if (xx)
  {
    if (xx->fProxy)
    {
      freeobject(reinterpret_cast<PObject>(xx->fProxy));
      xx->fProxy = NULL_PTR;
    }
    /* Now return any chunks to the free list: */
    if (xx->fTotalAtoms > 0)
    {
      xx->fLastChunk->fNext = xx->fFreeChunks;
      xx->fFreeChunks = xx->fFirstChunk;
      xx->fFirstChunk = xx->fLastChunk = NULL_PTR;
      xx->fTotalAtoms = xx->fAtomsInLastChunk = 0;
    }
    /* And, finally, return the free list to the system: */
    for ( ; xx->fFreeChunks; )
    {
      CollectChunkPtr  next = xx->fFreeChunks->fNext;

      FREEBYTES(xx->fFreeChunks, sizeof(CollectChunk))
      xx->fFreeChunks = next; 
    }
  }
  LOG_EXIT()
  ExitMaxMessageHandler()
} /* VcollectFree */

/*------------------------------------ collectAddAtoms ---*/
void collectAddAtoms
  (VcollectPtr xx,
   PAtom       atoms,
   const short	count)
{
  if (xx->fCollecting && (count > 0))
  {
    short lastCount = xx->fAtomsInLastChunk;

    if (! xx->fLastChunk)
    {
      if (xx->fFreeChunks)
      {
        xx->fFirstChunk = xx->fFreeChunks;
        xx->fFreeChunks = xx->fFreeChunks->fNext;
      }
      else
        xx->fFirstChunk = GETBYTES(1, CollectChunk);
      xx->fLastChunk = xx->fFirstChunk;
      xx->fLastChunk->fNext = NULL_PTR;
    }
    if (xx->fLastChunk)
    {
      short           offset = 0, thisMove, workingCount = count;
      CollectChunkPtr outWalk = xx->fLastChunk;

      for ( ; workingCount > 0; )
      {
        if ((lastCount + workingCount) <= CHUNK_SIZE)
        {
          /* They all fit in the current chunk */
          memcpy(&outWalk->fAtoms[lastCount], atoms + offset,
          				workingCount * sizeof(Atom));
          xx->fAtomsInLastChunk += workingCount;
          xx->fTotalAtoms += workingCount;
          break;  

        }
        /* There will be an overflow */
        thisMove = short(CHUNK_SIZE - lastCount);                  
        memcpy(&outWalk->fAtoms[lastCount], atoms + offset,
        thisMove * sizeof(Atom));
        workingCount -= thisMove;
        offset += thisMove;
        xx->fTotalAtoms += thisMove;
        /* Get another chunk */
        if (xx->fFreeChunks)
        {
          outWalk->fNext = xx->fFreeChunks;
          xx->fFreeChunks = xx->fFreeChunks->fNext;
        }
        else
          outWalk->fNext = GETBYTES(1, CollectChunk);
        outWalk = outWalk->fNext;
        if (! outWalk)
          break;

        xx->fLastChunk = outWalk;
        outWalk->fNext = NULL_PTR;
        lastCount = xx->fAtomsInLastChunk = 0;
      }
    }
  }
} /* collectAddAtoms */

StandardInfoRoutine(VcollectPtr)