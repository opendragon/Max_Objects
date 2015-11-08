/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       PhidgetEntryPoints.h                                                    */
/*                                                                                      */
/*  Contains:   The data type declarations that are needed for standard phidgets.       */
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
/*  Created:    2003/12/19                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(PHIDGETENTRYPOINTS_H_))
 #define PHIDGETENTRYPOINTS_H_ /* */

 #include "Common_HIDX.h"
 #include "Common_USBX.h"

enum E_PhidgResult
{
    kPhidgSuccess,
    kPhidgCheckErrorValue,
    kPhidgDoDefault
}; /* E_PhidgResult */

struct PhidgEnvStruct
{
 #if COMPILE_FOR_OSX_4
    syslog_FP fSyslogFunction;
 #endif /* COMPILE_FOR_OSX_4 */
    bool filler; /* so that we always have something */
}; /* PhidgEnvStruct */

 #if COMPILE_FOR_OSX_4
  #define SET_UP_ENVIRONMENT(zz)    \
    zz.fSyslogFunction = gSyslogFunction
 #endif /* COMPILE_FOR_OSX_4 */
 #if COMPILE_FOR_OS9_4
  #define SET_UP_ENVIRONMENT(zz)    /* */
 #endif /* COMPILE_FOR_OS9_4 */

typedef PhidgEnvStruct * PhidgEnvPtr;

 #if COMPILE_FOR_OSX_4
struct PhidgRefConStruct
{
    PSymbol          fDeviceType;
    POutlet          fOutlet;
    Ptr              fPrivateStorage;
    Ptr              fSharedStorage;
    HIDDeviceDataPtr fThisDevice;
}; /* PhidgRefConStruct */

typedef PhidgRefConStruct * PhidgRefConPtr;
 #endif /* COMPILE_FOR_OSX_4 */

 #define STANDARD_PHID_ARGS_ATTACH  \
    Pchar name, \
    PSymbol deviceType, \
    Ptr sharedStorage, \
    Ptr privateStorage, \
    HIDDeviceDataPtr thisDevice, \
    OSErr *                     result

 #if COMPILE_FOR_OSX_4
  #define STANDARD_PHID_ARGS_DEFINECALLBACK \
    Pchar name, \
    Ptr sharedStorage, \
    Ptr privateStorage, \
    HIDDeviceDataPtr thisDevice, \
    InputEventHandler *         aFun, \
    IOHIDElementCookie * *cookies, \
    Pint numCookies, \
    OSErr *                                 result
 #endif /* COMPILE_FOR_OSX_4 */

 #define STANDARD_PHID_ARGS_DO  \
    Pchar name, \
    PSymbol deviceType, \
    POutlet outlet, \
    Ptr sharedStorage, \
    Ptr privateStorage, \
    HIDDeviceDataPtr thisDevice, \
    const short argc, \
    PAtom argv, \
    OSErr *                     result

 #if COMPILE_FOR_OSX_4
  #define STANDARD_PHID_ARGS_IDENTIFY   \
    Pchar name, \
    Plong productID, \
    Plong privateSize, \
    Plong sharedSize, \
    Pbool isAsynchronous
 #endif /* COMPILE_FOR_OSX_4 */
 #if COMPILE_FOR_OS9_4
  #define STANDARD_PHID_ARGS_IDENTIFY   \
    Pchar name, \
    Plong productID, \
    Plong privateSize, \
    Plong sharedSize
 #endif /* COMPILE_FOR_OS9_4 */

 #define STANDARD_PHID_ARGS_MAIN    \
    Pchar name, \
    Ptr sharedStorage, \
    PhidgEnvPtr environment

 #define STANDARD_PHID_ARGS_NIAM    \
    Pchar name, \
    Ptr sharedStorage

 #if COMPILE_FOR_OS9_4
  #define STANDARD_PHID_ARGS_REPORTHANDLER  \
    Pchar name, \
    PSymbol deviceType, \
    Ptr sharedStorage, \
    Ptr privateStorage, \
    HIDDeviceDataPtr thisDevice, \
    POutlet outlet, \
    Pvoid inHIDReport, \
    const UInt32 inHIDReportLength
 #endif /* COMPILE_FOR_OS9_4 */

 #if COMPILE_FOR_OSX_4
typedef E_PhidgResult (*FpDefineCallback)
    (STANDARD_PHID_ARGS_DEFINECALLBACK);

typedef E_PhidgResult (*FpDoCustom)
    (STANDARD_PHID_ARGS_DO);

typedef E_PhidgResult (*FpDoGet)
    (STANDARD_PHID_ARGS_DO);

typedef E_PhidgResult (*FpDoPut)
    (STANDARD_PHID_ARGS_DO);

typedef OSErr (*FpIdentify)
    (STANDARD_PHID_ARGS_IDENTIFY);

typedef OSErr (*FpMain)
    (STANDARD_PHID_ARGS_MAIN);

typedef OSErr (*FpNiam)
    (STANDARD_PHID_ARGS_NIAM);

typedef E_PhidgResult (*FpOnAttach)
    (STANDARD_PHID_ARGS_ATTACH);

typedef E_PhidgResult (*FpOnDetach)
    (STANDARD_PHID_ARGS_ATTACH);
 #endif /* COMPILE_FOR_OSX_4 */
 #if COMPILE_FOR_OS9_4
enum
{
    uppDoCustomProcInfo = (kCStackBased |
                           RESULT_SIZE(SIZE_CODE(sizeof(E_PhidgResult))) |
                           STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(Pchar))) | // name
                           STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(PSymbol))) | // deviceType
                           STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(POutlet))) | // outlet
                           STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(Ptr))) | // sharedStorage
                           STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(Ptr))) | // privateStorage
                           STACK_ROUTINE_PARAMETER(6, SIZE_CODE(sizeof(HIDDeviceDataPtr))) | // thisDevice
                           STACK_ROUTINE_PARAMETER(7, SIZE_CODE(sizeof(short))) | // argc
                           STACK_ROUTINE_PARAMETER(8, SIZE_CODE(sizeof(PAtom))) | // argv
                           STACK_ROUTINE_PARAMETER(9, SIZE_CODE(sizeof(OSErr *)))), // result
    uppDoGetProcInfo = (kCStackBased |
                        RESULT_SIZE(SIZE_CODE(sizeof(E_PhidgResult))) |
                        STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(Pchar))) | // name
                        STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(PSymbol))) | // deviceType
                        STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(POutlet))) | // outlet
                        STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(Ptr))) | // sharedStorage
                        STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(Ptr))) | // privateStorage
                        STACK_ROUTINE_PARAMETER(6, SIZE_CODE(sizeof(HIDDeviceDataPtr))) | // thisDevice
                        STACK_ROUTINE_PARAMETER(7, SIZE_CODE(sizeof(short))) | // argc
                        STACK_ROUTINE_PARAMETER(8, SIZE_CODE(sizeof(PAtom))) | // argv
                        STACK_ROUTINE_PARAMETER(9, SIZE_CODE(sizeof(OSErr *)))), // result
    uppDoPutProcInfo = (kCStackBased |
                        RESULT_SIZE(SIZE_CODE(sizeof(E_PhidgResult))) |
                        STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(Pchar))) | // name
                        STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(PSymbol))) | // deviceType
                        STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(POutlet))) | // outlet
                        STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(Ptr))) | // sharedStorage
                        STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(Ptr))) | // privateStorage
                        STACK_ROUTINE_PARAMETER(6, SIZE_CODE(sizeof(HIDDeviceDataPtr))) | // thisDevice
                        STACK_ROUTINE_PARAMETER(7, SIZE_CODE(sizeof(short))) | // argc
                        STACK_ROUTINE_PARAMETER(8, SIZE_CODE(sizeof(PAtom))) | // argv
                        STACK_ROUTINE_PARAMETER(9, SIZE_CODE(sizeof(OSErr *)))), // result
    uppIdentifyProcInfo = (kCStackBased |
                           RESULT_SIZE(SIZE_CODE(sizeof(OSErr))) |
                           STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(Pchar))) | // name
                           STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(Plong))) | // productID
                           STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(Plong))) | // privateSize
                           STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(Plong))) | // sharedSize
                           STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(Pbool)))), // isAsynchronous
    uppMainProcInfo = (kCStackBased |
                       RESULT_SIZE(SIZE_CODE(sizeof(OSErr))) |
                       STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(Pchar))) | // name
                       STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(Ptr))) | // sharedStorage
                       STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(PhidgEnvPtr)))), // environment
    uppNiamProcInfo = (kCStackBased |
                       RESULT_SIZE(SIZE_CODE(sizeof(OSErr))) |
                       STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(Pchar))) | // name
                       STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(Ptr)))), // sharedStorage
    uppOnAttachProcInfo = (kCStackBased |
                           RESULT_SIZE(SIZE_CODE(sizeof(E_PhidgResult))) |
                           STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(Pchar))) | // name
                           STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(PSymbol))) | // deviceType
                           STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(Ptr))) | // sharedStorage
                           STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(Ptr))) | // privateStorage
                           STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(HIDDeviceDataPtr))) | // thisDevice
                           STACK_ROUTINE_PARAMETER(6, SIZE_CODE(sizeof(OSErr *)))), // result
    uppOnDetachProcInfo = (kCStackBased |
                           RESULT_SIZE(SIZE_CODE(sizeof(E_PhidgResult))) |
                           STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(Pchar))) | // name
                           STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(PSymbol))) | // deviceType
                           STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(Ptr))) | // sharedStorage
                           STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(Ptr))) | // privateStorage
                           STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(HIDDeviceDataPtr))) | // thisDevice
                           STACK_ROUTINE_PARAMETER(6, SIZE_CODE(sizeof(OSErr *)))), // result
    uppReportHandlerProcInfo = (kCStackBased |
                                RESULT_SIZE(kNoByteCode) |
                                STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(Pchar))) | // name
                                STACK_ROUTINE_PARAMETER(2, SIZE_CODE(sizeof(PSymbol))) | // deviceType
                                STACK_ROUTINE_PARAMETER(3, SIZE_CODE(sizeof(Ptr))) | // sharedStorage
                                STACK_ROUTINE_PARAMETER(4, SIZE_CODE(sizeof(Ptr))) | // privateStorage
                                STACK_ROUTINE_PARAMETER(5, SIZE_CODE(sizeof(HIDDeviceDataPtr))) | // thisDevice
                                STACK_ROUTINE_PARAMETER(6, SIZE_CODE(sizeof(POutlet))) | // outlet
                                STACK_ROUTINE_PARAMETER(7, SIZE_CODE(sizeof(Pvoid))) | // inHIDReport
                                STACK_ROUTINE_PARAMETER(8, SIZE_CODE(sizeof(UInt32)))) // inHIDReportLength
};
 #endif /* COMPILE_FOR_OS9_4 */

#endif /* not PHIDGETENTRYPOINTS_H_ */
