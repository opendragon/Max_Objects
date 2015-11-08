{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 5,
			"minor" : 1,
			"revision" : 9
		}
,
		"rect" : [ 331.0, 280.0, 371.0, 319.0 ],
		"bglocked" : 0,
		"defrect" : [ 331.0, 280.0, 371.0, 319.0 ],
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
					"text" : "blirg",
					"fontsize" : 9.0,
					"patching_rect" : [ 159.0, 76.0, 29.0, 17.0 ],
					"numinlets" : 2,
					"id" : "obj-1",
					"fontname" : "Geneva",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "blorg",
					"fontsize" : 9.0,
					"patching_rect" : [ 125.0, 76.0, 31.0, 17.0 ],
					"numinlets" : 2,
					"id" : "obj-2",
					"fontname" : "Geneva",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "compares",
					"fontsize" : 14.0,
					"patching_rect" : [ 7.0, 24.0, 80.0, 25.0 ],
					"numinlets" : 1,
					"id" : "obj-3",
					"fontname" : "Geneva",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Compare two symbols",
					"fontsize" : 9.0,
					"patching_rect" : [ 7.0, 44.0, 112.0, 19.0 ],
					"numinlets" : 1,
					"id" : "obj-4",
					"fontname" : "Geneva",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Argument: none;\rLeft Inlet: symbol;\rRight Inlet: symbol;\rOutlet: int",
					"linecount" : 4,
					"fontsize" : 9.0,
					"patching_rect" : [ 201.0, 22.0, 102.0, 55.0 ],
					"numinlets" : 1,
					"id" : "obj-5",
					"fontname" : "Geneva",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "The inlets take arbitrary symbols and return a result corresponding to the comparison of the symbol strings.",
					"linecount" : 2,
					"fontsize" : 9.0,
					"patching_rect" : [ 6.0, 233.0, 340.0, 31.0 ],
					"numinlets" : 1,
					"id" : "obj-6",
					"fontname" : "Geneva",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "compares",
					"fontsize" : 9.0,
					"patching_rect" : [ 26.0, 136.0, 51.0, 19.0 ],
					"color" : [ 1.0, 0.890196, 0.090196, 1.0 ],
					"numinlets" : 2,
					"id" : "obj-7",
					"fontname" : "Geneva",
					"numoutlets" : 1,
					"outlettype" : [ "int" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"fontsize" : 9.0,
					"cantchange" : 1,
					"htextcolor" : [ 0.870588, 0.870588, 0.870588, 1.0 ],
					"patching_rect" : [ 26.0, 166.0, 35.0, 19.0 ],
					"triscale" : 0.9,
					"numinlets" : 1,
					"id" : "obj-8",
					"fontname" : "Geneva",
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
					"numoutlets" : 2,
					"triangle" : 0,
					"outlettype" : [ "int", "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "blarg",
					"fontsize" : 9.0,
					"patching_rect" : [ 59.0, 76.0, 31.0, 17.0 ],
					"numinlets" : 2,
					"id" : "obj-9",
					"fontname" : "Geneva",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "blorg",
					"fontsize" : 9.0,
					"patching_rect" : [ 26.0, 76.0, 31.0, 17.0 ],
					"numinlets" : 2,
					"id" : "obj-10",
					"fontname" : "Geneva",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "blarg",
					"fontsize" : 9.0,
					"patching_rect" : [ 92.0, 76.0, 31.0, 17.0 ],
					"numinlets" : 2,
					"id" : "obj-11",
					"fontname" : "Geneva",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Output values:",
					"fontsize" : 9.0,
					"patching_rect" : [ 83.0, 114.0, 74.0, 19.0 ],
					"numinlets" : 1,
					"id" : "obj-12",
					"fontname" : "Geneva",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "1 -> left symbol greater;\r0 -> symbols identical;\r-1 -> right symbol greater;\r",
					"linecount" : 3,
					"fontsize" : 9.0,
					"patching_rect" : [ 84.0, 127.0, 134.0, 43.0 ],
					"numinlets" : 1,
					"id" : "obj-13",
					"fontname" : "Geneva",
					"numoutlets" : 0
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-7", 1 ],
					"hidden" : 0,
					"midpoints" : [ 134.5, 104.0, 67.5, 104.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-7", 1 ],
					"hidden" : 0,
					"midpoints" : [ 168.5, 104.0, 67.5, 104.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-7", 1 ],
					"hidden" : 0,
					"midpoints" : [ 101.5, 104.0, 67.5, 104.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-7", 0 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [ 35.5, 159.0, 35.5, 159.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-9", 0 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 0,
					"midpoints" : [ 68.5, 103.0, 35.5, 103.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 0 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 0,
					"midpoints" : [ 35.5, 103.0, 35.5, 103.0 ]
				}

			}
 ]
	}

}
