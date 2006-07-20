% File: tcpClientstate.mp

input boxes;
beginfig(-1);
verbatimtex
%&latex
\documentclass{article}
\usepackage{color}
\newcommand{\gotocour}{\fontfamily{pcr}\fontseries{b}\fontshape{n}\fontsize{7}{8}\selectfont}
\newcommand{\gotohel}{\fontfamily{phv}\fontseries{m}\fontshape{n}\fontsize{9}{10}\selectfont}
\newcommand{\forcircle}[1]{\makebox[40pt]{\gotocour #1}}
\newcommand{\twolines}[2]{$\displaystyle{\matrix{\mbox{\gotohel #1}\cr\mbox{\gotohel #2}\cr}}$}
\begin{document}
etex;

input maxboxmacros;

numeric gap;
gap := 50;

pair cwidth, cheight;

circmargin := 4;

circleit.In(btex \gotocour in etex);
In.col = 0; % black
circleit.Inactive(btex \forcircle{inactive} etex);
Inactive.col = 2; % green
circleit.Active(btex \forcircle{active} etex);
Active.col = 0; % black

In.e = Inactive.w - (0.75gap,0);
Inactive.e = Active.w - (gap,0);

drawunboxed(In);
drawshadowed(Inactive,Active);

label.ulft("", cuta(In,Inactive) In.c..Inactive.c);
label.top(btex \gotohel connect etex, cuta(Inactive,Active) Inactive.c{dir30}..Active.c);
label.bot(btex \gotohel disconnect etex, cuta(Active,Inactive) Active.c{dir210}..Inactive.c);
label.rt(btex \gotohel receive/send data etex, self.Active(gap,0));

endfig;
verbatimtex
\end{document}
etex
end

% $Log: tcpClientstate.mp,v $
% Revision 1.4  2006/07/20 04:47:54  churchoflambda
% Re-added the files to record their changes.
%
% Revision 1.2  2005/08/02 15:07:09  churchoflambda
% Added CVS tags; add rail diagrams for pfsm, map1d, map2d, map3d and listen.
%
