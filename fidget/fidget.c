/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       fidget.c                                                                */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the fidget module.        */
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

#define IS_MAIN /* */
#include "fidget.h"
#include "displayAboutBox.h"
#include "loadAboutBoxData.h"
#include "loadOtherSegments.h"
#include "reportAnything.h"
#include "reportVersion.h"
#include "Common_HIDX.h"
#include "Common_USBX.h"
#include "PhidgetEntryPoints.h"

#define PHIDGET_KIND					'Äi¶j'
#define PHIDGET_FOLDER_NAME		1
#define PHIDGET_FOLDER_ALIAS	2
#define PHIDGET_VENDOR				1730 

#define QUEUE_HANDLER_MESSAGE_SIZE	6
#define REPORT_HANDLER_MESSAGE_SIZE 3

/* Forward references: */
Pvoid fidgetCreate
  (void);

Pvoid fidgetFree
  (FidgetPtr	xx);

bool checkForPhidgetsFolder
  (const short	volumeID,
   const long		dirID);

bool compareStrings
  (Puchar	str1,
   Puchar	str2);
   
void freeRoutineDescriptors
  (PhidgetDescriptorPtr	descriptor);

OSErr getAppDir
  (FSSpec * pAppFile);

#if defined(COMPILE_FOR_OSX_4)
OSErr getEntryPoint
  (CFragConnectionID	connID,
   Str255							name,
   ProcPtr *					newUpp);
#endif /* COMPILE_FOR_OSX_4 */

#if defined(COMPILE_FOR_OS9_4)
OSErr getEntryPoint
  (CFragConnectionID  connID,
   Str255             name,
   ProcInfoType       procInfo,
   UniversalProcPtr * newUpp);
#endif /* COMPILE_FOR_OS9_4 */

bool initPhidgetList
  (void);

void reprocessUSBDevices
	(FidgetPtr	xx);
	
void scanPhidgetFolder
  (FidgetPtr	xx);

bool validateEntryPoints
  (PhidgetDescriptorPtr	descriptor,
   FSSpec *							anFSS,
   Puchar								name);

/*------------------------------------ main ---*/
void main
  (Pfptr	ff)
{
  OSErr myErr = noErr;

  EnterCodeResource();
  PrepareCallback();
  FNS = ff;   /* Connect up the function macros. */
  /* Allocate class memory and set up class. */
  setup(reinterpret_cast<t_messlist**>(&gClass), reinterpret_cast<method>(fidgetCreate),
        reinterpret_cast<method>(fidgetFree), short(sizeof(FidgetData)),
        reinterpret_cast<method>(0L), A_DEFLONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Anything), "anything", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Assist), "assist", A_CANT, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Do), "do", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Get), "get", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Info), "info", A_CANT, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Listen), "listen", A_DEFSYM, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Put), "put", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Report), "report", A_DEFSYM, A_DEFSYM,
          A_DEFLONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Structure), "structure", A_SYM, A_SYM, A_NOTHING);
  /* Add object class to New Object List */
  finder_addclass("Devices", OUR_NAME);
  /* Make sure that our string list resources are present */
  rescopy('STR#', OUR_RES_NUMB);
  rescopy('STR#', OUR_RES_NUMB + 10000);
  loadAboutBoxData();
  gVersionNumber = reportVersion(OUR_NAME);
  gAddedSymbol = gensym("added");
  gAsteriskSymbol = gensym("*");
	gCollectionSymbol = gensym("collection");
	gDeviceSymbol = gensym("device");
	gDevicesSymbol = gensym("devices");
	gElementSymbol = gensym("element");
	gElementsSymbol = gensym("elements");
  gEmptySymbol = gensym("");
	gFeatureSymbol = gensym("feature");
	gInputAxisSymbol = gensym("input-axis");
	gInputButtonSymbol = gensym("input-button");
	gInputMiscSymbol = gensym("input-misc");
	gInputScanCodesSymbol = gensym("input-scancodes");
	gOffSymbol = gensym("off");
	gOnSymbol = gensym("on");
	gOutputSymbol = gensym("output");
	gRemovedSymbol = gensym("removed");
	gUnknownSymbol = gensym("unknown");
	loadOtherSegments();
  GetIndString(gPhidgetFolderName, OUR_RES_NUMB + 10000, PHIDGET_FOLDER_NAME);
  GetIndString(gPhidgetFolderAlias, OUR_RES_NUMB + 10000, PHIDGET_FOLDER_ALIAS);
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
  if (myErr == noErr)
   	initPhidgetList();
  else
    LOG_ERROR_2(OUTPUT_PREFIX "couldn't find Preference folder: %d", myErr)
  ExitCodeResource();
} /* main */

#if defined(COMPILE_FOR_OSX_4)
/*------------------------------------ removeHIDDevice ---*/
static void removeHIDDevice
	(Pvoid					refCon,
	 io_service_t		service,
	 natural_t			messageType,
	 Pvoid					messageArgument)
{
 #pragma unused(service, messageArgument)
	HIDDeviceDataPtr	thisDeviceHID = reinterpret_cast<HIDDeviceDataPtr>(refCon);

	if (messageType == kIOMessageServiceIsTerminated)
	{
		PSymbol								whichOne = gUnknownSymbol;
  	E_PhidgResult					action = kPhidgDoDefault;
  	OSErr									result;
		FidgetPtr							xx = reinterpret_cast<FidgetPtr>(thisDeviceHID->fOwner);
		PhidgetDescriptorPtr	kind = reinterpret_cast<PhidgetDescriptorPtr>(thisDeviceHID->fClass);
		
		if (kind)
		{
			if (kind->fIsAsynchronous)
			{
			}
			whichOne = kind->fName;
			action = reinterpret_cast<FpOnDetach>(kind->fOnDetachFun)
																	(OUR_NAME, whichOne, kind->fShared, thisDeviceHID->fPrivate,
																	thisDeviceHID, &result);
		}
		if (action == kPhidgDoDefault)
			LOG_POST_3("detach %s:%s", whichOne->s_name, thisDeviceHID->fSerialNumber->s_name);
		if (xx->fReportEvents)
		{
			Atom	outList[2];

			SETSYM(outList, whichOne);
			SETSYM(outList + 1, thisDeviceHID->fSerialNumber);
			outlet_anything(xx->fDataOut, gRemovedSymbol, sizeof(outList) / sizeof(*outList),
											outList);
		}
		if (thisDeviceHID == xx->fHIDDevices)
			xx->fHIDDevices = thisDeviceHID->fNext;
		--xx->fHIDDeviceCount;
		releaseHIDData(xx->fHIDControl, thisDeviceHID);
	}
} /* removeHIDDevice */
#endif /* COMPILE_FOR_OSX_4 */

#if defined(COMPILE_FOR_OS9_4)
/*------------------------------------ removeHIDDevice ---*/
static void removeHIDDevice
	(FidgetPtr															xx,
	 USBContext &														rec,
	 USBDeviceNotificationParameterBlockPtr	pb)
{
 #pragma unused(rec)
	HIDDeviceDataPtr	thisDeviceHID;
		
	// locate the device...
	for (thisDeviceHID = xx->fHIDDevices; thisDeviceHID; thisDeviceHID = thisDeviceHID->fNext)
	{
		if (thisDeviceHID->fDevice == pb->usbDeviceRef)
			break;

	}
	// remove it...
	if (thisDeviceHID)
	{
		PSymbol								whichOne = gUnknownSymbol;
  	E_PhidgResult					action = kPhidgDoDefault;
  	OSStatus							result;
  	OSErr									result2;
		PhidgetDescriptorPtr	kind = reinterpret_cast<PhidgetDescriptorPtr>(thisDeviceHID->fClass);
		
		result = (*thisDeviceHID->fDispatchTable->pHIDRemoveReportHandler)(thisDeviceHID->fConnection);		
		if (kind)
		{
			if (kind->fIsAsynchronous)
			{
			}
			whichOne = kind->fName;
			action = static_cast<E_PhidgResult>(CallUniversalProc(kind->fOnDetachUpp, uppOnDetachProcInfo,
																				OUR_NAME, whichOne, kind->fShared, thisDeviceHID->fPrivate,
																				thisDeviceHID, &result2));
		}
		if (action == kPhidgDoDefault)
			LOG_POST_3("detach %s:%s", whichOne->s_name, thisDeviceHID->fSerialNumber->s_name);
		if (xx->fReportEvents)
		{
			Atom	outList[2];

			SETSYM(outList, whichOne);
			SETSYM(outList + 1, thisDeviceHID->fSerialNumber);
			outlet_anything(xx->fDataOut, gRemovedSymbol, sizeof(outList) / sizeof(*outList),
											outList);
		}
		if (thisDeviceHID == xx->fHIDDevices)
			xx->fHIDDevices = thisDeviceHID->fNext;
		--xx->fHIDDeviceCount;
		releaseHIDData(thisDeviceHID);	
	}
} /* removeHIDDevice */
#endif /* COMPILE_FOR_OS9_4 */

#if defined(COMPILE_FOR_OSX_4)
/*------------------------------------ queueHandlerRedirect ---*/
static void queueHandlerRedirect
	(FidgetPtr	xx,
	 PSymbol		message,
	 short			argc,
	 PAtom			argv)
{
 #pragma unused(xx,message)
LOG_POST_1("queue redirect called")//!!
	if (argc == QUEUE_HANDLER_MESSAGE_SIZE)
	{
		InputEventHandler		aFun = reinterpret_cast<InputEventHandler>(argv[0].a_w.w_long);
		Pvoid								refCon = reinterpret_cast<Pvoid>(argv[1].a_w.w_long);
		IOHIDElementCookie	elementCookie = reinterpret_cast<IOHIDElementCookie>(argv[2].a_w.w_long);
		SInt32							value = static_cast<SInt32>(argv[3].a_w.w_long);
		UInt32							longValueSize = static_cast<UInt32>(argv[4].a_w.w_long);
		Ptr									longValue = reinterpret_cast<Ptr>(argv[5].a_w.w_long);

		// call the real function here...
		if (aFun)
			aFun(refCon, elementCookie, value, longValueSize, longValue);
		// Make sure to dispose of the duplicate event data!
		if (longValueSize && longValue)
			FREEBYTES(longValue, longValueSize);		
	}	
} /* queueHandlerRedirect */
#endif /* COMPILE_FOR_OSX_4 */

#if defined(COMPILE_FOR_OS9_4)
/*------------------------------------ reportHandlerRedirect ---*/
static void reportHandlerRedirect
	(FidgetPtr	xx,
	 PSymbol		message,
	 short			argc,
	 PAtom			argv)
{
 #pragma unused(message)
	if (argc == REPORT_HANDLER_MESSAGE_SIZE)
	{
		Ptr										reportData = reinterpret_cast<Ptr>(argv[0].a_w.w_long);
		UInt32								reportLength = static_cast<UInt32>(argv[1].a_w.w_long);
		HIDDeviceDataPtr			thisDeviceHID = reinterpret_cast<HIDDeviceDataPtr>(argv[2].a_w.w_long);
		PhidgetDescriptorPtr	kind = reinterpret_cast<PhidgetDescriptorPtr>(thisDeviceHID->fClass);
  	E_PhidgResult					action;
					
		// Get the phidget to process the report...
		action = static_cast<E_PhidgResult>(CallUniversalProc(kind->fReportHandlerUpp, uppReportHandlerProcInfo,
																OUR_NAME, kind->fName, kind->fShared, thisDeviceHID->fPrivate, thisDeviceHID,
																xx->fDataOut, reportData, reportLength));
		// Make sure to dispose of the duplicate report data!
		FREEBYTES(reportData, reportLength);		
	}	
} /* reportHandlerRedirect */
#endif /* COMPILE_FOR_OS9_4 */

#if defined(COMPILE_FOR_OSX_4)
/*------------------------------------ rawQueueHandlerProc ---*/
static void rawQueueHandlerProc
	(STANDARD_HID_ARGS_INPUTEVENTHANDLER)
{
LOG_POST_1("raw queue handler called")//!!
	Atom							eventData[QUEUE_HANDLER_MESSAGE_SIZE];
	PhidgRefConPtr		data = reinterpret_cast<PhidgRefConPtr>(refCon);
	HIDDeviceDataPtr	thisDeviceHID = data->fThisDevice;
	FidgetPtr					xx = reinterpret_cast<FidgetPtr>(thisDeviceHID->fOwner);
	Ptr								duplicateData = NULL_PTR;

	if (longValueSize && longValue)
	{
		duplicateData = GETBYTES(longValueSize, char);
		memcpy(duplicateData, longValue, longValueSize);
	}
	SETLONG(eventData, long(thisDeviceHID->fQueueHandler));
	SETLONG(eventData + 1, long(refCon));
	SETLONG(eventData + 2, long(elementCookie));
	SETLONG(eventData + 3, value);
	SETLONG(eventData + 4, long(longValueSize));
	SETLONG(eventData + 5, long(duplicateData));
	defer_low(xx, reinterpret_cast<method>(queueHandlerRedirect), NULL_PTR,
						QUEUE_HANDLER_MESSAGE_SIZE, eventData);
} /* rawQueueHandlerProc */
#endif /* COMPILE_FOR_OSX_4 */

#if defined(COMPILE_FOR_OS9_4)
/*------------------------------------ rawReportHandlerProc ---*/
static void rawReportHandlerProc
	(Pvoid	inHIDReport,
	 UInt32	inHIDReportLength,
	 UInt32	inRefCon)
{
	Atom									eventData[REPORT_HANDLER_MESSAGE_SIZE];
	HIDDeviceDataPtr			thisDeviceHID = reinterpret_cast<HIDDeviceDataPtr>(inRefCon);
	FidgetPtr							xx = reinterpret_cast<FidgetPtr>(thisDeviceHID->fOwner);
	PhidgetDescriptorPtr	kind = reinterpret_cast<PhidgetDescriptorPtr>(thisDeviceHID->fClass);

	// Duplicate the report data, in case it is only transient...
	Ptr										duplicateReport = GETBYTES(inHIDReportLength, char);
	
	memcpy(duplicateReport, inHIDReport, inHIDReportLength);
	SETLONG(eventData, long(duplicateReport));
	SETLONG(eventData + 1, long(inHIDReportLength));
	SETLONG(eventData + 2, long(inRefCon));
	defer_low(xx, reinterpret_cast<method>(reportHandlerRedirect), NULL_PTR,
						REPORT_HANDLER_MESSAGE_SIZE, eventData);
} /* rawReportHandlerProc */
#endif /* COMPILE_FOR_OS9_4 */

#if defined(COMPILE_FOR_OSX_4)
/*------------------------------------ addHIDDevice ---*/
static void addHIDDevice
	(FidgetPtr		xx,
	 io_service_t	obj)
{
	HIDDeviceDataPtr					newData = NULL_PTR;	
	IOCFPlugInInterface * * 	cfPlugInInterface = NULL_PTR; 
	SInt32 										theScore; 
	kern_return_t							kernResult;
	CFTypeRef									idVendorRef = NULL_PTR;
	bool											okSoFar = true;
	IOHIDDeviceInterface * *	device = NULL_PTR;
	ULONG											relResult;
	IOReturn									operResult;					

	if (okSoFar)
	{
		// Create a device interface
		if (createPlugInInterface(xx->fHIDControl, obj, kIOHIDDeviceUserClientTypeID,
																kIOCFPlugInInterfaceID, cfPlugInInterface, theScore,
																kernResult))
		{
			if ((kernResult == KERN_SUCCESS) && cfPlugInInterface)
			{
				okSoFar = (queryPlugInInterface(cfPlugInInterface,
																	CFUUIDGetUUIDBytes(kIOHIDDeviceInterfaceID), 
																reinterpret_cast<LPVOID*>(&device)) == KERN_SUCCESS);
				releasePlugInInterface(cfPlugInInterface, relResult);
				if (okSoFar && device)
				{
					operResult = openHIDDevice(device, 0);
					if (operResult != KERN_SUCCESS)
					{
						okSoFar = false;
						LOG_POST_3("open returns: %d (0x%x)", operResult, operResult);
					}
				}
			}
		}
	}
	if (okSoFar)
	{
		newData = GETBYTES(1, HIDDeviceDataStruct);
		okSoFar = (newData && initHIDDeviceData(OUR_NAME, xx->fHIDControl, obj, *newData,
																							xx, device));
		if (okSoFar)
		{
			for (PhidgetDescriptorPtr walker = xx->fPhidgets; walker; walker = walker->fNext)
			{
				if (walker->fProductID == newData->fProductID)
				{
					newData->fClass = walker;
					break;
					
				}
			}
			// connect release callback:
			newData->fInterestCallback = createSpringboard(IOServiceInterestCallback,
																											removeHIDDevice);
			okSoFar = addInterestNotification(xx->fHIDControl, obj, kIOGeneralInterest,
																				newData->fInterestCallback, newData,
																				newData->fNotification, kernResult);
		}
	}
	if (okSoFar)
	{
		PSymbol								whichOne = gUnknownSymbol;
  	E_PhidgResult					action = kPhidgDoDefault;
  	OSErr									result;
		PhidgetDescriptorPtr	kind = reinterpret_cast<PhidgetDescriptorPtr>(newData->fClass);
		
		if (kind)
			newData->fPrivateSize = kind->fPrivateSize;
		if (newData->fPrivateSize > 0)
			newData->fPrivate = GETBYTES(newData->fPrivateSize, char);
		if (kind)
		{
			whichOne = kind->fName;
			action = reinterpret_cast<FpOnAttach>(kind->fOnAttachFun)
																(OUR_NAME, whichOne, kind->fShared, newData->fPrivate,
																newData, &result);
			if ((action == kPhidgSuccess) && kind->fIsAsynchronous)
			{
				IOHIDElementCookie *	cookies = NULL_PTR;
				int										numCookies = 0;
				
				action = reinterpret_cast<FpDefineCallback>(kind->fDefineCallbackFun)
																	(OUR_NAME, kind->fShared, newData->fPrivate,
																	newData, &newData->fQueueHandler, &cookies,
																	&numCookies, &result);
				if ((action == kPhidgSuccess) && newData->fQueueHandler && cookies && numCookies)
				{
					PhidgRefConPtr	refCon = GETBYTES(1, PhidgRefConStruct);
					 
					// We've got a live one!
					newData->fRefConSize = sizeof(PhidgRefConStruct);
					newData->fRefCon = reinterpret_cast<Ptr>(refCon);
					refCon->fDeviceType = kind->fName;
					refCon->fOutlet = xx->fDataOut;
					refCon->fSharedStorage = kind->fShared;
					refCon->fPrivateStorage = newData->fPrivate;
					refCon->fThisDevice = newData;
					setUpHIDInputQueue(OUR_NAME, xx->fHIDControl, *newData, rawQueueHandlerProc, cookies,
															numCookies);
				}
			}
		}
		if (action == kPhidgDoDefault)
			LOG_POST_3("attach %s:%s", whichOne->s_name, newData->fSerialNumber->s_name);
		if (xx->fReportEvents)
		{
			Atom	outList[2];

			SETSYM(outList, whichOne);
			SETSYM(outList + 1, newData->fSerialNumber);
			outlet_anything(xx->fDataOut, gAddedSymbol, sizeof(outList) / sizeof(*outList),
											outList);
		}
		if (xx->fHIDDevices)
			xx->fHIDDevices->fPrevious = newData;
		newData->fNext = xx->fHIDDevices;
		xx->fHIDDevices = newData;
		++xx->fHIDDeviceCount;
	}
	if (! okSoFar)
		// We can't use this object, or ...
		releaseHIDData(xx->fHIDControl, newData);
} /* addHIDDevice */
#endif /* COMPILE_FOR_OSX_4 */

#if defined(COMPILE_FOR_OS9_4)
/*------------------------------------ addHIDDevice ---*/
static void addHIDDevice
	(FidgetPtr															xx,
	 USBContext &														rec,
	 USBDeviceNotificationParameterBlockPtr	pb)
{
	if (xx && rec.fActive && (! xx->fStopping))
	{
		CFragConnectionID	connID;
		OSStatus					result;

		result = USBGetDriverConnectionID(&pb->usbDeviceRef, &connID);
		if (result == noErr)
		{
			// Find the device dispatch table:
			CFragSymbolClass					symClass;
			THz												savedZone = GetZone();
			HIDDeviceDispatchTablePtr	aTable = NULL_PTR;
			
			SetZone(SystemZone());			
			result = FindSymbol(connID, "\pTheHIDDeviceDispatchTable",
													reinterpret_cast<Ptr*>(&aTable), &symClass);
			SetZone(savedZone);
			if (result == noErr)
			{
				// Note that the dispatch table will probably be found when the interface is added,
				// rather than when the device is added, but we'll check both times!
				HIDDeviceDataPtr	aDevice = GETBYTES(1, HIDDeviceDataStruct);
				bool							doAdd = false;

				if (aDevice && initHIDDeviceData(OUR_NAME, *aDevice, pb->usbDeviceRef, aTable, xx))
				{
					for (PhidgetDescriptorPtr walker = xx->fPhidgets; walker; walker = walker->fNext)
					{
						if (walker->fProductID == aDevice->fProductID)
						{
							aDevice->fClass = walker;
							break;
							
						}
					}
					// Check if this device is already on the chain:
					doAdd = true;
					for (HIDDeviceDataPtr walker = xx->fHIDDevices; walker; walker = walker->fNext)
					{
						if ((walker->fProductID == aDevice->fProductID) &&
								(walker->fSerialNumber == aDevice->fSerialNumber))
						{
							// Already known...
							doAdd = false;
							break;
						
						}
					}
				}
				if (doAdd)
				{
					PSymbol								whichOne = gUnknownSymbol;
			  	E_PhidgResult					action = kPhidgDoDefault;
					PhidgetDescriptorPtr	kind = reinterpret_cast<PhidgetDescriptorPtr>(aDevice->fClass);
					
					if (kind)
						aDevice->fPrivateSize = kind->fPrivateSize;
					if (aDevice->fPrivateSize > 0)
						aDevice->fPrivate = GETBYTES(aDevice->fPrivateSize, char);
					if (kind)
					{
						whichOne = kind->fName;
						// Connect the report handler...
						result = (*aTable->pHIDInstallReportHandler)(aDevice->fConnection, 0, rawReportHandlerProc,
																													reinterpret_cast<UInt32>(aDevice));
						action = static_cast<E_PhidgResult>(CallUniversalProc(kind->fOnAttachUpp, uppOnAttachProcInfo,
																				OUR_NAME, whichOne, kind->fShared, aDevice->fPrivate, aDevice,
																				&result));
					}
					if (action == kPhidgDoDefault)
						LOG_POST_3("attach %s:%s", whichOne->s_name, aDevice->fSerialNumber->s_name);
					if (xx->fReportEvents)
					{
						Atom	outList[2];

						SETSYM(outList, whichOne);
						SETSYM(outList + 1, aDevice->fSerialNumber);
						outlet_anything(xx->fDataOut, gAddedSymbol, sizeof(outList) / sizeof(*outList),
														outList);
					}
					if (xx->fHIDDevices)
						xx->fHIDDevices->fPrevious = aDevice;
					aDevice->fNext = xx->fHIDDevices;
					xx->fHIDDevices = aDevice;
					++xx->fHIDDeviceCount;
				}
				else
					// We can't use this object, or ...
					releaseHIDData(aDevice);
			}
		}
		else
			LOG_ERROR_3(OUTPUT_PREFIX "could not get driver connection: %d(0x%x)", result,
									result);
	}
} /* addHIDDevice */
#endif /* COMPILE_FOR_OS9_4 */

#if defined(COMPILE_FOR_OSX_4)
/*------------------------------------ HIDDeviceAdded ---*/
static void HIDDeviceAdded
	(Pvoid					refCon,
	 io_iterator_t	iterator)
{
	FidgetPtr	xx = reinterpret_cast<FidgetPtr>(refCon);

	if (xx && xx->fHIDControl.fActive && (! xx->fStopping))
	{
		for ( ; ; )
		{
		 	io_service_t	obj;
		 	
			if (! getNextServiceObject(xx->fHIDControl, iterator, obj))
				break;

			if (! obj)
				break;

			CFTypeRef	idVendorRef = NULL_PTR;

			if (getObjectProperty(xx->fHIDControl, obj, CFSTR(kIOHIDVendorIDKey),
														idVendorRef))
			{
				if (idVendorRef)
				{
					long	number;
					
					if (CFNumberGetValue(static_cast<CFNumberRef>(idVendorRef),
																kCFNumberLongType, &number))
					{
						if (number == PHIDGET_VENDOR)
							addHIDDevice(xx, obj);
					}
				}
			}
			releaseServiceObject(OUR_NAME, xx->fHIDControl, obj);
		}
  }	
} /* HIDDeviceAdded */
#endif /* COMPILE_FOR_OSX_4 */

#if defined(COMPILE_FOR_OSX_4)
/*------------------------------------ addUSBDevice ---*/
static void addUSBDevice
	(FidgetPtr		xx,
	 io_service_t	obj)
{
	CFTypeRef									idVendorRef = NULL_PTR;
	bool											okSoFar = true;
	kern_return_t							kernResult;
	IOCFPlugInInterface * * 	cfPlugInInterface = NULL_PTR; 
	SInt32 										theScore; 
	IOUSBDeviceInterface * *	device = NULL_PTR;
	ULONG											relResult;
	IOReturn									operResult;

	if (okSoFar)
	{
		if (createPlugInInterface(xx->fUSBControl, obj,
															kIOUSBDeviceUserClientTypeID, 
															kIOCFPlugInInterfaceID, cfPlugInInterface,
															theScore, kernResult))
		{
			if ((kernResult == KERN_SUCCESS) && cfPlugInInterface)
			{
				// get the device-specific interface...
				okSoFar = (queryPlugInInterface(cfPlugInInterface,
																	CFUUIDGetUUIDBytes(kIOUSBDeviceInterfaceID), 
																reinterpret_cast<LPVOID*>(&device)) == KERN_SUCCESS);
				releasePlugInInterface(cfPlugInInterface, relResult);
//				LOG_POST_3("release1 returns: %d (0x%x)", relResult, relResult);//!!
				if (okSoFar && device)
				{	 
					// Some of the documentation states that we need to do a IODestroyPlugInInterface of
					// cfPlugInInterface when we're done with the device-specific interface...
					operResult = openUSBDevice(device);
					if (operResult != KERN_SUCCESS)
					{
						okSoFar = false;
						LOG_POST_3("open returns: %d (0x%x)", operResult, operResult);
					}
					if (okSoFar)
					{
						operResult = configureUSBDevice(device, 0);
						if (operResult != KERN_SUCCESS)
						{
							okSoFar = false;
							LOG_POST_3("configure returns: %d (0x%x)", operResult, operResult);
						}
					}
					if (okSoFar)
					{
						operResult = closeUSBDevice(device);
						if (operResult != KERN_SUCCESS)
						{
							okSoFar = false;
							LOG_POST_3("close returns: %d (0x%x)", operResult, operResult);
						}
					}
					relResult = releaseUSBDevice(device);
//					LOG_POST_3("release2 returns: %d (0x%x)", relResult, relResult);//!!
				}
			}
		}
	}
} /* addUSBDevice */
#endif /* COMPILE_FOR_OSX_4 */

#if defined(COMPILE_FOR_OSX_4)
/*------------------------------------ USBDeviceAdded ---*/
static void USBDeviceAdded
	(Pvoid					refCon,
	 io_iterator_t	iterator)
{
	FidgetPtr	xx = reinterpret_cast<FidgetPtr>(refCon);

	if (xx && xx->fUSBControl.fActive && (! xx->fStopping))
	{
		for ( ; ; )
		{
		 	io_service_t	obj;
		 	
			if (! getNextServiceObject(xx->fUSBControl, iterator, obj))
				break;

			if (! obj)
				break;

			CFTypeRef	idVendorRef = NULL_PTR, serialNumberRef = NULL_PTR;
			CFTypeRef	productIDRef = NULL_PTR;
			bool			added = false;

			if (getObjectProperty(xx->fUSBControl, obj, CFSTR(kUSBVendorID),
														idVendorRef))
			{
				if (idVendorRef)
				{
					long	number;
					
					if (CFNumberGetValue(static_cast<CFNumberRef>(idVendorRef),
																kCFNumberLongType, &number))
					{
						if (number == PHIDGET_VENDOR)
						{
							// Check if already on HID chain:
							PSymbol	serialNumberString = NULL_PTR;
							long		productID = 0;
							bool		doAdd = true;
							
							if (getObjectProperty(xx->fUSBControl, obj, CFSTR(kUSBSerialNumber),
																		serialNumberRef) &&
									getObjectProperty(xx->fUSBControl, obj, CFSTR(kUSBProductID),
																		productIDRef))
							{
								CFIndex	bufferSize = CFStringGetLength(reinterpret_cast<CFStringRef>(serialNumberRef)) + 1;
								char *	buffer = GETBYTES(bufferSize + 1, char);
								
								if (buffer)
								{
									*buffer = '_'; // We want to be sure that the string is non-numeric!
									if (CFStringGetCString(reinterpret_cast<CFStringRef>(serialNumberRef),
																					buffer + 1, bufferSize, CFStringGetSystemEncoding()))
										serialNumberString = gensym(buffer);
									FREEBYTES(buffer, bufferSize);
								}
								CFNumberGetValue(reinterpret_cast<CFNumberRef>(productIDRef), kCFNumberLongType,
																&productID);
								for (HIDDeviceDataPtr walker = xx->fHIDDevices; walker; walker = walker->fNext)
								{
									if ((walker->fProductID == productID) &&
											(walker->fSerialNumber == serialNumberString))
									{
										// Already known...
										doAdd = false;
										break;
									
									}
								}
							}
							if (doAdd)
							{
								addUSBDevice(xx, obj);
								added = true;
							}
						}
					}
				}
			}
			if (! added)
				releaseServiceObject(OUR_NAME, xx->fUSBControl, obj);
		}
  }	
} /* USBDeviceAdded */
#endif /* COMPILE_FOR_OSX_4 */

#if defined(COMPILE_FOR_OS9_4)
/*------------------------------------ USBDeviceNotification ---*/
static void USBDeviceNotification
	(Pvoid	arg)
{
	USBDeviceNotificationParameterBlockPtr	pb = reinterpret_cast<USBDeviceNotificationParameterBlockPtr>(arg);
	
	if (pb->usbVendor == PHIDGET_VENDOR)
	{
		USBContext *	rec = reinterpret_cast<USBContext *>(pb->refcon);
		FidgetPtr			xx = reinterpret_cast<FidgetPtr>(rec->fOwner);
		
		switch (pb->usbDeviceNotification)
		{
			case kNotifyAddDevice:
				addHIDDevice(xx, *rec, pb);
				break;
				
			case kNotifyRemoveDevice:
				removeHIDDevice(xx, *rec, pb);
				break;
				
			case kNotifyAddInterface:
				addHIDDevice(xx, *rec, pb);
				break;
				
			case kNotifyRemoveInterface:
				removeHIDDevice(xx, *rec, pb);
				break;
				
		}
	}	
} /* USBDeviceNotification */
#endif /* COMPILE_FOR_OS9_4 */

#if defined(COMPILE_FOR_OSX_4)
/*------------------------------------ USBDeviceRemoved ---*/
static void USBDeviceRemoved
	(Pvoid					refCon,
	 io_iterator_t	iterator)
{
	FidgetPtr	xx = reinterpret_cast<FidgetPtr>(refCon);

	if (xx && xx->fUSBControl.fActive && (! xx->fStopping))
	{
		for ( ; ; )
		{
		 	io_service_t	obj;
		 	
			if (! getNextServiceObject(xx->fUSBControl, iterator, obj))
				break;
			
			if (! obj)
				break;
				
			releaseServiceObject(OUR_NAME, xx->fUSBControl, obj);		
		}
  }	
} /* USBDeviceRemoved */
#endif /* COMPILE_FOR_OSX_4 */

/*------------------------------------ checkForPhidgetsFolder ---*/
bool checkForPhidgetsFolder
  (const short	volumeID,
   const long		dirID)
{
  Boolean		targetIsFolder, wasAliased;
  bool			foundFolder = false;
  HFileInfo	myCPB;
  OSErr			myErr = noErr, aliasErr = noErr;
  FSSpec		newFSS;
  short			index;
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
        LOG_ERROR_2(OUTPUT_PREFIX "bad phidgets folder spec: %d", myErr)
        return false;

      }
      // aliasErr, we don't want the while loop to abort because of a 
      // bad alias. We don't care about a bad alias.
      aliasErr = ResolveAliasFile(&newFSS, true, &targetIsFolder, &wasAliased);
      if ((aliasErr == userCanceledErr) || (aliasErr == fnfErr))
      {
      	// do nothing
      }
      else if (wasAliased && (compareStrings(myCPB.ioNamePtr, gPhidgetFolderName) ||
                  compareStrings(myCPB.ioNamePtr, gPhidgetFolderAlias)))
      {
        workingDir = newFSS.parID;	// reset dir to aliases parent directory
        index = 0;	// reset index to search aliases parent directory
        // Note: index will get incremented below
        myCPB.ioVRefNum = newFSS.vRefNum;	// set volume (might not be default vol)
      }
      else
      {
        if (myCPB.ioFlAttrib & ioDirMask)
        {
          if (compareStrings(myCPB.ioNamePtr, gPhidgetFolderName)) 
          {
            foundFolder = true;
            gPhidgetDir = myCPB.ioDirID;
            gPhidgetVol = myCPB.ioVRefNum;
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
} /* checkForPhidgetsFolder */

/*------------------------------------ compareStrings ---*/
bool compareStrings
  (Puchar	str1,
   Puchar	str2)
{
  if (! (str1 && str2))
    return false;

  return EqualString(static_cast<ConstStr255Param>(str1),
                    static_cast<ConstStr255Param>(str2), false, false);
} /* compareStrings */

/*------------------------------------ fidgetCreate ---*/
Pvoid fidgetCreate
  (void)
{
  FidgetPtr xx;

  EnterCallback();
  LOG_ENTER()
  PSymbol	checkGlobal = gensym(FIDGET_CONTROL_SYMBOL);

  if (checkGlobal->s_thing)
  {
  	LOG_ERROR_1(OUTPUT_PREFIX "fidget object already present");
  	xx = NULL_PTR; 	
  }
  else
  	xx = static_cast<FidgetPtr>(newobject(gClass));
  if (xx)
  {
		xx->fHIDDevices = NULL_PTR;
		xx->fPhidgets = NULL_PTR;
		xx->fHIDDeviceCount = 0;
		xx->fStopping = xx->fReportEvents = false;
		xx->fResultOut = static_cast<POutlet>(intout(xx));
    xx->fDataOut = static_cast<POutlet>(outlet_new(xx, NULL_PTR));
    if (xx->fResultOut && xx->fDataOut)
    {
    	scanPhidgetFolder(xx);
#if defined(COMPILE_FOR_OSX_4)
			if (setUpIOKit(xx, OUR_NAME, xx->fUSBControl, kIOUSBDeviceClassName,
											USBDeviceAdded, USBDeviceRemoved) &&
					setUpIOKit(xx, OUR_NAME, xx->fHIDControl, kIOHIDDeviceKey,
											HIDDeviceAdded, NULL_PTR))
			{
				// other initialization...
				triggerIterators(xx, xx->fHIDControl, HIDDeviceAdded, NULL_PTR);
				triggerIterators(xx, xx->fUSBControl, USBDeviceAdded, USBDeviceRemoved);
	    }
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
 			if (setUpUSB(xx, OUR_NAME, xx->fUSBControl, USBDeviceNotification))
 			{
 				// other initialization...
 			} 
#endif /* COMPILE_FOR_OS9_4 */
			else
			{
	      freeobject(reinterpret_cast<PObject>(xx));
	      xx = NULL_PTR;
			}
		}
    else
    {
      LOG_ERROR_1(OUTPUT_PREFIX "unable to create ports for object")
      freeobject(reinterpret_cast<PObject>(xx));
      xx = NULL_PTR;
    }
  }
  if (xx)
	  checkGlobal->s_thing = reinterpret_cast<PObject>(xx);
  ExitCodeResource();
  return xx;
} /* fidgetCreate */

/*------------------------------------ fidgetFree ---*/
Pvoid fidgetFree
  (FidgetPtr	xx)
{
  EnterCallback();
  if (xx)
  {
	  xx->fReportEvents = false;
	  xx->fStopping = true;
	  // need to delete all objects...
	  for (HIDDeviceDataPtr walker = xx->fHIDDevices, next; walker; walker = next)
	  {
  		PSymbol								whichOne = gUnknownSymbol;
  		E_PhidgResult					action = kPhidgDoDefault;
  		OSErr									result;
	  	PhidgetDescriptorPtr	kind = reinterpret_cast<PhidgetDescriptorPtr>(walker->fClass);
	  	
	  	if (kind)
	  	{
	  		whichOne = kind->fName;
#if defined(COMPILE_FOR_OSX_4)
	  		action = reinterpret_cast<FpOnDetach>(kind->fOnDetachFun)
													  			(OUR_NAME, whichOne, kind->fShared, walker->fPrivate,
													  			walker, &result);
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
				action = static_cast<E_PhidgResult>(CallUniversalProc(kind->fOnDetachUpp, uppOnDetachProcInfo,
																				OUR_NAME, whichOne, kind->fShared, walker->fPrivate, walker,
																				&result));
#endif /* COMPILE_FOR_OS9_4 */
			}
			if (action == kPhidgDoDefault)
				LOG_POST_3("detach %s:%s", whichOne->s_name, walker->fSerialNumber->s_name);
	  	next = walker->fNext;
#if defined(COMPILE_FOR_OSX_4)
			releaseHIDData(xx->fHIDControl, walker);	  	
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
			releaseHIDData(walker);
#endif /* COMPILE_FOR_OS9_4 */
	  }
	  xx->fHIDDevices = NULL_PTR;
	  for (PhidgetDescriptorPtr walker = xx->fPhidgets, next; walker; walker = next)
	  {
	  	next = walker->fNext;
	  	
      /* Execute the niam entry point: */
#if defined(COMPILE_FOR_OSX_4)
      OSErr	myErr = reinterpret_cast<FpNiam>(walker->fNiamFun)
								      								(OUR_NAME, walker->fShared);
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
      OSErr	myErr = static_cast<OSErr>(CallUniversalProc(walker->fNiamUpp, uppNiamProcInfo, OUR_NAME,
      																			walker->fShared));
#endif /* COMPILE_FOR_OS9_4 */
      
      if (myErr != noErr)
        LOG_ERROR_2(OUTPUT_PREFIX "problem running niam() for phidget: %d", myErr)	  	
	  	FREEBYTES(walker->fShared, walker->fSharedSize);
	  	freeRoutineDescriptors(walker);	  	
	  	FREEBYTES(walker, 1);
	  }
	  xx->fPhidgets = NULL_PTR;
#if defined(COMPILE_FOR_OSX_4)
	  shutDownIOKit(xx->fHIDControl);
	  shutDownIOKit(xx->fUSBControl);
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
		shutDownUSB(xx->fUSBControl);
#endif /* COMPILE_FOR_OS9_4 */
	  gensym(FIDGET_CONTROL_SYMBOL)->s_thing = NULL_PTR;
  }
  LOG_EXIT()
  ExitMaxMessageHandler()
} /* fidgetFree */

/*------------------------------------ fidgetGetFirstHIDData ---*/
HIDDeviceDataPtr fidgetGetFirstHIDData
	(FidgetPtr	xx,
	 PSymbol		deviceType)
{
	HIDDeviceDataPtr	walker = xx->fHIDDevices;
	 
	for ( ; walker; walker = walker->fNext)
	{
		PSymbol	name = gUnknownSymbol;

		if (walker->fClass)
			name = reinterpret_cast<PhidgetDescriptorPtr>(walker->fClass)->fName;
		if (name == deviceType)
			break;

	}
	return walker;
} /* fidgetGetFirstHIDData */
	 
/*------------------------------------ fidgetGetNextHIDData ---*/
HIDDeviceDataPtr fidgetGetNextHIDData
	(PSymbol					deviceType,
	 HIDDeviceDataPtr	currentDevice)
{
	HIDDeviceDataPtr	walker = currentDevice;
	 
	if (walker)
		walker = walker->fNext;
	for ( ; walker; walker = walker->fNext)
	{
		PSymbol	name = gUnknownSymbol;

		if (walker->fClass)
			name = reinterpret_cast<PhidgetDescriptorPtr>(walker->fClass)->fName;
		if (name == deviceType)
			break;

	}
	return walker;
} /* fidgetGetNextHIDData */
	 
/*------------------------------------ fidgetLocateHIDData ---*/
HIDDeviceDataPtr fidgetLocateHIDData
	(FidgetPtr	xx,
	 PSymbol		deviceType,
	 PSymbol		serialNumber)
{
	HIDDeviceDataPtr	walker = xx->fHIDDevices;
	 
	for ( ; walker; walker = walker->fNext)
	{
		PSymbol	name = gUnknownSymbol;

		if (walker->fClass)
			name = reinterpret_cast<PhidgetDescriptorPtr>(walker->fClass)->fName;
		if ((name == deviceType) && (serialNumber == walker->fSerialNumber))
			break;

	}
	return walker;
} /* fidgetLocateHIDData */

/*------------------------------------ fidgetMapElementType ---*/
PSymbol fidgetMapElementType
	(IOHIDElementType elementType)
{
	switch (elementType)
	{
		case kIOHIDElementTypeInput_Misc:
			return gInputMiscSymbol;

		case kIOHIDElementTypeInput_Button:
			return gInputButtonSymbol;

		case kIOHIDElementTypeInput_Axis:
			return gInputAxisSymbol;

		case kIOHIDElementTypeInput_ScanCodes:
			return gInputScanCodesSymbol;

		case kIOHIDElementTypeOutput:
			return gOutputSymbol;

		case kIOHIDElementTypeFeature:
			return gFeatureSymbol;

		case kIOHIDElementTypeCollection:
			return gCollectionSymbol;
			
	}
	return gUnknownSymbol;
} /* fidgetMapElementType */

/*------------------------------------ freeRoutineDescriptors ---*/
void freeRoutineDescriptors
  (PhidgetDescriptorPtr	descriptor)
{
  descriptor->fValidMask = 0;
  CloseConnection(&descriptor->fConnID);
} /* freeRoutineDescriptors */

/*------------------------------------ getAppDir ---*/
OSErr getAppDir
  (FSSpec *	pAppFile)
{
  ProcessInfoRec			myInfo;
  ProcessSerialNumber	myPSN;
  OSErr								myErr = GetCurrentProcess(&myPSN);

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
} /* getAppDir */

#if defined(COMPILE_FOR_OSX_4)
/*------------------------------------ getEntryPoint ---*/
OSErr getEntryPoint
  (CFragConnectionID  connID,
   Str255             name,
   ProcPtr *          newFun)
{
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
} /* getEntryPoint */
#endif /* COMPILE_FOR_OS9_4 */

/*------------------------------------ initPhidgetList ---*/
bool initPhidgetList
  (void)
{
	gPhidgetVol = 0;
  bool	result = true;

	if (! checkForPhidgetsFolder(gAppVRef, gAppDir))
  {
    if (! checkForPhidgetsFolder(gSysVRef, gSysParID))
    {
      LOG_ERROR_1(OUTPUT_PREFIX "couldn't locate Phidgets folder")
      result = false;
    }
  }
  return result;
} /* initPhidgetList */

/*------------------------------------ scanPhidgetFolder ---*/
void scanPhidgetFolder
  (FidgetPtr	xx)
{
  Boolean		targetIsFolder, wasAliased;
  HFileInfo	myCPB;
  FSSpec		newFSS;
  short			index = 1;
  OSErr			myErr = noErr;

  // Begin to search for the phidgets in the current (dirID) folder...
  myCPB.ioNamePtr = reinterpret_cast<StringPtr>(NewPtrClear(40L));
  while (myErr == noErr) 
  {
    myCPB.ioVRefNum = gPhidgetVol;
    myCPB.ioFDirIndex = index++;
    myCPB.ioNamePtr[0] = 0;
    myCPB.ioDirID = gPhidgetDir;
    myErr = PBGetCatInfo(reinterpret_cast<CInfoPBPtr>(&myCPB), false);
    if (myErr == noErr) 
    {
      if (((myCPB.ioFlFndrInfo.fdType == PHIDGET_KIND)) || (myCPB.ioFlAttrib & ioDirMask))
      {
        /* Determine if this phidget is already known: */
        PhidgetDescriptorPtr	walker = xx->fPhidgets;
        bool									found = false;
        PSymbol								aName;
        
#if defined(COMPILE_FOR_OSX_4)
        char                	tempString[256];
        
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
        myErr = FSMakeFSSpec(gPhidgetVol, gPhidgetDir, myCPB.ioNamePtr, &newFSS);
        if (myErr != noErr)
        {
          LOG_ERROR_2(OUTPUT_PREFIX "bad phidget file spec: %d", myErr)
          break;

        }				
        myErr = ResolveAliasFile(&newFSS, true, &targetIsFolder, &wasAliased);
        if (myErr != noErr)
        {
          LOG_ERROR_2(OUTPUT_PREFIX "bad phidget file alias: %d", myErr)
          break;

        }				
        if (! (myCPB.ioFlAttrib & ioDirMask))
        {	    
          if (found)
          {
          	// "Last one wins"
            freeRoutineDescriptors(walker);
            if (! validateEntryPoints(walker, &newFSS, myCPB.ioNamePtr))
            	break;
            
            FREEBYTES(walker->fShared, walker->fSharedSize);	
          }
          else
          {
            PhidgetDescriptorPtr	descriptor = GETBYTES(1, PhidgetDescriptor);

            descriptor->fShared = NULL_PTR;
	          descriptor->fName = aName;
            if (! validateEntryPoints(descriptor, &newFSS, myCPB.ioNamePtr))
            {
            	FREEBYTES(descriptor, 1);
            	break;
            
            }
            descriptor->fNext = xx->fPhidgets;
            descriptor->fPrevious = NULL_PTR;
            if (xx->fPhidgets)
              xx->fPhidgets->fPrevious = descriptor;
            xx->fPhidgets = descriptor;
            walker = descriptor;
          }
          memcpy(&walker->fSpec, &newFSS, sizeof(newFSS));
          walker->fPrivateSize = walker->fSharedSize = 0;
#if defined(COMPILE_FOR_OSX_4)
          walker->fIsAsynchronous = false;
					myErr = reinterpret_cast<FpIdentify>(walker->fIdentifyFun)
																			(OUR_NAME, &walker->fProductID, &walker->fPrivateSize,
																			&walker->fSharedSize, &walker->fIsAsynchronous);
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
					myErr = static_cast<OSErr>(CallUniversalProc(walker->fIdentifyUpp, uppIdentifyProcInfo,
																			OUR_NAME, &walker->fProductID, &walker->fPrivateSize,
																			&walker->fSharedSize));
#endif /* COMPILE_FOR_OS9_4 */
					if (myErr != noErr)
					{
            LOG_ERROR_2(OUTPUT_PREFIX "problem running identify() for phidget: %d", myErr)
            break;

          }
          if (walker->fSharedSize > 0)
          	walker->fShared = GETBYTES(walker->fSharedSize, char);
          /* Execute the main entry point: */
          PhidgEnvStruct	ourEnvironment;
          
					SET_UP_ENVIRONMENT(ourEnvironment);
#if defined(COMPILE_FOR_OSX_4)
          myErr = reinterpret_cast<FpMain>(walker->fMainFun)
                        							(OUR_NAME, walker->fShared, &ourEnvironment);
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
          myErr = static_cast<OSErr>(CallUniversalProc(walker->fMainUpp, uppMainProcInfo,
                                          OUR_NAME, walker->fShared, &ourEnvironment));
#endif /* COMPILE_FOR_OS9_4 */
          if (myErr != noErr)
          {
            LOG_ERROR_2(OUTPUT_PREFIX "problem running main() for phidget: %d", myErr)
            break;

          }						
        }
      }
    }
  }
  DisposePtr(reinterpret_cast<Ptr>(myCPB.ioNamePtr));
} /* scanPhidgetFolder */

/*------------------------------------ validateEntryPoints ---*/
bool validateEntryPoints
  (PhidgetDescriptorPtr	descriptor,
   FSSpec *							anFSS,
   Puchar								name)
{
  Str255	errName;
  long		routineMask = 0;
  OSErr		myErr = GetDiskFragment(anFSS, 0, kCFragGoesToEOF, name, kPrivateCFragCopy,
                                  &descriptor->fConnID, &descriptor->fMainAddress, errName);
                                  
  if (myErr == noErr)
  {
#if defined(COMPILE_FOR_OSX_4)
    descriptor->fMainFun = reinterpret_cast<ProcPtr>(descriptor->fMainAddress);
    if (! descriptor->fMainFun)
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
    descriptor->fMainUpp = NewRoutineDescriptor(reinterpret_cast<ProcPtr>(descriptor->fMainAddress),
    																						uppMainProcInfo, GetCurrentISA());                                       
    if (! descriptor->fMainUpp)
#endif /* COMPILE_FOR_OS9_4 */
      myErr = memFullErr;
  }
  if (myErr == noErr)
    routineMask |= kMainValid;        
#if defined(COMPILE_FOR_OSX_4)
  if (myErr == noErr)
    myErr = getEntryPoint(descriptor->fConnID, "\pdefineCallback", &descriptor->fDefineCallbackFun);
	if (myErr == noErr)
		routineMask != kDefineCallbackValid;
#endif /* COMPILE_FOR_OSX_4 */
  if (myErr == noErr)
  {
#if defined(COMPILE_FOR_OSX_4)
    myErr = getEntryPoint(descriptor->fConnID, "\pdoCustom", &descriptor->fDoCustomFun);
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
    myErr = getEntryPoint(descriptor->fConnID, "\pdoCustom", uppDoCustomProcInfo,
    											&descriptor->fDoCustomUpp);
#endif /* COMPILE_FOR_OS9_4 */
  }
  if (myErr == noErr)
  {
    routineMask |= kDoCustomValid;        
#if defined(COMPILE_FOR_OSX_4)
    myErr = getEntryPoint(descriptor->fConnID, "\pdoGet", &descriptor->fDoGetFun);
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
    myErr = getEntryPoint(descriptor->fConnID, "\pdoGet", uppDoGetProcInfo, &descriptor->fDoGetUpp);
#endif /* COMPILE_FOR_OS9_4 */
  }
  if (myErr == noErr)
  {
    routineMask |= kDoGetValid;        
#if defined(COMPILE_FOR_OSX_4)
    myErr = getEntryPoint(descriptor->fConnID, "\pdoPut", &descriptor->fDoPutFun);
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
    myErr = getEntryPoint(descriptor->fConnID, "\pdoPut", uppDoPutProcInfo, &descriptor->fDoPutUpp);
#endif /* COMPILE_FOR_OS9_4 */
  }
  if (myErr == noErr)
  {
    routineMask |= kDoPutValid;        
#if defined(COMPILE_FOR_OSX_4)
    myErr = getEntryPoint(descriptor->fConnID, "\pidentify", &descriptor->fIdentifyFun);
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
    myErr = getEntryPoint(descriptor->fConnID, "\pidentify", uppIdentifyProcInfo, &descriptor->fIdentifyUpp);
#endif /* COMPILE_FOR_OS9_4 */
  }
  if (myErr == noErr)
  {
    routineMask |= kIdentifyValid;        
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
    myErr = getEntryPoint(descriptor->fConnID, "\ponAttach", &descriptor->fOnAttachFun);
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
    myErr = getEntryPoint(descriptor->fConnID, "\ponAttach", uppOnAttachProcInfo, &descriptor->fOnAttachUpp);
#endif /* COMPILE_FOR_OS9_4 */
  }
  if (myErr == noErr)
  {
    routineMask |= kOnAttachValid;
#if defined(COMPILE_FOR_OSX_4)
    myErr = getEntryPoint(descriptor->fConnID, "\ponDetach", &descriptor->fOnDetachFun);
#endif /* COMPILE_FOR_OSX_4 */
#if defined(COMPILE_FOR_OS9_4)
    myErr = getEntryPoint(descriptor->fConnID, "\ponDetach", uppOnDetachProcInfo, &descriptor->fOnDetachUpp);
#endif /* COMPILE_FOR_OS9_4 */
  }
  if (myErr == noErr)
    routineMask |= kOnDetachValid;
#if defined(COMPILE_FOR_OS9_4)
	if (myErr == noErr)
    myErr = getEntryPoint(descriptor->fConnID, "\preportHandler", uppReportHandlerProcInfo, &descriptor->fReportHandlerUpp);
	if (myErr == noErr)
		routineMask |= kReportHandlerValid;
#endif /* COMPILE_FOR_OS9_4 */
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
} /* validateEntryPoints */	              		                        

StandardAnythingRoutine(FidgetPtr)
StandardInfoRoutine(FidgetPtr)
