{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 5,
			"minor" : 1,
			"revision" : 9
		}
,
		"rect" : [ 64.0, 328.0, 359.0, 289.0 ],
		"bglocked" : 0,
		"defrect" : [ 64.0, 328.0, 359.0, 289.0 ],
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
					"text" : "www.cycling74.com",
					"patching_rect" : [ 100.0, 63.0, 100.0, 17.0 ],
					"id" : "obj-1",
					"fontname" : "Geneva",
					"outlettype" : [ "" ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "www.apple.com",
					"patching_rect" : [ 18.0, 63.0, 79.0, 17.0 ],
					"id" : "obj-2",
					"fontname" : "Geneva",
					"outlettype" : [ "" ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "set",
					"patching_rect" : [ 207.0, 98.0, 22.0, 17.0 ],
					"id" : "obj-3",
					"fontname" : "Geneva",
					"outlettype" : [ "" ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"hidden" : 1,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadbang",
					"patching_rect" : [ 207.0, 63.0, 52.0, 19.0 ],
					"id" : "obj-4",
					"fontname" : "Geneva",
					"outlettype" : [ "bang" ],
					"color" : [ 1.0, 0.360784, 0.682353, 1.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1,
					"hidden" : 1,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"patching_rect" : [ 18.0, 215.0, 85.0, 17.0 ],
					"id" : "obj-5",
					"fontname" : "Geneva",
					"outlettype" : [ "" ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "prepend set",
					"patching_rect" : [ 18.0, 187.0, 63.0, 19.0 ],
					"id" : "obj-6",
					"fontname" : "Geneva",
					"outlettype" : [ "" ],
					"fontsize" : 9.0,
					"numinlets" : 1,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "tcpLocate",
					"patching_rect" : [ 10.0, 22.0, 86.0, 25.0 ],
					"id" : "obj-7",
					"fontname" : "Geneva",
					"fontsize" : 14.0,
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Get the IP address corresponding to a name",
					"patching_rect" : [ 9.0, 43.0, 221.0, 19.0 ],
					"id" : "obj-8",
					"fontname" : "Geneva",
					"fontsize" : 9.0,
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"patching_rect" : [ 121.0, 202.0, 27.0, 27.0 ],
					"id" : "obj-9",
					"outlettype" : [ "bang" ],
					"fgcolor" : [ 0.156863, 0.8, 0.54902, 1.0 ],
					"numinlets" : 1,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "tcpLocate",
					"patching_rect" : [ 18.0, 148.0, 59.0, 19.0 ],
					"id" : "obj-10",
					"fontname" : "Geneva",
					"outlettype" : [ "", "bang" ],
					"color" : [ 1.0, 0.890196, 0.090196, 1.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1,
					"numoutlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "error detected",
					"patching_rect" : [ 121.0, 233.0, 75.0, 19.0 ],
					"id" : "obj-11",
					"fontname" : "Geneva",
					"fontsize" : 9.0,
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "received message",
					"linecount" : 2,
					"patching_rect" : [ 19.0, 233.0, 85.0, 31.0 ],
					"id" : "obj-12",
					"fontname" : "Geneva",
					"fontsize" : 9.0,
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "tcpLocate returns the IP address corresponding to a name, as a symbol",
					"linecount" : 3,
					"patching_rect" : [ 217.0, 136.0, 127.0, 43.0 ],
					"id" : "obj-13",
					"fontname" : "Geneva",
					"fontsize" : 9.0,
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-4", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 1,
					"midpoints" : [ 216.5, 89.0, 216.0, 89.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 1 ],
					"destination" : [ "obj-9", 0 ],
					"hidden" : 0,
					"midpoints" : [ 67.5, 180.0, 130.5, 180.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 0,
					"midpoints" : [ 27.5, 211.0, 27.5, 211.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 1,
					"midpoints" : [ 216.5, 209.0, 27.5, 209.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 0 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 0,
					"midpoints" : [ 27.5, 169.0, 27.5, 169.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-10", 0 ],
					"hidden" : 0,
					"midpoints" : [ 109.5, 113.0, 27.5, 113.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-10", 0 ],
					"hidden" : 0,
					"midpoints" : [ 27.5, 113.0, 27.5, 113.0 ]
				}

			}
 ]
	}

}
