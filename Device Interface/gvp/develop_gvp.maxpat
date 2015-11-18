{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 5,
			"minor" : 1,
			"revision" : 9
		}
,
		"rect" : [ 231.0, 181.0, 716.0, 563.0 ],
		"bglocked" : 0,
		"defrect" : [ 231.0, 181.0, 716.0, 563.0 ],
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
					"text" : "DSK Clip Up",
					"linecount" : 3,
					"numoutlets" : 0,
					"id" : "obj-1",
					"fontname" : "Geneva",
					"patching_rect" : [ 601.0, 283.0, 40.0, 47.0 ],
					"fontsize" : 10.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "DSK Clip Down",
					"linecount" : 3,
					"numoutlets" : 0,
					"id" : "obj-2",
					"fontname" : "Geneva",
					"patching_rect" : [ 546.0, 283.0, 36.0, 47.0 ],
					"fontsize" : 10.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ubutton",
					"numoutlets" : 4,
					"outlettype" : [ "bang", "bang", "", "int" ],
					"id" : "obj-3",
					"handoff" : "",
					"patching_rect" : [ 601.0, 283.0, 39.0, 39.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ubutton",
					"numoutlets" : 4,
					"outlettype" : [ "bang", "bang", "", "int" ],
					"id" : "obj-4",
					"handoff" : "",
					"patching_rect" : [ 544.0, 283.0, 37.0, 38.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "0, 201 2000",
					"linecount" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-5",
					"fontname" : "Geneva",
					"patching_rect" : [ 601.0, 344.0, 39.0, 29.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "201, 0 1000",
					"linecount" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-6",
					"fontname" : "Geneva",
					"patching_rect" : [ 557.0, 344.0, 39.0, 29.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "DSK Clip Level",
					"numoutlets" : 0,
					"id" : "obj-7",
					"fontname" : "Geneva",
					"patching_rect" : [ 534.0, 260.0, 110.0, 25.0 ],
					"fontsize" : 14.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "slider",
					"orientation" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-8",
					"size" : 201.0,
					"patching_rect" : [ 648.0, 271.0, 18.0, 144.0 ],
					"bgcolor" : [ 0.760784, 0.709804, 0.811765, 1.0 ],
					"mult" : 5.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "/ 10.",
					"numoutlets" : 1,
					"outlettype" : [ "float" ],
					"id" : "obj-9",
					"fontname" : "Geneva",
					"patching_rect" : [ 648.0, 427.0, 34.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : ";\r2gvp !c $1",
					"linecount" : 4,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-10",
					"fontname" : "Geneva",
					"patching_rect" : [ 648.0, 461.0, 30.0, 53.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "line 30",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"id" : "obj-11",
					"fontname" : "Geneva",
					"patching_rect" : [ 557.0, 399.0, 40.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 3,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "s 2gvp",
					"numoutlets" : 0,
					"id" : "obj-12",
					"fontname" : "Geneva",
					"color" : [ 1.0, 0.541176, 0.843137, 1.0 ],
					"patching_rect" : [ 210.0, 446.0, 38.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "line 20",
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"id" : "obj-13",
					"fontname" : "Geneva",
					"patching_rect" : [ 336.0, 401.0, 50.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 3,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : ";\r2gvp !t $1",
					"linecount" : 3,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-14",
					"fontname" : "Geneva",
					"patching_rect" : [ 478.0, 463.0, 35.0, 41.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : ";\r2gvp !e $1",
					"linecount" : 3,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-15",
					"fontname" : "Geneva",
					"patching_rect" : [ 287.0, 466.0, 34.0, 41.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "/ 10.",
					"numoutlets" : 1,
					"outlettype" : [ "float" ],
					"id" : "obj-16",
					"fontname" : "Geneva",
					"patching_rect" : [ 478.0, 429.0, 34.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "/ 10.",
					"numoutlets" : 1,
					"outlettype" : [ "float" ],
					"id" : "obj-17",
					"fontname" : "Geneva",
					"patching_rect" : [ 287.0, 433.0, 38.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "slider",
					"orientation" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-18",
					"size" : 201.0,
					"patching_rect" : [ 478.0, 273.0, 18.0, 144.0 ],
					"bgcolor" : [ 1.0, 0.611765, 0.611765, 1.0 ],
					"mult" : 5.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "slider",
					"orientation" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-19",
					"size" : 201.0,
					"patching_rect" : [ 287.0, 273.0, 18.0, 144.0 ],
					"bgcolor" : [ 0.8, 0.611765, 0.380392, 1.0 ],
					"mult" : -5.0,
					"min" : 1000.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "!j 45.5 50.7",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-20",
					"fontname" : "Geneva",
					"patching_rect" : [ 32.0, 105.0, 92.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "v on",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-21",
					"fontname" : "Geneva",
					"patching_rect" : [ 137.0, 360.0, 28.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : ";\r2gvp p 74",
					"linecount" : 3,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-22",
					"fontname" : "Geneva",
					"patching_rect" : [ 396.0, 167.0, 35.0, 41.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Effects Arm",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-23",
					"fontname" : "Geneva",
					"patching_rect" : [ 228.0, 308.0, 58.0, 44.0 ],
					"fontsize" : 14.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "e 21 500",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-24",
					"fontname" : "Geneva",
					"patching_rect" : [ 225.0, 348.0, 51.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "e 21 1000",
					"linecount" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-25",
					"fontname" : "Geneva",
					"patching_rect" : [ 225.0, 368.0, 55.0, 29.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "e 21 1500",
					"linecount" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-26",
					"fontname" : "Geneva",
					"patching_rect" : [ 225.0, 387.0, 54.0, 29.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "e 21 2000",
					"linecount" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-27",
					"fontname" : "Geneva",
					"patching_rect" : [ 225.0, 407.0, 54.0, 29.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
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
					"patching_rect" : [ 399.0, 462.0, 38.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "e 11 2000",
					"linecount" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-29",
					"fontname" : "Geneva",
					"patching_rect" : [ 422.0, 425.0, 53.0, 29.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "e 11 1500",
					"linecount" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-30",
					"fontname" : "Geneva",
					"patching_rect" : [ 422.0, 404.0, 53.0, 29.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "e 11 1000",
					"linecount" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-31",
					"fontname" : "Geneva",
					"patching_rect" : [ 422.0, 384.0, 52.0, 29.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "e 11 500",
					"linecount" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-32",
					"fontname" : "Geneva",
					"patching_rect" : [ 422.0, 364.0, 48.0, 29.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "d 8 1600 10 50 11 240 12 200",
					"linecount" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-33",
					"fontname" : "Geneva",
					"patching_rect" : [ 28.0, 192.0, 150.0, 29.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "v off",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-34",
					"fontname" : "Geneva",
					"patching_rect" : [ 137.0, 339.0, 31.0, 17.0 ],
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
					"id" : "obj-35",
					"fontname" : "Geneva",
					"patching_rect" : [ 137.0, 294.0, 38.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "x",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-36",
					"fontname" : "Geneva",
					"patching_rect" : [ 137.0, 316.0, 16.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "w 3",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-37",
					"fontname" : "Geneva",
					"patching_rect" : [ 170.0, 23.0, 26.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : ";\r2gvp p 12",
					"linecount" : 3,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-38",
					"fontname" : "Geneva",
					"patching_rect" : [ 87.0, 430.0, 35.0, 41.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "r a 30 d",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-39",
					"fontname" : "Geneva",
					"patching_rect" : [ 508.0, 215.0, 44.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "r f 60 d",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-40",
					"fontname" : "Geneva",
					"patching_rect" : [ 418.0, 215.0, 43.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "r d 50 d",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-41",
					"fontname" : "Geneva",
					"patching_rect" : [ 331.0, 215.0, 45.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "r d 50",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-42",
					"fontname" : "Geneva",
					"patching_rect" : [ 288.0, 215.0, 41.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "r f 60",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-43",
					"fontname" : "Geneva",
					"patching_rect" : [ 378.0, 215.0, 38.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "r a 30",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-44",
					"fontname" : "Geneva",
					"patching_rect" : [ 466.0, 215.0, 38.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "m b",
					"linecount" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-45",
					"fontname" : "Geneva",
					"patching_rect" : [ 196.0, 215.0, 24.0, 29.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "m k",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-46",
					"fontname" : "Geneva",
					"patching_rect" : [ 223.0, 215.0, 25.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : ";\r2gvp p 78",
					"linecount" : 3,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-47",
					"fontname" : "Geneva",
					"patching_rect" : [ 507.0, 167.0, 36.0, 41.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : ";\r2gvp p 76",
					"linecount" : 3,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-48",
					"fontname" : "Geneva",
					"patching_rect" : [ 545.0, 167.0, 35.0, 41.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "set key - Chroma",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-49",
					"fontname" : "Geneva",
					"patching_rect" : [ 465.0, 97.0, 49.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "r a 30 k",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-50",
					"fontname" : "Geneva",
					"patching_rect" : [ 149.0, 215.0, 44.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "DSK cut - DSK",
					"linecount" : 3,
					"numoutlets" : 0,
					"id" : "obj-51",
					"fontname" : "Geneva",
					"patching_rect" : [ 369.0, 97.0, 41.0, 43.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "set take mix",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-52",
					"fontname" : "Geneva",
					"patching_rect" : [ 416.0, 127.0, 44.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : ";\r2gvp p 14",
					"linecount" : 3,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-53",
					"fontname" : "Geneva",
					"patching_rect" : [ 470.0, 167.0, 35.0, 41.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : ";\r2gvp p 15",
					"linecount" : 3,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-54",
					"fontname" : "Geneva",
					"patching_rect" : [ 432.0, 167.0, 37.0, 41.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : ";\r2gvp p 58",
					"linecount" : 3,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-55",
					"fontname" : "Geneva",
					"patching_rect" : [ 360.0, 167.0, 34.0, 41.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : ";\r2gvp p 67",
					"linecount" : 3,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-56",
					"fontname" : "Geneva",
					"patching_rect" : [ 323.0, 167.0, 33.0, 41.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : ";\r2gvp p 46",
					"linecount" : 3,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-57",
					"fontname" : "Geneva",
					"patching_rect" : [ 250.0, 167.0, 33.0, 41.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : ";\r2gvp p 47",
					"linecount" : 3,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-58",
					"fontname" : "Geneva",
					"patching_rect" : [ 287.0, 167.0, 34.0, 41.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "DSK fill - Color",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-59",
					"fontname" : "Geneva",
					"patching_rect" : [ 320.0, 97.0, 41.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "s 2gvp",
					"numoutlets" : 0,
					"id" : "obj-60",
					"fontname" : "Geneva",
					"color" : [ 1.0, 0.541176, 0.843137, 1.0 ],
					"patching_rect" : [ 128.0, 444.0, 38.0, 19.0 ],
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
					"id" : "obj-61",
					"fontname" : "Geneva",
					"color" : [ 1.0, 0.541176, 0.843137, 1.0 ],
					"patching_rect" : [ 241.0, 273.0, 38.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "r f 60 k",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-62",
					"fontname" : "Geneva",
					"patching_rect" : [ 102.0, 215.0, 45.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "r d 50 k",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-63",
					"fontname" : "Geneva",
					"patching_rect" : [ 53.0, 215.0, 46.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "e 28 3000 29 1 30 1000",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-64",
					"fontname" : "Geneva",
					"patching_rect" : [ 151.0, 85.0, 122.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "c pgm 6 i b preview black",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-65",
					"fontname" : "Geneva",
					"patching_rect" : [ 3.0, 23.0, 123.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "m k b",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-66",
					"fontname" : "Geneva",
					"patching_rect" : [ 252.0, 215.0, 34.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : ";\r2gvp p 11",
					"linecount" : 3,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-67",
					"fontname" : "Geneva",
					"patching_rect" : [ 48.0, 430.0, 36.0, 41.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : ";\r2gvp p 31",
					"linecount" : 3,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-68",
					"fontname" : "Geneva",
					"patching_rect" : [ 10.0, 430.0, 35.0, 41.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "e 20 192 27 128 31 192",
					"linecount" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-69",
					"fontname" : "Geneva",
					"patching_rect" : [ 29.0, 147.0, 120.0, 29.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "e 17 2000 18 1600",
					"linecount" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-70",
					"fontname" : "Geneva",
					"patching_rect" : [ 32.0, 85.0, 96.0, 29.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "w 1",
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-71",
					"fontname" : "Geneva",
					"patching_rect" : [ 131.0, 23.0, 25.0, 17.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "newobj",
					"text" : "s 2gvp",
					"numoutlets" : 0,
					"id" : "obj-72",
					"fontname" : "Geneva",
					"color" : [ 1.0, 0.541176, 0.843137, 1.0 ],
					"patching_rect" : [ 3.0, 234.0, 38.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "joystick coordinates",
					"numoutlets" : 0,
					"id" : "obj-73",
					"fontname" : "Geneva",
					"patching_rect" : [ 33.0, 68.0, 100.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "symmetry, softness, border",
					"numoutlets" : 0,
					"id" : "obj-74",
					"fontname" : "Geneva",
					"patching_rect" : [ 149.0, 68.0, 134.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "background hue, luminance, chroma",
					"numoutlets" : 0,
					"id" : "obj-75",
					"fontname" : "Geneva",
					"patching_rect" : [ 29.0, 131.0, 166.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "trigger DSK fade to/from black",
					"linecount" : 3,
					"numoutlets" : 0,
					"id" : "obj-76",
					"fontname" : "Geneva",
					"patching_rect" : [ 48.0, 293.0, 80.0, 43.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "set buses",
					"numoutlets" : 0,
					"id" : "obj-77",
					"fontname" : "Geneva",
					"patching_rect" : [ 20.0, 40.0, 51.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Transition rates and modes",
					"numoutlets" : 0,
					"id" : "obj-78",
					"fontname" : "Geneva",
					"patching_rect" : [ 191.0, 195.0, 180.0, 23.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Static settings",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-79",
					"fontname" : "Geneva",
					"patching_rect" : [ 205.0, 22.0, 75.0, 39.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "Action pushbuttons",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-80",
					"fontname" : "Geneva",
					"patching_rect" : [ 48.0, 255.0, 88.0, 39.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "State pushbuttons",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-81",
					"fontname" : "Geneva",
					"patching_rect" : [ 364.0, 23.0, 100.0, 39.0 ],
					"fontsize" : 12.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "DSK clip, hue, chroma, luminance",
					"numoutlets" : 0,
					"id" : "obj-82",
					"fontname" : "Geneva",
					"patching_rect" : [ 28.0, 175.0, 158.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "DSK Take Arm",
					"numoutlets" : 0,
					"id" : "obj-83",
					"fontname" : "Geneva",
					"patching_rect" : [ 364.0, 262.0, 110.0, 25.0 ],
					"fontsize" : 14.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ubutton",
					"numoutlets" : 4,
					"outlettype" : [ "bang", "bang", "", "int" ],
					"id" : "obj-84",
					"handoff" : "",
					"patching_rect" : [ 48.0, 293.0, 80.0, 26.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ubutton",
					"numoutlets" : 4,
					"outlettype" : [ "bang", "bang", "", "int" ],
					"id" : "obj-85",
					"handoff" : "",
					"patching_rect" : [ 465.0, 97.0, 49.0, 24.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ubutton",
					"numoutlets" : 4,
					"outlettype" : [ "bang", "bang", "", "int" ],
					"id" : "obj-86",
					"handoff" : "",
					"patching_rect" : [ 416.0, 127.0, 44.0, 24.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ubutton",
					"numoutlets" : 4,
					"outlettype" : [ "bang", "bang", "", "int" ],
					"id" : "obj-87",
					"handoff" : "",
					"patching_rect" : [ 320.0, 97.0, 40.0, 24.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ubutton",
					"numoutlets" : 4,
					"outlettype" : [ "bang", "bang", "", "int" ],
					"id" : "obj-88",
					"handoff" : "",
					"patching_rect" : [ 369.0, 98.0, 41.0, 23.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "201, 0 1000",
					"linecount" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-89",
					"fontname" : "Geneva",
					"patching_rect" : [ 336.0, 346.0, 39.0, 29.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "message",
					"text" : "0, 201 2000",
					"linecount" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"id" : "obj-90",
					"fontname" : "Geneva",
					"patching_rect" : [ 380.0, 346.0, 39.0, 29.0 ],
					"fontsize" : 9.0,
					"numinlets" : 2,
					"hidden" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ubutton",
					"numoutlets" : 4,
					"outlettype" : [ "bang", "bang", "", "int" ],
					"id" : "obj-91",
					"handoff" : "",
					"patching_rect" : [ 374.0, 285.0, 37.0, 38.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "wipe patterns",
					"numoutlets" : 0,
					"id" : "obj-92",
					"fontname" : "Geneva",
					"patching_rect" : [ 128.0, 41.0, 70.0, 19.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "trigger DSK take",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-93",
					"fontname" : "Geneva",
					"patching_rect" : [ 48.0, 355.0, 61.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ubutton",
					"numoutlets" : 4,
					"outlettype" : [ "bang", "bang", "", "int" ],
					"id" : "obj-94",
					"handoff" : "",
					"patching_rect" : [ 48.0, 356.0, 60.0, 24.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "DSK fill - Ext",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-95",
					"fontname" : "Geneva",
					"patching_rect" : [ 320.0, 61.0, 41.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ubutton",
					"numoutlets" : 4,
					"outlettype" : [ "bang", "bang", "", "int" ],
					"id" : "obj-96",
					"handoff" : "",
					"patching_rect" : [ 320.0, 61.0, 40.0, 25.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "DSK cut - CG",
					"linecount" : 3,
					"numoutlets" : 0,
					"id" : "obj-97",
					"fontname" : "Geneva",
					"patching_rect" : [ 369.0, 62.0, 41.0, 43.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ubutton",
					"numoutlets" : 4,
					"outlettype" : [ "bang", "bang", "", "int" ],
					"id" : "obj-98",
					"handoff" : "",
					"patching_rect" : [ 369.0, 63.0, 39.0, 23.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "set take cut",
					"linecount" : 3,
					"numoutlets" : 0,
					"id" : "obj-99",
					"fontname" : "Geneva",
					"patching_rect" : [ 416.0, 63.0, 43.0, 43.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "set take pattern",
					"linecount" : 3,
					"numoutlets" : 0,
					"id" : "obj-100",
					"fontname" : "Geneva",
					"patching_rect" : [ 416.0, 98.0, 43.0, 43.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ubutton",
					"numoutlets" : 4,
					"outlettype" : [ "bang", "bang", "", "int" ],
					"id" : "obj-101",
					"handoff" : "",
					"patching_rect" : [ 416.0, 98.0, 42.0, 24.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ubutton",
					"numoutlets" : 4,
					"outlettype" : [ "bang", "bang", "", "int" ],
					"id" : "obj-102",
					"handoff" : "",
					"patching_rect" : [ 416.0, 64.0, 42.0, 23.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "set key - EFF KEY",
					"linecount" : 2,
					"numoutlets" : 0,
					"id" : "obj-103",
					"fontname" : "Geneva",
					"patching_rect" : [ 465.0, 63.0, 47.0, 31.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ubutton",
					"numoutlets" : 4,
					"outlettype" : [ "bang", "bang", "", "int" ],
					"id" : "obj-104",
					"handoff" : "",
					"patching_rect" : [ 465.0, 64.0, 47.0, 24.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "trigger take auto",
					"linecount" : 3,
					"numoutlets" : 0,
					"id" : "obj-105",
					"fontname" : "Geneva",
					"patching_rect" : [ 48.0, 325.0, 49.0, 43.0 ],
					"fontsize" : 9.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ubutton",
					"numoutlets" : 4,
					"outlettype" : [ "bang", "bang", "", "int" ],
					"id" : "obj-106",
					"handoff" : "",
					"patching_rect" : [ 48.0, 326.0, 48.0, 23.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "ubutton",
					"numoutlets" : 4,
					"outlettype" : [ "bang", "bang", "", "int" ],
					"id" : "obj-107",
					"handoff" : "",
					"patching_rect" : [ 431.0, 285.0, 39.0, 39.0 ],
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "DSK Sweep Down",
					"linecount" : 4,
					"numoutlets" : 0,
					"id" : "obj-108",
					"fontname" : "Geneva",
					"patching_rect" : [ 376.0, 285.0, 36.0, 60.0 ],
					"fontsize" : 10.0,
					"numinlets" : 1
				}

			}
, 			{
				"box" : 				{
					"maxclass" : "comment",
					"text" : "DSK Sweep Up",
					"linecount" : 3,
					"numoutlets" : 0,
					"id" : "obj-109",
					"fontname" : "Geneva",
					"patching_rect" : [ 431.0, 285.0, 40.0, 47.0 ],
					"fontsize" : 10.0,
					"numinlets" : 1
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"source" : [ "obj-107", 0 ],
					"destination" : [ "obj-90", 0 ],
					"hidden" : 1,
					"midpoints" : [ 440.5, 332.0, 389.5, 332.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-104", 0 ],
					"destination" : [ "obj-48", 0 ],
					"hidden" : 1,
					"midpoints" : [ 474.5, 92.0, 554.5, 92.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-106", 0 ],
					"destination" : [ "obj-67", 0 ],
					"hidden" : 1,
					"midpoints" : [ 57.5, 353.0, 39.0, 353.0, 39.0, 418.0, 57.5, 418.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-102", 0 ],
					"destination" : [ "obj-22", 0 ],
					"hidden" : 1,
					"midpoints" : [ 425.5, 91.0, 412.0, 91.0, 412.0, 155.0, 405.5, 155.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-101", 0 ],
					"destination" : [ "obj-53", 0 ],
					"hidden" : 1,
					"midpoints" : [ 425.5, 125.0, 465.0, 125.0, 465.0, 149.0, 479.5, 149.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-98", 0 ],
					"destination" : [ "obj-56", 0 ],
					"hidden" : 1,
					"midpoints" : [ 378.5, 91.0, 364.0, 91.0, 364.0, 151.0, 332.5, 151.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-96", 0 ],
					"destination" : [ "obj-57", 0 ],
					"hidden" : 1,
					"midpoints" : [ 329.5, 90.0, 314.0, 90.0, 314.0, 136.0, 259.5, 136.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-94", 0 ],
					"destination" : [ "obj-38", 0 ],
					"hidden" : 1,
					"midpoints" : [ 57.5, 406.0, 96.5, 406.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-91", 0 ],
					"destination" : [ "obj-89", 0 ],
					"hidden" : 1,
					"midpoints" : [ 383.5, 332.0, 345.5, 332.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-90", 0 ],
					"destination" : [ "obj-13", 0 ],
					"hidden" : 1,
					"midpoints" : [ 389.5, 389.0, 345.5, 389.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-89", 0 ],
					"destination" : [ "obj-13", 0 ],
					"hidden" : 1,
					"midpoints" : [ 345.5, 389.0, 345.5, 389.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-88", 0 ],
					"destination" : [ "obj-55", 0 ],
					"hidden" : 1,
					"midpoints" : [ 378.5, 159.0, 369.5, 159.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-87", 0 ],
					"destination" : [ "obj-58", 0 ],
					"hidden" : 1,
					"midpoints" : [ 329.5, 142.0, 296.5, 142.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-86", 0 ],
					"destination" : [ "obj-54", 0 ],
					"hidden" : 1,
					"midpoints" : [ 425.5, 157.0, 441.5, 157.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-71", 0 ],
					"destination" : [ "obj-72", 0 ],
					"hidden" : 1,
					"midpoints" : [ 140.5, 63.0, 12.5, 63.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-70", 0 ],
					"destination" : [ "obj-72", 0 ],
					"hidden" : 1,
					"midpoints" : [ 41.5, 102.0, 12.5, 102.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-69", 0 ],
					"destination" : [ "obj-72", 0 ],
					"hidden" : 1,
					"midpoints" : [ 38.5, 171.0, 12.5, 171.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-66", 0 ],
					"destination" : [ "obj-61", 0 ],
					"hidden" : 1,
					"midpoints" : [ 261.5, 248.0, 250.5, 248.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-65", 0 ],
					"destination" : [ "obj-72", 0 ],
					"hidden" : 1,
					"midpoints" : [ 12.5, 63.0, 12.5, 63.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-64", 0 ],
					"destination" : [ "obj-72", 0 ],
					"hidden" : 1,
					"midpoints" : [ 160.5, 126.0, 12.5, 126.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-63", 0 ],
					"destination" : [ "obj-61", 0 ],
					"hidden" : 1,
					"midpoints" : [ 62.5, 248.0, 250.5, 248.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-62", 0 ],
					"destination" : [ "obj-61", 0 ],
					"hidden" : 1,
					"midpoints" : [ 111.5, 248.0, 250.5, 248.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-21", 0 ],
					"destination" : [ "obj-60", 0 ],
					"hidden" : 1,
					"midpoints" : [ 146.5, 381.0, 137.5, 381.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-46", 0 ],
					"destination" : [ "obj-61", 0 ],
					"hidden" : 1,
					"midpoints" : [ 232.5, 248.0, 250.5, 248.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-45", 0 ],
					"destination" : [ "obj-61", 0 ],
					"hidden" : 1,
					"midpoints" : [ 205.5, 248.0, 250.5, 248.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-44", 0 ],
					"destination" : [ "obj-61", 0 ],
					"hidden" : 1,
					"midpoints" : [ 475.5, 248.0, 250.5, 248.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-43", 0 ],
					"destination" : [ "obj-61", 0 ],
					"hidden" : 1,
					"midpoints" : [ 387.5, 248.0, 250.5, 248.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-42", 0 ],
					"destination" : [ "obj-61", 0 ],
					"hidden" : 1,
					"midpoints" : [ 297.5, 248.0, 250.5, 248.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-41", 0 ],
					"destination" : [ "obj-61", 0 ],
					"hidden" : 1,
					"midpoints" : [ 340.5, 248.0, 250.5, 248.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-39", 0 ],
					"destination" : [ "obj-61", 0 ],
					"hidden" : 1,
					"midpoints" : [ 517.5, 248.0, 250.5, 248.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-40", 0 ],
					"destination" : [ "obj-61", 0 ],
					"hidden" : 1,
					"midpoints" : [ 427.5, 248.0, 250.5, 248.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-37", 0 ],
					"destination" : [ "obj-72", 0 ],
					"hidden" : 1,
					"midpoints" : [ 179.5, 63.0, 12.5, 63.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-36", 0 ],
					"destination" : [ "obj-60", 0 ],
					"hidden" : 1,
					"midpoints" : [ 146.5, 336.0, 137.5, 336.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-35", 0 ],
					"destination" : [ "obj-60", 0 ],
					"hidden" : 1,
					"midpoints" : [ 146.5, 312.0, 137.5, 312.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-34", 0 ],
					"destination" : [ "obj-60", 0 ],
					"hidden" : 1,
					"midpoints" : [ 146.5, 358.0, 137.5, 358.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-85", 0 ],
					"destination" : [ "obj-47", 0 ],
					"hidden" : 1,
					"midpoints" : [ 474.5, 139.0, 516.5, 139.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-84", 0 ],
					"destination" : [ "obj-68", 0 ],
					"hidden" : 1,
					"midpoints" : [ 57.5, 322.0, 19.5, 322.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-19", 0 ],
					"destination" : [ "obj-17", 0 ],
					"hidden" : 1,
					"midpoints" : [ 296.5, 426.0, 296.5, 426.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-33", 0 ],
					"destination" : [ "obj-72", 0 ],
					"hidden" : 1,
					"midpoints" : [ 37.5, 220.0, 12.5, 220.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-32", 0 ],
					"destination" : [ "obj-28", 0 ],
					"hidden" : 1,
					"midpoints" : [ 431.5, 380.0, 408.5, 380.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-31", 0 ],
					"destination" : [ "obj-28", 0 ],
					"hidden" : 1,
					"midpoints" : [ 431.5, 400.0, 408.5, 400.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-30", 0 ],
					"destination" : [ "obj-28", 0 ],
					"hidden" : 1,
					"midpoints" : [ 431.5, 422.0, 408.5, 422.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-29", 0 ],
					"destination" : [ "obj-28", 0 ],
					"hidden" : 1,
					"midpoints" : [ 431.5, 444.0, 408.5, 444.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-27", 0 ],
					"destination" : [ "obj-12", 0 ],
					"hidden" : 1,
					"midpoints" : [ 234.5, 428.0, 219.5, 428.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-26", 0 ],
					"destination" : [ "obj-12", 0 ],
					"hidden" : 1,
					"midpoints" : [ 234.5, 404.0, 219.5, 404.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-25", 0 ],
					"destination" : [ "obj-12", 0 ],
					"hidden" : 1,
					"midpoints" : [ 234.5, 385.0, 219.5, 385.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-20", 0 ],
					"destination" : [ "obj-72", 0 ],
					"hidden" : 1,
					"midpoints" : [ 41.5, 126.0, 12.5, 126.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-24", 0 ],
					"destination" : [ "obj-12", 0 ],
					"hidden" : 1,
					"midpoints" : [ 234.5, 366.0, 219.5, 366.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-18", 0 ],
					"destination" : [ "obj-16", 0 ],
					"hidden" : 1,
					"midpoints" : [ 487.5, 425.0, 487.5, 425.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-17", 0 ],
					"destination" : [ "obj-15", 0 ],
					"hidden" : 1,
					"midpoints" : [ 296.5, 456.0, 296.5, 456.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-16", 0 ],
					"destination" : [ "obj-14", 0 ],
					"hidden" : 1,
					"midpoints" : [ 487.5, 453.0, 487.5, 453.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-13", 0 ],
					"destination" : [ "obj-18", 0 ],
					"hidden" : 1,
					"midpoints" : [ 345.5, 511.0, 524.0, 511.0, 524.0, 262.0, 487.5, 262.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-11", 0 ],
					"destination" : [ "obj-8", 0 ],
					"hidden" : 1,
					"midpoints" : [ 566.5, 509.0, 694.0, 509.0, 694.0, 260.0, 657.5, 260.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-9", 0 ],
					"destination" : [ "obj-10", 0 ],
					"hidden" : 1,
					"midpoints" : [ 657.5, 451.0, 657.5, 451.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-6", 0 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 1,
					"midpoints" : [ 566.5, 387.0, 566.5, 387.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-5", 0 ],
					"destination" : [ "obj-11", 0 ],
					"hidden" : 1,
					"midpoints" : [ 610.5, 387.0, 566.5, 387.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-4", 0 ],
					"destination" : [ "obj-6", 0 ],
					"hidden" : 1,
					"midpoints" : [ 553.5, 330.0, 566.5, 330.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-3", 0 ],
					"destination" : [ "obj-5", 0 ],
					"hidden" : 1,
					"midpoints" : [ 610.5, 330.0, 610.5, 330.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-8", 0 ],
					"destination" : [ "obj-9", 0 ],
					"hidden" : 1,
					"midpoints" : [ 657.5, 423.0, 657.5, 423.0 ]
				}

			}
, 			{
				"patchline" : 				{
					"source" : [ "obj-50", 0 ],
					"destination" : [ "obj-61", 0 ],
					"hidden" : 1,
					"midpoints" : [ 158.5, 248.0, 250.5, 248.0 ]
				}

			}
 ]
	}

}
