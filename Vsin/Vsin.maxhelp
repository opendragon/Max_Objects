{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 5,
			"minor" : 1,
			"revision" : 9
		}
,
		"rect" : [ 516.0, 209.0, 387.0, 319.0 ],
		"bglocked" : 0,
		"defrect" : [ 516.0, 209.0, 387.0, 319.0 ],
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
					"text" : "-1.1 2.3 4.6 -0.7",
					"fontsize" : 9.0,
					"patching_rect" : [ 35.0, 127.0, 85.0, 17.0 ],
					"id" : "obj-1",
					"numinlets" : 2,
					"fontname" : "Geneva",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"patching_rect" : [ 303.0, 257.0, 15.0, 15.0 ],
					"id" : "obj-2",
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
					"patching_rect" : [ 6.0, 202.0, 64.0, 19.0 ],
					"id" : "obj-3",
					"numinlets" : 1,
					"fontname" : "Geneva",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1 2 3 4",
					"fontsize" : 9.0,
					"patching_rect" : [ 6.0, 70.0, 41.0, 17.0 ],
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
					"text" : "1 2 3 blorg",
					"fontsize" : 9.0,
					"patching_rect" : [ 15.0, 90.0, 58.0, 17.0 ],
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
					"text" : "1 2 3 4.5",
					"fontsize" : 9.0,
					"patching_rect" : [ 25.0, 108.0, 50.0, 17.0 ],
					"id" : "obj-6",
					"numinlets" : 2,
					"fontname" : "Geneva",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Vsin",
					"fontsize" : 14.0,
					"patching_rect" : [ 3.0, 25.0, 85.0, 25.0 ],
					"id" : "obj-7",
					"numinlets" : 1,
					"fontname" : "Geneva",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Returns the sine of the input.",
					"fontsize" : 9.0,
					"patching_rect" : [ 3.0, 45.0, 236.0, 19.0 ],
					"id" : "obj-8",
					"numinlets" : 1,
					"fontname" : "Geneva",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Argument: none;\rLeft Inlet: list;\rOutlet: list",
					"linecount" : 3,
					"fontsize" : 9.0,
					"patching_rect" : [ 241.0, 22.0, 81.0, 43.0 ],
					"id" : "obj-9",
					"numinlets" : 1,
					"fontname" : "Geneva",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "Vsin",
					"fontsize" : 9.0,
					"patching_rect" : [ 6.0, 170.0, 27.0, 19.0 ],
					"id" : "obj-10",
					"numinlets" : 1,
					"color" : [ 1.0, 0.890196, 0.090196, 1.0 ],
					"fontname" : "Geneva",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "0.841471 0.909297 0.14112 -0.97753",
					"fontsize" : 9.0,
					"patching_rect" : [ 6.0, 257.0, 291.0, 17.0 ],
					"id" : "obj-11",
					"numinlets" : 2,
					"fontname" : "Geneva",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [ 15.5, 246.0, 312.0, 246.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 0,
					"midpoints" : [ 15.5, 248.0, 15.5, 248.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [ 15.5, 192.0, 15.5, 192.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-10", 0 ],
					"hidden" : 0,
					"midpoints" : [ 44.5, 154.0, 15.5, 154.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-10", 0 ],
					"hidden" : 0,
					"midpoints" : [ 34.5, 154.0, 15.5, 154.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-4", 0 ],
					"destination" : [ "obj-10", 0 ],
					"hidden" : 0,
					"midpoints" : [ 15.5, 154.0, 15.5, 154.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-10", 0 ],
					"hidden" : 0,
					"midpoints" : [ 24.5, 154.0, 15.5, 154.0 ]
				}

			}
 ]
	}

}
