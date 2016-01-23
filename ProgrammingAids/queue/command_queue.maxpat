{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 5,
			"minor" : 1,
			"revision" : 9
		}
,
		"rect" : [ 273.0, 84.0, 543.0, 655.0 ],
		"bglocked" : 0,
		"defrect" : [ 273.0, 84.0, 543.0, 655.0 ],
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
					"maxclass" : "newobj",
					"text" : "pv c-state 0",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-1",
					"fontname" : "Geneva",
					"patching_rect" : [ 334.0, 116.0, 64.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "pv c-state 0",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-2",
					"fontname" : "Geneva",
					"patching_rect" : [ 209.0, 181.0, 64.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "pv q-depth 0",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-3",
					"fontname" : "Geneva",
					"color" : [ 0.4, 0.4, 0.8, 1.0 ],
					"patching_rect" : [ 254.0, 467.0, 65.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "pv c-state 0",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-4",
					"fontname" : "Geneva",
					"patching_rect" : [ 172.0, 566.0, 64.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "pv q-depth 0",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-5",
					"fontname" : "Geneva",
					"color" : [ 0.4, 0.4, 0.8, 1.0 ],
					"patching_rect" : [ 71.0, 100.0, 65.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "t b b",
					"linecount" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "bang", "bang" ],
					"id" : "obj-6",
					"fontname" : "Geneva",
					"patching_rect" : [ 116.0, 60.0, 28.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "0",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-7",
					"fontname" : "Geneva",
					"patching_rect" : [ 375.0, 87.0, 16.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "depth",
					"linecount" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-8",
					"fontname" : "Geneva",
					"patching_rect" : [ 134.0, 146.0, 32.0, 29.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "t b l",
					"numoutlets" : 2,
					"outlettype" : [ "bang", "" ],
					"id" : "obj-9",
					"fontname" : "Geneva",
					"patching_rect" : [ 172.0, 475.0, 27.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "select 0",
					"numoutlets" : 2,
					"outlettype" : [ "bang", "" ],
					"id" : "obj-10",
					"fontname" : "Geneva",
					"patching_rect" : [ 71.0, 127.0, 44.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "1",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-11",
					"fontname" : "Geneva",
					"patching_rect" : [ 172.0, 525.0, 16.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "pull",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-12",
					"fontname" : "Geneva",
					"patching_rect" : [ 105.0, 175.0, 24.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "prepend add",
					"linecount" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-13",
					"fontname" : "Geneva",
					"patching_rect" : [ 238.0, 307.0, 61.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "t l b b",
					"numoutlets" : 3,
					"outlettype" : [ "", "bang", "bang" ],
					"id" : "obj-14",
					"fontname" : "Geneva",
					"patching_rect" : [ 194.0, 108.0, 40.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "t b b",
					"linecount" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "bang", "bang" ],
					"id" : "obj-15",
					"fontname" : "Geneva",
					"patching_rect" : [ 375.0, 60.0, 28.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "gswitch2",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"id" : "obj-16",
					"patching_rect" : [ 209.0, 261.0, 39.0, 32.0 ],
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "clear",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-17",
					"fontname" : "Geneva",
					"patching_rect" : [ 399.0, 87.0, 31.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "inlet",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-18",
					"patching_rect" : [ 375.0, 29.0, 15.0, 15.0 ],
					"numinlets" : 0,
					"comment" : "flush queue"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "outlet",
					"numoutlets" : 0,
					"id" : "obj-19",
					"patching_rect" : [ 238.0, 589.0, 15.0, 15.0 ],
					"numinlets" : 1,
					"comment" : "commands out"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "inlet",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-20",
					"patching_rect" : [ 194.0, 29.0, 15.0, 15.0 ],
					"numinlets" : 0,
					"comment" : "commands in"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "inlet",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-21",
					"patching_rect" : [ 116.0, 29.0, 15.0, 15.0 ],
					"numinlets" : 0,
					"comment" : "bang from command completion"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "queue 30",
					"linecount" : 2,
					"numoutlets" : 3,
					"outlettype" : [ "", "int", "bang" ],
					"id" : "obj-22",
					"fontname" : "Geneva",
					"color" : [ 1.0, 0.890196, 0.090196, 1.0 ],
					"patching_rect" : [ 134.0, 341.0, 48.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "bang from command completion",
					"linecount" : 3,
					"numoutlets" : 0,
					"id" : "obj-23",
					"fontname" : "Geneva",
					"patching_rect" : [ 56.0, 29.0, 58.0, 43.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "commands in",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-24",
					"fontname" : "Geneva",
					"patching_rect" : [ 210.0, 29.0, 52.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "commands out",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-25",
					"fontname" : "Geneva",
					"patching_rect" : [ 258.0, 589.0, 55.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "flush queue",
					"linecount" : 3,
					"numoutlets" : 0,
					"id" : "obj-26",
					"fontname" : "Geneva",
					"patching_rect" : [ 391.0, 29.0, 34.0, 43.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "route the command either directly out or to the queue",
					"linecount" : 3,
					"numoutlets" : 0,
					"id" : "obj-27",
					"fontname" : "Geneva",
					"patching_rect" : [ 254.0, 260.0, 100.0, 43.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "c-state records whether a command can be sent directly",
					"linecount" : 3,
					"numoutlets" : 0,
					"id" : "obj-28",
					"fontname" : "Geneva",
					"patching_rect" : [ 410.0, 110.0, 100.0, 43.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "q-depth is queue depth",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-29",
					"fontname" : "Geneva",
					"patching_rect" : [ 321.0, 465.0, 84.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-15", 1 ],
					"destination" : [ "obj-17", 0 ],
					"hidden" : 0,
					"midpoints" : [ 393.5, 83.0, 408.5, 83.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 0,
					"midpoints" : [ 384.5, 82.0, 381.0, 82.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"midpoints" : [ 384.0, 58.0, 384.5, 58.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-7", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [ 384.5, 109.0, 343.5, 109.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 1 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 0,
					"midpoints" : [ 158.0, 413.0, 263.5, 413.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-9", 1 ],
					"destination" : [ "obj-19", 0 ],
					"hidden" : 0,
					"midpoints" : [ 189.5, 504.0, 247.0, 504.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-19", 0 ],
					"hidden" : 0,
					"midpoints" : [ 143.5, 442.0, 247.0, 442.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-16", 1 ],
					"destination" : [ "obj-13", 0 ],
					"hidden" : 0,
					"midpoints" : [ 238.5, 300.0, 247.5, 300.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 0 ],
					"destination" : [ "obj-16", 1 ],
					"hidden" : 0,
					"midpoints" : [ 203.5, 243.0, 238.5, 243.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-16", 0 ],
					"hidden" : 0,
					"midpoints" : [ 218.5, 229.0, 218.5, 229.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 1 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [ 214.0, 153.0, 218.5, 153.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [ 80.5, 167.0, 218.5, 167.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-20", 0 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 0,
					"midpoints" : [ 203.0, 76.0, 203.5, 76.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-4", 0 ],
					"hidden" : 0,
					"midpoints" : [ 181.5, 553.0, 181.5, 553.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-9", 0 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 0,
					"midpoints" : [ 181.5, 508.0, 178.0, 508.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-9", 0 ],
					"hidden" : 0,
					"midpoints" : [ 218.5, 384.0, 181.5, 384.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-9", 0 ],
					"hidden" : 0,
					"midpoints" : [ 143.5, 442.0, 181.5, 442.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-22", 0 ],
					"hidden" : 0,
					"midpoints" : [ 408.5, 173.0, 143.5, 173.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-13", 0 ],
					"destination" : [ "obj-22", 0 ],
					"hidden" : 0,
					"midpoints" : [ 247.5, 333.0, 143.5, 333.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-12", 0 ],
					"destination" : [ "obj-22", 0 ],
					"hidden" : 0,
					"midpoints" : [ 114.5, 333.0, 143.5, 333.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-22", 0 ],
					"hidden" : 0,
					"midpoints" : [ 143.5, 173.0, 143.5, 173.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 2 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [ 224.5, 135.0, 143.5, 135.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 1 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [ 134.5, 111.0, 143.5, 111.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-21", 0 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 0,
					"midpoints" : [ 125.0, 52.0, 125.5, 52.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 1 ],
					"destination" : [ "obj-12", 0 ],
					"hidden" : 0,
					"midpoints" : [ 105.5, 159.0, 114.5, 159.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-10", 0 ],
					"hidden" : 0,
					"midpoints" : [ 80.5, 122.0, 80.5, 122.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 0,
					"midpoints" : [ 125.5, 88.0, 80.5, 88.0 ]
				}

			}
 ]
	}

}
