#include "Carbon.r"
#include "QuickDraw.r"

resource 'vers' (1, "map1d") {
	0x1,
	0x6,
	release,
	0x0,
	0,
	"1",
	"Map coordinate to value"
};

resource 'vers' (2, "map1d") {
	0x1,
	0x6,
	release,
	0x0,
	0,
	"1",
	"©2000 by Schiphorst, Jaffe, Gregory and "
	"Gregson."
};

resource 'STR#' (17153, "map1d") {
	{	/* array StringArray: 3 elements */
		/* [1] */
		"Locate a coordinate and return it‘s corr"
		"esponding value.";
		/* [2] */
		"Command/data input";
		/* [3] */
		"Match result output"
	}
};

