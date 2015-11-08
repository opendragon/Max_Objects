{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 5,
			"minor" : 1,
			"revision" : 9
		}
,
		"rect" : [ 505.0, 229.0, 389.0, 317.0 ],
		"bglocked" : 0,
		"defrect" : [ 505.0, 229.0, 389.0, 317.0 ],
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
					"maxclass" : "button",
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 104.0, 207.0, 15.0, 15.0 ],
					"id" : "obj-1",
					"fgcolor" : [ 0.156863, 0.8, 0.54902, 1.0 ],
					"numinlets" : 1,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1 2 3 4.5",
					"outlettype" : [ "" ],
					"patching_rect" : [ 14.0, 207.0, 88.0, 17.0 ],
					"id" : "obj-2",
					"fontname" : "Geneva",
					"fontsize" : 9.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "prepend set",
					"outlettype" : [ "" ],
					"patching_rect" : [ 14.0, 174.0, 70.0, 19.0 ],
					"id" : "obj-3",
					"fontname" : "Geneva",
					"fontsize" : 9.0,
					"numinlets" : 1,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "changes 0 -1",
					"outlettype" : [ "" ],
					"patching_rect" : [ 14.0, 148.0, 77.0, 19.0 ],
					"id" : "obj-4",
					"fontname" : "Geneva",
					"color" : [ 1.0, 0.890196, 0.090196, 1.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "blah blah blah",
					"outlettype" : [ "" ],
					"patching_rect" : [ 14.0, 59.0, 68.0, 17.0 ],
					"id" : "obj-5",
					"fontname" : "Geneva",
					"fontsize" : 9.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "bluh blah blah",
					"outlettype" : [ "" ],
					"patching_rect" : [ 14.0, 92.0, 68.0, 17.0 ],
					"id" : "obj-6",
					"fontname" : "Geneva",
					"fontsize" : 9.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1 2 3 4",
					"outlettype" : [ "" ],
					"patching_rect" : [ 96.0, 59.0, 41.0, 17.0 ],
					"id" : "obj-7",
					"fontname" : "Geneva",
					"fontsize" : 9.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1 2 3 blorg",
					"outlettype" : [ "" ],
					"patching_rect" : [ 96.0, 92.0, 58.0, 17.0 ],
					"id" : "obj-8",
					"fontname" : "Geneva",
					"fontsize" : 9.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1 2 3 4.5",
					"outlettype" : [ "" ],
					"patching_rect" : [ 96.0, 120.0, 50.0, 17.0 ],
					"id" : "obj-9",
					"fontname" : "Geneva",
					"fontsize" : 9.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "changes",
					"patching_rect" : [ 11.0, 24.0, 68.0, 25.0 ],
					"id" : "obj-10",
					"fontname" : "Geneva",
					"fontsize" : 14.0,
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Filter out repetitions for specific list elements",
					"patching_rect" : [ 11.0, 44.0, 218.0, 19.0 ],
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
					"text" : "Argument: int(s);\rInlet: list;\rOutlet: list",
					"linecount" : 3,
					"patching_rect" : [ 234.0, 21.0, 91.0, 43.0 ],
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
					"text" : "The inlet takes an arbitrary list and outputs an arbitrary list while filtering out repetitions. Thus inputting the same list more than once successively results in only one output value.",
					"linecount" : 3,
					"patching_rect" : [ 10.0, 233.0, 340.0, 43.0 ],
					"id" : "obj-13",
					"fontname" : "Geneva",
					"fontsize" : 9.0,
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Arguments specify which list elements are to be monitored for a change. The elements are numbered, starting from zero on the left and -1 on the right. If any of the specified elements change, the input list is sent to the output.",
					"linecount" : 5,
					"patching_rect" : [ 117.0, 140.0, 237.0, 67.0 ],
					"id" : "obj-14",
					"fontname" : "Geneva",
					"fontsize" : 9.0,
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [ 23.5, 197.0, 113.0, 197.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [ 23.5, 199.0, 23.5, 199.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-4", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [ 23.5, 170.0, 23.5, 170.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-9", 0 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 0,
					"midpoints" : [ 105.5, 143.0, 23.5, 143.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 0,
					"midpoints" : [ 105.5, 114.0, 88.0, 114.0, 88.0, 143.0, 23.5, 143.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-7", 0 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 0,
					"midpoints" : [ 105.5, 83.0, 88.0, 83.0, 88.0, 143.0, 23.5, 143.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 0,
					"midpoints" : [ 23.5, 143.0, 23.5, 143.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 0,
					"midpoints" : [ 23.5, 83.0, 6.0, 83.0, 6.0, 143.0, 23.5, 143.0 ]
				}

			}
 ]
	}

}
