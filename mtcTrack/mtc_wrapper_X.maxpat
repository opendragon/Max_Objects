{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 5,
			"minor" : 1,
			"revision" : 9
		}
,
		"rect" : [ 351.0, 209.0, 618.0, 500.0 ],
		"bglocked" : 0,
		"defrect" : [ 351.0, 209.0, 618.0, 500.0 ],
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
					"maxclass" : "gswitch2",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"id" : "obj-1",
					"patching_rect" : [ 436.0, 310.0, 39.0, 32.0 ],
					"numinlets" : 2,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print serial->",
					"numoutlets" : 0,
					"id" : "obj-2",
					"fontname" : "Geneva",
					"patching_rect" : [ 465.0, 414.0, 70.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "outlet",
					"numoutlets" : 0,
					"id" : "obj-3",
					"patching_rect" : [ 141.0, 414.0, 15.0, 15.0 ],
					"numinlets" : 1,
					"comment" : "Data start/stop"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "outlet",
					"numoutlets" : 0,
					"id" : "obj-4",
					"patching_rect" : [ 69.0, 414.0, 15.0, 15.0 ],
					"numinlets" : 1,
					"comment" : "Data seen"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "Tee",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"id" : "obj-5",
					"fontname" : "Geneva",
					"patching_rect" : [ 440.0, 190.0, 27.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print serial<-",
					"numoutlets" : 0,
					"id" : "obj-6",
					"fontname" : "Geneva",
					"patching_rect" : [ 384.0, 414.0, 69.0, 19.0 ],
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
					"patching_rect" : [ 355.0, 310.0, 39.0, 32.0 ],
					"numinlets" : 2,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "inlet",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-8",
					"patching_rect" : [ 0.0, 33.0, 15.0, 15.0 ],
					"numinlets" : 0,
					"comment" : "Toggle status tracing"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "outlet",
					"numoutlets" : 0,
					"id" : "obj-9",
					"patching_rect" : [ 317.0, 414.0, 15.0, 15.0 ],
					"numinlets" : 1,
					"comment" : "Error detected"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "outlet",
					"numoutlets" : 0,
					"id" : "obj-10",
					"patching_rect" : [ 233.0, 414.0, 15.0, 15.0 ],
					"numinlets" : 1,
					"comment" : "Command complete"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "inlet",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-11",
					"patching_rect" : [ 183.0, 33.0, 15.0, 15.0 ],
					"numinlets" : 0,
					"comment" : "Commands in"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print",
					"numoutlets" : 0,
					"id" : "obj-12",
					"fontname" : "Geneva",
					"patching_rect" : [ 29.0, 414.0, 30.0, 19.0 ],
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
					"id" : "obj-13",
					"patching_rect" : [ 0.0, 345.0, 39.0, 32.0 ],
					"numinlets" : 2,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "command complete",
					"linecount" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-14",
					"fontname" : "Geneva",
					"patching_rect" : [ 115.0, 292.0, 90.0, 29.0 ],
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
					"id" : "obj-15",
					"fontname" : "Geneva",
					"patching_rect" : [ 336.0, 414.0, 35.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "set up: mtc #hotspots map_file norm_file [mode [sort_order [poll rate]]]",
					"linecount" : 3,
					"numoutlets" : 0,
					"id" : "obj-16",
					"fontname" : "Geneva",
					"patching_rect" : [ 24.0, 204.0, 154.0, 43.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "serial #2 115200 8 1 0",
					"numoutlets" : 2,
					"outlettype" : [ "int", "" ],
					"id" : "obj-17",
					"fontname" : "Geneva",
					"color" : [ 1.0, 0.890196, 0.090196, 1.0 ],
					"patching_rect" : [ 457.0, 225.0, 115.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "mtc #1 mapping.txt normal.txt cooked p 30",
					"numoutlets" : 7,
					"outlettype" : [ "", "int", "bang", "bang", "", "bang", "bang" ],
					"id" : "obj-18",
					"fontname" : "Geneva",
					"color" : [ 1.0, 0.890196, 0.090196, 1.0 ],
					"patching_rect" : [ 183.0, 225.0, 204.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "chunk 60, dtr 0",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-19",
					"fontname" : "Geneva",
					"patching_rect" : [ 440.0, 138.0, 79.0, 17.0 ],
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
					"id" : "obj-20",
					"fontname" : "Geneva",
					"patching_rect" : [ 201.0, 33.0, 63.0, 31.0 ],
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
					"id" : "obj-21",
					"fontname" : "Geneva",
					"patching_rect" : [ 266.0, 414.0, 47.0, 55.0 ],
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
					"id" : "obj-22",
					"fontname" : "Geneva",
					"patching_rect" : [ 23.0, 33.0, 40.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "data seen",
					"numoutlets" : 0,
					"id" : "obj-23",
					"fontname" : "Geneva",
					"patching_rect" : [ 86.0, 414.0, 54.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "data start/stop",
					"linecount" : 3,
					"numoutlets" : 0,
					"id" : "obj-24",
					"fontname" : "Geneva",
					"patching_rect" : [ 160.0, 414.0, 53.0, 43.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-1", 1 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 1,
					"midpoints" : [ 465.5, 378.0, 474.5, 378.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 1 ],
					"destination" : [ "obj-1", 1 ],
					"hidden" : 1,
					"midpoints" : [ 562.5, 288.0, 465.5, 288.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-1", 1 ],
					"hidden" : 1,
					"midpoints" : [ 466.5, 267.0, 465.5, 267.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 1 ],
					"destination" : [ "obj-17", 0 ],
					"hidden" : 1,
					"midpoints" : [ 457.5, 213.0, 466.5, 213.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 0 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 1,
					"midpoints" : [ 449.5, 177.0, 449.5, 177.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 4 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 1,
					"midpoints" : [ 315.833344, 251.0, 406.0, 251.0, 406.0, 177.0, 449.5, 177.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 3 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 1,
					"midpoints" : [ 285.0, 251.0, 406.0, 251.0, 406.0, 177.0, 449.5, 177.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 5 ],
					"destination" : [ "obj-19", 0 ],
					"hidden" : 1,
					"midpoints" : [ 346.666656, 271.0, 392.0, 271.0, 392.0, 128.0, 449.5, 128.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 1,
					"midpoints" : [ 9.0, 275.0, 445.5, 275.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-7", 1 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 1,
					"midpoints" : [ 384.5, 368.0, 393.5, 368.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-7", 1 ],
					"hidden" : 1,
					"midpoints" : [ 449.5, 282.0, 384.5, 282.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-18", 1 ],
					"hidden" : 1,
					"midpoints" : [ 466.5, 267.0, 400.0, 267.0, 400.0, 210.0, 377.5, 210.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 1,
					"midpoints" : [ 9.0, 275.0, 364.5, 275.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 6 ],
					"destination" : [ "obj-9", 0 ],
					"hidden" : 1,
					"midpoints" : [ 377.5, 285.0, 326.0, 285.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 2 ],
					"destination" : [ "obj-10", 0 ],
					"hidden" : 1,
					"midpoints" : [ 254.166672, 260.0, 242.0, 260.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 1,
					"midpoints" : [ 192.0, 93.0, 188.0, 93.0, 188.0, 143.0, 192.5, 143.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 1 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 1,
					"midpoints" : [ 223.333328, 328.0, 150.0, 328.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 2 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 1,
					"midpoints" : [ 254.166672, 260.0, 124.5, 260.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 1,
					"midpoints" : [ 192.5, 255.0, 78.0, 255.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-13", 1 ],
					"destination" : [ "obj-12", 0 ],
					"hidden" : 1,
					"midpoints" : [ 29.5, 398.0, 38.5, 398.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 0 ],
					"destination" : [ "obj-13", 1 ],
					"hidden" : 1,
					"midpoints" : [ 124.5, 322.0, 29.5, 322.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-13", 0 ],
					"hidden" : 1,
					"midpoints" : [ 9.0, 269.0, 9.5, 269.0 ]
				}

			}
 ]
	}

}
