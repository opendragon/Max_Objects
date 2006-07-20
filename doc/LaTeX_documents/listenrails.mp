% File: listenrails.mp

input metaobj
input fixedBox

verbatimtex
%&latex
\documentclass{report}
\usepackage{color}
\begin{document}
etex;

numeric gap, hGap, lineRadius;

gap = 30;
hGap = 15;
lineRadius = 5;
tGap = 2;

setObjectDefaultOption("FixedBox")("rbox_radius")(8pt);
setObjectDefaultOption("FixedBox")("width")(20pt);
setObjectDefaultOption("FixedBox")("height")(16pt);

beginfig(-1);

% Model/1

newBox.Model(btex Model etex) "framed(false)";
Model.c = origin;
drawObj(Model);
newBox.symbol1(btex \textit{Symbol} etex);
symbol1.w = Model.w + (gap,-.5gap);
drawObj(symbol1);
newBox.descriptions1(btex \textit{Descriptions} etex);
descriptions1.w = symbol1.e + (.5gap,0);
drawObj(descriptions1);
drawarrow (xpart(Model.w),ypart(symbol1.w)) -- symbol1.w;
drawarrow symbol1.e -- descriptions1.w;
drawarrow descriptions1.e -- (xpart(descriptions1.e)+gap,ypart(descriptions1.e));

% Symbol/2

newBox.Symbol(btex Symbol etex) "framed(false)";
Symbol.w = Model.w + (0,-gap);
drawObj(Symbol);
newFixedBox.openAngle2(btex $<$ etex);
openAngle2.w = Symbol.w + (gap,-.5gap);
drawObj(openAngle2);
newFixedBox.nonBlankString2(btex NonBlankString etex) "width(85pt)";
nonBlankString2.w = openAngle2.e + (.5gap,0);
drawObj(nonBlankString2);
newFixedBox.closeAngle2(btex $>$ etex);
closeAngle2.w = nonBlankString2.e + (.5gap,0);
drawObj(closeAngle2);
drawarrow (xpart(Symbol.w),ypart(openAngle2.w)) -- openAngle2.w;
drawarrow openAngle2.e -- nonBlankString2.w;
drawarrow nonBlankString2.e -- closeAngle2.w;
drawarrow closeAngle2.e -- (xpart(closeAngle2.e)+gap,ypart(closeAngle2.e));

% Descriptions/3

newBox.Descriptions(btex Descriptions etex) "framed(false)";
Descriptions.w = Symbol.w + (0,-gap);
drawObj(Descriptions);
newBox.description3(btex \textit{Description} etex);
description3.w = Descriptions.w + (gap,-.5gap);
drawObj(description3);
drawarrow (xpart(Descriptions.w),ypart(description3.w)) -- description3.w;
drawarrow description3.e -- (xpart(description3.e)+hGap+gap,ypart(description3.e));
drawarrow (xpart(description3.e)+hGap-lineRadius,ypart(description3.e)){dir0} ..
			{dir270}(xpart(description3.e)+hGap,ypart(description3.e)-lineRadius) --
			(xpart(description3.e)+hGap,ypart(description3.e)-.25gap-tGap){dir270} ..
			{dir180}(xpart(description3.e)+hGap-lineRadius,ypart(description3.e)-.25gap-tGap-lineRadius);
drawarrow (xpart(description3.e)+hGap-lineRadius,ypart(description3.e)-.25gap-tGap-lineRadius) --
			(xpart(description3.w)+lineRadius-hGap,ypart(description3.e)-.25gap-tGap-lineRadius);
drawarrow (xpart(description3.w)+lineRadius-hGap,ypart(description3.w)-.25gap-tGap-lineRadius){dir180} ..
			(xpart(description3.w)-hGap,ypart(description3.w)-.25gap-tGap) --
			(xpart(description3.w)-hGap,ypart(description3.w)-lineRadius){dir90} ..
			{dir0}(xpart(description3.w)+lineRadius-hGap,ypart(description3.w));

% Description/4

newBox.Description(btex Description etex) "framed(false)";
Description.w = Descriptions.w + (0,-1.5gap);
drawObj(Description);
newBox.symbol4(btex \textit{Symbol} etex);
symbol4.w = Description.w + (gap,-.5gap);
drawObj(symbol4);
newBox.outputList4(btex \textit{OutputList} etex);
outputList4.w = symbol4.e + (.5gap,0);
drawObj(outputList4);
newFixedBox.equals4(btex \textbf{=} etex);
equals4.w = outputList4.e + (.5gap,0);
drawObj(equals4);
newBox.phraseList4(btex \textit{PhraseList} etex);
phraseList4.w = equals4.e + (.5gap,0);
drawObj(phraseList4);
newFixedBox.semicolon4(btex \textbf{;} etex);
semicolon4.w = phraseList4.e + (.5gap,0);
drawObj(semicolon4);
drawarrow (xpart(Description.w),ypart(symbol4.w)) -- symbol4.w;
drawarrow symbol4.e -- outputList4.w;
drawarrow outputList4.e -- equals4.w;
drawarrow equals4.e -- phraseList4.w;
drawarrow phraseList4.e -- semicolon4.w;
drawarrow semicolon4.e -- (xpart(semicolon4.e)+gap,ypart(semicolon4.e));

% OutputList/5

newBox.OutputList(btex OutputList etex) "framed(false)";
OutputList.w = Description.w + (0,-gap);
drawObj(OutputList);
newFixedBox.openCurly5(btex $\{$ etex);
openCurly5.w = OutputList.w + (gap,-(.5gap+3tGap));
drawObj(openCurly5);
newFixedBox.atom5(btex Atom etex) "width(40pt)";
atom5.w = openCurly5.e + (.5gap+hGap,0);
drawObj(atom5);
newFixedBox.closeCurly5(btex $\}$ etex);
closeCurly5.w = atom5.e + (.5gap+hGap,0);
drawObj(closeCurly5);
drawarrow openCurly5.e -- atom5.w;
drawarrow atom5.e -- closeCurly5.w;
drawarrow (xpart(OutputList.w),ypart(OutputList.s)-tGap) --
((xpart(closeCurly5.e)+gap+hGap+xpart(OutputList.w))/2,ypart(OutputList.s)-tGap);
drawarrow ((xpart(closeCurly5.e)+gap+hGap+xpart(OutputList.w))/2,ypart(OutputList.s)-tGap) --
(xpart(closeCurly5.e)+gap+hGap,ypart(OutputList.s)-tGap);
drawarrow closeCurly5.e --
(xpart(closeCurly5.e)+hGap-lineRadius,ypart(closeCurly5.e)){dir0} ..
{dir90}(xpart(closeCurly5.e)+hGap,ypart(closeCurly5.e)+lineRadius) --
(xpart(closeCurly5.e)+hGap,ypart(OutputList.s)-tGap-lineRadius){dir90} ..
{dir0}(xpart(closeCurly5.e)+hGap+lineRadius,ypart(OutputList.s)-tGap);
drawarrow (xpart(openCurly5.w)-hGap-lineRadius,ypart(OutputList.s)-tGap){dir0} ..
{dir270}(xpart(openCurly5.w)-hGap,ypart(OutputList.s)-tGap-lineRadius) --
(xpart(openCurly5.w)-hGap,ypart(openCurly5.w)+lineRadius){dir270} ..
{dir0}(xpart(openCurly5.w)+lineRadius-hGap,ypart(openCurly5.w)) -- openCurly5.w;
drawarrow (xpart(atom5.e)+hGap-lineRadius,ypart(atom5.e)){dir0} ..
			{dir270}(xpart(atom5.e)+hGap,ypart(atom5.e)-lineRadius) --
			(xpart(atom5.e)+hGap,ypart(atom5.e)-.25gap-tGap){dir270} ..
			{dir180}(xpart(atom5.e)+hGap-lineRadius,ypart(atom5.e)-.25gap-tGap-lineRadius);
drawarrow (xpart(atom5.e)+hGap-lineRadius,ypart(atom5.e)-.25gap-tGap-lineRadius) --
			(xpart(atom5.w)+lineRadius-hGap,ypart(atom5.e)-.25gap-tGap-lineRadius);
drawarrow (xpart(atom5.w)+lineRadius-hGap,ypart(atom5.w)-.25gap-tGap-lineRadius){dir180} ..
			(xpart(atom5.w)-hGap,ypart(atom5.w)-.25gap-tGap) --
			(xpart(atom5.w)-hGap,ypart(atom5.w)-lineRadius){dir90} ..
			{dir0}(xpart(atom5.w)+lineRadius-hGap,ypart(atom5.w));

% PhraseList/6

newBox.PhraseList(btex PhraseList etex) "framed(false)";
PhraseList.w = OutputList.w + (0,-1.5gap);
drawObj(PhraseList);
newBox.phrase6(btex \textit{Phrase} etex);
phrase6.w = PhraseList.w + (gap,-.5gap);
drawObj(phrase6);
newFixedBox.separator6(btex $|$ etex);
separator6.c = phrase6.c + (0,-(.5gap+2tGap));
drawObj(separator6);
drawarrow (xpart(PhraseList.w),ypart(phrase6.w)) -- phrase6.w;
drawarrow phrase6.e -- (xpart(phrase6.e)+hGap+gap,ypart(phrase6.e));
drawarrow (xpart(phrase6.e)+hGap-lineRadius,ypart(phrase6.e)){dir0} ..
			{dir270}(xpart(phrase6.e)+hGap,ypart(phrase6.e)-lineRadius) --
			(xpart(phrase6.e)+hGap,ypart(separator6.e)+lineRadius){dir270} ..
			{dir180}(xpart(phrase6.e)+hGap-lineRadius,ypart(separator6.e));
drawarrow (xpart(phrase6.e)+hGap-lineRadius,ypart(separator6.e)) -- separator6.e;
drawarrow separator6.w -- (xpart(phrase6.w)+lineRadius-hGap,ypart(separator6.w));
drawarrow (xpart(phrase6.w)+lineRadius-hGap,ypart(separator6.w)){dir180} ..
			(xpart(phrase6.w)-hGap,ypart(separator6.w)+lineRadius) --
			(xpart(phrase6.w)-hGap,ypart(phrase6.w)-lineRadius){dir90} ..
			{dir0}(xpart(phrase6.w)+lineRadius-hGap,ypart(phrase6.w));

% Phrase/7

newBox.Phrase(btex Phrase etex) "framed(false)";
Phrase.w = PhraseList.w + (0,-1.5gap);
drawObj(Phrase);
newBox.symbol9(btex \textit{Symbol} etex);
symbol9.w = Phrase.w + (gap,-.5gap); 
drawObj(symbol9);
newFixedBox.word9(btex Word etex) "width(35pt)";
word9.w = symbol9.w + (0,-(.5gap+2tGap));
drawObj(word9);
newBox.outputList9(btex \textit{OutputList} etex);
outputList9.w = word9.e + (.5gap,0);
drawObj(outputList9); 
drawarrow (xpart(Phrase.w),ypart(symbol9.c)) -- symbol9.w;
drawarrow (xpart(symbol9.w)-hGap-lineRadius,ypart(symbol9.c)){dir0} ..
{dir270}(xpart(symbol9.w)-hGap,ypart(symbol9.c)-lineRadius) --
(xpart(symbol9.w)-hGap,ypart(word9.c)+lineRadius){dir270} ..
{dir0}(xpart(symbol9.w)+lineRadius-hGap,ypart(word9.c)) --
word9.w;
drawarrow word9.e -- outputList9.w;
drawarrow symbol9.e --
(xpart(outputList9.e)+gap+hGap,ypart(symbol9.e));
drawarrow outputList9.e --
(xpart(outputList9.e)+hGap-lineRadius,ypart(outputList9.e)){dir0} ..
{dir90}(xpart(outputList9.e)+hGap,ypart(outputList9.e)+lineRadius) --
(xpart(outputList9.e)+hGap,ypart(symbol9.e)-lineRadius){dir90} ..
{dir0}(xpart(outputList9.e)+hGap+lineRadius,ypart(symbol9.e));

endfig;

end

% $Log: listenrails.mp,v $
% Revision 1.3  2006/07/20 04:47:52  churchoflambda
% Re-added the files to record their changes.
%
% Revision 1.1  2005/08/02 15:08:22  churchoflambda
% Added rail diagrams for listen, map1d, map2d, map3d and pfsm.
%
