#include "Carbon.r"
#include "QuickDraw.r"

resource 'vers' (1, "pfsm") {
	0x1,
	0x4,
	release,
	0x0,
	0,
	"1",
	"Probabilisitic Finite State Machine"
};

resource 'vers' (2, "pfsm") {
	0x1,
	0x4,
	release,
	0x0,
	0,
	"1",
	"©2000 by Schiphorst, Jaffe, Gregory and "
	"Gregson."
};

resource 'STR#' (17136, "pfsm") {
	{	/* array StringArray: 5 elements */
		/* [1] */
		"A Probabilistic Finite State Machine.";
		/* [2] */
		"Command input";
		/* [3] */
		"Data output";
		/* [4] */
		"Stop bang output";
		/* [5] */
		"Error bang output"
	}
};

