{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 5,
			"minor" : 1,
			"revision" : 9
		}
,
		"rect" : [ 331.0, 503.0, 344.0, 261.0 ],
		"bglocked" : 0,
		"defrect" : [ 331.0, 503.0, 344.0, 261.0 ],
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
					"text" : "0 0 10",
					"fontsize" : 9.0,
					"patching_rect" : [ 29.0, 103.0, 38.0, 17.0 ],
					"numinlets" : 2,
					"id" : "obj-1",
					"fontname" : "Geneva",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
					"triscale" : 0.9,
					"fontsize" : 9.0,
					"htextcolor" : [ 0.870588, 0.870588, 0.870588, 1.0 ],
					"patching_rect" : [ 7.0, 169.0, 35.0, 19.0 ],
					"numinlets" : 1,
					"id" : "obj-2",
					"fontname" : "Geneva",
					"numoutlets" : 2,
					"triangle" : 0,
					"outlettype" : [ "int", "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "Vdistance",
					"fontsize" : 9.0,
					"color" : [ 1.0, 0.890196, 0.090196, 1.0 ],
					"patching_rect" : [ 7.0, 142.0, 51.0, 19.0 ],
					"numinlets" : 1,
					"id" : "obj-3",
					"fontname" : "Geneva",
					"numoutlets" : 1,
					"outlettype" : [ "float" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "blah blah blah bluh",
					"fontsize" : 9.0,
					"patching_rect" : [ 7.0, 67.0, 89.0, 17.0 ],
					"numinlets" : 2,
					"id" : "obj-4",
					"fontname" : "Geneva",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1 2 3 4 5",
					"fontsize" : 9.0,
					"patching_rect" : [ 16.0, 86.0, 50.0, 17.0 ],
					"numinlets" : 2,
					"id" : "obj-5",
					"fontname" : "Geneva",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Vdistance",
					"fontsize" : 14.0,
					"patching_rect" : [ 4.0, 25.0, 87.0, 25.0 ],
					"numinlets" : 1,
					"id" : "obj-6",
					"fontname" : "Geneva",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Return the length of a vector",
					"fontsize" : 9.0,
					"patching_rect" : [ 4.0, 45.0, 183.0, 19.0 ],
					"numinlets" : 1,
					"id" : "obj-7",
					"fontname" : "Geneva",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Argument: none;\rInlet: list;\rOutlet: number",
					"linecount" : 3,
					"fontsize" : 9.0,
					"patching_rect" : [ 192.0, 22.0, 82.0, 43.0 ],
					"numinlets" : 1,
					"id" : "obj-8",
					"fontname" : "Geneva",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "The inlet takes a numeric list and outputs the length of the list, considered as an n-dimensional vector.",
					"linecount" : 2,
					"fontsize" : 9.0,
					"patching_rect" : [ 2.0, 189.0, 272.0, 31.0 ],
					"numinlets" : 1,
					"id" : "obj-9",
					"fontname" : "Geneva",
					"numoutlets" : 0
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [ 16.5, 165.0, 16.5, 165.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-4", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [ 16.5, 132.0, 16.5, 132.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [ 25.5, 132.0, 16.5, 132.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [ 38.5, 132.0, 16.5, 132.0 ]
				}

			}
 ]
	}

}
