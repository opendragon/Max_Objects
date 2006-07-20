#include "Carbon.r"
#include "QuickDraw.r"

resource 'vers' (1, "stack") {
	0x1,
	0x2,
	release,
	0x0,
	0,
	"1",
	"Push-down stack for arbitrary data"
};

resource 'vers' (2, "stack") {
	0x1,
	0x2,
	release,
	0x0,
	0,
	"1",
	"©2000 by Schiphorst, Jaffe, Gregory and "
	"Gregson."
};

resource 'STR#' (17138, "stack") {
	{	/* array StringArray: 5 elements */
		/* [1] */
		"A push–down stack for arbitrary data.";
		/* [2] */
		"Command input";
		/* [3] */
		"Data output";
		/* [4] */
		"Depth output";
		/* [5] */
		"Error bang output"
	}
};

