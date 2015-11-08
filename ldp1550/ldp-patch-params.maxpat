{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 5,
			"minor" : 1,
			"revision" : 9
		}
,
		"rect" : [ 5.0, 44.0, 778.0, 464.0 ],
		"bglocked" : 0,
		"defrect" : [ 5.0, 44.0, 778.0, 464.0 ],
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
					"text" : "print <ldp>",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-1",
					"fontname" : "Geneva",
					"patching_rect" : [ 26.0, 196.0, 56.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "sprintf %02.2lx",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-2",
					"fontname" : "Geneva",
					"patching_rect" : [ 26.0, 164.0, 81.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "inlet",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-3",
					"patching_rect" : [ 167.0, 21.0, 15.0, 15.0 ],
					"numinlets" : 0,
					"comment" : "Commands"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Commands",
					"numoutlets" : 0,
					"id" : "obj-4",
					"fontname" : "Geneva",
					"patching_rect" : [ 188.0, 21.0, 100.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Enable tracing",
					"numoutlets" : 0,
					"id" : "obj-5",
					"fontname" : "Geneva",
					"patching_rect" : [ 45.0, 21.0, 70.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "inlet",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-6",
					"patching_rect" : [ 26.0, 21.0, 15.0, 15.0 ],
					"numinlets" : 0,
					"comment" : "Enable tracing"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print",
					"numoutlets" : 0,
					"id" : "obj-7",
					"fontname" : "Geneva",
					"patching_rect" : [ 521.0, 212.0, 30.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "gate",
					"linecount" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-8",
					"fontname" : "Geneva",
					"patching_rect" : [ 532.0, 80.0, 27.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print <ldp-in>",
					"numoutlets" : 0,
					"id" : "obj-9",
					"fontname" : "Geneva",
					"patching_rect" : [ 532.0, 117.0, 69.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "receive go-trace",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-10",
					"fontname" : "Geneva",
					"color" : [ 1.0, 0.541176, 0.843137, 1.0 ],
					"patching_rect" : [ 532.0, 27.0, 84.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "send go-trace",
					"numoutlets" : 0,
					"id" : "obj-11",
					"fontname" : "Geneva",
					"color" : [ 1.0, 0.541176, 0.843137, 1.0 ],
					"patching_rect" : [ 39.0, 66.0, 70.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print <serial>",
					"numoutlets" : 0,
					"id" : "obj-12",
					"fontname" : "Geneva",
					"patching_rect" : [ 1.0, 312.0, 69.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "sprintf %02.2lx",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-13",
					"fontname" : "Geneva",
					"patching_rect" : [ 1.0, 278.0, 81.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Key Mode Status",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-14",
					"fontname" : "Geneva",
					"patching_rect" : [ 159.0, 394.0, 53.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Command Status",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-15",
					"fontname" : "Geneva",
					"patching_rect" : [ 228.0, 394.0, 51.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Program Stop Code Detected",
					"linecount" : 3,
					"numoutlets" : 0,
					"id" : "obj-16",
					"fontname" : "Geneva",
					"patching_rect" : [ 297.0, 394.0, 70.0, 43.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "outlet",
					"numoutlets" : 0,
					"id" : "obj-17",
					"patching_rect" : [ 339.0, 378.0, 15.0, 15.0 ],
					"numinlets" : 1,
					"comment" : "Program Stop Code Detected"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "outlet",
					"numoutlets" : 0,
					"id" : "obj-18",
					"patching_rect" : [ 382.0, 378.0, 15.0, 15.0 ],
					"numinlets" : 1,
					"comment" : "Command Complete"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Command Complete",
					"linecount" : 3,
					"numoutlets" : 0,
					"id" : "obj-19",
					"fontname" : "Geneva",
					"patching_rect" : [ 369.0, 394.0, 50.0, 43.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "outlet",
					"numoutlets" : 0,
					"id" : "obj-20",
					"patching_rect" : [ 256.0, 378.0, 15.0, 15.0 ],
					"numinlets" : 1,
					"comment" : "Command Status"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "gate",
					"linecount" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-21",
					"fontname" : "Geneva",
					"patching_rect" : [ 205.0, 294.0, 27.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print <command>",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-22",
					"fontname" : "Geneva",
					"patching_rect" : [ 205.0, 344.0, 84.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "sprintf %02.2lx",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-23",
					"fontname" : "Geneva",
					"patching_rect" : [ 205.0, 319.0, 81.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "outlet",
					"numoutlets" : 0,
					"id" : "obj-24",
					"patching_rect" : [ 172.0, 378.0, 15.0, 15.0 ],
					"numinlets" : 1,
					"comment" : "Key Mode Status"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "sprintf %02.2lx",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-25",
					"fontname" : "Geneva",
					"patching_rect" : [ 90.0, 324.0, 81.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "gate",
					"linecount" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-26",
					"fontname" : "Geneva",
					"patching_rect" : [ 90.0, 294.0, 27.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print <keymode>",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-27",
					"fontname" : "Geneva",
					"patching_rect" : [ 90.0, 350.0, 82.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "ldp1550 #3 #4 #5",
					"numoutlets" : 10,
					"outlettype" : [ "int", "bang", "int", "int", "bang", "bang", "bang", "int", "int", "bang" ],
					"id" : "obj-28",
					"fontname" : "Geneva",
					"color" : [ 1.0, 0.890196, 0.090196, 1.0 ],
					"patching_rect" : [ 167.0, 85.0, 316.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Polling Interval in msec (0 for default), Polls per Info request (0 for none), Number of buffers in command pool (0 for default)",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-29",
					"fontname" : "Geneva",
					"patching_rect" : [ 180.0, 56.0, 292.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "receive go-trace",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-30",
					"fontname" : "Geneva",
					"color" : [ 1.0, 0.541176, 0.843137, 1.0 ],
					"patching_rect" : [ 142.0, 239.0, 84.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 0
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "serial #1 #2 8 2 1",
					"numoutlets" : 2,
					"outlettype" : [ "int", "" ],
					"id" : "obj-31",
					"fontname" : "Geneva",
					"color" : [ 1.0, 0.890196, 0.090196, 1.0 ],
					"patching_rect" : [ 167.0, 206.0, 93.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "gate",
					"linecount" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-32",
					"fontname" : "Geneva",
					"patching_rect" : [ 1.0, 245.0, 27.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "Command accepted",
					"linecount" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-33",
					"fontname" : "Geneva",
					"patching_rect" : [ 558.0, 234.0, 92.0, 29.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "gate",
					"linecount" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-34",
					"fontname" : "Geneva",
					"patching_rect" : [ 558.0, 212.0, 27.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print",
					"numoutlets" : 0,
					"id" : "obj-35",
					"fontname" : "Geneva",
					"patching_rect" : [ 558.0, 263.0, 30.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "gate",
					"linecount" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-36",
					"fontname" : "Geneva",
					"patching_rect" : [ 521.0, 156.0, 27.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "Command complete",
					"linecount" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-37",
					"fontname" : "Geneva",
					"patching_rect" : [ 521.0, 178.0, 93.0, 29.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Parameters: 1) Port 2) Baud rate 3) Polling interval 4) Polls per info request 5) Number of buffers",
					"linecount" : 5,
					"numoutlets" : 0,
					"id" : "obj-38",
					"fontname" : "Geneva",
					"patching_rect" : [ 122.0, 130.0, 117.0, 67.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Error Detected",
					"linecount" : 3,
					"numoutlets" : 0,
					"id" : "obj-39",
					"fontname" : "Geneva",
					"patching_rect" : [ 573.0, 394.0, 46.0, 43.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "outlet",
					"numoutlets" : 0,
					"id" : "obj-40",
					"patching_rect" : [ 578.0, 378.0, 15.0, 15.0 ],
					"numinlets" : 1,
					"comment" : "Error Detected"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Frame Number",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-41",
					"fontname" : "Geneva",
					"patching_rect" : [ 528.0, 394.0, 42.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Chapter Number",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-42",
					"fontname" : "Geneva",
					"patching_rect" : [ 483.0, 394.0, 44.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "outlet",
					"numoutlets" : 0,
					"id" : "obj-43",
					"patching_rect" : [ 533.0, 378.0, 15.0, 15.0 ],
					"numinlets" : 1,
					"comment" : "Frame Number"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "outlet",
					"numoutlets" : 0,
					"id" : "obj-44",
					"patching_rect" : [ 490.0, 378.0, 15.0, 15.0 ],
					"numinlets" : 1,
					"comment" : "Chapter Number"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "outlet",
					"numoutlets" : 0,
					"id" : "obj-45",
					"patching_rect" : [ 430.0, 378.0, 15.0, 15.0 ],
					"numinlets" : 1,
					"comment" : "Command Accepted"
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Command Accepted",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-46",
					"fontname" : "Geneva",
					"patching_rect" : [ 425.0, 394.0, 52.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "gate",
					"linecount" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-47",
					"fontname" : "Geneva",
					"patching_rect" : [ 506.0, 253.0, 27.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "Program stop detected",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-48",
					"fontname" : "Geneva",
					"patching_rect" : [ 506.0, 284.0, 110.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print",
					"numoutlets" : 0,
					"id" : "obj-49",
					"fontname" : "Geneva",
					"patching_rect" : [ 507.0, 313.0, 30.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "gate",
					"linecount" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-50",
					"fontname" : "Geneva",
					"patching_rect" : [ 26.0, 134.0, 27.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "gate",
					"linecount" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-51",
					"fontname" : "Geneva",
					"patching_rect" : [ 623.0, 147.0, 27.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "Error detected",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-52",
					"fontname" : "Geneva",
					"patching_rect" : [ 623.0, 178.0, 75.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "print",
					"numoutlets" : 0,
					"id" : "obj-53",
					"fontname" : "Geneva",
					"patching_rect" : [ 624.0, 207.0, 30.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-28", 9 ],
					"destination" : [ "obj-51", 1 ],
					"hidden" : 0,
					"midpoints" : [ 473.5, 112.0, 640.5, 112.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-52", 0 ],
					"destination" : [ "obj-53", 0 ],
					"hidden" : 0,
					"midpoints" : [ 632.5, 208.0, 633.5, 208.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-51", 0 ],
					"destination" : [ "obj-52", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 0 ],
					"destination" : [ "obj-51", 0 ],
					"hidden" : 0,
					"midpoints" : [ 541.5, 62.0, 632.5, 62.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-28", 9 ],
					"destination" : [ "obj-40", 0 ],
					"hidden" : 0,
					"midpoints" : [ 473.5, 337.0, 587.0, 337.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-28", 6 ],
					"destination" : [ "obj-34", 1 ],
					"hidden" : 0,
					"midpoints" : [ 374.5, 200.0, 575.5, 200.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-33", 0 ],
					"destination" : [ "obj-35", 0 ],
					"hidden" : 0,
					"midpoints" : [ 567.5, 264.0, 567.5, 264.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-34", 0 ],
					"destination" : [ "obj-33", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 0 ],
					"destination" : [ "obj-34", 0 ],
					"hidden" : 0,
					"midpoints" : [ 541.5, 62.0, 511.0, 62.0, 511.0, 206.0, 567.5, 206.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-8", 1 ],
					"hidden" : 0,
					"midpoints" : [ 176.0, 50.0, 549.5, 50.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-28", 5 ],
					"destination" : [ "obj-36", 1 ],
					"hidden" : 0,
					"midpoints" : [ 341.5, 140.0, 538.5, 140.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-28", 8 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 0,
					"midpoints" : [ 440.5, 343.0, 542.0, 343.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-9", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 0 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-28", 4 ],
					"destination" : [ "obj-47", 1 ],
					"hidden" : 0,
					"midpoints" : [ 308.5, 247.0, 523.5, 247.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-37", 0 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 0,
					"midpoints" : [ 530.5, 213.0, 530.5, 213.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-36", 0 ],
					"destination" : [ "obj-37", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 0 ],
					"destination" : [ "obj-36", 0 ],
					"hidden" : 0,
					"midpoints" : [ 541.5, 62.0, 530.5, 62.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-48", 0 ],
					"destination" : [ "obj-49", 0 ],
					"hidden" : 0,
					"midpoints" : [ 515.5, 314.0, 516.5, 314.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-47", 0 ],
					"destination" : [ "obj-48", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 0 ],
					"destination" : [ "obj-47", 0 ],
					"hidden" : 0,
					"midpoints" : [ 541.5, 62.0, 511.0, 62.0, 511.0, 242.0, 515.5, 242.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-28", 7 ],
					"destination" : [ "obj-44", 0 ],
					"hidden" : 0,
					"midpoints" : [ 407.5, 347.0, 499.0, 347.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-31", 0 ],
					"destination" : [ "obj-28", 1 ],
					"hidden" : 0,
					"midpoints" : [ 176.5, 232.0, 498.0, 232.0, 498.0, 53.0, 473.5, 53.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-28", 6 ],
					"destination" : [ "obj-45", 0 ],
					"hidden" : 0,
					"midpoints" : [ 374.5, 353.0, 439.0, 353.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-28", 5 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [ 341.5, 359.0, 391.0, 359.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-28", 4 ],
					"destination" : [ "obj-17", 0 ],
					"hidden" : 0,
					"midpoints" : [ 308.5, 366.0, 348.0, 366.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-28", 3 ],
					"destination" : [ "obj-20", 0 ],
					"hidden" : 0,
					"midpoints" : [ 275.5, 274.0, 296.0, 274.0, 296.0, 370.0, 265.0, 370.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-28", 3 ],
					"destination" : [ "obj-21", 1 ],
					"hidden" : 0,
					"midpoints" : [ 275.5, 274.0, 222.5, 274.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-23", 0 ],
					"destination" : [ "obj-22", 0 ],
					"hidden" : 0,
					"midpoints" : [ 214.5, 344.0, 214.5, 344.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-21", 0 ],
					"destination" : [ "obj-23", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-30", 0 ],
					"destination" : [ "obj-21", 0 ],
					"hidden" : 0,
					"midpoints" : [ 151.5, 275.0, 214.5, 275.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-28", 2 ],
					"destination" : [ "obj-24", 0 ],
					"hidden" : 0,
					"midpoints" : [ 242.5, 110.0, 266.0, 110.0, 266.0, 264.0, 181.0, 264.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-28", 1 ],
					"destination" : [ "obj-31", 0 ],
					"hidden" : 0,
					"midpoints" : [ 209.5, 121.0, 251.0, 121.0, 251.0, 196.0, 176.5, 196.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-28", 0 ],
					"destination" : [ "obj-31", 0 ],
					"hidden" : 0,
					"midpoints" : [ 176.5, 121.0, 251.0, 121.0, 251.0, 196.0, 176.5, 196.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-28", 0 ],
					"hidden" : 0,
					"midpoints" : [ 176.0, 55.0, 176.5, 55.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-28", 2 ],
					"destination" : [ "obj-26", 1 ],
					"hidden" : 0,
					"midpoints" : [ 242.5, 110.0, 107.5, 110.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-25", 0 ],
					"destination" : [ "obj-27", 0 ],
					"hidden" : 0,
					"midpoints" : [ 99.5, 349.0, 99.5, 349.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-26", 0 ],
					"destination" : [ "obj-25", 0 ],
					"hidden" : 0,
					"midpoints" : [ 99.5, 318.0, 99.5, 318.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-30", 0 ],
					"destination" : [ "obj-26", 0 ],
					"hidden" : 0,
					"midpoints" : [ 151.5, 275.0, 99.5, 275.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-28", 0 ],
					"destination" : [ "obj-50", 1 ],
					"hidden" : 0,
					"midpoints" : [ 176.5, 121.0, 43.5, 121.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 0,
					"midpoints" : [ 35.0, 50.0, 48.5, 50.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-2", 0 ],
					"destination" : [ "obj-1", 0 ],
					"hidden" : 0,
					"midpoints" : [ 35.5, 191.0, 35.5, 191.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-50", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 0,
					"midpoints" : [ 35.5, 151.0, 35.5, 151.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-50", 0 ],
					"hidden" : 0,
					"midpoints" : [ 35.0, 70.0, 35.5, 70.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-31", 0 ],
					"destination" : [ "obj-32", 1 ],
					"hidden" : 0,
					"midpoints" : [ 176.5, 232.0, 18.5, 232.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-13", 0 ],
					"destination" : [ "obj-12", 0 ],
					"hidden" : 0,
					"midpoints" : [ 10.5, 304.0, 10.5, 304.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-32", 0 ],
					"destination" : [ "obj-13", 0 ],
					"hidden" : 0,
					"midpoints" : [ 10.5, 264.0, 10.5, 264.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-32", 0 ],
					"hidden" : 0,
					"midpoints" : [ 35.0, 50.0, 10.5, 50.0 ]
				}

			}
 ]
	}

}
