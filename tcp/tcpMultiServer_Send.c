/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       tcpMultiServer_Send.c                                                   */
/*                                                                                      */
/*  Contains:   The routine cmd_Send().                                                 */
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
/*  Created:    2000/10/15                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "tcpMultiServer.h"

/*------------------------------------ cmd_Send ---*/
Pvoid cmd_Send
  (TcpMultiServerPtr xx,
   PSymbol           message,
   short             argc,
   PAtom             argv)
{
#if OPEN_TRANSPORT_SUPPORTED
 #pragma unused(message)
#else /* not OPEN_TRANSPORT_SUPPORTED */
 #pragma unused(message, argc, argv)
#endif /* not OPEN_TRANSPORT_SUPPORTED */
  bool  okSoFar = true;
  long  client = 0;

  EnterCallback();
  REPORT_MAX_MESSAGE("send")
  if (xx)
  {
#if OPEN_TRANSPORT_SUPPORTED
    switch (argv->a_type)
    {
      case A_LONG:
        client = argv->a_w.w_long;
        break;

      case A_FLOAT:
        client = long(argv->a_w.w_long);
        break;

      case A_SYM:
        LOG_ERROR_2(OUTPUT_PREFIX "invalid client (%s)", argv->a_w.w_sym->s_name)
        okSoFar = false;
        break;

      case A_SEMI:
        LOG_ERROR_1(OUTPUT_PREFIX "invalid client ';'")
        okSoFar = false;
        break;

      case A_COMMA:
        LOG_ERROR_1(OUTPUT_PREFIX "invalid client ','")
        okSoFar = false;
        break;

      case A_DOLLAR:
        LOG_ERROR_1(OUTPUT_PREFIX "invalid client '$'")
        okSoFar = false;
        break;

      default:
        LOG_ERROR_2(OUTPUT_PREFIX "input of an unknown type (%hd) seen", argv->a_type)
        okSoFar = false;
        break;

    }
    if (okSoFar)
    {
      TcpConnectionPtr connection;
      uchar            listKind = getListKind(short(argc - 1), argv + 1);

      if (client)
      {
        connection = tcpMultiServerValidateClient(xx, client);
        if (connection)
          clearDataBuffer(connection->fSendBuffer);
        else
        {
          LOG_ERROR_2(OUTPUT_PREFIX "invalid client (%ld)", client)
          signalError(xx);
          okSoFar = false;
        }
        if (okSoFar && (connection->fState == TCP_OBJECT_CONNECTED))
        {
          okSoFar = (listKind != A_ERROR);
          if (okSoFar)
          {
	          connection->fSendBuffer->fDataType = listKind;
	          for (short ii = 1; okSoFar && (ii < argc); ++ii)
	            okSoFar = addAtomToBuffer(OUR_NAME, connection->fSendBuffer, &argv[ii],
	                                      connection->fRawMode);
          }
          else
            LOG_ERROR_1(OUTPUT_PREFIX "unrecognizable data")
          if (okSoFar)
            transmitBuffer(xx, connection->fDataEndpoint, connection->fSendBuffer,
                           connection->fRawMode);
          else
            signalError(xx);
        }
        else
        {
          LOG_ERROR_1(OUTPUT_PREFIX "not connected")
          signalError(xx);
        }
      }
      else
      {
        /* create a temporary buffer */
        DataBufferHdl temp_handle =
                              reinterpret_cast<DataBufferHdl>(newhandle(BUFF_MEMORY_TO_ALLOC));
        DataBufferPtr a_buffer;

        if (temp_handle)
        {
          HLock(reinterpret_cast<Handle>(temp_handle));
          a_buffer = *temp_handle;
          okSoFar = (listKind != A_ERROR);
          if (okSoFar)
          {
            bool saw_live_one = false;

            for (short ii = 0; ii < xx->fMaximumConnections; ++ii)
            {
              connection = *(xx->fConnections + ii);
              if (connection && (connection->fState == TCP_OBJECT_CONNECTED) &&
                    connection->fActive)
              {
			          clearDataBuffer(a_buffer);
			          a_buffer->fDataType = listKind;
			          for (short ii = 1; okSoFar && (ii < argc); ++ii)
			            okSoFar = addAtomToBuffer(OUR_NAME, a_buffer, &argv[ii],
											                       connection->fRawMode);
                if (okSoFar)
                {
	                copyDataBuffer(connection->fSendBuffer, a_buffer);
	                transmitBuffer(xx, connection->fDataEndpoint, connection->fSendBuffer,
	                               connection->fRawMode);
	                saw_live_one = true;
                }
              }            
            }
            if (! saw_live_one)
            {
              LOG_ERROR_1(OUTPUT_PREFIX "no clients connected")
              signalError(xx);
            }
          }
          else
            LOG_ERROR_1(OUTPUT_PREFIX "unrecognizable data")
          if (! okSoFar)
            signalError(xx);
          HUnlock(reinterpret_cast<Handle>(temp_handle));
          disposhandle(reinterpret_cast<Handle>(temp_handle));
        }
      }
    }
    else
      signalError(xx);
#endif /* OPEN_TRANSPORT_SUPPORTED */
  }
  ExitMaxMessageHandler()
} /* cmd_Send */
