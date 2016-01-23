/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       StandardPlugins.h                                                       */
/*                                                                                      */
/*  Contains:   The data type declarations that are needed for standard plugins.        */
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
/*  Created:    2003/05/19                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(STANDARDPLUGINS_H_))
 #define STANDARDPLUGINS_H_ /* */

 #include "MissingAndExtra.h"
 #include "StandardPluginTypes.h"

 #if defined(__cplusplus)
extern "C"
{
 #endif /* __cplusplus */

OSErr
do_Anything(PLUGIN_HEADER_PREFIX,
            PSymbol message,
            const short argc,
            PAtom argv);

OSErr
do_Bang(PLUGIN_HEADER_PREFIX);

OSErr
do_Double(PLUGIN_HEADER_PREFIX,
          const double value);

OSErr
do_List(PLUGIN_HEADER_PREFIX,
        const short argc,
        PAtom argv);

OSErr
do_Long(PLUGIN_HEADER_PREFIX,
        const long value);

OSErr
main(CFragConnectionID connID,
     OwnerPtr          owner,
     Handle            sharedStorage);

OSErr
niam(OwnerPtr owner,
     Ptr      sharedStorage);

OSErr
onCreate(OwnerPtr    owner,
         Pchar       theName,
         Ptr         sharedStorage,
         Handle      privateStorage,
         const short argc,
         PAtom       argv,
         Pshort      numInlets,
         Pshort      numOutlets);

OSErr
onDestroy(OwnerPtr owner,
          Ptr      sharedStorage,
          Ptr      privateStorage);

OSErr
onReload(CFragConnectionID connID,
         OwnerPtr          owner,
         Ptr               sharedStorage);

 #if defined(__cplusplus)
}
 #endif /* __cplusplus */

#endif /* not STANDARDPLUGINS_H_ */
