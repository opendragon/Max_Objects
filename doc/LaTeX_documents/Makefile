psfiles = Vabssymbol.ps Vassemblesymbol.ps Vceilingsymbol.ps \
	Vcollectsymbol.ps Vcossymbol.ps Vdecodesymbol.ps \
	Vdistancesymbol.ps Vdropsymbol.ps Vencodesymbol.ps \
	Vexpsymbol.ps Vfloorsymbol.ps Vinvertsymbol.ps \
	Vjetsymbol.ps Vlengthsymbol.ps Vlogsymbol.ps \
	Vltrimsymbol.ps Vmeansymbol.ps Vnegatesymbol.ps \
	Vreducesymbol.ps Vreversesymbol.ps Vrotatesymbol.ps \
	Vroundsymbol.ps Vrtrimsymbol.ps Vscansymbol.ps \
	Vsegmentsymbol.ps Vsinsymbol.ps Vsqrtsymbol.ps \
	Vtakesymbol.ps Vtokenizesymbol.ps Vtrimsymbol.ps \
	Vtruncatesymbol.ps Vunspellsymbol.ps \
	bqtsymbol.ps \
	caseShiftsymbol.ps changessymbol.ps comparessymbol.ps \
	dataTypesymbol.ps \
	fidgetsymbol.ps fileLoggersymbol.ps \
	gcdsymbol.ps gvp100connections.ps gvp100symbol.ps \
	ldp1550connections.ps ldp1550symbol.ps listTypesymbol.ps \
	listensymbol.ps \
	map1dsymbol.ps map2dsymbol.ps map3dsymbol.ps \
	memorysymbol.ps mtcTracksymbol.ps \
	mtcconnections.ps mtcstate.ps mtcsymbol.ps \
	notXsymbol.ps \
	pfsmstate.ps pfsmsymbol.ps placeholder.ps \
	queuesymbol.ps \
	rcxsymbol.ps \
	serialXsymbol.ps spaceballconnections.ps spaceballsymbol.ps \
	speaksymbol.ps stacksymbol.ps sysLoggersymbol.ps \
	tcpClientstate.ps tcpClientsymbol.ps tcpLocatesymbol.ps \
	tcpMultiServerstate.ps tcpMultiServersymbol.ps tcpServerstate.ps \
	tcpServersymbol.ps \
	wqtsymbol.ps \
	x10connections.ps x10symbol.ps x10unitssymbol.ps

mainsource = Max_Objects_from_Norm_Jaffe.tex

secondarysources = documentationHistory.tex frontispiece.tex

maindvi = $(mainsource:.tex=.dvi)

mainidx = $(mainsource:.tex=.idx)

mainps = $(mainsource:.tex=.ps)

mainpdf = $(mainsource:.tex=.pdf)

target = /Volumes/Merovingen/OpenDragon\ Sites/Com_domain\ ƒ/Com_domain/Objects/$(mainpdf)

texfiles = Vabs.tex Vassemble.tex Vceiling.tex \
	Vcollect.tex Vcos.tex Vdecode.tex \
	Vdistance.tex Vdrop.tex Vencode.tex \
	Vexp.tex Vfloor.tex Vinvert.tex \
	Vjet.tex Vlength.tex Vlog.tex \
	Vltrim.tex Vmean.tex Vnegate.tex \
	Vreduce.tex Vreverse.tex Vrotate.tex \
	Vround.tex Vrtrim.tex Vscan.tex \
	Vsegment.tex Vsin.tex Vsqrt.tex \
	Vtake.tex Vtokenize.tex Vtrim.tex \
	Vtruncate.tex Vunspell.tex \
	bqt.tex \
	caseShift.tex changes.tex compares.tex \
	dataType.tex \
	fidget.tex fileLogger.tex \
	gcd.tex gvp100.tex \
	ldp1550.tex listType.tex listen.tex \
	map1d.tex map2d.tex map3d.tex \
	memory.tex mtc.tex mtcTrack.tex \
	notX.tex \
	pfsm.tex Phidgets.tex phidg_interfacekit004.tex \
	phidg_interfacekit888.tex phidg_quadservo.tex \
	phidg_rfid.tex phidg_textlcd.tex phidg_textlcd088.tex \
	phidg_unitservo.tex \
	queue.tex \
	rcx.tex \
	serialX.tex spaceball.tex speak.tex \
	stack.tex sysLogger.tex \
	tcpClient.tex tcpLocate.tex tcpMultiServer.tex \
	tcpServer.tex \
	wqt.tex \
	x10.tex x10units.tex

$(psfiles) : maxboxmacros.mp

%.ps: %.mp
	mpost --interaction batchmode $<

$(maindvi) : $(psfiles) $(texfiles) $(mainsource) $(secondarysources)
	latex --interaction batchmode $(mainsource)
	latex --interaction batchmode $(mainsource)
	latex --interaction batchmode $(mainsource)
	makeindex $(mainidx)
	latex --interaction batchmode $(mainsource)

$(mainps) : $(maindvi)
	dvips $(maindvi) -o $(mainps)

$(mainpdf) : $(mainps)
	gs -dCompatibilityLevel=1.2 -dMaxSubsetPct=100 -dNOPAUSE -dBATCH \
		-sDEVICE=pdfwrite -sPAPERSIZE=letter -sOutputFile="$(mainpdf)" \
		-c save pop -f "$(mainps)" -sFONTPATH=.

$(target) : $(mainpdf)
	cp $(mainpdf) $(target)

.PHONY : clean manual
clean :
	rm $(psfiles) *.log *.pdf *.mpx *.dvi

manual : $(target)
