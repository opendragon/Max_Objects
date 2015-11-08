{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 5,
			"minor" : 1,
			"revision" : 9
		}
,
		"rect" : [ 126.0, 152.0, 592.0, 423.0 ],
		"bglocked" : 0,
		"defrect" : [ 126.0, 152.0, 592.0, 423.0 ],
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
					"maxclass" : "inlet",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-1",
					"patching_rect" : [ 20.0, 33.0, 15.0, 15.0 ],
					"numinlets" : 0,
					"comment" : "Toggle status tracing"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "outlet",
					"numoutlets" : 0,
					"id" : "obj-2",
					"patching_rect" : [ 97.0, 351.0, 15.0, 15.0 ],
					"numinlets" : 1,
					"comment" : "Sequence complete"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "outlet",
					"numoutlets" : 0,
					"id" : "obj-3",
					"patching_rect" : [ 265.0, 351.0, 15.0, 15.0 ],
					"numinlets" : 1,
					"comment" : "Error detected"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "outlet",
					"numoutlets" : 0,
					"id" : "obj-4",
					"patching_rect" : [ 182.0, 351.0, 15.0, 15.0 ],
					"numinlets" : 1,
					"comment" : "Command complete"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "inlet",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-5",
					"patching_rect" : [ 200.0, 33.0, 15.0, 15.0 ],
					"numinlets" : 0,
					"comment" : "Commands in"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print",
					"numoutlets" : 0,
					"id" : "obj-6",
					"fontname" : "Geneva",
					"patching_rect" : [ 46.0, 351.0, 30.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "gswitch2",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"id" : "obj-7",
					"patching_rect" : [ 210.0, 282.0, 39.0, 32.0 ],
					"numinlets" : 2,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "gswitch2",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"id" : "obj-8",
					"patching_rect" : [ 17.0, 282.0, 39.0, 32.0 ],
					"numinlets" : 2,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "sprintf serialX %lx '%c' %ld",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-9",
					"fontname" : "Geneva",
					"patching_rect" : [ 329.0, 226.0, 138.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 3,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "command complete",
					"linecount" : 3,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-10",
					"fontname" : "Geneva",
					"patching_rect" : [ 126.0, 216.0, 49.0, 41.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "error",
					"numoutlets" : 0,
					"id" : "obj-11",
					"fontname" : "Geneva",
					"patching_rect" : [ 284.0, 351.0, 35.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "sequence complete",
					"linecount" : 4,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-12",
					"fontname" : "Geneva",
					"patching_rect" : [ 46.0, 216.0, 47.0, 53.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "set up: gvp100 select_address, [poll rate], [poll size]",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-13",
					"fontname" : "Geneva",
					"patching_rect" : [ 43.0, 162.0, 154.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "serialX a #2 8 1 odd",
					"numoutlets" : 3,
					"outlettype" : [ "", "", "" ],
					"id" : "obj-14",
					"fontname" : "Geneva",
					"color" : [ 1.0, 0.890196, 0.090196, 1.0 ],
					"patching_rect" : [ 329.0, 164.0, 125.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "gvp100 #1 20",
					"numoutlets" : 6,
					"outlettype" : [ "", "", "", "", "", "" ],
					"id" : "obj-15",
					"fontname" : "Geneva",
					"color" : [ 1.0, 0.890196, 0.090196, 1.0 ],
					"patching_rect" : [ 200.0, 164.0, 79.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "break",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-16",
					"fontname" : "Geneva",
					"patching_rect" : [ 329.0, 113.0, 33.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "breakdone",
					"linecount" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-17",
					"fontname" : "Geneva",
					"patching_rect" : [ 232.0, 113.0, 53.0, 29.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "commands in",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-18",
					"fontname" : "Geneva",
					"patching_rect" : [ 218.0, 33.0, 63.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "sequence complete",
					"linecount" : 4,
					"numoutlets" : 0,
					"id" : "obj-19",
					"fontname" : "Geneva",
					"patching_rect" : [ 117.0, 351.0, 47.0, 55.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "command complete",
					"linecount" : 4,
					"numoutlets" : 0,
					"id" : "obj-20",
					"fontname" : "Geneva",
					"patching_rect" : [ 201.0, 351.0, 47.0, 55.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "enable tracing",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-21",
					"fontname" : "Geneva",
					"patching_rect" : [ 43.0, 33.0, 40.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "arguments:;\r1 --> select address;\r2 --> baud rate",
					"linecount" : 3,
					"numoutlets" : 0,
					"id" : "obj-22",
					"fontname" : "Geneva",
					"patching_rect" : [ 91.0, 33.0, 100.0, 43.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 1,
					"midpoints" : [ 241.5, 144.0, 209.5, 144.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 1,
					"midpoints" : [ 338.5, 150.0, 338.5, 150.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 3 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 1,
					"midpoints" : [ 245.5, 200.0, 319.0, 200.0, 319.0, 150.0, 338.5, 150.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 2 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 1,
					"midpoints" : [ 233.5, 200.0, 319.0, 200.0, 319.0, 150.0, 338.5, 150.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 4 ],
					"destination" : [ "obj-16", 0 ],
					"hidden" : 1,
					"midpoints" : [ 257.5, 191.0, 312.0, 191.0, 312.0, 102.0, 338.5, 102.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 5 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [ 269.5, 329.0, 274.0, 329.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 3 ],
					"destination" : [ "obj-7", 1 ],
					"hidden" : 1,
					"midpoints" : [ 269.5, 268.0, 239.5, 268.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 1 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 0,
					"midpoints" : [ 269.5, 209.0, 187.0, 209.0, 187.0, 323.0, 191.0, 323.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 1 ],
					"destination" : [ "obj-10", 0 ],
					"hidden" : 1,
					"midpoints" : [ 269.5, 209.0, 135.5, 209.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [ 209.5, 204.0, 102.0, 204.0, 102.0, 265.0, 106.0, 265.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 0 ],
					"destination" : [ "obj-15", 1 ],
					"hidden" : 1,
					"midpoints" : [ 338.5, 206.0, 302.0, 206.0, 302.0, 144.0, 269.5, 144.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-12", 0 ],
					"hidden" : 1,
					"midpoints" : [ 209.5, 204.0, 55.5, 204.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 0 ],
					"destination" : [ "obj-9", 2 ],
					"hidden" : 1,
					"midpoints" : [ 338.5, 206.0, 457.5, 206.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 0 ],
					"destination" : [ "obj-9", 1 ],
					"hidden" : 1,
					"midpoints" : [ 338.5, 206.0, 398.0, 206.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 0 ],
					"destination" : [ "obj-9", 0 ],
					"hidden" : 1,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 2 ],
					"destination" : [ "obj-17", 0 ],
					"hidden" : 1,
					"midpoints" : [ 444.5, 193.0, 480.0, 193.0, 480.0, 94.0, 241.5, 94.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 0 ],
					"destination" : [ "obj-8", 1 ],
					"hidden" : 1,
					"midpoints" : [ 135.5, 259.0, 46.5, 259.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-12", 0 ],
					"destination" : [ "obj-8", 1 ],
					"hidden" : 1,
					"midpoints" : [ 55.5, 260.0, 46.5, 260.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 1 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 1,
					"midpoints" : [ 46.5, 335.0, 55.5, 335.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-7", 1 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 1,
					"midpoints" : [ 239.5, 335.0, 55.5, 335.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"midpoints" : [ 209.0, 93.0, 205.0, 93.0, 205.0, 143.0, 209.5, 143.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 1,
					"midpoints" : [ 29.0, 269.0, 26.5, 269.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-1", 0 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 1,
					"midpoints" : [ 29.0, 269.0, 219.5, 269.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-9", 0 ],
					"destination" : [ "obj-8", 1 ],
					"hidden" : 1,
					"midpoints" : [ 338.5, 259.0, 46.5, 259.0 ]
				}

			}
 ]
	}

}
