/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       Vrotate_List.c                                                          */
/*                                                                                      */
/*  Contains:   The routine cmd_List().                                                 */
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
/*  Created:    2001/04/13                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "Vrotate.h"

/*------------------------------------ cmd_List ---*/
Pvoid cmd_List
  (VObjectPtr xx,
   PSymbol    message,
   short      argc,
   PAtom      argv)
{
#pragma unused(message)
  EnterCallback();
  if (xx)
  {
    long elementCount = xx->fHowMany;

    clearPrevious(xx);
    if (argc)
    {
      PAtom tempList = GETBYTES(argc, Atom);

      if (tempList)
      {
        if (elementCount > 0)
        {
          if (elementCount >= argc)
            elementCount %= argc;
        }
        else if (elementCount < 0)
        {
          elementCount = - elementCount;
          if (elementCount >= argc)
            elementCount %= argc;
        }
        if (elementCount)
        {
          if (tempList)
          {
            if (xx->fHowMany > 0)
            {
              /* Take last elementCount elements from the input: */
              memcpy(tempList, argv + argc - elementCount, elementCount * sizeof(Atom));
              /* Take first (argc - elementCount) elements from the input: */
              if (elementCount < argc)
                memcpy(tempList + elementCount, argv,
                        (argc - elementCount) * sizeof(Atom));
            }
            else
            {
              /* Take last (argc - elementCount) elements from the input: */
              if (argc > elementCount)
                memcpy(tempList, argv + elementCount,
                        (argc - elementCount) * sizeof(Atom));
              /* Take first elementCount elements from the input: */
              memcpy(tempList + argc - elementCount, argv, elementCount * sizeof(Atom));
            }
          }
        }
        else
          memcpy(tempList, argv, argc * sizeof(Atom));
        xx->fPreviousList = tempList;
        xx->fPreviousLength = argc;
        if (xx->fPreviousList)
          genericListOutput(xx->fResultOut, xx->fPreviousLength, xx->fPreviousList);
      }
    }
  }
  ExitMaxMessageHandler()
} /* cmd_List */
