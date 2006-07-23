/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       rcx.c                                                                   */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the rcx module.           */
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
/*  Created:    2002/03/31                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#define IS_MAIN /* */
#include "rcx.h"
#include "displayAboutBox.h"
#include "loadAboutBoxData.h"
#include "loadOtherSegments.h"
#include "reportVersion.h"

#define RCX2_VENDOR		0x0694 
#define RCX2_PRODUCT	0x0001

enum
{
	kReadPipe = 1,
	kWritePipe = 2,
	kReadPacketSize = 8,
	kMaxWritePacket = 200
};

/* Forward references: */
Pvoid rcxCreate
  (PSymbol portClass,
   PSymbol whichPortOrPower);

Pvoid rcxFree
  (RcxControlPtr xx);

#if defined(COMPILE_FOR_OS9_4)
UInt32 rcxNotify
  (GHNOTIFYCODE notifyCode,
   PBKRESULT    resultCode,
   GHSTACK      ghostStack,
   GHQUEUE      queue,
   GHCOMMAND    command,
   Pvoid        context);
#endif /* COMPILE_FOR_OS9_4 */

#if defined(COMPILE_FOR_OSX_4)
static uchar	gRcxSync[] = { 0x55, 0xFF, 0x00 };
static uchar	gRcxNo55Sync[] = { 0xFF, 0x00 };
#endif /* COMPILE_FOR_OSX_4 */
	
/*------------------------------------ main ---*/
void main
  (Pfptr ff)
{
  EnterCodeResource();
  PrepareCallback();
  FNS = ff;   /* Connect up the function macros. */
#if FOR_MAC_PPC
  /* Allocate class memory and set up class. */
  setup(reinterpret_cast<t_messlist**>(&gClass), reinterpret_cast<method>(rcxCreate), 
        reinterpret_cast<method>(rcxFree), short(sizeof(RcxControl)),
        reinterpret_cast<method>(0L), A_DEFSYM, A_DEFSYM, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Anything), "anything", A_GIMME, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Assist), "assist", A_CANT, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_ClearMemory), "clearmemory", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_ClearSensor), "clearsensor", A_LONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_ClearSound), "clearsound", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_ContinueTask), "continuetask", A_LONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_FloatOutput), "floatoutput", A_LONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_GetAllSensors), "getallsensors", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_GetAllVariables), "getallvariables", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_GetBattery), "getbattery", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_GetSensorMode), "getsensormode", A_LONG,
          A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_GetSensorType), "getsensortype", A_LONG,
          A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_GetSensorValue), "getsensorvalue", A_LONG,
          A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_GetSlot), "getslot", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_GetVariable), "getvariable", A_LONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_GetVersion), "getversion", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Info), "info", A_CANT, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Mute), "mute", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_PlaySound), "playsound", A_SYM, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_PlayTone), "playtone", A_LONG, A_LONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Run), "run", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetMessage), "setmessage", A_LONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetOutputDirection), "setoutputdirection", A_LONG,
          A_SYM, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetOutputPower), "setoutputpower", A_LONG, A_LONG,
          A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetSensorMode), "setsensormode", A_LONG, A_SYM,
          A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetSensorType), "setsensortype", A_LONG, A_SYM,
          A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetSlot), "setslot", A_LONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetVariable), "setvariable", A_LONG, A_LONG,
          A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_SetWatch), "setwatch", A_LONG, A_LONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_StartOutput), "startoutput", A_LONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_StartTask), "starttask", A_LONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_StopAllTasks), "stopalltasks", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_StopOutput), "stopoutput", A_LONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_StopTask), "stoptask", A_LONG, A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_TurnOff), "turnoff", A_NOTHING);
  addmess(reinterpret_cast<method>(cmd_Unmute), "unmute", A_NOTHING);
 #if defined(BE_VERBOSE)
  addmess(reinterpret_cast<method>(cmd_Verbose), "verbose", A_DEFSYM, A_NOTHING);
 #endif /* BE_VERBOSE */
  /* Add object class to New Object List */
  finder_addclass("Devices", OUR_NAME);
  /* Make sure that our string list resource is present */
  rescopy('STR#', OUR_RES_NUMB);
  loadAboutBoxData();
  gAllSensorsSymbol = gensym("allsensors");
  gAllVariablesSymbol = gensym("allvariables");
  gAngleSymbol = gensym("angle");
  gAngleStepSymbol = gensym("anglestep");
  gBackwardSymbol = gensym("backward");
  gBatterySymbol = gensym("battery");
  gBeepSymbol = gensym("beep");
  gBooleanSymbol = gensym("boolean");
  gCelsiusSymbol = gensym("celsius");
  gEmptySymbol = gensym("");
  gErrorSymbol = gensym("error");
  gFahrenheitSymbol = gensym("fahrenheit");
  gFastSweepSymbol = gensym("fastsweep");
  gForwardSymbol = gensym("forward");
  gHighSymbol = gensym("high");
  gKeyClickSymbol = gensym("keyclick");
  gLowSymbol = gensym("low");
  gNoSensorSymbol = gensym("nosensor");
  gOffSymbol = gensym("off");
  gOnSymbol = gensym("on");
  gPercentSymbol = gensym("percent");
  gPeriodCountSymbol = gensym("periodcount");
  gRawSymbol = gensym("raw");
  gReflectionSymbol = gensym("reflection");
  gReverseSymbol = gensym("reverse");
  gSensorModeSymbol = gensym("sensormode");
  gSensorSymbol = gensym("sensor");
  gSensorTypeSymbol = gensym("sensortype");
  gSerialSymbol = gensym("serial");
  gSlotSymbol = gensym("slot");
  gSweepDownSymbol = gensym("sweepdown");
  gSweepUpSymbol = gensym("sweepup");
  gSwitchSymbol = gensym("switch");
  gTemperatureSymbol = gensym("temperature");
  gTransitionSymbol = gensym("transition");
  gUnknownSymbol = gensym("unknown");
  gUsbSymbol = gensym("usb");
  gVariableSymbol = gensym("variable");
  gVersionSymbol = gensym("version");
  gVersionNumber = reportVersion(OUR_NAME);
  loadOtherSegments();
#else /* not FOR_MAC_PPC */
  error(OUTPUT_PREFIX "not supported on Max/68K");
#endif /* not FOR_MAC_PPC */
  ExitCodeResource();
} /* main */

/*------------------------------------ rcxCopyFromReply ---*/
UInt32 rcxCopyFromReply
  (RcxControlPtr	xx,
   Puchar       	replyBuffer,
   const UInt32		replySize)
{
  if (xx)
  {
    UInt32	length = xx->fReplyLength;
    Quchar	source = xx->fReplyBuffer + 1;

    if (length > replySize)
      length = replySize;
    for (UInt32 ii = 0; ii < length; ++ii, ++replyBuffer, ++source)
      *replyBuffer = *source;
    return length;

  }
  return 0;
} /* rcxCopyFromReply */

#if defined(COMPILE_FOR_OSX_4)
/*------------------------------------ addUSBDevice ---*/
static void addUSBDevice
	(RcxControlPtr	xx,
	 io_service_t		obj)
{
	CFTypeRef								idVendorRef = NULL_PTR;
	bool										okSoFar = true;
	kern_return_t						kernResult;
	IOCFPlugInInterface * *	cfPlugInInterface = NULL_PTR; 
	SInt32 									theScore; 
	ULONG										relResult;
	IOReturn								operResult;

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
															reinterpret_cast<LPVOID*>(&xx->fDevice)) == KERN_SUCCESS);
			releasePlugInInterface(cfPlugInInterface, relResult);
			//LOG_POST_3("release1 returns: %ld (0x%lx)", relResult, relResult);
			if (okSoFar && xx->fDevice)
			{	 
				// Some of the documentation states that we need to do a IODestroyPlugInInterface of
				// cfPlugInInterface when we're done with the device-specific interface...
				operResult = openUSBDevice(xx->fDevice);
				if (operResult != KERN_SUCCESS)
				{
					okSoFar = false;
					LOG_POST_3("open returns: %d (0x%x)", operResult, operResult);
				}
				if (okSoFar)
				{
					operResult = configureUSBDevice(xx->fDevice, uchar(xx->fHighPower ? 1 : 0));
					if (operResult != KERN_SUCCESS)
					{
						okSoFar = false;
						LOG_POST_3("configure returns: %d (0x%x)", operResult, operResult);
					}
				}
				// get first interface for the current configuration...
				IOUSBFindInterfaceRequest	request;
				io_iterator_t							iter;
				io_service_t							usbInterface;
				
				request.bInterfaceClass = kIOUSBFindInterfaceDontCare;
				request.bInterfaceSubClass = kIOUSBFindInterfaceDontCare;
				request.bInterfaceProtocol = kIOUSBFindInterfaceDontCare;
				request.bAlternateSetting = kIOUSBFindInterfaceDontCare;
				if (okSoFar)
				{
					operResult = createUSBInterfaceIterator(xx->fDevice, request, iter);
					if (operResult != KERN_SUCCESS)
					{
						okSoFar = false;
						LOG_POST_3("create interface iterator returns: %d (0x%x)", operResult,
												operResult);
					}
				}
				if (okSoFar)
				{
					getNextServiceObject(xx->fUSBControl, iter, usbInterface);
					
					if (! releaseServiceObject(OUR_NAME, xx->fUSBControl, iter))
					{
						okSoFar = false;
						LOG_POST_1("problem releasing iterator");
					}
					if (createPlugInInterface(xx->fUSBControl, usbInterface,
																		kIOUSBInterfaceUserClientTypeID, 
																		kIOCFPlugInInterfaceID, cfPlugInInterface,
																		theScore, kernResult))
					{
						if ((kernResult == KERN_SUCCESS) && cfPlugInInterface)
						{
							// get the interface that we want...
							okSoFar = (queryPlugInInterface(cfPlugInInterface,
																				CFUUIDGetUUIDBytes(kIOUSBInterfaceInterfaceID), 
																			reinterpret_cast<LPVOID*>(&xx->fInterface)) ==
																			KERN_SUCCESS);
							releasePlugInInterface(cfPlugInInterface, relResult);
							//LOG_POST_3("release2 returns: %ld (0x%lx)", relResult, relResult);
							if (okSoFar && xx->fInterface)
							{
								operResult = openUSBInterface(xx->fInterface);
								if (operResult == KERN_SUCCESS)
								{
									xx->fDeviceOpen = true;
									CFRunLoopSourceRef	source;
									CFStringRef					defaultRunLoopMode;
									CFRunLoopRef				runLoop;
									
									if (getRunLoopDefaultMode(xx->fUSBControl, defaultRunLoopMode) &&
											getRunLoopCurrent(xx->fUSBControl, runLoop) &&
											(createUSBInterfaceAsyncEventSource(xx->fInterface, source) == KERN_SUCCESS))
										runLoopAddSource(xx->fUSBControl, runLoop, source, defaultRunLoopMode);
									rcxSetRange(xx, LTW_RANGE_MEDIUM);
									rcxSetSpeed(xx, true);
								}
								else
								{
									okSoFar = false;
									LOG_POST_3("open interface returns: %d (0x%x)", operResult,
															operResult);
								}
							}
						}
					else
						LOG_POST_3("create plugin interface interface returns: %d (0x%x)",
												kernResult, kernResult);
					}
					else
						LOG_POST_1("problem creating plugin interface interface");
					if (! releaseServiceObject(OUR_NAME, xx->fUSBControl, usbInterface))
					{
						okSoFar = false;
						LOG_POST_1("problem releasing usbInterface object");
					}
				}
			}
		}
		else
			LOG_POST_3("create plugin device interface returns: %d (0x%x)",
									kernResult, kernResult);
	}
	else
		LOG_POST_1("problem creating plugin device interface");
} /* addUSBDevice */
#endif /* COMPILE_FOR_OSX_4 */

#if defined(COMPILE_FOR_OSX_4)
/*------------------------------------ USBDeviceAdded ---*/
static void USBDeviceAdded
	(Pvoid					refCon,
	 io_iterator_t	iterator)
{
	RcxControlPtr	xx = reinterpret_cast<RcxControlPtr>(refCon);

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
						if (number == RCX2_VENDOR)
						{
							if (getObjectProperty(xx->fUSBControl, obj, CFSTR(kUSBProductID),
																		productIDRef))
							{
								if (CFNumberGetValue(reinterpret_cast<CFNumberRef>(productIDRef),
																			kCFNumberLongType, &number))
								{
									if (number == RCX2_PRODUCT)
									{
										addUSBDevice(xx, obj);
										added = true;
									}
								}
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

#if defined(COMPILE_FOR_OSX_4)
/*------------------------------------ rcxCopyFromRawBuffer ---*/
static void rcxCopyFromRawBuffer
	(RcxControlPtr	xx)
{
	for ( ; xx->fReadRemaining && (xx->fRawReceiveStart < xx->fRawReceiveEnd);
				--xx->fReadRemaining)
		*(xx->fReadWalker)++ = *(xx->fRawReceiveStart)++;
	if (! xx->fReadRemaining)
		xx->fReadComplete = true;
} /* rcxCopyFromRawBuffer */
#endif /* COMPILE_FOR_OSX_4 */

#if defined(COMPILE_FOR_OSX_4)
/*------------------------------------ rcxTriggerRead ---*/
static void rcxTriggerRead
	(RcxControlPtr	xx)
{
	xx->fRawReceiveStart = xx->fRawReceiveEnd = xx->fRawReceiveBuffer;
	IOReturn	result = readUSBPipeAsync(xx->fInterface, kReadPipe, xx->fRawReceiveBuffer,
																			kReadPacketSize, xx->fReadCompletion, xx);

	if (result != KERN_SUCCESS)
		LOG_POST_3("readUSBPipeAsync returns: %d (0x%x)", result, result);
} /* rcxTriggerRead */
#endif /* COMPILE_FOR_OSX_4 */

#if defined(COMPILE_FOR_OSX_4)
/*------------------------------------ rcxReadCompletion ---*/
static void rcxReadCompletion
	(RcxControlPtr	xx,
	 IOReturn				result,
	 Pvoid					arg0)
{
	if (result == kIOReturnAborted)
		return;

	xx->fRawReceiveEnd = xx->fRawReceiveBuffer + reinterpret_cast<UInt32>(arg0);
	rcxCopyFromRawBuffer(xx);
	if (! xx->fReadComplete)
		rcxTriggerRead(xx);
} /* rcxReadCompletion */
#endif /* COMPILE_FOR_OSX_4 */

#if defined(COMPILE_FOR_OSX_4)
/*------------------------------------ USBDeviceRemoved ---*/
static void USBDeviceRemoved
	(Pvoid					refCon,
	 io_iterator_t	iterator)
{
	RcxControlPtr	xx = reinterpret_cast<RcxControlPtr>(refCon);

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

/*------------------------------------ rcxCreate ---*/
Pvoid rcxCreate
  (PSymbol portClass,
   PSymbol whichPortOrPower)
{
  RcxControlPtr xx = NULL_PTR;

  EnterCallback();
  LOG_ENTER()
#if FOR_MAC_PPC
 #if defined(COMPILE_FOR_OSX_4)
  PSymbol	checkGlobal = gensym(RCX_CONTROL_SYMBOL);

  if (checkGlobal->s_thing)
  {
  	LOG_ERROR_1(OUTPUT_PREFIX "rcx object already present");
  	xx = NULL_PTR; 	
  }
  else
 #endif /* COMPILE_FOR_OSX_4 */
	  xx = static_cast<RcxControlPtr>(newobject(gClass));
  if (xx)
  {
 #if defined(COMPILE_FOR_OS9_4)
    PBKRESULT ghostResult;
 #endif /* COMPILE_FOR_OS9_4 */
    bool      okSoFar = true;

 #if defined(COMPILE_FOR_OSX_4)
    xx->fStopping = xx->fReportEvents = xx->fFastMode = xx->fHighPower = false;
    xx->fComplementData = true;
    xx->fLastCommand = 0;
    xx->fDevice = NULL_PTR;
    xx->fInterface = NULL_PTR;
    xx->fSync = gRcxSync;
    xx->fSyncLen = sizeof(gRcxSync) / sizeof(*gRcxSync);
    xx->fReadCompletion = createSpringboard(IOAsyncCallback1, rcxReadCompletion);
    if (whichPortOrPower == gHighSymbol)
      xx->fHighPower = true;
    else if ((whichPortOrPower == gEmptySymbol) || (whichPortOrPower == gLowSymbol))
      xx->fHighPower = false;
    else
    {
      LOG_ERROR_2(OUTPUT_PREFIX "invalid power (%s) for device", whichPortOrPower->s_name)
      okSoFar = false;
    }
 #endif /* COMPILE_FOR_OSX_4 */
 #if defined(COMPILE_FOR_OS9_4)
    xx->fGhostStack = NULL_PTR;
 #endif /* COMPILE_FOR_OS9_4 */
    xx->fDeviceOpen = xx->fSynchronized = false;
    xx->fUseUSB = true;
    if (portClass == gSerialSymbol)
    {
      xx->fUseUSB = false;
 #if defined(COMPILE_FOR_OSX_4)
			LOG_ERROR_1(OUTPUT_PREFIX "serial port is not currently supported")
			okSoFar = false;
 #endif /* COMPILE_FOR_OSX_4 */
    }
    else if ((portClass == gEmptySymbol) || (portClass == gUsbSymbol))
      xx->fUseUSB = true;
    else
    {
      LOG_ERROR_2(OUTPUT_PREFIX "invalid port class (%s) for device", portClass->s_name)
      okSoFar = false;
    }
    if (okSoFar)
    {
      xx->fReplyBuffer = GETBYTES(MAX_REPLY_BUFFER, uchar);
 #if defined(COMPILE_FOR_OSX_4)
	    xx->fReceiveBuffer = GETBYTES(MAX_RECEIVE_BUFFER, uchar);
	    xx->fRawReceiveBuffer = GETBYTES(kReadPacketSize, uchar);
 #endif /* COMPILE_FOR_OSX_4 */
      xx->fErrorBangOut = static_cast<POutlet>(bangout(xx));
      xx->fCommandComplete = static_cast<POutlet>(bangout(xx));
      xx->fDataOut = static_cast<POutlet>(outlet_new(xx, 0L));   /* normally just a list */
 #if defined(COMPILE_FOR_OSX_4)
	    if (xx->fReplyBuffer && xx->fReceiveBuffer && xx->fRawReceiveBuffer &&
	    			xx->fErrorBangOut && xx->fCommandComplete && xx->fDataOut)
 #endif /* COMPILE_FOR_OSX_4 */
 #if defined(COMPILE_FOR_OS9_4)
      if (xx->fReplyBuffer && xx->fErrorBangOut && xx->fCommandComplete && xx->fDataOut)
 #endif /* COMPILE_FOR_OS9_4 */
      {
 #if defined(COMPILE_FOR_OSX_4)
				if (setUpIOKit(xx, OUR_NAME, xx->fUSBControl, kIOUSBDeviceClassName,
												USBDeviceAdded, USBDeviceRemoved))
				{
					// other initialization...
					triggerIterators(xx, xx->fUSBControl, USBDeviceAdded, USBDeviceRemoved);
		    }
 #endif /* COMPILE_FOR_OSX_4 */
 #if defined(COMPILE_FOR_OS9_4)
        // Create a Ghost stack for communication...
        ghostResult = GhCreateStack(xx->fUseUSB ? "LEGO.Pbk.CommStack.Port.USB" :
                                                  "LEGO.Pbk.CommStack.Port.RS232",
                                    "LEGO.Pbk.CommStack.Protocol.IR",
                                    "LEGO.Pbk.CommStack.Session", &xx->fGhostStack);
        if (PBK_IS_ERROR(ghostResult) || PBK_IS_WARNING(ghostResult))
        {
          LOG_ERROR_2(OUTPUT_PREFIX "unable to create Ghost stack for device --> 0x%lX",
                      long(ghostResult))
          okSoFar = false;
        }
        if (okSoFar)
        {
          static char deviceName[256], portName[256];

          // Locate the first available device...
          ghostResult = GhSelectFirstDevice(xx->fGhostStack, portName, sizeof(portName));
          if (PBK_IS_SUCCESS(ghostResult) || PBK_IS_INFO(ghostResult))
          {
            // If we're being picky, make sure that it matches!
            if (whichPortOrPower == gEmptySymbol)
            {
              GhGetDeviceName(xx->fGhostStack, deviceName, sizeof(deviceName));
              LOG_POST_3(OUTPUT_PREFIX "port='%s' device='%s'", portName, deviceName)
            }
            else
            {
              PSymbol deviceSymbol;

              okSoFar = false;
              do
              {
                GhGetDeviceName(xx->fGhostStack, deviceName, sizeof(deviceName));
                LOG_POST_3(OUTPUT_PREFIX "port='%s' device='%s'", portName, deviceName)
                deviceSymbol = gensym(deviceName);
                if (deviceSymbol == whichPortOrPower)
                {
                  okSoFar = true;
                  break;

                }
                ghostResult = GhSelectNextDevice(xx->fGhostStack, deviceName,
                                                  sizeof(deviceName));
              }
              while (PBK_IS_SUCCESS(ghostResult) || PBK_IS_INFO(ghostResult));
              if (! okSoFar)
                LOG_ERROR_3(OUTPUT_PREFIX "unable to locate matching device (%s) for "
                            "given port class (%s)", whichPortOrPower->s_name,
                            portClass->s_name)
            }
          }
          else
          {
            LOG_ERROR_3(OUTPUT_PREFIX "unable to locate device for given port class "
                        "(%s) --> 0x%lX", portClass->s_name, long(ghostResult))
            okSoFar = false;
          } 
        }
        if (okSoFar)
        {
          // We've selected the device, so connect to it...
          ghostResult = GhOpen(xx->fGhostStack);
          if (PBK_IS_SUCCESS(ghostResult) || PBK_IS_INFO(ghostResult))
            xx->fDeviceOpen = true;
          else
          {
            LOG_ERROR_2(OUTPUT_PREFIX "unable to open connection to device --> 0x%lX",
                        long(ghostResult))
            okSoFar = false;
          }
        }
        if (okSoFar)
        {
          ghostResult = GhGetRetries(xx->fGhostStack, &xx->fExecuteRetries,
                                      &xx->fDownloadRetries);
          if (PBK_IS_ERROR(ghostResult) || PBK_IS_WARNING(ghostResult))
          {
            LOG_ERROR_2(OUTPUT_PREFIX "unable to get retry counts --> 0x%lX",
                        long(ghostResult))
            okSoFar = false;
          }
        }
        if (okSoFar)
        {
          // Set the notification mode...
  #if defined(USE_NOWAIT)
          ghostResult = GhSetNoWaitMode(xx->fGhostStack, rcxNotify);
          if (PBK_IS_ERROR(ghostResult) || PBK_IS_WARNING(ghostResult))
          {
            LOG_ERROR_2(OUTPUT_PREFIX "unable to set device to no-wait mode --> 0x%lX",
                        long(ghostResult))
            okSoFar = false;
          }
  #else /* not USE_NOWAIT */
          //ghostResult = GhSetWaitMode(xx->fGhostStack, rcxNotify);
          //if (PBK_IS_ERROR(ghostResult) || PBK_IS_WARNING(ghostResult))
          //{
          //  LOG_ERROR_2(OUTPUT_PREFIX "unable to set device to wait mode --> 0x%lX",
          //              long(ghostResult))
          //  okSoFar = false;
          //}
  #endif /* not USE_NOWAIT */
        }
 #endif /* COMPILE_FOR_OS9_4 */
      }
      else
      {
        LOG_ERROR_1(OUTPUT_PREFIX "unable to create port or clock for device")
        okSoFar = false;
      }
    }
    if (! okSoFar)
    {
      freeobject(reinterpret_cast<PObject>(xx));
      xx = NULL_PTR;
    }
  }
#endif /* FOR_MAC_PPC */
  ExitCodeResource();
  return xx;
} /* rcxCreate */

/*------------------------------------ rcxFree ---*/
Pvoid rcxFree
  (RcxControlPtr xx)
{
  EnterCallback();
  if (xx)
  {
#if FOR_MAC_PPC
 #if defined(COMPILE_FOR_OSX_4)
  	IOReturn	operResult;
  	ULONG			relResult;
  	
	  xx->fReportEvents = false;
	  xx->fStopping = true;
	  if (xx->fDeviceOpen)
	  {
	  	operResult = closeUSBInterface(xx->fInterface);
	  	if (operResult != KERN_SUCCESS)
	  		LOG_POST_3("close interface returns: %d (0x%x)", operResult, operResult);
	  	relResult = releaseUSBInterface(xx->fInterface);
  		//LOG_POST_3("release interface returns: %ld (0x%lx)", relResult, relResult);//!!
  		xx->fInterface = NULL_PTR;
	  }
	  if (xx->fDevice)
	  {
	  	operResult = closeUSBDevice(xx->fDevice);
	  	if (operResult != KERN_SUCCESS)
	  		LOG_POST_3("close device returns: %d (0x%x)", operResult, operResult);
	  	relResult = releaseUSBDevice(xx->fDevice);
	  	//LOG_POST_3("release device returns: %ld (0x%lx)", relResult, relResult);//!!
	  	xx->fDevice = NULL_PTR;
	  }
	  xx->fDeviceOpen = false;
	  shutDownIOKit(xx->fUSBControl);
    FREEBYTES(xx->fReceiveBuffer, MAX_RECEIVE_BUFFER)
    FREEBYTES(xx->fRawReceiveBuffer, kReadPacketSize)
	  gensym(RCX_CONTROL_SYMBOL)->s_thing = NULL_PTR;
	  if (xx->fReadCompletion)
	  	releaseCallback(xx->fReadCompletion);
	  xx->fReadCompletion = NULL_PTR;
 #endif /* COMPILE_FOR_OSX_4 */
 #if defined(COMPILE_FOR_OS9_4)
    if (xx->fGhostStack)
    {
      PBKRESULT ghostResult;

      if (xx->fDeviceOpen)
      {
        ghostResult = GhClose(xx->fGhostStack);
        if (PBK_IS_ERROR(ghostResult) || PBK_IS_WARNING(ghostResult))
          LOG_ERROR_2(OUTPUT_PREFIX "unable to close port --> 0x%lX", long(ghostResult))
      }
      ghostResult = GhDestroyStack(xx->fGhostStack);
      if (PBK_IS_ERROR(ghostResult) || PBK_IS_WARNING(ghostResult))
        LOG_ERROR_2(OUTPUT_PREFIX "unable to free Ghost stack --> 0x%lX",
                    long(ghostResult))
      xx->fGhostStack = NULL_PTR;
    }
 #endif /* COMPILE_FOR_OS9_4 */
    FREEBYTES(xx->fReplyBuffer, MAX_REPLY_BUFFER)
#endif /* FOR_MAC_PPC */
  }
  LOG_EXIT()
  ExitMaxMessageHandler()
} /* rcxFree */

/*------------------------------------ rcxGetReplyByte ---*/
uchar rcxGetReplyByte
  (RcxControlPtr	xx,
   const ushort		index)
{
  if (xx)
    return *(xx->fReplyBuffer + index + 1);

  return 0;
} /* rcxGetReplyByte */

/*------------------------------------ rcxGetValue ---*/
bool rcxGetValue
  (RcxControlPtr	xx,
   const ulong		descriptor,
   long &					result)
{
  uchar valueCommand[] = { RCX_POLL_CMD, 0, 0 };

  valueCommand[1] = GET_RCX_VALUE_TYPE(descriptor);
  valueCommand[2] = static_cast<uchar>(GET_RCX_VALUE_DATA(descriptor));
  if (rcxSendCommand(xx, valueCommand, sizeof(valueCommand), RCX_POLL_REPLY, true))
  {
    uchar val1 = rcxGetReplyByte(xx, 0);
    uchar val2 = rcxGetReplyByte(xx, 1);

    result = ((val2 << 8) + val1);
    return true;

  }	
  return false;
} /* rcxGetValue */

#if defined(COMPILE_FOR_OSX_4)
/*------------------------------------ rcxFrameData ---*/
static Puchar rcxFrameData
	(RcxControlPtr	xx,
	 Quchar					sendData,
	 const UInt32		sendLength,
	 UInt32 &				framedSize,
	 const bool			reduceDuplicates)
{
	// Determine the size of the 'framed' data:
	framedSize = sendLength	+ (xx->fComplementData ? (sendLength + 2) : 1) +
									(xx->fFastMode ? 1 : 3);
	Puchar	framedData = GETBYTES(framedSize, uchar);
	Puchar	outWalker = framedData;
	
	// Set the header:
	if (xx->fFastMode)
		*outWalker++ = 0xFF;
	else
	{
		*outWalker++ = 0x55;
		*outWalker++ = 0xFF;
		*outWalker++ = 0x00;
	}
	Quchar	inWalker = sendData;
	uchar		aByte;
	UInt32	ii, dataSum = 0;
	
	if (xx->fComplementData)
	{
		// Interleaved data and complemented data...
		if (sendLength > 0)
		{
			aByte = *inWalker++;
			if (reduceDuplicates && (aByte == xx->fLastCommand))
				aByte ^= 8; // duplicate reduction...
			xx->fLastCommand = aByte;
			*outWalker++ = aByte;
			*outWalker++ = static_cast<uchar>(~aByte);
			dataSum += aByte;
		}
		for (ii = 1; ii < sendLength; ++ii, ++inWalker)
		{
			aByte = *inWalker;
			*outWalker++ = aByte;
			*outWalker++ = static_cast<uchar>(~aByte);
			dataSum += aByte;
		}
	}
	else
	{
		// Just the data...
		if (sendLength > 0)
		{
			aByte = *inWalker++;
			if (reduceDuplicates && (aByte == xx->fLastCommand))
				aByte ^= 8; // duplicate reduction...
			xx->fLastCommand = aByte;
			*outWalker++ = aByte;
			dataSum += aByte;
		}
		for (ii = 1; ii < sendLength; ++ii, ++inWalker, ++outWalker)
		{
			*outWalker = aByte = *inWalker;
			dataSum += aByte;
		}
	}
	// Write the checksum:
	*outWalker++ = static_cast<uchar>(dataSum);
	if (xx->fComplementData)
		*outWalker = static_cast<uchar>(~dataSum);
	return framedData;
} /* rcxFrameData */
#endif /* COMPILE_FOR_OSX_4 */

#if defined(COMPILE_FOR_OSX_4)
/*------------------------------------ rcxReadData ---*/
static UInt32 rcxReadData
	(RcxControlPtr	xx,
	 Puchar					buffer,
	 const UInt32		bufferSize,
	 const UInt32		timeout)
{
	xx->fReadWalker = buffer;
	xx->fReadRemaining = bufferSize;
	xx->fReadComplete = false;
	rcxCopyFromRawBuffer(xx);
	if (! xx->fReadComplete)
	{
		CFStringRef		defaultRunLoopMode;
		
		if (getRunLoopDefaultMode(xx->fUSBControl, defaultRunLoopMode))
		{
			rcxTriggerRead(xx);
			do
			{
				SInt32	reason = runLoopRunInMode(xx->fUSBControl, defaultRunLoopMode,
																					timeout / 1000.0, true);
				
				if (reason == kCFRunLoopRunTimedOut)
				{
					abortUSBPipe(xx->fInterface, kReadPipe);
					xx->fReadComplete = true;
				}
			}
			while (! xx->fReadComplete);
		}
	}
	return static_cast<UInt32>(xx->fReadWalker - buffer);			
} /* rcxReadData */
#endif /* COMPILE_FOR_OSX_4 */

#if defined(COMPILE_FOR_OSX_4)
/*------------------------------------ rcxDrainReadBuffer ---*/
static void rcxDrainReadBuffer
	(RcxControlPtr	xx)
{
	Puchar	buff = GETBYTES(MAX_REPLY_BUFFER, uchar);
	
	for ( ; rcxReadData(xx, buff, MAX_REPLY_BUFFER, 10) > 0; )
	{
	}
	FREEBYTES(buff, MAX_REPLY_BUFFER)
} /* rcxDrainReadBuffer */
#endif /* COMPILE_FOR_OSX_4 */

#if defined(COMPILE_FOR_OSX_4)
/*------------------------------------ rcxFindSync ---*/
static UInt32 rcxFindSync
	(RcxControlPtr	xx,
	 const UInt32		offset)
{
	Quchar	data = xx->fReceiveBuffer + offset;
	Quchar	sync = xx->fSync;
	UInt32	length = xx->fReceiveLength - offset;
	uchar		checkCmd = static_cast<uchar>((~xx->fLastCommand) & 0x00F7);
	
	for (UInt32 syncLen = xx->fSyncLen; syncLen > 0; ++sync, --syncLen)
	{
		Quchar	end = data + length - syncLen + 1;
		
		for (Quchar ptr = data; ptr < end; ++ptr)
		{
			UInt32	ii;
			
			for (ii = 0; ii < syncLen; ++ii)
			{
				if (*(ptr + ii) != *(sync + ii))
					break;
			
			}
			if ((ii == syncLen) && ((*(ptr + syncLen) & 0x00F7) == checkCmd))
				return (ptr - data + syncLen);

		} 
	}
	return 0;
} /* rcxFindSync */
#endif /* COMPILE_FOR_OSX_4 */

#if defined(COMPILE_FOR_OSX_4)
/*------------------------------------ rcxVerifyReply ---*/
static UInt32 rcxVerifyReply
	(RcxControlPtr	xx,
	 const UInt32		offset)
{
	Quchar	data = xx->fReceiveBuffer + offset;
	UInt32	length = xx->fReceiveLength - offset, width = static_cast<UInt32>(xx->fComplementData ? 2 : 1);
	Quchar	ptr = data, match = NULL_PTR, end = data + length + 1 - width;
	uchar		dataSum = *data;
	bool		complementCmd;

	if (xx->fFastMode && ((xx->fLastCommand & 0x00F7) == 0x00A5))
		complementCmd = true;
	else
		complementCmd = xx->fComplementData;
	if (length < ((complementCmd ? 2 : 1) + width))
		return 0;

	if ((*ptr++ & 0x00F7) != (~xx->fLastCommand & 0x00F7))
		return 0;

	if (complementCmd)
	{
		if ((*ptr++ & 0x00F7) != (xx->fLastCommand & 0x00F7))
			return 0;

	}
	if (xx->fComplementData)
	{
		for ( ; ptr < end; ptr += 2)
		{
			if ((*ptr & 0x00F7) != (~*(ptr + 1) & 0x00F7))
				break;
			
			if (*ptr == dataSum)
				match = ptr;
			dataSum += *ptr;
		}
	}
	else
	{
		for ( ; ptr < end; ++ptr)
		{
			if (*ptr == dataSum)
				match = ptr;
			dataSum += *ptr;
		}
	}
	if (! match)
		return 0;

	return ((match - data) / width);
} /* rcxVerifyReply */
#endif /* COMPILE_FOR_OSX_4 */

#if defined(COMPILE_FOR_OSX_4)
/*------------------------------------ rcxFindReply ---*/
static UInt32 rcxFindReply
	(RcxControlPtr	xx,
	 UInt32 &				offset)
{
	offset = 0;
	for ( ; ; )
	{
		UInt32	start = rcxFindSync(xx, offset);
		
		if (! start)
			break;
			
		offset += start;
		UInt32	length = rcxVerifyReply(xx, offset);
		
		if (length > 0)
			return length;
	
	}
	return 0;
} /* rcxFindReply */
#endif /* COMPILE_FOR_OSX_4 */

#if defined(COMPILE_FOR_OSX_4)
/*------------------------------------ rcxReceiveReply ---*/
static UInt32 rcxReceiveReply
	(RcxControlPtr	xx,
	 const UInt32		expected,
	 const UInt32		timeout,
	 UInt32 &				replyOffset)
{
	// Determine the number of bytes to watch for:
	UInt32	framedSize = expected	+ (xx->fComplementData ? (expected + 2) : 1) +
													(xx->fFastMode ? 1 : 3);

	xx->fReceiveLength = 0;
	UInt32	length = 0;
	UInt32	firstChunk = rcxReadData(xx, xx->fReceiveBuffer, framedSize, timeout);
	
	if (firstChunk)
	{
		xx->fReceiveLength = firstChunk;
		// check for replies
		length = rcxFindReply(xx, replyOffset);
		if (length != expected)
		{
			for ( ; xx->fReceiveLength < MAX_RECEIVE_BUFFER; )
			{
				if (rcxReadData(xx, xx->fReceiveBuffer + xx->fReceiveLength, 1, timeout) != 1)
					break;
					
				++xx->fReceiveLength;
				// check for replies
				length = rcxFindReply(xx, replyOffset);
				if (length == expected)
					break;

			}
		}
	}
	return length;	
} /* rcxReceiveReply */
#endif /* COMPILE_FOR_OSX_4 */

#if defined(COMPILE_FOR_OSX_4)
/*------------------------------------ rcxCopyToReply ---*/
static void rcxCopyToReply
  (RcxControlPtr	xx,
   const UInt32		offset,
   const UInt32		count)
{
	UInt32	width = static_cast<UInt32>(xx->fComplementData ? 2 : 1);
	Quchar	data = xx->fReceiveBuffer + offset;
	Puchar	ptr = xx->fReplyBuffer;
	bool		complementCmd;
	
	if (xx->fFastMode && ((xx->fLastCommand & 0x00F7) == 0x00A5))
		complementCmd = true;
	else
		complementCmd = xx->fComplementData;
	*ptr++ = *data++; // copy the command
	if (complementCmd)
		++data;
	for (UInt32 ii = 1; ii < count; ++ii, ++ptr, data += width)
		*ptr = *data;
	xx->fReplyLength = count;
	if (xx->fVerbose)
	{
		LOG_POST_2("reply len:%ld", xx->fReplyLength)
		data = xx->fReplyBuffer;
    switch (xx->fReplyLength)
    {
    	case 0:
    		break;
    		
    	case 1:
    		LOG_POST_2("  0x%lx", long(*data))
    		break;
    		
    	case 2:
    		LOG_POST_3("  0x%lx 0x%lx", long(*data), long(*(data + 1)))
    		break;
    		
    	case 3:
    		LOG_POST_4("  0x%lx 0x%lx 0x%lx", long(*data), long(*(data + 1)),
    								long(*(data + 2)))
    		break;
    		
    	case 4:
    		LOG_POST_5("  0x%lx 0x%lx 0x%lx 0x%lx", long(*data), long(*(data + 1)),
    								long(*(data + 2)), long(*(data + 3)))
    		break;
    		
    	case 5:
    		LOG_POST_6("  0x%lx 0x%lx 0x%lx 0x%lx 0x%lx", long(*data),
    								long(*(data + 1)), long(*(data + 2)),
    								long(*(data + 3)), long(*(data + 4)))
    		break;

			default:
    		LOG_POST_6("  0x%lx 0x%lx 0x%lx 0x%lx 0x%lx ...", long(*data),
    								long(*(data + 1)), long(*(data + 2)),
    								long(*(data + 3)), long(*(data + 4)))
    		break;
			      		
    }
	}
} /* rcxCopyToReply */
#endif /* COMPILE_FOR_OSX_4 */

#if defined(COMPILE_FOR_OS9_4)
/*------------------------------------ rcxNotify ---*/
UInt32 rcxNotify
  (GHNOTIFYCODE notifyCode,
   PBKRESULT    resultCode,
   GHSTACK      ghostStack,
   GHQUEUE      queue,
   GHCOMMAND    command,
   Pvoid        context)
{
 #if FOR_MAC_PPC
  #pragma unused(ghostStack,queue)
 #else /* not FOR_MAC_PPC */
  #pragma unused(notifyCode, resultCode, ghostStack, queue, command, context)
 #endif /* not FOR_MAC_PPC */
  RcxControlPtr xx = static_cast<RcxControlPtr>(context);
  uint32        retCode = 1;

 #if FOR_MAC_PPC
  LOG_POST_3(OUTPUT_PREFIX "notify triggered notifyCode=%d, resultCode=0x%lX",
              notifyCode, resultCode)
  if (notifyCode == NotifyCommand)
  {
    uchar     cmd[256], reply[256];
    uint32    cmdLen, replyLen;
    PBKRESULT ghostResult;
    bool      okSoFar = true;

    ghostResult = GhGetCommandDataLen(command, &cmdLen);
    if ((! PBK_IS_SUCCESS(ghostResult)) && (! PBK_IS_INFO(ghostResult)))
    {
      LOG_ERROR_2(OUTPUT_PREFIX "GhGetCommandDataLen failed --> 0x%lX", long(ghostResult))
      okSoFar = false;
    }
    if (okSoFar)
    {
      ghostResult = GhGetCommandData(command, cmd, sizeof(cmd)); 
      if ((! PBK_IS_SUCCESS(ghostResult)) && (! PBK_IS_INFO(ghostResult)))
      {
        LOG_ERROR_2(OUTPUT_PREFIX "GhGetCommandData failed --> 0x%lX", long(ghostResult))
        okSoFar = false;
      }
    }
    if (okSoFar)
    {
      ghostResult = GhGetCommandReplyLen(command, &replyLen); 
      if ((! PBK_IS_SUCCESS(ghostResult)) && (! PBK_IS_INFO(ghostResult)))
      {
        LOG_ERROR_2(OUTPUT_PREFIX "GhGetCommandReplyLen failed --> 0x%lX",
                    long(ghostResult))
        okSoFar = false;
      }
    }
    if (okSoFar)
    {
      if (replyLen > 0) 
      { 
        ghostResult = GhGetCommandReply(command, reply, sizeof(reply)); 
        if ((! PBK_IS_SUCCESS(ghostResult)) && (! PBK_IS_INFO(ghostResult)))
        {
          LOG_ERROR_2(OUTPUT_PREFIX "GhGetCommandReply failed --> 0x%lX",
                      long(ghostResult))
          okSoFar = false;
        }
      }
    }
    if (okSoFar)
    {
      if (cmdLen > 0)
        LOG_POST_2("Command code: 0x%lX", long(*cmd))
      if (PBK_IS_SUCCESS(resultCode) || PBK_IS_INFO(resultCode)) 
      { 
        if (replyLen > 0)
          LOG_POST_3("Succeeded Reply: 0x%lX (length=%ld)", long(*reply), long(replyLen))
        else
          LOG_POST_1("Succeeded")
      } 
      else 
      { 
        LOG_ERROR_2(OUTPUT_PREFIX "Failed --> 0x%lX", resultCode)
        // Cause the next commands to be aborted
        retCode = 0;
      }
    }
    else
      retCode = 0;
  }
  return retCode;
 #else /* not FOR_MAC_PPC */
  return 0;
 #endif /* not FOR_MAC_PPC */
} /* rcxNotify */
#endif /* COMPILE_FOR_OS9_4 */

/*------------------------------------ rcxSendCommand ---*/
bool rcxSendCommand
  (RcxControlPtr	xx,
   const uchar *	sendData,
   const UInt32   sendLength,
   const UInt32   expected,
   const bool     doRetry)
{
#if (! FOR_MAC_PPC)
 #pragma unused(sendData, sendLength, expected, doRetry)
#endif /* not FOR_MAC_PPC */
  bool okSoFar = true, actualRetry = doRetry;

  if (xx)
  {
#if FOR_MAC_PPC
    if (! expected)
      actualRetry = false;
    if (xx->fVerbose)
    {
      LOG_POST_5("cmd:0x%lx len:%ld expected:%ld retry:%d", static_cast<ulong>(*sendData), sendLength,
                  expected, long(actualRetry))
      switch (sendLength)
      {
      	case 0:
      		break;
      		
      	case 1:
      		LOG_POST_2("  0x%lx", long(*sendData))
      		break;
      		
      	case 2:
      		LOG_POST_3("  0x%lx 0x%lx", long(*sendData), long(*(sendData + 1)))
      		break;
      		
      	case 3:
      		LOG_POST_4("  0x%lx 0x%lx 0x%lx", long(*sendData), long(*(sendData + 1)),
      								long(*(sendData + 2)))
      		break;
      		
      	case 4:
      		LOG_POST_5("  0x%lx 0x%lx 0x%lx 0x%lx", long(*sendData), long(*(sendData + 1)),
      								long(*(sendData + 2)), long(*(sendData + 3)))
      		break;
      		
      	case 5:
      		LOG_POST_6("  0x%lx 0x%lx 0x%lx 0x%lx 0x%lx", long(*sendData),
      								long(*(sendData + 1)), long(*(sendData + 2)),
      								long(*(sendData + 3)), long(*(sendData + 4)))
      		break;

				default:
      		LOG_POST_6("  0x%lx 0x%lx 0x%lx 0x%lx 0x%lx ...", long(*sendData),
      								long(*(sendData + 1)), long(*(sendData + 2)),
      								long(*(sendData + 3)), long(*(sendData + 4)))
      		break;
				      		
      }
    }
 #if defined(COMPILE_FOR_OSX_4)
		UInt32	framedSize = 0;
		Puchar	framedData = rcxFrameData(xx, sendData, sendLength, framedSize, actualRetry);
		int			numTries = (doRetry ? 4 : 1);

		for (int ii = 0; ii < numTries; ++ii)
		{		
			Puchar	walker = framedData;
			UInt32	actualSize = framedSize;
			
			for ( ; actualSize > 0; )
			{
				rcxDrainReadBuffer(xx);			
				UInt32		transfer = ((actualSize > kMaxWritePacket) ? kMaxWritePacket : actualSize);
				IOReturn	err = writeUSBPipe(xx->fInterface, kWritePipe, walker, transfer);
				
				if (err != KERN_SUCCESS)
				{
					LOG_POST_3("writeUSBPipe returns: %d (0x%x)", err, err);
					okSoFar = false;
					break;
					
				}
				actualSize -= transfer;
				walker += transfer;
			}
			if (! expected)
				break;

			UInt32	replyOffset, result = rcxReceiveReply(xx, expected, 500, replyOffset);
			
			if (result > 0)
			{
				rcxCopyToReply(xx, replyOffset, result);
				break;
				
			}
		}
    FREEBYTES(framedData, framedSize)		
 #endif /* COMPILE_FOR_OSX_4 */
 #if defined(COMPILE_FOR_OS9_4)
    PBKRESULT ghostResult = PBKOK;
    GHQUEUE   aQueue = NULL_PTR;

    ghostResult = GhCreateCommandQueue(&aQueue);
    if (PBK_IS_ERROR(ghostResult) || PBK_IS_WARNING(ghostResult))
    {
      LOG_ERROR_2(OUTPUT_PREFIX "unable to create command queue --> 0x%lX",
                  long(ghostResult))
      okSoFar = false;
    }
    if (okSoFar)
    {
      ghostResult = GhSetQueueContext(aQueue, xx);
      if (PBK_IS_ERROR(ghostResult) || PBK_IS_WARNING(ghostResult))
      {
        LOG_ERROR_2(OUTPUT_PREFIX "unable to set queue context --> 0x%lX",
                    long(ghostResult))
        okSoFar = false;
      }
    }
    if (okSoFar)
    {
      ghostResult = GhAppendCommand(aQueue, const_cast<Puchar>(sendData), sendLength, expected);
      if (PBK_IS_ERROR(ghostResult) || PBK_IS_WARNING(ghostResult))
      {
        LOG_ERROR_2(OUTPUT_PREFIX "unable to add to command queue --> 0x%lX",
                    long(ghostResult))
        okSoFar = false;
      }			
    }
    if (okSoFar)
    {
      if (actualRetry)
        ghostResult = GhExecute(xx->fGhostStack, aQueue);
      else
      {
        GhSetRetries(xx->fGhostStack, 0, 0);
        ghostResult = GhExecute(xx->fGhostStack, aQueue);
        GhSetRetries(xx->fGhostStack, xx->fExecuteRetries, xx->fDownloadRetries);
      }
      if (PBK_IS_SUCCESS(ghostResult) || PBK_IS_INFO(ghostResult))
      {
  #if (! defined(USE_NOWAIT))
        if (expected)
        {
          GHCOMMAND aCommand;

          ghostResult = GhGetFirstCommand(aQueue, &aCommand);
          if (PBK_IS_ERROR(ghostResult) || PBK_IS_WARNING(ghostResult))
          {
            LOG_ERROR_2(OUTPUT_PREFIX "unable to get first command in queue --> 0x%lX",
                        long(ghostResult))
            okSoFar = false;
          }
          if (okSoFar)
          {
            ghostResult = GhGetCommandReplyLen(aCommand, &xx->fReplyLength);
            if (PBK_IS_ERROR(ghostResult) || PBK_IS_WARNING(ghostResult))
            {
              LOG_ERROR_2(OUTPUT_PREFIX "unable to get reply length --> 0x%lX",
                          long(ghostResult))
              okSoFar = false;
            }
          }
          if (okSoFar)
          {
            if (xx->fReplyLength > MAX_REPLY_BUFFER)
              xx->fReplyLength = MAX_REPLY_BUFFER;
            ghostResult = GhGetCommandReply(aCommand, xx->fReplyBuffer, xx->fReplyLength);
            if (PBK_IS_ERROR(ghostResult) || PBK_IS_WARNING(ghostResult))
            {
              LOG_ERROR_2(OUTPUT_PREFIX "unable to get reply --> 0x%lX",
                          long(ghostResult))
              okSoFar = false;
            }
          }	
        }
  #endif /* not USE_NOWAIT */
      }
      else
      {
        LOG_ERROR_2(OUTPUT_PREFIX "problem executing commands --> 0x%lX",
                    long(ghostResult))
        okSoFar = false;
      }
    }
  #if (! defined(USE_NOWAIT))
    if (aQueue)
      GhDestroyCommandQueue(aQueue);
  #endif /* not USE_NOWAIT */
 #endif /* COMPILE_FOR_OS9_4 */
#endif /* FOR_MAC_PPC */
  }
  return okSoFar;
} /* rcxSendCommand */

#if defined(COMPILE_FOR_OSX_4)
/*------------------------------------ rcxSendControlRequest ---*/
bool rcxSendControlRequest
	(RcxControlPtr	xx,
	 const uchar		controlOperation,
	 const ushort		controlData,
	 Pvoid					reply,
	 const UInt16		replySize)
{
	IOReturn					result;
	IOUSBDevRequestTO	request;

	request.bmRequestType = (kUSBVendor << kUSBRqTypeShift) + (kUSBIn << kUSBRqDirnShift) + kUSBDevice;
	request.bRequest = controlOperation;
	request.wValue = controlData;
	request.wIndex = 0;
	request.wLength = replySize;
	request.pData = reply;
	request.noDataTimeout = kUSBDefaultControlNoDataTimeoutMS;
	request.completionTimeout = 0;
	result = sendUSBControlRequestTO(xx->fInterface, 0, request);
	return (result == KERN_SUCCESS);
} /* rcxSendControlRequest */
#endif /* COMPILE_FOR_OSX_4 */

#if defined(COMPILE_FOR_OSX_4)
/*------------------------------------ rcxSendControlRequest ---*/
bool rcxSendControlRequest
	(RcxControlPtr	xx,
	 const uchar		controlOperation,
	 const uchar		controlDataLow,
	 const uchar		controlDataHigh,
	 Pvoid					reply,
	 const UInt16		replySize)
{
	IOReturn					result;
	IOUSBDevRequestTO	request;

	request.bmRequestType = (kUSBVendor << kUSBRqTypeShift) + (kUSBIn << kUSBRqDirnShift) + kUSBDevice;
	request.bRequest = controlOperation;
	request.wValue = static_cast<UInt16>((controlDataHigh << 8) + controlDataLow);
	request.wIndex = 0;
	request.wLength = replySize;
	request.pData = reply;
	request.noDataTimeout = kUSBDefaultControlNoDataTimeoutMS;
	request.completionTimeout = 0;
	result = sendUSBControlRequestTO(xx->fInterface, 0, request);
	return (result == KERN_SUCCESS);
} /* rcxSendControlRequest */
#endif /* COMPILE_FOR_OSX_4 */

#if defined(COMPILE_FOR_OSX_4)
/*------------------------------------ rcxSetRange ---*/
bool rcxSetRange
	(RcxControlPtr	xx,
	 const LTWRange	range)
{
	LTWRequestReply	reply;
	
	return rcxSendControlRequest(xx, LTW_REQ_SET_PARM, LTW_PARM_RANGE, range, &reply, sizeof(reply));
} /* rcxSetRange */
#endif /* COMPILE_FOR_OSX_4 */
	 
#if defined(COMPILE_FOR_OSX_4)
/*------------------------------------ rcxSetSpeed ---*/
bool rcxSetSpeed
	(RcxControlPtr	xx,
	 const bool			normalSpeed)
{
	LTWRequestSpeedReply	reply;
	LTWRequestTxFreqReply	reply2;
	bool									didIt = false;

	if (normalSpeed)
		didIt = rcxSendControlRequest(xx, LTW_REQ_SET_RX_SPEED, SPEED_COMM_BAUD_2400, &reply, sizeof(reply)) &&
							rcxSendControlRequest(xx, LTW_REQ_SET_TX_SPEED, SPEED_COMM_BAUD_2400, &reply, sizeof(reply));
	else
		didIt = rcxSetRange(xx, LTW_RANGE_SHORT) &&
						rcxSendControlRequest(xx, LTW_REQ_SET_RX_SPEED, SPEED_COMM_BAUD_4800, &reply, sizeof(reply)) &&
						rcxSendControlRequest(xx, LTW_REQ_SET_TX_SPEED, SPEED_COMM_BAUD_4800, &reply, sizeof(reply)) &&
						rcxSendControlRequest(xx, LTW_REQ_SET_TX_CARRIER_FREQUENCY, 38, &reply2, sizeof(reply2)); /* 38 kHz */
	return didIt;
} /* rcxSetSpeed */
#endif /* COMPILE_FOR_OSX_4 */

/*------------------------------------ rcxSynchronize ---*/
bool rcxSynchronize
  (RcxControlPtr xx)
{
  bool didIt = false;

  if (xx)
  {
#if FOR_MAC_PPC
    if (xx->fSynchronized)
      didIt = true;
    else
    {
      static uchar pingCommand[] = { RCX_PING_CMD };

      // Start with a ping
      didIt = rcxSendCommand(xx, pingCommand, sizeof(pingCommand), RCX_PING_REPLY, true);
      if (! didIt)
        didIt = rcxSendCommand(xx, pingCommand, sizeof(pingCommand), RCX_PING_REPLY,
                                true);
      xx->fSynchronized = didIt;
    }
#endif /* FOR_MAC_PPC */
  }
  return didIt;
} /* rcxSynchronize */

StandardInfoRoutine(RcxControlPtr)