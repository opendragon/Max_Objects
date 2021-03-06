/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       NotSoSimplePlugin.cp                                                    */
/*                                                                                      */
/*  Contains:   The main entry points for this plugin.                                  */
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
#include "NotSoSimplePlugin.h"
#include "PluginCallbacks.h"

using namespace std;  //introduces namespace std

class SomethingShared
{
  public:
    long justANumber;

}; /* SomethingShared */

typedef SomethingShared * PSomethingShared;

class SomethingPrivate
{
  public:
    double anotherNumber;
}; /* SomethingPrivate */

typedef SomethingPrivate * PSomethingPrivate;

/*------------------------------------ do_Anything ---*/
OSErr
do_Anything(PLUGIN_HEADER_PREFIX,
            PSymbol            message,
            const short    argc,
            PAtom                argv)
{
#pragma unused(owner, outlets)
  PSomethingPrivate privateStuff = reinterpret_cast<PSomethingPrivate>(privateStorage);
  PSomethingShared  sharedStuff = reinterpret_cast<PSomethingShared>(sharedStorage);

  LOG_POST_3("message '%s' seen on inlet: %ld", message->s_name, inletNumber)
  LOG_POST_3("argc: %hd argv: 0x%lx", argc, long(argv))
  for (short ii = 0; ii < argc; ++ii)
  {
    switch (argv[ii].a_type)
    {
      case A_LONG:
        LOG_POST_3("  argument %hd is a long (%ld)", ii, argv[ii].a_w.w_long)
        break;

      case A_SYM:
        LOG_POST_3("  argument %hd is a symbol (%s)", ii, argv[ii].a_w.w_sym->s_name)
        break;

      case A_FLOAT:
        LOG_POST_3("  argument %hd is a float (%g)", ii, double(argv[ii].a_w.w_float))
        break;

    }
  }
  LOG_POST_1("BLORG!!!!")
  outlet_int(*outlets, 5L);
  outlet_float(*(outlets + 1), 3.5);
  return noErr;
} /* do_Anything */

/*------------------------------------ do_Bang ---*/
OSErr
do_Bang(PLUGIN_HEADER_PREFIX)
{
#pragma unused(owner, outlets)
  PSomethingPrivate privateStuff = reinterpret_cast<PSomethingPrivate>(privateStorage);
  PSomethingShared  sharedStuff = reinterpret_cast<PSomethingShared>(sharedStorage);

  LOG_POST_2("bang seen on inlet: %ld", inletNumber)
  return noErr;
} /* do_Bang */

/*------------------------------------ do_Double ---*/
OSErr
do_Double(PLUGIN_HEADER_PREFIX,
          const double    value)
{
#pragma unused(owner, outlets)
  PSomethingPrivate privateStuff = reinterpret_cast<PSomethingPrivate>(privateStorage);
  PSomethingShared  sharedStuff = reinterpret_cast<PSomethingShared>(sharedStorage);

  LOG_POST_3("value '%g' seen on inlet: %ld", value, inletNumber)
  return noErr;
} /* do_Double */

/*------------------------------------ do_List ---*/
OSErr
do_List(PLUGIN_HEADER_PREFIX,
        const short    argc,
        PAtom                argv)
{
#pragma unused(owner, outlets)
  PSomethingPrivate privateStuff = reinterpret_cast<PSomethingPrivate>(privateStorage);
  PSomethingShared  sharedStuff = reinterpret_cast<PSomethingShared>(sharedStorage);

  LOG_POST_2("list seen on inlet: %ld", inletNumber)
  LOG_POST_3("argc: %hd argv: 0x%lx", argc, long(argv))
  for (short ii = 0; ii < argc; ++ii)
  {
    switch (argv[ii].a_type)
    {
      case A_LONG:
        LOG_POST_3("  argument %hd is a long (%ld)", ii, argv[ii].a_w.w_long)
        break;

      case A_SYM:
        LOG_POST_3("  argument %hd is a symbol (%s)", ii, argv[ii].a_w.w_sym->s_name)
        break;

      case A_FLOAT:
        LOG_POST_3("  argument %hd is a float (%g)", ii, double(argv[ii].a_w.w_float))
        break;

    }
  }
  return noErr;
} /* do_List */

/*------------------------------------ do_Long ---*/
OSErr
do_Long(PLUGIN_HEADER_PREFIX,
        const long    value)
{
#pragma unused(owner, outlets)
  PSomethingPrivate privateStuff = reinterpret_cast<PSomethingPrivate>(privateStorage);
  PSomethingShared  sharedStuff = reinterpret_cast<PSomethingShared>(sharedStorage);

  LOG_POST_3("value '%ld' seen on inlet: %ld", value, inletNumber)
  return noErr;
} /* do_Long */

/*------------------------------------ initialTwas ---*/
static OSErr
initialTwas(PLUGIN_HEADER_PREFIX,
            const short    argc,
            PAtom                argv)
{
#pragma unused(owner, outlets, privateStorage, sharedStorage, argv)
  LOG_POST_3("initialTwas, argc: %hd, inlet: %ld", argc, inletNumber)
  return noErr;
} /* initialTwas */

/*------------------------------------ main ---*/
OSErr
main(CFragConnectionID connID,
     OwnerPtr          owner,
     Handle            sharedStorage)
{
  PSomethingShared forUs = new SomethingShared;

  *sharedStorage = reinterpret_cast<Ptr>(forUs);
  LOG_POST_3("in main(0x%lx,0x%lx)", long(connID), long(*sharedStorage))
  registerMessage(owner, gensym("twas"), initialTwas);
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
onCreate(OwnerPtr         owner,
         Pchar            theName,
         Ptr              sharedStorage,
         Handle           privateStorage,
         const short    argc,
         PAtom            argv,
         Pshort           numInlets,
         Pshort           numOutlets)
{
#pragma unused(owner, theName, argv)
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

/*------------------------------------ twasAfterReload ---*/
static OSErr
twasAfterReload(PLUGIN_HEADER_PREFIX,
                const short    argc,
                PAtom                argv)
{
#pragma unused(owner, outlets, privateStorage, sharedStorage, argv)
  LOG_POST_3("twasAfterReload, argc: %hd, inlet: %ld", argc, inletNumber)
  return noErr;
} /* twasAfterReload */

/*------------------------------------ onReload ---*/
OSErr
onReload(CFragConnectionID connID,
         OwnerPtr          owner,
         Ptr               sharedStorage)
{
  LOG_POST_3("in onReload(0x%lx,0x%lx)", long(connID), long(sharedStorage))
  registerMessage(owner, gensym("twas"), twasAfterReload);
  return noErr;
} /* onReload */
