% File: gvp100connections.mp

input boxes;
beginfig(-1);
verbatimtex
%&latex
\documentclass{article}
\usepackage{color}
\newcommand{\gotohel}{\fontfamily{phv}\fontseries{m}\fontshape{n}\fontsize{12}{14}\selectfont}
\newcommand{\objectText}[1]{{\hspace*{2pt}\rule[-2pt]{0pt}{12pt}\gotohel #1\hspace*{10pt}}}
\begin{document}
etex;

% Create the text boxes:
input maxboxmacros;

picture p;
p := thelabel.rt(btex \objectText{gvp100} etex, (0,0));
create_inside_box_no_pins((0, 0), p, 2, 6, 1);

p := thelabel.rt(btex \objectText{serialX} etex, (0,0));
create_inside_box_no_pins((180, 0), p, 1, 3, 2);

p := thelabel.rt(btex \objectText{breakdone} etex, (0,0));
create_hollow_box((0, 60), p, 1, 1, 3);

p := thelabel.rt(btex \objectText{break} etex, (0,0));
create_hollow_box((200, 60), p, 1, 1, 4);

% Build the connections:
numeric pt, slop, igap;
pt := 1;
slop := 20;
igap := 6;

pair cv, ahcv;
cv := (0, 0.5bandThick);
ahcv := (0, 0.25ahlength);

linecap := butt;
pickup pencircle scaled pt;
drawoptions(withcolor red);

z1 = (xpart(0.25[ci[1][2],co[2][1]]),-10);
z2 = (xpart(0.25[ci[1][2],co[2][1]]),-20);
drawarrow (co[3][1]-cv) -- (ci[1][1]+cv+ahcv);
drawarrow (co[2][3]-cv) -- (co[2][3]+(0, -slop)) -- (co[2][3]+(2slop, -slop)) --
		(xpart(co[2][3])+2slop, 120) -- (xpart(ci[3][1]), 120) -- (ci[3][1]+cv+ahcv);
drawarrow (co[2][1]-cv) -- (xpart(co[2][1]), -30) -- (xpart(0.25[ci[1][2],co[2][1]]), -30) --
		(xpart(0.25[ci[1][2],co[2][1]]),ypart(ci[1][2])+slop) -- (ci[1][2]+(0,slop)) -- (ci[1][2]+cv+ahcv);

draw (co[1][5]-cv) -- (xpart(co[1][5]),-10) -- (z1-(0.5igap,0));
drawarrow (z1+(0.5igap,0)) -- (xpart(0.5[ci[1][2],co[2][1]]),-10) --
		(xpart(0.5[ci[1][2],co[2][1]]),ypart(ci[4][1])+slop) -- (ci[4][1]+(0,slop)) -- (ci[4][1]+cv+ahcv);

drawarrow (co[4][1]-cv) -- (xpart(co[4][1]),ypart(0.5[ci[2][1],co[4][1]])) --
		(xpart(ci[2][1]),ypart(0.5[ci[2][1],co[4][1]])) -- (ci[2][1]+cv+ahcv);

draw (co[1][3]-cv) -- (xpart(co[1][3]),-20) -- (z2-(0.5igap,0));
drawarrow (z2+(0.5igap,0)) -- (xpart(0.75[ci[1][2],co[2][1]]),-20) --
		(xpart(0.75[ci[1][2],co[2][1]]),ypart(co[4][1])) -- (xpart(ci[2][1]),ypart(co[4][1])) --
		(xpart(ci[2][1]),ypart(0.5[ci[2][1],co[4][1]]));

drawarrow (co[1][4]-cv) -- (xpart(co[1][4]),-20);

endfig;
verbatimtex
\end{document}
etex
end

% $Log: gvp100connections.mp,v $
% Revision 1.4  2006/07/20 04:47:52  churchoflambda
% Re-added the files to record their changes.
%
% Revision 1.2  2005/08/02 15:07:09  churchoflambda
% Added CVS tags; add rail diagrams for pfsm, map1d, map2d, map3d and listen.
%

