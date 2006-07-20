/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       Common_HIDXData.h                                                       */
/*                                                                                      */
/*  Contains:   The data type declarations and forward references to routines.          */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2004 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2004/02/09                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(COMMON_HIDXDATA_H_))
 #define COMMON_HIDXDATA_H_ /* */

 #if defined(COMPILE_FOR_CATS)
// The following datatypes and defines are copied from the relevant Framework headers,
// which aren't directly useable in CFM source:

struct IOHIDEventStruct
{
	IOHIDElementType		type;
	IOHIDElementCookie	elementCookie;
	SInt32							value;
	AbsoluteTime				timestamp;
	UInt32							longValueSize;
	void *							longValue;
};

struct IOHIDQueueInterface;

struct IOHIDOutputTransactionInterface;

/*! @typedef IOHIDElementCallbackFunction
    @discussion Type and arguments of callout C function that is used when a
                completion routine is called, see IOHIDLib.h:setElementValue().
    @param target void * pointer to your data, often a pointer to an object.
    @param result completion result of desired operation
    @param refcon void * pointer to more data.
    @param sender interface instance sending the completion routine.
    @param elementCookie element within interface instance sending completion.
*/
typedef void (*IOHIDElementCallbackFunction)
              (void *	 		target,
               IOReturn 		result,
               void * 			refcon,
               void * 			sender,
               IOHIDElementCookie 	elementCookie);

/*! @typedef IOHIDReportCallbackFunction
    @discussion Type and arguments of callout C function that is used when a
                completion routine is called, see IOHIDLib.h:setReport().
    @param target void * pointer to your data, often a pointer to an object.
    @param result completion result of desired operation
    @param refcon void * pointer to more data.
    @param sender interface instance sending the completion routine.
    @param bufferSize size of the buffer received upon completion.
*/
typedef void (*IOHIDReportCallbackFunction)
              (void *	 		target,
               IOReturn 		result,
               void * 			refcon,
               void * 			sender,
               UInt32		 	bufferSize);

struct IOHIDDeviceInterface
{
    IUNKNOWN_C_GUTS;

    /* Completion plumbing (must be set before any callbacks used) */
    /* Either createAsyncEventSource or createAsyncPort should be used */
    IOReturn (*createAsyncEventSource)(void * 			self,
                                       CFRunLoopSourceRef * 	source);
    CFRunLoopSourceRef (*getAsyncEventSource)(void * self);

    IOReturn (*createAsyncPort)(void * self, mach_port_t * port);
    mach_port_t (*getAsyncPort)(void * self);
    
    /* Basic IOHIDDevice interface */
    IOReturn (*open)(void * self, UInt32 flags);
    IOReturn (*close)(void * self);

    /* removalCallback is called if the device is removed. */
    /* removeTarget and removalRefcon are passed to the callback. */
    IOReturn (*setRemovalCallback)(void * 			self,
                                   IOHIDCallbackFunction	removalCallback,
                                   void *			removalTarget,
                                   void *			removalRefcon);


/*! @function getElementValue
    @abstract Called to obtain the most recent value of an element
    @discussion This call is most useful for interrupt driven elements,
        such as input type elements.  Since feature type element values 
        need to be polled from the device, it is recommended to use the 
        queryElementValue method to obtain the current value.  The  
        timestamp field in the event details the last time the element 
        value was altered.
    @param elementCookie The element of interest. 
    @param valueEvent The event that will be filled.   If a long value is 
        present, it is up to the caller to deallocate it. */
    IOReturn (*getElementValue)(void * 			self,
                                IOHIDElementCookie	elementCookie,
                                IOHIDEventStruct *	valueEvent);


/*! @function setElementValue
    @abstract Called to set an element value on the device
    @discussion This call is most useful for feature type elements.  It is
        recommended to use IOOutputTransaction for output type elements
    @param elementCookie The element of interest. 
    @param valueEvent The event that will be filled.  If a long value is
        present, it will be copied.

    *** THIS METHOD IS SYNCHRONOUS ***
    *** THE FOLLOWING PARAMS ARE UNSUPPORTED ***
    @param timeoutMS
    @param callback 
    @param callbackTarget
    @param callbackRefcon */
    IOReturn (*setElementValue)(void *	 			self,
                                IOHIDElementCookie		elementCookie,
                                IOHIDEventStruct *		valueEvent,
                                UInt32 				timeoutMS,
                                IOHIDElementCallbackFunction	callback,
                                void * 				callbackTarget,
                                void *				callbackRefcon);


/*! @function queryElementValue
    @abstract Called to obtain the current value of an element
    @discussion This call is most useful for feature type elements.  This
        method will poll the device for the current element value.
    @param elementCookie The element of interest. 
    @param valueEvent The event that will be filled.  If a long value is 
        present, it is up to the caller to deallocate it.

    *** THIS METHOD IS SYNCHRONOUS ***
    *** THE FOLLOWING PARAMS ARE UNSUPPORTED ***
    @param timeoutMS
    @param callback 
    @param callbackTarget
    @param callbackRefcon */
    IOReturn (*queryElementValue)(void * 			self,
                                IOHIDElementCookie		elementCookie,
                                IOHIDEventStruct *		valueEvent,
                                UInt32 				timeoutMS,
                                IOHIDElementCallbackFunction	callback,
                                void * 				callbackTarget,
                                void *				callbackRefcon);

/*! @function startAllQueues
    @abstract Start data delivery on all queue for a this device */
    IOReturn (*startAllQueues)(void * self);
    
/*! @function stopAllQueues
    @abstract Stop data delivery on all queue for a this device */
    IOReturn (*stopAllQueues)(void * self);

/*! @function allocQueue
    @abstract Wrapper to return instances of the IOHIDQueueInterface */
    IOHIDQueueInterface ** (*allocQueue) (void *self);
    
/*! @function allocOutputTransaction
    @abstract Wrapper to return instances of the IOHIDOutputTransactionInterface */
    IOHIDOutputTransactionInterface ** (*allocOutputTransaction) (void *self);
    
    
/*! @function setReport
    @abstract Called to send a report to the device
    @discussion
    @param reportType The report type.
    @param reportID The report id.
    @param reportBuffer Pointer to a preallocated buffer.
    @param reportBufferSize Size of the reportBuffer in bytes.
    @param timeoutMS
    @param callback If null, this method will behave synchronously.
    @param callbackTarget The callback target passed to the callback.
    @param callbackRefcon The callback refcon passed to the callback. */
    IOReturn (*setReport)	(void * 			self,
                                IOHIDReportType			reportType,
                                UInt32				reportID,
                                void *				reportBuffer,
                                UInt32				reportBufferSize,
                                UInt32 				timeoutMS,
                                IOHIDReportCallbackFunction	callback,
                                void * 				callbackTarget,
                                void *				callbackRefcon);


/*! @function getReport
    @abstract Called to obtain a report from the device
    @discussion
    @param reportType The report type.
    @param reportID The report id.
    @param reportBuffer Pointer to a preallocated buffer.
    @param reportBufferSize Size of the reportBuffer in bytes.  
        When finished, will contain the actual size of the report.
    @param timeoutMS
    @param callback If null, this method will behave synchronously.
    @param callbackTarget The callback target passed to the callback.
    @param callbackRefcon The callback refcon passed to the callback. */
    IOReturn (*getReport)	(void * 			self,
                                IOHIDReportType			reportType,
                                UInt32				reportID,
                                void *				reportBuffer,
                                UInt32 *			reportBufferSize,
                                UInt32 				timeoutMS,
                                IOHIDReportCallbackFunction	callback,
                                void * 				callbackTarget,
                                void *				callbackRefcon);
}; /* IOHIDDeviceInterface */

/*! @typedef IOHIDQueueInterface
    @discussion CFPlugin object subclass which privides interface for input
                queues from HID devices. Created by a IOHIDDeviceInterface
                object.
*/
struct IOHIDQueueInterface
{
    IUNKNOWN_C_GUTS;
    
/*! @function createAsyncEventSource
    @abstract Called to create an async event source
    @discussion This will be used with setEventCallout.
    @param source The newly created event source */
    IOReturn (*createAsyncEventSource)(void * 			self, 
                                        CFRunLoopSourceRef * 	source);

/*! @function getAsyncEventSource
    @abstract Called to obtain the current event source */
    CFRunLoopSourceRef (*getAsyncEventSource)(void * self);

/*! @function createAsyncPort
    @abstract Called to create an async port
    @discussion This will be used with createAsyncEventSource.
    @param port The newly created async port */
    IOReturn (*createAsyncPort)(void * self, mach_port_t * port);
    
/*! @function getAsyncPort
    @abstract Called to obtain the current async port */
    mach_port_t (*getAsyncPort)(void * self);
    
/*! @function create
    @abstract Create the current queue. 
    @param flags
    @param depth The maximum number of elements in the queue 
        before the oldest elements in the queue begin to be lost. */
    IOReturn (*create)(void * 			self, 
                        UInt32 			flags,
                        UInt32			depth);

/*! @function create
    @abstract Dispose of the current queue. */
    IOReturn (*dispose)(void * self);
    
/*! @function addElement
    @abstract Called to add an element to the queue
    @discussion If the element has already been added to queue,
        an error will be returned.
    @param elementCookie The element of interest. 
    @flags */
    IOReturn (*addElement)(void * self,
                           IOHIDElementCookie elementCookie,
                           UInt32 flags);

/*! @function removeElement
    @abstract Called to remove an element to the queue
    @discussion If the element has not been added to queue,
        an error will be returned.
    @param elementCookie The element of interest. */
    IOReturn (*removeElement)(void * self, IOHIDElementCookie elementCookie);
    
/*! @function hasElement
    @abstract Called to check whether an element has been added to 
        the queue.
    @discussion Will return true if present, otherwise will return fales.
    @param elementCookie The element of interest. */
    Boolean (*hasElement)(void * self, IOHIDElementCookie elementCookie);

/*! @function start
    @abstract Called to start event delivery to the queue. */
    IOReturn (*start)(void * self);
    
/*! @function stop
    @abstract Called to stop event delivery to the queue. */
    IOReturn (*stop)(void * self);

/*! @function getNextEvent
    @abstract Called to read next event from the queue
    @discussion
    @param event The event that will be filled.  If a long value is
        present, it is up to the caller to deallocate it.
        
    *** THE FOLLOWING PARAMETERS ARE UNSUPPORTED ***
    @param maxtime If non-zero, limits read events to those that occured
        on or before maxTime
    @param timoutMS The timeout in milliseconds, a zero timeout will 
        cause this call to be non-blocking (returning queue empty) if 
        there is a NULL callback, and blocking forever until the queue
        is non-empty if their is a valid callback */

    IOReturn (*getNextEvent)(void * 			self,
                            IOHIDEventStruct *		event,
                            AbsoluteTime		maxTime,
                            UInt32 			timeoutMS);

/*! @function setEventCallout
    @abstract Set the event callout to be called when the queue 
        transitions to non-empty.
    @discussion In order for this to work correctly, you must call
        createAsyncPort and createAsyncEventSource.
    @param callback if non-NULL is a callback to be called when data 
        is  inserted to the queue
    @param callbackTarget The callback target passed to the callback
    @param callbackRefcon The callback refcon passed to the callback */
    IOReturn (*setEventCallout)(void * 			self,
                                IOHIDCallbackFunction   callback,
                                void * 			callbackTarget,
                                void *			callbackRefcon);

/*! @function getEventCallout
    @abstract Get the event callout.
    @discussion This callback will be called the queue transitions
        to non-empty.
    @param callback if non-NULL is a callback to be called when data 
        is  inserted to the queue
    @param callbackTarget The callback target passed to the callback
    @param callbackRefcon The callback refcon passed to the callback */
    IOReturn (*getEventCallout)(void * 			self,
                                IOHIDCallbackFunction * outCallback,
                                void ** 		outCallbackTarget,
                                void **			outCallbackRefcon);
};

// End of copies ...

static const UInt32 MAX_QUEUE_DEPTH = 20;

typedef IOHIDQueueInterface * * (* hidAllocQueue_FP)
	(Pvoid	self);
	
typedef IOReturn (* hidDeviceClose_FP)
	(Pvoid	self);

typedef IOReturn (* hidDeviceOpen_FP)
	(Pvoid	self,
	 UInt32	flags);

typedef IOReturn (* hidGetElementValue_FP)
	(Pvoid							self,
	 IOHIDElementCookie	elementCookie,
	 IOHIDEventStruct *	valueEvent);

typedef IOReturn (* hidqAddElement_FP)
	(Pvoid							self,
	 IOHIDElementCookie	elementCookie,
	 UInt32							flags);
	 
typedef IOReturn (* hidqCreate_FP)
	(Pvoid	self,
	 UInt32	flags,
	 UInt32	depth);
	 
typedef IOReturn (* hidqCreateAsyncEventSource_FP)
	(Pvoid								self,
	 CFRunLoopSourceRef *	source);
	 
typedef IOReturn (* hidqDispose_FP)
	(Pvoid	self);
	
typedef IOReturn (* hidqGetNextEvent_FP)
	(Pvoid							self,
	 IOHIDEventStruct *	event,
	 AbsoluteTime				maxTime,
	 UInt32							timeoutMS);
	 
typedef Boolean (* hidqHasElement_FP)
	(Pvoid							self,
	 IOHIDElementCookie	elementCookie);
	 
typedef IOReturn (* hidqSetEventCallout_FP)
	(Pvoid									self,
	 IOHIDCallbackFunction	callback,
	 Pvoid									callbackTarget,
	 Pvoid									callbackRefcon);
	 
typedef IOReturn (* hidqStart_FP)
	(Pvoid	self);
	
typedef IOReturn (* hidqStop_FP)
	(Pvoid	self);
	 
typedef IOReturn ( * hidQueryElementValue_FP)
	(Pvoid												self,
	 IOHIDElementCookie						elementCookie,
	 IOHIDEventStruct *						valueEvent,
	 UInt32												timeoutMS,
	 IOHIDElementCallbackFunction	callback,
	 Pvoid												callbackTarget,
	 Pvoid												callbackRefcon);	 

typedef IOReturn ( * hidSetElementValue_FP)
	(Pvoid												self,
	 IOHIDElementCookie						elementCookie,
	 IOHIDEventStruct *						valueEvent,
	 UInt32												timeoutMS,
	 IOHIDElementCallbackFunction	callback,
	 Pvoid												callbackTarget,
	 Pvoid												callbackRefcon);	 

 #endif /* COMPILE_FOR_CATS */

#endif /* not COMMON_HIDXDATA_H_ */
