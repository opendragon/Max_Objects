#include "Carbon.r"
#include "QuickDraw.r"

resource 'STR#' (17120, "ldp1550") {
	{	/* array StringArray: 13 elements */
		/* [1] */
		"An interface to the Sony LDPÐ1550 Laser "
		"Disk Player.";
		/* [2] */
		"Command channel";
		/* [3] */
		"Serial device feedback";
		/* [4] */
		"Commands to serial device";
		/* [5] */
		"Poller output to serial device";
		/* [6] */
		"Key mode status output";
		/* [7] */
		"Command status output";
		/* [8] */
		"Program stop code detected";
		/* [9] */
		"Command complete output";
		/* [10] */
		"Command accepted output";
		/* [11] */
		"Chapter number output";
		/* [12] */
		"Frame number output";
		/* [13] */
		"Error detected output"
	}
};

resource 'vers' (1, "ldp1550") {
	0x1,
	0x6,
	release,
	0x0,
	0,
	"1",
	"Laser Disk Player interface"
};

resource 'vers' (2, "ldp1550") {
	0x1,
	0x6,
	release,
	0x0,
	0,
	"1",
	"©1996 by Schiphorst, Jaffe, Gregory and "
	"Gregson."
};

