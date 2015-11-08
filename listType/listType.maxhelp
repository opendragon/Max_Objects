{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 5,
			"minor" : 1,
			"revision" : 9
		}
,
		"rect" : [ 475.0, 475.0, 451.0, 318.0 ],
		"bglocked" : 0,
		"defrect" : [ 475.0, 475.0, 451.0, 318.0 ],
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
					"text" : "17 12.5",
					"id" : "obj-1",
					"outlettype" : [ "" ],
					"fontname" : "Geneva",
					"patching_rect" : [ 382.0, 76.0, 44.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1.2 2.3",
					"id" : "obj-2",
					"outlettype" : [ "" ],
					"fontname" : "Geneva",
					"patching_rect" : [ 340.0, 76.0, 41.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "list",
					"id" : "obj-3",
					"outlettype" : [ "" ],
					"fontname" : "Geneva",
					"patching_rect" : [ 316.0, 76.0, 23.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "listType",
					"id" : "obj-4",
					"fontname" : "Geneva",
					"patching_rect" : [ 7.0, 24.0, 80.0, 25.0 ],
					"fontsize" : 14.0,
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Report the type of a given value",
					"id" : "obj-5",
					"fontname" : "Geneva",
					"patching_rect" : [ 7.0, 44.0, 218.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Argument: none;\rInlet: anything;\rOutlet: int",
					"linecount" : 3,
					"id" : "obj-6",
					"fontname" : "Geneva",
					"patching_rect" : [ 248.0, 21.0, 91.0, 43.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "The inlet takes an arbitrary input and outputs a value that represents the kind of data that was input.",
					"linecount" : 2,
					"id" : "obj-7",
					"fontname" : "Geneva",
					"patching_rect" : [ 6.0, 244.0, 340.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "listType",
					"id" : "obj-8",
					"outlettype" : [ "int" ],
					"fontname" : "Geneva",
					"patching_rect" : [ 26.0, 140.0, 45.0, 19.0 ],
					"color" : [ 1.0, 0.890196, 0.090196, 1.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"triscale" : 0.9,
					"id" : "obj-9",
					"outlettype" : [ "int", "bang" ],
					"fontname" : "Geneva",
					"patching_rect" : [ 26.0, 170.0, 35.0, 19.0 ],
					"fontsize" : 9.0,
					"triangle" : 0,
					"numinlets" : 1,
					"htextcolor" : [ 0.870588, 0.870588, 0.870588, 1.0 ],
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
					"numoutlets" : 2,
					"cantchange" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "symbol blarg",
					"id" : "obj-10",
					"outlettype" : [ "" ],
					"fontname" : "Geneva",
					"patching_rect" : [ 188.0, 76.0, 65.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "blorg  blurg",
					"id" : "obj-11",
					"outlettype" : [ "" ],
					"fontname" : "Geneva",
					"patching_rect" : [ 127.0, 76.0, 60.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "blorg",
					"id" : "obj-12",
					"outlettype" : [ "" ],
					"fontname" : "Geneva",
					"patching_rect" : [ 95.0, 76.0, 31.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1 2 3",
					"id" : "obj-13",
					"outlettype" : [ "" ],
					"fontname" : "Geneva",
					"patching_rect" : [ 61.0, 76.0, 32.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1.5",
					"id" : "obj-14",
					"outlettype" : [ "" ],
					"fontname" : "Geneva",
					"patching_rect" : [ 36.0, 76.0, 23.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1",
					"id" : "obj-15",
					"outlettype" : [ "" ],
					"fontname" : "Geneva",
					"patching_rect" : [ 20.0, 76.0, 16.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"id" : "obj-16",
					"outlettype" : [ "bang" ],
					"fgcolor" : [ 0.156863, 0.8, 0.54902, 1.0 ],
					"patching_rect" : [ 2.0, 76.0, 15.0, 15.0 ],
					"numinlets" : 1,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1 blat 2 3.5",
					"id" : "obj-17",
					"outlettype" : [ "" ],
					"fontname" : "Geneva",
					"patching_rect" : [ 254.0, 76.0, 61.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"numoutlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Output values:",
					"id" : "obj-18",
					"fontname" : "Geneva",
					"patching_rect" : [ 84.0, 119.0, 74.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "1 -> non-list seen;\r2 -> empty list seen;\r3 -> integer list seen;\r4 -> float list seen;\r5 -> numeric list seen;\r6 -> symbol list seen;\r7 -> mixed list seen",
					"linecount" : 7,
					"id" : "obj-19",
					"fontname" : "Geneva",
					"patching_rect" : [ 84.0, 131.0, 110.0, 91.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1,
					"numoutlets" : 0
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-9", 0 ],
					"hidden" : 0,
					"midpoints" : [ 35.5, 163.0, 35.5, 163.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [ 391.5, 107.0, 35.5, 107.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [ 349.5, 107.0, 35.5, 107.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [ 325.5, 107.0, 35.5, 107.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 0 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [ 197.5, 107.0, 35.5, 107.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [ 136.5, 107.0, 35.5, 107.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-12", 0 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [ 104.5, 107.0, 35.5, 107.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-13", 0 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [ 70.5, 107.0, 35.5, 107.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 0 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [ 45.5, 107.0, 35.5, 107.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [ 11.0, 107.0, 35.5, 107.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [ 29.5, 107.0, 35.5, 107.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [ 263.5, 107.0, 35.5, 107.0 ]
				}

			}
 ]
	}

}
