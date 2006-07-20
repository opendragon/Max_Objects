#include "Carbon.r"
#include "QuickDraw.r"

resource 'STR#' (17157, "speak") {
	{	/* array StringArray: 6 elements */
		/* [1] */
		"Generates spoken output.";
		/* [2] */
		"Command input";
		/* [3] */
		"Speech done output";
		/* [4] */
		"Paused status output";
		/* [5] */
		"Numeric output";
		/* [6] */
		"Error detected output"
	}
};

resource 'vers' (1, "speak") {
	0x1,
	0x4,
	release,
	0x0,
	0,
	"1",
	"Generates spoken output"
};

resource 'vers' (2, "speak") {
	0x1,
	0x4,
	release,
	0x0,
	0,
	"1",
	"©2001 by Schiphorst, Jaffe, Gregory and "
	"Gregson."
};

