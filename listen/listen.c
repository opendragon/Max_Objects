/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       listen.c                                                                */
/*                                                                                      */
/*  Contains:   The main entry point and support routines for the listen module.        */
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
/*  Created:    2001/04/08                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#define IS_MAIN /* */
#include "listen.h"
#include "displayAboutBox.h"
#include "loadAboutBoxData.h"
#include "loadOtherSegments.h"
#include "reportAnything.h"
#include "reportVersion.h"

/* Forward references: */
bool isSpeechRecognitionAvailable
  (void);

pascal void listenCallBack
  (SRCallBackStruct * param);

Pvoid listenCreate
  (PSymbol doFeedback,
   PSymbol initialFile);

Pvoid listenFree
  (ListenPtr xx);

Pvoid listenProcessQueue
  (ListenPtr xx);

#if SPEECH_RECOGNITION_SUPPORTED
bool traverseResponse
  (ListenPtr        xx,
   PBinbuf          accumulator,
   Pshort           count,
   SRLanguageObject model);
#endif /* SPEECH_RECOGNITION_SUPPORTED */

/*------------------------------------ main ---*/
void main
  (Pfptr ff)
{
  EnterCodeResource();
  PrepareCallback();
  FNS = ff;   /* Connect up the function macros. */
  if (isSpeechRecognitionAvailable())
  {
    /* Allocate class memory and set up class. */
    setup(reinterpret_cast<t_messlist**>(&gClass), reinterpret_cast<method>(listenCreate),  
          reinterpret_cast<method>(listenFree), short(sizeof(ListenData)),
          reinterpret_cast<method>(0L), A_DEFSYM, A_DEFSYM, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Anything), "anything", A_GIMME, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Assist), "assist", A_CANT, A_NOTHING);
    addbang(reinterpret_cast<method>(cmd_Bang));
    addmess(reinterpret_cast<method>(cmd_Info), "info", A_CANT, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Load), "load", A_SYM, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Recognize), "recognize", A_GIMME, A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Start), "start", A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Status), "status", A_NOTHING);
    addmess(reinterpret_cast<method>(cmd_Stop), "stop", A_NOTHING);
    /* Add object class to New Object List */
    finder_addclass("Devices", OUR_NAME);
    /* Make sure that our string list resource is present */
    rescopy('STR#', OUR_RES_NUMB);
    loadAboutBoxData();
    gCloseCurlySymbol = gensym("}");
    gCloseSquareSymbol = gensym("]");
    gEmptySymbol = gensym("");
    gEqualsSymbol = gensym("=");
    gIdleSymbol = gensym("idle");
    gLoadedSymbol = gensym("loaded");
    gLowerOSymbol = gensym("o");
    gLowerRSymbol = gensym("r");
    gNoSymbol = gensym("no");
    gOpenCurlySymbol = gensym("{");
    gOpenSquareSymbol = gensym("[");
    gRejectedWordToken = gensym("!?*$");
    gResultSymbol = gensym("result");
    gStartedSymbol = gensym("started");
    gStatusSymbol = gensym("status");
    gTopLevelSymbol = gensym("<TopLevel>");
    gUpperOSymbol = gensym("O");
    gUpperRSymbol = gensym("R");
    gVertBarSymbol = gensym("|");
    gYesSymbol = gensym("yes");
    gVersionNumber = reportVersion(OUR_NAME);
    loadOtherSegments();
  }
#if (! SPEECH_RECOGNITION_SUPPORTED)
  error(OUTPUT_PREFIX "not supported on Max/69K");
#endif /* not SPEECH_RECOGNITION_SUPPORTED */
  ExitCodeResource();
} /* main */

/*------------------------------------ listenCreate ---*/
Pvoid listenCreate
  (PSymbol doFeedback,
   PSymbol initialFile)
{
#if (! SPEECH_RECOGNITION_SUPPORTED)
 #pragma unused(doFeedback, initialFile)
#endif /* not SPEECH_RECOGNITION_SUPPORTED */
#if SPEECH_RECOGNITION_SUPPORTED
  OSErr     result;
#endif /* SPEECH_RECOGNITION_SUPPORTED */
  bool      okSoFar = true;
  ListenPtr xx;

  EnterCallback();
  LOG_ENTER()
  xx = static_cast<ListenPtr>(newobject(gClass));
  if (xx)
  {
    xx->fPreviousCount = 0;
    xx->fPreviousResult = NULL_PTR;
    xx->fModels = NULL_HDL;
    xx->fBuffer = NULL_PTR;
    xx->fBufferTypeOffset = xx->fBufferStuffOffset = 0;
    xx->fErrorBangOut = static_cast<POutlet>(bangout(xx)); 
    xx->fResultOut = static_cast<POutlet>(outlet_new(xx, NULL_PTR));
    xx->fResultQueue = static_cast<PQelem>(qelem_new(xx,
                                        reinterpret_cast<method>(listenProcessQueue)));
    if (! (xx->fResultOut && xx->fResultQueue && xx->fErrorBangOut))
    {
      LOG_ERROR_1(OUTPUT_PREFIX "unable to create port for object")
      okSoFar = false;
    }
#if SPEECH_RECOGNITION_SUPPORTED
    xx->fRecognitionSystem = NULL_PTR;
    xx->fRecognizer = NULL_PTR;
    xx->fRecognized = NULL_PTR;
    xx->fModelLoaded = xx->fStarted = xx->fProvideFeedback = false;
    if (okSoFar)
    {
      PSymbol checkGlobal = gensym(SR_CONTROL_SYMBOL);

      if (checkGlobal->s_thing)
      {
        /* already set up! */
        LOG_ERROR_1(OUTPUT_PREFIX "listen object already present")
        okSoFar = false;
      }
      else
        checkGlobal->s_thing = reinterpret_cast<PObject>(xx);
    }
    if (okSoFar)
    {
      if (doFeedback == gYesSymbol)
        xx->fProvideFeedback = true;
      else if ((doFeedback != gEmptySymbol) && (doFeedback != gNoSymbol))
      {
        LOG_ERROR_2(OUTPUT_PREFIX "unrecognized feedback option '%s'", doFeedback->s_name)
        okSoFar = false;
      }
    }
    if (okSoFar)
    {
      result = SROpenRecognitionSystem(&xx->fRecognitionSystem,
                                        kSRDefaultRecognitionSystemID);
      if (result != noErr)
      {
        LOG_ERROR_2(OUTPUT_PREFIX "unable to create recognition system for object (%ld)",
                    result)
        okSoFar = false;
      }
    }
    if (okSoFar)
    {
      short feedbackModes = (xx->fProvideFeedback ? kSRHasFeedbackHasListenModes :
                                                    kSRNoFeedbackHasListenModes);

      result = SRSetProperty(xx->fRecognitionSystem, kSRFeedbackAndListeningModes, 
                              &feedbackModes, sizeof(feedbackModes));
      if (result != noErr)
      {
        LOG_ERROR_2(OUTPUT_PREFIX "problem setting listen/feedback modes for "
                    "object (%ld)", result)
        okSoFar = false;
      }
    }
    if (okSoFar)
    {
      SRWord rejectedWord = 0;
      Size   aLen = sizeof(rejectedWord);

      result = SRGetProperty(xx->fRecognitionSystem, kSRRejectedWord, &rejectedWord,
                              &aLen);
      if (result == noErr)
      {
        result = SRSetProperty(rejectedWord, kSRRefCon, &gRejectedWordToken,
        sizeof(gRejectedWordToken));
        if (result != noErr)
        {
          LOG_ERROR_2(OUTPUT_PREFIX "problem tagging rejected word (%ld}", result)
          okSoFar = false;
        }
      }
      else
      {
        LOG_ERROR_2(OUTPUT_PREFIX "problem getting rejected word (%ld)", result)
        okSoFar = false;
      }
      if (rejectedWord)
        SRReleaseObject(rejectedWord);
    }
    if (okSoFar)
    {
      result = SRNewRecognizer(xx->fRecognitionSystem, &xx->fRecognizer,
                                kSRDefaultSpeechSource);
      if (result != noErr)
      {
        LOG_ERROR_2(OUTPUT_PREFIX "unable to create recognizer for object (%ld)", result)
        okSoFar = false;
      }
    }
    if (okSoFar)
    {
      SRCallBackParam speechCallBackPB;

#if defined(COMPILE_FOR_CATS)
      speechCallBackPB.callBack = NewSRCallBackUPP(listenCallBack);
#else /* not COMPILE_FOR_CATS */
      speechCallBackPB.callBack = NewSRCallBackProc(listenCallBack);
#endif /* not COMPILE_FOR_CATS */
      speechCallBackPB.refCon = long(xx);
      result = SRSetProperty(xx->fRecognizer, kSRCallBackParam, &speechCallBackPB,
                              sizeof(speechCallBackPB));
      if (result != noErr)
      {
        LOG_ERROR_2(OUTPUT_PREFIX "unable to attach callback procedure for object (%ld)",
                    result)
        okSoFar = false;
      }
    }
    if (okSoFar)
    {
      long notificationMode = (kSRNotifyRecognitionBeginning | kSRNotifyRecognitionDone);

      result = SRSetProperty(xx->fRecognizer, kSRNotificationParam, &notificationMode,
                              sizeof(notificationMode));
      if (result != noErr)
      {
        LOG_ERROR_2(OUTPUT_PREFIX "unable to set notification mode for object (%ld)",
                    result)
        okSoFar = false;
      }
    }
    if (okSoFar)
    {
      if (initialFile == gEmptySymbol)
        LOG_POST_1(OUTPUT_PREFIX "no initial language file")
      else
      {
        LOG_POST_2(OUTPUT_PREFIX "initial file: %s", initialFile->s_name)
        doLoadLanguageFile(xx, initialFile->s_name);
      }
    }
#endif /* SPEECH_RECOGNITION_SUPPORTED */
    if (! okSoFar)
    {
      freeobject(reinterpret_cast<PObject>(xx));
      xx = NULL_PTR;
    }
  }
  ExitCodeResource();
  return xx;
} /* listenCreate */

/*------------------------------------ listenFree ---*/
Pvoid listenFree
  (ListenPtr xx)
{
  EnterCallback();
  if (xx)
  {
    PSymbol checkGlobal = gensym(SR_CONTROL_SYMBOL);

    if (xx->fResultQueue)
    {
      qelem_unset(xx->fResultQueue);
      qelem_free(xx->fResultQueue);
      xx->fResultQueue = NULL_PTR;
    }
    clearModelData(xx);
#if SPEECH_RECOGNITION_SUPPORTED
    checkGlobal->s_thing = NULL_PTR;
    if (xx->fRecognizer)
    {
      OSErr           result;
      SRCallBackParam callBackPB;
      SRCallBackUPP   savedCallBack;
      Size            aLen = sizeof(callBackPB);

      SRStopListening(xx->fRecognizer);
      result = SRGetProperty(xx->fRecognizer, kSRCallBackParam, &callBackPB, &aLen);
      if (result == noErr)
      {
        if (callBackPB.callBack)
        {
          savedCallBack = callBackPB.callBack;
          callBackPB.callBack = NULL_PTR;
          result = SRSetProperty(xx->fRecognizer, kSRCallBackParam, &callBackPB,
                                  sizeof(callBackPB));
 #if defined(COMPILE_FOR_CATS)
          DisposeSRCallBackUPP(savedCallBack);
 #else /* not COMPILE_FOR_CATS */
          DisposeRoutineDescriptor(savedCallBack);
 #endif /* not COMPILE_FOR_CATS */
        }
      }
      SRReleaseObject(xx->fRecognizer);
      xx->fRecognizer = NULL_PTR;
    }
    if (xx->fRecognitionSystem)
    {
      SRCloseRecognitionSystem(xx->fRecognitionSystem);
      xx->fRecognitionSystem = NULL_PTR;
    }
#endif /* SPEECH_RECOGNITION_SUPPORTED */
  }
  LOG_EXIT()
  ExitMaxMessageHandler()
} /* listenFree */

/*------------------------------------ isSpeechRecognitionAvailable ---*/
bool isSpeechRecognitionAvailable
  (void)
{
#if SPEECH_RECOGNITION_SUPPORTED
  UInt32   gSRVersion;
  OSStatus err = Gestalt(gestaltSpeechRecognitionVersion,
                          reinterpret_cast<Plong>(&gSRVersion));
  bool     okSoFar = true;

  if ((err != noErr) || (gSRVersion < kSRVersionUsed))
  {
    LOG_ERROR_2(OUTPUT_PREFIX "newer version of Speech Recognition Manager "
                "required (%ld)", err)
    okSoFar = false;
  }
  return okSoFar;
#else /* not SPEECH_RECOGNITION_SUPPORTED */
  return false;
#endif /* SPEECH_RECOGNITION_SUPPORTED */
} /* isSpeechRecognitionAvailable */

/*------------------------------------ listenCallBack ---*/
pascal void listenCallBack
  (SRCallBackStruct * param)
{
  EnterCallback();
  if (param)
  {
#if SPEECH_RECOGNITION_SUPPORTED
    ListenPtr xx = reinterpret_cast<ListenPtr>(param->refCon);

    if (xx)
    {
      if (param->what & kSRNotifyRecognitionBeginning)
      {
        SRRecognizer aRecog = static_cast<SRRecognizer>(param->instance);
        OSErr        result = SRContinueRecognition(aRecog);

        if (result != noErr)
        {
          LOG_ERROR_1(OUTPUT_PREFIX "unable to continue")
          outlet_bang(xx->fErrorBangOut);
        }
      }
      else if (param->what & kSRNotifyRecognitionDone)
      {
        SRRecognitionResult aResult = reinterpret_cast<SRRecognitionResult>(param->message);

        if ((param->status == noErr) && aResult)
        {
          xx->fRecognized = aResult;
          qelem_set(xx->fResultQueue);
        }
        else
        {
          if (aResult)
            SRReleaseObject(aResult);
          outlet_bang(xx->fErrorBangOut);
        }
      }
    }
#endif /* SPEECH_RECOGNITION_SUPPORTED */
  }
  ExitCodeResource();
} /* listenCallBack */

/*------------------------------------ listenProcessQueue ---*/
Pvoid listenProcessQueue
  (ListenPtr xx)
{
  EnterCallback();
  if (xx)
  {
#if SPEECH_RECOGNITION_SUPPORTED
    short               prevLock = lockout_set(1), atomCount = 0;
    OSErr               result;
    SRRecognitionResult recognized = xx->fRecognized;
    PBinbuf             accumulator;
    SRLanguageModel     theResultLM;
    Size                len = sizeof(theResultLM);
    bool                errorSeen = false;

    xx->fRecognized = NULL_PTR;
    lockout_set(prevLock);
    evnum_incr(); 
    FREEBYTES(xx->fPreviousResult, xx->fPreviousCount)
    xx->fPreviousCount = 0;
    /* Construct the response from the matching elements */
    accumulator = static_cast<PBinbuf>(binbuf_new());
    /* Get the result as a language model that we can traverse: */
    result = SRGetProperty(recognized, kSRLanguageModelFormat, &theResultLM, &len);
    if (result == noErr)
    {
      long refCon;

      len = sizeof(refCon);
      result = SRGetProperty(theResultLM, kSRRefCon, &refCon, &len);
      if (result == noErr)
      {
        if (refCon == long(gRejectedWordToken))
        {
          LOG_ERROR_1(OUTPUT_PREFIX "unrecognized word or phrase heard")
          errorSeen = true;
        }
        else
          traverseResponse(xx, accumulator, &atomCount, theResultLM);
      }
      else
      {
        LOG_ERROR_1(OUTPUT_PREFIX "problem getting reference constant")
        errorSeen = true;
      }
      if (atomCount > 0)
      {
        PAtom tempList = GETBYTES(atomCount, Atom);

        if (tempList)
        {
          bool  okSoFar = true;
          long  stuffOffset = 0, typeOffset = 0;
          Atom  tempAtom;

          /* Prefill the atom vector, in case of early termination. */
          for (short ii = 0; ii < atomCount; ii++)
            SETLONG(tempList + ii, 0);
          for (short ii = 0; okSoFar && (ii < atomCount); ii++)
          {       
            okSoFar = (! binbuf_getatom(accumulator, &typeOffset, &stuffOffset,
                                        &tempAtom));
            if (okSoFar)
              tempList[ii] = tempAtom;
          }
          xx->fPreviousCount = atomCount;
          xx->fPreviousResult = tempList;
          outlet_anything(xx->fResultOut, gResultSymbol, xx->fPreviousCount,
                          xx->fPreviousResult);
        }
      }
    }
    else
    {
      LOG_ERROR_2(OUTPUT_PREFIX "problem converting response into a model (%ld)", result)
      errorSeen = true;
    }
    SRReleaseObject(theResultLM);
    freeobject(reinterpret_cast<PObject>(accumulator));
    result = SRReleaseObject(recognized);
    if (result != noErr)
    {
      LOG_ERROR_2(OUTPUT_PREFIX "problem freeing response (%ld)", result)
      errorSeen = true;
    }
    if (errorSeen)
      outlet_bang(xx->fErrorBangOut);
#endif /* SPEECH_RECOGNITION_SUPPORTED */
  }
  ExitMaxMessageHandler()
} /* listenProcessQueue */

#if SPEECH_RECOGNITION_SUPPORTED
/*------------------------------------ traverseResponse ---*/
bool traverseResponse
  (ListenPtr        xx,
   PBinbuf          accumulator,
   Pshort           count,
   SRLanguageObject model)
{
  OSErr result;
  long  objType, refCon;
  Size  len = sizeof(objType);
  bool  hasChildren = false, okSoFar = true;

  result = SRGetProperty(model, kSRLMObjType, &objType, &len);
  if (result == noErr)
  {
    MatchDataPtr moreStuff;

    switch (objType)
    {
      case kSRLanguageModelType:
        /* Add the objects attached to the language model: */
        len = sizeof(refCon);
        result = SRGetProperty(model, kSRRefCon, &refCon, &len);
        if (result == noErr)
        {
          moreStuff = (MatchDataPtr) refCon;
          if (moreStuff)
          {
            binbuf_append(accumulator, NULL_PTR, moreStuff->fCount, moreStuff->fData);
            *count += moreStuff->fCount;
          }
        }
        hasChildren = true;
        break;

      case kSRPathType:
        hasChildren = true;
        break;

      case kSRPhraseType:
        hasChildren = true;
        break;

      case kSRWordType:
        /* Add the objects attached to the language model: */
        len = sizeof(refCon);
        result = SRGetProperty(model, kSRRefCon, &refCon, &len);
        if (result == noErr)
        {
          moreStuff = reinterpret_cast<MatchDataPtr>(refCon);
          if (moreStuff)
          {
            binbuf_append(accumulator, NULL_PTR, moreStuff->fCount, moreStuff->fData);
            *count += moreStuff->fCount;
          }
        }
        break;

    }
  }
  if (hasChildren && (result == noErr))
  {
    long           number;
    SRSpeechObject child;

    /* Process the children: */
    result = SRCountItems(model, &number);
    for (long ii = 0; (result == noErr) && (ii < number) && okSoFar; ii++)
    {
      result = SRGetIndexedItem(model, &child, ii);
      if (result == noErr)
      {
        okSoFar = traverseResponse(xx, accumulator, count, child);
        SRReleaseObject(child);
      }
    }
  }
  if (result != noErr)
    okSoFar = false;
  return okSoFar;
} /* traverseResponse */
#endif /* SPEECH_RECOGNITION_SUPPORTED */

StandardAnythingRoutine(ListenPtr)
StandardInfoRoutine(ListenPtr)