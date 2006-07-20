% File: map1drails.mp

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

% Ranges/1

newBox.Ranges(btex Ranges etex) "framed(false)";
Ranges.c = origin;
drawObj(Ranges);
newBox.rangeSpec1(btex \textit{RangeSpec} etex);
rangeSpec1.w = Ranges.w + (gap,-.5gap);
drawObj(rangeSpec1);
newBox.outputList1(btex \textit{OutputList} etex);
outputList1.w = rangeSpec1.e + (.5gap,0);
drawObj(outputList1);
newFixedBox.semicolon1(btex \textbf{;} etex);
semicolon1.w = outputList1.e + (.5gap,0);
drawObj(semicolon1);
drawarrow (xpart(Ranges.w),ypart(rangeSpec1.w)) -- rangeSpec1.w;
drawarrow rangeSpec1.e -- outputList1.w;
drawarrow outputList1.e -- semicolon1.w;
drawarrow semicolon1.e -- (xpart(semicolon1.e)+hGap+gap,ypart(semicolon1.e));
drawarrow (xpart(semicolon1.e)+hGap-lineRadius,ypart(semicolon1.e)){dir0} ..
			{dir270}(xpart(semicolon1.e)+hGap,ypart(semicolon1.e)-lineRadius) --
			(xpart(semicolon1.e)+hGap,ypart(semicolon1.e)-.25gap-tGap){dir270} ..
			{dir180}(xpart(semicolon1.e)+hGap-lineRadius,ypart(semicolon1.e)-.25gap-tGap-lineRadius);
drawarrow (xpart(semicolon1.e)+hGap-lineRadius,ypart(semicolon1.e)-.25gap-tGap-lineRadius) --
			(xpart(rangeSpec1.w)+lineRadius-hGap,ypart(semicolon1.e)-.25gap-tGap-lineRadius);
drawarrow (xpart(rangeSpec1.w)+lineRadius-hGap,ypart(rangeSpec1.w)-.25gap-tGap-lineRadius){dir180} ..
			(xpart(rangeSpec1.w)-hGap,ypart(rangeSpec1.w)-.25gap-tGap) --
			(xpart(rangeSpec1.w)-hGap,ypart(rangeSpec1.w)-lineRadius){dir90} ..
			{dir0}(xpart(rangeSpec1.w)+lineRadius-hGap,ypart(rangeSpec1.w));

% RangeSpec/2

newBox.RangeSpec(btex RangeSpec etex) "framed(false)";
RangeSpec.w = Ranges.w + (0,-1.5gap);
drawObj(RangeSpec);
newFixedBox.asterisk2(btex $*$ etex);
asterisk2.w = RangeSpec.w + (gap,-.5gap); 
drawObj(asterisk2);
newBox.rangex2(btex \textit{Range} etex);
rangex2.w = asterisk2.w + (0,-(.5gap+2tGap));
drawObj(rangex2);
drawarrow (xpart(RangeSpec.w),ypart(asterisk2.c)) -- asterisk2.w;
drawarrow (xpart(asterisk2.w)-hGap-lineRadius,ypart(asterisk2.c)){dir0} ..
{dir270}(xpart(asterisk2.w)-hGap,ypart(asterisk2.c)-lineRadius) --
(xpart(asterisk2.w)-hGap,ypart(rangex2.c)+lineRadius){dir270} ..
{dir0}(xpart(asterisk2.w)+lineRadius-hGap,ypart(rangex2.c)) --
rangex2.w;
drawarrow rangex2.e --
(xpart(rangex2.e)+hGap-lineRadius,ypart(rangex2.e)){dir0} ..
{dir90}(xpart(rangex2.e)+hGap,ypart(rangex2.e)+lineRadius) --
(xpart(rangex2.e)+hGap,ypart(asterisk2.e)-lineRadius){dir90} ..
{dir0}(xpart(rangex2.e)+hGap+lineRadius,ypart(asterisk2.e));
drawarrow asterisk2.e -- (xpart(rangex2.e)+gap,ypart(asterisk2.e));

% Range/3

newBox.Range(btex Range etex) "framed(false)";
Range.w = RangeSpec.w + (0,-(1.5gap+2tGap));
drawObj(Range);
newFixedBox.openSquare3(btex $[$ etex);
openSquare3.w = Range.w + (gap,-.5gap);
drawObj(openSquare3);
newFixedBox.openRound3(btex $($ etex);
openRound3.w = openSquare3.w + (0,-(.5gap+2tGap));
drawObj(openRound3);
newBox.lowerValue3(btex \textit{LowerValue} etex);
lowerValue3.w = (xpart(openSquare3.e)+2hGap,(ypart(openRound3.e)+ypart(openSquare3.e))/2);
drawObj(lowerValue3);
newBox.upperValue3(btex \textit{UpperValue} etex);
upperValue3.w = lowerValue3.e + (.5gap,0);
drawObj(upperValue3);
newFixedBox.closeSquare3(btex $]$ etex);
closeSquare3.w = (xpart(upperValue3.e)+2hGap,ypart(openSquare3.e));
drawObj(closeSquare3);
newFixedBox.closeRound3(btex $)$ etex);
closeRound3.w = (xpart(closeSquare3.w),ypart(openRound3.e));
drawObj(closeRound3);
drawarrow lowerValue3.e -- upperValue3.w;
drawarrow (xpart(Range.w),ypart(openSquare3.c)) -- openSquare3.w;
drawarrow (xpart(openSquare3.w)-hGap-lineRadius,ypart(openSquare3.c)){dir0} ..
{dir270}(xpart(openSquare3.w)-hGap,ypart(openSquare3.c)-lineRadius) --
(xpart(openSquare3.w)-hGap,ypart(openRound3.c)+lineRadius){dir270} ..
{dir0}(xpart(openSquare3.w)+lineRadius-hGap,ypart(openRound3.c)) --
openRound3.w;
drawarrow closeSquare3.e -- (xpart(closeSquare3.e)+gap+hGap,ypart(closeSquare3.e));
drawarrow openSquare3.e --
	(xpart(openSquare3.e)+hGap-lineRadius,ypart(openSquare3.e)){dir0} ..
	{dir270}(xpart(openSquare3.e)+hGap,ypart(openSquare3.e)-lineRadius) --
	(xpart(openSquare3.e)+hGap,ypart(lowerValue3.w)+lineRadius){dir270} ..
	{dir0}(xpart(openSquare3.e)+hGap+lineRadius,ypart(lowerValue3.w)) --
	lowerValue3.w;
draw openRound3.e --
	(xpart(openRound3.e)+hGap-lineRadius,ypart(openRound3.e)){dir0} ..
	{dir90}(xpart(openRound3.e)+hGap,ypart(openRound3.e)+lineRadius) --
	(xpart(openRound3.e)+hGap,ypart(lowerValue3.w)-lineRadius){dir90} ..
	{dir0}(xpart(openRound3.e)+hGap+lineRadius,ypart(lowerValue3.w));
drawarrow upperValue3.e --
	(xpart(upperValue3.e)+hGap-lineRadius,ypart(upperValue3.e)){dir0} ..
	{dir90}(xpart(upperValue3.e)+hGap,ypart(upperValue3.e)+lineRadius) --
	(xpart(upperValue3.e)+hGap,ypart(closeSquare3.w)-lineRadius){dir90} ..
	{dir0}(xpart(upperValue3.e)+hGap+lineRadius,ypart(closeSquare3.w)) --
	closeSquare3.w;
drawarrow (xpart(upperValue3.e)+hGap-lineRadius,ypart(upperValue3.e)){dir0} ..
	{dir270}(xpart(upperValue3.e)+hGap,ypart(upperValue3.e)-lineRadius) --
	(xpart(upperValue3.e)+hGap,ypart(closeRound3.w)+lineRadius){dir270} ..
	{dir0}(xpart(upperValue3.e)+hGap+lineRadius,ypart(closeRound3.w)) --
	closeRound3.w;
drawarrow closeRound3.e --
(xpart(closeRound3.e)+hGap-lineRadius,ypart(closeRound3.e)){dir0} ..
{dir90}(xpart(closeRound3.e)+hGap,ypart(closeRound3.e)+lineRadius) --
(xpart(closeRound3.e)+hGap,ypart(closeSquare3.e)-lineRadius){dir90} ..
{dir0}(xpart(closeRound3.e)+hGap+lineRadius,ypart(closeSquare3.e));

% LowerValue/4

newBox.LowerValue(btex LowerValue etex) "framed(false)";
LowerValue.w = Range.w + (0,-(1.5gap+2tGap));
drawObj(LowerValue);
newFixedBox.number4(btex Number etex) "width(45pt)";
number4.w = LowerValue.w + (gap,-.5gap);
drawObj(number4);
newFixedBox.negInf4A(btex $-$inf etex) "width(30pt)";
negInf4A.w = number4.w + (0,-(.5gap+2tGap));
drawObj(negInf4A);
newFixedBox.negInf4B(btex $-\infty$ etex) "width(30pt)";
negInf4B.w = negInf4A.w + (0,-(.5gap+2tGap));
drawObj(negInf4B);
drawarrow (xpart(LowerValue.w),ypart(number4.w)) -- number4.w;
drawarrow (xpart(number4.w)-hGap-lineRadius,ypart(number4.w)){dir0} ..
	{dir270}(xpart(number4.w)-hGap,ypart(number4.w)-lineRadius) --
	(xpart(number4.w)-hGap,ypart(negInf4B.w)+lineRadius){dir270} ..
	{dir0}(xpart(number4.w)+lineRadius-hGap,ypart(negInf4B.w)) --
	negInf4B.w;
drawarrow (xpart(number4.w)-hGap,ypart(negInf4A.w)+lineRadius){dir270} ..
	{dir0}(xpart(number4.w)+lineRadius-hGap,ypart(negInf4A.w)) --
	negInf4A.w;
drawarrow number4.e -- (xpart(number4.e)+gap,ypart(number4.e));
drawarrow negInf4B.e --
	(xpart(number4.e)+hGap-lineRadius,ypart(negInf4B.e)){dir0} ..
	{dir90}(xpart(number4.e)+hGap,ypart(negInf4B.e)+lineRadius) --
	(xpart(number4.e)+hGap,ypart(number4.e)-lineRadius){dir90} ..
	{dir0}(xpart(number4.e)+hGap+lineRadius,ypart(number4.e));
drawarrow negInf4A.e --
	(xpart(number4.e)+hGap-lineRadius,ypart(negInf4A.e)){dir0} ..
	{dir90}(xpart(number4.e)+hGap,ypart(negInf4A.e)+lineRadius);

% UpperValue/5

newBox.UpperValue(btex UpperValue etex) "framed(false)";
UpperValue.w = LowerValue.w + (0,-(2gap+5tGap));
drawObj(UpperValue);
newFixedBox.number5(btex Number etex) "width(45pt)";
number5.w = UpperValue.w + (gap,-.5gap);
drawObj(number5);
newFixedBox.posInf5A(btex $+$inf etex) "width(30pt)";
posInf5A.w = number5.w + (0,-(.5gap+2tGap));
drawObj(posInf5A);
newFixedBox.posInf5B(btex inf etex) "width(25pt)";
posInf5B.w = posInf5A.w + (0,-(.5gap+2tGap));
drawObj(posInf5B);
newFixedBox.posInf5C(btex $\infty$ etex) "width(25pt)";
posInf5C.w = posInf5B.w + (0,-(.5gap+2tGap));
drawObj(posInf5C);
newFixedBox.posInf5D(btex $+\infty$ etex) "width(30pt)";
posInf5D.w = posInf5C.w + (0,-(.5gap+2tGap));
drawObj(posInf5D);
drawarrow (xpart(UpperValue.w),ypart(number5.w)) -- number5.w;
drawarrow (xpart(number5.w)-hGap-lineRadius,ypart(number5.w)){dir0} ..
	{dir270}(xpart(number5.w)-hGap,ypart(number5.w)-lineRadius) --
	(xpart(number5.w)-hGap,ypart(posInf5D.w)+lineRadius){dir270} ..
	{dir0}(xpart(number5.w)+lineRadius-hGap,ypart(posInf5D.w)) --
	posInf5D.w;
drawarrow (xpart(number5.w)-hGap,ypart(posInf5A.w)+lineRadius){dir270} ..
	{dir0}(xpart(number5.w)+lineRadius-hGap,ypart(posInf5A.w)) --
	posInf5A.w;
drawarrow (xpart(number5.w)-hGap,ypart(posInf5B.w)+lineRadius){dir270} ..
	{dir0}(xpart(number5.w)+lineRadius-hGap,ypart(posInf5B.w)) --
	posInf5B.w;
drawarrow (xpart(number5.w)-hGap,ypart(posInf5C.w)+lineRadius){dir270} ..
	{dir0}(xpart(number5.w)+lineRadius-hGap,ypart(posInf5C.w)) --
	posInf5C.w;
drawarrow number5.e -- (xpart(number5.e)+gap,ypart(number5.e));
drawarrow posInf5D.e --
	(xpart(number5.e)+hGap-lineRadius,ypart(posInf5D.e)){dir0} ..
	{dir90}(xpart(number5.e)+hGap,ypart(posInf5D.e)+lineRadius) --
	(xpart(number5.e)+hGap,ypart(number5.e)-lineRadius){dir90} ..
	{dir0}(xpart(number5.e)+hGap+lineRadius,ypart(number5.e));
drawarrow posInf5A.e --
	(xpart(number5.e)+hGap-lineRadius,ypart(posInf5A.e)){dir0} ..
	{dir90}(xpart(number5.e)+hGap,ypart(posInf5A.e)+lineRadius);
drawarrow posInf5B.e --
	(xpart(number5.e)+hGap-lineRadius,ypart(posInf5B.e)){dir0} ..
	{dir90}(xpart(number5.e)+hGap,ypart(posInf5B.e)+lineRadius);
drawarrow posInf5C.e --
	(xpart(number5.e)+hGap-lineRadius,ypart(posInf5C.e)){dir0} ..
	{dir90}(xpart(number5.e)+hGap,ypart(posInf5C.e)+lineRadius);

% OutputList/6

newBox.OutputList(btex OutputList etex) "framed(false)";
OutputList.w = UpperValue.w + (0,-(3.5gap+tGap));
drawObj(OutputList);
newFixedBox.atom6(btex Atom etex) "width(40pt)";
atom6.w = OutputList.w + (gap,-(.5gap+3tGap));
drawObj(atom6);
drawarrow (xpart(OutputList.w),ypart(OutputList.s)-tGap) --
((xpart(atom6.e)+gap+hGap+xpart(OutputList.w))/2,ypart(OutputList.s)-tGap);
drawarrow ((xpart(atom6.e)+gap+hGap+xpart(OutputList.w))/2,ypart(OutputList.s)-tGap) --
(xpart(atom6.e)+gap+hGap,ypart(OutputList.s)-tGap);
drawarrow atom6.w --
(xpart(atom6.w)+lineRadius-hGap,ypart(atom6.c)){dir180} ..
{dir90}(xpart(atom6.w)-hGap,ypart(atom6.c)+lineRadius) --
(xpart(atom6.w)-hGap,ypart(OutputList.s)-tGap-lineRadius){dir90} ..
{dir0}(xpart(atom6.w)+lineRadius-hGap,ypart(OutputList.s)-tGap);
drawarrow (xpart(atom6.e)+hGap-lineRadius,ypart(OutputList.s)-tGap){dir0} ..
{dir270}(xpart(atom6.e)+hGap,ypart(OutputList.s)-2tGap-lineRadius) --
(xpart(atom6.e)+hGap,ypart(atom6.e)+lineRadius){dir270} ..
{dir180}(xpart(atom6.e)+hGap-lineRadius,ypart(atom6.e)) -- atom6.e;

endfig;

end

% $Log: map1drails.mp,v $
% Revision 1.3  2006/07/20 04:47:52  churchoflambda
% Re-added the files to record their changes.
%
% Revision 1.1  2005/08/02 15:08:22  churchoflambda
% Added rail diagrams for listen, map1d, map2d, map3d and pfsm.
%