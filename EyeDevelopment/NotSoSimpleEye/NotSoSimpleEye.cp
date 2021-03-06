/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       NotSoSimpleEye.cp                                                       */
/*                                                                                      */
/*  Contains:   The routines that are needed for standard eyes.                         */
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
/*  Created:    2001/08/28                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#define USE_SYSLOG /* */

#include <CodeFragments.h>
#include "NotSoSimpleEye.h"

using namespace std;  //introduces namespace std

class SomethingShared
{
  public:
    long justANumber;

};

typedef SomethingShared * PSomethingShared;

class SomethingPrivate
{
  public:
    double anotherNumber;
};

typedef SomethingPrivate * PSomethingPrivate;

/*------------------------------------ do_Assist ---*/
void
do_Assist(EYE_HEADER_PREFIX,
          long  msg,
          long  arg,
          Pchar dstString)
{
#pragma unused(owner, msg, arg, dstString)
  PSomethingPrivate privateStuff = reinterpret_cast<PSomethingPrivate>(privateStorage);
  PSomethingShared  sharedStuff = reinterpret_cast<PSomethingShared>(sharedStorage);
} /* do_Assist */

/*------------------------------------ do_Display ---*/
void
do_Display(EYE_HEADER_PREFIX,
           GWorldPtr world,
           HPixmap   offPixMap,
           long      displayIncomingImage,
           long      displayColor,
           long      displayDim)
{
#pragma unused(owner, world, offPixMap, displayIncomingImage, displayColor, displayDim)
  PSomethingPrivate privateStuff = reinterpret_cast<PSomethingPrivate>(privateStorage);
  PSomethingShared  sharedStuff = reinterpret_cast<PSomethingShared>(sharedStorage);
} /* do_Display */

/*------------------------------------ do_GetSettings ---*/
void
do_GetSettings(EYE_HEADER_PREFIX,
               Pbool twoOrOne,
               Plong displayIncoming)
{
#pragma unused(owner, twoOrOne, displayIncoming)
  PSomethingPrivate privateStuff = reinterpret_cast<PSomethingPrivate>(privateStorage);
  PSomethingShared  sharedStuff = reinterpret_cast<PSomethingShared>(sharedStorage);
} /* do_GetSettings */

/*------------------------------------ do_GetValues ---*/
void
do_GetValues(EYE_HEADER_PREFIX,
             Pshort ac,
             PAtom  av)
{
#pragma unused(owner, ac, av)
  PSomethingPrivate privateStuff = reinterpret_cast<PSomethingPrivate>(privateStorage);
  PSomethingShared  sharedStuff = reinterpret_cast<PSomethingShared>(sharedStorage);
} /* do_GetValues */

/*------------------------------------ do_Go ---*/
void
do_Go(EYE_HEADER_PREFIX)
{
#pragma unused(owner)
  PSomethingPrivate privateStuff = reinterpret_cast<PSomethingPrivate>(privateStorage);
  PSomethingShared  sharedStuff = reinterpret_cast<PSomethingShared>(sharedStorage);
} /* do_Go */

/*------------------------------------ do_Graphics ---*/
void
do_Graphics(EYE_HEADER_PREFIX)
{
#pragma unused(owner)
  PSomethingPrivate privateStuff = reinterpret_cast<PSomethingPrivate>(privateStorage);
  PSomethingShared  sharedStuff = reinterpret_cast<PSomethingShared>(sharedStorage);
} /* do_Graphics */

/*------------------------------------ do_Index ---*/
void
do_Index(EYE_HEADER_PREFIX,
         Pvoid sensor)
{
#pragma unused(owner, sensor)
  PSomethingPrivate privateStuff = reinterpret_cast<PSomethingPrivate>(privateStorage);
  PSomethingShared  sharedStuff = reinterpret_cast<PSomethingShared>(sharedStorage);
} /* do_Index */

/*------------------------------------ do_MyName ---*/
Pchar
do_MyName(EYE_HEADER_PREFIX)
{
#pragma unused(owner)
  PSomethingPrivate privateStuff = reinterpret_cast<PSomethingPrivate>(privateStorage);
  PSomethingShared  sharedStuff = reinterpret_cast<PSomethingShared>(sharedStorage);

return NULL_PTR;
} /* do_MyName */

/*------------------------------------ do_Save ---*/
void
do_Save(EYE_HEADER_PREFIX,
        Pshort ac,
        PAtom  av)
{
#pragma unused(owner, ac, av)
  PSomethingPrivate privateStuff = reinterpret_cast<PSomethingPrivate>(privateStorage);
  PSomethingShared  sharedStuff = reinterpret_cast<PSomethingShared>(sharedStorage);
} /* do_Save */

/*------------------------------------ do_SetParameterDouble ---*/
void
do_SetParameterDouble(EYE_HEADER_PREFIX,
                      short  which,
                      double value)
{
#pragma unused(owner, which, value)
  PSomethingPrivate privateStuff = reinterpret_cast<PSomethingPrivate>(privateStorage);
  PSomethingShared  sharedStuff = reinterpret_cast<PSomethingShared>(sharedStorage);
} /* do_SetParameterDouble */

/*------------------------------------ do_SetParameterList ---*/
void
do_SetParameterList(EYE_HEADER_PREFIX,
                    short which,
                    short argc,
                    PAtom argv)
{
#pragma unused(owner, which, argc, argv)
  PSomethingPrivate privateStuff = reinterpret_cast<PSomethingPrivate>(privateStorage);
  PSomethingShared  sharedStuff = reinterpret_cast<PSomethingShared>(sharedStorage);
} /* do_SetParameterList */

/*------------------------------------ do_SetParameterLong ---*/
void
do_SetParameterLong(EYE_HEADER_PREFIX,
                    short which,
                    long  value)
{
#pragma unused(owner, which, value)
  PSomethingPrivate privateStuff = reinterpret_cast<PSomethingPrivate>(privateStorage);
  PSomethingShared  sharedStuff = reinterpret_cast<PSomethingShared>(sharedStorage);
} /* do_SetParameterLong */

/*------------------------------------ do_SetParameters ---*/
void
do_SetParameters(EYE_HEADER_PREFIX,
                 Pshort base,
                 short  argc,
                 PAtom  argv)
{
#pragma unused(owner, base, argc, argv)
  PSomethingPrivate privateStuff = reinterpret_cast<PSomethingPrivate>(privateStorage);
  PSomethingShared  sharedStuff = reinterpret_cast<PSomethingShared>(sharedStorage);
} /* do_SetParameters */

/*------------------------------------ do_Source ---*/
void
do_Source(EYE_HEADER_PREFIX,
          Pvoid source)
{
#pragma unused(owner, source)
  PSomethingPrivate privateStuff = reinterpret_cast<PSomethingPrivate>(privateStorage);
  PSomethingShared  sharedStuff = reinterpret_cast<PSomethingShared>(sharedStorage);
} /* do_Source */

/*------------------------------------ do_Source2 ---*/
void
do_Source2(EYE_HEADER_PREFIX,
           Pvoid source)
{
#pragma unused(owner, source)
  PSomethingPrivate privateStuff = reinterpret_cast<PSomethingPrivate>(privateStorage);
  PSomethingShared  sharedStuff = reinterpret_cast<PSomethingShared>(sharedStorage);
} /* do_Source2 */

/*------------------------------------ do_Target ---*/
void
do_Target(EYE_HEADER_PREFIX,
          Pvoid target)
{
#pragma unused(owner, target)
  PSomethingPrivate privateStuff = reinterpret_cast<PSomethingPrivate>(privateStorage);
  PSomethingShared  sharedStuff = reinterpret_cast<PSomethingShared>(sharedStorage);
} /* do_Target */

/*------------------------------------ main ---*/
OSErr
main(CFragConnectionID connID,
     OwnerPtr          owner,
     Handle            sharedStorage)
{
#pragma unused(owner)
  PSomethingShared forUs = new SomethingShared;

  *sharedStorage = reinterpret_cast<Ptr>(forUs);
  LOG_POST_3("in main(%ld,0x%lx)", long(connID), long(*sharedStorage))
  return noErr;
} /* main */

/*------------------------------------ niam ---*/
OSErr
niam(OwnerPtr owner,
     Ptr      sharedStorage)
{
#pragma unused(owner)
  PSomethingShared  sharedStuff = reinterpret_cast<PSomethingShared>(sharedStorage);

  LOG_POST_2("in niam(0x%lx)", long(sharedStorage))
  if (sharedStuff)
    delete sharedStuff;
  return noErr;
} /* niam */

/*------------------------------------ onCreate ---*/
OSErr
onCreate(OwnerPtr owner,
         Pchar    theName,
         Ptr      sharedStorage,
         Handle   privateStorage,
         short    argc,
         PAtom    argv,
         Pshort   numInlets,
         Pshort   numOutlets)
{
#pragma unused(owner, theName, argc, argv)
  PSomethingShared  sharedStuff = reinterpret_cast<PSomethingShared>(sharedStorage);
  PSomethingPrivate forUs = new SomethingPrivate;

  *privateStorage = reinterpret_cast<Ptr>(forUs);
  *numInlets = 2;
  *numOutlets = 2;
  LOG_POST_4("in onCreate(0x%lx,0x%lx,%ld)", long(sharedStorage), long(*privateStorage),
            long(argc))
  return noErr;
} /* onCreate */

/*------------------------------------ onDestroy ---*/
OSErr
onDestroy(OwnerPtr owner,
          Ptr      sharedStorage,
          Ptr      privateStorage)
{
#pragma unused(owner)
  PSomethingPrivate privateStuff = reinterpret_cast<PSomethingPrivate>(privateStorage);
  PSomethingShared  sharedStuff = reinterpret_cast<PSomethingShared>(sharedStorage);

  LOG_POST_3("in onDestroy(0x%lx,0x%lx)", long(sharedStorage), long(privateStorage))
  if (privateStuff)
    delete privateStuff;
  return noErr;
} /* onDestroy */

/*------------------------------------ onReload ---*/
OSErr
onReload(CFragConnectionID connID,
         OwnerPtr          owner,
         Ptr               sharedStorage)
{
#pragma unused(owner)
  LOG_POST_3("in onReload(0x%lx,0x%lx)", long(connID), long(sharedStorage))
  return noErr;
} /* onReload */
