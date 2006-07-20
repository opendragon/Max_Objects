% File: pfsmstate.mp

input boxes;
beginfig(-1);
verbatimtex
%&latex
\documentclass{article}
\usepackage{color}
\newcommand{\gotocour}{\fontfamily{pcr}\fontseries{b}\fontshape{n}\fontsize{7}{8}\selectfont}
\newcommand{\gotohel}{\fontfamily{phv}\fontseries{m}\fontshape{n}\fontsize{7}{8}\selectfont}
\newcommand{\forcircle}[1]{\makebox[30pt]{\gotocour #1}}
\begin{document}
etex;

input maxboxmacros;

numeric gap;
gap := 50;

pair cwidth, cheight;

circmargin := 4;

circleit.In(btex \gotocour in etex);
In.col = 0; % black
circleit.Alpha(btex \forcircle{alpha} etex);
Alpha.col = 2; % green
circleit.Beta(btex \forcircle{beta} etex);
Beta.col = 0; % black
circleit.Gamma(btex \forcircle{gamma} etex);
Gamma.col = 0; % black
circleit.Omega(btex \forcircle{omega} etex);
Omega.col = 1; % red
circleit.Theta(btex \forcircle{theta} etex);
Theta.col = 3; % blue
circleit.Test(btex \forcircle{test} etex);
Test.col = 0; % black

In.e = Alpha.w + (-gap,0);
Alpha.s = Beta.n + (0,gap);
Beta.s = Theta.n + (0,gap);
Gamma.w = Beta.e + (2gap,0);
Omega.w = Alpha.e + (2gap,0);
Test.w = Theta.e + (2gap,0);

drawunboxed(In);
drawshadowed(Alpha,Beta,Gamma,Omega,Theta,Test);

label.ulft("", cuta(In,Alpha) In.c..Alpha.c);
label.lft(btex \gotohel 'b\ldots'g etex, cuta(Alpha,Theta) Alpha.c{dir225}..Theta.c);
label.lft(btex \gotohel 42 etex, cuta(Alpha,Beta) Alpha.c{dir255}..Beta.c);
label.rt(btex \gotohel 'a etex, cuta(Alpha,Beta) Alpha.c{dir285}..Beta.c);
label.rt(btex \gotohel $<$symbol$>$ etex, cuta(Alpha,Gamma) Alpha.c{dir340}..Gamma.c);
label.rt(btex \gotohel blorg etex, cuta(Beta,Alpha) Beta.c{dir35}..Alpha.c);
label.bot(btex \gotohel $<$number$>$ etex, cuta(Gamma,Beta) Gamma.c{dir210}..Beta.c);
label.rt(btex \gotohel blirg etex, cuta(Beta,Theta) Beta.c..Theta.c);
label.lrt(btex \gotohel $<$number$>$ etex, self.Theta(35,-30));
label.llft(btex \gotohel $<$symbol$>$ etex, self.Theta(-35,-30));
label.top(btex \gotohel $<$symbol$>$ etex, cuta(Gamma,Beta) Gamma.c{dir150}..Beta.c);
label.bot(btex \gotohel $<$symbol$>$ etex, self.Gamma(30,-35));
label.rt(btex \gotohel garbage etex, self.Test(35,0));

endfig;
verbatimtex
\end{document}
etex
end

% $Log: pfsmstate.mp,v $
% Revision 1.4  2006/07/20 04:47:53  churchoflambda
% Re-added the files to record their changes.
%
% Revision 1.2  2005/08/02 15:07:09  churchoflambda
% Added CVS tags; add rail diagrams for pfsm, map1d, map2d, map3d and listen.
%
