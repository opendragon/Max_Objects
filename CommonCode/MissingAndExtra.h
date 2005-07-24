/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       MissingAndExtra.h                                                       */
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
 #define MISSINGANDEXTRA_H_ /* */
 
 #if defined(__cplusplus)
extern "C"
{
 #endif /* __cplusplus */

 #if (! defined(__TYPES__))
  #include <Types.h>
 #endif /* not __TYPES__ */

 #if (! defined(MAX_INCLUDES))
  #define MAX_INCLUDES /* */
  #include "ext.h"
  #include "ext_user.h"
 #endif /* not MAX_INCLUDES */
 
 #include <Dialogs.h>
 #include <string.h>

 #if defined(__cplusplus)
}
 #endif /* __cplusplus */
  
 #define NULL_HDL 0L
 #define NULL_PTR 0L /* There is a problem with NULL == 0, rather than == 0L */
/* If NULL is passed to a routine that doesn't have a signature, such as via defer(), */
/* the fact that sizeof(NULL) != sizeof(Pvoid) results in argument alignment problems */
/* that are almost impossible to detect. */
 
/* Missing type declarations */
struct atombuf
{
	long 				a_argc;
	struct atom a_argv[1];
}; /* atombuf */

typedef struct atombuf	Atombuf;
typedef struct binbuf		Binbuf;
typedef struct clock  	Clock;
typedef struct outlet 	Outlet;
typedef struct qelem  	Qelem;

/* Note that TRUE and FALSE are (internally) defined as single byte values, */
/* and must be avoided in macros!!!! */

 #if (! defined(ASSIST_INLET))
  #define ASSIST_INLET 1
 #endif /* not ASSIST_INLET */
 #if (! defined(ASSIST_OUTLET))
  #define ASSIST_OUTLET 2
 #endif /* not ASSIST_OUTLET */
 
 #define USE_ASSIST_STRING /* */
 #define OUR_RES_NUMB_BASE 17120
 
 #define OUR_SEGMENT_RESOURCE 'NjSg'
 #define OUR_ESCAPE_CHAR      '\\'

 #define OUR_OK_BUTTON_ID 1
 #define OUR_PICTURE_ID   2
 #define OUR_TEXT_AREA_ID 3
 #define OUR_STR_OFFSET		1

 #if (__powerc)
  #define SYSLOG_OK 1
 #else /* not __powerc */
  #define SYSLOG_OK 0
 #endif /* not __powerc */

 #if (! defined(SYSLOG_LEVEL))
  #define SYSLOG_LEVEL LOG_DEBUG
 #endif /* not SYSLOG_LEVEL */

/* Standardize routine entry and exit */

 #define ExitMaxMessageHandler() \
{\
 	ExitCodeResource();\
	return NULL_PTR;\
\
}

/* Convenience macros */
 #if defined(IS_MAIN)
  #define mextern(xx) xx
  #define minit(vv)   = vv
 #else /* not ISMAIN */
  #define mextern(xx) extern xx
  #define minit(vv)  /* */
 #endif /* not ISMAIN */

/* Convenience type declarations */
 #if (! defined(UCHAR_DEFINED))
typedef unsigned char  uchar;
 #endif /* not UCHAR_DEFINED */
typedef unsigned short 	ushort;
typedef unsigned long  	ulong;
typedef Atom *         	PAtom;
typedef Atombuf *				PAtombuf;
typedef Binbuf *       	PBinbuf;
typedef bool *         	Pbool;
typedef Box *          	PBox;
typedef char *         	Pchar;
typedef Clock *        	PClock;
typedef double *       	Pdouble;
typedef float *        	Pfloat;
typedef fptr *         	Pfptr;
typedef Handle *       	PHandle;
typedef int *          	Pint;
typedef long *         	Plong;
typedef Object *       	PObject;
typedef Outlet *       	POutlet;
typedef Patcher *      	PPatcher;
typedef Qelem *        	PQelem;
typedef short *        	Pshort;
typedef Symbol *       	PSymbol;
typedef uchar *        	Puchar;
typedef void *         	Pvoid;
typedef POutlet *      	HOutlet;
typedef Pvoid *        	Hvoid;
typedef GWorldPtr      	PGWorld;
typedef PixMapHandle   	HPixmap;

typedef const char *		Qchar;

 /* Set up A0/A4 code. Note that this will ONLY work if the called routine is in */
 /* the same segment as the caller, and it CANNOT be mixed with the standard */
 /* versions of these routines. */
 /* This code 'borrowed' from <SetupA4.h> */
 #define EnterCodeResource EnterCallback
 #define ExitCodeResource  ExitCallback
 #if defined(__cplusplus)
extern "C"
{
 #endif /* __cplusplus */
 #if (! __powerc) && (! defined(__CFM68K__))
  #define PrepareCallback() RememberA4() 
  #define EnterCallback()   long oldA4 = SetUpA4()
  #define ExitCallback()    RestoreA4(oldA4)
 #else /* __powerc or __CFM68K__ */
  #define PrepareCallback() /* */
  #define EnterCallback()   /* */
  #define ExitCallback()    /* */
 #endif /* __powerc or __CFM68K__ */
 #if defined(IS_MAIN)
  #if (! __powerc) && (! defined(__CFM68K__))
	void RememberA4
		(void);
		
	long SetUpA4
		(void);
	
	asm long SetUpA4
		(void)
	{
		move.l a4,d0
		lea    __storage,a4
		move.l (a4),a4
		rts
	
	__storage: dc.l	0	/* this storage is only referenced thru data cache */
	
		entry  RememberA4
		lea    __storage,a0
		move.l a4,(a0)
		rts
	} /* SetUpA4 */
	
	long RestoreA4
		(long : __D0) :
			__D0 = 0xC18C;
  #else /* __powerc or __CFM68K__ */
   #define RememberA4()  0
   #define SetUpA4()     0L
   #define RestoreA4(xx) 0L
  #endif /* __powerc or __CFM68K__ */
 #else /* not IS_MAIN */
  #if (! __powerc) && (! defined(__CFM68K__))
	extern void RememberA4
		(void);
	
	extern long SetUpA4
		(void);
		
	long RestoreA4
		(long : __D0) :
			__D0 = 0xC18C;
  #else /* __powerc or __CFM68K__ */
   #define RememberA4()  0
   #define SetUpA4()     0L
   #define RestoreA4(xx) 0L
  #endif /* __powerc or __CFM68K__ */
 #endif /* not IS_MAIN */
 #if defined(__cplusplus)
}
 #endif /* __cplusplus */

 #if defined(__powerc)
  #if defined(__cplusplus)
extern "C"
{
  #endif /* __cplusplus */
  /* Missing function prototypes!!!! */
Pchar wind_helpfolder
	(void);
  /*#define ed_free(xx) ((*(FNS[f_ed_free]))(xx))*/
  #define box_redraw(xx) box_erase(xx)
  
long fopen
	(Qchar	name,
	 Qchar	mode);

int fclose
	(long xx);

int fprintf
	(long		xx,
	 Qchar	format,
	 ...);

int fscanf
	(long		xx,
	 Qchar	format,
	 ...);

int fseek
	(long xx,
	 long offset,
	 int  mode);
  #if defined(__cplusplus)
}
  #endif /* __cplusplus */
  
  /*#define bf_singlefast(h,hsize,argc,argv) ((*(FNS[f_bf_singlefast]))(h,hsize,argc,argv))*/
 #else /* not __powerc */
  /* Incorrect function definitions!!!! */
  #undef fclose
  #define fclose(xx)      ((*(FNS[f_fclose])) (xx))
  #undef fprintf
  #define fprintf         (*(FNS[f_fprintf]))
  #undef fscanf
  #define fscanf          (*(FNS[f_fscanf]))
  #undef fseek
  #define fseek(xx,yy,zz) ((*(FNS[f_fseek])) (xx, yy, zz))
 #endif /* not __powerc */

 #define FREEBYTES(ptr, numb)   \
{\
  if (ptr && numb)\
  {\
    freebytes(ptr, short((numb) * sizeof(*ptr)));\
    ptr = NULL_PTR;\
  }\
}
 #define GETBYTES(numb, kind)   \
  reinterpret_cast<kind*>(getbytes(short((numb) * sizeof(kind))))
    
 #define StandardRoutineDeclarations(type)   \
Pvoid cmd_Anything\
  (type     xx,\
   PSymbol  message,\
   short    argc,\
   PAtom    argv);\
\
Pvoid cmd_Assist\
  (type   xx,\
   Pvoid  bb,\
   long   msg,\
   long   arg,\
   Ptr    dstString);\
\
Pvoid cmd_Bang\
  (type xx);\
\
Pvoid cmd_Float\
  (type   xx,\
   double msg);\
\
Pvoid cmd_Info\
  (type     xx,\
   PPatcher pp,\
   PBox     bb);\
\
Pvoid cmd_Int\
  (type xx,\
   long num);\
\
Pvoid cmd_List\
  (type     xx,\
   PSymbol  message,\
   short    argc,\
   PAtom    argv);
 #define StandardInfoRoutine(type) \
Pvoid cmd_Info\
  (type     xx,\
   PPatcher pp,\
   PBox     bb)\
{\
  EnterCallback();\
  xx;\
  pp;\
  bb;\
  displayAboutBox(OUR_NAME, OUR_RES_NUMB, gVersionNumber);\
  ExitMaxMessageHandler()\
}
 #define StandardAnythingRoutine(type) \
Pvoid cmd_Anything\
  (type    xx,\
   PSymbol message,\
   short   argc,\
   PAtom   argv)\
{\
  EnterCallback();\
  xx;\
  reportAnything(OUTPUT_PREFIX, message, argc, argv);\
  ExitMaxMessageHandler()\
}

 /* really weird C code to count the number of bits in a word */
 #define BITCOUNT(xx)	(((BX_(xx) + (BX_(xx) >> 4)) & 0x0F0F0F0F) % 255)
 #define BX_(xx) 		  ((xx) - (((xx) >> 1) & 0x77777777)    \
                             - (((xx) >> 2) & 0x33333333)    \
                             - (((xx) >> 3) & 0x11111111))

/* Syslog stuff: */
 #if SYSLOG_OK
  #if defined(COMPILE_FOR_CATS)
   #include <sys/syslog.h>
  #else /* not COMPILE_FOR_CATS */
   #include "syslog.h"
  #endif /* not COMPILE_FOR_CATS */
 #endif /* SYSLOG_OK */
 
 #if SYSLOG_OK && defined(USE_SYSLOG)
  #define SYSLOG_AVAILABLE 1
 #else /* not SYSLOG_OK or not USE_SYSLOG */
  #define SYSLOG_AVAILABLE 0
 #endif /* not SYSLOG_OK or not USE_SYSLOG */

 #if defined(COMPILE_FOR_CATS)
  #define Syslog	\
   	if (gSyslogFunction)\
   		gSyslogFunction /* to connect to the "real thing" */
 #endif /* COMPILE_FOR_CATS */

 #if SYSLOG_AVAILABLE
  #define LOG_ERROR_1(xx) \
    {\
      Syslog(LOG_ERR, xx);\
      error(xx);\
    }
  #define LOG_ERROR_2(xx, aa) \
    {\
      Syslog(LOG_ERR, xx, aa);\
      error(xx, aa);\
    }
  #define LOG_ERROR_3(xx, aa, bb) \
    {\
      Syslog(LOG_ERR, xx, aa, bb);\
      error(xx, aa, bb);\
    }
  #define LOG_ERROR_4(xx, aa, bb, cc) \
    {\
      Syslog(LOG_ERR, xx, aa, bb, cc);\
      error(xx, aa, bb, cc);\
    }
  #define LOG_ERROR_5(xx, aa, bb, cc, dd) \
    {\
      Syslog(LOG_ERR, xx, aa, bb, cc, dd);\
      error(xx, aa, bb, cc, dd);\
    }
  #define LOG_ERROR_6(xx, aa, bb, cc, dd, ee) \
    {\
      Syslog(LOG_ERR, xx, aa, bb, cc, dd, ee);\
      error(xx, aa, bb, cc, dd, ee);\
    }
  #define LOG_POST_1(xx) \
    {\
      post(xx);\
      Syslog(SYSLOG_LEVEL, xx);\
    }
  #define LOG_POST_2(xx, aa) \
    {\
      post(xx, aa);\
      Syslog(SYSLOG_LEVEL, xx, aa);\
    }
  #define LOG_POST_3(xx, aa, bb) \
    {\
      post(xx, aa, bb);\
      Syslog(SYSLOG_LEVEL, xx, aa, bb);\
    }
  #define LOG_POST_4(xx, aa, bb, cc) \
    {\
      post(xx, aa, bb, cc);\
      Syslog(SYSLOG_LEVEL, xx, aa, bb, cc);\
    }
  #define LOG_POST_5(xx, aa, bb, cc, dd) \
    {\
      post(xx, aa, bb, cc, dd);\
      Syslog(SYSLOG_LEVEL, xx, aa, bb, cc, dd);\
    }
  #define LOG_POST_6(xx, aa, bb, cc, dd, ee) \
    {\
      post(xx, aa, bb, cc, dd, ee);\
      Syslog(SYSLOG_LEVEL, xx, aa, bb, cc, dd, ee);\
    }
 #else /* not SYSLOG_AVAILABLE */
  #define LOG_ERROR_1(xx) \
    error(xx);
  #define LOG_ERROR_2(xx, aa) \
    error(xx, aa);
  #define LOG_ERROR_3(xx, aa, bb) \
    error(xx, aa, bb);
  #define LOG_ERROR_4(xx, aa, bb, cc) \
    error(xx, aa, bb, cc);
  #define LOG_ERROR_5(xx, aa, bb, cc, dd) \
    error(xx, aa, bb, cc, dd);
  #define LOG_ERROR_6(xx, aa, bb, cc, dd, ee) \
    error(xx, aa, bb, cc, dd, ee);
  #define LOG_POST_1(xx) \
    post(xx);
  #define LOG_POST_2(xx, aa) \
    post(xx, aa);
  #define LOG_POST_3(xx, aa, bb) \
    post(xx, aa, bb);
  #define LOG_POST_4(xx, aa, bb, cc) \
  	post(xx, aa, bb, cc);
  #define LOG_POST_5(xx, aa, bb, cc, dd) \
   	post(xx, aa, bb, cc, dd);
  #define LOG_POST_6(xx, aa, bb, cc, dd, ee) \
   	post(xx, aa, bb, cc, dd, ee);
 #endif /* not SYSLOG_AVAILABLE */

 #if (defined(TRACE_MAX_MESSAGES) && defined(BE_VERBOSE))
  #if SYSLOG_AVAILABLE
   #define REPORT_MAX_MESSAGE(msg) \
{\
  Syslog(SYSLOG_LEVEL, OUTPUT_PREFIX "Max/" msg);\
  if (xx->fVerbose)\
    post(OUTPUT_PREFIX "Max/" msg);\
}
  #else /* not SYSLOG_AVAILABLE */
   #define REPORT_MAX_MESSAGE(msg) \
{\
  if (xx->fVerbose)\
    post(OUTPUT_PREFIX "Max/" msg);\
}
  #endif /* not SYSLOG_AVAILABLE */
 #else /* not TRACE_MAX_MESSAGES or not BE_VERBOSE */
  #define REPORT_MAX_MESSAGE(msg) /* */
 #endif /* not TRACE_MAX_MESSAGES or not BE_VERBOSE */

 #if SYSLOG_AVAILABLE
  #define LOG_ENTER() acquireSyslog();
  #define LOG_EXIT()  relinquishSyslog();
 #else /* not SYSLOG_AVAILABLE */
  #define LOG_ENTER() /* */
  #define LOG_EXIT()  /* */
 #endif /* not SYSLOG_AVAILABLE */

 #if defined(ALLOW_TRACE)
  #define TRACE_POST_1(xx)	\
	LOG_POST_1(xx)
  #define TRACE_POST_2(xx, aa)	\
	LOG_POST_2(xx, aa)
  #define TRACE_POST_3(xx, aa, bb)	\
	LOG_POST_3(xx, aa, bb)
  #define TRACE_POST_4(xx, aa, bb, cc)	\
	LOG_POST_4(xx, aa, bb, cc)
  #define TRACE_POST_5(xx, aa, bb, cc, dd)	\
	LOG_POST_5(xx, aa, bb, cc, dd)
  #define TRACE_POST_6(xx, aa, bb, cc, dd, ee)	\
	LOG_POST_6(xx, aa, bb, cc, dd, ee)
 #else /* not ALLOW_TRACE */
  #define TRACE_POST_1(xx)	/* */
  #define TRACE_POST_2(xx, aa)	/* */
  #define TRACE_POST_3(xx, aa, bb)	/* */
  #define TRACE_POST_4(xx, aa, bb, cc)	/* */
  #define TRACE_POST_5(xx, aa, bb, cc, dd)	/* */
  #define TRACE_POST_6(xx, aa, bb, cc, dd, ee)	/* */
 #endif /* not ALLOW_TRACE */

void acquireSyslog
  (void);

void relinquishSyslog
  (void);

mextern(Pfptr)	FNS;            /* Max function macros table */
mextern(Pvoid)	gClass;         /* Pointer to class object */
mextern(ulong)	gVersionNumber; /* The version number obtained from the 'vers' resource */

 #if defined(COMPILE_FOR_CATS)
typedef void (* closelog_FP)
	(void);
typedef void (* openlog_FP)
	(Qchar	ident,
	 int		logopt,
	 int		facility);
typedef OSErr (* syslog_FP)
	(int		priority,
	 Qchar	format,
	 ...);
extern syslog_FP	gSyslogFunction; // Note that the variable is NOT present in main()
 #endif /* COMPILE_FOR_CATS */
extern long				gSyslogCount; // Note that the variable is NOT present in main()

#endif /* not MISSINGANDEXTRA_H_ */
