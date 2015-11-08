{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 5,
			"minor" : 1,
			"revision" : 9
		}
,
		"rect" : [ 406.0, 262.0, 392.0, 318.0 ],
		"bglocked" : 0,
		"defrect" : [ 406.0, 262.0, 392.0, 318.0 ],
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
					"patching_rect" : [ 119.0, 258.0, 15.0, 15.0 ],
					"id" : "obj-2",
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
					"patching_rect" : [ 40.0, 119.0, 55.0, 17.0 ],
					"id" : "obj-3",
					"numinlets" : 2,
					"fontname" : "Geneva",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"patching_rect" : [ 101.0, 258.0, 15.0, 15.0 ],
					"id" : "obj-4",
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
					"patching_rect" : [ 11.0, 225.0, 73.0, 19.0 ],
					"id" : "obj-5",
					"numinlets" : 1,
					"fontname" : "Geneva",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "Vltrim 1",
					"fontsize" : 9.0,
					"patching_rect" : [ 11.0, 199.0, 45.0, 19.0 ],
					"id" : "obj-6",
					"numinlets" : 1,
					"color" : [ 1.0, 0.890196, 0.090196, 1.0 ],
					"fontname" : "Geneva",
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1 1 0 1",
					"fontsize" : 9.0,
					"patching_rect" : [ 11.0, 63.0, 41.0, 17.0 ],
					"id" : "obj-7",
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
					"patching_rect" : [ 20.0, 83.0, 58.0, 17.0 ],
					"id" : "obj-8",
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
					"patching_rect" : [ 30.0, 101.0, 50.0, 17.0 ],
					"id" : "obj-9",
					"numinlets" : 2,
					"fontname" : "Geneva",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Vltrim",
					"fontsize" : 14.0,
					"patching_rect" : [ 8.0, 25.0, 78.0, 25.0 ],
					"id" : "obj-10",
					"numinlets" : 1,
					"fontname" : "Geneva",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Removes leading separators from a list.",
					"fontsize" : 9.0,
					"patching_rect" : [ 8.0, 47.0, 200.0, 19.0 ],
					"id" : "obj-11",
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
					"patching_rect" : [ 232.0, 24.0, 100.0, 43.0 ],
					"id" : "obj-12",
					"numinlets" : 1,
					"fontname" : "Geneva",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "-1 2 -4 37",
					"fontsize" : 9.0,
					"patching_rect" : [ 11.0, 258.0, 87.0, 17.0 ],
					"id" : "obj-13",
					"numinlets" : 2,
					"fontname" : "Geneva",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-6", 1 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [ 46.5, 220.0, 128.0, 220.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 0,
					"midpoints" : [ 20.5, 248.0, 110.0, 248.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-13", 0 ],
					"hidden" : 0,
					"midpoints" : [ 20.5, 250.0, 20.5, 250.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 0,
					"midpoints" : [ 20.5, 221.0, 20.5, 221.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-7", 0 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 0,
					"midpoints" : [ 20.5, 186.0, 20.5, 186.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 0,
					"midpoints" : [ 49.5, 186.0, 20.5, 186.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 0,
					"midpoints" : [ 29.5, 186.0, 20.5, 186.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-9", 0 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 0,
					"midpoints" : [ 39.5, 186.0, 20.5, 186.0 ]
				}

			}
 ]
	}

}
