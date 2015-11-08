/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       x10base.h                                                               */
/*                                                                                      */
/*  Contains:   The data type declarations that are common to all the x10 code.         */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 1996 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    1996/09/21                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(X10BASE_H_))
# define X10BASE_H_ /* */

# include "missingAndExtra.h"

# define NUM_HOUSECODES 16
# define HOUSECODES_DATA \
    { \
        0x0080, /* 1 */ \
        0x0040, /* 2 */ \
        0x0020, /* 3 */ \
        0x0010, /* 4 */ \
        0x0008, /* 5 */ \
        0x0004, /* 6 */ \
        0x0002, /* 7 */ \
        0x0001, /* 8 */ \
        0x8000, /* 9 */ \
        0x4000, /* 10 */ \
        0x2000, /* 11 */ \
        0x1000, /* 12 */ \
        0x0800, /* 13 */ \
        0x0400, /* 14 */ \
        0x0200, /* 15 */ \
        0x0100  /* 16 */ \
    }

mextern(unsigned short) kUnitCodeToBits[NUM_HOUSECODES] minit(HOUSECODES_DATA);

#endif /* not X10BASE_H_ */
