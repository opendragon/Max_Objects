#include "Carbon.r"
#include "QuickDraw.r"

resource 'STR#' (17127, "tcpClient") {
	{	/* array StringArray: 4 elements */
		/* [1] */
		"An interface to the Open Transport TCP/I"
		"P subsystem as a client.";
		/* [2] */
		"Command input";
		/* [3] */
		"Status/response output";
		/* [4] */
		"Error output"
	}
};

resource 'vers' (1, "tcpClient") {
	0x1,
	0x16,
	release,
	0x0,
	0,
	"1",
	"TCP/IP client"
};

resource 'vers' (2, "tcpClient") {
	0x1,
	0x16,
	release,
	0x0,
	0,
	"1",
	"©1998 by Schiphorst, Jaffe, Gregory and "
	"Gregson."
};

