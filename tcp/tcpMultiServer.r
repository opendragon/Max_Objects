#include "Carbon.r"
#include "QuickDraw.r"

resource 'STR#' (17144, "tcpMultiServer") {
	{	/* array StringArray: 4 elements */
		/* [1] */
		"An interface to the Open Transport TCP/I"
		"P subsystem as a server that handles mul"
		"tiple clients.";
		/* [2] */
		"Command input";
		/* [3] */
		"Status/response output";
		/* [4] */
		"Error output"
	}
};

resource 'vers' (1, "tcpMultiServer") {
	0x1,
	0x10,
	release,
	0x0,
	0,
	"1",
	"TCP/IP multiple-client server"
};

resource 'vers' (2, "tcpMultiServer") {
	0x1,
	0x10,
	release,
	0x0,
	0,
	"1",
	"©2000 by Schiphorst, Jaffe, Gregory and "
	"Gregson."
};

