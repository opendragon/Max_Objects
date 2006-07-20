/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       rcx.h                                                                   */
/*                                                                                      */
/*  Contains:   The data type declarations and forward references to routines.          */
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
/*  Created:    2002/03/31                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(RCX_H_))
 #define RCX_H_ /* */
 
//#define USE_SYSLOG /* */

 #if (! defined(COMPILE_FOR_CATS))
  #include "GhostAPI.h"
  #define UCHAR_DEFINED /* */
 #endif /* not COMPILE_FOR_CATS */
 #include "MissingAndExtra.h"
 #if defined(COMPILE_FOR_CATS)
  #include "Common_USBX.h"
  #include "Common_USBXData.h"
  #include "LTW_Data.h"
 #else /* not COMPILE_FOR_CATS */
typedef int32	Int32;
 #endif /* not COMPILE_FOR_CATS */

 #define OUR_NAME      "rcx"
 #define OUR_RES_NUMB  17191
 #define OUTPUT_PREFIX "rcx: "

 #define RCX_CONTROL_SYMBOL "##rcx##"

 #define BE_VERBOSE       /* */
 #define MAX_REPLY_BUFFER		100
 #define MAX_RECEIVE_BUFFER	((MAX_REPLY_BUFFER * 2) + 17) /* 17 is for luck... */
// #define USE_NOWAIT       /* */

 #define RCX_NUM_SENSORS   3
 #define RCX_NUM_OUTPUTS   3
 #define RCX_NUM_PROGRAMS  5
 #define RCX_NUM_TASKS     10
 #define RCX_NUM_VARIABLES 32
 #define RCX_OUTPUT_MASK   7

enum RcxCommandCode
{
  RCX_no_command             = 0x0000,
  RCX_PLAY_TONE_VAR_CMD      = 0x0002,
  RCX_DIRECT_EVENT_CMD       = 0x0003,
  RCX_CALIBRATE_EVENT_CMD    = 0x0004,
  RCX_SET_SOURCE_VALUE_CMD   = 0x0005,
  RCX_CLEAR_ALL_EVENTS_CMD   = 0x0006,
  RCX_PING_CMD               = 0x0010,
  	RCX_CLEAR_TACHO_CMD        = 0x0011, //??
  RCX_POLL_CMD               = 0x0012,
  RCX_SET_POWER_CMD          = 0x0013,
  RCX_SET_VARIABLE_CMD       = 0x0014,
  RCX_UNLOCK_CMD             = 0x0015,
  RCX_MEMMAP_CMD             = 0x0020,
  RCX_ON_OFF_FLOAT_CMD       = 0x0021,
  RCX_SET_WATCH_CMD          = 0x0022,
  RCX_PLAY_TONE_CMD          = 0x0023,
  RCX_SUM_VARIABLE_CMD       = 0x0024,
  RCX_BEGIN_TASK_CMD         = 0x0025,
  RCX_POLL_BATTERY_CMD       = 0x0030,
  RCX_TRANSMIT_POWER_CMD     = 0x0031,
  RCX_SET_SENSOR_TYPE_CMD    = 0x0032,
  RCX_SELECT_DISPLAY_CMD     = 0x0033,
  RCX_SUBTRACT_VARIABLE_CMD  = 0x0034,
  RCX_BEGIN_SUB_CMD          = 0x0035,
  RCX_DELETE_ALL_TASKS_CMD   = 0x0040,
    RCX_DRIVE_CMD              = 0x0041, //??
  RCX_SET_SENSOR_MODE_CMD    = 0x0042,
  RCX_DIVIDE_VARIABLE_CMD    = 0x0044,
  RCX_CONTINUE_DOWNLOAD_CMD  = 0x0045,
  RCX_STOP_ALL_TASKS_CMD     = 0x0050,
  RCX_PLAY_SOUND_CMD         = 0x0051,
  RCX_SET_DATALOG_CMD        = 0x0052,
  RCX_MULTIPLY_VARIABLE_CMD  = 0x0054,
  RCX_TURN_OFF_CMD           = 0x0060,
  RCX_DELETE_TASK_CMD        = 0x0061,
  RCX_DATALOG_NEXT_CMD       = 0x0062,
  RCX_UPLOAD_RAM_CMD         = 0x0063,
  RCX_SIGN_VARIABLE_CMD      = 0x0064,
  RCX_RESET_CMD              = 0x0065,
  RCX_CONNECT_DISCONNECT_CMD = 0x0067,
  RCX_DELETE_ALL_SUBS_CMD    = 0x0070,
  RCX_START_TASK_CMD         = 0x0071,
  RCX_ABS_VARIABLE_CMD       = 0x0074,
  RCX_BEGIN_DOWNLOAD_CMD     = 0x0075,
  RCX_NORM_REV_ALT_CMD       = 0x0077,
  RCX_CLEAR_SOUND_CMD        = 0x0080,
  RCX_STOP_TASK_CMD          = 0x0081,
  RCX_AND_VARIABLE_CMD       = 0x0084,
  RCX_CLEAR_MESSAGE_CMD      = 0x0090,
  RCX_SELECT_PROGRAM_CMD     = 0x0091,
  RCX_SET_EVENT_CMD          = 0x0093,
  RCX_OR_VARIABLE_CMD        = 0x0094,
  RCX_INC_COUNTER_CMD        = 0x0097,
  RCX_CLEAR_TIMER_CMD        = 0x00A1,
  RCX_SET_MAX_POWER_CMD      = 0x00A3,
  RCX_UPLOAD_DATALOG_CMD     = 0x00A4,
  RCX_UNLOCK_FIRMWARE_CMD    = 0x00A5,
  RCX_DEC_COUNTER_CMD        = 0x00A7,
  RCX_POWER_DOWN_TIME_CMD    = 0x00B1,
  RCX_CLEAR_COUNTER_CMD      = 0x00B7,
  RCX_DELETE_SUB_CMD         = 0x00C1,
  RCX_SEND_UART_DATA_CMD     = 0x00C2,
  RCX_MUTE_SOUND_CMD         = 0x00D0,
  RCX_CLEAR_SENSOR_VALUE_CMD = 0x00D1,
  RCX_REMOTE_CMD             = 0x00D2,
  RCX_UNMUTE_SOUND_CMD       = 0x00E0,
  RCX_FWD_REV_BACK_CMD       = 0x00E1,
  RCX_VIEW_SOURCE_VALUE_CMD  = 0x00E5,
    RCX_CONTINUE_TASK_CMD      = 0x00F1, //??
  RCX_INTERN_MESSAGE_CMD     = 0x00F7
}; /* RcxCommandCode */

enum RcxExpectedReplyLength
{
  RCX_PLAY_TONE_VAR_REPLY      = 1,
  RCX_DIRECT_EVENT_REPLY       = 1,
  RCX_CALIBRATE_EVENT_REPLY    = 1,
  RCX_SET_SOURCE_VALUE_REPLY   = 1,
  RCX_CLEAR_ALL_EVENTS_REPLY   = 1,
  RCX_PING_REPLY               = 1,
  	RCX_CLEAR_TACHO_REPLY        = -1, //??
  RCX_POLL_REPLY               = 3,
  RCX_SET_POWER_REPLY          = 1,
  RCX_SET_VARIABLE_REPLY       = 1,
  RCX_UNLOCK_REPLY             = 9,
  RCX_MEMMAP_REPLY             = 94,
  RCX_ON_OFF_FLOAT_REPLY       = 1,
  RCX_SET_WATCH_REPLY          = 1,
  RCX_PLAY_TONE_REPLY          = 1,
  RCX_SUM_VARIABLE_REPLY       = 1,
  RCX_BEGIN_TASK_REPLY         = 2,
  RCX_POLL_BATTERY_REPLY       = 3,
  RCX_TRANSMIT_POWER_REPLY     = 1,
  RCX_SET_SENSOR_TYPE_REPLY    = 1,
  RCX_SELECT_DISPLAY_REPLY     = 1,
  RCX_SUBTRACT_VARIABLE_REPLY  = 1,
  RCX_BEGIN_SUB_REPLY          = 2,
  RCX_DELETE_ALL_TASKS_REPLY   = 1,
  	RCX_DRIVE_REPLY              = -1, //??
  RCX_SET_SENSOR_MODE_REPLY    = 1,
  RCX_DIVIDE_VARIABLE_REPLY    = 1,
  RCX_CONTINUE_DOWNLOAD_REPLY  = 2,
  RCX_STOP_ALL_TASKS_REPLY     = 1,
  RCX_PLAY_SOUND_REPLY         = 1,
  RCX_SET_DATALOG_REPLY        = 1,
  RCX_MULTIPLY_VARIABLE_REPLY  = 1,
  RCX_TURN_OFF_REPLY           = 1,
  RCX_DELETE_TASK_REPLY        = 1,
  RCX_DATALOG_NEXT_REPLY       = 1,
  RCX_UPLOAD_RAM_REPLY         = 150, /* variable */
  RCX_SIGN_VARIABLE_REPLY      = 1,
  RCX_RESET_REPLY              = 1,
  RCX_CONNECT_DISCONNECT_REPLY = 1,
  RCX_DELETE_ALL_SUBS_REPLY    = 1,
  RCX_START_TASK_REPLY         = 1,
  RCX_ABS_VARIABLE_REPLY       = 1,
  RCX_BEGIN_DOWNLOAD_REPLY     = 2,
  RCX_NORM_REV_ALT_REPLY       = 1,
  RCX_CLEAR_SOUND_REPLY        = 1,
  RCX_STOP_TASK_REPLY          = 1,
  RCX_AND_VARIABLE_REPLY       = 1,
  RCX_CLEAR_MESSAGE_REPLY      = 1,
  RCX_SELECT_PROGRAM_REPLY     = 1,
  RCX_SET_EVENT_REPLY          = 1,
  RCX_OR_VARIABLE_REPLY        = 1,
  RCX_INC_COUNTER_REPLY        = 1,
  RCX_CLEAR_TIMER_REPLY        = 1,
  RCX_SET_MAX_POWER_REPLY      = 1,
  RCX_UPLOAD_REPLY             = 150, /* variable */
  RCX_UNLOCK_FIRMWARE_REPLY    = 26, /* xx, "Just a bit off the block!" */
  RCX_DEC_COUNTER_REPLY        = 1,
  RCX_POWER_DOWN_REPLY         = 1,
  RCX_CLEAR_COUNTER_REPLY      = 1,
  RCX_DELETE_SUB_REPLY         = 1,
  RCX_SEND_UART_DATA_REPLY     = 0,
  RCX_MUTE_SOUND_REPLY         = 1,
  RCX_CLEAR_SENSOR_VALUE_REPLY = 1,
  RCX_REMOTE_REPLY             = 0,
  RCX_UNMUTE_SOUND_REPLY       = 1,
  RCX_FWD_REV_BACK_REPLY       = 1,
  RCX_VIEW_SOURCE_VALUE_REPLY  = 1,
  	RCX_CONTINUE_TASK_REPLY      = 1, //??
  RCX_INTERN_MESSAGE_REPLY     = 0
}; /* RcxExpectedReplyLength */

enum RcxOutputDirection
{
  RCX_OUTPUT_BACKWARD = 0x0000,
  RCX_OUTPUT_REVERSE  = 0x0040,
  RCX_OUTPUT_FORWARD  = 0x0080
}; /* RcxOutputDirection */

enum RcxOutputStatus
{
  RCX_OUTPUT_FLOAT = 0x0000,
  RCX_OUTPUT_OFF   = 0x0040,
  RCX_OUTPUT_ON    = 0x0080
}; /* RcxOutputStatus */

enum RcxSensorMode
{
  RCX_SENSOR_RAW                = 0x0000,
  RCX_SENSOR_BOOLEAN            = 0x0020,
  RCX_SENSOR_TRANSITION_COUNT   = 0x0040,
  RCX_SENSOR_PERIOD_COUNTER     = 0x0060,
  RCX_SENSOR_PERCENT_FULL_SCALE = 0x0080,
  RCX_SENSOR_CELSIUS            = 0x00A0,
  RCX_SENSOR_FAHRENHEIT         = 0x00C0,
  RCX_SENSOR_ANGLE_STEP         = 0x00E0,
  RCX_MODE_MASK                 = 0X00E0
}; /* RcxSensorMode */

enum RcxSensorType
{
  RCX_SENSOR_NONE        = 0,
  RCX_SENSOR_SWITCH      = 1,
  RCX_SENSOR_TEMPERATURE = 2,
  RCX_SENSOR_REFLECTION  = 3,
  RCX_SENSOR_ANGLE       = 4
}; /* RcxSensorType */

enum RcxSoundType
{
  RCX_KEY_CLICK_SOUND  = 0,
  RCX_BEEP_SOUND       = 1,
  RCX_SWEEP_DOWN_SOUND = 2,
  RCX_SWEEP_UP_SOUND   = 3,
  RCX_ERROR_SOUND      = 4,
  RCX_FAST_SWEEP_SOUND = 5
}; /* RcxSoundType */

enum RcxValueType
{
  RCX_VARIABLE_TYPE            = 0,
  RCX_TIMER_TYPE               = 1,
  RCX_CONSTANT_TYPE            = 2,
  RCX_MOTOR_STATUS_TYPE        = 3,
  RCX_RANDOM_TYPE              = 4,
  RCX_TACH_COUNTER_TYPE        = 5,
  RCX_TACH_SPEED_TYPE          = 6,
  RCX_OUTPUT_CURRENT_TYPE      = 7,
  RCX_PROGRAM_SLOT_TYPE        = 8,
  RCX_SENSOR_VALUE_TYPE        = 9,
  RCX_SENSOR_TYPE_TYPE         = 10,
  RCX_SENSOR_MODE_TYPE         = 11,
  RCX_SENSOR_RAW_TYPE          = 12,
  RCX_SENSOR_BOOLEAN_TYPE      = 13,
  RCX_WATCH_TYPE               = 14,
  RCX_MESSAGE_TYPE             = 15,
  RCX_AGC_TYPE                 = 16,
  RCX_GLOBAL_MOTOR_STATUS_TYPE = 17,
  RCX_COUNTER_TYPE             = 21,
  RCX_TASK_EVENT_TYPE          = 23,
  RCX_EVENT_STATE_TYPE         = 25,
  RCX_10MS_TIMER_TYPE          = 26,
  RCX_CLICK_COUNTER_TYPE       = 27,
  RCX_UPPER_THRESHOLD_TYPE     = 28,
  RCX_LOWER_THRESHOLD_TYPE     = 29,
  RCX_HYSTERESIS_TYPE          = 30,
  RCX_DURATION_TYPE            = 31,
  RCX_UART_SETUP_TYPE          = 33,
  RCX_BATTERY_LEVEL_TYPE       = 34,
  RCX_FIRMWARE_VERSION_TYPE    = 35,
  RCX_INDIRECT_TYPE            = 36
}; /* RcxValueType */

struct RcxControl
{
  Object  										fObject;
  POutlet 										fCommandComplete;
  POutlet 										fDataOut;
  POutlet 										fErrorBangOut;
  Puchar  										fReplyBuffer;
  UInt32  										fReplyLength;
  bool    										fDeviceOpen;
  bool    										fSynchronized;
  bool    										fUseUSB;
 #if defined(COMPILE_FOR_CATS)
	IOKitContext								fUSBControl;
	IOUSBDeviceInterface * *		fDevice;	
	IOUSBInterfaceInterface * *	fInterface;
	IOAsyncCallback1						fReadCompletion;
	uchar												fLastCommand;
	Puchar											fRawReceiveBuffer;
	Puchar											fRawReceiveEnd;
	Puchar											fRawReceiveStart;
	Puchar											fReadWalker;
	Puchar											fReceiveBuffer;
  Quchar											fSync;
  UInt32											fReadPacketSize;
  UInt32											fReadRemaining;
  UInt32											fReceiveLength;
  UInt32											fSyncLen;
  bool												fComplementData;
  bool												fFastMode;
  bool												fHighPower;
  bool												fReadComplete;
  bool												fReportEvents;
  bool												fStopping;
 #else /* not COMPILE_FOR_CATS */
  GHSTACK 										fGhostStack;
  Int32   										fDownloadRetries;
  Int32   										fExecuteRetries;
 #endif /* not COMPILE_FOR_CATS */
 #if defined(BE_VERBOSE)
  bool    										fVerbose;
 #endif /* BE_VERBOSE */
}; /* RcxControl */

typedef RcxControl * RcxControlPtr;

Pvoid cmd_ClearMemory
  (RcxControlPtr xx);

Pvoid cmd_ClearSensor
  (RcxControlPtr xx,
   long          slot);

Pvoid cmd_ClearSound
  (RcxControlPtr xx);

Pvoid cmd_ContinueTask
  (RcxControlPtr xx,
   long          slot);

Pvoid cmd_FloatOutput
  (RcxControlPtr xx,
   long          slots);

Pvoid cmd_GetAllSensors
  (RcxControlPtr xx);

Pvoid cmd_GetAllVariables
  (RcxControlPtr xx);

Pvoid cmd_GetBattery
  (RcxControlPtr xx);

Pvoid cmd_GetSensorMode
  (RcxControlPtr xx,
   long          slot);

Pvoid cmd_GetSensorType
  (RcxControlPtr xx,
   long          slot);

Pvoid cmd_GetSensorValue
  (RcxControlPtr xx,
   long          slot);

Pvoid cmd_GetSlot
  (RcxControlPtr xx);

Pvoid cmd_GetVariable
  (RcxControlPtr xx,
   long          slot);

Pvoid cmd_GetVersion
  (RcxControlPtr xx);

Pvoid cmd_Mute
  (RcxControlPtr xx);

Pvoid cmd_PlaySound
  (RcxControlPtr xx,
   PSymbol       newSound);

Pvoid cmd_PlayTone
  (RcxControlPtr xx,
   long          frequency,
   long          duration);

Pvoid cmd_Run
  (RcxControlPtr xx);

Pvoid cmd_SetMessage
  (RcxControlPtr xx,
   long          aByte);

Pvoid cmd_SetOutputDirection
  (RcxControlPtr xx,
   long          slots,
   PSymbol       newDirection);

Pvoid cmd_SetOutputPower
  (RcxControlPtr xx,
   long          slots,
   long          newPower);

Pvoid cmd_SetSensorMode
  (RcxControlPtr xx,
   long          slot,
   PSymbol       newMode);

Pvoid cmd_SetSensorType
  (RcxControlPtr xx,
   long          slot,
   PSymbol       newType);

Pvoid cmd_SetSlot
  (RcxControlPtr xx,
   long          slot);

Pvoid cmd_SetVariable
  (RcxControlPtr xx,
   long          slot,
   long          newValue);

Pvoid cmd_SetWatch
  (RcxControlPtr xx,
   long          hour,
   long          minute);

Pvoid cmd_StartOutput
  (RcxControlPtr xx,
   long          slots);

Pvoid cmd_StartTask
  (RcxControlPtr xx,
   long          slot);

Pvoid cmd_StopOutput
  (RcxControlPtr xx,
   long          slots);

Pvoid cmd_StopTask
  (RcxControlPtr xx,
   long          slot);

Pvoid cmd_StopAllTasks
  (RcxControlPtr xx);

Pvoid cmd_TurnOff
  (RcxControlPtr xx);

Pvoid cmd_Unmute
  (RcxControlPtr xx);

Pvoid cmd_Verbose
  (RcxControlPtr xx,
   PSymbol       onOff);

UInt32 rcxCopyFromReply
  (RcxControlPtr	xx,
   Puchar       	replyBuffer,
   const UInt32		replySize);

uchar rcxGetReplyByte
  (RcxControlPtr	xx,
   const ushort		index);

bool rcxGetValue
  (RcxControlPtr	xx,
   const ulong		descriptor,
   long &         result);

bool rcxSendCommand
  (RcxControlPtr	xx,
   Quchar					sendData,
   const UInt32		sendLength,
   const UInt32		expected,
   const bool			doRetry);

 #if defined(COMPILE_FOR_CATS)
bool rcxSendControlRequest
	(RcxControlPtr	xx,
	 const uchar		controlOperation,
	 const ushort		controlData,
	 Pvoid					reply,
	 const UInt16		replySize);

bool rcxSendControlRequest
	(RcxControlPtr	xx,
	 const uchar		controlOperation,
	 const uchar		controlDataLow,
	 const uchar		controlDataHigh,
	 Pvoid					reply,
	 const UInt16		replySize);

bool rcxSetRange
	(RcxControlPtr	xx,
	 const LTWRange	range);
	 
bool rcxSetSpeed
	(RcxControlPtr	xx,
	 const bool			normalSpeed);
 #endif /* COMPILE_FOR_CATS */

bool rcxSynchronize
  (RcxControlPtr xx);

StandardRoutineDeclarations(RcxControlPtr)

 #define MAKE_RCX_VALUE(tt, dd) static_cast<ulong>((int(tt) << 16) | ((dd) & 0x0FFFF))
 #define GET_RCX_VALUE_TYPE(vv) (static_cast<RcxValueType>(((vv) >> 16) & 0x0FF))
 #define GET_RCX_VALUE_DATA(vv) short((vv) & 0x0FFFF)

mextern(PSymbol) gAllSensorsSymbol;   /* Pointer to unique symbol for 'allsensors' */
mextern(PSymbol) gAllVariablesSymbol; /* Pointer to unique symbol for 'allvariables' */
mextern(PSymbol) gAngleSymbol;        /* Pointer to unique symbol for 'angle' */
mextern(PSymbol) gAngleStepSymbol;    /* Pointer to unique symbol for 'anglestep' */
mextern(PSymbol) gBackwardSymbol;     /* Pointer to unique symbol for 'backward' */
mextern(PSymbol) gBatterySymbol;      /* Pointer to unique symbol for 'battery' */
mextern(PSymbol) gBeepSymbol;         /* Pointer to unique symbol for 'beep' */
mextern(PSymbol) gBooleanSymbol;      /* Pointer to unique symbol for 'boolean' */
mextern(PSymbol) gCelsiusSymbol;      /* Pointer to unique symbol for 'celsius' */
mextern(PSymbol) gEmptySymbol;        /* Pointer to unique symbol for '' */
mextern(PSymbol) gErrorSymbol;        /* Pointer to unique symbol for 'error' */
mextern(PSymbol) gFahrenheitSymbol;   /* Pointer to unique symbol for 'fahrenheit' */
mextern(PSymbol) gFastSweepSymbol;    /* Pointer to unique symbol for 'fastsweep' */
mextern(PSymbol) gForwardSymbol;      /* Pointer to unique symbol for 'forward' */
mextern(PSymbol) gHighSymbol;					/* Pointer to unique symbol for 'high' */
mextern(PSymbol) gKeyClickSymbol;     /* Pointer to unique symbol for 'keyclick' */
mextern(PSymbol) gLowSymbol;					/* Pointer to unique symbol for 'low' */
mextern(PSymbol) gNoSensorSymbol;     /* Pointer to unique symbol for 'nosensor' */
mextern(PSymbol) gOffSymbol;          /* Pointer to unique symbol for 'off' */
mextern(PSymbol) gOnSymbol;           /* Pointer to unique symbol for 'on' */
mextern(PSymbol) gPercentSymbol;      /* Pointer to unique symbol for 'percent' */
mextern(PSymbol) gPeriodCountSymbol;  /* Pointer to unique symbol for 'periodcount' */
mextern(PSymbol) gRawSymbol;          /* Pointer to unique symbol for 'raw' */
mextern(PSymbol) gReflectionSymbol;   /* Pointer to unique symbol for 'reflection' */
mextern(PSymbol) gReverseSymbol;      /* Pointer to unique symbol for 'reverse' */
mextern(PSymbol) gSensorModeSymbol;   /* Pointer to unique symbol for 'sensormode' */
mextern(PSymbol) gSensorSymbol;       /* Pointer to unique symbol for 'sensor' */
mextern(PSymbol) gSensorTypeSymbol;   /* Pointer to unique symbol for 'sensortype' */
mextern(PSymbol) gSerialSymbol;       /* Pointer to unique symbol for 'serial' */
mextern(PSymbol) gSlotSymbol;         /* Pointer to unique symbol for 'slot' */
mextern(PSymbol) gSweepDownSymbol;    /* Pointer to unique symbol for 'sweepdown' */
mextern(PSymbol) gSweepUpSymbol;      /* Pointer to unique symbol for 'sweepup' */
mextern(PSymbol) gSwitchSymbol;       /* Pointer to unique symbol for 'switch' */
mextern(PSymbol) gTemperatureSymbol;  /* Pointer to unique symbol for 'temperature' */
mextern(PSymbol) gTransitionSymbol;   /* Pointer to unique symbol for 'transition' */
mextern(PSymbol) gUnknownSymbol;      /* Pointer to unique symbol for 'unknown' */
mextern(PSymbol) gUsbSymbol;          /* Pointer to unique symbol for 'usb' */
mextern(PSymbol) gVariableSymbol;     /* Pointer to unique symbol for 'variable' */
mextern(PSymbol) gVersionSymbol;      /* Pointer to unique symbol for 'version' */

#endif /* not RCX_H_ */
