#if (! defined(LTW_DATA_H_))
 #define LTW_DATA_H_ /* */
 
enum LTWVendorRequest
{
	LTW_REQ_GET_PARM									= 0x0001, /* Get parameter for standard IR mode */
	LTW_REQ_SET_PARM									= 0x0002, /* Set parameter for standard IR mode */
	LTW_REQ_FLUSH											= 0x0003, /* Flush UART buffers */
	LTW_REQ_RESET											= 0x0004, /* Perform a user reset */
	LTW_REQ_GET_STAT									= 0x0005, /* Get statistics on IR receiver */
	LTW_REQ_GET_POWER									= 0x0006, /* Get current power mode */
	LTW_REQ_GET_LED										= 0x0008, /* Get LED state and color */
	LTW_REQ_SET_LED										= 0x0009, /* Set LED state and color */
	LTW_REQ_RESET_STAT								= 0x0010, /* Reset statistics on IR receiver */
	LTW_REQ_GET_PARM_IRC							= 0x0011, /* Get parameter for IRC mode */
	LTW_REQ_SET_PARM_IRC							= 0x0012, /* Set parameter for IRC mode */
	LTW_REQ_GET_TX_SPEED							= 0x00EE,	/* Get IR transmission speed */
	LTW_REQ_SET_TX_SPEED							= 0x00EF,	/* Set IR transmission speed */
	LTW_REQ_GET_RX_SPEED							= 0x00F0,	/* Get IR reception speed */
	LTW_REQ_SET_RX_SPEED							= 0x00F1,	/* Set IR reception speed */
	LTW_REQ_GET_TX_STATE							= 0x00F2, /* Get IR transmitter state */
	LTW_REQ_GET_TX_CARRIER_FREQUENCY	= 0x00F3, /* Get IR transmission frequency */
	LTW_REQ_SET_TX_CARRIER_FREQUENCY	= 0x00F4, /* Set IR transmission frequency */
	LTW_REQ_GET_TX_CARRIER_DUTY_CYCLE	= 0x00F5, /* Get IR transmission duty cycle */
	LTW_REQ_SET_TX_CARRIER_DUTY_CYCLE	= 0x00F6, /* Set IR transmission duty cycle */
	LTW_REQ_GET_CAPS									= 0x00FC, /* Get capabilities */
	LTW_REQ_GET_VERSION								= 0x00FD, /* Get version information */
	LTW_REQ_GET_COPYRIGHT							= 0x00FE, /* Get copyright information */
	LTW_REQ_GET_CREDITS								= 0x00FF  /* Get credits list */
}; /* LTWVendorRequest */

enum LTWRequestReturnCode
{
	LTW_REQERR_SUCCESS		= 0x0000, /* Request succeeded */
	LTW_REQERR_BADPARM		= 0x0001, /* Bad vendparameter and/or value */
	LTW_REQERR_BUSY				= 0x0002, /* Tower is busy */
	LTW_REQERR_NOPOWER		= 0x0003, /* Not enough power (current) */
	LTW_REQERR_WRONGMODE	= 0x0004, /* Not in the right mode */
	LTW_INTERNAL_ERROR		= 0x00FE, /* Internal error in the Tower */
	LTW_REQERR_BADREQUEST	= 0x00FF  /* Bad request */
}; /* LTWRequestReturnCode */

struct LTWRequestReply
{
	UInt16	wNoOfBytes;	/* Number of bytes in the reply */
	UInt8		bErrCode;		/* Request return code */
	UInt8		bValue;			/* Request return value */
}; /* LTWRequestReply */

// Vendor request: LTW_REQ_GET_PARM and LTW_REQ_SET_PARM

enum LTWRequestParm
{
	LTW_PARM_MODE					= 0x0001, /* Tower mode */
	LTW_PARM_RANGE				= 0x0002, /* Transmission range */
	LTW_PARM_ERRDETECT		= 0x0003,	/* Error detection on IR receiver */
	LTW_PARM_ERRSTATUS		= 0x0004, /* Current internal error code */
	LTW_PARM_ENDIAN				= 0x0097, /* Endian (word format) */
	LTW_PARM_ID_LED_MODE	= 0x0098, /* Indicator LED control mode */
	LTW_PARM_ERROR_SIGNAL	= 0x0099  /* Signal on internal error */
}; /* LTWRequestParm */

enum LTWMode
{
	LTW_MODE_VLL		= 0x0001, /* Send VLL */
	LTW_MODE_IR			= 0x0002, /* Send/Receive IR */
	LTW_MODE_IRC		= 0x0004, /* Send/Receive IRC (Technic protocol) */
	LTW_MODE_RADIO	= 0x0008  /* Send/Receive Radio signals */
}; /* LTWMode */

enum LTWRange
{
	LTW_RANGE_SHORT		= 0x0001, /* Short */
	LTW_RANGE_MEDIUM	= 0x0002, /* Medium */
	LTW_RANGE_LONG		= 0x0003  /* Long */
}; /* LTWRange */

enum LTWErrorDetect
{
	LTW_ERRDETECT_ON	= 0x0001, /* On */
	LTW_ERRDETECT_OFF	= 0x0002  /* Off */
}; /* LTWErrorDetect */

enum LTWErrorStatus
{
	LTW_NO_ERRORS								= 0, /* No error */
	LTW_NO_SUPPORT							= 1, /* No support for this function */
	LTW_ERR_VEND_REQ						= 10, /* Invalid vendor request */
	
	LTW_ERR_USB									= 100, /* Unspecified USB problem */
	LTW_ERR_USB_FUNC						= 101, /* Call to wrong USB function */
	LTW_ERR_USB_DATA						= 102, /* No or invalid data received */
	LTW_ERR_USB_ENDPOINT				= 103, /* Wrong endpoint */
	LTW_ERR_USB_PACKETSIZE			= 104, /* Invalid packet size */
	LTW_ERR_USB_TX_EP0					= 105, /* Error on sending on endpoint 0 */
	LTW_ERR_USB_EP0_PACKETSIZE	= 106, /* Wrong packet size on endpoint 0 */
	LTW_ERR_USB_EP1_PACKETSIZE	= 107, /* Wrong packet size on endpoint 1 */
	LTW_ERR_USB_VEND_BUFF_FULL	= 108, /* Vendor answer buffer full */
	LTW_ERR_USB_TX_EP1					= 109, /* Error on sending on endpoint 1 */
	
	LTW_ERR_UART								= 200, /* UART problems */
	LTW_ERR_UART_MODE						= 201, /* Invalid UART mode */
	LTW_ERR_UART_TX_STATE				= 210, /* Invalid TX state */
	LTW_ERR_UART_LOW_POWER			= 211, /* Tried to send in long range at low power */
	
	LTW_ERR_TOWER								= 240, /* Tower out of order */
	
	LTW_ERR_UNDEFINED						= 255  /* Undefined error */
}; /* LTWErrorStatus */

enum LTWEndian
{
	LTW_ENDIAN_LITTLE	= 0x0001, /* Little-endian */
	LTW_ENDIAN_BIG		= 0x0002  /* Big-endian */
}; /* LTWEndian */

enum LTWLEDMode
{
	LTW_ID_LED_HW_CTRL	= 0x0001, /* LED controlled by firmware */
	LTW_ID_LED_SW_CTRL	= 0x0002  /* LED controlled by the host */
}; /* LTWLEDMode */

enum LTWErrorSignal
{
	LTW_ID_LED_ON		= 0x0001, /* Indicator LED switched on for seriaus errors */
	LTW_ID_LED_OFF	= 0x0002  /* Indicator LED not switched on for serious errors */
}; /* LTWErrorSignal */

// Vendor request: LTW_REQ_GET_PARM_IRC and LTW_REQ_SET_PARM_IRC

enum LTWRequestParmIRC
{
	LTW_PARM_IRC_PACKETSIZE	= 0x0001, /* Packet size in bytes (default = 2) */
	LTW_PARM_IRC_TIMEFRAME	= 0x0002  /* Time frame for packet sync, ms (default: 80) */
}; /* LTWRequestParmIRC */

// Vendor request: LTW_REQ_FLUSH

enum LTWRequestFlush /* Values for the wValue field */
{
	LTW_TX_BUFFER		= 0x0001, /* Transmission buffer */
	LTW_RX_BUFFER		= 0x0002, /* Reception buffer */
	LTW_ALL_BUFFERS	= 0x0003  /* All buffers */
}; /* LTWRequestFlush */

// Vendor request: LTW_REQ_GET_STAT

struct LTWRequestStatReply
{
	UInt16	wNoOfBytes;					/* Number of bytes in the reply */
	UInt8		bErrCode;						/* Request return code */
	UInt8		bSpare;							/* Filler */
	UInt16	wNoOfRxBytes;				/* Number of received bytes */
	UInt16	wOverrunErrorCount;	/* Number of overrun errors */
	UInt16	wNoiseCount;				/* Number of bytes with wrong bits */
	UInt16	wFramingErrorCount;	/* Number of bytes with framing errors */
}; /* LTWRequestStatReply */

// Vendor request: LTW_REQ_GET_POWER

struct LTWRequestPowerReply
{
	UInt16	wNoOfBytes;	/* Number of bytes in the reply */
	UInt8		bErrCode;		/* Request return code */
	UInt8		bSpare;			/* Filler */
	UInt8		bPower;			/* Power mode */
	UInt8		bSpare2;		/* Filler */
}; /* LTWRequestPowerReply */

enum LTWPowerMode
{
	LTW_POWER_LOW		= 0x0001, /* Tower is set to low power device */
	LTW_POWER_HIGH	= 0x0002  /* Tower is set to high power device */
}; /* LTWPowerMode */

// Vendor request: LTW_REQ_GET_LED and LTW_REQ_SET_LED

enum LTWLEDModeAndColor /* Values for the wValue field */
{
	LTW_LED_ID						= 0x0001, /* ID LED */
	LTW_LED_VLL						= 0x0002, /* VLL LED */
	
	LTW_LED_COLOR_BLACK		= 0x0000, /* Black, same as 'off' */
	LTW_LED_COLOR_GREEN		= 0x0100, /* Green */
	LTW_LED_COLOR_YELLOW	= 0x0200, /* Yellow */
	LTW_LED_COLOR_ORANGE	= 0x0400, /* Orange */
	LTW_LED_COLOR_RED			= 0x0800, /* Red */
	
	LTW_LED_COLOR_DEFAULT	= 0xFF00, /* Whatever color is the default */
	
	LTW_LED_COLOR_ON			= LTW_LED_COLOR_DEFAULT,	/* Generic ON */
	LTW_LED_COLOR_OFF			= LTW_LED_COLOR_BLACK			/* Generic OFF */
}; /* LTWLEDModeAndColor */

struct LTWRequestLEDReply
{
	UInt16	wNoOfBytes;	/* Number of bytes in the reply */
	UInt8		bErrCode;		/* Request return code */
	UInt8		bSpare;			/* Filler */
	UInt8		bLEDID;			/* LTW_LED_ID or LTW_LED_VLL */
	UInt8		bColor;			/* colors from LTWLEDMode, shifted right 8 bits */
}; /* LTWRequestLEDReply */

// Vendor request: LTW_REQ_GET_CAPS

enum LTWCapabilities /* Values for the wValue field */
{
	LTW_CAPS_VLL		= LTW_MODE_VLL,
	LTW_CAPS_IR			= LTW_MODE_IR,
	LTW_CAPS_IRC		= LTW_MODE_IRC,
	LTW_CAPS_RADIO	= LTW_MODE_RADIO
}; /* LTWCapabilities */

struct LTWRequestCapsReply
{
	UInt16	wNoOfBytes;									/* Number of bytes in the reply */
	UInt8		bErrCode;										/* Request return code */
	UInt8		bSpare;											/* Filler */
	UInt8		bCommDirections;						/* Available communication directions */
	UInt8		bCommRange;									/* Communication range */
	UInt16	wCommTransmitRate;					/* Communication transmit rate */
	UInt16	wCommReceiveRate;						/* Communication receive rate */
	UInt8		bCommTransmitMinFrequency;	/* Minimum frequency (30 to 99 kHz) */
	UInt8		bCommTransmitMaxFrequency;	/* Maximum frequency (30 to 99 kHz) */
	UInt16	wCommTransmitMinDutyCycle;	/* Minimum duty cycle (units of 0.01 microseconds) */
	UInt16	wCommTransmitMaxDutyCycle;	/* Maximum duty cycle (units of 0.01 microseconds) */
	UInt8		bNoOfBytesInUartTxBuffer;		/* Number of bytes in UART transmitter buffer */
	UInt8		bNoOfBytesInUartRxBuffer;		/* Number of bytes in UART receiver buffer */
}; /* LTWRequestCapsReply */

enum LTWCapsCommunication
{
	CAPS_COMM_DIRECTION_TRANSMIT	= 0x0001, /* Transmit */
	CAPS_COMM_DIRECTION_RECEIVE		= 0x0002, /* Receive */
	CAPS_COMM_DIRECTION_BOTH			= 0x0003, /* Both transmit and receive */
	
	CAPS_COMM_RANGE_SHORT					= 0x0001, /* Short */
	CAPS_COMM_RANGE_MEDIUM				= 0x0002, /* Medium */
	CAPS_COMM_RANGE_LONG					= 0x0004, /* Long */
	CAPS_COMM_RANGE_ALL						= 0x0007, /* Short, medium and long */
	
	CAPS_COMM_BAUD_1200						= 0x0004, /* 1200 baud */
	CAPS_COMM_BAUD_2400						= 0x0008, /* 2400 baud */
	CAPS_COMM_BAUD_4800						= 0x0010, /* 4800 baud */
	CAPS_COMM_BAUD_9600						= 0x0020, /* 9600 baud */
	CAPS_COMM_BAUD_19200					= 0x0040  /* 19200 baud */
}; /* LTWCapsCommunication */

// Vendor request: LTW_REQ_GET_RX_SPEED, LTW_REQ_SET_RX_SPEED,
//									LTW_REQ_GET_TX_SPEED and LTX_REQ_SET_TX_SPEED

enum LTWRxTxSpeed
{
	SPEED_COMM_BAUD_1200	= CAPS_COMM_BAUD_1200,
	SPEED_COMM_BAUD_2400	= CAPS_COMM_BAUD_2400,
	SPEED_COMM_BAUD_4800	= CAPS_COMM_BAUD_4800,
	SPEED_COMM_BAUD_9600	= CAPS_COMM_BAUD_9600,
	SPEED_COMM_BAUD_19200	= CAPS_COMM_BAUD_19200
}; /* LTWRxTxSpeed */

struct LTWRequestSpeedReply
{
	UInt16	wNoOfBytes;	/* Number of bytes in the reply */
	UInt8		bErrCode;		/* Request return code */
	UInt8		bSpare;			/* Filler */
	UInt16	wSpeed;			/* Transmission/reception speed */
}; /* LTWRequestSpeedReply */

// Vendor request: LTW_REQ_GET_TX_STATE

struct LTWRequestTxStateReply
{
	UInt16	wNoOfBytes;	/* Number of bytes in the reply */
	UInt8		bErrCode;		/* Request return code */
	UInt8		bSpare;			/* Filler */
	UInt8		bTxState;		/* Transmitter state */
	UInt8		bSpare2;		/* Filler */
}; /* LTWRequestRxStateReply */

enum LTWTxState
{
	CAPS_TX_STATE_READY	= 0x0001, /* Transmitter buffer empty */
	CAPS_TX_STATE_BUSY	= 0x0002  /* Transmitter busy sending data */
}; /* LTWTxState */

// Vendor request: LTW_REQ_GET_TX_CARRIER_FREQUENCY and
//									LTW_REQ_SET_TX_CARRIER_FREQUENCY

struct LTWRequestTxFreqReply
{
	UInt16	wNoOfBytes;		/* Number of bytes in the reply */
	UInt8		bErrCode;			/* Request return code */
	UInt8		bSpare;				/* Filler */
	UInt8		bTxFrequency;	/* Transmitter frequency */
	UInt8		bSpare2;			/* Filler */
}; /* LTWRequestTxFreqReply */

// Vendor request: LTW_REQ_GET_TX_CARRIER_DUTY_CYCLE and
//									LTW_REQ_SET_TX_CARRIER_DUTY_CYCLE

struct LTWRequestTxDutyReply
{
	UInt16	wNoOfBytes;	/* Number of bytes in the reply */
	UInt8		bErrCode;		/* Request return code */
	UInt8		bSpare;			/* Filler */
	UInt8		bTxRange;		/* Transmitter duty cycle, in units of 0.1 microseconds */
	UInt8		bSpare2;		/* Filler */
}; /* LTWRequestTxDutyReply */

// Vendor request: LTW_REQ_GET_VERSION

struct LTWRequestVersionReply
{
	UInt16	wNoOfBytes;			/* Number of bytes in the reply */
	UInt8		bErrCode;				/* Request return code */
	UInt8		bSpare;					/* Filler */
	UInt8		bMajorVersion;	/* Major version */
	UInt8		bMinorVersion;	/* Minor version */
	UInt16	wBuildNo;				/* Build number */
}; /* LTWRequestVersionReply */

// Vendor request: LTW_REQ_GET_COPYRIGHT

struct LTWRequestCopyrightReply
{
	UInt16	wNoOfBytes;	/* Number of bytes in the reply */
	UInt8		bErrCode;		/* Request return code */
	UInt8		bSpare;			/* Filler */
	// Copyright string is return here; not NULL-terminated!
}; /* LTWRequestCopyrightReply */

#endif /* not LTW_DATA_H_ */