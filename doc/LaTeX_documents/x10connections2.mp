% File: x10connections2.mp

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
p := thelabel.rt(btex \objectText{x10} etex, (0,0));
create_inside_box_no_pins((0, 60), p, 2, 9, 1);

p := thelabel.rt(btex \objectText{serialX} etex, (0,0));
create_inside_box_no_pins((0, 0), p, 1, 3, 2);

% Build the connections:
numeric pt, slop;
pt := 1;
slop := 20;

pair cv, ahcv;
cv := (0, 0.5bandThick);
ahcv := (0, 0.25ahlength);

linecap := butt;
pickup pencircle scaled pt;
drawoptions(withcolor red);

drawarrow (co[1][1]-cv) -- (ci[2][1]+cv+ahcv);
drawarrow (co[1][2]-cv) -- (xpart(co[1][2]),ypart(0.5[co[1][1],ci[2][1]])) --
		(0.5[co[1][1],ci[2][1]]);
drawarrow (co[2][1]-cv) -- (co[2][1]-(0,slop)) -- (xpart(ci[1][2])+slop,ypart(co[2][1])-slop) --
		(ci[1][2]+(slop,slop)) -- (ci[1][2]+(0,slop)) -- (ci[1][2]+cv+ahcv);

endfig;
verbatimtex
\end{document}
etex
end

% $Log: x10connections2.mp,v $
% Revision 1.1  2006/03/25 21:51:18  churchoflambda
% Added the 'senseX' object and modified the connection diagrams to show 'serial' as well as 'serialX'.
%
% Revision 1.2  2005/08/02 15:07:10  churchoflambda
% Added CVS tags; add rail diagrams for pfsm, map1d, map2d, map3d and listen.
%
