/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       map3d_List.c                                                            */
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
/*  Created:    2001/04/07                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "map3d.h"

/*------------------------------------ cmd_List ---*/
Pvoid cmd_List
  (Map3dPtr xx,
   PSymbol  message,
   short    argc,
   PAtom    argv)
{
#pragma unused(message)
  bool okSoFar = true;

  EnterCallback();
  if (xx)
  {
    FloatOrInteger xCoord, yCoord, zCoord;

    if (argc == 3)
    {
      if (argv[0].a_type == A_FLOAT)
        setFOI2Float(xCoord, argv[0].a_w.w_float);
      else if (argv[0].a_type == A_LONG)
        setFOI2Integer(xCoord, argv[0].a_w.w_long);
      else
      {
        LOG_ERROR_1(OUTPUT_PREFIX "first value is not numeric")
        okSoFar = false;
      }
      if (okSoFar)
      {
        if (argv[1].a_type == A_FLOAT)
          setFOI2Float(yCoord, argv[1].a_w.w_float);
        else if (argv[1].a_type == A_LONG)
          setFOI2Integer(yCoord, argv[1].a_w.w_long);
        else
        {
          LOG_ERROR_1(OUTPUT_PREFIX "second value is not numeric")
          okSoFar = false;
        }
      }
      if (okSoFar)
      {
        if (argv[2].a_type == A_FLOAT)
          setFOI2Float(zCoord, argv[2].a_w.w_float);
        else if (argv[2].a_type == A_LONG)
          setFOI2Integer(zCoord, argv[2].a_w.w_long);
        else
        {
          LOG_ERROR_1(OUTPUT_PREFIX "third value is not numeric")
          okSoFar = false;
        }
      }
    }
    else
    {
      LOG_ERROR_1(OUTPUT_PREFIX "list is not the right length")
      okSoFar = false;
    }
    if (okSoFar)
      map3dProcessData(xx, xCoord, yCoord, zCoord);
  }
  ExitMaxMessageHandler()
} /* cmd_List */
