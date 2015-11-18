{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 5,
			"minor" : 1,
			"revision" : 9
		}
,
		"rect" : [ 528.0, 380.0, 344.0, 261.0 ],
		"bglocked" : 0,
		"defrect" : [ 528.0, 380.0, 344.0, 261.0 ],
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
					"maxclass" : "number",
					"fontsize" : 9.0,
					"triangle" : 0,
					"patching_rect" : [ 7.0, 169.0, 35.0, 19.0 ],
					"id" : "obj-1",
					"htextcolor" : [ 0.870588, 0.870588, 0.870588, 1.0 ],
					"numinlets" : 1,
					"triscale" : 0.9,
					"fontname" : "Geneva",
					"numoutlets" : 2,
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
					"outlettype" : [ "int", "bang" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "bleh blah",
					"fontsize" : 9.0,
					"patching_rect" : [ 30.0, 105.0, 57.0, 17.0 ],
					"id" : "obj-2",
					"numinlets" : 2,
					"fontname" : "Geneva",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "Vlength",
					"fontsize" : 9.0,
					"patching_rect" : [ 7.0, 142.0, 49.0, 19.0 ],
					"id" : "obj-3",
					"numinlets" : 1,
					"color" : [ 1.0, 0.890196, 0.090196, 1.0 ],
					"fontname" : "Geneva",
					"numoutlets" : 1,
					"outlettype" : [ "int" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "blah blah blah bluh",
					"fontsize" : 9.0,
					"patching_rect" : [ 7.0, 67.0, 89.0, 17.0 ],
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
					"text" : "1 2 3 4 5",
					"fontsize" : 9.0,
					"patching_rect" : [ 16.0, 86.0, 50.0, 17.0 ],
					"id" : "obj-5",
					"numinlets" : 2,
					"fontname" : "Geneva",
					"numoutlets" : 1,
					"outlettype" : [ "" ]
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Vlength",
					"fontsize" : 14.0,
					"patching_rect" : [ 4.0, 25.0, 68.0, 25.0 ],
					"id" : "obj-6",
					"numinlets" : 1,
					"fontname" : "Geneva",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Return the number of elements in a list",
					"fontsize" : 9.0,
					"patching_rect" : [ 4.0, 45.0, 183.0, 19.0 ],
					"id" : "obj-7",
					"numinlets" : 1,
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
					"id" : "obj-8",
					"numinlets" : 1,
					"fontname" : "Geneva",
					"numoutlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "The inlet takes an arbitrary list and outputs the number of elements in the list.",
					"linecount" : 2,
					"fontsize" : 9.0,
					"patching_rect" : [ 2.0, 189.0, 272.0, 31.0 ],
					"id" : "obj-9",
					"numinlets" : 1,
					"fontname" : "Geneva",
					"numoutlets" : 0
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [ 16.5, 165.0, 16.5, 165.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [ 39.5, 132.0, 16.5, 132.0 ]
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
 ]
	}

}
