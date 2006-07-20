/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       SimplePlugin.cp                                                         */
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

#include <CodeFragments.h>
#include "SimplePlugin.h"
#include "PluginCallbacks.h"

using namespace std;  //introduces namespace std

/*------------------------------------ do_Anything ---*/
OSErr do_Anything
  (PLUGIN_HEADER_PREFIX,
   PSymbol			message,
   const short	argc,
   PAtom				argv)
{
#pragma unused(owner, outlets, sharedStorage, privateStorage, inletNumber, message, argc, argv)
  return noErr;
} /* do_Anything */

/*------------------------------------ do_Bang ---*/
OSErr do_Bang
  (PLUGIN_HEADER_PREFIX)
{
#pragma unused(owner, outlets, sharedStorage, privateStorage, inletNumber)
  return noErr;
} /* do_Bang */

/*------------------------------------ do_Double ---*/
OSErr do_Double
  (PLUGIN_HEADER_PREFIX,
   const double	value)
{
#pragma unused(owner, outlets, sharedStorage, privateStorage, inletNumber, value)
  return noErr;
} /* do_Double */

/*------------------------------------ do_List ---*/
OSErr do_List
  (PLUGIN_HEADER_PREFIX,
   const short	argc,
   PAtom				argv)
{
#pragma unused(owner, outlets, sharedStorage, privateStorage, inletNumber, argc, argv)
  return noErr;
} /* do_List */

/*------------------------------------ do_Long ---*/
OSErr do_Long
  (PLUGIN_HEADER_PREFIX,
   const long	value)
{
#pragma unused(owner, outlets, sharedStorage, privateStorage, inletNumber, value)
  return noErr;
} /* do_Long */

/*------------------------------------ main ---*/
OSErr main
  (CFragConnectionID connID,
   OwnerPtr          owner,
   Handle            sharedStorage)
{
#pragma unused(owner)
  *sharedStorage = NULL_HDL;
  LOG_POST_3("in main(0x%lx,0x%lx)", long(connID), long(sharedStorage))
  return noErr;
} /* main */

/*------------------------------------ niam ---*/
OSErr niam
  (OwnerPtr owner,
   Ptr      sharedStorage)
{
#pragma unused(owner)
  LOG_POST_2("in niam(0x%lx)", long(sharedStorage))
  return noErr;
} /* niam */

/*------------------------------------ onCreate ---*/
OSErr onCreate
  (OwnerPtr 		owner,
   Pchar    		theName,
   Ptr      		sharedStorage,
   Handle   		privateStorage,
   const short	argc,
   PAtom    		argv,
   Pshort   		numInlets,
   Pshort   		numOutlets)
{
#pragma unused(owner, theName, argv)
  *privateStorage = NULL_HDL;
  *numInlets = 1;
  *numOutlets = 1;
  LOG_POST_4("in onCreate(0x%lx,0x%lx,%ld)", long(sharedStorage), long(*privateStorage),
              long(argc))
  return noErr;
} /* onCreate */

/*------------------------------------ onDestroy ---*/
OSErr onDestroy
  (OwnerPtr owner,
   Ptr      sharedStorage,
   Ptr      privateStorage)
{
#pragma unused(owner)
  LOG_POST_3("in onDestroy(0x%lx,0x%lx)", long(sharedStorage), long(privateStorage))
  return noErr;
} /* onDestroy */

/*------------------------------------ onReload ---*/
OSErr onReload
  (CFragConnectionID connID,
   OwnerPtr          owner,
   Ptr               sharedStorage)
{
#pragma unused(owner)
  LOG_POST_3("in onReload(0x%lx,0x%lx)", long(connID), long(sharedStorage))
  return noErr;
} /* onReload */
