% File: tcpServersymbol.mp

input boxes;
beginfig(-1);
verbatimtex
%&latex
\documentclass{article}
\usepackage{color}
\newcommand{\gotohel}{\fontfamily{phv}\fontseries{m}\fontshape{n}\fontsize{12}{14}\selectfont}
\newcommand{\gotocour}{\fontfamily{pcr}\fontseries{b}\fontshape{n}\fontsize{7}{8}\selectfont}
\newcommand{\objectText}[2]{{\hspace*{2pt}\rule[-2pt]{0pt}{12pt}\gotohel #1 %
     \fontshape{sl}\selectfont #2\hspace*{12pt}}}
\begin{document}
etex;

% Define pin pictures:
picture qn[];
qn[1] := thelabel(btex \gotocour 1 etex, (0,0));
qn[2] := thelabel(btex \gotocour 2 etex, (0,0));
qn[3] := thelabel(btex \gotocour 3 etex, (0,0));
qn[4] := thelabel(btex \gotocour 4 etex, (0,0));
qn[5] := thelabel(btex \gotocour 5 etex, (0,0));
qn[6] := thelabel(btex \gotocour 6 etex, (0,0));
qn[7] := thelabel(btex \gotocour 7 etex, (0,0));
qn[8] := thelabel(btex \gotocour 8 etex, (0,0));
qn[9] := thelabel(btex \gotocour 9 etex, (0,0));
qn[10] := thelabel(btex \gotocour 10 etex, (0,0));
qn[11] := thelabel(btex \gotocour 11 etex, (0,0));
qn[12] := thelabel(btex \gotocour 12 etex, (0,0));

input maxboxmacros;

% Create the text box:

picture p;
p := thelabel.rt(btex \objectText{tcpServer}{[\,port buffers\,]} etex, (0,0));
create_inside_box(p, 1, 2);

endfig;
verbatimtex
\end{document}
etex
end

% $Log: tcpServersymbol.mp,v $
% Revision 1.4  2006/07/20 04:47:54  churchoflambda
% Re-added the files to record their changes.
%
% Revision 1.2  2005/08/02 15:07:10  churchoflambda
% Added CVS tags; add rail diagrams for pfsm, map1d, map2d, map3d and listen.
%
