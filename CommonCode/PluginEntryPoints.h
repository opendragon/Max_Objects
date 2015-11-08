/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       PluginEntryPoints.h                                                     */
/*                                                                                      */
/*  Contains:   The data type declarations that are needed for standard plugins.        */
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
/*  Created:    2001/08/11                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(PLUGINENTRYPOINTS_H_))
 #define PLUGINENTRYPOINTS_H_ /* */

 #include <CodeFragments.h>
 #include <MixedMode.h>
 #include "StandardPluginTypes.h"

 #if COMPILE_FOR_OSX_4
typedef OSErr (*FpDoAnything)
    (OwnerPtr   owner,
    HOutlet     outlets,
    Ptr         sharedStorage,
    Ptr         privateStorage,
    const long  inletNumber,
    PSymbol     message,
    const short argc,
    PAtom       argv);

typedef OSErr (*FpDoBang)
    (OwnerPtr  owner,
    HOutlet    outlets,
    Ptr        sharedStorage,
    Ptr        privateStorage,
    const long inletNumber);

typedef OSErr (*FpDoDouble)
    (OwnerPtr    owner,
    HOutlet      outlets,
    Ptr          sharedStorage,
    Ptr          privateStorage,
    const long   inletNumber,
    const double value);

typedef OSErr (*FpDoList)
    (OwnerPtr   owner,
    HOutlet     outlets,
    Ptr         sharedStorage,
    Ptr         privateStorage,
    const long  inletNumber,
    const short argc,
    PAtom       argv);

typedef OSErr (*FpDoLong)
    (OwnerPtr  owner,
    HOutlet    outlets,
    Ptr        sharedStorage,
    Ptr        privateStorage,
    const long inletNumber,
    const long value);

typedef OSErr (*FpMain)
    (CFragConnectionID connID,
    OwnerPtr           owner,
    Handle             sharedStorage);

typedef OSErr (*FpNiam)
    (OwnerPtr owner,
    Ptr       sharedStorage);

typedef OSErr (*FpOnCreate)
    (OwnerPtr   owner,
    Pchar       theName,
    Ptr         sharedStorage,
    Handle      privateStorage,
    const short argc,
    PAtom       argv,
    Pshort      numInlets,
    Pshort      numOutlets);

typedef OSErr (*FpOnDestroy)
    (OwnerPtr owner,
    Ptr       sharedStorage,
    Ptr       privateStorage);

typedef OSErr (*FpOnReload)
    (CFragConnectionID connID,
    OwnerPtr           owner,
    Ptr                sharedStorage);
 #endif /* COMPILE_FOR_OSX_4 */
 #if COMPILE_FOR_OS9_4
  #define PLUGIN_PROCINFO_PREFIX \
    STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(OwnerPtr))) | \
    STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(HOutlet))) | \
    STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(Ptr))) | \
    STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(Ptr))) | \
    STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(long)))

  #define FIRST_PLUGIN_ARG 6

enum
{
    uppDoAnythingProcInfo = (kCStackBased |
                             RESULT_SIZE(SIZE_CODE(sizeof(OSErr))) |
                             PLUGIN_PROCINFO_PREFIX |
                             STACK_ROUTINE_PARAMETER(FIRST_PLUGIN_ARG, SIZE_CODE(sizeof(PSymbol))) |
                             STACK_ROUTINE_PARAMETER((FIRST_PLUGIN_ARG + 1), SIZE_CODE(sizeof(short))) |
                             STACK_ROUTINE_PARAMETER((FIRST_PLUGIN_ARG + 2), SIZE_CODE(sizeof(PAtom)))),
    uppDoBangProcInfo = (kCStackBased |
                         RESULT_SIZE(SIZE_CODE(sizeof(OSErr))) |
                         PLUGIN_PROCINFO_PREFIX),
    uppDoDoubleProcInfo = (kCStackBased |
                           RESULT_SIZE(SIZE_CODE(sizeof(OSErr))) |
                           PLUGIN_PROCINFO_PREFIX |
                           STACK_ROUTINE_PARAMETER(FIRST_PLUGIN_ARG, SIZE_CODE(sizeof(double)))),
    uppDoListProcInfo = (kCStackBased |
                         RESULT_SIZE(SIZE_CODE(sizeof(OSErr))) |
                         PLUGIN_PROCINFO_PREFIX |
                         STACK_ROUTINE_PARAMETER(FIRST_PLUGIN_ARG, SIZE_CODE(sizeof(short))) |
                         STACK_ROUTINE_PARAMETER((FIRST_PLUGIN_ARG + 1), SIZE_CODE(sizeof(PAtom)))),
    uppDoLongProcInfo = (kCStackBased |
                         RESULT_SIZE(SIZE_CODE(sizeof(OSErr))) |
                         PLUGIN_PROCINFO_PREFIX |
                         STACK_ROUTINE_PARAMETER(FIRST_PLUGIN_ARG, SIZE_CODE(sizeof(long)))),
    uppMainProcInfo = (kCStackBased |
                       RESULT_SIZE(SIZE_CODE(sizeof(OSErr))) |
                       STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(CFragConnectionID))) |
                       STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(OwnerPtr))) |
                       STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(Handle)))),
    uppNiamProcInfo = (kCStackBased |
                       RESULT_SIZE(SIZE_CODE(sizeof(OSErr))) |
                       STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(OwnerPtr))) |
                       STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(Ptr)))),
    uppOnCreateProcInfo = (kCStackBased |
                           RESULT_SIZE(SIZE_CODE(sizeof(OSErr))) |
                           STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(OwnerPtr))) |
                           STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(Pchar))) |
                           STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(Ptr))) |
                           STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(Handle))) |
                           STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(short))) |
                           STACK_ROUTINE_PARAMETER(6, SIZE_CODE(sizeof(PAtom))) |
                           STACK_ROUTINE_PARAMETER(7, SIZE_CODE(sizeof(Pshort))) |
                           STACK_ROUTINE_PARAMETER(8, SIZE_CODE(sizeof(Pshort)))),
    uppOnDestroyProcInfo = (kCStackBased |
                            RESULT_SIZE(SIZE_CODE(sizeof(OSErr))) |
                            STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(OwnerPtr))) |
                            STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(Ptr))) |
                            STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(Ptr)))),
    uppOnReloadProcInfo = (kCStackBased |
                           RESULT_SIZE(SIZE_CODE(sizeof(OSErr))) |
                           STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(CFragConnectionID))) |
                           STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(OwnerPtr))) |
                           STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(Ptr))))
};
 #endif /* COMPILE_FOR_OS9_4 */

#endif /* not PLUGINENTRYPOINTS_H_ */