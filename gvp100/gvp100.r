#include "Carbon.r"
#include "QuickDraw.r"

resource 'STR#' (17124, "gvp100") {
	{	/* array StringArray: 9 elements */
		/* [1] */
		"An interface to video switchers that use"
		" the Grass Valley Protocol.";
		/* [2] */
		"Command channel";
		/* [3] */
		"Serial device feedback";
		/* [4] */
		"Sequence complete";
		/* [5] */
		"Command complete";
		/* [6] */
		"Sample request to serial device";
		/* [7] */
		"Data to serial device";
		/* [8] */
		"Break request to serial device";
		/* [9] */
		"Error detected"
	}
};

resource 'vers' (1, "gvp100") {
	0x1,
	0x5,
	release,
	0x0,
	0,
	"1",
	"Grass Valley Protocol interface"
};

resource 'vers' (2, "gvp100") {
	0x1,
	0x5,
	release,
	0x0,
	0,
	"1",
	"©1998 by Schiphorst, Jaffe, Gregory and "
	"Gregson."
};

