/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       Vsegment_Anything.cpp                                                   */
/*                                                                                      */
/*  Contains:   The routine cmd_Anything().                                             */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2000 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2000/07/09                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "Vsegment.h"

/*------------------------------------ cmd_Anything ---*/
ANYTHING_HEADER(VsegmentData)
{
    if (xx)
    {
        short elementCount = xx->fHowMany;
        short actArgs = static_cast<short>(argc + 1);

        clearPrevious(xx);
        if (elementCount)
        {
            short start = xx->fStart;
            short end;

            if (start < 0)
            {
                start = static_cast<short>(actArgs + start);
            }
            if (elementCount > 0)
            {
                end = static_cast<short>(start + elementCount - 1);
            }
            else
            {
                short temp = static_cast<short>(start + elementCount + 1);

                end = start;
                start = temp;
            }
            if (0 > start)
            {
                start = 0;
            }
            if (end >= actArgs)
            {
                end = static_cast<short>(actArgs - 1);
            }
            if ((0 <= end) && (start < actArgs))
            {
                t_atom * newList = GET_BYTES(end + 1 - start, t_atom);

                if (newList)
                {
                    if (start)
                    {
                        memcpy(newList, argv + start - 1, (end + 1 - start) * sizeof(t_atom));
                    }
                    else
                    {
                        A_SETSYM(newList, message);
                        memcpy(newList + 1, argv, (end - start) * sizeof(t_atom));
                    }
                    xx->fPreviousList = newList;
                    xx->fPreviousLength = static_cast<short>(end + 1 - start);
                    genericListOutput(xx->fResultOut, xx->fPreviousLength, xx->fPreviousList);
                }
            }
        }
    }
} // cmd_Anything
