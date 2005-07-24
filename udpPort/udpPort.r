#include "Carbon.r"
#include "QuickDraw.r"

resource 'STR#' (17181, "udpPort") {
	{	/* array StringArray: 4 elements */
		/* [1] */
		"An interface to the Open Transport UDP"
		" subsystem as a port.";
		/* [2] */
		"Command input";
		/* [3] */
		"Status/response output";
		/* [4] */
		"Error output"
	}
};

resource 'vers' (1, "udpPort") {
	0x1,
	0x00,
	release,
	0x0,
	0,
	"1",
	"UDP port"
};

resource 'vers' (2, "udpPort") {
	0x1,
	0x00,
	release,
	0x0,
	0,
	"1",
	"©2005 by Schiphorst, Jaffe, Gregory and "
	"Gregson."
};

