/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       mapErrorCodeToString.c                                                  */
/*                                                                                      */
/*  Contains:   The routine mapErrorCodeToString().                                     */
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

#include "acquireOpenTransport.h"

/*------------------------------------ mapErrorCodeToString ---*/
Ptr mapErrorCodeToString
  (const OSStatus errCode)
{
#if defined(COMPILE_FOR_STUB)
 #pragma unused(errCode)
	return NULL_PTR;
#else /* not COMPILE_FOR_STUB */
 #if (! OPEN_TRANSPORT_SUPPORTED)
  #pragma unused(errCode)
 #else /* OPEN_TRANSPORT_SUPPORTED */
  switch (errCode)
  {
    case kOTNoError:
      return "kOTNoError: no error";

    case kOTOutOfMemoryErr:
      return "kOTOutOfMemoryErr: out of memory";

    case kOTNotFoundErr:
      return "kOTNotFoundErr: not found";

    case kOTDuplicateFoundErr:
      return "kOTDuplicateFoundErr: duplicate found";

    case kOTBadAddressErr:
      return "kOTBadAddressErr: a bad address was specified";

    case kOTBadOptionErr:
      return "kOTBadOptionErr: a bad option was specified";

    case kOTAccessErr:
      return "kOTAccessErr: missing access permission";

    case kOTBadReferenceErr:
      return "kOTBadReferenceErr: bad provider reference";

    case kOTNoAddressErr:
      return "kOTNoAddressErr: no address was specified";

    case kOTOutStateErr:
      return "kOTOutStateErr: call issued in wrong state";

    case kOTBadSequenceErr:
      return "kOTBadSequenceErr: sequence specified does not exist";

    case kOTSysErrorErr:
      return "kOTSysErrorErr: a system error occurred";

    case kOTLookErr:
      return "kOTLookErr: an event occurred - call Look()";
 
    case kOTBadDataErr:
      return "kOTBadDataErr: an illegal amount of data was specified";

    case kOTBufferOverflowErr:
      return "kOTBufferOverflowErr: passed buffer not big enough";

    case kOTFlowErr:
      return "kOTFlowErr: provider is flow-controlled";

    case kOTNoDataErr:
      return "kOTNoDataErr: no data available for reading";

    case kOTNoDisconnectErr:
      return "kOTNoDisconnectErr: no disconnect indication available";

    case kOTNoUDErrErr:
      return "kOTNoUDErrErr: no Unit Data Error indication available";

    case kOTBadFlagErr:
      return "kOTBadFlagErr: a bad flag value was supplied";

    case kOTNoReleaseErr:
      return "kOTNoReleaseErr: no orderly release indication available";

    case kOTNotSupportedErr:
      return "kOTNotSupportedErr: command is not supported";

    case kOTStateChangeErr:
      return "kOTStateChangeErr: state is changing - try again later";

    case kOTNoStructureTypeErr:
      return "kOTNoStructureTypeErr: bad structure type requested for OTAlloc";

    case kOTBadNameErr:
      return "kOTBadNameErr: a bad endpoint name was supplied";

    case kOTBadQLenErr:
      return "kOTBadQLenErr: a bind to an in-use address with qlen > 0";

    case kOTAddressBusyErr:
      return "kOTAddressBusyErr: address requested is already in use";

    case kOTIndOutErr:
      return "kOTIndOutErr: accept failed because of pending listen";

    case kOTProviderMismatchErr:
      return "kOTProviderMismatchErr: tried to accept on incompatible endpoint";

    case kOTResQLenErr:
      return "kOTResQLenErr: alternate connection with qlen = 0";

    case kOTResAddressErr:
      return "kOTResAddressErr: alternate address does not match bind address";

    case kOTQFullErr:
      return "kOTQFullErr: number of outstanding connections is qlen";

    case kOTProtocolErr:
      return "kOTProtocolErr: an unspecified provider error occurred";

    case kOTBadSyncErr:
      return "kOTBadSyncErr: a synchronous call at interrupt time";

    case kOTCanceledErr:
      return "kOTCanceledErr: the command was cancelled";

    case kEPERMErr:
      return "kEPERMErr: permission denied";

    case kENORSRCErr:
      return "kENORSRCErr: no such resource";

    case kEINTRErr:
      return "kEINTRErr: interrupted system service";

    case kEIOErr:
      return "kEIOErr: I/O error";

    case kENXIOErr:
      return "kENXIOErr: no such device or address";

    case kEBADFErr:
      return "kEBADFErr: bad file number";

    case kEAGAINErr:
      return "kEAGAINErr: try operation again later";

    case kEACCESErr:
      return "kEACCESErr: permission denied";

    case kEFAULTErr:
      return "kEFAULTErr: bad address";

    case kEBUSYErr:
      return "kEBUSYErr: device or resource busy";

    case kENODEVErr:
      return "kENODEVErr: no such device";

    case kEINVALErr:
      return "kEINVALErr: invalid argument";

    case kENOTTYErr:
      return "kENOTTYErr: not a character device";

    case kEPIPEErr:
      return "kEPIPEErr: broken pipe";

    case kERANGEErr:
      return "kERANGEErr: message size too large for STREAM";

    case kEWOULDBLOCKErr:
      return "kEWOULDBLOCKErr: call would block, so was aborted or a deadlock would occur";

    case kEALREADYErr:
      return "kEALREADYErr";

    case kENOTSOCKErr:
      return "kENOTSOCKErr: socket operation on non-socket";

    case kEDESTADDRREQErr:
      return "kEDESTADDRREQErr: destination address required";

    case kEMSGSIZEErr:
      return "kEMSGSIZEErr: message too long";

    case kEPROTOTYPEErr:
      return "kEPROTOTYPEErr: protocol wrong type for socket";

    case kENOPROTOOPTErr:
      return "kENOPROTOOPTErr: protocol not available";

    case kEPROTONOSUPPORTErr:
      return "kEPROTONOSUPPORTErr: protocol not supported";

    case kESOCKTNOSUPPORTErr:
      return "kESOCKTNOSUPPORTErr: socket type not supported";

    case kEOPNOTSUPPErr:
      return "kEOPNOTSUPPErr: operation not supported on socket";

    case kEADDRINUSEErr:
      return "kEADDRINUSEErr: address already in use";

    case kEADDRNOTAVAILErr:
      return "kEADDRNOTAVAILErr: can't assign requested address";

    case kENETDOWNErr:
      return "kENETDOWNErr: network is down";

    case kENETUNREACHErr:
      return "kENETUNREACHErr: network is unreachable";

    case kENETRESETErr:
      return "kENETRESETErr: network dropped connection on reset";

    case kECONNABORTEDErr:
      return "kECONNABORTEDErr: software caused connection abort";

    case kECONNRESETErr:
      return "kECONNRESETErr: connection reset by peer";

    case kENOBUFSErr:
      return "kENOBUFSErr: no buffer space available";

    case kEISCONNErr:
      return "kEISCONNErr: socket is already connected";

    case kENOTCONNErr:
      return "kENOTCONNErr: socket is not connected";

    case kESHUTDOWNErr:
      return "kESHUTDOWNErr: can't send after socket shutdown";

    case kETOOMANYREFSErr:
      return "kETOOMANYREFSErr: too many references: can't splice";

    case kETIMEDOUTErr:
      return "kETIMEDOUTErr: connection timed out";

    case kECONNREFUSEDErr:
      return "kECONNREFUSEDErr: connection refused";

    case kEHOSTDOWNErr:
      return "kEHOSTDOWNErr: host is down";

    case kEHOSTUNREACHErr:
      return "kEHOSTUNREACHErr: no route to host";

    case kEPROTOErr:
      return "kEPROTOErr: a catastrophic error has occurred";

    case kETIMEErr:
      return "kETIMEErr: an ioctl command has timed out";

    case kENOSRErr:
      return "kENOSRErr: can't allocate enough system resources";

    case kEBADMSGErr:
      return "kEBADMSGErr";

    case kECANCELErr:
      return "kECANCELErr";

    case kENOSTRErr:
      return "kENOSTRErr";

    case kENODATAErr:
      return "kENODATAErr";

    case kEINPROGRESSErr:
      return "kEINPROGRESSErr";

    case kESRCHErr:
      return "kESRCHErr";

    case kENOMSGErr:
      return "kENOMSGErr";

    case kOTClientNotInittedErr:
      return "kOTClientNotInittedErr: the client has not initialized Open Transport";

    case kOTPortHasDiedErr:
      return "kOTPortHasDiedErr: the port is unregistered";

    case kOTPortWasEjectedErr:
      return "kOTPortWasEjectedErr: the port is ejected";

    case kOTBadConfigurationErr:
      return "kOTBadConfigurationErr: TCP/IP is improperly configured";

    case kOTConfigurationChangedErr:
      return "kOTConfigurationChangedErr: an AppleTalk configuration has changed";

    case kOTUserRequestedErr:
      return "kOTUserRequestedErr: the port was disabled by the user";

    case kOTPortLostConnection:
      return "kOTPortLostConnection: the port was disabled due to a lost connection";

  }
#endif /* OPEN_TRANSPORT_SUPPORTED */
  return "unknown";
#endif /* not COMPILE_FOR_STUB */
} /* mapErrorCodeToString */

#if defined(COMPILE_FOR_STUB)
 #pragma export list mapErrorCodeToString
#endif /* COMPILE_FOR_STUB */