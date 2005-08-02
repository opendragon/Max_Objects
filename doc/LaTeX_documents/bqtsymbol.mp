% File: bqtsymbol.mp

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

label.rt(btex \gotohel{} [No image---\fontshape{sl}\selectfont bqt \fontshape{n}\selectfont is a user interface object] etex, (0,0));

endfig;
verbatimtex
\end{document}
etex
end

% $Log: bqtsymbol.mp,v $
% Revision 1.4  2005/08/02 15:07:08  churchoflambda
% Added CVS tags; add rail diagrams for pfsm, map1d, map2d, map3d and listen.
%

