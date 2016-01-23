/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       fidget.h                                                                */
/*                                                                                      */
/*  Contains:   The data type declarations and forward references to routines.          */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2003 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2003/11/08                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(FIDGET_H_))
 #define FIDGET_H_  /* */
 
 #define USE_SYSLOG /* */

 #include "MissingAndExtra.h"
 #include "Common_HIDX.h"
 #include "PhidgetEntryPoints.h"

 #define OUR_NAME         "fidget"
 #define OUR_RES_NUMB     17199
 #define OUTPUT_PREFIX    "fidget: "

 #define FIDGET_CONTROL_SYMBOL "##fidget##"

enum 
{
  kDefineCallbackValid = (1 << 0),
  kDoCustomValid = (1 << 1),
  kDoGetValid = (1 << 2),
  kDoPutValid = (1 << 3),
  kIdentifyValid = (1 << 4),
  kMainValid = (1 << 5),
  kNiamValid = (1 << 6),
  kOnAttachValid = (1 << 7),
  kOnDetachValid = (1 << 8),
  kReportHandlerValid = (1 << 9)
};

struct PhidgetDescriptor;

typedef PhidgetDescriptor *    PhidgetDescriptorPtr;

struct PhidgetDescriptor
{
  CFragConnectionID       fConnID;
  Ptr                     fMainAddress;
  PSymbol                 fName;
  PhidgetDescriptorPtr    fNext;
  PhidgetDescriptorPtr    fPrevious;
  long                    fPrivateSize;
  long                    fProductID;  
  Ptr                     fShared;
  long                    fSharedSize;
  FSSpec                  fSpec;
  bool                    fIsAsynchronous;
  long                    fValidMask;
 #if defined(COMPILE_FOR_OSX_4)
  ProcPtr                 fDefineCallbackFun;
  ProcPtr                 fDoCustomFun;
  ProcPtr                 fDoGetFun;
  ProcPtr                 fDoPutFun;
  ProcPtr                 fIdentifyFun;
  ProcPtr                 fMainFun;
  ProcPtr                 fNiamFun;
  ProcPtr                 fOnAttachFun;
  ProcPtr                 fOnDetachFun;
 #endif /* COMPILE_FOR_OSX_4 */
 #if defined(COMPILE_FOR_OS9_4)
  UniversalProcPtr        fDoCustomUpp;
  UniversalProcPtr        fDoGetUpp;
  UniversalProcPtr        fDoPutUpp;
  UniversalProcPtr        fIdentifyUpp;
  UniversalProcPtr        fMainUpp;
  UniversalProcPtr        fNiamUpp;
  UniversalProcPtr        fOnAttachUpp;
  UniversalProcPtr        fOnDetachUpp;
  UniversalProcPtr        fReportHandlerUpp;
 #endif /* COMPILE_FOR_OS9_4 */
}; /* PhidgetDescriptor */

struct FidgetData
{
  Object                  fObject;
  POutlet                 fDataOut;
  long                    fHIDDeviceCount;
  HIDDeviceDataPtr        fHIDDevices;
  PhidgetDescriptorPtr    fPhidgets;
  bool                    fReportEvents;
  POutlet                 fResultOut;
  bool                    fStopping;
 #if defined(COMPILE_FOR_OSX_4)
  IOKitContext            fHIDControl;
  IOKitContext            fUSBControl;
 #endif /* COMPILE_FOR_OSX_4 */
 #if defined(COMPILE_FOR_OS9_4)
  USBContext              fUSBControl;
 #endif /* COMPILE_FOR_OS9_4 */
}; /* FidgetData */

typedef FidgetData * FidgetPtr;

HIDDeviceDataPtr
fidgetGetFirstHIDData(FidgetPtr xx,
                      PSymbol   deviceType);
     
HIDDeviceDataPtr
fidgetGetNextHIDData(PSymbol          deviceType,
                     HIDDeviceDataPtr currentDevice);
     
HIDDeviceDataPtr
fidgetLocateHIDData(FidgetPtr xx,
                    PSymbol   deviceType,
                    PSymbol   serialNumber);

PSymbol
fidgetMapElementType(IOHIDElementType elementType);

Pvoid
cmd_Do(FidgetPtr xx,
       PSymbol   message,
       short     argc,
       PAtom     argv);  

Pvoid
cmd_Get(FidgetPtr xx,
        PSymbol   message,
        short     argc,
        PAtom     argv);
   
Pvoid
cmd_Listen(FidgetPtr xx,
           PSymbol   onOff);

Pvoid
cmd_Put(FidgetPtr xx,
        PSymbol   message,
        short     argc,
        PAtom     argv);

Pvoid
cmd_Report(FidgetPtr xx,
           PSymbol   deviceType,
           PSymbol   serialNumber,
           long      element);

Pvoid
cmd_Structure(FidgetPtr xx,
              PSymbol   deviceType,
              PSymbol   serialNumber);

StandardRoutineDeclarations(FidgetPtr)

mextern(long)       gAppDir;
mextern(FSSpec)     gAppFile;
mextern(short)      gAppVRef;
mextern(PSymbol)    gAddedSymbol;                        /* Pointer to unique symbol for 'added' */
mextern(PSymbol)    gAsteriskSymbol;                /* Pointer to unique symbol for '*' */
mextern(PSymbol)    gCollectionSymbol;            /* Pointer to unique symbol for 'collection' */
mextern(PSymbol)    gDeviceSymbol;                    /* Pointer to unique symbol for 'device' */
mextern(PSymbol)    gDevicesSymbol;                    /* Pointer to unique symbol for 'devices' */
mextern(PSymbol)    gElementSymbol;                    /* Pointer to unique symbol for 'element' */
mextern(PSymbol)    gElementsSymbol;                /* Pointer to unique symbol for 'elements' */
mextern(PSymbol)    gEmptySymbol;                        /* Pointer to unique symbol for '' */
mextern(PSymbol)    gFeatureSymbol;                    /* Pointer to unique symbol for 'feature' */
mextern(PSymbol)    gInputAxisSymbol;                /* Pointer to unique symbol for 'input-axis' */
mextern(PSymbol)    gInputButtonSymbol;            /* Pointer to unique symbol for 'input-button' */
mextern(PSymbol)    gInputMiscSymbol;                /* Pointer to unique symbol for 'input-misc' */
mextern(PSymbol)    gInputScanCodesSymbol;    /* Pointer to unique symbol for 'input-scancodes' */
mextern(PSymbol)    gOffSymbol;                            /* Pointer to unique symbol for 'off' */
mextern(PSymbol)    gOnSymbol;                            /* Pointer to unique symbol for 'on' */
mextern(PSymbol)    gOutputSymbol;                    /* Pointer to unique symbol for 'output' */
mextern(PSymbol)    gRemovedSymbol;                    /* Pointer to unique symbol for 'removed' */
mextern(long)       gPhidgetDir;
mextern(Str255)     gPhidgetFolderAlias;
mextern(Str255)     gPhidgetFolderName;
mextern(short)      gPhidgetVol;
mextern(long)       gSysParID;
mextern(short)      gSysVRef;
mextern(PSymbol)    gUnknownSymbol;                    /* Pointer to unique symbol for 'unknown' */

#endif /* not FIDGET_H_ */
