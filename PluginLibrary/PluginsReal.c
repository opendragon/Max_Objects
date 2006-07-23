/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       PluginsReal.c                                                           */
/*                                                                                      */
/*  Contains:   Support routines for plugins.                                           */
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
/*  Created:    2001/08/28                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#define USE_SYSLOG /* */

#include "MissingAndExtra.h"
#include "StandardPluginTypes.h"

// Define the real data structures that we'll be using:

struct MessageDescriptor
{
  MessageDescriptor * fNext;
  MessageDescriptor * fPrevious;
  PluginFunk          fHandler;
  PSymbol             fName;
}; /* MessageDescriptor */

typedef MessageDescriptor * MessageDescriptorPtr;

typedef MessageDescriptorPtr * MessageDescriptorHdl;

#define MESSAGE_DESCRIPTOR_TABLE_SIZE 23

/*------------------------------------ doRealRegistryCreate ---*/
static MessageDescriptorHdl doRealRegistryCreate
  (void)
{
  return reinterpret_cast<MessageDescriptorHdl>(NewPtrClear(MESSAGE_DESCRIPTOR_TABLE_SIZE *
                                                            sizeof(MessageDescriptorPtr)));
} /* doRealRegistryCreate */

/*------------------------------------ doRealRegistryFlush ---*/
static void doRealRegistryFlush
  (MessageDescriptorHdl registry)
{
  if (registry)
  {
    MessageDescriptorPtr walker, next;

    for (short ii = 0; ii < MESSAGE_DESCRIPTOR_TABLE_SIZE; ++ii)
    {
      for (walker = *(registry + ii); walker; walker = next)
      {
        next = walker->fNext;
        DisposePtr(reinterpret_cast<Ptr>(walker));
      }
      *(registry + ii) = NULL_PTR;
    }
  }
} /* doRealRegistryFlush */

/*------------------------------------ doRealRegistryFree ---*/
static void doRealRegistryFree
  (MessageDescriptorHdl registry)
{
  if (registry)
  {
    doRealRegistryFlush(registry);
    DisposePtr(reinterpret_cast<Ptr>(registry));
  }
} /* doRealRegistryFree */

/*------------------------------------ doMessageHash ---*/
static long doMessageHash
  (PSymbol name)
{
  return (long(name) % MESSAGE_DESCRIPTOR_TABLE_SIZE);
} /* doMessageHash */

/*------------------------------------ doRealIdentifyMessage ---*/
static PluginFunk doRealIdentifyMessage
  (MessageDescriptorHdl registry,
   PSymbol              name)
{
  if (registry && name)
  {
    long	slot = doMessageHash(name);
    
    for (MessageDescriptorPtr walker = *(registry + slot); walker;
    			walker = walker->fNext)
    {
      if (walker->fName == name)
        return walker->fHandler;

    }
  }
  return NULL_PTR;
} /* doRealIdentifyMessage */

/*------------------------------------ doRealAddMessageToRegistry ---*/
static bool doRealAddMessageToRegistry
  (MessageDescriptorHdl registry,
   PSymbol              name,
   PluginFunk           aHandle)
{
  if (registry && name && aHandle)
  {
    long                 slot = doMessageHash(name);
    MessageDescriptorPtr walker = *(registry + slot), newDescriptor;

    for ( ; walker; walker = walker->fNext)
    {
      if (walker->fName == name)
        return false;

    }
    newDescriptor = reinterpret_cast<MessageDescriptorPtr>(NewPtrClear(sizeof(MessageDescriptor)));
    if (newDescriptor)
    {
      newDescriptor->fNext = *(registry + slot);
      (*(registry + slot))->fPrevious = newDescriptor;
      *(registry + slot) = newDescriptor;
      newDescriptor->fHandler = aHandle;
      newDescriptor->fName = name;
      return true;

    }
  }
  return false;
} /* doRealAddMessageToRegistry */

/*------------------------------------ doRealRemoveMessageFromRegistry ---*/
static bool doRealRemoveMessageFromRegistry
  (MessageDescriptorHdl registry,
   PSymbol              name)
{
  if (registry && name)
  {
    long	slot = doMessageHash(name);

    for (MessageDescriptorPtr walker = *(registry + slot), previous = NULL_PTR; walker;
    			walker = walker->fNext)
    {
      if (walker->fName == name)
      {
        if (walker->fNext)
          walker->fNext->fPrevious = previous;
        if (previous)
          previous->fNext = walker->fNext;
        else
          *(registry + slot) = walker->fNext;
        DisposePtr(reinterpret_cast<Ptr>(walker));
        return true;

      }            
      previous = walker;
    }
  }
  return false;
} /* doRealRemoveMessageFromRegistry */

#define CADS_FOR_REAL /* */
#include "PluginAuxCallbacks.c"
#include "PluginCallbacks.c"
