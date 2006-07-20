% File: mtcstate.mp

input boxes;
beginfig(-1);
verbatimtex
%&latex
\documentclass{article}
\usepackage{color}
\newcommand{\gotocour}{\fontfamily{pcr}\fontseries{b}\fontshape{n}\fontsize{7}{8}\selectfont}
\newcommand{\gotohel}{\fontfamily{phv}\fontseries{m}\fontshape{n}\fontsize{9}{10}\selectfont}
\newcommand{\forcircle}[1]{\makebox[30pt]{\gotocour #1}}
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
circleit.Idle(btex \forcircle{idle} etex);
Idle.col = 2; % green
circleit.Sampling(btex \forcircle{sampling} etex);
Sampling.col = 0; % black
circleit.Training(btex \forcircle{training} etex);
Training.col = 0; % black

In.s = Idle.n + (0,0.75gap);
Idle.s = Sampling.n + (1.5gap,gap);
Idle.s = Training.n + (-1.5gap,gap);

drawunboxed(In);
drawshadowed(Idle,Sampling,Training);

label.ulft("", cuta(In,Idle) In.c..Idle.c);
label.lft(btex \gotohel start etex, cuta(Idle,Sampling) Idle.c{dir210}..Sampling.c);
label.rt(btex \gotohel stop etex, cuta(Sampling,Idle) Sampling.c{dir30}..Idle.c);
label.urt(btex \twolines{train}{start} etex, cuta(Idle,Training) Idle.c{dir325}..Training.c);
label.llft(btex \twolines{train}{stop} etex, cuta(Training,Idle) Training.c{dir145}..Idle.c);

endfig;
verbatimtex
\end{document}
etex
end

% $Log: mtcstate.mp,v $
% Revision 1.4  2006/07/20 04:47:53  churchoflambda
% Re-added the files to record their changes.
%
% Revision 1.2  2005/08/02 15:07:09  churchoflambda
% Added CVS tags; add rail diagrams for pfsm, map1d, map2d, map3d and listen.
%
