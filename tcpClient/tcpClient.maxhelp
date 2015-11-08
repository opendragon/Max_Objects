{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 5,
			"minor" : 1,
			"revision" : 9
		}
,
		"rect" : [ 390.0, 101.0, 446.0, 363.0 ],
		"bglocked" : 0,
		"defrect" : [ 390.0, 101.0, 446.0, 363.0 ],
		"openrect" : [ 0.0, 0.0, 0.0, 0.0 ],
		"openinpresentation" : 0,
		"default_fontsize" : 12.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 0,
		"gridsize" : [ 15.0, 15.0 ],
		"gridsnaponopen" : 0,
		"toolbarvisible" : 1,
		"boxanimatetime" : 200,
		"imprint" : 0,
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"boxes" : [ 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "self",
					"outlettype" : [ "" ],
					"fontsize" : 9.0,
					"patching_rect" : [ 247.0, 157.0, 25.0, 17.0 ],
					"id" : "obj-1",
					"numinlets" : 2,
					"fontname" : "Geneva",
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "set",
					"outlettype" : [ "" ],
					"fontsize" : 9.0,
					"patching_rect" : [ 373.0, 120.0, 22.0, 17.0 ],
					"id" : "obj-2",
					"numinlets" : 2,
					"fontname" : "Geneva",
					"hidden" : 1,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadbang",
					"outlettype" : [ "bang" ],
					"fontsize" : 9.0,
					"patching_rect" : [ 373.0, 89.0, 51.0, 19.0 ],
					"color" : [ 1.0, 0.360784, 0.682353, 1.0 ],
					"id" : "obj-3",
					"numinlets" : 1,
					"fontname" : "Geneva",
					"hidden" : 1,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "status",
					"outlettype" : [ "" ],
					"fontsize" : 9.0,
					"patching_rect" : [ 30.0, 157.0, 43.0, 17.0 ],
					"id" : "obj-4",
					"numinlets" : 2,
					"fontname" : "Geneva",
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "self 192.168.17.12",
					"linecount" : 3,
					"outlettype" : [ "" ],
					"fontsize" : 9.0,
					"patching_rect" : [ 18.0, 268.0, 50.0, 41.0 ],
					"id" : "obj-5",
					"numinlets" : 2,
					"fontname" : "Geneva",
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "prepend set",
					"outlettype" : [ "" ],
					"fontsize" : 9.0,
					"patching_rect" : [ 18.0, 240.0, 66.0, 19.0 ],
					"id" : "obj-6",
					"numinlets" : 1,
					"fontname" : "Geneva",
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "tcpClient",
					"fontsize" : 14.0,
					"patching_rect" : [ 10.0, 22.0, 75.0, 25.0 ],
					"id" : "obj-7",
					"numinlets" : 1,
					"fontname" : "Geneva",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Get and send messages via a TCP/IP connection",
					"fontsize" : 9.0,
					"patching_rect" : [ 9.0, 43.0, 221.0, 19.0 ],
					"id" : "obj-8",
					"numinlets" : 1,
					"fontname" : "Geneva",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Arguments are: 1. server IP address in dotted notation;\r2. server port number;\r3. number of receive buffers to use (default = 25)",
					"linecount" : 5,
					"fontsize" : 9.0,
					"patching_rect" : [ 215.0, 193.0, 135.0, 67.0 ],
					"id" : "obj-9",
					"numinlets" : 1,
					"fontname" : "Geneva",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "server 10.0.0.4",
					"outlettype" : [ "" ],
					"fontsize" : 9.0,
					"patching_rect" : [ 113.0, 95.0, 81.0, 17.0 ],
					"id" : "obj-10",
					"numinlets" : 2,
					"fontname" : "Geneva",
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 71.0, 157.0, 15.0, 15.0 ],
					"id" : "obj-11",
					"numinlets" : 1,
					"fgcolor" : [ 0.156863, 0.8, 0.54902, 1.0 ],
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "95.5",
					"outlettype" : [ "" ],
					"fontsize" : 9.0,
					"patching_rect" : [ 215.0, 64.0, 29.0, 17.0 ],
					"id" : "obj-12",
					"numinlets" : 2,
					"fontname" : "Geneva",
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 123.0, 250.0, 27.0, 27.0 ],
					"id" : "obj-13",
					"numinlets" : 1,
					"fgcolor" : [ 0.156863, 0.8, 0.54902, 1.0 ],
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "send alas poor yorick",
					"outlettype" : [ "" ],
					"fontsize" : 9.0,
					"patching_rect" : [ 107.0, 64.0, 104.0, 17.0 ],
					"id" : "obj-14",
					"numinlets" : 2,
					"fontname" : "Geneva",
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "55",
					"outlettype" : [ "" ],
					"fontsize" : 9.0,
					"patching_rect" : [ 84.0, 64.0, 20.0, 17.0 ],
					"id" : "obj-15",
					"numinlets" : 2,
					"fontname" : "Geneva",
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "server 1.2.3.4",
					"outlettype" : [ "" ],
					"fontsize" : 9.0,
					"patching_rect" : [ 33.0, 95.0, 75.0, 17.0 ],
					"id" : "obj-16",
					"numinlets" : 2,
					"fontname" : "Geneva",
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "disconnect",
					"outlettype" : [ "" ],
					"fontsize" : 9.0,
					"patching_rect" : [ 78.0, 125.0, 56.0, 17.0 ],
					"id" : "obj-17",
					"numinlets" : 2,
					"fontname" : "Geneva",
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tcpClient 10.0.0.2 8192",
					"outlettype" : [ "", "bang" ],
					"fontsize" : 9.0,
					"patching_rect" : [ 18.0, 201.0, 116.0, 19.0 ],
					"color" : [ 1.0, 0.890196, 0.090196, 1.0 ],
					"id" : "obj-18",
					"numinlets" : 1,
					"fontname" : "Geneva",
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "connect",
					"outlettype" : [ "" ],
					"fontsize" : 9.0,
					"patching_rect" : [ 31.0, 125.0, 47.0, 17.0 ],
					"id" : "obj-19",
					"numinlets" : 2,
					"fontname" : "Geneva",
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "port 40000",
					"outlettype" : [ "" ],
					"fontsize" : 9.0,
					"patching_rect" : [ 136.0, 125.0, 60.0, 17.0 ],
					"id" : "obj-20",
					"numinlets" : 2,
					"fontname" : "Geneva",
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "57 12 13",
					"outlettype" : [ "" ],
					"fontsize" : 9.0,
					"patching_rect" : [ 31.0, 64.0, 50.0, 17.0 ],
					"id" : "obj-21",
					"numinlets" : 2,
					"fontname" : "Geneva",
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "error detected",
					"fontsize" : 9.0,
					"patching_rect" : [ 133.0, 223.0, 75.0, 19.0 ],
					"id" : "obj-22",
					"numinlets" : 1,
					"fontname" : "Geneva",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "received message",
					"linecount" : 2,
					"fontsize" : 9.0,
					"patching_rect" : [ 29.0, 223.0, 85.0, 31.0 ],
					"id" : "obj-23",
					"numinlets" : 1,
					"fontname" : "Geneva",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "tcpClient returns either a status message (a list beginning with 'status') or a data message (a list beginning with 'reply')",
					"linecount" : 3,
					"fontsize" : 9.0,
					"patching_rect" : [ 180.0, 270.0, 191.0, 43.0 ],
					"id" : "obj-24",
					"numinlets" : 1,
					"fontname" : "Geneva",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "bang and 'status' send a 'status' message out the outlet.",
					"linecount" : 2,
					"fontsize" : 9.0,
					"patching_rect" : [ 94.0, 157.0, 151.0, 31.0 ],
					"id" : "obj-25",
					"numinlets" : 1,
					"fontname" : "Geneva",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "'port' changes the server port number",
					"linecount" : 2,
					"fontsize" : 9.0,
					"patching_rect" : [ 199.0, 124.0, 99.0, 31.0 ],
					"id" : "obj-26",
					"numinlets" : 1,
					"fontname" : "Geneva",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "'server' changes the server IP address",
					"linecount" : 2,
					"fontsize" : 9.0,
					"patching_rect" : [ 197.0, 94.0, 100.0, 31.0 ],
					"id" : "obj-27",
					"numinlets" : 1,
					"fontname" : "Geneva",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "numbers, lists, and text messages can be sent",
					"linecount" : 2,
					"fontsize" : 9.0,
					"patching_rect" : [ 247.0, 63.0, 119.0, 31.0 ],
					"id" : "obj-28",
					"numinlets" : 1,
					"fontname" : "Geneva",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "'self' returns the IP address of the client",
					"linecount" : 2,
					"fontsize" : 9.0,
					"patching_rect" : [ 275.0, 157.0, 100.0, 31.0 ],
					"id" : "obj-30",
					"numinlets" : 1,
					"fontname" : "Geneva",
					"numoutlets" : 0
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 1,
					"midpoints" : [ 382.5, 115.0, 382.0, 115.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 1 ],
					"destination" : [ "obj-13", 0 ],
					"hidden" : 0,
					"midpoints" : [ 124.5, 233.0, 132.5, 233.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 0,
					"midpoints" : [ 27.5, 264.0, 27.5, 264.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 1,
					"midpoints" : [ 382.5, 262.0, 27.5, 262.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 0,
					"midpoints" : [ 27.5, 222.0, 27.5, 222.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [ 256.5, 186.0, 27.5, 186.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-4", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [ 39.5, 186.0, 27.5, 186.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [ 80.0, 186.0, 27.5, 186.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-21", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [ 40.5, 88.0, 27.5, 88.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-20", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [ 145.5, 151.0, 27.5, 151.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [ 40.5, 151.0, 27.5, 151.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [ 87.5, 151.0, 27.5, 151.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [ 42.5, 116.0, 27.5, 116.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [ 93.5, 88.0, 27.5, 88.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [ 116.5, 88.0, 27.5, 88.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-12", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [ 224.5, 88.0, 27.5, 88.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [ 122.5, 116.0, 27.5, 116.0 ]
				}

			}
 ]
	}

}
