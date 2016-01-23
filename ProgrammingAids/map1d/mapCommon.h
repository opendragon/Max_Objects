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
# define MAPCOMMON_H_ /* */

# include "missingAndExtra.h"
# include "genericListOutput.h"

# define kCommentCharacter '#'

enum Category
{
    MatchInteger,
    MatchFloat,
    MatchInfinity,
    MatchUnknown
}; // Category

union ForI_
{
    double      fFloat;
    t_atom_long fInteger;
}; // ForI_

struct FloatOrInteger
{
    ForI_ fValue;
    bool  fIsFloat;
}; // FloatOrInteger

struct RangeElement
{
    FloatOrInteger fValue;
    Category       fKind;
    bool           fIsClosed;
}; // RangeElement

inline double
getFOIFloat(const FloatOrInteger & value)
{
    return value.fValue.fFloat;
} // getFOIFloat

inline t_atom_long
getFOILong(const FloatOrInteger & value)
{
    return value.fValue.fInteger;
} // getFOILong

inline double
getFOIValue(const FloatOrInteger & value)
{
    return (value.fIsFloat ? value.fValue.fFloat : TO_DBL(value.fValue.fInteger));
} // getFOIValue

inline void
setFOI2Float(FloatOrInteger & target,
             const double     value)
{
    target.fIsFloat = true;
    target.fValue.fFloat = value;
} // setFOI2Float

inline void
setFOI2Integer(FloatOrInteger &  target,
               const t_atom_long value)
{
    target.fIsFloat = false;
    target.fValue.fInteger = value;
} // setFOI2Integer

# define ADD_HEADER(type) \
    void\
    cmd_Add(type *     xx, \
            t_symbol * message, \
            const long argc, \
            t_atom *   argv)

# define AFTER_HEADER(type) \
    void\
    cmd_After(type *     xx, \
              t_symbol * message, \
              const long argc, \
              t_atom *   argv)

# define BEFORE_HEADER(type) \
    void\
    cmd_Before(type *     xx, \
               t_symbol * message, \
               const long argc, \
               t_atom *   argv)

# define DELETE_HEADER(type) \
    void\
    cmd_Delete(type *     xx, \
               const long num)

# define DUMP_HEADER(type) \
    void\
    cmd_Dump(type * xx)

# define GET_HEADER(type) \
    void\
    cmd_Get(type *     xx, \
            const long num, \
            t_symbol * which)

# define REPLACE_HEADER(type) \
    void\
    cmd_Replace(type *     xx, \
                t_symbol * message, \
                const long argc, \
                t_atom *   argv)

# define SET_HEADER(type) \
    void\
    cmd_Set(type *     xx, \
            t_symbol * message, \
            const long argc, \
            t_atom *   argv)

# define SHOW_HEADER(type) \
    void\
    cmd_Show(type *     xx, \
             const long num)

# define MapRoutineDeclarations(type) \
    ADD_HEADER(type); \
    AFTER_HEADER(type); \
    BEFORE_HEADER(type); \
    CLEAR_HEADER(type); \
    COUNT_HEADER(type); \
    DELETE_HEADER(type); \
    DUMP_HEADER(type); \
    GET_HEADER(type); \
    LOAD_HEADER(type); \
    REPLACE_HEADER(type); \
    SET_HEADER(type); \
    SHOW_HEADER(type); \
    VERBOSE_HEADER(type)

mextern(t_symbol *) gAsteriskSymbol;      /* Pointer to unique symbol for '*' */
mextern(t_symbol *) gCloseRoundSymbol;    /* Pointer to unique symbol for ')' */
mextern(t_symbol *) gCloseSquareSymbol;   /* Pointer to unique symbol for ']' */
mextern(t_symbol *) gCountSymbol;         /* Pointer to unique symbol for 'count' */
mextern(t_symbol *) gDollarSymbol;        /* Pointer to unique symbol for '$' */
mextern(t_symbol *) gDollarXSymbol;       /* Pointer to unique symbol for '$z' */
mextern(t_symbol *) gDollarYSymbol;       /* Pointer to unique symbol for '$z' */
mextern(t_symbol *) gDollarZSymbol;       /* Pointer to unique symbol for '$z' */
mextern(t_symbol *) gDoubleDollarSymbol;  /* Pointer to unique symbol for '$$' */
mextern(t_symbol *) gDoubleDollarXSymbol; /* Pointer to unique symbol for '$$z' */
mextern(t_symbol *) gDoubleDollarYSymbol; /* Pointer to unique symbol for '$$z' */
mextern(t_symbol *) gDoubleDollarZSymbol; /* Pointer to unique symbol for '$$z' */
mextern(t_symbol *) gEmptySymbol;         /* Pointer to unique symbol for '' */
mextern(t_symbol *) gNegInfSymbol1;       /* Pointer to unique symbol for '-inf' */
mextern(t_symbol *) gNegInfSymbol2;       /* Pointer to unique symbol for '-∞' */
mextern(t_symbol *) gOffSymbol;           /* Pointer to unique symbol for 'off' */
mextern(t_symbol *) gOnSymbol;            /* Pointer to unique symbol for 'on' */
mextern(t_symbol *) gOpenRoundSymbol;     /* Pointer to unique symbol for '(' */
mextern(t_symbol *) gOpenSquareSymbol;    /* Pointer to unique symbol for '[' */
mextern(t_symbol *) gPosInfSymbol1;       /* Pointer to unique symbol for 'inf' */
mextern(t_symbol *) gPosInfSymbol2;       /* Pointer to unique symbol for '+inf' */
mextern(t_symbol *) gPosInfSymbol3;       /* Pointer to unique symbol for '∞' */
mextern(t_symbol *) gPosInfSymbol4;       /* Pointer to unique symbol for '+∞" */
mextern(t_symbol *) gRangeSymbol;         /* Pointer to unique symbol for 'range' */
mextern(t_symbol *) gResultSymbol;        /* Pointer to unique symbol for 'result' */
mextern(t_symbol *) gValueSymbol;         /* Pointer to unique symbol for 'value' */

#endif /* not MAPCOMMON_H_ */
