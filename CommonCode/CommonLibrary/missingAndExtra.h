/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       missingAndExtra.h                                                       */
/*                                                                                      */
/*  Contains:   The data type declarations that are needed for Max development.         */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 1998 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
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
/*  Created:    1998/07/13                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(MISSINGANDEXTRA_H_))
# define MISSINGANDEXTRA_H_ /* */

# include "ext.h"      // standard Max include, always required
# include "ext_obex.h" // required for new style Max object
# include "ext_strings.h"

# define MAX_ASSISTANCE 512 // The maximum length of an 'assistance' string
# define NUM_BUFF_SIZE  32 // The size of a buffer used to format a number

/* Note that TRUE and FALSE are (internally) defined as single byte values, */
/* and must be avoided in macros!!!! */

# if (! defined(ASSIST_INLET))
#  define ASSIST_INLET 1
# endif /* not ASSIST_INLET */
# if (! defined(ASSIST_OUTLET))
#  define ASSIST_OUTLET 2
# endif /* not ASSIST_OUTLET */

/* Convenience macros */
# if defined(IS_MAIN)
#  define mextern(xx) xx
#  define minit(vv)   = vv
# else /* not ISMAIN */
#  define mextern(xx) extern xx
#  define minit(vv)  /* */
# endif /* not ISMAIN */

# undef USE_EVNUM
# if (0x610 <= C74_MAX_SDK_VERSION)
#  define USE_EVNUM FALSE
# else /* 0x610 > C74_MAX_SDK_VERSION */
#  define USE_EVNUM TRUE
# endif /* 0x610 > C74_MAX_SDK_VERSION */

typedef t_object t_outlet; // Missing declaration
typedef void     t_binbuf; // Missing declaration

// Message macros to prevent spelling errors:
# define MESSAGE_ANYTHING   "anything"
# define MESSAGE_ASSIST     "assist"
# define MESSAGE_BANG       "bang"
# define MESSAGE_FLOAT      "float"
# define MESSAGE_IN1        "in1"
# define MESSAGE_IN2        "in2"
# define MESSAGE_INT        "int"
# define MESSAGE_LIST       "list"

# define FILETYPE_TEXT	static_cast<t_fourcc>('TEXT')

# define FREE_BYTES(ptr) \
    { \
        if (ptr) \
        { \
            sysmem_freeptr(ptr); \
            ptr = NULL; \
        } \
    }

# define GET_BYTES(numb, kind) \
    reinterpret_cast<kind *>(sysmem_newptr(static_cast<long>((numb) * sizeof(kind))))

# define MAKE_CLOCK(xx, ff) \
    static_cast<t_clock *>(clock_new(xx, reinterpret_cast<method>(ff)))

# define MAKE_QELEM(xx, ff) \
    static_cast<t_qelem *>(qelem_new(xx, reinterpret_cast<method>(ff)))

# define MAKE_TYPED_HANDLE(type, count) \
    reinterpret_cast<type **>(sysmem_newhandle(static_cast<long>(sizeof(type) * (count))))

# define TO_DBL(value) \
    static_cast<t_atom_float>(value)

# define TO_INT(value) \
    static_cast<t_atom_long>(value)

# define ANYTHING_HEADER(type) \
    void cmd_Anything(type *      xx, \
                      t_symbol *  message, \
                      const short argc, \
                      t_atom *    argv)

# define ASSIST_HEADER(type) \
    void cmd_Assist(type *     xx, \
                    void *     bb, \
                    const long msg, \
                    const long arg, \
                    char *     dstString)

# define BANG_HEADER(type) \
    void cmd_Bang(type * xx)

# define CLEAR_HEADER(type) \
    void cmd_Clear(type * xx)

# define COUNT_HEADER(type) \
    void cmd_Count(type * xx)

# define FLOAT_HEADER(type) \
    void cmd_Float(type *       xx, \
                   const double msg)

# define IN1_HEADER(type) \
    void cmd_In1(type *     xx, \
                 const long msg)

# define IN2_HEADER(type) \
    void cmd_In2(type *     xx, \
                 const long msg)

# define INT_HEADER(type) \
    void cmd_Int(type *     xx, \
                 const long msg)

# define LIST_HEADER(type) \
    void cmd_List(type *      xx, \
                  t_symbol *  message, \
                  const short argc, \
                  t_atom *    argv)

# define LOAD_HEADER(type) \
    void cmd_Load(type *     xx, \
                  t_symbol * fileName)

# define VERBOSE_HEADER(type) \
    void cmd_Verbose(type *     xx, \
                     t_symbol * onOff)

# define StandardRoutineDeclarations(type) \
\
    ANYTHING_HEADER(type); \
    ASSIST_HEADER(type); \
    BANG_HEADER(type); \
    FLOAT_HEADER(type); \
    IN1_HEADER(type); \
    IN2_HEADER(type); \
    INT_HEADER(type); \
    LIST_HEADER(type)

#define StandardAnythingRoutine(type) \
    ANYTHING_HEADER(type) \
    { \
        reportAnything(reinterpret_cast<t_object *>(xx), OUTPUT_PREFIX, message, argc, argv); \
    }

/* really weird C code to count the number of bits in a word */
# define BITCOUNT(xx)   (((BX_(xx) + (BX_(xx) >> 4)) & 0x0F0F0F0F) % 255)
# define BX_(xx)        ((xx) - (((xx) >> 1) & 0x77777777) - (((xx) >> 2) & 0x33333333) - \
                        (((xx) >> 3) & 0x11111111))

# define LOG_ERROR_1(xx, ff) \
    cpost(ff); \
    object_error(reinterpret_cast<t_object *>(xx), ff);
# define LOG_ERROR_2(xx, ff, aa) \
    cpost(ff, aa); \
    object_error(reinterpret_cast<t_object *>(xx), ff, aa);
# define LOG_ERROR_3(xx, ff, aa, bb) \
    cpost(ff, aa, bb); \
    object_error(reinterpret_cast<t_object *>(xx), ff, aa, bb);
# define LOG_ERROR_4(xx, ff, aa, bb, cc) \
    cpost(ff, aa, bb, cc); \
    object_error(reinterpret_cast<t_object *>(xx), ff, aa, bb, cc);
# define LOG_ERROR_5(xx, ff, aa, bb, cc, dd) \
    cpost(ff, aa, bb, cc, dd); \
    object_error(reinterpret_cast<t_object *>(xx), ff, aa, bb, cc, dd);
# define LOG_ERROR_6(xx, ff, aa, bb, cc, dd, ee) \
    cpost(ff, aa, bb, cc, dd, ee); \
    object_error(reinterpret_cast<t_object *>(xx), ff, aa, bb, cc, dd, ee);
# define LOG_POST_1(xx, ff) \
    object_post(reinterpret_cast<t_object *>(xx), ff);
# define LOG_POST_2(xx, ff, aa) \
    object_post(reinterpret_cast<t_object *>(xx), ff, aa);
# define LOG_POST_3(xx, ff, aa, bb) \
    object_post(reinterpret_cast<t_object *>(xx), ff, aa, bb);
# define LOG_POST_4(xx, ff, aa, bb, cc) \
    object_post(reinterpret_cast<t_object *>(xx), ff, aa, bb, cc);
# define LOG_POST_5(xx, ff, aa, bb, cc, dd) \
    object_post(reinterpret_cast<t_object *>(xx), ff, aa, bb, cc, dd);
# define LOG_POST_6(xx, ff, aa, bb, cc, dd, ee) \
    object_post(reinterpret_cast<t_object *>(xx), ff, aa, bb, cc, dd, ee);

# if (defined(TRACE_MAX_MESSAGES) && defined(BE_VERBOSE))
#  define REPORT_MAX_MESSAGE(msg) \
    { \
        if (xx->fVerbose) \
        { \
            object_post(reinterpret_cast<t_object *>(xx), OUTPUT_PREFIX "Max/" msg); \
        } \
    }
# else /* not TRACE_MAX_MESSAGES or not BE_VERBOSE */
#  define REPORT_MAX_MESSAGE(msg) /* */
# endif /* not TRACE_MAX_MESSAGES or not BE_VERBOSE */

# if defined(ALLOW_TRACE)
#  define TRACE_POST_1(xx, ff)  \
    LOG_POST_1(xx, ff)
#  define TRACE_POST_2(xx, ff, aa)  \
    LOG_POST_2(xx, ff, aa)
#  define TRACE_POST_3(xx, ff, aa, bb)  \
    LOG_POST_3(xx, ff, aa, bb)
#  define TRACE_POST_4(xx, ff, aa, bb, cc)  \
    LOG_POST_4(xx, ff, aa, bb, cc)
#  define TRACE_POST_5(xx, ff, aa, bb, cc, dd)  \
    LOG_POST_5(xx, ff, aa, bb, cc, dd)
#  define TRACE_POST_6(xx, ff, aa, bb, cc, dd, ee)  \
    LOG_POST_6(xx, ff, aa, bb, cc, dd, ee)
# else /* not ALLOW_TRACE */
#  define TRACE_POST_1(xx, ff)  /* */
#  define TRACE_POST_2(xx, ff, aa)  /* */
#  define TRACE_POST_3(xx, ff, aa, bb)  /* */
#  define TRACE_POST_4(xx, ff, aa, bb, cc)  /* */
#  define TRACE_POST_5(xx, ff, aa, bb, cc, dd)  /* */
#  define TRACE_POST_6(xx, ff, aa, bb, cc, dd, ee)  /* */
# endif /* not ALLOW_TRACE */

# if defined(C74_X64)
#  define LONG_FORMAT "%lld"
# else /* ! defined(C74_X64) */
#  define LONG_FORMAT "%ld"
# endif /* ! defined(C74_X64) */

mextern(t_class *) gClass; /* Pointer to class object */

#endif /* not MISSINGANDEXTRA_H_ */
