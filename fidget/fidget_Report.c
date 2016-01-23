/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       fidget_Report.c                                                         */
/*                                                                                      */
/*  Contains:   The routine cmd_Report().                                               */
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
/*  Created:    2003/12/13                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "fidget.h"

/*------------------------------------ cmd_Report ---*/
Pvoid
cmd_Report(FidgetPtr xx,
           PSymbol   deviceType,
           PSymbol   serialNumber,
           long      element)
{
  long                            allocCount = 0, outputCount = 0, ii;
  PAtom                            tempList = NULL_PTR;
  PSymbol                        outType = NULL_PTR;
  HIDDeviceDataPtr    walker;

  EnterCallback();
  if (xx)
  { 
      if (deviceType == gEmptySymbol)
      {
          // List all devices
          outType = gDevicesSymbol;
          walker = xx->fHIDDevices;
          allocCount = (xx->fHIDDeviceCount * 2);
          tempList = GETBYTES(allocCount, Atom);
          for (ii = 0; ii < xx->fHIDDeviceCount; ++ii)
          {
                PSymbol    name = gUnknownSymbol;

                if (walker->fClass)
                    name = reinterpret_cast<PhidgetDescriptorPtr>(walker->fClass)->fName;
               SETSYM(tempList + outputCount, name);
               SETSYM(tempList + outputCount + 1, walker->fSerialNumber);
               outputCount += 2;
              walker = walker->fNext;
              if (! walker)
                  break;
                      
          }
      }
      else if (serialNumber == gEmptySymbol)
      {
          // List all devices for a given device type
          outType = gDeviceSymbol;
          walker = xx->fHIDDevices;
          allocCount = xx->fHIDDeviceCount + 1;
          tempList = GETBYTES(allocCount, Atom);
          for (ii = 0; ii < xx->fHIDDeviceCount; ++ii)
          {
                PSymbol    name = gUnknownSymbol;

                if (walker->fClass)
                    name = reinterpret_cast<PhidgetDescriptorPtr>(walker->fClass)->fName;
              if (name == deviceType)
              {
                  if (! outputCount)
                  {
                      SETSYM(tempList, deviceType);
                      ++outputCount;
                  }
                   SETSYM(tempList + outputCount, walker->fSerialNumber);
                   ++outputCount;
               }
              walker = walker->fNext;
              if (! walker)
                  break;
                      
          }
      }
      else if (serialNumber == gAsteriskSymbol)
      {
          walker = fidgetGetFirstHIDData(xx, deviceType);
          
          for ( ; walker; )
          {
              HIDElementDataPtr    anElement = walker->fFirstElement;
              
              if (element)
              {
                     // List the specific element
                     outType = gElementSymbol;
                     allocCount = 2;
                     tempList = GETBYTES(allocCount, Atom);
                     for ( ; anElement; anElement = anElement->fNext)
                     {
                         if (anElement->fCookie == reinterpret_cast<IOHIDElementCookie>(element))
                         {
                             SETSYM(tempList, fidgetMapElementType(anElement->fType));
                             SETLONG(tempList + 1, anElement->fSize);
                             outputCount = 2;
                             break;
                             
                         }
                     }
              }
              else
              {
                  // List all elements of the given device
                  outType = gElementsSymbol;
                  allocCount = (walker->fElementCount * 3);
                  tempList = GETBYTES(allocCount, Atom);
                  for (ii = 0; ii < walker->fElementCount; ++ii)
                  {
                       SETLONG(tempList + outputCount, reinterpret_cast<long>(anElement->fCookie));
                       SETSYM(tempList + outputCount + 1, fidgetMapElementType(anElement->fType));
                       SETLONG(tempList + outputCount + 2, anElement->fSize);
                       outputCount += 3;
                      anElement = anElement->fNext;
                      if (! anElement)
                          break;
                          
                  }
              }
              if (outputCount)
                outlet_anything(xx->fDataOut, outType, short(outputCount), tempList);
              if (allocCount)
                FREEBYTES(tempList, allocCount)
              allocCount = outputCount = 0;
              walker = fidgetGetNextHIDData(deviceType, walker);
          }
      }
      else
      {
          // Find the matching device:
          walker = fidgetLocateHIDData(xx, deviceType, serialNumber);
            if (walker)
            {
              HIDElementDataPtr    anElement = walker->fFirstElement;
              
              if (element)
              {
                     // List the specific element
                     outType = gElementSymbol;
                     allocCount = 2;
                     tempList = GETBYTES(allocCount, Atom);
                     for ( ; anElement; anElement = anElement->fNext)
                     {
                         if (anElement->fCookie == reinterpret_cast<IOHIDElementCookie>(element))
                         {
                             SETSYM(tempList, fidgetMapElementType(anElement->fType));
                             SETLONG(tempList + 1, anElement->fSize);
                             outputCount = 2;
                             break;
                             
                         }
                     }
              }
              else
              {
                  // List all elements of the given device
                  outType = gElementsSymbol;
                  allocCount = (walker->fElementCount * 3);
                  tempList = GETBYTES(allocCount, Atom);
                  for (ii = 0; ii < walker->fElementCount; ++ii)
                  {
                       SETLONG(tempList + outputCount, reinterpret_cast<long>(anElement->fCookie));
                       SETSYM(tempList + outputCount + 1, fidgetMapElementType(anElement->fType));
                       SETLONG(tempList + outputCount + 2, anElement->fSize);
                       outputCount += 3;
                      anElement = anElement->fNext;
                      if (! anElement)
                          break;
                          
                  }
              }
            }      
      }
      if (outputCount)
        outlet_anything(xx->fDataOut, outType, short(outputCount), tempList);
      if (allocCount)
        FREEBYTES(tempList, allocCount)
  }
  ExitMaxMessageHandler()
} /* cmd_Report */
