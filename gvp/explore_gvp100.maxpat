{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 5,
			"minor" : 1,
			"revision" : 9
		}
,
		"rect" : [ 9.0, 51.0, 976.0, 708.0 ],
		"bglocked" : 0,
		"defrect" : [ 9.0, 51.0, 976.0, 708.0 ],
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
					"maxclass" : "comment",
					"text" : "!clip value",
					"numoutlets" : 0,
					"id" : "obj-1",
					"fontname" : "Geneva",
					"patching_rect" : [ 440.0, 185.0, 100.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "s analog_value",
					"numoutlets" : 0,
					"id" : "obj-2",
					"fontname" : "Geneva",
					"color" : [ 1.0, 0.541176, 0.843137, 1.0 ],
					"patching_rect" : [ 715.0, 190.0, 74.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "s analog_value",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-3",
					"fontname" : "Geneva",
					"patching_rect" : [ 556.0, 394.0, 71.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "r analog_value",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-4",
					"fontname" : "Geneva",
					"color" : [ 1.0, 0.541176, 0.843137, 1.0 ],
					"patching_rect" : [ 203.0, 386.0, 75.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 0,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"triangle" : 0,
					"numoutlets" : 2,
					"htextcolor" : [ 0.870588, 0.870588, 0.870588, 1.0 ],
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-5",
					"fontname" : "Geneva",
					"cantchange" : 1,
					"triscale" : 0.9,
					"patching_rect" : [ 203.0, 415.0, 52.0, 33.0 ],
					"fontsize" : 20.0,
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "analog value in decimal",
					"linecount" : 3,
					"numoutlets" : 0,
					"id" : "obj-6",
					"fontname" : "Geneva",
					"patching_rect" : [ 256.0, 412.0, 54.0, 43.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "b 3",
					"numoutlets" : 3,
					"outlettype" : [ "bang", "bang", "bang" ],
					"id" : "obj-7",
					"fontname" : "Geneva",
					"patching_rect" : [ 331.0, 22.0, 50.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "pack 0 0",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-8",
					"fontname" : "Geneva",
					"patching_rect" : [ 321.0, 394.0, 50.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "effectsanalogcontrol $1 $2",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-9",
					"fontname" : "Geneva",
					"patching_rect" : [ 321.0, 422.0, 130.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "dskanalogcontrol $1 $2",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-10",
					"fontname" : "Geneva",
					"patching_rect" : [ 495.0, 422.0, 114.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "pack 0 0",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-11",
					"fontname" : "Geneva",
					"patching_rect" : [ 495.0, 394.0, 50.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "crosspoint insert $1",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-12",
					"fontname" : "Geneva",
					"patching_rect" : [ 133.0, 592.0, 98.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "crosspoint pgm $1",
					"linecount" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-13",
					"fontname" : "Geneva",
					"patching_rect" : [ 25.0, 592.0, 88.0, 29.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "wipepattern $1",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-14",
					"fontname" : "Geneva",
					"patching_rect" : [ 36.0, 453.0, 77.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numoutlets" : 2,
					"htextcolor" : [ 0.870588, 0.870588, 0.870588, 1.0 ],
					"format" : 1,
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-15",
					"fontname" : "Geneva",
					"triscale" : 0.9,
					"mouseup" : 1,
					"minimum" : 0,
					"patching_rect" : [ 147.0, 288.0, 35.0, 19.0 ],
					"fontsize" : 9.0,
					"maximum" : 255,
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"id" : "obj-16",
					"fgcolor" : [ 0.156863, 0.8, 0.54902, 1.0 ],
					"patching_rect" : [ 147.0, 265.0, 15.0, 15.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "off $1",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-17",
					"fontname" : "Geneva",
					"patching_rect" : [ 147.0, 325.0, 41.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "transitionrate f2b $1 background",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-18",
					"fontname" : "Geneva",
					"patching_rect" : [ 795.0, 210.0, 157.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "transitionrate auto $1 key",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-19",
					"fontname" : "Geneva",
					"patching_rect" : [ 661.0, 210.0, 128.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "push $1",
					"linecount" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-20",
					"fontname" : "Geneva",
					"patching_rect" : [ 87.0, 325.0, 41.0, 29.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "on $1",
					"linecount" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-21",
					"fontname" : "Geneva",
					"patching_rect" : [ 19.0, 325.0, 33.0, 29.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "loadbang",
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"id" : "obj-22",
					"fontname" : "Geneva",
					"color" : [ 1.0, 0.360784, 0.682353, 1.0 ],
					"patching_rect" : [ 255.0, 22.0, 50.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "analog value",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-23",
					"fontname" : "Geneva",
					"patching_rect" : [ 687.0, 131.0, 40.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"id" : "obj-24",
					"fgcolor" : [ 0.156863, 0.8, 0.54902, 1.0 ],
					"patching_rect" : [ 661.0, 133.0, 15.0, 15.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numoutlets" : 2,
					"htextcolor" : [ 0.870588, 0.870588, 0.870588, 1.0 ],
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-25",
					"fontname" : "Geneva",
					"triscale" : 0.9,
					"mouseup" : 1,
					"minimum" : 0,
					"patching_rect" : [ 661.0, 162.0, 50.0, 19.0 ],
					"fontsize" : 9.0,
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "analog value",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-26",
					"fontname" : "Geneva",
					"patching_rect" : [ 818.0, 131.0, 40.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numoutlets" : 2,
					"htextcolor" : [ 0.870588, 0.870588, 0.870588, 1.0 ],
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-27",
					"fontname" : "Geneva",
					"triscale" : 0.9,
					"mouseup" : 1,
					"minimum" : 0,
					"patching_rect" : [ 795.0, 162.0, 50.0, 19.0 ],
					"fontsize" : 9.0,
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "s 2gvp",
					"numoutlets" : 0,
					"id" : "obj-28",
					"fontname" : "Geneva",
					"color" : [ 1.0, 0.541176, 0.843137, 1.0 ],
					"patching_rect" : [ 730.0, 274.0, 38.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"id" : "obj-29",
					"fgcolor" : [ 0.156863, 0.8, 0.54902, 1.0 ],
					"patching_rect" : [ 795.0, 133.0, 15.0, 15.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numoutlets" : 2,
					"htextcolor" : [ 0.870588, 0.870588, 0.870588, 1.0 ],
					"format" : 1,
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-30",
					"fontname" : "Geneva",
					"triscale" : 0.9,
					"mouseup" : 1,
					"minimum" : 0,
					"patching_rect" : [ 133.0, 553.0, 35.0, 19.0 ],
					"fontsize" : 9.0,
					"maximum" : 255,
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"id" : "obj-31",
					"fgcolor" : [ 0.156863, 0.8, 0.54902, 1.0 ],
					"patching_rect" : [ 133.0, 530.0, 15.0, 15.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numoutlets" : 2,
					"htextcolor" : [ 0.870588, 0.870588, 0.870588, 1.0 ],
					"format" : 1,
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-32",
					"fontname" : "Geneva",
					"triscale" : 0.9,
					"mouseup" : 1,
					"minimum" : 0,
					"patching_rect" : [ 25.0, 553.0, 35.0, 19.0 ],
					"fontsize" : 9.0,
					"maximum" : 255,
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"id" : "obj-33",
					"fgcolor" : [ 0.156863, 0.8, 0.54902, 1.0 ],
					"patching_rect" : [ 25.0, 530.0, 15.0, 15.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "s 2gvp",
					"numoutlets" : 0,
					"id" : "obj-34",
					"fontname" : "Geneva",
					"color" : [ 1.0, 0.541176, 0.843137, 1.0 ],
					"patching_rect" : [ 68.0, 634.0, 38.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "s sel_no",
					"numoutlets" : 0,
					"id" : "obj-35",
					"fontname" : "Geneva",
					"color" : [ 1.0, 0.541176, 0.843137, 1.0 ],
					"patching_rect" : [ 119.0, 634.0, 45.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"id" : "obj-36",
					"fgcolor" : [ 0.156863, 0.8, 0.54902, 1.0 ],
					"patching_rect" : [ 418.0, 313.0, 15.0, 15.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "s sel_no",
					"numoutlets" : 0,
					"id" : "obj-37",
					"fontname" : "Geneva",
					"patching_rect" : [ 417.0, 394.0, 45.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "s 2gvp",
					"numoutlets" : 0,
					"id" : "obj-38",
					"fontname" : "Geneva",
					"color" : [ 1.0, 0.541176, 0.843137, 1.0 ],
					"patching_rect" : [ 382.0, 468.0, 38.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "s sel_no",
					"numoutlets" : 0,
					"id" : "obj-39",
					"fontname" : "Geneva",
					"color" : [ 1.0, 0.541176, 0.843137, 1.0 ],
					"patching_rect" : [ 117.0, 499.0, 45.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "s 2gvp",
					"numoutlets" : 0,
					"id" : "obj-40",
					"fontname" : "Geneva",
					"color" : [ 1.0, 0.541176, 0.843137, 1.0 ],
					"patching_rect" : [ 36.0, 499.0, 38.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"id" : "obj-41",
					"fgcolor" : [ 0.156863, 0.8, 0.54902, 1.0 ],
					"patching_rect" : [ 36.0, 397.0, 15.0, 15.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numoutlets" : 2,
					"htextcolor" : [ 0.870588, 0.870588, 0.870588, 1.0 ],
					"format" : 1,
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-42",
					"fontname" : "Geneva",
					"triscale" : 0.9,
					"mouseup" : 1,
					"minimum" : 0,
					"patching_rect" : [ 36.0, 420.0, 35.0, 19.0 ],
					"fontsize" : 9.0,
					"maximum" : 255,
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "s sel_no",
					"numoutlets" : 0,
					"id" : "obj-43",
					"fontname" : "Geneva",
					"color" : [ 1.0, 0.541176, 0.843137, 1.0 ],
					"patching_rect" : [ 132.0, 369.0, 45.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "selection number in decimal",
					"linecount" : 3,
					"numoutlets" : 0,
					"id" : "obj-44",
					"fontname" : "Geneva",
					"patching_rect" : [ 257.0, 342.0, 54.0, 43.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"triangle" : 0,
					"numoutlets" : 2,
					"htextcolor" : [ 0.870588, 0.870588, 0.870588, 1.0 ],
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-45",
					"fontname" : "Geneva",
					"cantchange" : 1,
					"triscale" : 0.9,
					"patching_rect" : [ 204.0, 345.0, 52.0, 33.0 ],
					"fontsize" : 20.0,
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "r sel_no",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-46",
					"fontname" : "Geneva",
					"color" : [ 1.0, 0.541176, 0.843137, 1.0 ],
					"patching_rect" : [ 204.0, 316.0, 45.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 0,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "s 2gvp",
					"numoutlets" : 0,
					"id" : "obj-47",
					"fontname" : "Geneva",
					"color" : [ 1.0, 0.541176, 0.843137, 1.0 ],
					"patching_rect" : [ 62.0, 369.0, 38.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "r 2gvp",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-48",
					"fontname" : "Geneva",
					"color" : [ 1.0, 0.541176, 0.843137, 1.0 ],
					"patching_rect" : [ 340.0, 491.0, 38.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 0,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "toggle",
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"id" : "obj-49",
					"patching_rect" : [ 237.0, 491.0, 15.0, 15.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "gvp_wrapper 48 38400",
					"numoutlets" : 3,
					"outlettype" : [ "", "", "" ],
					"id" : "obj-50",
					"fontname" : "Geneva",
					"color" : [ 0.290196, 0.611765, 0.380392, 1.0 ],
					"patching_rect" : [ 237.0, 537.0, 113.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "verbose off",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-51",
					"fontname" : "Geneva",
					"patching_rect" : [ 539.0, 44.0, 60.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "verbose on",
					"linecount" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-52",
					"fontname" : "Geneva",
					"patching_rect" : [ 605.0, 44.0, 55.0, 29.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numoutlets" : 2,
					"htextcolor" : [ 0.870588, 0.870588, 0.870588, 1.0 ],
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-53",
					"fontname" : "Geneva",
					"triscale" : 0.9,
					"minimum" : 0,
					"patching_rect" : [ 418.0, 343.0, 50.0, 19.0 ],
					"fontsize" : 9.0,
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"id" : "obj-54",
					"fgcolor" : [ 0.156863, 0.8, 0.54902, 1.0 ],
					"patching_rect" : [ 19.0, 265.0, 15.0, 15.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numoutlets" : 2,
					"htextcolor" : [ 0.870588, 0.870588, 0.870588, 1.0 ],
					"format" : 1,
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-55",
					"fontname" : "Geneva",
					"triscale" : 0.9,
					"mouseup" : 1,
					"minimum" : 0,
					"patching_rect" : [ 19.0, 288.0, 35.0, 19.0 ],
					"fontsize" : 9.0,
					"maximum" : 255,
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"id" : "obj-56",
					"fgcolor" : [ 0.156863, 0.8, 0.54902, 1.0 ],
					"patching_rect" : [ 321.0, 313.0, 15.0, 15.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numoutlets" : 2,
					"htextcolor" : [ 0.870588, 0.870588, 0.870588, 1.0 ],
					"format" : 1,
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-57",
					"fontname" : "Geneva",
					"triscale" : 0.9,
					"mouseup" : 1,
					"minimum" : 0,
					"patching_rect" : [ 321.0, 343.0, 35.0, 19.0 ],
					"fontsize" : 9.0,
					"maximum" : 255,
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"id" : "obj-58",
					"fgcolor" : [ 0.156863, 0.8, 0.54902, 1.0 ],
					"patching_rect" : [ 495.0, 313.0, 15.0, 15.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numoutlets" : 2,
					"htextcolor" : [ 0.870588, 0.870588, 0.870588, 1.0 ],
					"format" : 1,
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-59",
					"fontname" : "Geneva",
					"triscale" : 0.9,
					"mouseup" : 1,
					"minimum" : 0,
					"patching_rect" : [ 495.0, 343.0, 35.0, 19.0 ],
					"fontsize" : 9.0,
					"maximum" : 255,
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "analog value",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-60",
					"fontname" : "Geneva",
					"patching_rect" : [ 442.0, 308.0, 40.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "learn-emem 5",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-61",
					"fontname" : "Geneva",
					"patching_rect" : [ 487.0, 134.0, 71.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "recall-emem 5",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-62",
					"fontname" : "Geneva",
					"patching_rect" : [ 409.0, 134.0, 74.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "endsequence",
					"linecount" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-63",
					"fontname" : "Geneva",
					"patching_rect" : [ 567.0, 135.0, 63.0, 29.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "transitionmode key",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-64",
					"fontname" : "Geneva",
					"patching_rect" : [ 536.0, 86.0, 94.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "transitionmode background",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-65",
					"fontname" : "Geneva",
					"patching_rect" : [ 404.0, 86.0, 128.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "xreset",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-66",
					"fontname" : "Geneva",
					"patching_rect" : [ 417.0, 44.0, 38.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "allstop",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-67",
					"fontname" : "Geneva",
					"patching_rect" : [ 461.0, 44.0, 38.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "'auto' can be abbreviated to 'a';\r'background' can be abbreviated to 'b';\r'doit' and 'dsk' can be abbreviated to 'd';\r'f2b' can be abbreviated 'f';\r'insert' can be abbreviated to 'i';\r'key' can be abbreviated to 'k';\r'program' can be abbreviated to 'pgm';\r'preset' and 'preview' are equivalent;\r'key' and 'insert' are equivalent.",
					"linecount" : 9,
					"numoutlets" : 0,
					"id" : "obj-68",
					"fontname" : "Geneva",
					"patching_rect" : [ 212.0, 203.0, 192.0, 115.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "• endsequence",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-69",
					"fontname" : "Geneva",
					"patching_rect" : [ 1.0, 116.0, 71.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "• allstop",
					"numoutlets" : 0,
					"id" : "obj-70",
					"fontname" : "Geneva",
					"patching_rect" : [ 1.0, 44.0, 48.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "commands",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-71",
					"fontname" : "Geneva",
					"patching_rect" : [ 1.0, 31.0, 54.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "• crosspoint program/preset/key [crosspoint#] [program/preset/key [crosspoint#]] …",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-72",
					"fontname" : "Geneva",
					"patching_rect" : [ 1.0, 56.0, 228.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "• dskanalogcontrol control [value] [control [value]] …",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-73",
					"fontname" : "Geneva",
					"patching_rect" : [ 1.0, 80.0, 175.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "• effectsanalogcontrol control [value] …",
					"numoutlets" : 0,
					"id" : "obj-74",
					"fontname" : "Geneva",
					"patching_rect" : [ 1.0, 104.0, 188.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "• learn-emem [register]",
					"numoutlets" : 0,
					"id" : "obj-75",
					"fontname" : "Geneva",
					"patching_rect" : [ 1.0, 128.0, 120.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "• on/off/push [control] [control] …",
					"numoutlets" : 0,
					"id" : "obj-76",
					"fontname" : "Geneva",
					"patching_rect" : [ 1.0, 140.0, 171.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "• recall-emem [register]",
					"numoutlets" : 0,
					"id" : "obj-77",
					"fontname" : "Geneva",
					"patching_rect" : [ 1.0, 152.0, 120.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "• transitionmode key/bkgd [key/bkgd]",
					"numoutlets" : 0,
					"id" : "obj-78",
					"fontname" : "Geneva",
					"patching_rect" : [ 1.0, 164.0, 181.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "• transitionrate auto/dsk/f2b [rate] [key/bkgd/doit] [key/bkgd/doit] [key/bkgd/doit]",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-79",
					"fontname" : "Geneva",
					"patching_rect" : [ 1.0, 176.0, 254.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "• wipepattern [pattern]",
					"numoutlets" : 0,
					"id" : "obj-80",
					"fontname" : "Geneva",
					"patching_rect" : [ 1.0, 200.0, 126.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "• xreset",
					"numoutlets" : 0,
					"id" : "obj-81",
					"fontname" : "Geneva",
					"patching_rect" : [ 1.0, 212.0, 47.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"id" : "obj-82",
					"fgcolor" : [ 0.156863, 0.8, 0.54902, 1.0 ],
					"patching_rect" : [ 87.0, 265.0, 15.0, 15.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "number",
					"numoutlets" : 2,
					"htextcolor" : [ 0.870588, 0.870588, 0.870588, 1.0 ],
					"format" : 1,
					"outlettype" : [ "int", "bang" ],
					"id" : "obj-83",
					"fontname" : "Geneva",
					"triscale" : 0.9,
					"mouseup" : 1,
					"minimum" : 0,
					"patching_rect" : [ 87.0, 288.0, 35.0, 19.0 ],
					"fontsize" : 9.0,
					"maximum" : 255,
					"bgcolor" : [ 0.866667, 0.866667, 0.866667, 1.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "s 2gvp",
					"numoutlets" : 0,
					"id" : "obj-84",
					"fontname" : "Geneva",
					"color" : [ 1.0, 0.541176, 0.843137, 1.0 ],
					"patching_rect" : [ 395.0, 188.0, 38.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "button",
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"id" : "obj-85",
					"fgcolor" : [ 0.156863, 0.8, 0.54902, 1.0 ],
					"patching_rect" : [ 339.0, 611.0, 30.0, 30.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "set up:;\rgvp_wrapper select_address baud_rate [poll_rate]",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-86",
					"fontname" : "Geneva",
					"patching_rect" : [ 356.0, 533.0, 240.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "c = crosspoint;\rd = dskanalogcontrol;\re = effectsanalogcontrol;\rp = push;\rm = transitionmode;\rr = transitionrate;\rw = wipepattern;\rx = xreset",
					"linecount" : 8,
					"numoutlets" : 0,
					"id" : "obj-87",
					"fontname" : "Geneva",
					"patching_rect" : [ 234.0, 61.0, 122.0, 103.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "special commands",
					"numoutlets" : 0,
					"id" : "obj-88",
					"fontname" : "Geneva",
					"patching_rect" : [ 440.0, 173.0, 100.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "!effects value",
					"numoutlets" : 0,
					"id" : "obj-89",
					"fontname" : "Geneva",
					"patching_rect" : [ 440.0, 197.0, 100.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "!joystick horiz vert",
					"numoutlets" : 0,
					"id" : "obj-90",
					"fontname" : "Geneva",
					"patching_rect" : [ 440.0, 209.0, 100.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "!take value",
					"numoutlets" : 0,
					"id" : "obj-91",
					"fontname" : "Geneva",
					"patching_rect" : [ 440.0, 221.0, 100.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "'!clip' can be abbreviated '!c';\r'!effects' can be abbreviated '!e';\r'!joystick' can be abbreviated '!j';\r'!take' can be abbreviated '!t'",
					"linecount" : 4,
					"numoutlets" : 0,
					"id" : "obj-92",
					"fontname" : "Geneva",
					"patching_rect" : [ 440.0, 242.0, 162.0, 55.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "ERROR",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-93",
					"fontname" : "Arial",
					"patching_rect" : [ 372.0, 617.0, 46.0, 34.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-27", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 0,
					"midpoints" : [ 804.5, 190.0, 804.5, 190.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-29", 0 ],
					"destination" : [ "obj-27", 0 ],
					"hidden" : 0,
					"midpoints" : [ 804.0, 154.0, 804.5, 154.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 0 ],
					"destination" : [ "obj-28", 0 ],
					"hidden" : 1,
					"midpoints" : [ 670.5, 243.0, 739.5, 243.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-28", 0 ],
					"hidden" : 1,
					"midpoints" : [ 804.5, 243.0, 739.5, 243.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-25", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 1,
					"midpoints" : [ 670.5, 183.0, 724.5, 183.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-27", 0 ],
					"destination" : [ "obj-2", 0 ],
					"hidden" : 1,
					"midpoints" : [ 804.5, 183.0, 724.5, 183.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-25", 0 ],
					"destination" : [ "obj-19", 0 ],
					"hidden" : 0,
					"midpoints" : [ 670.5, 199.0, 670.5, 199.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-24", 0 ],
					"destination" : [ "obj-25", 0 ],
					"hidden" : 0,
					"midpoints" : [ 670.0, 153.0, 670.5, 153.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-53", 0 ],
					"destination" : [ "obj-3", 0 ],
					"hidden" : 1,
					"midpoints" : [ 427.5, 368.0, 565.5, 368.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-7", 2 ],
					"destination" : [ "obj-51", 0 ],
					"hidden" : 1,
					"midpoints" : [ 371.5, 47.0, 398.0, 47.0, 398.0, 24.0, 548.5, 24.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-53", 0 ],
					"destination" : [ "obj-11", 1 ],
					"hidden" : 0,
					"midpoints" : [ 427.5, 368.0, 535.5, 368.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-10", 0 ],
					"hidden" : 0,
					"midpoints" : [ 504.5, 422.0, 504.5, 422.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-59", 0 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 0,
					"midpoints" : [ 504.5, 379.0, 504.5, 379.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-58", 0 ],
					"destination" : [ "obj-59", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-7", 0 ],
					"destination" : [ "obj-67", 0 ],
					"hidden" : 1,
					"midpoints" : [ 340.5, 54.0, 404.0, 54.0, 404.0, 29.0, 470.5, 29.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-36", 0 ],
					"destination" : [ "obj-53", 0 ],
					"hidden" : 0,
					"midpoints" : [ 427.0, 329.0, 427.5, 329.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-57", 0 ],
					"destination" : [ "obj-37", 0 ],
					"hidden" : 1,
					"midpoints" : [ 330.5, 379.0, 426.5, 379.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-59", 0 ],
					"destination" : [ "obj-37", 0 ],
					"hidden" : 1,
					"midpoints" : [ 504.5, 379.0, 426.5, 379.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-7", 1 ],
					"destination" : [ "obj-66", 0 ],
					"hidden" : 1,
					"midpoints" : [ 356.0, 50.0, 410.0, 50.0, 410.0, 34.0, 426.5, 34.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-51", 0 ],
					"destination" : [ "obj-84", 0 ],
					"hidden" : 1,
					"midpoints" : [ 548.5, 76.0, 404.5, 76.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-52", 0 ],
					"destination" : [ "obj-84", 0 ],
					"hidden" : 1,
					"midpoints" : [ 614.5, 76.0, 404.5, 76.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-61", 0 ],
					"destination" : [ "obj-84", 0 ],
					"hidden" : 1,
					"midpoints" : [ 496.5, 167.0, 404.5, 167.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-62", 0 ],
					"destination" : [ "obj-84", 0 ],
					"hidden" : 1,
					"midpoints" : [ 418.5, 167.0, 404.5, 167.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-63", 0 ],
					"destination" : [ "obj-84", 0 ],
					"hidden" : 1,
					"midpoints" : [ 576.5, 167.0, 404.5, 167.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-64", 0 ],
					"destination" : [ "obj-84", 0 ],
					"hidden" : 1,
					"midpoints" : [ 545.5, 126.0, 404.5, 126.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-65", 0 ],
					"destination" : [ "obj-84", 0 ],
					"hidden" : 1,
					"midpoints" : [ 413.5, 126.0, 404.5, 126.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-66", 0 ],
					"destination" : [ "obj-84", 0 ],
					"hidden" : 1,
					"midpoints" : [ 426.5, 76.0, 404.5, 76.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-67", 0 ],
					"destination" : [ "obj-84", 0 ],
					"hidden" : 1,
					"midpoints" : [ 470.5, 76.0, 404.5, 76.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-9", 0 ],
					"destination" : [ "obj-38", 0 ],
					"hidden" : 1,
					"midpoints" : [ 330.5, 453.0, 391.5, 453.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-10", 0 ],
					"destination" : [ "obj-38", 0 ],
					"hidden" : 1,
					"midpoints" : [ 504.5, 453.0, 391.5, 453.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-53", 0 ],
					"destination" : [ "obj-8", 1 ],
					"hidden" : 0,
					"midpoints" : [ 427.5, 368.0, 361.5, 368.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-48", 0 ],
					"destination" : [ "obj-50", 1 ],
					"hidden" : 1,
					"midpoints" : [ 349.5, 518.0, 340.5, 518.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-50", 2 ],
					"destination" : [ "obj-85", 0 ],
					"hidden" : 0,
					"midpoints" : [ 340.5, 595.0, 348.5, 595.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-22", 0 ],
					"destination" : [ "obj-7", 0 ],
					"hidden" : 1,
					"midpoints" : [ 264.5, 46.0, 319.0, 46.0, 319.0, 18.0, 340.5, 18.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-9", 0 ],
					"hidden" : 0,
					"midpoints" : [ 330.5, 417.0, 330.5, 417.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-57", 0 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 0,
					"midpoints" : [ 330.5, 379.0, 330.5, 379.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-56", 0 ],
					"destination" : [ "obj-57", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-49", 0 ],
					"destination" : [ "obj-50", 0 ],
					"hidden" : 0,
					"midpoints" : [ 246.0, 523.0, 246.5, 523.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-46", 0 ],
					"destination" : [ "obj-45", 0 ],
					"hidden" : 1,
					"midpoints" : [ 213.5, 344.0, 213.5, 344.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-4", 0 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 1,
					"midpoints" : [ 212.5, 414.0, 212.5, 414.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-17", 0 ],
					"hidden" : 0,
					"midpoints" : [ 156.5, 315.0, 156.5, 315.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-30", 0 ],
					"destination" : [ "obj-12", 0 ],
					"hidden" : 0,
					"midpoints" : [ 142.5, 579.0, 142.5, 579.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-31", 0 ],
					"destination" : [ "obj-30", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-15", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 1,
					"midpoints" : [ 156.5, 314.0, 141.5, 314.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-55", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 1,
					"midpoints" : [ 28.5, 314.0, 141.5, 314.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-83", 0 ],
					"destination" : [ "obj-43", 0 ],
					"hidden" : 1,
					"midpoints" : [ 96.5, 314.0, 141.5, 314.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-30", 0 ],
					"destination" : [ "obj-35", 0 ],
					"hidden" : 1,
					"midpoints" : [ 142.5, 579.0, 128.5, 579.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-32", 0 ],
					"destination" : [ "obj-35", 0 ],
					"hidden" : 1,
					"midpoints" : [ 34.5, 579.0, 128.5, 579.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-42", 0 ],
					"destination" : [ "obj-39", 0 ],
					"hidden" : 1,
					"midpoints" : [ 45.5, 445.0, 126.5, 445.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-83", 0 ],
					"destination" : [ "obj-20", 0 ],
					"hidden" : 0,
					"midpoints" : [ 96.5, 315.0, 96.5, 315.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-82", 0 ],
					"destination" : [ "obj-83", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-12", 0 ],
					"destination" : [ "obj-34", 0 ],
					"hidden" : 1,
					"midpoints" : [ 142.5, 617.0, 77.5, 617.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-13", 0 ],
					"destination" : [ "obj-34", 0 ],
					"hidden" : 1,
					"midpoints" : [ 34.5, 617.0, 77.5, 617.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-47", 0 ],
					"hidden" : 1,
					"midpoints" : [ 156.5, 352.0, 71.5, 352.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-20", 0 ],
					"destination" : [ "obj-47", 0 ],
					"hidden" : 1,
					"midpoints" : [ 96.5, 352.0, 71.5, 352.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-21", 0 ],
					"destination" : [ "obj-47", 0 ],
					"hidden" : 1,
					"midpoints" : [ 28.5, 352.0, 71.5, 352.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-14", 0 ],
					"destination" : [ "obj-40", 0 ],
					"hidden" : 1,
					"midpoints" : [ 45.5, 495.0, 45.5, 495.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-42", 0 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 0,
					"midpoints" : [ 45.5, 448.0, 45.5, 448.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-41", 0 ],
					"destination" : [ "obj-42", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-32", 0 ],
					"destination" : [ "obj-13", 0 ],
					"hidden" : 0,
					"midpoints" : [ 34.5, 583.0, 34.5, 583.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-33", 0 ],
					"destination" : [ "obj-32", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-55", 0 ],
					"destination" : [ "obj-21", 0 ],
					"hidden" : 0,
					"midpoints" : [ 28.5, 314.0, 28.5, 314.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-54", 0 ],
					"destination" : [ "obj-55", 0 ],
					"hidden" : 0,
					"midpoints" : [  ]
				}

			}
 ]
	}

}
