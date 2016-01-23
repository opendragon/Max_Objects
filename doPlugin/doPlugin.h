/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       doPlugin.h                                                              */
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
/*  Created:    2001/08/07                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(DOPLUGIN_H_))
 #define DOPLUGIN_H_ /* */
 
 #define USE_SYSLOG /* */

 #include <CodeFragments.h>
 #include "MissingAndExtra.h"
 #include "genericListOutput.h"
 #include "PluginEntryPoints.h"
 #include "PluginAuxCallbacks.h"

 #define OUR_NAME      "doPlügin"
 #define OUR_RES_NUMB  17183
 #define OUTPUT_PREFIX "doPlügin: "

enum 
{
  kDoAnythingValid = (1 << 0),
  kDoBangValid = (1 << 1),
  kDoDoubleValid = (1 << 2),
  kDoListValid = (1 << 3),
  kDoLongValid = (1 << 4),
  kMainValid = (1 << 5),
  kNiamValid = (1 << 6),
  kOnCreateValid = (1 << 7),
  kOnDestroyValid = (1 << 8),
  kOnReloadValid = (1 << 9)
};

struct PluginDescriptor
{
  PluginDescriptor * fNext;
  PluginDescriptor * fPrevious;
  PSymbol            fName;
  FSSpec             fSpec;
  Ptr                fShared;
  long               fRefCount;
  long               fValidMask;
 #if FOR_MAC_PPC
  CFragConnectionID  fConnID;
  Ptr                fMainAddress;
  Pvoid              fRegistry;
  #if defined(COMPILE_FOR_OSX_4)
  ProcPtr            fDoAnythingFun;
  ProcPtr            fDoBangFun;
  ProcPtr            fDoDoubleFun;
  ProcPtr            fDoListFun;
  ProcPtr            fDoLongFun;
  ProcPtr            fMainFun;
  ProcPtr            fNiamFun;
  ProcPtr            fOnCreateFun;
  ProcPtr            fOnDestroyFun;
  ProcPtr            fOnReloadFun;
  #endif /* COMPILE_FOR_OSX_4 */
  #if defined(COMPILE_FOR_OS9_4)
  UniversalProcPtr   fDoAnythingUpp;
  UniversalProcPtr   fDoBangUpp;
  UniversalProcPtr   fDoDoubleUpp;
  UniversalProcPtr   fDoListUpp;
  UniversalProcPtr   fDoLongUpp;
  UniversalProcPtr   fMainUpp;
  UniversalProcPtr   fNiamUpp;
  UniversalProcPtr   fOnCreateUpp;
  UniversalProcPtr   fOnDestroyUpp;
  UniversalProcPtr   fOnReloadUpp;
  #endif /* COMPILE_FOR_OS9_4 */
 #endif /* FOR_MAC_PPC */
}; /* PluginDescriptor */

typedef PluginDescriptor * PluginDescriptorPtr;

struct DoPluginData
{
  Object              fObject;
  PluginDescriptorPtr fActivePlugin;
  short               fNumInlets;
  short               fNumOutlets;
  long                fInletNumber;
  Ptr                 fPrivate;
  HOutlet             fOutlets;
  Hvoid               fProxies;
  OwnerData           fOwnerInfo;   
}; /* DoPluginData */

typedef DoPluginData * DoPluginPtr;

Pvoid
cmd_Plugin(DoPluginPtr xx,
           PSymbol     message,
           short       argc,
           PAtom       argv);

void
refreshDescriptors(DoPluginPtr xx);

void
rescanPlugins(void);

 #define PLUGIN_FIXED_ARGUMENTS \
    &xx->fOwnerInfo, xx->fOutlets, descriptor->fShared, xx->fPrivate, xx->fInletNumber

StandardRoutineDeclarations(DoPluginPtr)

mextern(long)                 gAppDir;
mextern(FSSpec)               gAppFile;
mextern(short)                gAppVRef;
mextern(PSymbol)              gBangSymbol;
mextern(PSymbol)              gEmptySymbol;
mextern(PSymbol)              gFloatSymbol;
mextern(PSymbol)              gIntSymbol;
mextern(PSymbol)              gListSymbol;
mextern(long)                 gObjectCount;
mextern(PluginDescriptorPtr)  gPluginAnchor;
mextern(long)                 gPluginDir;
mextern(Str255)               gPluginFolderAlias;
mextern(bool)                 gPluginFolderFound;
mextern(Str255)               gPluginFolderName;
mextern(short)                gPluginVol;
mextern(PSymbol)              gReloadSymbol;
mextern(PSymbol)              gRescanSymbol;
mextern(long)                 gSysParID;
mextern(short)                gSysVRef;

#endif /* not DOPLUGIN_H_ */
