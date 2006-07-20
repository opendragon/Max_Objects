#include "Carbon.r"
#include "QuickDraw.r"

resource 'STR#' (17125, "serialX") {
	{	/* array StringArray: 5 elements */
		/* [1] */
		"An interface to the serial ports with Br"
		"eak and Chunk capability.";
		/* [2] */
		"Outbound data to serial device";
		/* [3] */
		"Returned characters from serial device";
		/* [4] */
		"Break seen on input";
		/* [5] */
		"Break operation completed"
	}
};

resource 'vers' (1, "serialX") {
	0x1,
	0x14,
	release,
	0x0,
	0,
	"1",
	"Serial interface with Break and Chunk"
};

resource 'vers' (2, "serialX") {
	0x1,
	0x14,
	release,
	0x0,
	0,
	"1",
	"©1998 by Schiphorst, Jaffe, Gregory and "
	"Gregson."
};

