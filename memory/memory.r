#include "Carbon.r"
#include "QuickDraw.r"

resource 'vers' (1, "memory") {
	0x1,
	0x2,
	release,
	0x0,
	0,
	"1",
	"Remember arbitrary data."
};

resource 'vers' (2, "memory") {
	0x1,
	0x2,
	release,
	0x0,
	0,
	"1",
	"©2000 by Schiphorst, Jaffe, Gregory and "
	"Gregson."
};

resource 'STR#' (17137, "memory") {
	{	/* array StringArray: 4 elements */
		/* [1] */
		"Remember arbitrary data.";
		/* [2] */
		"Command input";
		/* [3] */
		"Data output";
		/* [4] */
		"Error bang output"
	}
};

