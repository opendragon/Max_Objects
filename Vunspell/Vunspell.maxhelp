{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 5,
			"minor" : 1,
			"revision" : 9
		}
,
		"rect" : [ 442.0, 396.0, 438.0, 322.0 ],
		"bglocked" : 0,
		"defrect" : [ 442.0, 396.0, 438.0, 322.0 ],
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
					"text" : "32",
					"fontsize" : 9.0,
					"patching_rect" : [ 112.0, 162.0, 20.0, 17.0 ],
					"id" : "obj-1",
					"numinlets" : 2,
					"fontname" : "Geneva",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "13",
					"fontsize" : 9.0,
					"patching_rect" : [ 69.0, 162.0, 20.0, 17.0 ],
					"id" : "obj-2",
					"numinlets" : 2,
					"fontname" : "Geneva",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1 2 3 blorg",
					"fontsize" : 9.0,
					"patching_rect" : [ 151.0, 101.0, 59.0, 17.0 ],
					"id" : "obj-3",
					"numinlets" : 2,
					"fontname" : "Geneva",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "49 32 50 32 51 32 52 46 53",
					"fontsize" : 9.0,
					"patching_rect" : [ 209.0, 77.0, 141.0, 17.0 ],
					"id" : "obj-4",
					"numinlets" : 2,
					"fontname" : "Geneva",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "49 32 50 32 51 32 98 108 111 114 103",
					"fontsize" : 9.0,
					"patching_rect" : [ 219.0, 101.0, 195.0, 17.0 ],
					"id" : "obj-5",
					"numinlets" : 2,
					"fontname" : "Geneva",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "blorg 1 2 3",
					"fontsize" : 9.0,
					"patching_rect" : [ 69.0, 101.0, 59.0, 17.0 ],
					"id" : "obj-6",
					"numinlets" : 2,
					"fontname" : "Geneva",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "spell",
					"fontsize" : 9.0,
					"patching_rect" : [ 7.0, 162.0, 30.0, 19.0 ],
					"id" : "obj-7",
					"numinlets" : 1,
					"fontname" : "Geneva",
					"numoutlets" : 1,
					"outlettype" : [ "int" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"patching_rect" : [ 93.0, 162.0, 15.0, 15.0 ],
					"id" : "obj-8",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "13",
					"fontsize" : 9.0,
					"patching_rect" : [ 7.0, 77.0, 20.0, 17.0 ],
					"id" : "obj-9",
					"numinlets" : 2,
					"fontname" : "Geneva",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"patching_rect" : [ 153.0, 258.0, 15.0, 15.0 ],
					"id" : "obj-11",
					"numinlets" : 1,
					"numoutlets" : 1,
					"fgcolor" : [ 1.0, 0.611765, 0.611765, 1.0 ],
					"outlettype" : [ "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "-1 2 -4 37",
					"fontsize" : 9.0,
					"patching_rect" : [ 82.0, 129.0, 55.0, 17.0 ],
					"id" : "obj-12",
					"numinlets" : 2,
					"fontname" : "Geneva",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"patching_rect" : [ 135.0, 258.0, 15.0, 15.0 ],
					"id" : "obj-13",
					"numinlets" : 1,
					"numoutlets" : 1,
					"fgcolor" : [ 0.156863, 0.8, 0.54902, 1.0 ],
					"outlettype" : [ "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "prepend set",
					"fontsize" : 9.0,
					"patching_rect" : [ 7.0, 225.0, 64.0, 19.0 ],
					"id" : "obj-14",
					"numinlets" : 1,
					"fontname" : "Geneva",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "Vunspell 13",
					"fontsize" : 9.0,
					"patching_rect" : [ 7.0, 199.0, 61.0, 19.0 ],
					"id" : "obj-15",
					"numinlets" : 1,
					"color" : [ 1.0, 0.890196, 0.090196, 1.0 ],
					"fontname" : "Geneva",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1 5 6 1 0 2 1",
					"fontsize" : 9.0,
					"patching_rect" : [ 59.0, 77.0, 71.0, 17.0 ],
					"id" : "obj-16",
					"numinlets" : 2,
					"fontname" : "Geneva",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1 2 3 4.5",
					"fontsize" : 9.0,
					"patching_rect" : [ 140.0, 77.0, 50.0, 17.0 ],
					"id" : "obj-17",
					"numinlets" : 2,
					"fontname" : "Geneva",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Vunspell",
					"fontsize" : 14.0,
					"patching_rect" : [ 1.0, 25.0, 86.0, 25.0 ],
					"id" : "obj-18",
					"numinlets" : 1,
					"fontname" : "Geneva",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Reconstruct  atoms from a 'spelled' list.",
					"fontsize" : 9.0,
					"patching_rect" : [ 1.0, 47.0, 186.0, 19.0 ],
					"id" : "obj-19",
					"numinlets" : 1,
					"fontname" : "Geneva",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Argument: list;\rInlet: list;\rOutlet: list",
					"linecount" : 3,
					"fontsize" : 9.0,
					"patching_rect" : [ 188.0, 24.0, 100.0, 43.0 ],
					"id" : "obj-20",
					"numinlets" : 1,
					"fontname" : "Geneva",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "blorg 1 2 3 blorg 1 2 3",
					"fontsize" : 9.0,
					"patching_rect" : [ 7.0, 258.0, 125.0, 17.0 ],
					"id" : "obj-21",
					"numinlets" : 2,
					"fontname" : "Geneva",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "a.b",
					"fontsize" : 9.0,
					"patching_rect" : [ 29.0, 77.0, 27.0, 17.0 ],
					"id" : "obj-22",
					"numinlets" : 2,
					"fontname" : "Geneva",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Note that 'spell' doesn't properly process lists with floating point numbers or embedded symbols;\rthe lists above simulate what 'spell' should do with these.",
					"linecount" : 5,
					"fontsize" : 9.0,
					"patching_rect" : [ 232.0, 125.0, 177.0, 67.0 ],
					"id" : "obj-23",
					"numinlets" : 1,
					"fontname" : "Geneva",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Send 32 between lists and 13 or bang to terminate a list.",
					"linecount" : 3,
					"fontsize" : 9.0,
					"patching_rect" : [ 168.0, 196.0, 100.0, 43.0 ],
					"id" : "obj-24",
					"numinlets" : 1,
					"fontname" : "Geneva",
					"numoutlets" : 0
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-15", 1 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 0,
					"midpoints" : [ 58.5, 220.0, 162.0, 220.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 0 ],
					"destination" : [ "obj-13", 0 ],
					"hidden" : 0,
					"midpoints" : [ 16.5, 248.0, 144.0, 248.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 0 ],
					"destination" : [ "obj-21", 0 ],
					"hidden" : 0,
					"midpoints" : [ 16.5, 250.0, 16.5, 250.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 0,
					"midpoints" : [ 16.5, 221.0, 16.5, 221.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"midpoints" : [ 102.0, 190.0, 16.5, 190.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"midpoints" : [ 228.5, 190.0, 16.5, 190.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-4", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"midpoints" : [ 218.5, 190.0, 16.5, 190.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-7", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"midpoints" : [ 16.5, 190.0, 16.5, 190.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"midpoints" : [ 121.5, 190.0, 16.5, 190.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"midpoints" : [ 78.5, 190.0, 16.5, 190.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 0,
					"midpoints" : [ 68.5, 152.0, 16.5, 152.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 0,
					"midpoints" : [ 149.5, 152.0, 16.5, 152.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-12", 0 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 0,
					"midpoints" : [ 91.5, 152.0, 16.5, 152.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-9", 0 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 0,
					"midpoints" : [ 16.5, 152.0, 16.5, 152.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 0,
					"midpoints" : [ 78.5, 152.0, 16.5, 152.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 0,
					"midpoints" : [ 38.5, 152.0, 16.5, 152.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 0,
					"midpoints" : [ 160.5, 152.0, 16.5, 152.0 ]
				}

			}
 ]
	}

}
