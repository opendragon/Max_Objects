#include "Carbon.r"
#include "QuickDraw.r"

resource 'vers' (1, "map2d") {
	0x1,
	0x6,
	release,
	0x0,
	0,
	"1",
	"Map coordinate pair to value"
};

resource 'vers' (2, "map2d") {
	0x1,
	0x6,
	release,
	0x0,
	0,
	"1",
	"�2000 by Schiphorst, Jaffe, Gregory and "
	"Gregson."
};

resource 'STR#' (17154, "map2d") {
	{	/* array StringArray: 3 elements */
		/* [1] */
		"Locate a coordinate pair and return it�s"
		" corresponding value.";
		/* [2] */
		"Command/data input";
		/* [3] */
		"Match result output"
	}
};
