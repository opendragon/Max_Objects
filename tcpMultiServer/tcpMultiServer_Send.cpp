/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       tcpMultiServer_Send.cpp                                                 */
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
void cmd_Send(TcpMultiServerData * xx,
              t_symbol *           message,
              short                argc,
              t_atom *             argv)
{
#pragma unused(message)
    bool okSoFar = true;
    long client = 0;

    REPORT_MAX_MESSAGE("send")
    if (xx)
    {
        switch (argv->a_type)
        {
            case A_LONG:
                client = argv->a_w.w_long;
                break;

            case A_FLOAT:
                client = static_cast<long>(argv->a_w.w_long);
                break;

            case A_SYM:
                LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid client (%s)", argv->a_w.w_sym->s_name)
                okSoFar = false;
                break;

            case A_SEMI:
                LOG_ERROR_1(xx, OUTPUT_PREFIX "invalid client ';'")
                okSoFar = false;
                break;

            case A_COMMA:
                LOG_ERROR_1(xx, OUTPUT_PREFIX "invalid client ','")
                okSoFar = false;
                break;

            case A_DOLLAR:
                LOG_ERROR_1(xx, OUTPUT_PREFIX "invalid client '$'")
                okSoFar = false;
                break;

            default:
                LOG_ERROR_2(xx, OUTPUT_PREFIX "input of an unknown type (%d) seen", static_cast<int>(argv->a_type))
                okSoFar = false;
                break;
        }
        if (okSoFar)
        {
            TcpConnectionData * connection;
            unsigned char       listKind = getListKind(static_cast<short>(argc - 1), argv + 1);

            if (client)
            {
                connection = tcpMultiServerValidateClient(xx, client);
                if (connection)
                {
                    clearDataBuffer(connection->fSendBuffer);
                }
                else
                {
                    LOG_ERROR_2(xx, OUTPUT_PREFIX "invalid client (%ld)", client)
                    signalError(xx);
                    okSoFar = false;
                }
                if (okSoFar && (kTcpStateConnected == connection->fState))
                {
                    okSoFar = (listKind != A_ERROR);
                    if (okSoFar)
                    {
                        connection->fSendBuffer->fDataType = listKind;
                        for (short ii = 1; okSoFar && (ii < argc); ++ii)
                        {
                            okSoFar = addAtomToBuffer(xx, OUR_NAME, connection->fSendBuffer, &argv[ii],
                                                      connection->fRawMode);
                        }
                    }
                    else
                    {
                        LOG_ERROR_1(xx, OUTPUT_PREFIX "unrecognizable data")
                    }
                    if (okSoFar)
                    {
                        transmitBuffer(xx, connection->fDataEndpoint, connection->fSendBuffer, connection->fRawMode);
                    }
                    else
                    {
                        signalError(xx);
                    }
                }
                else
                {
                    LOG_ERROR_1(xx, OUTPUT_PREFIX "not connected")
                    signalError(xx);
                }
            }
            else
            {
                /* create a temporary buffer */
                DataBuffer ** temp_handle = reinterpret_cast<DataBuffer **>(sysmem_newhandle(BUFF_MEMORY_TO_ALLOC));
                DataBuffer *  a_buffer;

                if (temp_handle)
                {
                    sysmem_lockhandle(reinterpret_cast<t_handle>(temp_handle), 1);
                    a_buffer = *temp_handle;
                    okSoFar = (listKind != A_ERROR);
                    if (okSoFar)
                    {
                        bool saw_live_one = false;

                        for (short ii = 0; ii < xx->fMaximumConnections; ++ii)
                        {
                            connection = *(xx->fConnections + ii);
                            if (connection && (connection->fState == kTcpStateConnected) && connection->fActive)
                            {
                                clearDataBuffer(a_buffer);
                                a_buffer->fDataType = listKind;
                                for (short ii = 1; okSoFar && (ii < argc); ++ii)
                                {
                                    okSoFar = addAtomToBuffer(xx, OUR_NAME, a_buffer, &argv[ii], connection->fRawMode);
                                }
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
                            LOG_ERROR_1(xx, OUTPUT_PREFIX "no clients connected")
                            signalError(xx);
                        }
                    }
                    else
                    {
                        LOG_ERROR_1(xx, OUTPUT_PREFIX "unrecognizable data")
                    }
                    if (! okSoFar)
                    {
                        signalError(xx);
                    }
                    sysmem_lockhandle(reinterpret_cast<t_handle>(temp_handle), 0);
                    sysmem_freehandle(reinterpret_cast<t_handle>(temp_handle));
                }
            }
        }
        else
        {
            signalError(xx);
        }
    }
} // cmd_Send
