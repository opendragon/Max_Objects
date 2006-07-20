/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       mtclowlevel.c                                                           */
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
#if defined(COMPILE_FOR_CATS)
 #include "ext_path.h"
#endif /* COMPILE_FOR_CATS */

#define LINEBUFFER_SIZE     256 /* For buffering from external files */
#define NEIGHBOUR_FUDGE     1.2 /* Factor to determine neighbours */
#define NEIGHBOUR_REDUCTION 0.5 /* Factor to reduce neigbour of hot spot */

/*------------------------------------ mtcMoveRawDataFromBuffer ---*/
void mtcMoveRawDataFromBuffer
  (MtcControlPtr xx)
{
  Puchar      inWalker = xx->fDataBuffer;
  MtcTaxelPtr aTaxel = *xx->fTaxelMapHandle;

  /* Recover the 10-bit data from the 8-bit received data */
  if (xx->fUseCompression && xx->fDataCompressed)
  {
    for ( ; ; )
    {
      short inMap = *inWalker++;
      uchar outValues[8];

      if (inMap)
      {
        long  numBits = BITCOUNT(inMap);
        ulong bits = 0;
        uchar value1 = *inWalker++, value2 = 0, value3 = 0;

        if (numBits > 1)
          value2 = *inWalker++;
        if (numBits > 2)
          value3 = *inWalker++;
        if (numBits > 3)
          bits = (static_cast<ulong>(value1 << 16) | static_cast<ulong>(value2 << 8) |
                  static_cast<ulong>(value3));
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
            outValues[3] = static_cast<uchar>((bits & 0x3F) << 2);
            outValues[2] = static_cast<uchar>(((bits >> 6) & 0x3F) << 2);
            outValues[1] = static_cast<uchar>(((bits >> 12) & 0x3F) << 2);
            outValues[0] = static_cast<uchar>(((bits >> 18) & 0x3F) << 2);
            break;

          case 6: /* 4 bits per taxel */
            /* fall through */
            outValues[5] = static_cast<uchar>((bits & 0x0F) << 4);

          case 5:
            outValues[4] = static_cast<uchar>(((bits >> 4) & 0x0F) << 4);
            outValues[3] = static_cast<uchar>(((bits >> 8) & 0x0F) << 4);
            outValues[2] = static_cast<uchar>(((bits >> 12) & 0x0F) << 4);
            outValues[1] = static_cast<uchar>(((bits >> 16) & 0x0F) << 4);
            outValues[0] = static_cast<uchar>(((bits >> 20) & 0x0F) << 4);
            break;

          case 8: /* 3 bits per taxel */
            /* fall through */
            outValues[7] = static_cast<uchar>((bits & 0x03) << 5);

          case 7:
            outValues[6] = static_cast<uchar>(((bits >> 3) & 0x03) << 5);
            outValues[5] = static_cast<uchar>(((bits >> 6) & 0x03) << 5);
            outValues[4] = static_cast<uchar>(((bits >> 9) & 0x03) << 5);
            outValues[3] = static_cast<uchar>(((bits >> 12) & 0x03) << 5);
            outValues[2] = static_cast<uchar>(((bits >> 15) & 0x03) << 5);
            outValues[1] = static_cast<uchar>(((bits >> 18) & 0x03) << 5);
            outValues[0] = static_cast<uchar>(((bits >> 21) & 0x03) << 5);
            break;

        }
      }
      // Set values from the outValues vector into the set of taxels starting at aTaxel,
      // using the bit fields of inValue.
      for (short ii = 0, jj = 0; ii < 8; ii++)
      {
        if (inMap & (0x0080 >> ii))
        {
          (aTaxel + ii)->fRawData = *(xx->fDataRecovery + outValues[jj]);
          jj++;
        }
        else
          (aTaxel + ii)->fRawData = 0;
      }
      aTaxel += 8; /* Number of output taxels per input packet */
      if (inWalker >= (xx->fDataBuffer + xx->fLastByte))
        break;

    }
  }
  else
  {
    for (short taxelNum = 0; taxelNum < xx->fNumTaxels; taxelNum++, aTaxel++, inWalker++)
      aTaxel->fRawData = *(xx->fDataRecovery + static_cast<uchar>(*inWalker));
  }
} /* mtcMoveRawDataFromBuffer */

/*------------------------------------ mtcProcessTaxels ---*/
void mtcProcessTaxels
  (MtcControlPtr xx)
{
  MtcTaxelPtr aTaxel;
  short       taxelNum;

#if defined(BE_VERBOSE)
  if (xx->fVerbose)
    LOG_POST_2(OUTPUT_PREFIX "processing received taxels, sample %ld", xx->fSampleNumber)
#endif /* BE_VERBOSE */
  mtcMoveRawDataFromBuffer(xx);
  if (xx->fNormalizing)
  {
    float range;
    bool  changed;
    short value;

    for (taxelNum = 0, aTaxel = *xx->fTaxelMapHandle; taxelNum < xx->fNumTaxels;
        aTaxel++, taxelNum++)
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
        if (range > 0)
          aTaxel->fScale = (MTC_MAX_COOKED_PRESSURE / range);
      }
    }
  }
  else
  {
    MtcTaxelPtr aSpot, sentinel = xx->fSentinelTaxel;
    short       spotsSeen = 0, threshold = xx->fThreshold;
    double      pressure, maxPressure;

    /* Cook the raw data */
    for (taxelNum = 0, aTaxel = *xx->fTaxelMapHandle; taxelNum < xx->fNumTaxels;
        aTaxel++, taxelNum++)
    {
      pressure = aTaxel->fRawData - aTaxel->fRawMin;
      if (pressure <= 0)
        pressure = 0;
      else
        pressure *= aTaxel->fScale;
      aTaxel->fPressure = pressure;
      aTaxel->fCookedData = short(pressure);
    }
    if (xx->fModeRaw)
    {
      MtcTaxelHdl iWalker = xx->fTaxelMatrix;
      PAtom       oWalker = xx->fRawRow;

      outlet_int(xx->fDataStartStopOut, 1);
      /* Process each row of output */
      for (short ii = 0; ii < xx->fMaxRow; ii++)
      {                
        for (short jj = 0; jj < xx->fMaxCol; jj++)
        {
          SETFLOAT(oWalker, float((*iWalker)->fPressure));
          iWalker++;
          oWalker++;
        }
      }
      outlet_list(xx->fDataOut, 0L, short(xx->fMaxCol * xx->fMaxRow), xx->fRawRow);
      outlet_int(xx->fDataStartStopOut, 0);
    }
    else
    {
      while (spotsSeen < xx->fNumSpots)
      {
        /* Determine if we have a candidate spot */
        aSpot = NULL_PTR;
        maxPressure = float(-1e6);
        for (taxelNum = 0, aTaxel = *xx->fTaxelMapHandle; taxelNum < xx->fNumTaxels;
            aTaxel++, taxelNum++)
        {
          pressure = aTaxel->fPressure;
          if (aTaxel->fBottomTaxel != sentinel)
            pressure += (aTaxel->fBottomTaxel->fPressure * NEIGHBOUR_REDUCTION);
          if (aTaxel->fLeftTaxel != sentinel)
            pressure += (aTaxel->fLeftTaxel->fPressure * NEIGHBOUR_REDUCTION);
          if (aTaxel->fRightTaxel != sentinel)
            pressure += (aTaxel->fRightTaxel->fPressure * NEIGHBOUR_REDUCTION);
          if (aTaxel->fTopTaxel != sentinel)
            pressure += (aTaxel->fTopTaxel->fPressure * NEIGHBOUR_REDUCTION);
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
          double      spotX, spotY, divideX, divideY, factor;
          MtcTaxelPtr otherTaxel;

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
          SETFLOAT(&(xx->fSpots + spotsSeen)->fXCoord, float(spotX / divideX));
          SETFLOAT(&(xx->fSpots + spotsSeen)->fYCoord, float(spotY / divideY));
          SETFLOAT(&(xx->fSpots + spotsSeen)->fPressure, float(maxPressure));
          spotsSeen++;

          /* Adjust values to hide the neighbourhood of the spot */
          aSpot->fPressure = 0;
          aSpot->fBottomTaxel->fPressure *= NEIGHBOUR_REDUCTION;
          aSpot->fLeftTaxel->fPressure *= NEIGHBOUR_REDUCTION;
          aSpot->fRightTaxel->fPressure *= NEIGHBOUR_REDUCTION;
          aSpot->fTopTaxel->fPressure *= NEIGHBOUR_REDUCTION;
        }
        else
          /* No candidate found, so exit */
          break;

      }
      if (spotsSeen)
      {
        if ((spotsSeen > 1) && (xx->fSortOrder != MTC_UNORDERED))
          mtcSortTaxels(xx, spotsSeen);
        outlet_int(xx->fDataStartStopOut, 1);
        outlet_list(xx->fDataOut, 0L, short(spotsSeen * AtomsPerSpot),
                    reinterpret_cast<PAtom>(xx->fSpots));
        outlet_int(xx->fDataStartStopOut, 0);
      }
    }
  }
} /* mtcProcessTaxels */

/*------------------------------------ mtcReadMapFile ---*/
bool mtcReadMapFile
  (MtcControlPtr xx)
{
  bool		okSoFar = false, controlLineSeen = false;
  Handle	bufferHandle;
  long   	bufferSize;
  Qchar		bufferScan, bufferEnd, bufferLineStart, bufferLineEnd;
#if defined(COMPILE_FOR_CATS)
	long		outType, inType = long('TEXT');
#endif /* COMPILE_FOR_CATS */

  if (! strlen(xx->fMapFileName))
    /* No mapping file given. */
    return true; /* ?? */

#if defined(COMPILE_FOR_CATS)
	if (locatefile_extended(xx->fMapFileName, &xx->fMapFilePath, &outType, &inType, 1))
#else /* not COMPILE_FOR_CATS */
  if (locatefiletype(xx->fMapFileName, &xx->fMapFileVolume, long('TEXT'), 0L))
#endif /* not COMPILE_FOR_CATS */    
    LOG_ERROR_2(OUTPUT_PREFIX "could not locate mapping file '%s'", xx->fMapFileName)
  else
  {
#if (! defined(COMPILE_FOR_CATS))
    Str255 tempName;
    FSSpec fileSpec;
#endif /* not COMPILE_FOR_CATS */
    OSErr  err;

#if (! defined(COMPILE_FOR_CATS))
    strcpy(reinterpret_cast<Pchar>(tempName), xx->fMapFileName);
    C2PStr(reinterpret_cast<Pchar>(tempName));
    err = FSMakeFSSpec(xx->fMapFileVolume, 0L, tempName, &fileSpec);
    if (err != noErr)
      LOG_ERROR_2(OUTPUT_PREFIX "problem (%ld) building file spec for mapping file",
                  long(err))
    else
    {
#endif /* not COMPILE_FOR_CATS */
#if defined(COMPILE_FOR_CATS)
			FILE_REF	refNum;
#else /* not COMPILE_FOR_CATS */
      short 		refNum;
#endif /* not COMPILE_FOR_CATS */

#if defined(COMPILE_FOR_CATS)
			err = path_openfile(xx->fMapFileName, xx->fMapFilePath, &refNum, READ_PERM); 
#else /* not COMPILE_FOR_CATS */
      err = FSpOpenDF(&fileSpec, fsRdPerm, &refNum);
#endif /* not COMPILE_FOR_CATS */
      if (err != noErr)
        LOG_ERROR_2(OUTPUT_PREFIX "problem (%ld) opening the mapping file", long(err))
      else
      {
        okSoFar = true;
        err = GetEOF(refNum, &bufferSize);
        if (err != noErr)
        {
          LOG_ERROR_2(OUTPUT_PREFIX "problem (%ld) getting mapping file size", long(err))
          okSoFar = false;
        }
        if (okSoFar)
        {
          bufferHandle = NewHandle(bufferSize);
          if (! bufferHandle)
          {
            LOG_ERROR_1(OUTPUT_PREFIX "problem allocating space for mapping file buffer")
            okSoFar = false;
          }
        }
        if (okSoFar)
        {
          long count = bufferSize;

          HLock(bufferHandle);
          err = FSRead(refNum, &count, *bufferHandle);
          HUnlock(bufferHandle);
          if (err != noErr)
          {
            LOG_ERROR_2(OUTPUT_PREFIX "problem (%ld) reading from mapping file",
                        long(err))
            okSoFar = false;
          }
        }
        err = FSClose(refNum);
        if (err != noErr)
          LOG_ERROR_2(OUTPUT_PREFIX "problem (%ld) closing the mapping file", long(err))
      }
#if (! defined(COMPILE_FOR_CATS))
    }
#endif /* not COMPILE_FOR_CATS */
    if (okSoFar)
    {
      char   aChar;
      short  lineCount = 0;
      size_t lineSize;
      char   lineBuffer[LINEBUFFER_SIZE + 1];    /* Use for scan processing. */

      /* We now have the whole file in our buffer, to be read. */
      HLock(bufferHandle);
      bufferScan = *bufferHandle;
      bufferEnd = bufferScan + bufferSize;
      xx->fNumTaxels = 128;
      xx->fMaxRow = xx->fMaxCol = 0;
      xx->fXMax = xx->fYMax = xx->fRateMax = 0;
      xx->fXMaxActual = xx->fYMaxActual = float(-1e6);
      xx->fXMinActual = xx->fYMinActual = float(1e6);

      /* Locate the control line, if present. */
      bufferLineStart = *bufferHandle;
      for ( ; ; )
      {
        bufferLineEnd = bufferLineStart;
        for ( ; ; )
        {
          aChar = *bufferLineEnd++;
          if ((bufferLineEnd == bufferEnd) || (aChar == '\r'))
            break;

        }
        if (*bufferLineStart == '$')
        {
          /* Analyze the control line. */
          controlLineSeen = true;
          lineSize = static_cast<size_t>(bufferLineEnd - (bufferLineStart + 1));
          if (lineSize > LINEBUFFER_SIZE)
            lineSize = LINEBUFFER_SIZE;
          strncpy(lineBuffer, bufferLineStart + 1, lineSize);
          *(lineBuffer + lineSize) = 0;
          if (sscanf(lineBuffer, "%hd%f%f%f", &xx->fNumTaxels, &xx->fXMax, &xx->fYMax,
                      &xx->fRateMax) != 4)
          {
            LOG_ERROR_2(OUTPUT_PREFIX "problem with mapping file, control line %hd",
                        lineCount)
            okSoFar = false;
          }
          break;

        }
        if (bufferLineEnd == bufferEnd)
          /* No control line seen. */
          break;

        lineCount++;
        bufferLineStart = bufferLineEnd;
      }
      if (okSoFar)
      {
        short       xIndex, yIndex, taxelCount;
        float       xCoord, yCoord;
        MtcTaxelHdl anArray;
        MtcTaxelPtr aTaxel, sentinel;

        xx->fRateCurrent = xx->fRateMax;
        anArray = reinterpret_cast<MtcTaxelHdl>(NewHandle(long((xx->fNumTaxels + 1) *
                                                      sizeof(MtcTaxelDesc))));
        if (anArray)
        {
          HLock(reinterpret_cast<Handle>(anArray));
          xx->fTaxelMapHandle = anArray;
          /* Initialize the sentinel taxel */
          sentinel = *anArray + xx->fNumTaxels;
          xx->fSentinelTaxel = sentinel;
          sentinel->fRawData = sentinel->fCookedData = sentinel->fRawMin =
              sentinel->fRawMax = 0;
          sentinel->fPressure = sentinel->fScale = 0;
          sentinel->fXCoord = sentinel->fYCoord = 0;
          sentinel->fXIndex = sentinel->fYIndex = sentinel->fTaxelNumber = -1;
          sentinel->fBottomTaxel = sentinel->fLeftTaxel = sentinel->fRightTaxel =
              sentinel->fTopTaxel = sentinel;
#if defined(MTC_USE_CORNERS)
          sentinel->fBottomLeftTaxel = sentinel->fBottomRightTaxel =
              sentinel->fTopLeftTaxel = sentinel->fTopRightTaxel = sentinel;
#endif /* MTC_USE_CORNERS */
          bufferScan = *bufferHandle;
          /* Load up the taxels, from the beginning of the buffer. */
          bufferLineStart = *bufferHandle;
          lineCount = 0;
          aTaxel = *anArray;
          for (taxelCount = 0; taxelCount < xx->fNumTaxels; )
          {
            bufferLineEnd = bufferLineStart;
            for ( ; ; )
            {
              aChar = *bufferLineEnd++;
              if ((bufferLineEnd == bufferEnd) || (aChar == '\r'))
                break;

            }
            if ((*bufferLineStart != '$') && (*bufferLineStart != '#'))
            {
              /* Ignore comments and control lines. Process a data line. */
              lineSize = static_cast<size_t>(bufferLineEnd - (bufferLineStart + 1));
              if (lineSize > LINEBUFFER_SIZE)
                lineSize = LINEBUFFER_SIZE;
              strncpy(lineBuffer, bufferLineStart + 1, lineSize);
              *(lineBuffer + lineSize) = 0;
              if (sscanf(lineBuffer, "%hd%hd%f%f", &yIndex, &xIndex, &xCoord,
                          &yCoord) != 4)
              {
                LOG_ERROR_2(OUTPUT_PREFIX "problem with mapping file, data line %hd",
                            lineCount)
                okSoFar = false;
                break;

              }
              if (xIndex > xx->fMaxCol)
                xx->fMaxCol = xIndex;
              if (yIndex > xx->fMaxRow)
                xx->fMaxRow = yIndex;
              if (xCoord > xx->fXMax)
              {
                if (controlLineSeen)
                  LOG_ERROR_1(OUTPUT_PREFIX "taxel with x coord greater than declared "
                              "max seen")
                xx->fXMax = xCoord;
              }
              if (yCoord > xx->fYMax)
              {
                if (controlLineSeen)
                  LOG_ERROR_1(OUTPUT_PREFIX "taxel with y coord greater than declared "
                              "max seen")
                xx->fYMax = yCoord;
              }
              if (xCoord > xx->fXMaxActual)
                xx->fXMaxActual = xCoord;
              if (xCoord < xx->fXMinActual)
                xx->fXMinActual = xCoord;
              if (yCoord > xx->fYMaxActual)
                xx->fYMaxActual = yCoord;
              if (yCoord < xx->fYMinActual)
                xx->fYMinActual = yCoord;
              aTaxel->fXCoord = xCoord;
              aTaxel->fYCoord = yCoord;
              aTaxel->fXIndex = xIndex;
              aTaxel->fYIndex = yIndex;
              aTaxel->fTaxelNumber = taxelCount;
              aTaxel->fBottomTaxel = aTaxel->fLeftTaxel = aTaxel->fRightTaxel =
                  aTaxel->fTopTaxel = sentinel;
#if defined(MTC_USE_CORNERS)
              aTaxel->fBottomLeftTaxel = aTaxel->fBottomRightTaxel =
                  aTaxel->fTopLeftTaxel = aTaxel->fTopRightTaxel = sentinel;
#endif /* MTC_USE_CORNERS */
              aTaxel++;
              taxelCount++;
            }
            if (bufferLineEnd == bufferEnd)
              /* Processed last line. */
              break;

            lineCount++;
            bufferLineStart = bufferLineEnd;
          }
        }
        else
        {
          LOG_ERROR_1(OUTPUT_PREFIX "could not allocate taxel data")
          okSoFar = false;
        }
      }
      HUnlock(bufferHandle);
      DisposeHandle(bufferHandle);
    }
  }
  if (okSoFar)
  {
    MtcTaxelHdl aMatrix = GETBYTES(xx->fMaxCol * xx->fMaxRow, MtcTaxelPtr);

    if (aMatrix)
    {
      xx->fTaxelMatrix = aMatrix;
      xx->fExpectedPackets = short(((xx->fMaxCol * xx->fMaxRow) + 7) / 8);
      for (long ii = 0; ii < (xx->fMaxRow * xx->fMaxCol); ii++, aMatrix++)
        *aMatrix = xx->fSentinelTaxel;
    }
    else
    {
      LOG_ERROR_1(OUTPUT_PREFIX "could not allocate taxel matrix")
      okSoFar = false;
    }
  }
  return okSoFar;
} /* mtcReadMapFile */

/*------------------------------------ mtcReadNormalizationFile ---*/
bool mtcReadNormalizationFile
  (MtcControlPtr xx)
{
  bool    okSoFar = false;
  Handle  bufferHandle;
  long    bufferSize;
  OSErr   err;
  Qchar		bufferScan, bufferEnd, bufferLineStart, bufferLineEnd;
#if defined(COMPILE_FOR_CATS)
	long		outType, inType = long('TEXT');
#else /* not COMPILE_FOR_CATS */
  Str255	tempName;
  FSSpec	fileSpec;
#endif /* not COMPILE_FOR_CATS */

  if (! strlen(xx->fNormalFileName))
    /* No normalization file given. */
    return true; /* ?? */

#if defined(COMPILE_FOR_CATS)
	err = locatefile_extended(xx->fNormalFileName, &xx->fNormalFilePath,
														&outType, &inType, 1);
  if (err != noErr)
    LOG_ERROR_2(OUTPUT_PREFIX "could not locate normalization file '%s'",
                xx->fNormalFileName)
#else /* not COMPILE_FOR_CATS */
  strcpy(reinterpret_cast<Pchar>(tempName), xx->fNormalFileName);
  C2PStr(reinterpret_cast<Pchar>(tempName));
  err = FSMakeFSSpec(xx->fMapFileVolume, 0L, tempName, &fileSpec);
  if (err != noErr)
    LOG_ERROR_2(OUTPUT_PREFIX "problem (%ld) building file spec for normalization file",
                long(err))
#endif /* not COMPILE_FOR_CATS */
  else
  {
#if defined(COMPILE_FOR_CATS)
    FILE_REF	refNum;
#else /* not COMPILE_FOR_CATS */
    short			refNum;
#endif /* not COMPILE_FOR_CATS */

#if defined(COMPILE_FOR_CATS)
    err = path_openfile(xx->fNormalFileName, xx->fNormalFilePath, &refNum, READ_PERM);
#else /* not COMPILE_FOR_CATS */
    err = FSpOpenDF(&fileSpec, fsRdPerm, &refNum);
#endif /* not COMPILE_FOR_CATS */
    if (err != noErr)
      LOG_ERROR_2(OUTPUT_PREFIX "problem (%ld) opening the normalization file",
                  long(err))
    else
    {
      okSoFar = true;
      err = GetEOF(refNum, &bufferSize);
      if (err != noErr)
      {
        LOG_ERROR_2(OUTPUT_PREFIX "problem (%ld) getting normalization file size",
                    long(err))
        okSoFar = false;
      }
      if (okSoFar)
      {
        bufferHandle = NewHandle(bufferSize);
        if (! bufferHandle)
        {
          LOG_ERROR_1(OUTPUT_PREFIX "problem allocating space for normalization file "
                      "buffer")
          okSoFar = false;
        }
      }
      if (okSoFar)
      {
        long    count = bufferSize;

        HLock(bufferHandle);
        err = FSRead(refNum, &count, *bufferHandle);
        HUnlock(bufferHandle);
        if (err != noErr)
        {
          LOG_ERROR_2(OUTPUT_PREFIX "problem (%ld) reading from normalization file",
                      long(err))
          okSoFar = false;
        }
      }
      err = FSClose(refNum);
      if (err != noErr)
        LOG_ERROR_2(OUTPUT_PREFIX "problem (%ld) closing the normalization file",
                    long(err))
    }
  }
  if (okSoFar)
  {
    char        aChar;
    short       lineCount = 0;
    size_t      lineSize;
    char        lineBuffer[LINEBUFFER_SIZE + 1];    /* Use for scan processing. */
    MtcTaxelPtr aTaxel;

    /* We now have the whole file in our buffer, to be read. */
    HLock(bufferHandle);
    bufferScan = *bufferHandle;
    bufferEnd = bufferScan + bufferSize;
    bufferLineStart = *bufferHandle;
    aTaxel = *xx->fTaxelMapHandle;

    /* Load up the taxels, from the beginning of the buffer. */
    for (short taxelCount = 0; taxelCount < xx->fNumTaxels; )
    {
      bufferLineEnd = bufferLineStart;
      for ( ; ; )
      {
        aChar = *bufferLineEnd++;
        if ((bufferLineEnd == bufferEnd) || (aChar == '\r'))
          break;

      }
      if (*bufferLineStart != '#')
      {
        /* Ignore comments. Process a data line. */
        lineSize = static_cast<size_t>(bufferLineEnd - (bufferLineStart + 1));
        if (lineSize > LINEBUFFER_SIZE)
          lineSize = LINEBUFFER_SIZE;
        strncpy(lineBuffer, bufferLineStart + 1, lineSize);
        *(lineBuffer + lineSize) = 0;
        if (sscanf(lineBuffer, "%hd%lf", &aTaxel->fRawMin, &aTaxel->fScale) != 2)
        {
          LOG_ERROR_2(OUTPUT_PREFIX "problem with normalization file, data line %hd",
                      lineCount)
          okSoFar = false;
          break;

        }
        aTaxel++;
        taxelCount++;
      }
      if (bufferLineEnd == bufferEnd)
        /* Processed last line. */
        break;

      lineCount++;
      bufferLineStart = bufferLineEnd;
    }
    HUnlock(bufferHandle);
    DisposeHandle(bufferHandle);
  }
  return okSoFar;
} /* mtcReadNormalizationFile */

/*------------------------------------ mtcResetNormalization ---*/
void mtcResetNormalization
  (MtcControlPtr xx)
{
  MtcTaxelPtr aTaxel = *xx->fTaxelMapHandle;

  for (short taxelCount = 0; taxelCount < xx->fNumTaxels; taxelCount++, aTaxel++)
  {
    aTaxel->fRawMin = MTC_MAX_RAW_PRESSURE;
    aTaxel->fRawMax = MTC_MIN_RAW_PRESSURE;
    aTaxel->fScale = 0;
  }
} /* mtcResetNormalization */

/*------------------------------------ mtcSetupIndices ---*/
void mtcSetupIndices
  (MtcControlPtr xx)
{
  double      deltaX, deltaY, distance, fudge, test, closest;
  double      leftBracket, rightBracket, topBracket, bottomBracket;
  short       outerTaxelCount, innerTaxelCount;
  MtcTaxelPtr outerTaxel, innerTaxel, candidate, sentinel = xx->fSentinelTaxel;

  /* Fill in the data recovery table */
  for (short ii = 0; ii < 256; ii++)
    *(xx->fDataRecovery + ii) = short((ii < 200) ? ii :
                                                  (200 + (((ii - 200) * 823) / 55)));
  if (xx->fMaxCol > 1)
    deltaX = (xx->fXMaxActual - xx->fXMinActual) / (xx->fMaxCol - 1);
  else
    deltaX = 1; /* for the lack of any better value! */
  if (xx->fMaxRow > 1)
    deltaY = (xx->fYMaxActual - xx->fYMinActual) / (xx->fMaxRow - 1);
  else
    deltaY = 1; /* for the lack of any better value! */
  for (outerTaxelCount = 0, outerTaxel = *xx->fTaxelMapHandle;
        outerTaxelCount < xx->fNumTaxels; outerTaxelCount++, outerTaxel++)
  {
    /* Determine right neighbour */
    candidate = NULL_PTR;
    closest = 1e6;
    leftBracket = (deltaX * NEIGHBOUR_FUDGE / 2);
    rightBracket = (deltaX * NEIGHBOUR_FUDGE);
    fudge = (deltaY * NEIGHBOUR_FUDGE / 2);
    for (innerTaxelCount = 0, innerTaxel = *xx->fTaxelMapHandle;
          innerTaxelCount < xx->fNumTaxels; innerTaxelCount++, innerTaxel++)
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
      outerTaxel->fRightTaxel = candidate;
    /* Determine left neighbour */
    candidate = NULL_PTR;
    closest = 1e6;
    leftBracket = (deltaX * NEIGHBOUR_FUDGE / 2);
    rightBracket = (deltaX * NEIGHBOUR_FUDGE);
    for (innerTaxelCount = 0, innerTaxel = *xx->fTaxelMapHandle;
          innerTaxelCount < xx->fNumTaxels; innerTaxelCount++, innerTaxel++)
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
      outerTaxel->fLeftTaxel = candidate;
    /* Determine top neighbour */
    candidate = NULL_PTR;
    closest = 1e6;
    bottomBracket = (deltaY * NEIGHBOUR_FUDGE / 2);
    topBracket = (deltaY * NEIGHBOUR_FUDGE);
    fudge = (deltaX * NEIGHBOUR_FUDGE / 2);
    for (innerTaxelCount = 0, innerTaxel = *xx->fTaxelMapHandle;
          innerTaxelCount < xx->fNumTaxels; innerTaxelCount++, innerTaxel++)
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
      outerTaxel->fTopTaxel = candidate;
    /* Determine bottom neighbour */
    candidate = NULL_PTR;
    closest = 1e6;
    bottomBracket = (deltaY * NEIGHBOUR_FUDGE / 2);
    topBracket = (deltaY * NEIGHBOUR_FUDGE);
    fudge = (deltaX * NEIGHBOUR_FUDGE / 2);
    for (innerTaxelCount = 0, innerTaxel = *xx->fTaxelMapHandle;
          innerTaxelCount < xx->fNumTaxels; innerTaxelCount++, innerTaxel++)
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
      outerTaxel->fBottomTaxel = candidate;
  }
#if defined(MTC_USE_CORNERS)
  /* Set up diagonal indices */
  for (outerTaxelCount = 0, outerTaxel = *xx->fTaxelMapHandle;
        outerTaxelCount < xx->fNumTaxels; outerTaxelCount++, outerTaxel++)
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
  for (outerTaxelCount = 0, outerTaxel = *xx->fTaxelMapHandle;
        outerTaxelCount < xx->fNumTaxels; outerTaxelCount++, outerTaxel++)
    *(xx->fTaxelMatrix + (((outerTaxel->fYIndex - 1) * xx->fMaxCol) +
        outerTaxel->fXIndex - 1)) = outerTaxel;
} /* mtcSetupIndices */

/*------------------------------------ mtcSortByPressure ---*/
static void mtcSortByPressure
  (MtcSpotPtr		spots,
   const short	numSpots)
{
  /* Note that we are guaranteed that the Atoms are all float values. */
  bool didSwap = true;

  while (didSwap)
  {
    didSwap = false;
    for (short ii = 0; ii < numSpots; ii++)
    {
      bool       swappedII = false;
      MtcSpotPtr atomII = spots + ii;
      float      iiValue = atomII->fPressure.a_w.w_float;

      for (short jj = short(ii + 1); jj < numSpots; jj++)
      {
        MtcSpotPtr atomJJ = spots + jj;

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
        didSwap = true;
    }
  }   
} /* mtcSortByPressure */

/*------------------------------------ mtcSortByX ---*/
static void mtcSortByX
  (MtcSpotPtr		spots,
   const short	numSpots)
{
  /* Note that we are guaranteed that the Atoms are all float values. */
  bool didSwap = true;

  while (didSwap)
  {
    didSwap = false;
    for (short ii = 0; ii < numSpots; ii++)
    {
      bool       swappedII = false;
      MtcSpotPtr atomII = spots + ii;
      float      iiValue = atomII->fXCoord.a_w.w_float;

      for (short jj = short(ii + 1); jj < numSpots; jj++)
      {
        MtcSpotPtr atomJJ = spots + jj;

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
      didSwap = true;
    }
  }   
} /* mtcSortByX */

/*------------------------------------ mtcSortByY ---*/
static void mtcSortByY
  (MtcSpotPtr		spots,
   const short	numSpots)
{
  /* Note that we are guaranteed that the Atoms are all float values. */
  bool didSwap = true;

  while (didSwap)
  {
    didSwap = false;
    for (short ii = 0; ii < numSpots; ii++)
    {
      bool       swappedII = false;
      MtcSpotPtr atomII = spots + ii;
      float      iiValue = atomII->fYCoord.a_w.w_float;

      for (short jj = short(ii + 1); jj < numSpots; jj++)
      {
        MtcSpotPtr atomJJ = spots + jj;

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
        didSwap = true;
    }
  }   
} /* mtcSortByY */

/*------------------------------------ mtcSortTaxels ---*/
void mtcSortTaxels
  (MtcControlPtr	xx,
   const short		numSpots)
{
  /* We'll use a simple bubble sort, as the maximum number of spots is quite small */
  switch (xx->fSortOrder)
  {
    case MTC_BY_PRESSURE:
      mtcSortByPressure(xx->fSpots, numSpots);
      break;

    case MTC_BY_X:
      mtcSortByX(xx->fSpots, numSpots);
      break;

    case MTC_BY_Y:
      mtcSortByY(xx->fSpots, numSpots);
      break;

  }
} /* mtcSortTaxels */

/*------------------------------------ mtcWriteNormalizationFile ---*/
bool mtcWriteNormalizationFile
  (MtcControlPtr xx)
{
  bool		okSoFar = false;
  OSErr		err;
  char		lineBuffer[LINEBUFFER_SIZE + 1];    /* Use for scan processing. */
#if defined(COMPILE_FOR_CATS)
	long		outType, inType = long('TEXT');
#else /* not COMPILE_FOR_CATS */
  Str255	tempName;
  FSSpec	fileSpec;
#endif /* not COMPILE_FOR_CATS */

#if (! defined(COMPILE_FOR_CATS))
  strcpy(reinterpret_cast<Pchar>(tempName), xx->fNormalFileName);
  C2PStr(reinterpret_cast<Pchar>(tempName));
#endif /* not COMPILE_FOR_CATS */
#if defined(COMPILE_FOR_CATS)
	err = locatefile_extended(xx->fNormalFileName, &xx->fNormalFilePath,
														&outType, &inType, 1); 
  if (err != noErr)
    LOG_ERROR_2(OUTPUT_PREFIX "could not locate normalization file '%s'",
                xx->fNormalFileName)
#else /* not COMPILE_FOR_CATS */
  err = FSMakeFSSpec(xx->fMapFileVolume, 0L, tempName, &fileSpec);
  if (err != noErr)
    LOG_ERROR_2(OUTPUT_PREFIX "problem (%ld) building file spec for normalization file",
                long(err))
#endif /* not COMPILE_FOR_CATS */
  else
  {
#if defined(COMPILE_FOR_CATS)
    FILE_REF	refNum;
#else /* not COMPILE_FOR_CATS */
    short			refNum;
#endif /* not COMPILE_FOR_CATS */

#if defined(COMPILE_FOR_CATS)
		err = path_openfile(xx->fNormalFileName, xx->fNormalFilePath, &refNum, WRITE_PERM);
#else /* not COMPILE_FOR_CATS */
    err = FSpOpenDF(&fileSpec, fsWrPerm, &refNum);
#endif /* not COMPILE_FOR_CATS */
    if (err != noErr)
      LOG_ERROR_2(OUTPUT_PREFIX "problem (%ld) opening the normalization file",
                  long(err))
    else
    {
      MtcTaxelPtr aTaxel;
      long        count, here;

      okSoFar = true;
      aTaxel = *xx->fTaxelMapHandle;
      for (short taxelCount = 0; taxelCount < xx->fNumTaxels; taxelCount++)
      {
        sprintf(lineBuffer, " %hd %lf\r", aTaxel->fRawMin, aTaxel->fScale);
        count = long(strlen(lineBuffer));
        err = FSWrite(refNum, &count, lineBuffer);
        if (err != noErr)
        {
          LOG_ERROR_2(OUTPUT_PREFIX "problem (%ld) writing to normalization file",
                      long(err))
          okSoFar = false;
          break;

        }
        aTaxel++;
      }
      /* Truncate the file. */
      if (okSoFar)
      {
        err = GetFPos(refNum, &here);
        if (err != noErr)
        {
          LOG_ERROR_2(OUTPUT_PREFIX "problem (%ld) getting the normalization file "
                      "position", long(err))
          okSoFar = false;
        }
      }
      if (okSoFar)
      {
        err = SetEOF(refNum, here);
        if (err != noErr)
        {
          LOG_ERROR_2(OUTPUT_PREFIX "problem (%ld) truncating the normalization file",
                      long(err))
          okSoFar = false;
        }
      }
      err = FSClose(refNum);
      if (err != noErr)
      {
        LOG_ERROR_2(OUTPUT_PREFIX "problem (%ld) closing the normalization file",
                    long(err))
        okSoFar = false;
      }
    }
  }
  return okSoFar;
} /* mtcWriteNormalizationFile */
