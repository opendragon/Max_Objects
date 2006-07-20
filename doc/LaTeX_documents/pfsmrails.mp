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

% StateFile/1

newBox.StateFile(btex StateFile etex) "framed(false)";
StateFile.c = origin;
drawObj(StateFile);
newBox.stateList1A(btex \textit{StateList} etex);
stateList1A.w = StateFile.w + (gap,-.5gap);
drawBox(stateList1A);
newBox.state1(btex \textit{State} etex);
state1.w = stateList1A.e + (.5gap,0);
drawObj(state1);
newBox.stateList1B(btex \textit{StateList} etex);
stateList1B.w = state1.e + (.5gap,0);
drawObj(stateList1B);
newBox.transitions1(btex \textit{Transitions} etex);
transitions1.w = stateList1B.e + (.5gap,0);
drawObj(transitions1);
drawarrow (xpart(StateFile.w),ypart(stateList1A.c)) -- stateList1A.w;
drawarrow stateList1A.e -- state1.w;
drawarrow state1.e -- stateList1B.w;
drawarrow stateList1B.e -- transitions1.w;
drawarrow transitions1.e -- (xpart(transitions1.e)+gap,ypart(transitions1.e));

% State/2

newBox.State(btex State etex) "framed(false)";
State.w = StateFile.w + (0,-gap);
drawObj(State);
newFixedBox.symbol2(btex Symbol etex) "width(45pt)";
symbol2.w = State.w + (gap,-.5gap);
drawObj(symbol2);
drawarrow (xpart(State.w),ypart(symbol2.c)) -- symbol2.w;
drawarrow symbol2.e -- (xpart(symbol2.e)+gap,ypart(symbol2.e));

% StateList/3

newBox.StateList(btex StateList etex) "framed(false)";
StateList.w = State.w + (0,-gap);
drawObj(StateList);
newFixedBox.openSquare3(btex $[$ etex);
openSquare3.w = StateList.w + (gap,-.5gap);
drawObj(openSquare3);
newBox.state3(btex \textit{State} etex);
state3.w = openSquare3.e + (.5gap+hGap,0);
drawObj(state3);
newFixedBox.closeSquare3(btex $]$ etex);
closeSquare3.w = state3.e + (.5gap+hGap,0);
drawObj(closeSquare3);
drawarrow (xpart(StateList.w),ypart(openSquare3.w)) -- openSquare3.w;
drawarrow closeSquare3.e -- (xpart(closeSquare3.e)+hGap+gap,ypart(closeSquare3.e));
drawarrow openSquare3.e -- state3.w;
drawarrow state3.e -- closeSquare3.w;
drawarrow (xpart(state3.e)+hGap-lineRadius,ypart(state3.e)){dir0} ..
			{dir270}(xpart(state3.e)+hGap,ypart(state3.e)-lineRadius) --
			(xpart(state3.e)+hGap,ypart(state3.e)-.25gap-tGap){dir270} ..
			{dir180}(xpart(state3.e)+hGap-lineRadius,ypart(state3.e)-.25gap-tGap-lineRadius);
drawarrow (xpart(state3.e)+hGap-lineRadius,ypart(state3.e)-.25gap-tGap-lineRadius) --
			(xpart(state3.w)+lineRadius-hGap,ypart(state3.e)-.25gap-tGap-lineRadius);
drawarrow (xpart(state3.w)+lineRadius-hGap,ypart(state3.w)-.25gap-tGap-lineRadius){dir180} ..
			(xpart(state3.w)-hGap,ypart(state3.w)-.25gap-tGap) --
			(xpart(state3.w)-hGap,ypart(state3.w)-lineRadius){dir90} ..
			{dir0}(xpart(state3.w)+lineRadius-hGap,ypart(state3.w));

% Transitions/4

newBox.Transitions(btex Transitions etex) "framed(false)";
Transitions.w = StateList.w + (0,-(gap+tGap));
drawObj(Transitions);
newBox.condition4(btex \textit{Condition} etex);
condition4.w = Transitions.w + (gap,-.5gap);
drawObj(condition4);
newBox.outputList4(btex \textit{OutputList} etex);
outputList4.w = condition4.e + (.5gap,0);
drawObj(outputList4);
newFixedBox.semicolon4(btex $;$ etex);
semicolon4.w = outputList4.e + (.5gap,0);
drawObj(semicolon4);
drawarrow condition4.e -- outputList4.w;
drawarrow outputList4.e -- semicolon4.w;
drawarrow (xpart(Transitions.w),ypart(condition4.w)) -- condition4.w;
drawarrow semicolon4.e -- (xpart(semicolon4.e)+hGap+gap,ypart(semicolon4.e));
drawarrow (xpart(semicolon4.e)+hGap-lineRadius,ypart(semicolon4.e)){dir0} ..
			{dir270}(xpart(semicolon4.e)+hGap,ypart(semicolon4.e)-lineRadius) --
			(xpart(semicolon4.e)+hGap,ypart(semicolon4.e)-.25gap-tGap){dir270} ..
			{dir180}(xpart(semicolon4.e)+hGap-lineRadius,ypart(semicolon4.e)-.25gap-tGap-lineRadius);
drawarrow (xpart(semicolon4.e)+hGap-lineRadius,ypart(semicolon4.e)-.25gap-tGap-lineRadius) --
			(xpart(condition4.w)+lineRadius-hGap,ypart(semicolon4.e)-.25gap-tGap-lineRadius);
drawarrow (xpart(condition4.w)+lineRadius-hGap,ypart(condition4.w)-.25gap-tGap-lineRadius){dir180} ..
			(xpart(condition4.w)-hGap,ypart(condition4.w)-.25gap-tGap) --
			(xpart(condition4.w)-hGap,ypart(condition4.w)-lineRadius){dir90} ..
			{dir0}(xpart(condition4.w)+lineRadius-hGap,ypart(condition4.w));

% OutputList/5

newBox.OutputList(btex OutputList etex) "framed(false)";
OutputList.w = Transitions.w + (0,-(gap+4tGap));
drawObj(OutputList);
newFixedBox.atom5(btex Atom etex) "width(40pt)";
atom5.w = OutputList.w + (gap,-(.5gap+3tGap));
drawObj(atom5);
drawarrow (xpart(OutputList.w),ypart(OutputList.s)-tGap) --
((xpart(atom5.e)+gap+hGap+xpart(OutputList.w))/2,ypart(OutputList.s)-tGap);
drawarrow ((xpart(atom5.e)+gap+hGap+xpart(OutputList.w))/2,ypart(OutputList.s)-tGap) --
(xpart(atom5.e)+gap+hGap,ypart(OutputList.s)-tGap);
drawarrow atom5.w --
(xpart(atom5.w)+lineRadius-hGap,ypart(atom5.c)){dir180} ..
{dir90}(xpart(atom5.w)-hGap,ypart(atom5.c)+lineRadius) --
(xpart(atom5.w)-hGap,ypart(OutputList.s)-tGap-lineRadius){dir90} ..
{dir0}(xpart(atom5.w)+lineRadius-hGap,ypart(OutputList.s)-tGap);
drawarrow (xpart(atom5.e)+hGap-lineRadius,ypart(OutputList.s)-tGap){dir0} ..
{dir270}(xpart(atom5.e)+hGap,ypart(OutputList.s)-2tGap-lineRadius) --
(xpart(atom5.e)+hGap,ypart(atom5.e)+lineRadius){dir270} ..
{dir180}(xpart(atom5.e)+hGap-lineRadius,ypart(atom5.e)) -- atom5.e;

% Condition/6

newBox.Condition(btex Condition etex) "framed(false)";
Condition.w = OutputList.w + (0,-(gap+3tGap));
drawObj(Condition);
newBox.state6A(btex \textit{State} etex);
state6A.w = Condition.w + (gap,-.5gap);
drawObj(state6A);
newBox.match6(btex \textit{Match} etex);
match6.w = state6A.e + (.5gap,0);
drawObj(match6);
newBox.transition6(btex \textit{Transition} etex);
transition6.w = match6.e + (.5gap,0);
drawObj(transition6);
newBox.state6B(btex \textit{State} etex);
state6B.w = transition6.e + (.5gap,0);
drawObj(state6B);
drawarrow (xpart(Condition.w),ypart(state6A.w)) -- state6A.w;
drawarrow state6A.e -- match6.w;
drawarrow match6.e -- transition6.w;
drawarrow transition6.e -- state6B.w;
drawarrow state6B.e -- (xpart(state6B.e)+gap,ypart(state6B.e));

% Match/7

newBox.Match(btex Match etex) "framed(false)";
Match.w = Condition.w + (0,-gap);
drawObj(Match);


newFixedBox.atR7(btex $@$r etex) "width(25pt)";
atR7.w = Match.w + (gap,-.5gap);
drawObj(atR7);
newFixedBox.number7A(btex Number etex) "width(45pt)";
number7A.w = atR7.e + (.5gap,0);
drawObj(number7A);
newFixedBox.number7B(btex Number etex) "width(45pt)";
number7B.w = number7A.e + (.5gap,0);
drawObj(number7B);
newFixedBox.atS7(btex $@$s etex) "width(25pt)";
atS7.w = atR7.w + (0,-(.5gap+2tGap));
drawObj(atS7);
newFixedBox.atN7(btex $@$n etex) "width(25pt)";
atN7.w = atS7.w + (0,-(.5gap+2tGap));
drawObj(atN7);
newFixedBox.atom7(btex Atom etex) "width(40pt)";
atom7.w = atN7.w + (0,-(.5gap+2tGap));
drawObj(atom7);
drawarrow (xpart(Match.w),ypart(atR7.w)) -- atR7.w;
drawarrow (xpart(atR7.w)-hGap-lineRadius,ypart(atR7.w)){dir0} ..
	{dir270}(xpart(atR7.w)-hGap,ypart(atR7.w)-lineRadius) --
	(xpart(atR7.w)-hGap,ypart(atom7.w)+lineRadius){dir270} ..
	{dir0}(xpart(atR7.w)+lineRadius-hGap,ypart(atom7.w)) --
	atom7.w;
drawarrow (xpart(atR7.w)-hGap,ypart(atS7.w)+lineRadius){dir270} ..
	{dir0}(xpart(atR7.w)+lineRadius-hGap,ypart(atS7.w)) --
	atS7.w;
drawarrow (xpart(atR7.w)-hGap,ypart(atN7.w)+lineRadius){dir270} ..
	{dir0}(xpart(atR7.w)+lineRadius-hGap,ypart(atN7.w)) --
	atN7.w;
drawarrow atR7.e -- number7A.w;
drawarrow number7A.e -- number7B.w;
drawarrow number7B.e -- (xpart(number7B.e)+gap,ypart(number7B.e));
drawarrow atom7.e --
	(xpart(number7B.e)+hGap-lineRadius,ypart(atom7.e)){dir0} ..
	{dir90}(xpart(number7B.e)+hGap,ypart(atom7.e)+lineRadius) --
	(xpart(number7B.e)+hGap,ypart(number7B.e)-lineRadius){dir90} ..
	{dir0}(xpart(number7B.e)+hGap+lineRadius,ypart(number7B.e));
drawarrow atS7.e --
	(xpart(number7B.e)+hGap-lineRadius,ypart(atS7.e)){dir0} ..
	{dir90}(xpart(number7B.e)+hGap,ypart(atS7.e)+lineRadius);
drawarrow atN7.e --
	(xpart(number7B.e)+hGap-lineRadius,ypart(atN7.e)){dir0} ..
	{dir90}(xpart(number7B.e)+hGap,ypart(atN7.e)+lineRadius);

% Transition/8

newBox.Transition(btex Transition etex) "framed(false)";
Transition.w = Match.w + (0,-3gap);
drawObj(Transition);
newFixedBox.unconditional8(btex $-$$>$ etex) "width(20pt)";
unconditional8.w = Transition.w + (gap,-.5gap); 
drawObj(unconditional8);
newFixedBox.conditional8(btex $-$? etex) "width(20pt)";
conditional8.w = unconditional8.w + (0,-(.5gap+2tGap));
drawObj(conditional8);
newFixedBox.number8(btex Number etex) "width(45pt)";
number8.w = conditional8.e + (.5gap,0);
drawObj(number8); 
drawarrow (xpart(Transition.w),ypart(unconditional8.c)) -- unconditional8.w;
drawarrow (xpart(unconditional8.w)-hGap-lineRadius,ypart(unconditional8.c)){dir0} ..
{dir270}(xpart(unconditional8.w)-hGap,ypart(unconditional8.c)-lineRadius) --
(xpart(unconditional8.w)-hGap,ypart(conditional8.c)+lineRadius){dir270} ..
{dir0}(xpart(unconditional8.w)+lineRadius-hGap,ypart(conditional8.c)) --
conditional8.w;
drawarrow conditional8.e -- number8.w;
drawarrow unconditional8.e --
(xpart(number8.e)+gap+hGap,ypart(unconditional8.e));
drawarrow number8.e --
(xpart(number8.e)+hGap-lineRadius,ypart(number8.e)){dir0} ..
{dir90}(xpart(number8.e)+hGap,ypart(number8.e)+lineRadius) --
(xpart(number8.e)+hGap,ypart(unconditional8.e)-lineRadius){dir90} ..
{dir0}(xpart(number8.e)+hGap+lineRadius,ypart(unconditional8.e));

endfig;

end

% $Log: pfsmrails.mp,v $
% Revision 1.3  2006/07/20 04:47:53  churchoflambda
% Re-added the files to record their changes.
%
% Revision 1.1  2005/08/02 15:08:22  churchoflambda
% Added rail diagrams for listen, map1d, map2d, map3d and pfsm.
%