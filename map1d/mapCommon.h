/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       mapCommon.h                                                             */
/*                                                                                      */
/*  Contains:   The data type declarations and forward references to routines.          */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2001 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
/*                                                                                      */
/*              All rights reserved. Redistribution and use in source and binary forms, */
/*              with or without modification, are permitted provided that the following */
/*              conditions are met:                                                     */
/*                * Redistributions of source code must retain the above copyright      */
/*                  notice, this list of conditions and the following disclaimer.       */
/*                * Redistributions in binary form must reproduce the above copyright   */
/*                  notice, this list of conditions and the following disclaimer in the */
/*                  documentation and/or other materials provided with the              */
/*                  distribution.                                                       */
/*                * Neither the name of the copyright holders nor the names of its      */
/*                  contributors may be used to endorse or promote products derived     */
/*                  from this software without specific prior written permission.       */
/*                                                                                      */
/*              THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS     */
/*              "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT       */
/*              LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A */
/*              PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT      */
/*              OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,   */
/*              SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT        */
/*              LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,   */
/*              DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY   */
/*              THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT     */
/*              (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE   */
/*              OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.    */
/*                                                                                      */
/*  Created:    2001/05/01                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(MAPCOMMON_H_))
 #define MAPCOMMON_H_ /* */

 #include "MissingAndExtra.h"
 #include "genericListOutput.h"

 #define kCommentCharacter '#'

enum Category
{
  MatchInteger,
  MatchFloat,
  MatchInfinity,
  MatchUnknown
}; /* Category */

struct FloatOrInteger
{
  union
  {
    float fFloat;
    long  fInteger;
  } fValue;
  bool fIsFloat;
}; /* FloatOrInteger */

struct RangeElement
{
  FloatOrInteger fValue;
  Category       fKind;
  bool           fIsClosed;
}; /* RangeElement */

inline float getFOIFloat
  (const FloatOrInteger & value)
{
  return value.fValue.fFloat;
} /* getFOIFloat */

inline long getFOILong
  (const FloatOrInteger & value)
{
  return value.fValue.fInteger;
} /* getFOILong */

inline float getFOIValue
  (const FloatOrInteger & value)
{
  return (value.fIsFloat ? value.fValue.fFloat : value.fValue.fInteger);
} /* getFOIValue */

inline void setFOI2Float
  (FloatOrInteger & target,
   const float      value)
{
  target.fIsFloat = true;
  target.fValue.fFloat = value;
} /* setFOI2Float */

inline void setFOI2Integer
  (FloatOrInteger & target,
   const long       value)
{
  target.fIsFloat = false;
  target.fValue.fInteger = value;
} /* setFOI2Integer */

typedef RangeElement * RangeElementPtr;

 #define MapRoutineDeclarations(ObjPtr) \
Pvoid cmd_Add\
  (ObjPtr  xx,\
   PSymbol message,\
   short   argc,\
   PAtom   argv);\
\
Pvoid cmd_After\
  (ObjPtr  xx,\
   PSymbol message,\
   short   argc,\
   PAtom   argv);\
\
Pvoid cmd_Before\
  (ObjPtr  xx,\
   PSymbol message,\
   short   argc,\
   PAtom   argv);\
\
Pvoid cmd_Clear\
  (ObjPtr xx);\
\
Pvoid cmd_Count\
  (ObjPtr xx);\
\
Pvoid cmd_Delete\
  (ObjPtr xx,\
   long   num);\
\
Pvoid cmd_Dump\
  (ObjPtr xx);\
\
Pvoid cmd_Get\
  (ObjPtr  xx,\
   long    num,\
   PSymbol which);\
\
Pvoid cmd_Load\
  (ObjPtr  xx,\
   PSymbol fileName);\
\
Pvoid cmd_Replace\
  (ObjPtr  xx,\
   PSymbol message,\
   short   argc,\
   PAtom   argv);\
\
Pvoid cmd_Set\
  (ObjPtr  xx,\
   PSymbol message,\
   short   argc,\
   PAtom   argv);\
\
Pvoid cmd_Show\
  (ObjPtr xx,\
   long   num);\
\
Pvoid cmd_Verbose\
  (ObjPtr  xx,\
   PSymbol onOff);

 #define SetUpSymbols() \
gAsteriskSymbol = gensym("*");\
gCloseRoundSymbol = gensym(")");\
gCloseSquareSymbol = gensym("]");\
gCountSymbol = gensym("count");\
gDollarSymbol = gensym("$");\
gDollarXSymbol = gensym("$x");\
gDollarYSymbol = gensym("$y");\
gDollarZSymbol = gensym("$z");\
gDoubleDollarSymbol = gensym("$$");\
gDoubleDollarXSymbol = gensym("$$x");\
gDoubleDollarYSymbol = gensym("$$y");\
gDoubleDollarZSymbol = gensym("$$z");\
gEmptySymbol = gensym("");\
gNegInfSymbol1 = gensym("-inf");\
gNegInfSymbol2 = gensym("-°");\
gOffSymbol = gensym("off");\
gOnSymbol = gensym("on");\
gOpenRoundSymbol = gensym("(");\
gOpenSquareSymbol = gensym("[");\
gPosInfSymbol1 = gensym("inf");\
gPosInfSymbol2 = gensym("+inf");\
gPosInfSymbol3 = gensym("°");\
gPosInfSymbol4 = gensym("+°");\
gRangeSymbol = gensym("range");\
gResultSymbol = gensym("result");\
gValueSymbol = gensym("value");

mextern(PSymbol) gAsteriskSymbol;      /* Pointer to unique Symbol for '*' */
mextern(PSymbol) gCloseRoundSymbol;    /* Pointer to unique Symbol for ')' */
mextern(PSymbol) gCloseSquareSymbol;   /* Pointer to unique Symbol for ']' */
mextern(PSymbol) gCountSymbol;         /* Pointer to unique Symbol for 'count' */
mextern(PSymbol) gDollarSymbol;        /* Pointer to unique Symbol for '$' */
mextern(PSymbol) gDollarXSymbol;       /* Pointer to unique Symbol for '$z' */
mextern(PSymbol) gDollarYSymbol;       /* Pointer to unique Symbol for '$z' */
mextern(PSymbol) gDollarZSymbol;       /* Pointer to unique Symbol for '$z' */
mextern(PSymbol) gDoubleDollarSymbol;  /* Pointer to unique Symbol for '$$' */
mextern(PSymbol) gDoubleDollarXSymbol; /* Pointer to unique Symbol for '$$z' */
mextern(PSymbol) gDoubleDollarYSymbol; /* Pointer to unique Symbol for '$$z' */
mextern(PSymbol) gDoubleDollarZSymbol; /* Pointer to unique Symbol for '$$z' */
mextern(PSymbol) gEmptySymbol;         /* Pointer to unique Symbol for '' */
mextern(PSymbol) gNegInfSymbol1;       /* Pointer to unique Symbol for '-inf' */
mextern(PSymbol) gNegInfSymbol2;       /* Pointer to unique Symbol for '-°' */
mextern(PSymbol) gOffSymbol;           /* Pointer to unique Symbol for 'off' */
mextern(PSymbol) gOnSymbol;            /* Pointer to unique Symbol for 'on' */
mextern(PSymbol) gOpenRoundSymbol;     /* Pointer to unique Symbol for '(' */
mextern(PSymbol) gOpenSquareSymbol;    /* Pointer to unique Symbol for '[' */
mextern(PSymbol) gPosInfSymbol1;       /* Pointer to unique Symbol for 'inf' */
mextern(PSymbol) gPosInfSymbol2;       /* Pointer to unique Symbol for '+inf' */
mextern(PSymbol) gPosInfSymbol3;       /* Pointer to unique Symbol for '°' */
mextern(PSymbol) gPosInfSymbol4;       /* Pointer to unique Symbol for '+°" */
mextern(PSymbol) gRangeSymbol;         /* Pointer to unique Symbol for 'range' */
mextern(PSymbol) gResultSymbol;        /* Pointer to unique Symbol for 'result' */
mextern(PSymbol) gValueSymbol;         /* Pointer to unique Symbol for 'value' */

#endif /* not MAPCOMMON_H_ */
