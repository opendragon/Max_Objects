/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       doPlugin.c                                                              */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the notX module.          */
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

#define IS_MAIN /* */
#include "doPlugin.h"
#include "displayAboutBox.h"
#include "loadAboutBoxData.h"
#include "loadOtherSegments.h"
#include "reportVersion.h"
#include "PluginAuxCallbacks.h"

/* Forward references: */
Pvoid doPluginCreate
  (PSymbol ss,
   short   argc,
   PAtom   argv);

Pvoid doPluginFree
   (DoPluginPtr xx);

#define PLUGIN_KIND 		'Plüg'
#define PLUGIN_FOLDER_NAME  1
#define PLUGIN_FOLDER_ALIAS 2

#define OPTIONAL_FUNCTIONS /* */

bool checkForPluginsFolder
  (const short	volumeID,
   const long		dirID);

bool compareStrings
  (Puchar str1,
   Puchar str2);

void freeRoutineDescriptors
  (PluginDescriptorPtr descriptor);

OSErr getAppDir
  (FSSpec * pAppFile);

#if defined(COMPILE_FOR_OSX_4)
OSErr getEntryPoint
  (CFragConnectionID  connID,
   Str255             name,
   ProcPtr *          newUpp);
#endif /* COMPILE_FOR_OSX_4 */

#if defined(COMPILE_FOR_OS9_4)
OSErr getEntryPoint
  (CFragConnectionID  connID,
   Str255             name,
   ProcInfoType       procInfo,
   UniversalProcPtr * newUpp);
#endif /* COMPILE_FOR_OS9_4 */

#if defined(OPTIONAL_FUNCTIONS)
OSErr getFullPath
  (const FSSpec * spec,
   long &         fullPathLength,
   Handle &				fullPath);
#endif // OPTIONAL_FUNCTIONS

bool initPluginList
  (void);

#if defined(OPTIONAL_FUNCTIONS)
OSErr makeFSSpecCompat
  (const short			vRefNum,
   const long				dirID,
   ConstStr255Param fileName,
   FSSpec *         spec);
#endif // OPTIONAL_FUNCTIONS

bool validateEntryPoints
  (PluginDescriptorPtr descriptor,
   FSSpec *            anFSS,
   Puchar              name);

/*------------------------------------ main ---*/
void main
  (Pfptr ff)
{
  OSErr myErr = noErr;

  EnterCodeResource();
  PrepareCallback();
  FNS = ff;   /* Connect up the function macros. */
#if FOR_MAC_PPC
  /* Allocate class memory and set up class. */
  setup(reinterpret_cast<t_messlist**>(&gClass), reinterpret_cast<method>(doPluginCreate),
        reinterpret_cast<method>(doPluginFree), short(sizeof(DoPluginData)),
        reinterpret_cast<method>(0L), A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Anything), "anything", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Assist), "assist", A_CANT, A_NOTHING);
  addbang(reinterpret_cast<method>(cmd_Bang));
  addfloat(reinterpret_cast<method>(cmd_Float));
  addmess(reinterpret_cast<method>(cmd_Info), "info", A_CANT, A_NOTHING);
  addint(reinterpret_cast<method>(cmd_Int));
  addmess(reinterpret_cast<method>(cmd_List), "list", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Plugin), "plugin", A_GIMME, A_NOTHING);
  /* Add object class to New Object List */
  finder_addclass("Arith/Logic/Bitwise", OUR_NAME);
  rescopy('STR#', OUR_RES_NUMB);
  rescopy('STR#', OUR_RES_NUMB + 10000);
  loadAboutBoxData();
  gBangSymbol = gensym("bang");
  gEmptySymbol = gensym("");
  gFloatSymbol = gensym("float");
  gIntSymbol = gensym("int");
  gListSymbol = gensym("list");
  gReloadSymbol = gensym("reload");
  gRescanSymbol = gensym("rescan");
  gPluginAnchor = NULL_PTR;
  gPluginFolderFound = false;
  gObjectCount = 0;
  gVersionNumber = reportVersion(OUR_NAME);
  loadOtherSegments();
  GetIndString(gPluginFolderName, OUR_RES_NUMB + 10000, PLUGIN_FOLDER_NAME);
  GetIndString(gPluginFolderAlias, OUR_RES_NUMB + 10000, PLUGIN_FOLDER_ALIAS);
  if (getAppDir(&gAppFile) == noErr)
  {
    gAppDir = gAppFile.parID;
    gAppVRef = gAppFile.vRefNum;
  }
  else
  {
    gAppDir = 0;
    gAppVRef = 0;
  }
  myErr = FindFolder(kOnSystemDisk, kPreferencesFolderType, kCreateFolder, &gSysVRef,
                      &gSysParID);
  if (myErr != noErr)
    LOG_ERROR_2(OUTPUT_PREFIX "couldn't find Preference folder: %d", myErr)
  else
    gPluginFolderFound = initPluginList();
#else /* not FOR_MAC_PPC */
  error(OUTPUT_PREFIX "not supported on Max/68K");
#endif /* not FOR_MAC_PPC */
  ExitCodeResource();
} /* main */

/*------------------------------------ doPluginCreate ---*/
Pvoid doPluginCreate
  (PSymbol ss,
   short   argc,
   PAtom   argv)
{
#if FOR_MAC_PPC
 #pragma unused(ss)
#else /* not FOR_MAC_PPC */
 #pragma unused(ss, argc, argv)
#endif /* not FOR_MAC_PPC */
  DoPluginPtr xx = NULL_PTR;

  EnterCallback();
  LOG_ENTER()
#if FOR_MAC_PPC
  if (gPluginFolderFound)
  {
    xx = static_cast<DoPluginPtr>(newobject(gClass));
    if (xx)
    {
      bool    failed = false;
      OSErr   myErr = noErr;
      PSymbol aName;

      xx->fActivePlugin = NULL_PTR;
      xx->fPrivate = NULL_PTR;
      xx->fOutlets = NULL_HDL;
      xx->fProxies = NULL_HDL;
      xx->fNumInlets = xx->fNumOutlets = 0;
      xx->fInletNumber = 0;
      memset(&xx->fOwnerInfo, 0, sizeof(OwnerData));
      if (! argc)
      {
        LOG_ERROR_1(OUTPUT_PREFIX "missing plügin name")
        failed = true;
      }
      else if (argv[0].a_type != A_SYM)
      {
        LOG_ERROR_1(OUTPUT_PREFIX "plügin name is not a symbol")
        failed = true;
      }
      else if (argv[0].a_w.w_sym == gEmptySymbol)
      {
        LOG_ERROR_1(OUTPUT_PREFIX "missing plügin name")
        failed = true;
      }
      else
      {
        aName = argv[0].a_w.w_sym;
        rescanPlugins();
        if (gPluginAnchor)
        {
          /* locate initial plugin reference */
          PluginDescriptorPtr descriptor = gPluginAnchor;

          for ( ; descriptor; descriptor = descriptor->fNext)
          {
            if (descriptor->fName == aName)
            {
              xx->fActivePlugin = descriptor;
              ++descriptor->fRefCount;
              break;

            }            			
          }
          if (xx->fActivePlugin)
          {
            xx->fOwnerInfo.fMaxObject = xx;
            xx->fOwnerInfo.fRegistry = xx->fActivePlugin->fRegistry;
            /* Invoke the onCreate() routine */
#if defined(COMPILE_FOR_OSX_4)
            myErr = reinterpret_cast<FpOnCreate>(descriptor->fOnCreateFun)
                          (&xx->fOwnerInfo, descriptor->fName->s_name,
                          descriptor->fShared, &xx->fPrivate, short(argc - 1),
                          argv + 1, &xx->fNumInlets, &xx->fNumOutlets);
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
            myErr = static_cast<OSErr>(CallUniversalProc(descriptor->fOnCreateUpp, uppOnCreateProcInfo,
            																&xx->fOwnerInfo, descriptor->fName->s_name, descriptor->fShared,
            																&xx->fPrivate, short(argc - 1), argv + 1, &xx->fNumInlets,
            																&xx->fNumOutlets));
#endif /* COMPILE_FOR_OS9_4 */
            if (myErr == noErr)
            {
              if ((xx->fNumInlets < 1) || (xx->fNumOutlets < 1))
              {
                LOG_ERROR_1(OUTPUT_PREFIX "bad number of inlets or outlets from plügin")
                failed = true;
              }
              else
              {
                short pMax = short(xx->fNumInlets - 1);

                xx->fOutlets = GETBYTES(xx->fNumOutlets, POutlet);
                if (xx->fOutlets)
                  memset(xx->fOutlets, 0, (xx->fNumOutlets * sizeof(POutlet)));
                if (xx->fNumInlets > 1)
                {
                  xx->fProxies = GETBYTES(pMax, Pvoid);
                  if (xx->fProxies)
                    memset(xx->fProxies, 0, (pMax * sizeof(Pvoid)));
                }
                if (xx->fOutlets && (xx->fProxies || (xx->fNumInlets == 1)))
                {
                  for (short ii = 0; (ii < xx->fNumOutlets) && (! failed); ++ii)
                  {
                    long  tag = xx->fNumOutlets - (ii + 1);
                    Pvoid temp = outlet_new(xx, NULL_PTR);

                    if (temp)
                      *(xx->fOutlets + tag) = static_cast<POutlet>(temp);
                    else
                    {
                      LOG_ERROR_1(OUTPUT_PREFIX "problem allocating outlet for plügin")
                      failed = true;
                    }
                  }
                  for (short ii = 0; (ii < pMax) && (! failed); ++ii)
                  {
                    long  tag = pMax - ii;
                    Pvoid temp = proxy_new(xx, tag, &xx->fInletNumber);

                    if (temp)
                      *(xx->fProxies + tag - 1) = temp;
                    else
                    {
                      LOG_ERROR_1(OUTPUT_PREFIX "problem allocating inlet for plügin")
                      failed = true;
                    }
                  }
                }
                else
                {
                  LOG_ERROR_1(OUTPUT_PREFIX "unable to allocate storage for plügin")
                  failed = true;
                }
              }
            }
            else
            {
              LOG_ERROR_2(OUTPUT_PREFIX "problem running onCreate() for plügin: %d", myErr)
              failed = true;
            }
          }
          else
          {
            LOG_ERROR_2(OUTPUT_PREFIX "matching plügin (%s) not found", aName->s_name)
            failed = true;
          }
        }
        else
        {
          LOG_ERROR_1(OUTPUT_PREFIX "no plügins in folder")
          failed = true;
        }
      }
      if (failed)
      {
        freeobject(reinterpret_cast<PObject>(xx));
        xx = NULL_PTR;
      }
      else
        ++gObjectCount;
    }
  }
#endif /* FOR_MAC_PPC */
  ExitCodeResource();
  return xx;
} /* doPluginCreate */

/*------------------------------------ doPluginFree ---*/
Pvoid doPluginFree
  (DoPluginPtr xx)
{
  EnterCallback();
  if (xx)
  {
#if FOR_MAC_PPC
    if (xx->fActivePlugin)
    {
      PluginDescriptorPtr descriptor = xx->fActivePlugin;

      /* Invoke the onDestroy() routine */
#if defined(COMPILE_FOR_OSX_4)
      OSErr myErr = reinterpret_cast<FpOnDestroy>(descriptor->fOnDestroyFun)
                          (&xx->fOwnerInfo, descriptor->fShared, xx->fPrivate);
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
      OSErr myErr = static_cast<OSErr>(CallUniversalProc(descriptor->fOnDestroyUpp, uppOnDestroyProcInfo,
      																			&xx->fOwnerInfo, descriptor->fShared, xx->fPrivate));
#endif /* COMPILE_FOR_OS9_4 */

      if (myErr != noErr)
        LOG_ERROR_2(OUTPUT_PREFIX "problem running onDestroy() for plügin: %d", myErr)
      --descriptor->fRefCount;
    }
    if ((xx->fNumInlets > 1) && xx->fProxies)
    {
      FREEBYTES(xx->fProxies, xx->fNumOutlets - 1)
      xx->fProxies = NULL_HDL;
    }
    if (! --gObjectCount)
    {
      /* Walk down the descriptor list, disconnecting */
      for (PluginDescriptorPtr walker = gPluginAnchor, next; walker; walker = next)
      {
#if defined(COMPILE_FOR_OSX_4)
        OSErr	myErr = reinterpret_cast<FpNiam>(walker->fNiamFun)
                                          (&xx->fOwnerInfo, walker->fShared);
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
        OSErr	myErr = static_cast<OSErr>(CallUniversalProc(walker->fNiamUpp,
        																										uppNiamProcInfo, &xx->fOwnerInfo,
                                                            walker->fShared));
#endif /* COMPILE_FOR_OS9_4 */

        next = walker->fNext;
        if (myErr != noErr)
          LOG_ERROR_2(OUTPUT_PREFIX "problem running niam() for plügin: %d", myErr)
        freeRoutineDescriptors(walker);
        freeRegistry(walker->fRegistry);
        FREEBYTES(walker, 1)
        walker = next;
      }
      gPluginAnchor = NULL_PTR;
    }
#endif /* FOR_MAC_PPC */
  }
  LOG_EXIT()
  ExitMaxMessageHandler()
} /* doPluginFree */

/*------------------------------------ checkForPluginsFolder ---*/
bool checkForPluginsFolder
  (const short	volumeID,
   const long		dirID)
{
#if (! FOR_MAC_PPC)
 #pragma unused(volumeID, dirID)
#endif /* not FOR_MAC_PPC */
#if FOR_MAC_PPC
  Boolean   targetIsFolder, wasAliased;
  bool      foundFolder = false;
  HFileInfo myCPB;
  OSErr	    myErr = noErr, aliasErr = noErr;
  FSSpec		newFSS;
  short	    index;
  long			workingDir = dirID;

  myCPB.ioNamePtr = reinterpret_cast<StringPtr>(NewPtrClear(40L));
  index = 1;
  myCPB.ioVRefNum = volumeID;	// other volume support
  while (myErr == noErr) 
  {
    myCPB.ioFDirIndex = index;
    myCPB.ioNamePtr[0] = 0;
    myCPB.ioDirID = workingDir;
    myErr = PBGetCatInfo(reinterpret_cast<CInfoPBPtr>(&myCPB), false);
    if (myErr == noErr) 
    {
      myErr = FSMakeFSSpec(myCPB.ioVRefNum, workingDir, myCPB.ioNamePtr, &newFSS);
      if (myErr != noErr)
      {
        LOG_ERROR_2(OUTPUT_PREFIX "bad plügins folder spec: %d", myErr)
        return false;

      }
      // aliasErr, we don't want the while loop to abort because of a 
      // bad alias. We don't care about a  bad alias.
      aliasErr = ResolveAliasFile(&newFSS, true, &targetIsFolder, &wasAliased);
      if ((aliasErr == userCanceledErr) || (aliasErr == fnfErr))
      {
      // do nothing
      }
      else if (wasAliased && (compareStrings(myCPB.ioNamePtr, gPluginFolderName) ||
                  compareStrings(myCPB.ioNamePtr, gPluginFolderAlias)))
      {
        workingDir = newFSS.parID;	// reset dir to aliases parent directory
        index = 0;	// reset index to search aliases parent directory
        // Note: index will get incremented below
        myCPB.ioVRefNum = newFSS.vRefNum;	// set volume (might not be
                                          //	default vol)
      }
      else
      {
        if (myCPB.ioFlAttrib & ioDirMask)
        {
          if (compareStrings(myCPB.ioNamePtr, gPluginFolderName)) 
          {
            foundFolder = true;
            gPluginDir = myCPB.ioDirID;
            gPluginVol = myCPB.ioVRefNum;
            break;		// Found the folder, so get out of this...

          }					
        }
      }
    }
    ++index;
  }
  DisposePtr(reinterpret_cast<Ptr>(myCPB.ioNamePtr));
  myCPB.ioNamePtr = 0;
  return foundFolder;
#else /* not FOR_MAC_PPC */
  return false;
#endif /* not FOR_MAC_PPC */
} /* checkForPluginsFolder */

/*------------------------------------ compareStrings ---*/
bool compareStrings
  (Puchar str1,
   Puchar str2)
{
  if (! (str1 && str2))
    return false;

  return EqualString(static_cast<ConstStr255Param>(str1),
                    static_cast<ConstStr255Param>(str2), false, false);
} /* compareStrings */

/*------------------------------------ freeRoutineDescriptors ---*/
void freeRoutineDescriptors
  (PluginDescriptorPtr descriptor)
{
#if (! FOR_MAC_PPC)
 #pragma unused(descriptor)
#endif /* not FOR_MAC_PPC */
#if FOR_MAC_PPC
 #if defined(COMPILE_FOR_OS9_4)
  long routineMask = descriptor->fValidMask;

  if (routineMask & kDoAnythingValid)
  {
    DisposeRoutineDescriptor(descriptor->fDoAnythingUpp);
    descriptor->fDoAnythingUpp = NULL_PTR;
  }
  if (routineMask & kDoBangValid)
  {
    DisposeRoutineDescriptor(descriptor->fDoBangUpp);   
    descriptor->fDoBangUpp = NULL_PTR;
  }
  if (routineMask & kDoDoubleValid)
  {
    DisposeRoutineDescriptor(descriptor->fDoDoubleUpp);   
    descriptor->fDoDoubleUpp = NULL_PTR;
  }
  if (routineMask & kDoListValid)
  {
    DisposeRoutineDescriptor(descriptor->fDoListUpp);   
    descriptor->fDoListUpp = NULL_PTR;
  }
  if (routineMask & kDoLongValid)
  {
    DisposeRoutineDescriptor(descriptor->fDoLongUpp);   
    descriptor->fDoLongUpp = NULL_PTR;
  }
  if (routineMask & kMainValid)
  {
    DisposeRoutineDescriptor(descriptor->fMainUpp);   
    descriptor->fMainUpp = NULL_PTR;
  }
  if (routineMask & kNiamValid)
  {
    DisposeRoutineDescriptor(descriptor->fNiamUpp);   
    descriptor->fNiamUpp = NULL_PTR;
  }
  if (routineMask & kOnCreateValid)
  {
    DisposeRoutineDescriptor(descriptor->fOnCreateUpp);   
    descriptor->fOnCreateUpp = NULL_PTR;
  }
  if (routineMask & kOnDestroyValid)
  {
    DisposeRoutineDescriptor(descriptor->fOnDestroyUpp);   
    descriptor->fOnDestroyUpp = NULL_PTR;
  }
  if (routineMask & kOnReloadValid)
  {
    DisposeRoutineDescriptor(descriptor->fOnReloadUpp);   
    descriptor->fOnReloadUpp = NULL_PTR;
  }
 #endif /* COMPILE_FOR_OS9_4 */
  descriptor->fValidMask = 0;
  CloseConnection(&descriptor->fConnID);
  #endif /* FOR_MAC_PPC */
} /* freeRoutineDescriptors */

/*------------------------------------ getAppDir ---*/
OSErr getAppDir
  (FSSpec * pAppFile)
{
#if (! FOR_MAC_PPC)
 #pragma unused(pAppFile)
#endif /* not FOR_MAC_PPC */
#if FOR_MAC_PPC
  ProcessInfoRec      myInfo;
  ProcessSerialNumber myPSN;
  OSErr               myErr = GetCurrentProcess(&myPSN);

  if (myErr == noErr)
  {
    myInfo.processInfoLength = sizeof(ProcessInfoRec);
    myInfo.processName = 0L;
    myInfo.processAppSpec = pAppFile;
    myErr = GetProcessInformation(&myPSN, &myInfo);
  }
  else
    LOG_ERROR_2(OUTPUT_PREFIX "problem getting current process: %d", myErr)
  return myErr;
#else /* not FOR_MAC_PPC */
  return noErr;
#endif /* not FOR_MAC_PPC */
} /* getAppDir */

#if defined(COMPILE_FOR_OSX_4)
/*------------------------------------ getEntryPoint ---*/
OSErr getEntryPoint
  (CFragConnectionID  connID,
   Str255             name,
   ProcPtr *          newFun)
{
 #if (! FOR_MAC_PPC)
  #pragma unused(connID, name, procInfo, newUpp)
 #endif /* not FOR_MAC_PPC */
 #if FOR_MAC_PPC
  OSErr            myErr = noErr;
  Ptr              entryAddress;
  CFragSymbolClass symClass;

  myErr = FindSymbol(connID, name, &entryAddress, &symClass);
  if (myErr == noErr)
    *newFun = reinterpret_cast<ProcPtr>(entryAddress);
  else
  {
    char  tempString[256];
    
    CopyPascalStringToC(name, tempString);
    LOG_ERROR_2(OUTPUT_PREFIX "couldn't locate symbol: %s", tempString)
  }    
  return myErr;
 #else /* not FOR_MAC_PPC */
  return noErr;
 #endif /* not FOR_MAC_PPC */
} /* getEntryPoint */
#endif /* COMPILE_FOR_OSX_4 */

#if defined(COMPILE_FOR_OS9_4)
/*------------------------------------ getEntryPoint ---*/
OSErr getEntryPoint
  (CFragConnectionID  connID,
   Str255             name,
   ProcInfoType       procInfo,
   UniversalProcPtr * newUpp)
{
 #if (! FOR_MAC_PPC)
  #pragma unused(connID, name, procInfo, newUpp)
 #endif /* not FOR_MAC_PPC */
 #if FOR_MAC_PPC
  OSErr            myErr = noErr;
  Ptr              entryAddress;
  CFragSymbolClass symClass;

  myErr = FindSymbol(connID, name, &entryAddress, &symClass);
  if (myErr == noErr)
  {
    *newUpp = NewRoutineDescriptor(reinterpret_cast<ProcPtr>(entryAddress),
                                  procInfo, GetCurrentISA());                                       
    if (! *newUpp)
      myErr = memFullErr;
  }
  else
  {
    p2cstr(name);
    LOG_ERROR_2(OUTPUT_PREFIX "couldn't locate symbol: %s", name)
    c2pstr(reinterpret_cast<Pchar>(name));
  }    
  return myErr;
 #else /* not FOR_MAC_PPC */
  return noErr;
 #endif /* not FOR_MAC_PPC */
} /* getEntryPoint */
#endif /* COMPILE_FOR_OS9_4 */

#if defined(OPTIONAL_FUNCTIONS)
/*------------------------------------ getFullPath ---*/
OSErr getFullPath
  (const FSSpec * spec,
   long &         fullPathLength,
   Handle &       fullPath)
{
 #if (! FOR_MAC_PPC)
  #pragma unused(spec, fullPathLength, fullPath)
 #endif /* not FOR_MAC_PPC */
 #if FOR_MAC_PPC
  OSErr      result = noErr, realResult = noErr;
  FSSpec     tempSpec;
  CInfoPBRec pb;

  fullPathLength = 0;
  fullPath = NULL_HDL;

  /* work around Nav Services "bug" (it returns invalid FSSpecs with empty names) */
  if (! *spec->name)
    result = makeFSSpecCompat(spec->vRefNum, spec->parID, spec->name, &tempSpec);
  else
    /* Make a copy of the input FSSpec that can be modified */
    BlockMoveData(spec, &tempSpec, sizeof(FSSpec));
  if (result == noErr)
  {
    if (tempSpec.parID == fsRtParID)
    {
      /* The object is a volume */

      /* Add a colon to make it a full pathname */
      ++*tempSpec.name;
      tempSpec.name[*tempSpec.name] = ':';

      /* We're done */
      result = PtrToHand(&tempSpec.name[1], &fullPath, *tempSpec.name);
    }
    else
    {
      /* The object isn't a volume */

      /* Is the object a file or a directory? */
      pb.dirInfo.ioNamePtr = tempSpec.name;
      pb.dirInfo.ioVRefNum = tempSpec.vRefNum;
      pb.dirInfo.ioDrDirID = tempSpec.parID;
      pb.dirInfo.ioFDirIndex = 0;
      result = PBGetCatInfoSync(&pb);
      // Allow file/directory name at end of path to not exist.
      realResult = result;
      if ((result == noErr) || (result == fnfErr))
      {
        /* if the object is a directory, append a colon so full pathname ends with colon */
        if ((result == noErr) && ((pb.hFileInfo.ioFlAttrib & kioFlAttribDirMask) != 0))
        {
          ++*tempSpec.name;
          tempSpec.name[*tempSpec.name] = ':';
        }

        /* Put the object name in first */
        result = PtrToHand(&tempSpec.name[1], &fullPath, *tempSpec.name);
        if (result == noErr)
        {
          /* Get the ancestor directory names */
          pb.dirInfo.ioNamePtr = tempSpec.name;
          pb.dirInfo.ioVRefNum = tempSpec.vRefNum;
          pb.dirInfo.ioDrParID = tempSpec.parID;
          do	/* loop until we have an error or find the root directory */
          {
            pb.dirInfo.ioFDirIndex = -1;
            pb.dirInfo.ioDrDirID = pb.dirInfo.ioDrParID;
            result = PBGetCatInfoSync(&pb);
            if (result == noErr)
            {
              /* Append colon to directory name */
              ++*tempSpec.name;
              tempSpec.name[*tempSpec.name] = ':';

              /* Add directory name to beginning of fullPath */
              Munger(fullPath, 0, NULL_PTR, 0, &tempSpec.name[1], *tempSpec.name);
              result = MemError();
            }
          }
          while ((result == noErr) && (pb.dirInfo.ioDrDirID != fsRtDirID));
        }
      }
    }
  }
  if (result == noErr)
  {
    /* Return the length */
    fullPathLength = GetHandleSize(fullPath);
    result = realResult;	// return realResult in case it was fnfErr
  }
  else
  {
    /* Dispose of the handle and return NULL and zero length */
    if (fullPath)
      DisposeHandle(fullPath);
    fullPath = NULL_HDL;
    fullPathLength = 0;
  }
  return result;
 #else /* not FOR_MAC_PPC */
  return noErr;
 #endif /* not FOR_MAC_PPC */
} /* getFullPath */
#endif // OPTIONAL_FUNCTIONS

/*------------------------------------ initPluginList ---*/
bool initPluginList
  (void)
{
#if FOR_MAC_PPC	
  OSErr             myErr = noErr;
  bool              result = true;
  Str255            errName;
  CFragConnectionID connID;
  Ptr               mainAddr;

  gPluginVol = 0;
  /* Get the shared library: */
  myErr = GetSharedLibrary("\pPluginsReal.lib", kCompiledCFragArch, kPrivateCFragCopy, &connID,
                            &mainAddr, errName);
  if (myErr == noErr)
  {
    if (! checkForPluginsFolder(gAppVRef, gAppDir))
    {
      if (! checkForPluginsFolder(gSysVRef, gSysParID))
      {
        LOG_ERROR_1(OUTPUT_PREFIX "couldn't locate Plügins folder")
        result = false;
      }
    }
  }
  else
  {
 #if defined(COMPILE_FOR_OSX_4)
    char  tempString[256];
    
    CopyPascalStringToC(errName, tempString);
    LOG_ERROR_3(OUTPUT_PREFIX "loading problem: %d, name: '%s'", myErr, tempString)
 #endif /* COMPILE_FOR_OSX_4 */
 #if defined(COMPILE_FOR_OS9_4)
    p2cstr(errName);
    LOG_ERROR_3(OUTPUT_PREFIX "loading problem: %d, name: '%s'", myErr,
                  reinterpret_cast<Pchar>(errName))
    c2pstr(reinterpret_cast<Pchar>(errName));
 #endif /* COMPILE_FOR_OS9_4 */
    result = false;
  }
  return result;
#else /* not FOR_MAC_PPC */
  return false;
#endif /* not FOR_MAC_PPC */
} /* initPluginList */

#if defined(OPTIONAL_FUNCTIONS)
/*------------------------------------ makeFSSpecCompat ---*/
OSErr makeFSSpecCompat
  (const short			vRefNum,
   const long       dirID,
   ConstStr255Param fileName,
   FSSpec *         spec)
{
 #if (! FOR_MAC_PPC)
  #pragma unused(vRefNum, dirID, fileName, spec)
 #endif /* not FOR_MAC_PPC */
 #if FOR_MAC_PPC
  /* Let the file system create the FSSpec if it can since it does the job */
  /* much more efficiently than I can. */
  OSErr result = FSMakeFSSpec(vRefNum, dirID, fileName, spec);

  /* Fix a bug in Macintosh PC Exchange's MakeFSSpec code where 0 is */
  /* returned in the parID field when making an FSSpec to the volume's */
  /* root directory by passing a full pathname in MakeFSSpec's */
  /* fileName parameter. Fixed in Mac OS 8.1 */
  if ((result == noErr) && (spec->parID == 0))
    spec->parID = fsRtParID;
  return result;
 #else /* not FOR_MAC_PPC */
  return noErr;
 #endif /* not FOR_MAC_PPC */
} /* makeFSSpecCompat */
#endif // OPTIONAL_FUNCTIONS

/*------------------------------------ refreshDescriptors ---*/
void refreshDescriptors
  (DoPluginPtr xx)
{
  if (xx)
  {
#if FOR_MAC_PPC
    PluginDescriptorPtr descriptor = xx->fActivePlugin;

    if (descriptor)
    {
      Str255 tempName;

 #if defined(COMPILE_FOR_OSX_4)
      CopyCStringToPascal(descriptor->fName->s_name, tempName);
 #endif /* COMPILE_FOR_OSX_4 */
 #if defined(COMPILE_FOR_OS9_4)
      strcpy(reinterpret_cast<Pchar>(tempName), descriptor->fName->s_name);
      c2pstr(reinterpret_cast<Pchar>(tempName));
 #endif /* COMPILE_FOR_OS9_4 */
      freeRoutineDescriptors(descriptor);
      flushRegistry(descriptor->fRegistry);
      if (validateEntryPoints(descriptor, &descriptor->fSpec, tempName))
      {
#if defined(COMPILE_FOR_OSX_4)
        OSErr myErr = reinterpret_cast<FpOnReload>(descriptor->fOnReloadFun)
                            (descriptor->fConnID, &xx->fOwnerInfo, descriptor->fShared);
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
        OSErr myErr = static_cast<OSErr>(CallUniversalProc(descriptor->fOnReloadUpp, uppOnReloadProcInfo,
                                              descriptor->fConnID, &xx->fOwnerInfo,
                                              descriptor->fShared));
#endif /* COMPILE_FOR_OS9_4 */

        if (myErr != noErr)
          LOG_ERROR_2(OUTPUT_PREFIX "problem running onReload() for plügin: %d", myErr)
      }
      else
        LOG_ERROR_2("couldn't refresh the entry points for '%s'", descriptor->fName->s_name)
    }
#endif /* FOR_MAC_PPC */
  }
} /* refreshDescriptors */

/*------------------------------------ rescanPlugins ---*/
void rescanPlugins
  (void)
{
#if FOR_MAC_PPC
  Boolean   targetIsFolder, wasAliased;
  HFileInfo myCPB;
  FSSpec    newFSS;
  short     index = 1;
  OSErr     myErr = noErr;

  // Begin to search for the plugins in the current (dirID) folder...
  myCPB.ioNamePtr = reinterpret_cast<StringPtr>(NewPtrClear(40L));
  while (myErr == noErr) 
  {
    myCPB.ioVRefNum = gPluginVol;
    myCPB.ioFDirIndex = index++;
    myCPB.ioNamePtr[0] = 0;
    myCPB.ioDirID = gPluginDir;
    myErr = PBGetCatInfo(reinterpret_cast<CInfoPBPtr>(&myCPB), false);
    if (myErr == noErr) 
    {
      if (((myCPB.ioFlFndrInfo.fdType == PLUGIN_KIND)) || (myCPB.ioFlAttrib & ioDirMask))
      {
        /* Determine if this plügin is already known: */
        PluginDescriptorPtr walker = gPluginAnchor;
        bool                found = false;
        PSymbol             aName;

 #if defined(COMPILE_FOR_OSX_4)
        char                tempString[256];
        
        CopyPascalStringToC(myCPB.ioNamePtr, tempString);
        aName = gensym(tempString);
 #endif /* COMPILE_FOR_OSX_4 */
 #if defined(COMPILE_FOR_OS9_4)
        aName = gensym(p2cstr(myCPB.ioNamePtr));
        c2pstr(reinterpret_cast<Pchar>(myCPB.ioNamePtr));
 #endif /* COMPILE_FOR_OS9_4 */
        for ( ; walker; walker = walker->fNext)
        {
          if (walker->fName == aName)
          {
            found = true;
            break;

          }
        }
        myErr = FSMakeFSSpec(gPluginVol, gPluginDir, myCPB.ioNamePtr, &newFSS);
        if (myErr != noErr)
        {
          LOG_ERROR_2(OUTPUT_PREFIX "bad plügin file spec: %d", myErr)
          break;

        }				
        myErr = ResolveAliasFile(&newFSS, true, &targetIsFolder, &wasAliased);
        if (myErr != noErr)
        {
          LOG_ERROR_2(OUTPUT_PREFIX "bad plügin file alias: %d", myErr)
          break;

        }				
        if (! (myCPB.ioFlAttrib & ioDirMask))
        {	    
          if (found)
          {
            freeRoutineDescriptors(walker);
            if (validateEntryPoints(walker, &newFSS, myCPB.ioNamePtr))
              memcpy(&walker->fSpec, &newFSS, sizeof(newFSS));
          }
          else
          {
            PluginDescriptorPtr descriptor = GETBYTES(1, PluginDescriptor);

            if (validateEntryPoints(descriptor, &newFSS, myCPB.ioNamePtr))
            {
              OwnerData transientStuff;

              memset(&transientStuff, 0, sizeof(transientStuff));
              descriptor->fNext = gPluginAnchor;
              descriptor->fPrevious = NULL_PTR;
              if (gPluginAnchor)
                gPluginAnchor->fPrevious = descriptor;
              gPluginAnchor = descriptor;
              descriptor->fName = aName;
              descriptor->fShared = NULL_PTR;
              descriptor->fRefCount = 0;
              memcpy(&descriptor->fSpec, &newFSS, sizeof(newFSS));
              transientStuff.fRegistry = descriptor->fRegistry = createRegistry();
              /* Execute the main entry point: */
#if defined(COMPILE_FOR_OSX_4)
              myErr = reinterpret_cast<FpMain>(descriptor->fMainFun)
                            (descriptor->fConnID, &transientStuff, &descriptor->fShared);
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
              myErr = static_cast<OSErr>(CallUniversalProc(descriptor->fMainUpp, uppMainProcInfo,
                                              descriptor->fConnID, &transientStuff,
                                              &descriptor->fShared));
#endif /* COMPILE_FOR_OS9_4 */
              if (myErr != noErr)
              {
                LOG_ERROR_2(OUTPUT_PREFIX "problem running main() for plügin: %d", myErr)
                break;

              }    	
            }
          }						
        }
      }
    }
  }
  DisposePtr(reinterpret_cast<Ptr>(myCPB.ioNamePtr));
#endif /* FOR_MAC_PPC */
} /* rescanPlugins */

/*------------------------------------ validateEntryPoints ---*/
bool validateEntryPoints
  (PluginDescriptorPtr descriptor,
   FSSpec *            anFSS,
   Puchar              name)
{
#if (! FOR_MAC_PPC)
 #pragma unused(descriptor, anFSS, name)
#endif /* not FOR_MAC_PPC */
#if FOR_MAC_PPC
  Str255 errName;
  long   routineMask = 0;
  OSErr  myErr = GetDiskFragment(anFSS, 0, kCFragGoesToEOF, name, kPrivateCFragCopy,
                                  &descriptor->fConnID, &descriptor->fMainAddress, errName);

  if (myErr == noErr)
  {
 #if defined(COMPILE_FOR_OSX_4)
    descriptor->fMainFun = reinterpret_cast<ProcPtr>(descriptor->fMainAddress);
    if (! descriptor->fMainFun)
 #endif /* COMPILE_FOR_OSX_4 */
 #if defined(COMPILE_FOR_OS9_4)
    descriptor->fMainUpp = NewRoutineDescriptor(reinterpret_cast<ProcPtr>(descriptor->fMainAddress), uppMainProcInfo,
                                                GetCurrentISA());                                       
    if (! descriptor->fMainUpp)
 #endif /* COMPILE_FOR_OS9_4 */
      myErr = memFullErr;
  }
  if (myErr == noErr)
  {
    routineMask |= kMainValid;        
 #if defined(COMPILE_FOR_OSX_4)
    myErr = getEntryPoint(descriptor->fConnID, "\pdo_Anything", &descriptor->fDoAnythingFun);
 #endif /* COMPILE_FOR_OSX_4 */
 #if defined(COMPILE_FOR_OS9_4)
    myErr = getEntryPoint(descriptor->fConnID, "\pdo_Anything", uppDoAnythingProcInfo, &descriptor->fDoAnythingUpp);
 #endif /* COMPILE_FOR_OS9_4 */
  }
  if (myErr == noErr)
  {
    routineMask |= kDoAnythingValid;        
 #if defined(COMPILE_FOR_OSX_4)
    myErr = getEntryPoint(descriptor->fConnID, "\pdo_Bang", &descriptor->fDoBangFun);
 #endif /* COMPILE_FOR_OSX_4 */
 #if defined(COMPILE_FOR_OS9_4)
    myErr = getEntryPoint(descriptor->fConnID, "\pdo_Bang", uppDoBangProcInfo, &descriptor->fDoBangUpp);
 #endif /* COMPILE_FOR_OS9_4 */
  }
  if (myErr == noErr)
  {
    routineMask |= kDoBangValid;        
 #if defined(COMPILE_FOR_OSX_4)
    myErr = getEntryPoint(descriptor->fConnID, "\pdo_Double", &descriptor->fDoDoubleFun);
 #endif /* COMPILE_FOR_OSX_4 */
 #if defined(COMPILE_FOR_OS9_4)
    myErr = getEntryPoint(descriptor->fConnID, "\pdo_Double", uppDoDoubleProcInfo, &descriptor->fDoDoubleUpp);
 #endif /* COMPILE_FOR_OS9_4 */
  }
  if (myErr == noErr)
  {
    routineMask |= kDoDoubleValid;        
 #if defined(COMPILE_FOR_OSX_4)
    myErr = getEntryPoint(descriptor->fConnID, "\pdo_List", &descriptor->fDoListFun);
 #endif /* COMPILE_FOR_OSX_4 */
 #if defined(COMPILE_FOR_OS9_4)
    myErr = getEntryPoint(descriptor->fConnID, "\pdo_List", uppDoListProcInfo, &descriptor->fDoListUpp);
 #endif /* COMPILE_FOR_OS9_4 */
  }
  if (myErr == noErr)
  {
    routineMask |= kDoListValid;        
 #if defined(COMPILE_FOR_OSX_4)
    myErr = getEntryPoint(descriptor->fConnID, "\pdo_Long", &descriptor->fDoLongFun);
 #endif /* COMPILE_FOR_OSX_4 */
 #if defined(COMPILE_FOR_OS9_4)
    myErr = getEntryPoint(descriptor->fConnID, "\pdo_Long", uppDoLongProcInfo, &descriptor->fDoLongUpp);
 #endif /* COMPILE_FOR_OS9_4 */
  }
  if (myErr == noErr)
  {
    routineMask |= kDoLongValid;        
 #if defined(COMPILE_FOR_OSX_4)
    myErr = getEntryPoint(descriptor->fConnID, "\pniam", &descriptor->fNiamFun);
 #endif /* COMPILE_FOR_OSX_4 */
 #if defined(COMPILE_FOR_OS9_4)
    myErr = getEntryPoint(descriptor->fConnID, "\pniam", uppNiamProcInfo, &descriptor->fNiamUpp);
 #endif /* COMPILE_FOR_OS9_4 */
  }
  if (myErr == noErr)
  {
    routineMask |= kNiamValid;        
 #if defined(COMPILE_FOR_OSX_4)
    myErr = getEntryPoint(descriptor->fConnID, "\ponCreate", &descriptor->fOnCreateFun);
 #endif /* COMPILE_FOR_OSX_4 */
 #if defined(COMPILE_FOR_OS9_4)
    myErr = getEntryPoint(descriptor->fConnID, "\ponCreate", uppOnCreateProcInfo, &descriptor->fOnCreateUpp);
 #endif /* COMPILE_FOR_OS9_4 */
  }
  if (myErr == noErr)
  {
    routineMask |= kOnCreateValid;
 #if defined(COMPILE_FOR_OSX_4)
    myErr = getEntryPoint(descriptor->fConnID, "\ponDestroy", &descriptor->fOnDestroyFun);
 #endif /* COMPILE_FOR_OSX_4 */
 #if defined(COMPILE_FOR_OS9_4)
    myErr = getEntryPoint(descriptor->fConnID, "\ponDestroy", uppOnDestroyProcInfo, &descriptor->fOnDestroyUpp);
 #endif /* COMPILE_FOR_OS9_4 */
  }
  if (myErr == noErr)
  {
    routineMask |= kOnDestroyValid;
 #if defined(COMPILE_FOR_OSX_4)
    myErr = getEntryPoint(descriptor->fConnID, "\ponReload", &descriptor->fOnReloadFun);
 #endif /* COMPILE_FOR_OSX_4 */
 #if defined(COMPILE_FOR_OS9_4)
    myErr = getEntryPoint(descriptor->fConnID, "\ponReload", uppOnReloadProcInfo, &descriptor->fOnReloadUpp);
 #endif /* COMPILE_FOR_OS9_4 */
  }
  if (myErr == noErr)
    routineMask |= kOnReloadValid;
  descriptor->fValidMask = routineMask;
  if (myErr != noErr)
  {
 #if defined(COMPILE_FOR_OSX_4)
    char  tempString[256];
    
    CopyPascalStringToC(errName, tempString);
    LOG_ERROR_3(OUTPUT_PREFIX "loading problem: %d, name: '%s'", myErr, tempString)
 #endif /* COMPILE_FOR_OSX_4 */
 #if defined(COMPILE_FOR_OS9_4)
    p2cstr(errName);
    LOG_ERROR_3(OUTPUT_PREFIX "loading problem: %d, name: '%s'", myErr,
                reinterpret_cast<Pchar>(errName))
    c2pstr(reinterpret_cast<Pchar>(errName));
 #endif /* COMPILE_FOR_OS9_4 */
    /* Clear out routine descriptors */
    freeRoutineDescriptors(descriptor);
    return false;

  }
  return true;
#else /* not FOR_MAC_PPC */
  return false;
#endif /* not FOR_MAC_PPC */
} /* validateEntryPoints */	              		                        

StandardInfoRoutine(DoPluginPtr)
