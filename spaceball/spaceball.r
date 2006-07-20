#include "Carbon.r"
#include "QuickDraw.r"

resource 'STR#' (17161, "spaceball") {
	{	/* array StringArray: 8 elements */
		/* [1] */
		"An interface to the Labtec Spaceball 6DO"
		"F input device.";
		/* [2] */
		"Command channel";
		/* [3] */
		"Serial device feedback";
		/* [4] */
		"Data out";
		/* [5] */
		"Sample request to serial device";
		/* [6] */
		"Data to serial device";
		/* [7] */
		"Chunk request to serial device";
		/* [8] */
		"Error detected"
	}
};

resource 'vers' (1, "spaceball") {
	0x1,
	0x3,
	release,
	0x0,
	0,
	"1",
	"Labtec Spaceball interface"
};

resource 'vers' (2, "spaceball") {
	0x1,
	0x3,
	release,
	0x0,
	0,
	"1",
	"©2001 by Schiphorst, Jaffe, Gregory and "
	"Gregson."
};

