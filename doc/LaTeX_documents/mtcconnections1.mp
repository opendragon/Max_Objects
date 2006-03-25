% File: mtcconnections1.mp

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
p := thelabel.rt(btex \objectText{mtc} etex, (0,0));
create_inside_box_no_pins((0, 100), p, 2, 7, 1);

p := thelabel.rt(btex \objectText{serial} etex, (0,0));
create_inside_box_no_pins((150, 0), p, 1, 3, 2);

p := thelabel.rt(btex \objectText{chunk 1} etex, (0,0));
create_hollow_box((200, 50), p, 1, 1, 3);

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

z1 = (xpart(0.5[ci[1][2],co[2][1]]),ypart(0.5[ci[3][1],co[1][6]]));
z2 = (xpart(0.5[ci[1][2],co[2][1]]),ypart(0.66[ci[2][1],co[1][5]]));
z3 = (xpart(0.5[ci[1][2],co[2][1]]),ypart(0.33[ci[2][1],co[1][4]]));
drawarrow (co[2][1]-cv) -- (co[2][1]-(0,slop)) -- (xpart(0.5[ci[1][2],co[2][1]]),ypart(co[2][1])-slop) --
		(xpart(0.5[ci[1][2],co[2][1]]),ypart(ci[1][2])+slop) -- (ci[1][2]+(0,slop)) -- (ci[1][2]+cv+ahcv);
drawarrow (co[3][1]-cv) -- (xpart(co[3][1]),ypart(0.5[ci[2][1],co[3][1]])) --
		(xpart(ci[2][1]),ypart(0.5[ci[2][1],co[3][1]])) -- (ci[2][1]+cv+ahcv);

draw (co[1][6]-cv) -- (xpart(co[1][6]),ypart(0.5[ci[3][1],co[1][6]])) -- (z1-(0.5igap,0));
drawarrow (z1+(0.5igap,0)) -- (xpart(ci[3][1]),ypart(0.5[ci[3][1],co[1][6]])) -- (ci[3][1]+cv+ahcv);

draw (co[1][5]-cv) -- (xpart(co[1][5]),ypart(0.66[ci[2][1],co[1][5]])) -- (z2-(0.5igap,0));
drawarrow (z2+(0.5igap,0)) -- (xpart(ci[2][1]),ypart(0.66[ci[2][1],co[1][5]])) --
		(xpart(ci[2][1]),ypart(0.5[ci[2][1],co[3][1]]));

draw (co[1][4]-cv) -- (xpart(co[1][4]),ypart(0.33[ci[2][1],co[1][4]])) -- (z3-(0.5igap,0));
drawarrow (z3+(0.5igap,0)) -- (xpart(ci[2][1]),ypart(0.33[ci[2][1],co[1][4]]));

endfig;
verbatimtex
\end{document}
etex
end

% $Log: mtcconnections1.mp,v $
% Revision 1.1  2006/03/25 21:51:18  churchoflambda
% Added the 'senseX' object and modified the connection diagrams to show 'serial' as well as 'serialX'.
%
% Revision 1.2  2005/08/02 15:07:09  churchoflambda
% Added CVS tags; add rail diagrams for pfsm, map1d, map2d, map3d and listen.
%
