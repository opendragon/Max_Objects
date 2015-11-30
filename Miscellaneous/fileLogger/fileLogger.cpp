/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       fileLogger.cpp                                                          */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the fileLogger module.    */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2002 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    2002/03/17                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#define IS_MAIN /* */
#include "fileLogger.h"
#include "reportVersion.h"

/*------------------------------------ fileLoggerCreate ---*/
static void * fileLoggerCreate(t_symbol * fileName)
{
    FileLoggerData * xx = static_cast<FileLoggerData *>(object_alloc(gClass));
    
    if (xx)
    {
        xx->fName = fileName;
    }
    return xx;
} // fileLoggerCreate

/*------------------------------------ fileLoggerFree ---*/
static void fileLoggerFree(FileLoggerData * xx)
{
    fileLoggerReleaseTheFile(xx);
} // fileLoggerFree

/*------------------------------------ main ---*/
int C74_EXPORT main(void)
{
    /* Allocate class memory and set up class. */
    t_class * temp = class_new(OUR_NAME, reinterpret_cast<method>(fileLoggerCreate),
                               reinterpret_cast<method>(fileLoggerFree),
                               static_cast<long>(sizeof(FileLoggerData)),
                               reinterpret_cast<method>(NULL), A_SYM, 0);

    if (temp)
    {
        class_addmethod(temp, reinterpret_cast<method>(cmd_Anything), MESSAGE_ANYTHING, A_GIMME, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Assist), MESSAGE_ASSIST, A_CANT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Float), MESSAGE_FLOAT, A_FLOAT, 0);
        class_addmethod(temp, reinterpret_cast<method>(stdinletinfo), MESSAGE_INLETINFO, A_CANT, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_Int), MESSAGE_INT, A_LONG, 0);
        class_addmethod(temp, reinterpret_cast<method>(cmd_List), MESSAGE_LIST, A_GIMME, 0);
        class_register(CLASS_BOX, temp);
    }
    gClass = temp;
    reportVersion(OUR_NAME);
    return 0;
} // main

/*------------------------------------ fileLoggerGetTheFile ---*/
bool fileLoggerGetTheFile(FileLoggerData * xx)
{
    bool result = false;

    // Attempt to open the file in read/write mode.
    if (path_opensysfile(xx->fName->s_name, path_getdefault(), &xx->fFileRef, PATH_RW_PERM))
    {
        // If unsuccessful, attempt to create the file.
        result = (! path_createsysfile(xx->fName->s_name, path_getdefault(), FILETYPE_TEXT,
                                       &xx->fFileRef));
    }
    else
    {
        // If successful, position to the end.
        result = (! sysfile_setpos(xx->fFileRef, SYSFILE_FROMLEOF, 0));
    }
    return result;
} // fileLoggerGetTheFile

/*------------------------------------ fileLoggerReleaseTheFile ---*/
bool fileLoggerReleaseTheFile(FileLoggerData * xx)
{
    bool result;
    
    if (xx->fFileRef)
    {
        result = (! sysfile_close(xx->fFileRef));
        memset(&xx->fFileRef, 0, sizeof(xx->fFileRef));
    }
    else
    {
        result = true;
    }
    return result;
} // fileLoggerReleaseTheFile

/*------------------------------------ fileLoggerWriteStringToTheFile ---*/
bool fileLoggerWriteStringToTheFile(FileLoggerData * xx,
                                    const char *     value)
{
    bool result;

    if (value)
    {
        t_ptr_size count = static_cast<t_ptr_size>(strlen(value));

        if (count)
        {
            result = (! sysfile_write(xx->fFileRef, &count, value));
        }
        else
        {
            result = true;
        }
    }
    else
    {
        result = true;
    }
    return result;
} // fileLoggerWriteStringToTheFile
