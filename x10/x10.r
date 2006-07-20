#include "Carbon.r"
#include "QuickDraw.r"

resource 'STR#' (17121, "x10") {
	{	/* array StringArray: 12 elements */
		/* [1] */
		"An interface to the X�10 Home Control sy"
		"stem.";
		/* [2] */
		"Command channel";
		/* [3] */
		"Serial device feedback";
		/* [4] */
		"Commands to serial device";
		/* [5] */
		"Poller output to serial device";
		/* [6] */
		"Base housecode output";
		/* [7] */
		"Command complete output";
		/* [8] */
		"Device status output";
		/* [9] */
		"Day number output";
		/* [10] */
		"Hour number output";
		/* [11] */
		"Minute number output";
		/* [12] */
		"Error detected output"
	}
};

resource 'vers' (1, "x10") {
	0x1,
	0x8,
	release,
	0x0,
	0,
	"1",
	"X10 Home Control interface"
};

resource 'vers' (2, "x10") {
	0x1,
	0x8,
	release,
	0x0,
	0,
	"1",
	"�1996 by Schiphorst, Jaffe, Gregory and "
	"Gregson."
};
