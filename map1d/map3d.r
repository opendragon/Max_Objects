#include "Carbon.r"
#include "QuickDraw.r"

resource 'vers' (1, "map3d") {
	0x1,
	0x6,
	release,
	0x0,
	0,
	"1",
	"Map coordinate triple to value"
};

resource 'vers' (2, "map3d") {
	0x1,
	0x6,
	release,
	0x0,
	0,
	"1",
	"�2001 by Schiphorst, Jaffe, Gregory and "
	"Gregson."
};

resource 'STR#' (17164, "map3d") {
	{	/* array StringArray: 3 elements */
		/* [1] */
		"Locate a coordinate triple and return it"
		"�s corresponding value.";
		/* [2] */
		"Command/data input";
		/* [3] */
		"Match result output"
	}
};
