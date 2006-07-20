% File: tcpServerstate.mp

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
circleit.Listening(btex \forcircle{listening} etex);
Listening.col = 0; % black
circleit.Connected(btex \forcircle{connected} etex);
Connected.col = 0; % black

In.e = Inactive.w - (0.5gap,0);
Inactive.e = Listening.w - (0.75gap,0);
Listening.e = Connected.w - (0.75gap,0);

drawunboxed(In);
drawshadowed(Inactive,Listening,Connected);

label.ulft("", cuta(In,Inactive) In.c..Inactive.c);
label.top(btex \twolines{listen}{on} etex, cuta(Inactive,Listening) Inactive.c{dir40}..Listening.c);
label.bot(btex \twolines{listen}{off} etex, cuta(Listening,Inactive) Listening.c{dir220}..Inactive.c);
label.top(btex \twolines{client}{connect} etex, cuta(Listening,Connected) Listening.c{dir40}..Connected.c);
label.bot(btex \gotohel disconnect etex, cuta(Connected,Listening) Connected.c{dir220}..Listening.c);
label.rt(btex \twolines{receive/send}{data} etex, self.Connected(gap,0));

endfig;
verbatimtex
\end{document}
etex
end

% $Log: tcpServerstate.mp,v $
% Revision 1.4  2006/07/20 04:47:54  churchoflambda
% Re-added the files to record their changes.
%
% Revision 1.2  2005/08/02 15:07:10  churchoflambda
% Added CVS tags; add rail diagrams for pfsm, map1d, map2d, map3d and listen.
%
