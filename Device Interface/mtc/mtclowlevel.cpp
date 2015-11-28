/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       mtclowlevel.cpp                                                         */
/*                                                                                      */
/*  Contains:   The low-level support routines for the mtc module.                      */
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
/*  Created:    2000/06/16                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "mtc.h"
#include <math.h>
#include "ext_path.h"

#define LINEBUFFER_SIZE     256 /* For buffering from external files */
#define NEIGHBOUR_FUDGE     1.2 /* Factor to determine neighbours */
#define NEIGHBOUR_REDUCTION 0.5 /* Factor to reduce neigbour of hot spot */

/*------------------------------------ mtcMoveRawDataFromBuffer ---*/
void mtcMoveRawDataFromBuffer(MtcData * xx)
{
    unsigned char * inWalker = xx->fDataBuffer;
    MtcTaxelDesc *  aTaxel = *xx->fTaxelMapHandle;

    /* Recover the 10-bit data from the 8-bit received data */
    if (xx->fUseCompression && xx->fDataCompressed)
    {
        for ( ; ; )
        {
            short         inMap = *inWalker++;
            unsigned char outValues[8];

            if (inMap)
            {
                long          numBits = BITCOUNT(inMap);
                unsigned long bits = 0;
                unsigned char value1 = *inWalker++;
                unsigned char value2 = 0;
                unsigned char value3 = 0;

                if (1 < numBits)
                {
                    value2 = *inWalker++;
                }
                if (2 < numBits)
                {
                    value3 = *inWalker++;
                }
                if (3 < numBits)
                {
                    bits = (static_cast<unsigned long>(value1 << 16) |
                            static_cast<unsigned long>(value2 << 8) |
                            static_cast<unsigned long>(value3));
                }
                switch (numBits)
                {
                    case 3: /* 8 bits per taxel */
                        outValues[2] = value3;
                    /* fall through */

                    case 2: /* 8 bits per taxel */
                        outValues[1] = value2;
                    /* fall through */

                    case 1: /* 8 bits per taxel */
                        outValues[0] = value1;
                        break;

                    case 4: /* 6 bits per taxel */
                        outValues[3] = static_cast<unsigned char>((bits & 0x3F) << 2);
                        outValues[2] = static_cast<unsigned char>(((bits >> 6) & 0x3F) << 2);
                        outValues[1] = static_cast<unsigned char>(((bits >> 12) & 0x3F) << 2);
                        outValues[0] = static_cast<unsigned char>(((bits >> 18) & 0x3F) << 2);
                        break;

                    case 6: /* 4 bits per taxel */
                        /* fall through */
                        outValues[5] = static_cast<unsigned char>((bits & 0x0F) << 4);

                    case 5:
                        outValues[4] = static_cast<unsigned char>(((bits >> 4) & 0x0F) << 4);
                        outValues[3] = static_cast<unsigned char>(((bits >> 8) & 0x0F) << 4);
                        outValues[2] = static_cast<unsigned char>(((bits >> 12) & 0x0F) << 4);
                        outValues[1] = static_cast<unsigned char>(((bits >> 16) & 0x0F) << 4);
                        outValues[0] = static_cast<unsigned char>(((bits >> 20) & 0x0F) << 4);
                        break;

                    case 8: /* 3 bits per taxel */
                        /* fall through */
                        outValues[7] = static_cast<unsigned char>((bits & 0x03) << 5);

                    case 7:
                        outValues[6] = static_cast<unsigned char>(((bits >> 3) & 0x03) << 5);
                        outValues[5] = static_cast<unsigned char>(((bits >> 6) & 0x03) << 5);
                        outValues[4] = static_cast<unsigned char>(((bits >> 9) & 0x03) << 5);
                        outValues[3] = static_cast<unsigned char>(((bits >> 12) & 0x03) << 5);
                        outValues[2] = static_cast<unsigned char>(((bits >> 15) & 0x03) << 5);
                        outValues[1] = static_cast<unsigned char>(((bits >> 18) & 0x03) << 5);
                        outValues[0] = static_cast<unsigned char>(((bits >> 21) & 0x03) << 5);
                        break;
                        
                }
            }
            // Set values from the outValues vector into the set of taxels starting at aTaxel,
            // using the bit fields of inValue.
            for (short ii = 0, jj = 0; 8 > ii; ++ii)
            {
                if (inMap & (0x0080 >> ii))
                {
                    (aTaxel + ii)->fRawData = *(xx->fDataRecovery + outValues[jj]);
                    ++jj;
                }
                else
                {
                    (aTaxel + ii)->fRawData = 0;
                }
            }
            aTaxel += 8; /* Number of output taxels per input packet */
            if (inWalker >= (xx->fDataBuffer + xx->fLastByte))
            {
                break;
            }
        }
    }
    else
    {
        for (short taxelNum = 0; taxelNum < xx->fNumTaxels; ++taxelNum, ++aTaxel, ++inWalker)
        {
            aTaxel->fRawData = *(xx->fDataRecovery + static_cast<unsigned char>(*inWalker));
        }
    }
} // mtcMoveRawDataFromBuffer
/*------------------------------------ mtcProcessTaxels ---*/
void mtcProcessTaxels(MtcData * xx)
{
    MtcTaxelDesc * aTaxel;
    short          taxelNum;

#if defined(BE_VERBOSE)
    if (xx->fVerbose)
    {
        LOG_POST_2(xx, OUTPUT_PREFIX "processing received taxels, sample %ld", xx->fSampleNumber)
    }
#endif /* BE_VERBOSE */
    mtcMoveRawDataFromBuffer(xx);
    if (xx->fNormalizing)
    {
        float range;
        bool  changed;
        short value;

        for (taxelNum = 0, aTaxel = *xx->fTaxelMapHandle; taxelNum < xx->fNumTaxels;
             ++aTaxel, ++taxelNum)
        {
            changed = false;
            value = aTaxel->fRawData;
            aTaxel->fCookedData = value;
            if (value < aTaxel->fRawMin)
            {
                aTaxel->fRawMin = value;
                changed = true;
            }
            if (value > aTaxel->fRawMax)
            {
                aTaxel->fRawMax = value;
                changed = true;
            }
            if (changed)
            {
                range = aTaxel->fRawMax - aTaxel->fRawMin;
                if (0 < range)
                {
                    aTaxel->fScale = (MTC_MAX_COOKED_PRESSURE / range);
                }
            }
        }
    }
    else
    {
        MtcTaxelDesc * aSpot;
        MtcTaxelDesc * sentinel = xx->fSentinelTaxel;
        short          spotsSeen = 0;
        short          threshold = xx->fThreshold;
        double         pressure;
        double         maxPressure;

        /* Cook the raw data */
        for (taxelNum = 0, aTaxel = *xx->fTaxelMapHandle; taxelNum < xx->fNumTaxels;
             ++aTaxel, ++taxelNum)
        {
            pressure = aTaxel->fRawData - aTaxel->fRawMin;
            if (0 >= pressure)
            {
                pressure = 0;
            }
            else
            {
                pressure *= aTaxel->fScale;
            }
            aTaxel->fPressure = pressure;
            aTaxel->fCookedData = pressure;
        }
        if (xx->fModeRaw)
        {
            MtcTaxelDesc ** iWalker = xx->fTaxelMatrix;
            t_atom *        oWalker = xx->fRawRow;

            outlet_int(xx->fDataStartStopOut, 1);
            /* Process each row of output */
            for (short ii = 0; ii < xx->fMaxRow; ++ii)
            {
                for (short jj = 0; jj < xx->fMaxCol; ++jj, ++iWalker, ++oWalker)
                {
                    atom_setfloat(oWalker, TO_DBL((*iWalker)->fPressure));
                }
            }
            outlet_list(xx->fDataOut, 0L, xx->fMaxCol * xx->fMaxRow, xx->fRawRow);
            outlet_int(xx->fDataStartStopOut, 0);
        }
        else
        {
            for ( ; spotsSeen < xx->fNumSpots; )
            {
                /* Determine if we have a candidate spot */
                aSpot = NULL;
                maxPressure = -1e6;
                for (taxelNum = 0, aTaxel = *xx->fTaxelMapHandle; taxelNum < xx->fNumTaxels;
                     ++aTaxel, ++taxelNum)
                {
                    pressure = aTaxel->fPressure;
                    if (aTaxel->fBottomTaxel != sentinel)
                    {
                        pressure += (aTaxel->fBottomTaxel->fPressure * NEIGHBOUR_REDUCTION);
                    }
                    if (aTaxel->fLeftTaxel != sentinel)
                    {
                        pressure += (aTaxel->fLeftTaxel->fPressure * NEIGHBOUR_REDUCTION);
                    }
                    if (aTaxel->fRightTaxel != sentinel)
                    {
                        pressure += (aTaxel->fRightTaxel->fPressure * NEIGHBOUR_REDUCTION);
                    }
                    if (aTaxel->fTopTaxel != sentinel)
                    {
                        pressure += (aTaxel->fTopTaxel->fPressure * NEIGHBOUR_REDUCTION);
                    }
                    pressure *= NEIGHBOUR_REDUCTION;    /* reduce sum for safety */
                    if (pressure >= threshold)
                    {
                        if (aSpot)
                        {
                            if (pressure > maxPressure)
                            {
                                aSpot = aTaxel;
                                maxPressure = pressure;
                            }
                        }
                        else
                        {
                            aSpot = aTaxel;
                            maxPressure = pressure;
                        }
                    }
                }
                if (aSpot)
                {
                    double         spotX;
                    double         spotY;
                    double         divideX;
                    double         divideY;
                    double         factor;
                    MtcTaxelDesc * otherTaxel;

                    /* Calculate coordinates of spot as a composite of the winner */
                    spotX = (aSpot->fXCoord * aSpot->fPressure);
                    spotY = (aSpot->fYCoord * aSpot->fPressure);
                    divideX = divideY = aSpot->fPressure;
                    otherTaxel = aSpot->fBottomTaxel;
                    if (otherTaxel != sentinel)
                    {
                        factor = fabs(otherTaxel->fPressure * NEIGHBOUR_REDUCTION *
                                      NEIGHBOUR_REDUCTION);
                        spotY -= (factor * otherTaxel->fYCoord);
                        divideY += factor;
                    }
                    otherTaxel = aSpot->fLeftTaxel;
                    if (otherTaxel != sentinel)
                    {
                        factor = fabs(otherTaxel->fPressure * NEIGHBOUR_REDUCTION *
                                      NEIGHBOUR_REDUCTION);
                        spotX -= (factor * otherTaxel->fXCoord);
                        divideX += factor;
                    }
                    otherTaxel = aSpot->fRightTaxel;
                    if (otherTaxel != sentinel)
                    {
                        factor = fabs(otherTaxel->fPressure * NEIGHBOUR_REDUCTION *
                                      NEIGHBOUR_REDUCTION);
                        spotX += (factor * otherTaxel->fXCoord);
                        divideX += factor;
                    }
                    otherTaxel = aSpot->fTopTaxel;
                    if (otherTaxel != sentinel)
                    {
                        factor = fabs(otherTaxel->fPressure * NEIGHBOUR_REDUCTION *
                                      NEIGHBOUR_REDUCTION);
                        spotY += (factor * otherTaxel->fYCoord);
                        divideY += factor;
                    }
                    /* Remember the spot */
                    atom_setfloat(&(xx->fSpots + spotsSeen)->fXCoord, TO_DBL(spotX / divideX));
                    atom_setfloat(&(xx->fSpots + spotsSeen)->fYCoord, TO_DBL(spotY / divideY));
                    atom_setfloat(&(xx->fSpots + spotsSeen)->fPressure, TO_DBL(maxPressure));
                    ++spotsSeen;

                    /* Adjust values to hide the neighbourhood of the spot */
                    aSpot->fPressure = 0;
                    aSpot->fBottomTaxel->fPressure *= NEIGHBOUR_REDUCTION;
                    aSpot->fLeftTaxel->fPressure *= NEIGHBOUR_REDUCTION;
                    aSpot->fRightTaxel->fPressure *= NEIGHBOUR_REDUCTION;
                    aSpot->fTopTaxel->fPressure *= NEIGHBOUR_REDUCTION;
                }
                else
                {
                    /* No candidate found, so exit */
                    break;
                }
            }
            if (spotsSeen)
            {
                if ((1 < spotsSeen) && (kMtcOrderUnordered != xx->fSortOrder))
                {
                    mtcSortTaxels(xx, spotsSeen);
                }
                outlet_int(xx->fDataStartStopOut, 1);
                outlet_list(xx->fDataOut, 0L, spotsSeen * AtomsPerSpot,
                            reinterpret_cast<t_atom *>(xx->fSpots));
                outlet_int(xx->fDataStartStopOut, 0);
            }
        }
    }
} // mtcProcessTaxels

/*------------------------------------ mtcReadMapFile ---*/
bool mtcReadMapFile(MtcData * xx)
{
    bool         okSoFar = false;
    t_filehandle fileRef;

    if (! strlen(xx->fMapFileName))
    {
        /* No mapping file given. */
        return true; /* ?? */
    }

    if (path_opensysfile(xx->fMapFileName, path_getdefault(), &fileRef, PATH_READ_PERM))
    {
        LOG_ERROR_2(xx, OUTPUT_PREFIX "problem opening mapping file '%s'", xx->fMapFileName)
    }
    else
    {
        t_handle     fileContents = sysmem_newhandle(0);
        bool         controlLineSeen = false;
        long         bufferSize;
        const char * bufferScan;
        const char * bufferEnd;
        const char * bufferLineStart;
        const char * bufferLineEnd;

        if (sysfile_readtextfile(fileRef, fileContents, 0, TEXT_LB_NATIVE))
        {
            LOG_ERROR_2(xx, OUTPUT_PREFIX "problem reading mapping file '%s'", xx->fMapFileName)
        }
        else
        {
            char   aChar;
            short  lineCount = 0;
            size_t lineSize;
            char   lineBuffer[LINEBUFFER_SIZE + 1];    /* Use for scan processing. */

            /* We now have the whole file in our buffer, to be read. */
            okSoFar = true;
            sysmem_lockhandle(fileContents, 1);
            bufferSize = sysmem_handlesize(fileContents);
            bufferScan = *fileContents;
            bufferEnd = bufferScan + bufferSize;
            xx->fNumTaxels = 128;
            xx->fMaxRow = xx->fMaxCol = 0;
            xx->fXMax = xx->fYMax = xx->fRateMax = 0;
            xx->fXMaxActual = xx->fYMaxActual = -1e6;
            xx->fXMinActual = xx->fYMinActual = 1e6;

            /* Locate the control line, if present. */
            bufferLineStart = *fileContents;
            for ( ; ; )
            {
                bufferLineEnd = bufferLineStart;
                for ( ; ; )
                {
                    aChar = *bufferLineEnd++;
                    if ((bufferLineEnd == bufferEnd) || ('\n' == aChar))
                    {
                        break;
                    }
                    
                }
                if ('$' == *bufferLineStart)
                {
                    /* Analyze the control line. */
                    long numTaxels;

                    controlLineSeen = true;
                    lineSize = static_cast<size_t>(bufferLineEnd - (bufferLineStart + 1));
                    if (lineSize > LINEBUFFER_SIZE)
                    {
                        lineSize = LINEBUFFER_SIZE;
                    }
                    strncpy(lineBuffer, bufferLineStart + 1, lineSize);
                    *(lineBuffer + lineSize) = 0;
                    if (4 != sscanf(lineBuffer, "%ld%f%f%f", &numTaxels, &xx->fXMax, &xx->fYMax,
                                    &xx->fRateMax))
                    {
                        LOG_ERROR_2(xx, OUTPUT_PREFIX "problem with mapping file, control line %d",
                                    static_cast<int>(lineCount))
                        okSoFar = false;
                    }
                    xx->fNumTaxels = numTaxels;
                    break;
                }
                
                if (bufferLineEnd == bufferEnd)
                {
                    /* No control line seen. */
                    break;
                }

                ++lineCount;
                bufferLineStart = bufferLineEnd;
            }
            if (okSoFar)
            {
                int             xIndex;
                int             yIndex;
                short           taxelCount;
                float           xCoord;
                float           yCoord;
                MtcTaxelDesc ** anArray;
                MtcTaxelDesc *  aTaxel;
                MtcTaxelDesc *  sentinel;

                xx->fRateCurrent = xx->fRateMax;
                anArray = MAKE_TYPED_HANDLE(MtcTaxelDesc, xx->fNumTaxels + 1);
                if (anArray)
                {
                    sysmem_lockhandle(reinterpret_cast<t_handle>(anArray), 1);
                    xx->fTaxelMapHandle = anArray;
                    /* Initialize the sentinel taxel */
                    sentinel = *anArray + xx->fNumTaxels;
                    xx->fSentinelTaxel = sentinel;
                    sentinel->fRawData = sentinel->fCookedData = 0;
                    sentinel->fRawMin = sentinel->fRawMax = 0;
                    sentinel->fPressure = sentinel->fScale = 0;
                    sentinel->fXCoord = sentinel->fYCoord = 0;
                    sentinel->fXIndex = sentinel->fYIndex = sentinel->fTaxelNumber = -1;
                    sentinel->fBottomTaxel = sentinel->fLeftTaxel = sentinel;
                    sentinel->fRightTaxel = sentinel->fTopTaxel = sentinel;
#if defined(MTC_USE_CORNERS)
                    sentinel->fBottomLeftTaxel = sentinel->fBottomRightTaxel = sentinel;
                    sentinel->fTopLeftTaxel = sentinel->fTopRightTaxel = sentinel;
#endif /* MTC_USE_CORNERS */
                    bufferScan = *fileContents;
                    /* Load up the taxels, from the beginning of the buffer. */
                    bufferLineStart = *fileContents;
                    lineCount = 0;
                    aTaxel = *anArray;
                    for (taxelCount = 0; taxelCount < xx->fNumTaxels; )
                    {
                        bufferLineEnd = bufferLineStart;
                        for ( ; ; )
                        {
                            aChar = *bufferLineEnd++;
                            if ((bufferLineEnd == bufferEnd) || ('\n' == aChar))
                            {
                                break;
                            }
                        }
                        if (('$' != *bufferLineStart) && ('#' != *bufferLineStart))
                        {
                            /* Ignore comments and control lines. Process a data line. */
                            lineSize = static_cast<size_t>(bufferLineEnd - (bufferLineStart + 1));
                            if (lineSize > LINEBUFFER_SIZE)
                            {
                                lineSize = LINEBUFFER_SIZE;
                            }
                            strncpy(lineBuffer, bufferLineStart + 1, lineSize);
                            *(lineBuffer + lineSize) = 0;
                            if (4 != sscanf(lineBuffer, "%d%d%f%f", &yIndex, &xIndex, &xCoord,
                                            &yCoord))
                            {
                                LOG_ERROR_2(xx, OUTPUT_PREFIX "problem with mapping file, data line %d",
                                            static_cast<int>(lineCount))
                                okSoFar = false;
                                break;
                            }
                            if (xIndex > xx->fMaxCol)
                            {
                                xx->fMaxCol = xIndex;
                            }
                            if (yIndex > xx->fMaxRow)
                            {
                                xx->fMaxRow = yIndex;
                            }
                            if (xCoord > xx->fXMax)
                            {
                                if (controlLineSeen)
                                {
                                    LOG_ERROR_1(xx, OUTPUT_PREFIX "taxel with x coord greater than "
                                                "declared max seen")
                                }
                                xx->fXMax = xCoord;
                            }
                            if (yCoord > xx->fYMax)
                            {
                                if (controlLineSeen)
                                {
                                    LOG_ERROR_1(xx, OUTPUT_PREFIX "taxel with y coord greater than "
                                                "declared max seen")
                                }
                                xx->fYMax = yCoord;
                            }
                            if (xCoord > xx->fXMaxActual)
                            {
                                xx->fXMaxActual = xCoord;
                            }
                            if (xCoord < xx->fXMinActual)
                            {
                                xx->fXMinActual = xCoord;
                            }
                            if (yCoord > xx->fYMaxActual)
                            {
                                xx->fYMaxActual = yCoord;
                            }
                            if (yCoord < xx->fYMinActual)
                            {
                                xx->fYMinActual = yCoord;
                            }
                            aTaxel->fXCoord = xCoord;
                            aTaxel->fYCoord = yCoord;
                            aTaxel->fXIndex = xIndex;
                            aTaxel->fYIndex = yIndex;
                            aTaxel->fTaxelNumber = taxelCount;
                            aTaxel->fBottomTaxel = aTaxel->fLeftTaxel = sentinel;
                            aTaxel->fRightTaxel = aTaxel->fTopTaxel = sentinel;
#if defined(MTC_USE_CORNERS)
                            aTaxel->fBottomLeftTaxel = aTaxel->fBottomRightTaxel = sentinel;
                            aTaxel->fTopLeftTaxel = aTaxel->fTopRightTaxel = sentinel;
#endif /* MTC_USE_CORNERS */
                            ++aTaxel;
                            ++taxelCount;
                        }
                        if (bufferLineEnd == bufferEnd)
                        {
                            /* Processed last line. */
                            break;
                        }

                        ++lineCount;
                        bufferLineStart = bufferLineEnd;
                    }
                }
                else
                {
                    LOG_ERROR_1(xx, OUTPUT_PREFIX "could not allocate taxel data")
                    okSoFar = false;
                }
            }
            sysmem_lockhandle(fileContents, 0);
        }
        sysfile_close(fileRef);
        sysmem_freehandle(fileContents);
    }
    if (okSoFar)
    {
        MtcTaxelDesc ** aMatrix = GET_BYTES(xx->fMaxCol * xx->fMaxRow, MtcTaxelDesc *);

        if (aMatrix)
        {
            xx->fTaxelMatrix = aMatrix;
            xx->fExpectedPackets = (((xx->fMaxCol * xx->fMaxRow) + 7) / 8);
            for (long ii = 0; ii < (xx->fMaxRow * xx->fMaxCol); ++ii, ++aMatrix)
            {
                *aMatrix = xx->fSentinelTaxel;
            }
        }
        else
        {
            LOG_ERROR_1(xx, OUTPUT_PREFIX "could not allocate taxel matrix")
            okSoFar = false;
        }
    }
    return okSoFar;
} // mtcReadMapFile

/*------------------------------------ mtcReadNormalizationFile ---*/
bool mtcReadNormalizationFile(MtcData * xx)
{
    bool         okSoFar = false;
    t_filehandle fileRef;

    if (! strlen(xx->fNormalFileName))
    {
        /* No normalization file given. */
        return true; /* ?? */
    }

    if (path_opensysfile(xx->fNormalFileName, path_getdefault(), &fileRef, PATH_READ_PERM))
    {
        LOG_ERROR_2(xx, OUTPUT_PREFIX "problem opening normalization file '%s'",
                    xx->fNormalFileName)
    }
    else
    {
        t_handle     fileContents = sysmem_newhandle(0);
        long         bufferSize;
        const char * bufferScan;
        const char * bufferEnd;
        const char * bufferLineStart;
        const char * bufferLineEnd;

        if (sysfile_readtextfile(fileRef, fileContents, 0, TEXT_LB_NATIVE))
        {
            LOG_ERROR_2(xx, OUTPUT_PREFIX "problem reading normalization file '%s'",
                        xx->fNormalFileName)
        }
        else
        {
            char           aChar;
            short          lineCount = 0;
            size_t         lineSize;
            char           lineBuffer[LINEBUFFER_SIZE + 1]; /* Use for scan processing. */
            MtcTaxelDesc * aTaxel;

            /* We now have the whole file in our buffer, to be read. */
            okSoFar = true;
            sysmem_lockhandle(fileContents, 1);
            bufferSize = sysmem_handlesize(fileContents);
            bufferScan = *fileContents;
            bufferEnd = bufferScan + bufferSize;
            bufferLineStart = *fileContents;
            aTaxel = *xx->fTaxelMapHandle;

            /* Load up the taxels, from the beginning of the buffer. */
            for (short taxelCount = 0; taxelCount < xx->fNumTaxels; )
            {
                bufferLineEnd = bufferLineStart;
                for ( ; ; )
                {
                    aChar = *bufferLineEnd++;
                    if ((bufferLineEnd == bufferEnd) || ('\n' == aChar))
                    {
                        break;
                    }
                }
                if ('#' != *bufferLineStart)
                {
                    /* Ignore comments. Process a data line. */
                    int rawMin;

                    lineSize = static_cast<size_t>(bufferLineEnd - (bufferLineStart + 1));
                    if (lineSize > LINEBUFFER_SIZE)
                    {
                        lineSize = LINEBUFFER_SIZE;
                    }
                    strncpy(lineBuffer, bufferLineStart + 1, lineSize);
                    *(lineBuffer + lineSize) = 0;
                    if (2 != sscanf(lineBuffer, "%d%lf", &rawMin, &aTaxel->fScale))
                    {
                        LOG_ERROR_2(xx, OUTPUT_PREFIX "problem with normalization file, data line "
                                    "%d", static_cast<int>(lineCount))
                        okSoFar = false;
                        break;
                    }
                    aTaxel->fRawMin = rawMin;
                    ++aTaxel;
                    ++taxelCount;
                }
                if (bufferLineEnd == bufferEnd)
                {
                    /* Processed last line. */
                    break;
                }
                
                ++lineCount;
                bufferLineStart = bufferLineEnd;
            }
            sysmem_lockhandle(fileContents, 0);
        }
        sysfile_close(fileRef);
        sysmem_freehandle(fileContents);
    }
    return okSoFar;
} // mtcReadNormalizationFile

/*------------------------------------ mtcResetNormalization ---*/
void mtcResetNormalization(MtcData * xx)
{
    MtcTaxelDesc * aTaxel = *xx->fTaxelMapHandle;

    for (short taxelCount = 0; taxelCount < xx->fNumTaxels; ++taxelCount, ++aTaxel)
    {
        aTaxel->fRawMin = MTC_MAX_RAW_PRESSURE;
        aTaxel->fRawMax = MTC_MIN_RAW_PRESSURE;
        aTaxel->fScale = 0;
    }
} // mtcResetNormalization

/*------------------------------------ mtcSetupIndices ---*/
void mtcSetupIndices(MtcData * xx)
{
    double         deltaX;
    double         deltaY;
    double         distance;
    double         fudge;
    double         test;
    double         closest;
    double         leftBracket;
    double         rightBracket;
    double         topBracket;
    double         bottomBracket;
    short          outerTaxelCount;
    short          innerTaxelCount;
    MtcTaxelDesc * outerTaxel;
    MtcTaxelDesc * innerTaxel;
    MtcTaxelDesc * candidate;
//    MtcTaxelDesc * sentinel = xx->fSentinelTaxel;

    /* Fill in the data recovery table */
    for (short ii = 0; 256 > ii; ++ii)
    {
        *(xx->fDataRecovery + ii) = ((200 > ii) ? ii : (200 + (((ii - 200) * 823) / 55)));
    }
    if (1 < xx->fMaxCol)
    {
        deltaX = (xx->fXMaxActual - xx->fXMinActual) / (xx->fMaxCol - 1);
    }
    else
    {
        deltaX = 1; /* for the lack of any better value! */
    }
    if (1 < xx->fMaxRow)
    {
        deltaY = (xx->fYMaxActual - xx->fYMinActual) / (xx->fMaxRow - 1);
    }
    else
    {
        deltaY = 1; /* for the lack of any better value! */
    }
    for (outerTaxelCount = 0, outerTaxel = *xx->fTaxelMapHandle; outerTaxelCount < xx->fNumTaxels;
         ++outerTaxelCount, ++outerTaxel)
    {
        /* Determine right neighbour */
        candidate = NULL;
        closest = 1e6;
        leftBracket = (deltaX * NEIGHBOUR_FUDGE / 2);
        rightBracket = (deltaX * NEIGHBOUR_FUDGE);
        fudge = (deltaY * NEIGHBOUR_FUDGE / 2);
        for (innerTaxelCount = 0, innerTaxel = *xx->fTaxelMapHandle;
             innerTaxelCount < xx->fNumTaxels; ++innerTaxelCount, ++innerTaxel)
        {
            if (innerTaxel != outerTaxel)
            {
                distance = innerTaxel->fXCoord - outerTaxel->fXCoord;
                if ((distance >= leftBracket) && (distance < rightBracket))
                {
                    /* We've found a taxel to the right of the taxel */
                    /* of interest; check if it's close enough vertically */
                    test = fabs(innerTaxel->fYCoord - outerTaxel->fYCoord);
                    if (test < fudge)
                    {
                        /* There's a taxel to the right of the taxel of */
                        /* interest, and it's close enough vertically */
                        if (! candidate)
                        {
                            candidate = innerTaxel;
                            closest = test;
                        }
                        else if (test < closest)
                        {
                            candidate = innerTaxel;
                            closest = test;
                        }
                    }
                }
            }
        }
        if (candidate)
        {
            outerTaxel->fRightTaxel = candidate;
        }
        /* Determine left neighbour */
        candidate = NULL;
        closest = 1e6;
        leftBracket = (deltaX * NEIGHBOUR_FUDGE / 2);
        rightBracket = (deltaX * NEIGHBOUR_FUDGE);
        for (innerTaxelCount = 0, innerTaxel = *xx->fTaxelMapHandle;
             innerTaxelCount < xx->fNumTaxels; ++innerTaxelCount, ++innerTaxel)
        {
            if (innerTaxel != outerTaxel)
            {
                distance = outerTaxel->fXCoord - innerTaxel->fXCoord;
                if ((distance >= leftBracket) && (distance < rightBracket))
                {
                    /* We've found a taxel to the right of the taxel */
                    /* of interest; check if it's close enough vertically */
                    test = fabs(innerTaxel->fYCoord - outerTaxel->fYCoord);
                    if (test < fudge)
                    {
                        /* There's a taxel to the right of the taxel of */
                        /* interest, and it's close enough vertically */
                        if (! candidate)
                        {
                            candidate = innerTaxel;
                            closest = test;
                        }
                        else if (test < closest)
                        {
                            candidate = innerTaxel;
                            closest = test;
                        }
                    }
                }
            }
        }
        if (candidate)
        {
            outerTaxel->fLeftTaxel = candidate;
        }
        /* Determine top neighbour */
        candidate = NULL;
        closest = 1e6;
        bottomBracket = (deltaY * NEIGHBOUR_FUDGE / 2);
        topBracket = (deltaY * NEIGHBOUR_FUDGE);
        fudge = (deltaX * NEIGHBOUR_FUDGE / 2);
        for (innerTaxelCount = 0, innerTaxel = *xx->fTaxelMapHandle;
             innerTaxelCount < xx->fNumTaxels; ++innerTaxelCount, ++innerTaxel)
        {
            if (innerTaxel != outerTaxel)
            {
                distance = innerTaxel->fYCoord - outerTaxel->fYCoord;
                if ((distance >= bottomBracket) && (distance < topBracket))
                {
                    /* We've found a taxel to the right of the taxel */
                    /* of interest; check if it's close enough vertically */
                    test = fabs(innerTaxel->fXCoord - outerTaxel->fXCoord);
                    if (test < fudge)
                    {
                        /* There's a taxel to the right of the taxel of */
                        /* interest, and it's close enough vertically */
                        if (! candidate)
                        {
                            candidate = innerTaxel;
                            closest = test;
                        }
                        else if (test < closest)
                        {
                            candidate = innerTaxel;
                            closest = test;
                        }
                    }
                }
            }
        }
        if (candidate)
        {
            outerTaxel->fTopTaxel = candidate;
        }
        /* Determine bottom neighbour */
        candidate = NULL;
        closest = 1e6;
        bottomBracket = (deltaY * NEIGHBOUR_FUDGE / 2);
        topBracket = (deltaY * NEIGHBOUR_FUDGE);
        fudge = (deltaX * NEIGHBOUR_FUDGE / 2);
        for (innerTaxelCount = 0, innerTaxel = *xx->fTaxelMapHandle;
             innerTaxelCount < xx->fNumTaxels; ++innerTaxelCount, ++innerTaxel)
        {
            if (innerTaxel != outerTaxel)
            {
                distance = outerTaxel->fYCoord - innerTaxel->fYCoord;
                if ((distance >= bottomBracket) && (distance < topBracket))
                {
                    /* We've found a taxel to the right of the taxel */
                    /* of interest; check if it's close enough vertically */
                    test = fabs(innerTaxel->fXCoord - outerTaxel->fXCoord);
                    if (test < fudge)
                    {
                        /* There's a taxel to the right of the taxel of */
                        /* interest, and it's close enough vertically */
                        if (! candidate)
                        {
                            candidate = innerTaxel;
                            closest = test;
                        }
                        else if (test < closest)
                        {
                            candidate = innerTaxel;
                            closest = test;
                        }
                    }
                }
            }
        }
        if (candidate)
        {
            outerTaxel->fBottomTaxel = candidate;
        }
    }
#if defined(MTC_USE_CORNERS)
    /* Set up diagonal indices */
    for (outerTaxelCount = 0, outerTaxel = *xx->fTaxelMapHandle; outerTaxelCount < xx->fNumTaxels;
         ++outerTaxelCount, ++outerTaxel)
    {
        /* Note that we are taking advantage of two facts: */
        /* All pointer fields in an MtcTaxelDesc record are initialized to the sentinel */
        /* taxel, and the pointer fields in the sentinel taxel all point to itself. */
        innerTaxel = outerTaxel->fTopTaxel;
        outerTaxel->fTopLeftTaxel = innerTaxel->fLeftTaxel;
        outerTaxel->fTopRightTaxel = innerTaxel->fRightTaxel;
        innerTaxel = outerTaxel->fBottomTaxel;
        outerTaxel->fBottomLeftTaxel = innerTaxel->fLeftTaxel;
        outerTaxel->fBottomRightTaxel = innerTaxel->fRightTaxel;
    }
#endif /* MTC_USE_CORNERS */
    /* Build the pointer matrix */
    for (outerTaxelCount = 0, outerTaxel = *xx->fTaxelMapHandle; outerTaxelCount < xx->fNumTaxels;
         ++outerTaxelCount, ++outerTaxel)
    {
        *(xx->fTaxelMatrix + (((outerTaxel->fYIndex - 1) * xx->fMaxCol) +
                              outerTaxel->fXIndex - 1)) = outerTaxel;
    }
} // mtcSetupIndices

/*------------------------------------ mtcSortByPressure ---*/
static void mtcSortByPressure(MtcSpot *  spots,
                              const long numSpots)
{
    /* Note that we are guaranteed that the Atoms are all float values. */
    for (bool didSwap = true; didSwap; )
    {
        didSwap = false;
        for (long ii = 0; ii < numSpots; ++ii)
        {
            bool      swappedII = false;
            MtcSpot * atomII = spots + ii;
            float     iiValue = atomII->fPressure.a_w.w_float;

            for (long jj = ii + 1; jj < numSpots; ++jj)
            {
                MtcSpot * atomJJ = spots + jj;

                if (atomJJ->fPressure.a_w.w_float > iiValue)
                {
                    MtcSpot temp;

                    /* Need to swap! */
                    temp = *atomII;
                    *atomII = *atomJJ;
                    *atomJJ = temp;
                    swappedII = true;
                }
            }
            if (swappedII)
            {
                didSwap = true;
            }
        }
    }
} // mtcSortByPressure

/*------------------------------------ mtcSortByX ---*/
static void mtcSortByX(MtcSpot *  spots,
                       const long numSpots)
{
    /* Note that we are guaranteed that the Atoms are all float values. */
    for (bool didSwap = true; didSwap; )
    {
        didSwap = false;
        for (long ii = 0; ii < numSpots; ++ii)
        {
            bool      swappedII = false;
            MtcSpot * atomII = spots + ii;
            float     iiValue = atomII->fXCoord.a_w.w_float;

            for (long jj = ii + 1; jj < numSpots; ++jj)
            {
                MtcSpot * atomJJ = spots + jj;

                if (atomJJ->fXCoord.a_w.w_float > iiValue)
                {
                    MtcSpot temp;

                    /* Need to swap! */
                    temp = *atomII;
                    *atomII = *atomJJ;
                    *atomJJ = temp;
                    swappedII = true;
                }
            }
            if (swappedII)
            {
                didSwap = true;
            }
        }
    }
} // mtcSortByX

/*------------------------------------ mtcSortByY ---*/
static void mtcSortByY(MtcSpot *  spots,
                       const long numSpots)
{
    /* Note that we are guaranteed that the Atoms are all float values. */
    for (bool didSwap = true; didSwap; )
    {
        didSwap = false;
        for (long ii = 0; ii < numSpots; ++ii)
        {
            bool      swappedII = false;
            MtcSpot * atomII = spots + ii;
            float     iiValue = atomII->fYCoord.a_w.w_float;

            for (long jj = ii + 1; jj < numSpots; ++jj)
            {
                MtcSpot * atomJJ = spots + jj;

                if (atomJJ->fYCoord.a_w.w_float > iiValue)
                {
                    MtcSpot temp;

                    /* Need to swap! */
                    temp = *atomII;
                    *atomII = *atomJJ;
                    *atomJJ = temp;
                    swappedII = true;
                }
            }
            if (swappedII)
            {
                didSwap = true;
            }
        }
    }
} // mtcSortByY

/*------------------------------------ mtcSortTaxels ---*/
void mtcSortTaxels(MtcData *   xx,
                   const short numSpots)
{
    /* We'll use a simple bubble sort, as the maximum number of spots is quite small */
    switch (xx->fSortOrder)
    {
        case kMtcOrderByPressure:
            mtcSortByPressure(xx->fSpots, numSpots);
            break;

        case kMtcOrderByX:
            mtcSortByX(xx->fSpots, numSpots);
            break;

        case kMtcOrderByY:
            mtcSortByY(xx->fSpots, numSpots);
            break;

        default:
            break;
    }
} // mtcSortTaxels

/*------------------------------------ mtcWriteNormalizationFile ---*/
bool mtcWriteNormalizationFile(MtcData * xx)
{
    bool         okSoFar;
    t_filehandle fileRef;

    if (path_opensysfile(xx->fNormalFileName, path_getdefault(), &fileRef, PATH_WRITE_PERM))
    {
        // If unsuccessful, attempt to create the file.
        okSoFar = (! path_createsysfile(xx->fNormalFileName, path_getdefault(), FILETYPE_TEXT,
                                        &fileRef));
    }
    else
    {
        // Successful.
        okSoFar = true;
    }
    if (okSoFar)
    {
        OSErr          err;
        char           lineBuffer[LINEBUFFER_SIZE + 1];    /* Use for scan processing. */
        MtcTaxelDesc * aTaxel = *xx->fTaxelMapHandle;
        t_ptr_size     count;

        for (short taxelCount = 0; taxelCount < xx->fNumTaxels; ++taxelCount, ++aTaxel)
        {
            snprintf(lineBuffer, sizeof(lineBuffer), " %d %lf\n", static_cast<int>(aTaxel->fRawMin),
                     aTaxel->fScale);
            count = static_cast<long>(strlen(lineBuffer));
            err = sysfile_write(fileRef, &count, lineBuffer);
            if (err != noErr)
            {
                LOG_ERROR_2(xx, OUTPUT_PREFIX "problem (%ld) writing to normalization file",
                            static_cast<long>(err))
                okSoFar = false;
                break;
            }
        }
        sysfile_close(fileRef);
    }
    return okSoFar;
} // mtcWriteNormalizationFile
