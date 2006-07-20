#include "Carbon.r"
#include "QuickDraw.r"

resource 'STR#' (17191, "rcx") {
	{	/* array StringArray: 5 elements */
		/* [1] */
		"An interface to the LEGO Mindstorms robo"
		"tic systems.";
		/* [2] */
		"Command channel";
		/* [3] */
		"Data out";
		/* [4] */
		"Command complete";
		/* [5] */
		"Error detected"
	}
};

resource 'vers' (1, "rcx") {
	0x1,
	0x5,
	release,
	0x0,
	0,
	"1",
	"LEGO Mindstorms interface"
};

resource 'vers' (2, "rcx") {
	0x1,
	0x5,
	release,
	0x0,
	0,
	"1",
	"�2002 by Schiphorst, Jaffe, Gregory and "
	"Gregson."
};
