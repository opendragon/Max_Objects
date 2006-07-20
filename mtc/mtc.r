#include "Carbon.r"
#include "QuickDraw.r"

resource 'STR#' (17135, "mtc") {
	{	/* array StringArray: 10 elements */
		/* [1] */
		"An interface to the Tactex MultiŠTouch C"
		"ontroller.";
		/* [2] */
		"Command channel";
		/* [3] */
		"Serial device feedback";
		/* [4] */
		"Data out";
		/* [5] */
		"Data start/stop";
		/* [6] */
		"Command complete";
		/* [7] */
		"Sample request to serial device";
		/* [8] */
		"Data to serial device";
		/* [9] */
		"Chunk request to serial device";
		/* [10] */
		"Error detected"
	}
};

resource 'vers' (1, "mtc") {
	0x1,
	0x7,
	release,
	0x0,
	0,
	"1",
	"Multi-Touch Controller interface"
};

resource 'vers' (2, "mtc") {
	0x1,
	0x7,
	release,
	0x0,
	0,
	"1",
	"©1999 by Schiphorst, Jaffe, Gregory and "
	"Gregson."
};

