#include "Carbon.r"
#include "QuickDraw.r"

resource 'vers' (1, "queue") {
	0x1,
	0x2,
	release,
	0x0,
	0,
	"1",
	"First-in-first-out queue for arbitrary d"
	"ata"
};

resource 'vers' (2, "queue") {
	0x1,
	0x2,
	release,
	0x0,
	0,
	"1",
	"�2001 by Schiphorst, Jaffe, Gregory and "
	"Gregson."
};

resource 'STR#' (17178, "queue") {
	{	/* array StringArray: 5 elements */
		/* [1] */
		"A first-in-first-out queue for arbitrary"
		" data.";
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
