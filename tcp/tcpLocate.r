#include "Carbon.r"
#include "QuickDraw.r"

resource 'STR#' (17147, "tcpLocate") {
	{	/* array StringArray: 4 elements */
		/* [1] */
		"An interface to the Open Transport TCP/I"
		"P subsystem as a mapper.";
		/* [2] */
		"Command input";
		/* [3] */
		"Status/response output";
		/* [4] */
		"Error output"
	}
};

resource 'vers' (1, "tcpLocate") {
	0x1,
	0x2,
	release,
	0x0,
	0,
	"1",
	"TCP/IP locator"
};

resource 'vers' (2, "tcpLocate") {
	0x1,
	0x2,
	release,
	0x0,
	0,
	"1",
	"�2000 by Schiphorst, Jaffe, Gregory and "
	"Gregson."
};

