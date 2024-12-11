/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 29 "parser_gram.y"


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <locale.h>
#include <sys/utsname.h>
#include <sys/statvfs.h>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#ifdef HAVE_LIBTECLA
#include <libtecla.h>
#endif
#include "parsertypes.h"
#include "filebench.h"
#include "utils.h"
#include "stats.h"
#include "vars.h"
#include "eventgen.h"
#ifdef HAVE_LIBTECLA
#include "auto_comp.h"
#endif
#include "multi_client_sync.h"

/* yacc and lex externals */
extern FILE *yyin;
extern int yydebug;
extern void yyerror(char *s);
extern int yylex(void);

/* GetLine resource object */
#ifdef HAVE_LIBTECLA
static GetLine *gl;
#endif

/* executable name to execute worker processes later */
char *execname;

static int dofile = DOFILE_FALSE;
static FILE *parentscript;

static char *fbbasepath = FILEBENCHDIR;
static char cwd[MAXPATHLEN];
static pidlist_t *pidlist;
static int filecreate_done;

/* utilities */
static cmd_t *alloc_cmd(void);
static attr_t *alloc_attr(void);
static attr_t *alloc_lvar_attr(var_t *var);
static attr_t *get_attr(cmd_t *cmd, int64_t name);
static attr_t *get_attr_fileset(cmd_t *cmd, int64_t name);
static attr_t *get_attr_integer(cmd_t *cmd, int64_t name);
static attr_t *get_attr_bool(cmd_t *cmd, int64_t name);
static void get_attr_lvars(cmd_t *cmd, flowop_t *flowop);
static list_t *alloc_list();
static probtabent_t *alloc_probtabent(void);
static void add_lvar_to_list(var_t *newlvar, var_t **lvar_list);

/* Info Commands */
static void parser_list(cmd_t *);
static void parser_flowop_list(cmd_t *);

/* Define Commands */
static void parser_proc_define(cmd_t *);
static void parser_thread_define(cmd_t *, procflow_t *, int instances);
static void parser_flowop_define(cmd_t *, threadflow_t *, flowop_t **, int);
static void parser_file_define(cmd_t *);
static void parser_fileset_define(cmd_t *);
static void parser_posset_define(cmd_t *);
static void parser_randvar_define(cmd_t *);
static void parser_randvar_set(cmd_t *);
static void parser_composite_flowop_define(cmd_t *);

/* Create Commands */
static void parser_proc_create(cmd_t *);
static void parser_fileset_create(cmd_t *);

/* set commands */
static void parser_set_integer(cmd_t *cmd);
static void parser_set_var(cmd_t *cmd);
static void parser_set_var_op_int(cmd_t *cmd);
static void parser_set_int_op_var(cmd_t *cmd);
static void parser_set_var_op_var(cmd_t *cmd);

/* Shutdown Commands */
static void parser_proc_shutdown(cmd_t *);
static void parser_filebench_shutdown(cmd_t *cmd);
static void parser_fileset_shutdown(cmd_t *cmd);

/* Other Commands */
static void parser_echo(cmd_t *cmd);
static void parser_foreach_integer(cmd_t *cmd);
static void parser_foreach_string(cmd_t *cmd);
static void parser_fscheck(cmd_t *cmd);
static void parser_fsflush(cmd_t *cmd);
static void parser_log(cmd_t *cmd);
static void parser_statscmd(cmd_t *cmd);
static void parser_statsdump(cmd_t *cmd);
static void parser_statsxmldump(cmd_t *cmd);
static void parser_statsmultidump(cmd_t *cmd);
static void parser_usage(cmd_t *cmd);
static void parser_vars(cmd_t *cmd);
static void parser_printvars(cmd_t *cmd);
static void parser_system(cmd_t *cmd);
static void parser_statssnap(cmd_t *cmd);
static void parser_directory(cmd_t *cmd);
static void parser_eventgen(cmd_t *cmd);
static void parser_enable_mc(cmd_t *cmd);
static void parser_domultisync(cmd_t *cmd);
static void parser_run(cmd_t *cmd);
static void parser_run_variable(cmd_t *cmd);
static void parser_sleep(cmd_t *cmd);
static void parser_sleep_variable(cmd_t *cmd);
static void parser_warmup(cmd_t *cmd);
static void parser_warmup_variable(cmd_t *cmd);
static void parser_help(cmd_t *cmd);
static void arg_parse(const char *command);
static void parser_abort(int arg);
static void parser_version(cmd_t *cmd);
static void parser_osprof_enable(cmd_t *cmd);
static void parser_osprof_disable(cmd_t *cmd);


#line 202 "parser_gram.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_PARSER_GRAM_H_INCLUDED
# define YY_YY_PARSER_GRAM_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    FSC_LIST = 258,
    FSC_DEFINE = 259,
    FSC_EXEC = 260,
    FSC_QUIT = 261,
    FSC_DEBUG = 262,
    FSC_CREATE = 263,
    FSC_SLEEP = 264,
    FSC_STATS = 265,
    FSC_FOREACH = 266,
    FSC_SET = 267,
    FSC_SHUTDOWN = 268,
    FSC_LOG = 269,
    FSC_SYSTEM = 270,
    FSC_FLOWOP = 271,
    FSC_EVENTGEN = 272,
    FSC_ECHO = 273,
    FSC_LOAD = 274,
    FSC_RUN = 275,
    FSC_WARMUP = 276,
    FSC_NOUSESTATS = 277,
    FSC_FSCHECK = 278,
    FSC_FSFLUSH = 279,
    FSC_USAGE = 280,
    FSC_HELP = 281,
    FSC_VARS = 282,
    FSC_VERSION = 283,
    FSC_ENABLE = 284,
    FSC_DOMULTISYNC = 285,
    FSV_STRING = 286,
    FSV_VAL_INT = 287,
    FSV_VAL_BOOLEAN = 288,
    FSV_VARIABLE = 289,
    FSV_WHITESTRING = 290,
    FSV_RANDUNI = 291,
    FSV_RANDTAB = 292,
    FSV_RANDVAR = 293,
    FSV_URAND = 294,
    FSV_RAND48 = 295,
    FST_INT = 296,
    FST_BOOLEAN = 297,
    FSE_FILE = 298,
    FSE_PROC = 299,
    FSE_THREAD = 300,
    FSE_CLEAR = 301,
    FSE_ALL = 302,
    FSE_SNAP = 303,
    FSE_DUMP = 304,
    FSE_DIRECTORY = 305,
    FSE_COMMAND = 306,
    FSE_FILESET = 307,
    FSE_POSSET = 308,
    FSE_XMLDUMP = 309,
    FSE_RAND = 310,
    FSE_MODE = 311,
    FSE_MULTI = 312,
    FSE_MULTIDUMP = 313,
    FSK_SEPLST = 314,
    FSK_OPENLST = 315,
    FSK_CLOSELST = 316,
    FSK_ASSIGN = 317,
    FSK_IN = 318,
    FSK_QUOTE = 319,
    FSK_DIRSEPLST = 320,
    FSK_PLUS = 321,
    FSK_MINUS = 322,
    FSK_MULTIPLY = 323,
    FSK_DIVIDE = 324,
    FSA_SIZE = 325,
    FSA_PREALLOC = 326,
    FSA_PARALLOC = 327,
    FSA_PATH = 328,
    FSA_REUSE = 329,
    FSA_PROCESS = 330,
    FSA_MEMSIZE = 331,
    FSA_RATE = 332,
    FSA_CACHED = 333,
    FSA_READONLY = 334,
    FSA_TRUSTTREE = 335,
    FSA_IOSIZE = 336,
    FSA_FILE = 337,
    FSA_POSSET = 338,
    FSA_WSS = 339,
    FSA_NAME = 340,
    FSA_RANDOM = 341,
    FSA_INSTANCES = 342,
    FSA_DSYNC = 343,
    FSA_TARGET = 344,
    FSA_ITERS = 345,
    FSA_NICE = 346,
    FSA_VALUE = 347,
    FSA_BLOCKING = 348,
    FSA_HIGHWATER = 349,
    FSA_DIRECTIO = 350,
    FSA_DIRWIDTH = 351,
    FSA_FD = 352,
    FSA_SRCFD = 353,
    FSA_ROTATEFD = 354,
    FSA_NAMELENGTH = 355,
    FSA_FILESIZE = 356,
    FSA_ENTRIES = 357,
    FSA_FILESIZEGAMMA = 358,
    FSA_DIRDEPTHRV = 359,
    FSA_DIRGAMMA = 360,
    FSA_USEISM = 361,
    FSA_TYPE = 362,
    FSA_RANDTABLE = 363,
    FSA_RANDSRC = 364,
    FSA_RANDROUND = 365,
    FSA_LEAFDIRS = 366,
    FSA_INDEXED = 367,
    FSA_FSTYPE = 368,
    FSA_RANDSEED = 369,
    FSA_RANDGAMMA = 370,
    FSA_RANDMEAN = 371,
    FSA_RANDMIN = 372,
    FSA_RANDMAX = 373,
    FSA_MASTER = 374,
    FSA_CLIENT = 375,
    FSS_TYPE = 376,
    FSS_SEED = 377,
    FSS_GAMMA = 378,
    FSS_MEAN = 379,
    FSS_MIN = 380,
    FSS_SRC = 381,
    FSS_ROUND = 382,
    FSV_SET_LOCAL_VAR = 383,
    FSA_LVAR_ASSIGN = 384,
    FSA_ALLDONE = 385,
    FSA_FIRSTDONE = 386,
    FSA_TIMEOUT = 387,
    FSC_OSPROF_ENABLE = 388,
    FSC_OSPROF_DISABLE = 389,
    FSA_NOREADAHEAD = 390,
    FSA_IOPRIO = 391,
    FSA_WRITEONLY = 392
  };
#endif
/* Tokens.  */
#define FSC_LIST 258
#define FSC_DEFINE 259
#define FSC_EXEC 260
#define FSC_QUIT 261
#define FSC_DEBUG 262
#define FSC_CREATE 263
#define FSC_SLEEP 264
#define FSC_STATS 265
#define FSC_FOREACH 266
#define FSC_SET 267
#define FSC_SHUTDOWN 268
#define FSC_LOG 269
#define FSC_SYSTEM 270
#define FSC_FLOWOP 271
#define FSC_EVENTGEN 272
#define FSC_ECHO 273
#define FSC_LOAD 274
#define FSC_RUN 275
#define FSC_WARMUP 276
#define FSC_NOUSESTATS 277
#define FSC_FSCHECK 278
#define FSC_FSFLUSH 279
#define FSC_USAGE 280
#define FSC_HELP 281
#define FSC_VARS 282
#define FSC_VERSION 283
#define FSC_ENABLE 284
#define FSC_DOMULTISYNC 285
#define FSV_STRING 286
#define FSV_VAL_INT 287
#define FSV_VAL_BOOLEAN 288
#define FSV_VARIABLE 289
#define FSV_WHITESTRING 290
#define FSV_RANDUNI 291
#define FSV_RANDTAB 292
#define FSV_RANDVAR 293
#define FSV_URAND 294
#define FSV_RAND48 295
#define FST_INT 296
#define FST_BOOLEAN 297
#define FSE_FILE 298
#define FSE_PROC 299
#define FSE_THREAD 300
#define FSE_CLEAR 301
#define FSE_ALL 302
#define FSE_SNAP 303
#define FSE_DUMP 304
#define FSE_DIRECTORY 305
#define FSE_COMMAND 306
#define FSE_FILESET 307
#define FSE_POSSET 308
#define FSE_XMLDUMP 309
#define FSE_RAND 310
#define FSE_MODE 311
#define FSE_MULTI 312
#define FSE_MULTIDUMP 313
#define FSK_SEPLST 314
#define FSK_OPENLST 315
#define FSK_CLOSELST 316
#define FSK_ASSIGN 317
#define FSK_IN 318
#define FSK_QUOTE 319
#define FSK_DIRSEPLST 320
#define FSK_PLUS 321
#define FSK_MINUS 322
#define FSK_MULTIPLY 323
#define FSK_DIVIDE 324
#define FSA_SIZE 325
#define FSA_PREALLOC 326
#define FSA_PARALLOC 327
#define FSA_PATH 328
#define FSA_REUSE 329
#define FSA_PROCESS 330
#define FSA_MEMSIZE 331
#define FSA_RATE 332
#define FSA_CACHED 333
#define FSA_READONLY 334
#define FSA_TRUSTTREE 335
#define FSA_IOSIZE 336
#define FSA_FILE 337
#define FSA_POSSET 338
#define FSA_WSS 339
#define FSA_NAME 340
#define FSA_RANDOM 341
#define FSA_INSTANCES 342
#define FSA_DSYNC 343
#define FSA_TARGET 344
#define FSA_ITERS 345
#define FSA_NICE 346
#define FSA_VALUE 347
#define FSA_BLOCKING 348
#define FSA_HIGHWATER 349
#define FSA_DIRECTIO 350
#define FSA_DIRWIDTH 351
#define FSA_FD 352
#define FSA_SRCFD 353
#define FSA_ROTATEFD 354
#define FSA_NAMELENGTH 355
#define FSA_FILESIZE 356
#define FSA_ENTRIES 357
#define FSA_FILESIZEGAMMA 358
#define FSA_DIRDEPTHRV 359
#define FSA_DIRGAMMA 360
#define FSA_USEISM 361
#define FSA_TYPE 362
#define FSA_RANDTABLE 363
#define FSA_RANDSRC 364
#define FSA_RANDROUND 365
#define FSA_LEAFDIRS 366
#define FSA_INDEXED 367
#define FSA_FSTYPE 368
#define FSA_RANDSEED 369
#define FSA_RANDGAMMA 370
#define FSA_RANDMEAN 371
#define FSA_RANDMIN 372
#define FSA_RANDMAX 373
#define FSA_MASTER 374
#define FSA_CLIENT 375
#define FSS_TYPE 376
#define FSS_SEED 377
#define FSS_GAMMA 378
#define FSS_MEAN 379
#define FSS_MIN 380
#define FSS_SRC 381
#define FSS_ROUND 382
#define FSV_SET_LOCAL_VAR 383
#define FSA_LVAR_ASSIGN 384
#define FSA_ALLDONE 385
#define FSA_FIRSTDONE 386
#define FSA_TIMEOUT 387
#define FSC_OSPROF_ENABLE 388
#define FSC_OSPROF_DISABLE 389
#define FSA_NOREADAHEAD 390
#define FSA_IOPRIO 391
#define FSA_WRITEONLY 392

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 161 "parser_gram.y"

	int64_t		 ival;
	unsigned char	 bval;
	char *		 sval;
	fs_u		 val;
	avd_t		 avd;
	cmd_t		*cmd;
	attr_t		*attr;
	list_t		*list;
	probtabent_t	*rndtb;

#line 540 "parser_gram.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_GRAM_H_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   413

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  138
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  96
/* YYNRULES -- Number of rules.  */
#define YYNRULES  294
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  392

#define YYUNDEFTOK  2
#define YYMAXUTOK   392


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   256,   256,   263,   268,   270,   275,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   326,   332,
     355,   380,   387,   405,   412,   430,   436,   441,   450,   459,
     466,   473,   480,   489,   497,   504,   509,   542,   549,   557,
     560,   578,   595,   613,   632,   639,   647,   665,   682,   700,
     718,   735,   753,   756,   761,   767,   772,   780,   785,   794,
     802,   813,   814,   816,   826,   838,   862,   873,   884,   894,
     904,   910,   916,   922,   929,   938,   947,   958,   966,   973,
     981,   989,   996,  1003,  1012,  1019,  1022,  1040,  1052,  1055,
    1073,  1082,  1087,  1092,  1098,  1103,  1108,  1113,  1121,  1129,
    1134,  1153,  1172,  1179,  1187,  1194,  1202,  1209,  1216,  1224,
    1231,  1237,  1242,  1270,  1271,  1272,  1273,  1279,  1282,  1286,
    1300,  1304,  1318,  1323,  1330,  1335,  1343,  1347,  1360,  1379,
    1384,  1390,  1395,  1401,  1411,  1415,  1430,  1434,  1448,  1453,
    1461,  1465,  1478,  1492,  1497,  1505,  1509,  1523,  1528,  1536,
    1540,  1554,  1560,  1566,  1575,  1576,  1577,  1578,  1580,  1581,
    1583,  1585,  1586,  1588,  1590,  1593,  1594,  1595,  1598,  1599,
    1600,  1601,  1602,  1603,  1604,  1605,  1606,  1609,  1610,  1611,
    1612,  1613,  1614,  1615,  1616,  1617,  1618,  1619,  1620,  1621,
    1622,  1623,  1624,  1627,  1628,  1629,  1630,  1631,  1634,  1635,
    1636,  1637,  1638,  1639,  1642,  1643,  1644,  1645,  1646,  1647,
    1648,  1651,  1652,  1653,  1654,  1655,  1657,  1665,  1666,  1667,
    1669,  1677,  1678,  1681,  1682,  1683,  1684,  1685,  1686,  1689,
    1690,  1691,  1692,  1693,  1694,  1695,  1696,  1697,  1698,  1699,
    1700,  1701,  1702,  1703,  1704,  1705,  1706,  1709,  1712,  1713,
    1716,  1717,  1719,  1723,  1736,  1750,  1756,  1761,  1766,  1771,
    1776,  1781,  1789,  1790,  1792,  1797,  1801,  1805,  1811,  1815,
    1819,  1823,  1827,  1833,  1836
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "FSC_LIST", "FSC_DEFINE", "FSC_EXEC",
  "FSC_QUIT", "FSC_DEBUG", "FSC_CREATE", "FSC_SLEEP", "FSC_STATS",
  "FSC_FOREACH", "FSC_SET", "FSC_SHUTDOWN", "FSC_LOG", "FSC_SYSTEM",
  "FSC_FLOWOP", "FSC_EVENTGEN", "FSC_ECHO", "FSC_LOAD", "FSC_RUN",
  "FSC_WARMUP", "FSC_NOUSESTATS", "FSC_FSCHECK", "FSC_FSFLUSH",
  "FSC_USAGE", "FSC_HELP", "FSC_VARS", "FSC_VERSION", "FSC_ENABLE",
  "FSC_DOMULTISYNC", "FSV_STRING", "FSV_VAL_INT", "FSV_VAL_BOOLEAN",
  "FSV_VARIABLE", "FSV_WHITESTRING", "FSV_RANDUNI", "FSV_RANDTAB",
  "FSV_RANDVAR", "FSV_URAND", "FSV_RAND48", "FST_INT", "FST_BOOLEAN",
  "FSE_FILE", "FSE_PROC", "FSE_THREAD", "FSE_CLEAR", "FSE_ALL", "FSE_SNAP",
  "FSE_DUMP", "FSE_DIRECTORY", "FSE_COMMAND", "FSE_FILESET", "FSE_POSSET",
  "FSE_XMLDUMP", "FSE_RAND", "FSE_MODE", "FSE_MULTI", "FSE_MULTIDUMP",
  "FSK_SEPLST", "FSK_OPENLST", "FSK_CLOSELST", "FSK_ASSIGN", "FSK_IN",
  "FSK_QUOTE", "FSK_DIRSEPLST", "FSK_PLUS", "FSK_MINUS", "FSK_MULTIPLY",
  "FSK_DIVIDE", "FSA_SIZE", "FSA_PREALLOC", "FSA_PARALLOC", "FSA_PATH",
  "FSA_REUSE", "FSA_PROCESS", "FSA_MEMSIZE", "FSA_RATE", "FSA_CACHED",
  "FSA_READONLY", "FSA_TRUSTTREE", "FSA_IOSIZE", "FSA_FILE", "FSA_POSSET",
  "FSA_WSS", "FSA_NAME", "FSA_RANDOM", "FSA_INSTANCES", "FSA_DSYNC",
  "FSA_TARGET", "FSA_ITERS", "FSA_NICE", "FSA_VALUE", "FSA_BLOCKING",
  "FSA_HIGHWATER", "FSA_DIRECTIO", "FSA_DIRWIDTH", "FSA_FD", "FSA_SRCFD",
  "FSA_ROTATEFD", "FSA_NAMELENGTH", "FSA_FILESIZE", "FSA_ENTRIES",
  "FSA_FILESIZEGAMMA", "FSA_DIRDEPTHRV", "FSA_DIRGAMMA", "FSA_USEISM",
  "FSA_TYPE", "FSA_RANDTABLE", "FSA_RANDSRC", "FSA_RANDROUND",
  "FSA_LEAFDIRS", "FSA_INDEXED", "FSA_FSTYPE", "FSA_RANDSEED",
  "FSA_RANDGAMMA", "FSA_RANDMEAN", "FSA_RANDMIN", "FSA_RANDMAX",
  "FSA_MASTER", "FSA_CLIENT", "FSS_TYPE", "FSS_SEED", "FSS_GAMMA",
  "FSS_MEAN", "FSS_MIN", "FSS_SRC", "FSS_ROUND", "FSV_SET_LOCAL_VAR",
  "FSA_LVAR_ASSIGN", "FSA_ALLDONE", "FSA_FIRSTDONE", "FSA_TIMEOUT",
  "FSC_OSPROF_ENABLE", "FSC_OSPROF_DISABLE", "FSA_NOREADAHEAD",
  "FSA_IOPRIO", "FSA_WRITEONLY", "$accept", "commands", "inner_commands",
  "command", "foreach_command", "integer_seplist", "string_seplist",
  "eventgen_command", "system_command", "echo_command", "version_command",
  "osprof_enable_command", "osprof_disable_command", "usage_command",
  "vars_command", "enable_command", "multisync_command", "var_string",
  "var_string_list", "whitevar_string", "whitevar_string_list",
  "list_command", "fscheck_command", "fsflush_command", "log_command",
  "debug_command", "set_command", "set_integer_command",
  "set_other_command", "stats_command", "quit_command", "flowop_list",
  "thread", "thread_list", "proc_define_command", "files_define_command",
  "posset_define_command", "randvar_define_command", "fo_define_command",
  "create_command", "shutdown_command", "warmup_command", "sleep_command",
  "run_command", "help_command", "flowop_command", "load_command",
  "entity", "name", "files_attr_ops", "posset_attr_ops", "posset_attr_op",
  "files_attr_op", "randvar_attr_ops", "randvar_attr_op", "probtabentry",
  "probtabentry_list", "pt_attr_ops", "pt_attr_op", "fo_attr_ops",
  "fo_attr_op", "ev_attr_ops", "ev_attr_op", "enable_multi_ops",
  "enable_multi_op", "multisync_op", "fscheck_attr_op", "binary_op",
  "files_attr_name", "posset_attr_name", "pt_attr_name", "fo_attr_name",
  "ev_attr_name", "attrs_define_proc", "attrs_define_file",
  "attrs_define_fileset", "attrs_define_posset", "randvar_attr_name",
  "randvar_attr_tsp", "randvar_attr_param", "randvar_attr_typop",
  "randtype_name", "randvar_attr_srcop", "randsrc_name",
  "attrs_define_thread", "attrs_flowop", "attrs_eventgen", "em_attr_name",
  "fscheck_attr_name", "comp_attr_ops", "comp_attr_op", "comp_lvar_def",
  "attrs_define_comp", "attr_value", "attr_list_value", "var_int_val", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392
};
# endif

#define YYPACT_NINF (-287)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -287,    16,  -287,  -287,  -287,   121,  -287,   -29,   -34,   136,
     268,  -287,   141,   -34,    -4,    -4,  -287,    -4,    46,   190,
     249,    43,    43,    -4,  -287,  -287,  -287,    65,    69,  -287,
    -287,  -287,   158,   134,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,    61,  -287,   185,    43,  -287,  -287,  -287,  -287,   171,
    -287,  -287,  -287,    66,   147,    53,  -287,   119,  -287,  -287,
    -287,  -287,  -287,  -287,  -287,   119,  -287,    66,  -287,  -287,
     195,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,    -4,   -19,    -4,    -4,    -4,   168,   206,   201,  -287,
     286,   105,   124,   124,   124,  -287,  -287,  -287,  -287,  -287,
    -287,  -287,  -287,   173,  -287,   124,  -287,   180,  -287,   194,
    -287,   215,  -287,   203,  -287,  -287,  -287,   226,  -287,   234,
    -287,  -287,  -287,  -287,  -287,  -287,   263,  -287,  -287,  -287,
    -287,  -287,  -287,  -287,   244,  -287,   245,  -287,  -287,  -287,
    -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,  -287,  -287,  -287,  -287,   256,  -287,   289,  -287,  -287,
    -287,  -287,  -287,  -287,  -287,   266,  -287,   300,  -287,  -287,
    -287,   304,  -287,   302,   264,   290,  -287,   303,   305,  -287,
    -287,  -287,  -287,  -287,   307,  -287,   306,   124,  -287,  -287,
     197,   210,   124,   124,   124,   214,   308,  -287,  -287,  -287,
    -287,   309,  -287,   311,   157,  -287,  -287,  -287,  -287,  -287,
     213,  -287,  -287,  -287,   213,  -287,   338,   310,   -26,   134,
     310,    61,   310,  -287,  -287,    66,   310,   147,   314,    53,
     314,   118,   310,   356,   329,    95,   317,   184,   314,  -287,
    -287,  -287,  -287,  -287,  -287,  -287,  -287,   340,    95,   317,
     310,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
     341,   294,   299,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,  -287,  -287,  -287,   210,  -287,  -287,  -287,   315,  -287,
    -287,  -287,   347,     5,   178,    66,  -287,   102,  -287,  -287,
    -287,  -287,  -287,  -287,  -287,  -287,  -287,   318,  -287,  -287,
     319,  -287,  -287,  -287,   320,   349,   100,   321,   100,   222,
    -287,  -287,  -287,   178,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,  -287,   323,  -287,   324,  -287,   301,  -287,  -287,   327,
    -287,  -287,  -287,    44,  -287,   344,    72,  -287,  -287,  -287,
    -287,   353,   101,   310,   356,   330,  -287,  -287,   325,  -287,
     328,  -287,  -287,  -287,    33,   274,  -287,    75,  -287,  -287,
    -287,  -287,  -287,   332,   330,  -287,   274,  -287,   334,   274,
     333,  -287
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       4,     0,     1,     3,    74,     0,   104,     0,     0,     0,
       0,    38,     0,     0,     0,     0,    45,     0,     0,   128,
       0,     0,     0,     0,   129,    53,    49,     0,     0,    50,
      51,     2,    18,    13,    31,    15,    32,    33,    34,    16,
      17,    35,    36,    22,    19,    20,    24,    12,    26,    81,
      82,    30,    37,     7,     8,     9,    10,    11,    14,    27,
      29,    28,    25,    21,    23,     0,   112,     0,   113,   115,
       0,    80,   136,   133,   134,   135,   120,   124,   125,    98,
      97,     0,     0,     0,     0,     0,     0,     0,     0,   121,
       0,     0,    79,    47,    48,   132,   126,   127,   122,   123,
     270,   271,    76,     0,    78,    52,    54,     0,    56,     0,
     267,    46,   165,   168,   184,   268,   269,    55,   169,     0,
      75,    77,   174,   175,   176,   177,     0,   243,   245,   186,
     187,   185,   246,   248,   111,   156,   159,   182,   181,   188,
     194,   195,   190,   193,   209,   191,   192,   189,   200,   210,
     207,   201,   208,   211,   196,   114,   138,   145,   178,   179,
     213,   216,   214,   215,   217,   116,   140,   143,   180,   282,
     283,   119,   272,     0,     0,     0,   218,     0,     0,   223,
     219,   220,   221,   222,   117,   146,   150,   101,    58,    57,
      59,    99,   100,   102,   103,     0,     0,   231,   232,   233,
     234,     0,   235,     0,     0,    93,    64,    65,    67,    66,
       0,    73,    70,    69,     0,    72,     0,     0,     0,     0,
       0,     0,     0,    85,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    60,
      61,    62,    63,    89,    83,    87,    84,     0,     0,     0,
       0,    91,    92,    90,   224,   226,   227,   228,   229,   225,
     230,    68,    71,   173,   284,   285,   286,   287,   172,    41,
       0,     0,     0,   166,   167,   170,   171,   157,   158,   139,
      58,   290,   291,    57,   288,   144,   141,   142,   281,   273,
     274,   275,     0,     0,   105,     0,   108,     0,   237,   238,
     239,   151,   236,   241,   242,   152,   240,     0,   147,   149,
       0,    94,    95,    96,     0,     0,     0,     0,     0,     0,
     137,   130,   118,   106,   265,   250,   251,   249,   252,   253,
     257,   260,   261,   262,   263,   264,   258,   254,   255,   256,
     259,   266,   131,   160,   164,   183,     0,   110,   109,     0,
      88,    43,    42,     0,     5,     0,     0,   279,   277,   276,
     280,     0,     0,     0,     0,     0,    39,     6,     0,    40,
       0,   161,   162,   163,     0,     0,   154,     0,    44,   278,
     107,   293,   294,     0,     0,   148,     0,   155,     0,     0,
       0,   153
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -287,  -287,    77,    -1,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,   316,  -287,
     131,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,    32,   103,  -287,  -287,  -287,  -287,  -287,  -287,  -287,
    -287,  -287,  -287,  -287,  -287,  -286,  -287,   384,  -287,  -287,
    -287,   170,   174,  -287,   165,    19,  -287,   -66,   179,  -287,
      45,  -287,   186,  -287,   187,  -287,   122,  -287,  -287,  -287,
    -287,  -287,  -287,  -287,  -287,  -287,  -287,  -287,   192,  -287,
     161,  -287,   162,  -287,  -287,  -287,  -287,  -287,  -287,   345,
     181,    51,  -287,  -218,  -225,  -107
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,   353,   354,    32,   271,   272,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,   190,   284,    91,
      92,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,   293,   296,   297,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,   294,    64,    76,   321,   155,
     165,   166,   156,   184,   185,   376,   377,   134,   135,   342,
     343,   111,   112,   117,   118,   108,   102,   126,   157,   167,
     136,   344,   113,   137,   158,   159,   168,   186,   261,   203,
     301,   302,   305,   306,   138,   345,   114,   119,   103,   171,
     172,   290,   173,   268,   285,   383
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      31,   175,   274,    71,   276,   287,   269,   323,   278,    72,
      73,    74,   188,   309,   291,   189,     2,     3,    75,     4,
       5,   292,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,   313,    16,    17,    18,    19,    20,   270,    21,
      22,    23,    24,    25,    26,    27,    28,     4,     5,   292,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      90,    16,    17,    18,    19,    20,   322,    21,    22,    23,
      24,    25,    26,    27,    28,     4,     5,    95,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,   323,    16,
      17,    18,    19,    20,   380,    21,    22,    23,    24,    25,
      26,    27,    28,     4,     5,   366,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,   100,    16,    17,    18,
      19,    20,   106,    21,    22,    23,    24,    25,    26,    27,
      28,   298,   299,   369,   384,   288,   385,    65,   160,   208,
     209,   127,   128,   210,   104,   373,    93,   295,    94,    29,
      30,   129,   288,   130,   105,   161,   101,   131,   212,   213,
     162,   107,   214,   347,    66,    67,   121,   163,    77,   211,
      78,   164,   132,    68,    69,    86,    70,    29,    30,    87,
     115,   116,   324,   325,   326,   327,   328,   329,   215,   330,
     331,   332,   109,   333,   334,   335,   336,    88,   337,   338,
     339,   120,   133,   169,   169,    29,    30,   204,   170,   170,
     300,   110,   187,   340,   192,   193,   194,   139,   140,   141,
     142,   143,    96,   205,    97,   144,   145,   146,   239,   346,
     195,   240,   147,    29,    30,   216,   341,   122,   123,   124,
     125,   241,   217,   148,   242,   243,   244,   245,   246,   149,
     150,   151,   152,   357,   358,   359,   360,   218,   153,   324,
     325,   326,   327,   328,   329,   220,   330,   331,   332,   176,
     333,   334,   335,   336,   219,   337,   338,   339,   247,   388,
     176,    98,   390,    99,   154,   221,   361,   251,   252,   253,
     340,   177,   307,   178,   179,   223,   222,   224,   180,   181,
     182,   183,   177,   225,   178,   179,   381,   226,   382,   180,
     181,   182,   183,   341,    79,   227,    80,    81,    82,    83,
     206,   207,    84,   231,   233,   229,    85,   196,   197,   198,
     199,   200,   201,   202,   254,   255,   256,   257,   258,   259,
     260,   264,   265,   266,   267,   280,   281,   282,   283,   225,
     234,   228,   367,   315,   316,   367,   303,   304,   317,   318,
     225,   364,   230,   231,   232,   235,   237,   236,   238,   263,
     248,   249,   292,   250,   295,   310,   314,   319,   320,   368,
     349,   352,   362,   350,   351,   355,   363,   365,   370,   378,
     375,   386,   379,   389,   391,   356,   374,    89,   191,   286,
     348,   279,   308,   387,   277,   273,   262,   371,   275,   311,
     174,   312,   289,   372
};

static const yytype_int16 yycheck[] =
{
       1,    67,   220,    32,   222,   230,    32,   293,   226,    43,
      44,    45,    31,   238,   232,    34,     0,     1,    52,     3,
       4,    16,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,   250,    17,    18,    19,    20,    21,    64,    23,
      24,    25,    26,    27,    28,    29,    30,     3,     4,    16,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      64,    17,    18,    19,    20,    21,    61,    23,    24,    25,
      26,    27,    28,    29,    30,     3,     4,    31,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,   374,    17,
      18,    19,    20,    21,    61,    23,    24,    25,    26,    27,
      28,    29,    30,     3,     4,    61,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    73,    17,    18,    19,
      20,    21,    57,    23,    24,    25,    26,    27,    28,    29,
      30,    36,    37,    61,    59,    34,    61,    16,    85,    34,
      35,    75,    76,    38,    22,   363,    15,    45,    17,   133,
     134,    85,    34,    87,    23,   102,   113,    91,    34,    35,
     107,    92,    38,    61,    43,    44,    44,   114,    32,    64,
      34,   118,   106,    52,    53,    34,    55,   133,   134,    38,
     119,   120,    81,    82,    83,    84,    85,    86,    64,    88,
      89,    90,    34,    92,    93,    94,    95,    56,    97,    98,
      99,    16,   136,    85,    85,   133,   134,     6,    90,    90,
     115,    77,    81,   112,    83,    84,    85,    70,    71,    72,
      73,    74,    32,    22,    34,    78,    79,    80,    31,   295,
      62,    34,    85,   133,   134,    62,   135,    66,    67,    68,
      69,    31,    62,    96,    34,    31,    32,    33,    34,   102,
     103,   104,   105,    31,    32,    33,    34,    63,   111,    81,
      82,    83,    84,    85,    86,    62,    88,    89,    90,    85,
      92,    93,    94,    95,    59,    97,    98,    99,    64,   386,
      85,    32,   389,    34,   137,    59,    64,   130,   131,   132,
     112,   107,   108,   109,   110,    32,    62,    34,   114,   115,
     116,   117,   107,    59,   109,   110,    32,    62,    34,   114,
     115,   116,   117,   135,    46,    59,    48,    49,    50,    51,
      34,    35,    54,    59,    60,    59,    58,   121,   122,   123,
     124,   125,   126,   127,   121,   122,   123,   124,   125,   126,
     127,    31,    32,    33,    34,    31,    32,    33,    34,    59,
      60,    62,   353,    59,    60,   356,    39,    40,    59,    60,
      59,    60,    62,    59,    62,    62,    59,    62,    62,    31,
      62,    62,    16,    62,    45,    35,    35,    62,    31,    35,
      62,    32,    59,    64,    64,    64,    62,    60,    35,    64,
      60,    59,    64,    59,    61,   318,   364,    13,    82,   229,
     297,   227,   237,   384,   225,   219,   214,   362,   221,   248,
      65,   249,   231,   362
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   139,     0,     1,     3,     4,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    17,    18,    19,    20,
      21,    23,    24,    25,    26,    27,    28,    29,    30,   133,
     134,   141,   142,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   184,    16,    43,    44,    52,    53,
      55,    32,    43,    44,    45,    52,   185,    32,    34,    46,
      48,    49,    50,    51,    54,    58,    34,    38,    56,   185,
      64,   157,   158,   158,   158,    31,    32,    34,    32,    34,
      73,   113,   204,   226,   204,   158,    57,    92,   203,    34,
      77,   199,   200,   210,   224,   119,   120,   201,   202,   225,
      16,   204,    66,    67,    68,    69,   205,    75,    76,    85,
      87,    91,   106,   136,   195,   196,   208,   211,   222,    70,
      71,    72,    73,    74,    78,    79,    80,    85,    96,   102,
     103,   104,   105,   111,   137,   187,   190,   206,   212,   213,
      85,   102,   107,   114,   118,   188,   189,   207,   214,    85,
      90,   227,   228,   230,   227,   195,    85,   107,   109,   110,
     114,   115,   116,   117,   191,   192,   215,   158,    31,    34,
     155,   156,   158,   158,   158,    62,   121,   122,   123,   124,
     125,   126,   127,   217,     6,    22,    34,    35,    34,    35,
      38,    64,    34,    35,    38,    64,    62,    62,    63,    59,
      62,    59,    62,    32,    34,    59,    62,    59,    62,    59,
      62,    59,    62,    60,    60,    62,    62,    59,    62,    31,
      34,    31,    34,    31,    32,    33,    34,    64,    62,    62,
      62,   130,   131,   132,   121,   122,   123,   124,   125,   126,
     127,   216,   216,    31,    31,    32,    33,    34,   231,    32,
      64,   143,   144,   200,   231,   202,   231,   196,   231,   190,
      31,    32,    33,    34,   156,   232,   189,   232,    34,   228,
     229,   231,    16,   169,   183,    45,   170,   171,    36,    37,
     115,   218,   219,    39,    40,   220,   221,   108,   192,   232,
      35,   218,   220,   231,    35,    59,    60,    59,    60,    62,
      31,   186,    61,   183,    81,    82,    83,    84,    85,    86,
      88,    89,    90,    92,    93,    94,    95,    97,    98,    99,
     112,   135,   197,   198,   209,   223,   195,    61,   170,    62,
      64,    64,    32,   140,   141,    64,   140,    31,    32,    33,
      34,    64,    59,    62,    60,    60,    61,   141,    35,    61,
      35,   198,   229,   231,   169,    60,   193,   194,    64,    64,
      61,    32,    34,   233,    59,    61,    59,   193,   233,    59,
     233,    61
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   138,   139,   139,   139,   140,   140,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   142,   142,
     142,   143,   143,   144,   144,   145,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   153,   154,   155,   155,   156,
     156,   156,   156,   156,   157,   157,   158,   158,   158,   158,
     158,   158,   158,   158,   159,   159,   160,   160,   161,   162,
     163,   164,   164,   165,   165,   165,   165,   166,   166,   166,
     166,   166,   166,   166,   166,   166,   166,   167,   167,   167,
     167,   167,   167,   167,   168,   169,   169,   170,   171,   171,
     172,   172,   173,   173,   173,   174,   174,   175,   176,   176,
     177,   178,   179,   179,   180,   180,   181,   181,   181,   182,
     183,   183,   184,   185,   185,   185,   185,   186,   187,   187,
     188,   188,   189,   189,   190,   190,   191,   191,   191,   192,
     192,   192,   192,   193,   194,   194,   195,   195,   196,   196,
     197,   197,   197,   198,   198,   199,   199,   200,   200,   201,
     201,   202,   203,   204,   205,   205,   205,   205,   206,   206,
     207,   208,   208,   209,   210,   211,   211,   211,   212,   212,
     212,   212,   212,   212,   212,   212,   212,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   214,   214,   214,   214,   214,   215,   215,
     215,   215,   215,   215,   216,   216,   216,   216,   216,   216,
     216,   217,   217,   217,   217,   217,   218,   219,   219,   219,
     220,   221,   221,   222,   222,   222,   222,   222,   222,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   224,   225,   225,
     226,   226,   227,   227,   227,   228,   229,   229,   229,   229,
     229,   229,   230,   230,   231,   231,   231,   231,   232,   232,
     232,   232,   232,   233,   233
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     2,     0,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     7,
       7,     1,     3,     3,     5,     1,     2,     2,     2,     1,
       1,     1,     2,     1,     2,     2,     2,     1,     1,     1,
       2,     2,     2,     2,     2,     2,     2,     2,     3,     2,
       2,     3,     2,     2,     1,     2,     2,     2,     2,     2,
       2,     1,     1,     4,     4,     3,     3,     4,     6,     4,
       4,     4,     4,     3,     5,     5,     5,     2,     2,     3,
       3,     3,     3,     3,     1,     1,     2,     5,     1,     2,
       6,     2,     2,     2,     2,     2,     2,     3,     6,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     1,     1,
       2,     2,     2,     1,     1,     1,     1,     1,     1,     3,
       1,     3,     3,     1,     3,     1,     1,     3,     7,     3,
       1,     3,     3,     7,     1,     3,     1,     3,     3,     1,
       1,     3,     3,     3,     1,     1,     3,     3,     1,     1,
       3,     3,     3,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     3,     3,     3,     3,     5,     3,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2:
#line 257 "parser_gram.y"
{
	if ((yyvsp[0].cmd)->cmd != NULL)
		(yyvsp[0].cmd)->cmd((yyvsp[0].cmd));

	free((yyvsp[0].cmd));
}
#line 2091 "parser_gram.c"
    break;

  case 3:
#line 264 "parser_gram.y"
{
	if (dofile == DOFILE_TRUE)
		YYABORT;
}
#line 2100 "parser_gram.c"
    break;

  case 5:
#line 271 "parser_gram.y"
{
	filebench_log(LOG_DEBUG_IMPL, "inner_command %zx", (yyvsp[0].cmd));
	(yyval.cmd) = (yyvsp[0].cmd);
}
#line 2109 "parser_gram.c"
    break;

  case 6:
#line 276 "parser_gram.y"
{
	cmd_t *list = NULL;
	cmd_t *list_end = NULL;

	/* Find end of list */
	for (list = (yyvsp[-1].cmd); list != NULL;
	    list = list->cmd_next)
		list_end = list;

	list_end->cmd_next = (yyvsp[0].cmd);

	filebench_log(LOG_DEBUG_IMPL,
	    "inner_commands adding cmd %zx to list %zx", (yyvsp[0].cmd), (yyvsp[-1].cmd));

	(yyval.cmd) = (yyvsp[-1].cmd);
}
#line 2130 "parser_gram.c"
    break;

  case 38:
#line 327 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	filebench_log(LOG_DEBUG_IMPL, "foreach_command %zx", (yyval.cmd));
}
#line 2140 "parser_gram.c"
    break;

  case 39:
#line 333 "parser_gram.y"
{
	cmd_t *inner_cmd;
	list_t *list;

	(yyval.cmd) = (yyvsp[-6].cmd);
	(yyval.cmd)->cmd_list = (yyvsp[-1].cmd);
	(yyval.cmd)->cmd_tgt1 = (yyvsp[-5].sval);
	(yyval.cmd)->cmd_param_list = (yyvsp[-3].list);
	(yyval.cmd)->cmd = parser_foreach_integer;

	for (list = (yyval.cmd)->cmd_param_list; list != NULL;
	    list = list->list_next) {
		for (inner_cmd = (yyval.cmd)->cmd_list;
		    inner_cmd != NULL;
		    inner_cmd = inner_cmd->cmd_next) {
			filebench_log(LOG_DEBUG_IMPL,
			    "packing foreach: %zx %s=%llu, cmd %zx",
			    (yyval.cmd), (yyval.cmd)->cmd_tgt1,
			    (u_longlong_t)avd_get_int(list->list_integer),
			    inner_cmd);
		}
	}
}
#line 2168 "parser_gram.c"
    break;

  case 40:
#line 356 "parser_gram.y"
{
	cmd_t *inner_cmd;
	list_t *list;

	(yyval.cmd) = (yyvsp[-6].cmd);
	(yyval.cmd)->cmd_list = (yyvsp[-1].cmd);
	(yyval.cmd)->cmd_tgt1 = (yyvsp[-5].sval);
	(yyval.cmd)->cmd_param_list = (yyvsp[-3].list);
	(yyval.cmd)->cmd = parser_foreach_string;

	for (list = (yyval.cmd)->cmd_param_list; list != NULL;
	    list = list->list_next) {
		for (inner_cmd = (yyval.cmd)->cmd_list;
		    inner_cmd != NULL;
		    inner_cmd = inner_cmd->cmd_next) {
			filebench_log(LOG_DEBUG_IMPL,
			    "packing foreach: %zx %s=%s, cmd %zx",
			    (yyval.cmd),
			    (yyval.cmd)->cmd_tgt1,
			    *list->list_string, inner_cmd);
		}
	}
}
#line 2196 "parser_gram.c"
    break;

  case 41:
#line 381 "parser_gram.y"
{
	if (((yyval.list) = alloc_list()) == NULL)
		YYERROR;

	(yyval.list)->list_integer = avd_int_alloc((yyvsp[0].ival));
}
#line 2207 "parser_gram.c"
    break;

  case 42:
#line 388 "parser_gram.y"
{
	list_t *list = NULL;
	list_t *list_end = NULL;

	if (((yyval.list) = alloc_list()) == NULL)
		YYERROR;

	(yyval.list)->list_integer = avd_int_alloc((yyvsp[0].ival));

	/* Find end of list */
	for (list = (yyvsp[-2].list); list != NULL;
	    list = list->list_next)
		list_end = list;
	list_end->list_next = (yyval.list);
	(yyval.list) = (yyvsp[-2].list);
}
#line 2228 "parser_gram.c"
    break;

  case 43:
#line 406 "parser_gram.y"
{
	if (((yyval.list) = alloc_list()) == NULL)
		YYERROR;

	(yyval.list)->list_string = avd_str_alloc((yyvsp[-1].sval));
}
#line 2239 "parser_gram.c"
    break;

  case 44:
#line 413 "parser_gram.y"
{
	list_t *list = NULL;
	list_t *list_end = NULL;

	if (((yyval.list) = alloc_list()) == NULL)
			YYERROR;

	(yyval.list)->list_string = avd_str_alloc((yyvsp[-1].sval));

	/* Find end of list */
	for (list = (yyvsp[-4].list); list != NULL;
	    list = list->list_next)
		list_end = list;
	list_end->list_next = (yyval.list);
	(yyval.list) = (yyvsp[-4].list);
}
#line 2260 "parser_gram.c"
    break;

  case 45:
#line 431 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	(yyval.cmd)->cmd = &parser_eventgen;
}
#line 2270 "parser_gram.c"
    break;

  case 46:
#line 437 "parser_gram.y"
{
	(yyvsp[-1].cmd)->cmd_attr_list = (yyvsp[0].attr);
}
#line 2278 "parser_gram.c"
    break;

  case 47:
#line 442 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;

	(yyval.cmd)->cmd_param_list = (yyvsp[0].list);
	(yyval.cmd)->cmd = parser_system;
}
#line 2290 "parser_gram.c"
    break;

  case 48:
#line 451 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;

	(yyval.cmd)->cmd_param_list = (yyvsp[0].list);
	(yyval.cmd)->cmd = parser_echo;
}
#line 2302 "parser_gram.c"
    break;

  case 49:
#line 460 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	(yyval.cmd)->cmd = parser_version;
}
#line 2312 "parser_gram.c"
    break;

  case 50:
#line 467 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	(yyval.cmd)->cmd = parser_osprof_enable;
}
#line 2322 "parser_gram.c"
    break;

  case 51:
#line 474 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	(yyval.cmd)->cmd = parser_osprof_disable;
}
#line 2332 "parser_gram.c"
    break;

  case 52:
#line 481 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;

	(yyval.cmd)->cmd_param_list = (yyvsp[0].list);
	(yyval.cmd)->cmd = parser_usage;
}
#line 2344 "parser_gram.c"
    break;

  case 53:
#line 490 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;

	(yyval.cmd)->cmd = parser_printvars;
}
#line 2355 "parser_gram.c"
    break;

  case 54:
#line 498 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;

	(yyval.cmd)->cmd = parser_enable_mc;
}
#line 2366 "parser_gram.c"
    break;

  case 55:
#line 505 "parser_gram.y"
{
	(yyvsp[-1].cmd)->cmd_attr_list = (yyvsp[0].attr);
}
#line 2374 "parser_gram.c"
    break;

  case 56:
#line 510 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;

	(yyval.cmd)->cmd = parser_domultisync;
	(yyval.cmd)->cmd_attr_list = (yyvsp[0].attr);
}
#line 2386 "parser_gram.c"
    break;

  case 57:
#line 543 "parser_gram.y"
{
	if (((yyval.list) = alloc_list()) == NULL)
			YYERROR;

	(yyval.list)->list_string = avd_str_alloc((yyvsp[0].sval));
}
#line 2397 "parser_gram.c"
    break;

  case 58:
#line 550 "parser_gram.y"
{
	if (((yyval.list) = alloc_list()) == NULL)
			YYERROR;

	(yyval.list)->list_string = avd_str_alloc((yyvsp[0].sval));
}
#line 2408 "parser_gram.c"
    break;

  case 59:
#line 558 "parser_gram.y"
{
	(yyval.list) = (yyvsp[0].list);
}
#line 2416 "parser_gram.c"
    break;

  case 60:
#line 561 "parser_gram.y"
{
	list_t *list = NULL;
	list_t *list_end = NULL;

	/* Add string */
	if (((yyval.list) = alloc_list()) == NULL)
		YYERROR;

	(yyval.list)->list_string = avd_str_alloc((yyvsp[0].sval));

	/* Find end of list */
	for (list = (yyvsp[-1].list); list != NULL;
	    list = list->list_next)
		list_end = list;
	list_end->list_next = (yyval.list);
	(yyval.list) = (yyvsp[-1].list);

}
#line 2439 "parser_gram.c"
    break;

  case 61:
#line 579 "parser_gram.y"
{
	list_t *list = NULL;
	list_t *list_end = NULL;

	/* Add variable */
	if (((yyval.list) = alloc_list()) == NULL)
		YYERROR;

	(yyval.list)->list_string = avd_str_alloc((yyvsp[0].sval));

	/* Find end of list */
	for (list = (yyvsp[-1].list); list != NULL;
	    list = list->list_next)
		list_end = list;
	list_end->list_next = (yyval.list);
	(yyval.list) = (yyvsp[-1].list);
}
#line 2461 "parser_gram.c"
    break;

  case 62:
#line 596 "parser_gram.y"
{
	list_t *list = NULL;
	list_t *list_end = NULL;

	/* Add string */
	if (((yyval.list) = alloc_list()) == NULL)
		YYERROR;

	(yyval.list)->list_string = avd_str_alloc((yyvsp[0].sval));

	/* Find end of list */
	for (list = (yyvsp[-1].list); list != NULL;
	    list = list->list_next)
		list_end = list;
	list_end->list_next = (yyval.list);
	(yyval.list) = (yyvsp[-1].list);

}
#line 2484 "parser_gram.c"
    break;

  case 63:
#line 614 "parser_gram.y"
{
	list_t *list = NULL;
	list_t *list_end = NULL;

	/* Add variable */
	if (((yyval.list) = alloc_list()) == NULL)
		YYERROR;

	(yyval.list)->list_string = avd_str_alloc((yyvsp[0].sval));

	/* Find end of list */
	for (list = (yyvsp[-1].list); list != NULL;
	    list = list->list_next)
		list_end = list;
	list_end->list_next = (yyval.list);
	(yyval.list) = (yyvsp[-1].list);
}
#line 2506 "parser_gram.c"
    break;

  case 64:
#line 633 "parser_gram.y"
{
	if (((yyval.list) = alloc_list()) == NULL)
			YYERROR;

	(yyval.list)->list_string = avd_str_alloc((yyvsp[0].sval));
}
#line 2517 "parser_gram.c"
    break;

  case 65:
#line 640 "parser_gram.y"
{
	if (((yyval.list) = alloc_list()) == NULL)
			YYERROR;

	(yyval.list)->list_string = avd_str_alloc((yyvsp[0].sval));
}
#line 2528 "parser_gram.c"
    break;

  case 66:
#line 648 "parser_gram.y"
{
	list_t *list = NULL;
	list_t *list_end = NULL;

	/* Add string */
	if (((yyval.list) = alloc_list()) == NULL)
		YYERROR;

	(yyval.list)->list_string = avd_str_alloc((yyvsp[0].sval));

	/* Find end of list */
	for (list = (yyvsp[-1].list); list != NULL;
	    list = list->list_next)
		list_end = list;
	list_end->list_next = (yyval.list);
	(yyval.list) = (yyvsp[-1].list);

}
#line 2551 "parser_gram.c"
    break;

  case 67:
#line 666 "parser_gram.y"
{
	list_t *list = NULL;
	list_t *list_end = NULL;

	/* Add variable */
	if (((yyval.list) = alloc_list()) == NULL)
		YYERROR;

	(yyval.list)->list_string = avd_str_alloc((yyvsp[0].sval));

	/* Find end of list */
	for (list = (yyvsp[-1].list); list != NULL;
	    list = list->list_next)
		list_end = list;
	list_end->list_next = (yyval.list);
	(yyval.list) = (yyvsp[-1].list);
}
#line 2573 "parser_gram.c"
    break;

  case 68:
#line 683 "parser_gram.y"
{
	list_t *list = NULL;
	list_t *list_end = NULL;

	/* Add variable */
	if (((yyval.list) = alloc_list()) == NULL)
		YYERROR;

	(yyval.list)->list_string = avd_str_alloc((yyvsp[-1].sval));
	(yyval.list)->list_integer = avd_int_alloc((yyvsp[0].ival));

	/* Find end of list */
	for (list = (yyvsp[-2].list); list != NULL;
	    list = list->list_next)
		list_end = list;
	list_end->list_next = (yyval.list);
	(yyval.list) = (yyvsp[-2].list);
}
#line 2596 "parser_gram.c"
    break;

  case 69:
#line 701 "parser_gram.y"
{
	list_t *list = NULL;
	list_t *list_end = NULL;

	/* Add string */
	if (((yyval.list) = alloc_list()) == NULL)
		YYERROR;

	(yyval.list)->list_string = avd_str_alloc((yyvsp[0].sval));

	/* Find end of list */
	for (list = (yyvsp[-1].list); list != NULL;
	    list = list->list_next)
		list_end = list;
	list_end->list_next = (yyval.list);
	(yyval.list) = (yyvsp[-1].list);

}
#line 2619 "parser_gram.c"
    break;

  case 70:
#line 719 "parser_gram.y"
{
	list_t *list = NULL;
	list_t *list_end = NULL;

	/* Add variable */
	if (((yyval.list) = alloc_list()) == NULL)
		YYERROR;

	(yyval.list)->list_string = avd_str_alloc((yyvsp[0].sval));

	/* Find end of list */
	for (list = (yyvsp[-1].list); list != NULL;
	    list = list->list_next)
		list_end = list;
	list_end->list_next = (yyval.list);
	(yyval.list) = (yyvsp[-1].list);
}
#line 2641 "parser_gram.c"
    break;

  case 71:
#line 736 "parser_gram.y"
{
	list_t *list = NULL;
	list_t *list_end = NULL;

	/* Add variable */
	if (((yyval.list) = alloc_list()) == NULL)
		YYERROR;

	(yyval.list)->list_string = avd_str_alloc((yyvsp[-1].sval));
	(yyval.list)->list_integer = avd_int_alloc((yyvsp[0].ival));

	/* Find end of list */
	for (list = (yyvsp[-2].list); list != NULL;
	    list = list->list_next)
		list_end = list;
	list_end->list_next = (yyval.list);
	(yyval.list) = (yyvsp[-2].list);
}
#line 2664 "parser_gram.c"
    break;

  case 72:
#line 754 "parser_gram.y"
{
	(yyval.list) = (yyvsp[-1].list);
}
#line 2672 "parser_gram.c"
    break;

  case 73:
#line 757 "parser_gram.y"
{
	(yyval.list) = (yyvsp[-1].list);
}
#line 2680 "parser_gram.c"
    break;

  case 74:
#line 762 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	(yyval.cmd)->cmd = &parser_list;
}
#line 2690 "parser_gram.c"
    break;

  case 75:
#line 768 "parser_gram.y"
{
	(yyvsp[-1].cmd)->cmd = &parser_flowop_list;
}
#line 2698 "parser_gram.c"
    break;

  case 76:
#line 773 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	(yyval.cmd)->cmd = &parser_fscheck;

	(yyval.cmd)->cmd_attr_list = (yyvsp[0].attr);
}
#line 2710 "parser_gram.c"
    break;

  case 77:
#line 781 "parser_gram.y"
{
	(yyvsp[-1].cmd)->cmd_attr_list->attr_next = (yyvsp[0].attr);
}
#line 2718 "parser_gram.c"
    break;

  case 78:
#line 786 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	(yyval.cmd)->cmd = &parser_fsflush;

	(yyval.cmd)->cmd_attr_list = (yyvsp[0].attr);
}
#line 2730 "parser_gram.c"
    break;

  case 79:
#line 795 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	(yyval.cmd)->cmd = &parser_log;
	(yyval.cmd)->cmd_param_list = (yyvsp[0].list);
}
#line 2741 "parser_gram.c"
    break;

  case 80:
#line 803 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	(yyval.cmd)->cmd = NULL;
	filebench_shm->shm_debug_level = (yyvsp[0].ival);
	if (filebench_shm->shm_debug_level > 9)
		yydebug = 1;
}
#line 2754 "parser_gram.c"
    break;

  case 83:
#line 817 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	(yyval.cmd)->cmd_tgt1 = (yyvsp[-2].sval);
	(yyval.cmd)->cmd_qty = (yyvsp[0].ival);
	if (parentscript) {
		parser_vars((yyval.cmd));
	}
	(yyval.cmd)->cmd = parser_set_integer;
}
#line 2769 "parser_gram.c"
    break;

  case 84:
#line 827 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	var_assign_var((yyvsp[-2].sval), (yyvsp[0].sval));
	(yyval.cmd)->cmd_tgt1 = (yyvsp[-2].sval);
	(yyval.cmd)->cmd_tgt2 = (yyvsp[0].sval);
	if (parentscript) {
		parser_vars((yyval.cmd));
	}
	(yyval.cmd)->cmd = parser_set_var;
}
#line 2785 "parser_gram.c"
    break;

  case 85:
#line 839 "parser_gram.y"
{
	if ((yyvsp[-2].cmd)->cmd == parser_set_integer) {
		switch ((yyvsp[-1].ival)) {
		case FSK_PLUS:
			var_assign_integer((yyvsp[-2].cmd)->cmd_tgt1, (yyvsp[-2].cmd)->cmd_qty + (yyvsp[0].ival));
			break;
		case FSK_MINUS:
			var_assign_integer((yyvsp[-2].cmd)->cmd_tgt1, (yyvsp[-2].cmd)->cmd_qty - (yyvsp[0].ival));
			break;
		case FSK_MULTIPLY:
			var_assign_integer((yyvsp[-2].cmd)->cmd_tgt1, (yyvsp[-2].cmd)->cmd_qty * (yyvsp[0].ival));
			break;
		case FSK_DIVIDE:
			var_assign_integer((yyvsp[-2].cmd)->cmd_tgt1, (yyvsp[-2].cmd)->cmd_qty / (yyvsp[0].ival));
			break;
		}
		(yyval.cmd)->cmd = NULL;
	} else {
		(yyvsp[-2].cmd)->cmd_qty = (yyvsp[0].ival);
		(yyvsp[-2].cmd)->cmd_subtype = (yyvsp[-1].ival);
		(yyvsp[-2].cmd)->cmd = parser_set_var_op_int;
	}
}
#line 2813 "parser_gram.c"
    break;

  case 86:
#line 863 "parser_gram.y"
{
	(yyvsp[-2].cmd)->cmd_tgt3 = (yyvsp[0].sval);
	(yyvsp[-2].cmd)->cmd_subtype = (yyvsp[-1].ival);
	if ((yyvsp[-2].cmd)->cmd == parser_set_integer) {
		(yyval.cmd)->cmd = parser_set_int_op_var;
	} else {
		(yyvsp[-2].cmd)->cmd = parser_set_var_op_var;
	}
}
#line 2827 "parser_gram.c"
    break;

  case 87:
#line 874 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	var_assign_boolean((yyvsp[-2].sval), (yyvsp[0].bval));
	if (parentscript) {
		(yyval.cmd)->cmd_tgt1 = (yyvsp[-2].sval);
		parser_vars((yyval.cmd));
	}
	(yyval.cmd)->cmd = NULL;
}
#line 2842 "parser_gram.c"
    break;

  case 88:
#line 885 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	var_assign_string((yyvsp[-4].sval), (yyvsp[-1].sval));
	if (parentscript) {
		(yyval.cmd)->cmd_tgt1 = (yyvsp[-4].sval);
		parser_vars((yyval.cmd));
	}
	(yyval.cmd)->cmd = NULL;
}
#line 2857 "parser_gram.c"
    break;

  case 89:
#line 895 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	var_assign_string((yyvsp[-2].sval), (yyvsp[0].sval));
	if (parentscript) {
		(yyval.cmd)->cmd_tgt1 = (yyvsp[-2].sval);
		parser_vars((yyval.cmd));
	}
	(yyval.cmd)->cmd = NULL;
}
#line 2872 "parser_gram.c"
    break;

  case 90:
#line 905 "parser_gram.y"
{
	filebench_shm->shm_rmode = FILEBENCH_MODE_TIMEOUT;
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	(yyval.cmd)->cmd = NULL;
}
#line 2883 "parser_gram.c"
    break;

  case 91:
#line 911 "parser_gram.y"
{
	filebench_shm->shm_rmode = FILEBENCH_MODE_QALLDONE;
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	(yyval.cmd)->cmd = NULL;
}
#line 2894 "parser_gram.c"
    break;

  case 92:
#line 917 "parser_gram.y"
{
	filebench_shm->shm_rmode = FILEBENCH_MODE_Q1STDONE;
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	(yyval.cmd)->cmd = NULL;
}
#line 2905 "parser_gram.c"
    break;

  case 93:
#line 923 "parser_gram.y"
{
	filebench_shm->shm_mmode |= FILEBENCH_MODE_NOUSAGE;
	filebench_log(LOG_INFO, "disabling CPU usage statistics");
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	(yyval.cmd)->cmd = NULL;
}
#line 2917 "parser_gram.c"
    break;

  case 94:
#line 930 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	(yyval.cmd)->cmd = &parser_randvar_set;
	(yyval.cmd)->cmd_tgt1 = (yyvsp[-3].sval);
	(yyval.cmd)->cmd_qty = FSS_TYPE;
	(yyval.cmd)->cmd_attr_list = (yyvsp[0].attr);

}
#line 2931 "parser_gram.c"
    break;

  case 95:
#line 939 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	(yyval.cmd)->cmd = &parser_randvar_set;
	(yyval.cmd)->cmd_tgt1 = (yyvsp[-3].sval);
	(yyval.cmd)->cmd_qty = FSS_SRC;
	(yyval.cmd)->cmd_attr_list = (yyvsp[0].attr);

}
#line 2945 "parser_gram.c"
    break;

  case 96:
#line 948 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	(yyval.cmd)->cmd = &parser_randvar_set;
	(yyval.cmd)->cmd_tgt1 = (yyvsp[-3].sval);
	(yyval.cmd)->cmd_qty = (yyvsp[-2].ival);
	(yyval.cmd)->cmd_attr_list = (yyvsp[0].attr);
	
}
#line 2959 "parser_gram.c"
    break;

  case 97:
#line 959 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	(yyval.cmd)->cmd = (void (*)(struct cmd *))&parser_statssnap;
	break;

}
#line 2971 "parser_gram.c"
    break;

  case 98:
#line 967 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	(yyval.cmd)->cmd = (void (*)(struct cmd *))&stats_clear;

}
#line 2982 "parser_gram.c"
    break;

  case 99:
#line 974 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	(yyval.cmd)->cmd_param_list = (yyvsp[0].list);
	(yyval.cmd)->cmd = (void (*)(struct cmd *))&parser_directory;

}
#line 2994 "parser_gram.c"
    break;

  case 100:
#line 982 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;

	(yyval.cmd)->cmd_param_list = (yyvsp[0].list);
	(yyval.cmd)->cmd = parser_statscmd;

}
#line 3007 "parser_gram.c"
    break;

  case 101:
#line 990 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;

	(yyval.cmd)->cmd_param_list = (yyvsp[0].list);
	(yyval.cmd)->cmd = parser_statsdump;
}
#line 3019 "parser_gram.c"
    break;

  case 102:
#line 997 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;

	(yyval.cmd)->cmd_param_list = (yyvsp[0].list);
	(yyval.cmd)->cmd = parser_statsxmldump;
}
#line 3031 "parser_gram.c"
    break;

  case 103:
#line 1004 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;

	(yyval.cmd)->cmd_param_list = (yyvsp[0].list);
	(yyval.cmd)->cmd = parser_statsmultidump;
}
#line 3043 "parser_gram.c"
    break;

  case 104:
#line 1013 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	(yyval.cmd)->cmd = parser_filebench_shutdown;
}
#line 3053 "parser_gram.c"
    break;

  case 105:
#line 1020 "parser_gram.y"
{
	(yyval.cmd) = (yyvsp[0].cmd);
}
#line 3061 "parser_gram.c"
    break;

  case 106:
#line 1023 "parser_gram.y"
{
	cmd_t *list = NULL;
	cmd_t *list_end = NULL;

	/* Find end of list */
	for (list = (yyvsp[-1].cmd); list != NULL;
	    list = list->cmd_next)
		list_end = list;

	list_end->cmd_next = (yyvsp[0].cmd);

	filebench_log(LOG_DEBUG_IMPL,
	    "flowop_list adding cmd %zx to list %zx", (yyvsp[0].cmd), (yyvsp[-1].cmd));

	(yyval.cmd) = (yyvsp[-1].cmd);
}
#line 3082 "parser_gram.c"
    break;

  case 107:
#line 1041 "parser_gram.y"
{
	/*
	 * Allocate a cmd node per thread, with a
	 * list of flowops attached to the cmd_list
	 */
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	(yyval.cmd)->cmd_list = (yyvsp[-1].cmd);
	(yyval.cmd)->cmd_attr_list = (yyvsp[-3].attr);
}
#line 3097 "parser_gram.c"
    break;

  case 108:
#line 1053 "parser_gram.y"
{
	(yyval.cmd) = (yyvsp[0].cmd);
}
#line 3105 "parser_gram.c"
    break;

  case 109:
#line 1056 "parser_gram.y"
{
	cmd_t *list = NULL;
	cmd_t *list_end = NULL;

	/* Find end of list */
	for (list = (yyvsp[-1].cmd); list != NULL;
	    list = list->cmd_next)
		list_end = list;

	list_end->cmd_next = (yyvsp[0].cmd);

	filebench_log(LOG_DEBUG_IMPL,
	    "thread_list adding cmd %zx to list %zx", (yyvsp[0].cmd), (yyvsp[-1].cmd));

	(yyval.cmd) = (yyvsp[-1].cmd);
}
#line 3126 "parser_gram.c"
    break;

  case 110:
#line 1074 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	(yyval.cmd)->cmd = &parser_proc_define;
	(yyval.cmd)->cmd_list = (yyvsp[-1].cmd);
	(yyval.cmd)->cmd_attr_list = (yyvsp[-3].attr);

}
#line 3139 "parser_gram.c"
    break;

  case 111:
#line 1083 "parser_gram.y"
{
	(yyvsp[-1].cmd)->cmd_attr_list = (yyvsp[0].attr);
}
#line 3147 "parser_gram.c"
    break;

  case 112:
#line 1088 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	(yyval.cmd)->cmd = &parser_file_define;
}
#line 3157 "parser_gram.c"
    break;

  case 113:
#line 1093 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	(yyval.cmd)->cmd = &parser_fileset_define;
}
#line 3167 "parser_gram.c"
    break;

  case 114:
#line 1099 "parser_gram.y"
{
	(yyvsp[-1].cmd)->cmd_attr_list = (yyvsp[0].attr);
}
#line 3175 "parser_gram.c"
    break;

  case 115:
#line 1104 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	(yyval.cmd)->cmd = &parser_posset_define;
}
#line 3185 "parser_gram.c"
    break;

  case 116:
#line 1109 "parser_gram.y"
{
	(yyvsp[-1].cmd)->cmd_attr_list = (yyvsp[0].attr);
}
#line 3193 "parser_gram.c"
    break;

  case 117:
#line 1114 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	(yyval.cmd)->cmd = &parser_randvar_define;
	(yyval.cmd)->cmd_attr_list = (yyvsp[0].attr);
}
#line 3204 "parser_gram.c"
    break;

  case 118:
#line 1122 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	(yyval.cmd)->cmd = &parser_composite_flowop_define;
	(yyval.cmd)->cmd_list = (yyvsp[-1].cmd);
	(yyval.cmd)->cmd_attr_list = (yyvsp[-3].attr);
}
#line 3216 "parser_gram.c"
    break;

  case 119:
#line 1130 "parser_gram.y"
{
	(yyvsp[-1].cmd)->cmd_attr_list = (yyvsp[0].attr);
}
#line 3224 "parser_gram.c"
    break;

  case 120:
#line 1135 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	switch ((yyvsp[0].ival)) {
	case FSE_PROC:
		(yyval.cmd)->cmd = &parser_proc_create;
		break;
	case FSE_FILESET:
	case FSE_FILE:
		(yyval.cmd)->cmd = &parser_fileset_create;
		break;
	default:
		filebench_log(LOG_ERROR, "unknown entity", (yyvsp[0].ival));
		YYERROR;
	}

}
#line 3246 "parser_gram.c"
    break;

  case 121:
#line 1154 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	switch ((yyvsp[0].ival)) {
	case FSE_PROC:
		(yyval.cmd)->cmd = &parser_proc_shutdown;
		break;
	case FSE_FILE:
	case FSE_FILESET:
		(yyval.cmd)->cmd = &parser_fileset_shutdown;
		break;
	default:
		filebench_log(LOG_ERROR, "unknown entity", (yyvsp[0].ival));
		YYERROR;
	}

}
#line 3268 "parser_gram.c"
    break;

  case 122:
#line 1173 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	(yyval.cmd)->cmd = parser_warmup;
	(yyval.cmd)->cmd_qty = (yyvsp[0].ival);
}
#line 3279 "parser_gram.c"
    break;

  case 123:
#line 1180 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	(yyval.cmd)->cmd = parser_warmup_variable;
	(yyval.cmd)->cmd_tgt1 = fb_stralloc((yyvsp[0].sval));
}
#line 3290 "parser_gram.c"
    break;

  case 124:
#line 1188 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	(yyval.cmd)->cmd = parser_sleep;
	(yyval.cmd)->cmd_qty = (yyvsp[0].ival);
}
#line 3301 "parser_gram.c"
    break;

  case 125:
#line 1195 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	(yyval.cmd)->cmd = parser_sleep_variable;
	(yyval.cmd)->cmd_tgt1 = fb_stralloc((yyvsp[0].sval));
}
#line 3312 "parser_gram.c"
    break;

  case 126:
#line 1203 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	(yyval.cmd)->cmd = parser_run;
	(yyval.cmd)->cmd_qty = (yyvsp[0].ival);
}
#line 3323 "parser_gram.c"
    break;

  case 127:
#line 1210 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	(yyval.cmd)->cmd = parser_run_variable;
	(yyval.cmd)->cmd_tgt1 = fb_stralloc((yyvsp[0].sval));
}
#line 3334 "parser_gram.c"
    break;

  case 128:
#line 1217 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	(yyval.cmd)->cmd = parser_run;
	(yyval.cmd)->cmd_qty = 60UL;
}
#line 3345 "parser_gram.c"
    break;

  case 129:
#line 1225 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	(yyval.cmd)->cmd = parser_help;
}
#line 3355 "parser_gram.c"
    break;

  case 130:
#line 1232 "parser_gram.y"
{
	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;
	(yyval.cmd)->cmd_name = fb_stralloc((yyvsp[0].sval));
}
#line 3365 "parser_gram.c"
    break;

  case 131:
#line 1238 "parser_gram.y"
{
	(yyvsp[-1].cmd)->cmd_attr_list = (yyvsp[0].attr);
}
#line 3373 "parser_gram.c"
    break;

  case 132:
#line 1243 "parser_gram.y"
{
	FILE *newfile;
	char loadfile[128];

	if (((yyval.cmd) = alloc_cmd()) == NULL)
		YYERROR;

	(void) strcpy(loadfile, (yyvsp[0].sval));
	(void) strcat(loadfile, ".f");

	if ((newfile = fopen(loadfile, "r")) == NULL) {
		(void) strcpy(loadfile, fbbasepath);
		(void) strcat(loadfile, "/workloads/");
		(void) strcat(loadfile, (yyvsp[0].sval));
		(void) strcat(loadfile, ".f");
		if ((newfile = fopen(loadfile, "r")) == NULL) {
			filebench_log(LOG_ERROR, "Cannot open %s", loadfile);
			YYERROR;
		}
	}

	parentscript = yyin;
	yyin = newfile;
	yy_switchfileparent(yyin);
}
#line 3403 "parser_gram.c"
    break;

  case 133:
#line 1270 "parser_gram.y"
                 {(yyval.ival) = FSE_PROC;}
#line 3409 "parser_gram.c"
    break;

  case 134:
#line 1271 "parser_gram.y"
             {(yyval.ival) = FSE_THREAD;}
#line 3415 "parser_gram.c"
    break;

  case 135:
#line 1272 "parser_gram.y"
              {(yyval.ival) = FSE_FILESET;}
#line 3421 "parser_gram.c"
    break;

  case 136:
#line 1273 "parser_gram.y"
           {(yyval.ival) = FSE_FILE;}
#line 3427 "parser_gram.c"
    break;

  case 138:
#line 1283 "parser_gram.y"
{
	(yyval.attr) = (yyvsp[0].attr);
}
#line 3435 "parser_gram.c"
    break;

  case 139:
#line 1287 "parser_gram.y"
{
	attr_t *attr = NULL;
	attr_t *list_end = NULL;

	for (attr = (yyvsp[-2].attr); attr != NULL;
	    attr = attr->attr_next)
		list_end = attr; /* Find end of list */

	list_end->attr_next = (yyvsp[0].attr);

	(yyval.attr) = (yyvsp[-2].attr);
}
#line 3452 "parser_gram.c"
    break;

  case 140:
#line 1301 "parser_gram.y"
{
	(yyval.attr) = (yyvsp[0].attr);
}
#line 3460 "parser_gram.c"
    break;

  case 141:
#line 1305 "parser_gram.y"
{
	attr_t *attr = NULL;
	attr_t *list_end = NULL;

	for (attr = (yyvsp[-2].attr); attr != NULL;
	    attr = attr->attr_next)
		list_end = attr; /* Find end of list */

	list_end->attr_next = (yyvsp[0].attr);

	(yyval.attr) = (yyvsp[-2].attr);
}
#line 3477 "parser_gram.c"
    break;

  case 142:
#line 1319 "parser_gram.y"
{
	(yyval.attr) = (yyvsp[0].attr);
	(yyval.attr)->attr_name = (yyvsp[-2].ival);
}
#line 3486 "parser_gram.c"
    break;

  case 143:
#line 1324 "parser_gram.y"
{
	if (((yyval.attr) = alloc_attr()) == NULL)
		YYERROR;
	(yyval.attr)->attr_name = (yyvsp[0].ival);
}
#line 3496 "parser_gram.c"
    break;

  case 144:
#line 1331 "parser_gram.y"
{
	(yyval.attr) = (yyvsp[0].attr);
	(yyval.attr)->attr_name = (yyvsp[-2].ival);
}
#line 3505 "parser_gram.c"
    break;

  case 145:
#line 1336 "parser_gram.y"
{
	if (((yyval.attr) = alloc_attr()) == NULL)
		YYERROR;
	(yyval.attr)->attr_name = (yyvsp[0].ival);
}
#line 3515 "parser_gram.c"
    break;

  case 146:
#line 1344 "parser_gram.y"
{
	(yyval.attr) = (yyvsp[0].attr);
}
#line 3523 "parser_gram.c"
    break;

  case 147:
#line 1348 "parser_gram.y"
{
	attr_t *attr = NULL;
	attr_t *list_end = NULL;

	for (attr = (yyvsp[-2].attr); attr != NULL;
	    attr = attr->attr_next)
		list_end = attr; /* Find end of list */

	list_end->attr_next = (yyvsp[0].attr);

	(yyval.attr) = (yyvsp[-2].attr);
}
#line 3540 "parser_gram.c"
    break;

  case 148:
#line 1361 "parser_gram.y"
{
	attr_t *attr = NULL;
	attr_t *list_end = NULL;

	for (attr = (yyvsp[-6].attr); attr != NULL;
	    attr = attr->attr_next)
		list_end = attr; /* Find end of list */

	
	if ((attr = alloc_attr()) == NULL)
		YYERROR;

	attr->attr_name = FSA_RANDTABLE;
	attr->attr_obj = (void *)(yyvsp[-1].rndtb);
	list_end->attr_next = attr;
	(yyval.attr) = (yyvsp[-6].attr);
}
#line 3562 "parser_gram.c"
    break;

  case 149:
#line 1380 "parser_gram.y"
{
	(yyval.attr) = (yyvsp[0].attr);
	(yyval.attr)->attr_name = (yyvsp[-2].ival);
}
#line 3571 "parser_gram.c"
    break;

  case 150:
#line 1385 "parser_gram.y"
{
	if (((yyval.attr) = alloc_attr()) == NULL)
		YYERROR;
	(yyval.attr)->attr_name = (yyvsp[0].ival);
}
#line 3581 "parser_gram.c"
    break;

  case 151:
#line 1391 "parser_gram.y"
{
	(yyval.attr) = (yyvsp[0].attr);
	(yyval.attr)->attr_name = FSA_TYPE;
}
#line 3590 "parser_gram.c"
    break;

  case 152:
#line 1396 "parser_gram.y"
{
	(yyval.attr) = (yyvsp[0].attr);
	(yyval.attr)->attr_name = FSA_RANDSRC;
}
#line 3599 "parser_gram.c"
    break;

  case 153:
#line 1402 "parser_gram.y"
{
	if (((yyval.rndtb) = alloc_probtabent()) == NULL)
		YYERROR;
	(yyval.rndtb)->pte_percent = (yyvsp[-5].avd);
	(yyval.rndtb)->pte_segmin  = (yyvsp[-3].avd);
	(yyval.rndtb)->pte_segmax  = (yyvsp[-1].avd);
}
#line 3611 "parser_gram.c"
    break;

  case 154:
#line 1412 "parser_gram.y"
{
	(yyval.rndtb) = (yyvsp[0].rndtb);
}
#line 3619 "parser_gram.c"
    break;

  case 155:
#line 1416 "parser_gram.y"
{
	probtabent_t *pte = NULL;
	probtabent_t *ptelist_end = NULL;

	for (pte = (yyvsp[-2].rndtb); pte != NULL;
	    pte = pte->pte_next)
		ptelist_end = pte; /* Find end of prob table entry list */

	ptelist_end->pte_next = (yyvsp[0].rndtb);

	(yyval.rndtb) = (yyvsp[-2].rndtb);
}
#line 3636 "parser_gram.c"
    break;

  case 156:
#line 1431 "parser_gram.y"
{
	(yyval.attr) = (yyvsp[0].attr);
}
#line 3644 "parser_gram.c"
    break;

  case 157:
#line 1435 "parser_gram.y"
{
	attr_t *attr = NULL;
	attr_t *list_end = NULL;

	for (attr = (yyvsp[-2].attr); attr != NULL;
	    attr = attr->attr_next)
		list_end = attr; /* Find end of list */

	list_end->attr_next = (yyvsp[0].attr);

	(yyval.attr) = (yyvsp[-2].attr);
}
#line 3661 "parser_gram.c"
    break;

  case 158:
#line 1449 "parser_gram.y"
{
	(yyval.attr) = (yyvsp[0].attr);
	(yyval.attr)->attr_name = (yyvsp[-2].ival);
}
#line 3670 "parser_gram.c"
    break;

  case 159:
#line 1454 "parser_gram.y"
{
	if (((yyval.attr) = alloc_attr()) == NULL)
		YYERROR;
	(yyval.attr)->attr_name = (yyvsp[0].ival);
}
#line 3680 "parser_gram.c"
    break;

  case 160:
#line 1462 "parser_gram.y"
{
	(yyval.attr) = (yyvsp[0].attr);
}
#line 3688 "parser_gram.c"
    break;

  case 161:
#line 1466 "parser_gram.y"
{
	attr_t *attr = NULL;
	attr_t *list_end = NULL;

	for (attr = (yyvsp[-2].attr); attr != NULL;
	    attr = attr->attr_next)
		list_end = attr; /* Find end of list */

	list_end->attr_next = (yyvsp[0].attr);

	(yyval.attr) = (yyvsp[-2].attr);
}
#line 3705 "parser_gram.c"
    break;

  case 162:
#line 1479 "parser_gram.y"
{
	attr_t *attr = NULL;
	attr_t *list_end = NULL;

	for (attr = (yyvsp[-2].attr); attr != NULL;
	    attr = attr->attr_next)
		list_end = attr; /* Find end of list */

	list_end->attr_next = (yyvsp[0].attr);

	(yyval.attr) = (yyvsp[-2].attr);
}
#line 3722 "parser_gram.c"
    break;

  case 163:
#line 1493 "parser_gram.y"
{
	(yyval.attr) = (yyvsp[0].attr);
	(yyval.attr)->attr_name = (yyvsp[-2].ival);
}
#line 3731 "parser_gram.c"
    break;

  case 164:
#line 1498 "parser_gram.y"
{
	if (((yyval.attr) = alloc_attr()) == NULL)
		YYERROR;
	(yyval.attr)->attr_name = (yyvsp[0].ival);
}
#line 3741 "parser_gram.c"
    break;

  case 165:
#line 1506 "parser_gram.y"
{
	(yyval.attr) = (yyvsp[0].attr);
}
#line 3749 "parser_gram.c"
    break;

  case 166:
#line 1510 "parser_gram.y"
{
	attr_t *attr = NULL;
	attr_t *list_end = NULL;

	for (attr = (yyvsp[-2].attr); attr != NULL;
	    attr = attr->attr_next)
		list_end = attr; /* Find end of list */

	list_end->attr_next = (yyvsp[0].attr);

	(yyval.attr) = (yyvsp[-2].attr);
}
#line 3766 "parser_gram.c"
    break;

  case 167:
#line 1524 "parser_gram.y"
{
	(yyval.attr) = (yyvsp[0].attr);
	(yyval.attr)->attr_name = (yyvsp[-2].ival);
}
#line 3775 "parser_gram.c"
    break;

  case 168:
#line 1529 "parser_gram.y"
{
	if (((yyval.attr) = alloc_attr()) == NULL)
		YYERROR;
	(yyval.attr)->attr_name = (yyvsp[0].ival);
}
#line 3785 "parser_gram.c"
    break;

  case 169:
#line 1537 "parser_gram.y"
{
	(yyval.attr) = (yyvsp[0].attr);
}
#line 3793 "parser_gram.c"
    break;

  case 170:
#line 1541 "parser_gram.y"
{
	attr_t *attr = NULL;
	attr_t *list_end = NULL;

	for (attr = (yyvsp[-2].attr); attr != NULL;
	    attr = attr->attr_next)
		list_end = attr; /* Find end of list */

	list_end->attr_next = (yyvsp[0].attr);

	(yyval.attr) = (yyvsp[-2].attr);
}
#line 3810 "parser_gram.c"
    break;

  case 171:
#line 1555 "parser_gram.y"
{
	(yyval.attr) = (yyvsp[0].attr);
	(yyval.attr)->attr_name = (yyvsp[-2].ival);
}
#line 3819 "parser_gram.c"
    break;

  case 172:
#line 1561 "parser_gram.y"
{
	(yyval.attr) = (yyvsp[0].attr);
	(yyval.attr)->attr_name = FSA_VALUE;
}
#line 3828 "parser_gram.c"
    break;

  case 173:
#line 1567 "parser_gram.y"
{
	if (((yyval.attr) = alloc_attr()) == NULL)
		YYERROR;
	(yyval.attr)->attr_avd = avd_str_alloc((yyvsp[0].sval));
	(yyval.attr)->attr_name = (yyvsp[-2].ival);
}
#line 3839 "parser_gram.c"
    break;

  case 174:
#line 1575 "parser_gram.y"
            {(yyval.ival) = FSK_PLUS;}
#line 3845 "parser_gram.c"
    break;

  case 175:
#line 1576 "parser_gram.y"
             {(yyval.ival) = FSK_MINUS;}
#line 3851 "parser_gram.c"
    break;

  case 176:
#line 1577 "parser_gram.y"
                {(yyval.ival) = FSK_MULTIPLY;}
#line 3857 "parser_gram.c"
    break;

  case 177:
#line 1578 "parser_gram.y"
              {(yyval.ival) = FSK_DIVIDE;}
#line 3863 "parser_gram.c"
    break;

  case 185:
#line 1593 "parser_gram.y"
           { (yyval.ival) = FSA_NICE;}
#line 3869 "parser_gram.c"
    break;

  case 186:
#line 1594 "parser_gram.y"
           { (yyval.ival) = FSA_NAME;}
#line 3875 "parser_gram.c"
    break;

  case 187:
#line 1595 "parser_gram.y"
                { (yyval.ival) = FSA_INSTANCES;}
#line 3881 "parser_gram.c"
    break;

  case 188:
#line 1598 "parser_gram.y"
           { (yyval.ival) = FSA_SIZE;}
#line 3887 "parser_gram.c"
    break;

  case 189:
#line 1599 "parser_gram.y"
           { (yyval.ival) = FSA_NAME;}
#line 3893 "parser_gram.c"
    break;

  case 190:
#line 1600 "parser_gram.y"
           { (yyval.ival) = FSA_PATH;}
#line 3899 "parser_gram.c"
    break;

  case 191:
#line 1601 "parser_gram.y"
               { (yyval.ival) = FSA_READONLY;}
#line 3905 "parser_gram.c"
    break;

  case 192:
#line 1602 "parser_gram.y"
                { (yyval.ival) = FSA_TRUSTTREE;}
#line 3911 "parser_gram.c"
    break;

  case 193:
#line 1603 "parser_gram.y"
            { (yyval.ival) = FSA_REUSE;}
#line 3917 "parser_gram.c"
    break;

  case 194:
#line 1604 "parser_gram.y"
               { (yyval.ival) = FSA_PREALLOC;}
#line 3923 "parser_gram.c"
    break;

  case 195:
#line 1605 "parser_gram.y"
               { (yyval.ival) = FSA_PARALLOC;}
#line 3929 "parser_gram.c"
    break;

  case 196:
#line 1606 "parser_gram.y"
                { (yyval.ival) = FSA_WRITEONLY;}
#line 3935 "parser_gram.c"
    break;

  case 197:
#line 1609 "parser_gram.y"
           { (yyval.ival) = FSA_SIZE;}
#line 3941 "parser_gram.c"
    break;

  case 198:
#line 1610 "parser_gram.y"
           { (yyval.ival) = FSA_NAME;}
#line 3947 "parser_gram.c"
    break;

  case 199:
#line 1611 "parser_gram.y"
           { (yyval.ival) = FSA_PATH;}
#line 3953 "parser_gram.c"
    break;

  case 200:
#line 1612 "parser_gram.y"
               { (yyval.ival) = FSA_DIRWIDTH;}
#line 3959 "parser_gram.c"
    break;

  case 201:
#line 1613 "parser_gram.y"
                 { (yyval.ival) = FSA_DIRDEPTHRV;}
#line 3965 "parser_gram.c"
    break;

  case 202:
#line 1614 "parser_gram.y"
               { (yyval.ival) = FSA_PREALLOC;}
#line 3971 "parser_gram.c"
    break;

  case 203:
#line 1615 "parser_gram.y"
               { (yyval.ival) = FSA_PARALLOC;}
#line 3977 "parser_gram.c"
    break;

  case 204:
#line 1616 "parser_gram.y"
            { (yyval.ival) = FSA_REUSE;}
#line 3983 "parser_gram.c"
    break;

  case 205:
#line 1617 "parser_gram.y"
               { (yyval.ival) = FSA_READONLY;}
#line 3989 "parser_gram.c"
    break;

  case 206:
#line 1618 "parser_gram.y"
                { (yyval.ival) = FSA_TRUSTTREE;}
#line 3995 "parser_gram.c"
    break;

  case 207:
#line 1619 "parser_gram.y"
                    { (yyval.ival) = FSA_FILESIZEGAMMA;}
#line 4001 "parser_gram.c"
    break;

  case 208:
#line 1620 "parser_gram.y"
               { (yyval.ival) = FSA_DIRGAMMA;}
#line 4007 "parser_gram.c"
    break;

  case 209:
#line 1621 "parser_gram.y"
             { (yyval.ival) = FSA_CACHED;}
#line 4013 "parser_gram.c"
    break;

  case 210:
#line 1622 "parser_gram.y"
              { (yyval.ival) = FSA_ENTRIES;}
#line 4019 "parser_gram.c"
    break;

  case 211:
#line 1623 "parser_gram.y"
               { (yyval.ival) = FSA_LEAFDIRS;}
#line 4025 "parser_gram.c"
    break;

  case 212:
#line 1624 "parser_gram.y"
                { (yyval.ival) = FSA_WRITEONLY;}
#line 4031 "parser_gram.c"
    break;

  case 213:
#line 1627 "parser_gram.y"
           { (yyval.ival) = FSA_NAME;}
#line 4037 "parser_gram.c"
    break;

  case 214:
#line 1628 "parser_gram.y"
           { (yyval.ival) = FSA_TYPE;}
#line 4043 "parser_gram.c"
    break;

  case 215:
#line 1629 "parser_gram.y"
               { (yyval.ival) = FSA_RANDSEED;}
#line 4049 "parser_gram.c"
    break;

  case 216:
#line 1630 "parser_gram.y"
              { (yyval.ival) = FSA_ENTRIES;}
#line 4055 "parser_gram.c"
    break;

  case 217:
#line 1631 "parser_gram.y"
              { (yyval.ival) = FSA_RANDMAX;}
#line 4061 "parser_gram.c"
    break;

  case 218:
#line 1634 "parser_gram.y"
           { (yyval.ival) = FSA_NAME;}
#line 4067 "parser_gram.c"
    break;

  case 219:
#line 1635 "parser_gram.y"
               { (yyval.ival) = FSA_RANDSEED;}
#line 4073 "parser_gram.c"
    break;

  case 220:
#line 1636 "parser_gram.y"
                { (yyval.ival) = FSA_RANDGAMMA;}
#line 4079 "parser_gram.c"
    break;

  case 221:
#line 1637 "parser_gram.y"
               { (yyval.ival) = FSA_RANDMEAN;}
#line 4085 "parser_gram.c"
    break;

  case 222:
#line 1638 "parser_gram.y"
              { (yyval.ival) = FSA_RANDMIN;}
#line 4091 "parser_gram.c"
    break;

  case 223:
#line 1639 "parser_gram.y"
                { (yyval.ival) = FSA_RANDROUND;}
#line 4097 "parser_gram.c"
    break;

  case 224:
#line 1642 "parser_gram.y"
           { (yyval.ival) = FSS_TYPE;}
#line 4103 "parser_gram.c"
    break;

  case 225:
#line 1643 "parser_gram.y"
          { (yyval.ival) = FSS_SRC;}
#line 4109 "parser_gram.c"
    break;

  case 226:
#line 1644 "parser_gram.y"
           { (yyval.ival) = FSS_SEED;}
#line 4115 "parser_gram.c"
    break;

  case 227:
#line 1645 "parser_gram.y"
            { (yyval.ival) = FSS_GAMMA;}
#line 4121 "parser_gram.c"
    break;

  case 228:
#line 1646 "parser_gram.y"
           { (yyval.ival) = FSS_MEAN;}
#line 4127 "parser_gram.c"
    break;

  case 229:
#line 1647 "parser_gram.y"
          { (yyval.ival) = FSS_MIN;}
#line 4133 "parser_gram.c"
    break;

  case 230:
#line 1648 "parser_gram.y"
            { (yyval.ival) = FSS_ROUND;}
#line 4139 "parser_gram.c"
    break;

  case 231:
#line 1651 "parser_gram.y"
           { (yyval.ival) = FSS_SEED;}
#line 4145 "parser_gram.c"
    break;

  case 232:
#line 1652 "parser_gram.y"
            { (yyval.ival) = FSS_GAMMA;}
#line 4151 "parser_gram.c"
    break;

  case 233:
#line 1653 "parser_gram.y"
           { (yyval.ival) = FSS_MEAN;}
#line 4157 "parser_gram.c"
    break;

  case 234:
#line 1654 "parser_gram.y"
          { (yyval.ival) = FSS_MIN;}
#line 4163 "parser_gram.c"
    break;

  case 235:
#line 1655 "parser_gram.y"
            { (yyval.ival) = FSS_ROUND;}
#line 4169 "parser_gram.c"
    break;

  case 236:
#line 1658 "parser_gram.y"
{
	if (((yyval.attr) = alloc_attr()) == NULL)
		YYERROR;
	(yyval.attr)->attr_avd = avd_int_alloc((yyvsp[0].ival));
}
#line 4179 "parser_gram.c"
    break;

  case 237:
#line 1665 "parser_gram.y"
              { (yyval.ival) = FSV_RANDUNI;}
#line 4185 "parser_gram.c"
    break;

  case 238:
#line 1666 "parser_gram.y"
              { (yyval.ival) = FSV_RANDTAB;}
#line 4191 "parser_gram.c"
    break;

  case 239:
#line 1667 "parser_gram.y"
                { (yyval.ival) = FSA_RANDGAMMA;}
#line 4197 "parser_gram.c"
    break;

  case 240:
#line 1670 "parser_gram.y"
{
	if (((yyval.attr) = alloc_attr()) == NULL)
		YYERROR;
	(yyval.attr)->attr_avd = avd_int_alloc((yyvsp[0].ival));
}
#line 4207 "parser_gram.c"
    break;

  case 241:
#line 1677 "parser_gram.y"
            { (yyval.ival) = FSV_URAND;}
#line 4213 "parser_gram.c"
    break;

  case 242:
#line 1678 "parser_gram.y"
             { (yyval.ival) = FSV_RAND48;}
#line 4219 "parser_gram.c"
    break;

  case 243:
#line 1681 "parser_gram.y"
              { (yyval.ival) = FSA_PROCESS;}
#line 4225 "parser_gram.c"
    break;

  case 244:
#line 1682 "parser_gram.y"
           { (yyval.ival) = FSA_NAME;}
#line 4231 "parser_gram.c"
    break;

  case 245:
#line 1683 "parser_gram.y"
              { (yyval.ival) = FSA_MEMSIZE;}
#line 4237 "parser_gram.c"
    break;

  case 246:
#line 1684 "parser_gram.y"
             { (yyval.ival) = FSA_USEISM;}
#line 4243 "parser_gram.c"
    break;

  case 247:
#line 1685 "parser_gram.y"
                { (yyval.ival) = FSA_INSTANCES;}
#line 4249 "parser_gram.c"
    break;

  case 248:
#line 1686 "parser_gram.y"
             { (yyval.ival) = FSA_IOPRIO;}
#line 4255 "parser_gram.c"
    break;

  case 249:
#line 1689 "parser_gram.y"
          { (yyval.ival) = FSA_WSS;}
#line 4261 "parser_gram.c"
    break;

  case 250:
#line 1690 "parser_gram.y"
           { (yyval.ival) = FSA_FILE;}
#line 4267 "parser_gram.c"
    break;

  case 251:
#line 1691 "parser_gram.y"
             { (yyval.ival) = FSA_POSSET;}
#line 4273 "parser_gram.c"
    break;

  case 252:
#line 1692 "parser_gram.y"
           { (yyval.ival) = FSA_NAME;}
#line 4279 "parser_gram.c"
    break;

  case 253:
#line 1693 "parser_gram.y"
             { (yyval.ival) = FSA_RANDOM;}
#line 4285 "parser_gram.c"
    break;

  case 254:
#line 1694 "parser_gram.y"
         { (yyval.ival) = FSA_FD;}
#line 4291 "parser_gram.c"
    break;

  case 255:
#line 1695 "parser_gram.y"
            { (yyval.ival) = FSA_SRCFD;}
#line 4297 "parser_gram.c"
    break;

  case 256:
#line 1696 "parser_gram.y"
               { (yyval.ival) = FSA_ROTATEFD;}
#line 4303 "parser_gram.c"
    break;

  case 257:
#line 1697 "parser_gram.y"
            { (yyval.ival) = FSA_DSYNC;}
#line 4309 "parser_gram.c"
    break;

  case 258:
#line 1698 "parser_gram.y"
               { (yyval.ival) = FSA_DIRECTIO;}
#line 4315 "parser_gram.c"
    break;

  case 259:
#line 1699 "parser_gram.y"
              { (yyval.ival) = FSA_INDEXED;}
#line 4321 "parser_gram.c"
    break;

  case 260:
#line 1700 "parser_gram.y"
             { (yyval.ival) = FSA_TARGET;}
#line 4327 "parser_gram.c"
    break;

  case 261:
#line 1701 "parser_gram.y"
            { (yyval.ival) = FSA_ITERS;}
#line 4333 "parser_gram.c"
    break;

  case 262:
#line 1702 "parser_gram.y"
            { (yyval.ival) = FSA_VALUE;}
#line 4339 "parser_gram.c"
    break;

  case 263:
#line 1703 "parser_gram.y"
               { (yyval.ival) = FSA_BLOCKING;}
#line 4345 "parser_gram.c"
    break;

  case 264:
#line 1704 "parser_gram.y"
                { (yyval.ival) = FSA_HIGHWATER;}
#line 4351 "parser_gram.c"
    break;

  case 265:
#line 1705 "parser_gram.y"
             { (yyval.ival) = FSA_IOSIZE;}
#line 4357 "parser_gram.c"
    break;

  case 266:
#line 1706 "parser_gram.y"
                  { (yyval.ival) = FSA_NOREADAHEAD;}
#line 4363 "parser_gram.c"
    break;

  case 267:
#line 1709 "parser_gram.y"
           { (yyval.ival) = FSA_RATE;}
#line 4369 "parser_gram.c"
    break;

  case 268:
#line 1712 "parser_gram.y"
             { (yyval.ival) = FSA_MASTER;}
#line 4375 "parser_gram.c"
    break;

  case 269:
#line 1713 "parser_gram.y"
             { (yyval.ival) = FSA_CLIENT;}
#line 4381 "parser_gram.c"
    break;

  case 270:
#line 1716 "parser_gram.y"
           { (yyval.ival) = FSA_PATH;}
#line 4387 "parser_gram.c"
    break;

  case 271:
#line 1717 "parser_gram.y"
             { (yyval.ival) = FSA_FSTYPE;}
#line 4393 "parser_gram.c"
    break;

  case 272:
#line 1720 "parser_gram.y"
{
	(yyval.attr) = (yyvsp[0].attr);
}
#line 4401 "parser_gram.c"
    break;

  case 273:
#line 1724 "parser_gram.y"
{
	attr_t *attr = NULL;
	attr_t *list_end = NULL;

	for (attr = (yyvsp[-2].attr); attr != NULL;
	    attr = attr->attr_next)
		list_end = attr; /* Find end of list */

	list_end->attr_next = (yyvsp[0].attr);

	(yyval.attr) = (yyvsp[-2].attr);
}
#line 4418 "parser_gram.c"
    break;

  case 274:
#line 1737 "parser_gram.y"
{
	attr_t *attr = NULL;
	attr_t *list_end = NULL;

	for (attr = (yyvsp[-2].attr); attr != NULL;
	    attr = attr->attr_next)
		list_end = attr; /* Find end of list */

	list_end->attr_next = (yyvsp[0].attr);

	(yyval.attr) = (yyvsp[-2].attr);
}
#line 4435 "parser_gram.c"
    break;

  case 275:
#line 1751 "parser_gram.y"
{
	(yyval.attr) = (yyvsp[0].attr);
	(yyval.attr)->attr_name = (yyvsp[-2].ival);
}
#line 4444 "parser_gram.c"
    break;

  case 276:
#line 1757 "parser_gram.y"
{
	if (((yyval.attr) = alloc_lvar_attr(var_lvar_assign_boolean((yyvsp[-2].sval), (yyvsp[0].bval)))) == NULL)
		YYERROR;
}
#line 4453 "parser_gram.c"
    break;

  case 277:
#line 1762 "parser_gram.y"
{
	if (((yyval.attr) = alloc_lvar_attr(var_lvar_assign_integer((yyvsp[-2].sval), (yyvsp[0].ival)))) == NULL)
		YYERROR;
}
#line 4462 "parser_gram.c"
    break;

  case 278:
#line 1767 "parser_gram.y"
{
	if (((yyval.attr) = alloc_lvar_attr(var_lvar_assign_string((yyvsp[-4].sval), (yyvsp[-1].sval)))) == NULL)
		YYERROR;
}
#line 4471 "parser_gram.c"
    break;

  case 279:
#line 1772 "parser_gram.y"
{
	if (((yyval.attr) = alloc_lvar_attr(var_lvar_assign_string((yyvsp[-2].sval), (yyvsp[0].sval)))) == NULL)
		YYERROR;
}
#line 4480 "parser_gram.c"
    break;

  case 280:
#line 1777 "parser_gram.y"
{
	if (((yyval.attr) = alloc_lvar_attr(var_lvar_assign_var((yyvsp[-2].sval), (yyvsp[0].sval)))) == NULL)
		YYERROR;
}
#line 4489 "parser_gram.c"
    break;

  case 281:
#line 1782 "parser_gram.y"
{
	if (((yyval.attr) = alloc_lvar_attr(var_lvar_alloc_local((yyvsp[0].sval)))) == NULL)
		YYERROR;
}
#line 4498 "parser_gram.c"
    break;

  case 282:
#line 1789 "parser_gram.y"
           { (yyval.ival) = FSA_NAME;}
#line 4504 "parser_gram.c"
    break;

  case 283:
#line 1790 "parser_gram.y"
            { (yyval.ival) = FSA_ITERS;}
#line 4510 "parser_gram.c"
    break;

  case 284:
#line 1793 "parser_gram.y"
{
	if (((yyval.attr) = alloc_attr()) == NULL)
		YYERROR;
	(yyval.attr)->attr_avd = avd_str_alloc((yyvsp[0].sval));
}
#line 4520 "parser_gram.c"
    break;

  case 285:
#line 1797 "parser_gram.y"
                {
	if (((yyval.attr) = alloc_attr()) == NULL)
		YYERROR;
	(yyval.attr)->attr_avd = avd_int_alloc((yyvsp[0].ival));
}
#line 4530 "parser_gram.c"
    break;

  case 286:
#line 1801 "parser_gram.y"
                    {
	if (((yyval.attr) = alloc_attr()) == NULL)
		YYERROR;
	(yyval.attr)->attr_avd = avd_bool_alloc((yyvsp[0].bval));
}
#line 4540 "parser_gram.c"
    break;

  case 287:
#line 1805 "parser_gram.y"
                 {
	if (((yyval.attr) = alloc_attr()) == NULL)
		YYERROR;
	(yyval.attr)->attr_avd = var_ref_attr((yyvsp[0].sval));
}
#line 4550 "parser_gram.c"
    break;

  case 288:
#line 1811 "parser_gram.y"
                                 {
	if (((yyval.attr) = alloc_attr()) == NULL)
		YYERROR;
	(yyval.attr)->attr_param_list = (yyvsp[0].list);
}
#line 4560 "parser_gram.c"
    break;

  case 289:
#line 1815 "parser_gram.y"
               {
	if (((yyval.attr) = alloc_attr()) == NULL)
		YYERROR;
	(yyval.attr)->attr_avd = avd_str_alloc((yyvsp[0].sval));
}
#line 4570 "parser_gram.c"
    break;

  case 290:
#line 1819 "parser_gram.y"
                {
	if (((yyval.attr) = alloc_attr()) == NULL)
		YYERROR;
	(yyval.attr)->attr_avd = avd_int_alloc((yyvsp[0].ival));
}
#line 4580 "parser_gram.c"
    break;

  case 291:
#line 1823 "parser_gram.y"
                    {
	if (((yyval.attr) = alloc_attr()) == NULL)
		YYERROR;
	(yyval.attr)->attr_avd = avd_bool_alloc((yyvsp[0].bval));
}
#line 4590 "parser_gram.c"
    break;

  case 292:
#line 1827 "parser_gram.y"
                 {
	if (((yyval.attr) = alloc_attr()) == NULL)
		YYERROR;
	(yyval.attr)->attr_avd = var_ref_attr((yyvsp[0].sval));
}
#line 4600 "parser_gram.c"
    break;

  case 293:
#line 1834 "parser_gram.y"
{
	(yyval.avd) = avd_int_alloc((yyvsp[0].ival));
}
#line 4608 "parser_gram.c"
    break;

  case 294:
#line 1837 "parser_gram.y"
{
	(yyval.avd) = var_ref_attr((yyvsp[0].sval));
}
#line 4616 "parser_gram.c"
    break;


#line 4620 "parser_gram.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 1841 "parser_gram.y"


/*
 * The following C routines implement the various commands defined in the above
 * yacc parser code. The yacc portion checks the syntax of the commands found in
 * a workload file (or typed interactively) and parses the commands' parameters
 * into lists. The lists are then passed in a cmd_t struct for each command to
 * its related routine in the following section for actual execution.  This
 * section also includes a few utility routines and the main entry point for the
 * program.
 */

#define	USAGE1	\
"Usage:\n" \
"filebench: interprets WML script and generates apporpriate workload\n" \
"Options:\n" \
"   [-h] Display verbose help\n" \
"   [-f <filename>] use specified file as an input instead of stdin\n"

#define	PARSER_CMDS \
"create [files|filesets|processes]\n" \
"stats [clear|snap]\n" \
"stats command \"shell command $var1,$var2...\"\n" \
"stats directory <directory>\n" \
"sleep <sleep-value>\n" \
"quit\n\n" \
"Variables:\n" \
"set $var = value\n" \
"    $var   - regular variables\n" \
"    ${var} - internal special variables\n" \
"    $(var) - environment variables\n\n"

#define	PARSER_EXAMPLE \
"Example:\n\n" \
"#!" FILEBENCHDIR "/bin/filebench -f\n" \
"\n" \
"define file name=bigfile,path=bigfile,size=1g,prealloc,reuse\n" \
"define process name=randomizer\n" \
"{\n" \
"  thread random-thread procname=randomizer\n"	\
"  {\n" \
"    flowop read name=random-read,filename=bigfile,iosize=16k,random\n" \
"  }\n" \
"}\n" \
"create files\n" \
"create processes\n" \
"stats clear\n" \
"sleep 30\n" \
"stats snap\n"

/*
 * Displays brief or verbose help for Filebench.
 */
static void
usage(int help)
{
	if (help >= 1)
		(void) fprintf(stderr, USAGE1);

	if (help >= 2) {
		(void) fprintf(stderr,
		    "\n'f' language definition:\n\n");
		fileset_usage();
		procflow_usage();
		threadflow_usage();
		flowoplib_usage();
		eventgen_usage();
		(void) fprintf(stderr, PARSER_CMDS);
		(void) fprintf(stderr, PARSER_EXAMPLE);
	}

	exit(1);
}

/*
 * mmap() call with MAP_FIXED argument does not guarantee
 * that the allocated memory region is not overlapped with
 * the previously existant mappings. According to POSIX, old mappings
 * are just disregarded. There is no generic way to detect
 * overlap. If overlap occurs strange runtime errors might happen,
 * because we might overlap stack, libraries, anything else.
 *
 * Since we always fork+exec same binary (filebench), theoretically
 * all the mappings should be the same, so no overlap should happen.
 * However, if virtual address space randomization is enabled on the target
 * machine - overlap is very likely (especially if workload defines a lot of
 * processes).  We observed numerous segmentation faults on CentOS because of
 * that.
 *
 * The function below checks if virtual address space randomization is
 * enabled on Linux. In case it is enabled, we print a warning and continue
 * execution.
 */
static void
check_va_randomization()
{
	char buf[4];
	int val;
	int ret;
	int fd;

	fd = open("/proc/sys/kernel/randomize_va_space", O_RDONLY);
	if (fd == -1) {
		/*
		 * probably this file just doesn't exist, so we conclude that the
		 * system does not support virtual address space randomization
		 * and silently return.
		 */
		return;
	}

	ret = read(fd, buf, sizeof(buf));
	if (ret == -1) {
		filebench_log(LOG_FATAL, "Coud not read from "
			"/proc/sys/kernel/randomize_va_space file!");
		return;
	}

	sscanf(buf, "%d", &val);
	if (val != 0) {
		filebench_log(LOG_FATAL, "IMPORTANT: Virtual address space "
			"randomization is enabled on this machine!\n"
			"It is highly recommended to disable randomization "
			"to provide stable Filebench runs.\n"
			"Echo 0 to /proc/sys/kernel/randomize_va_space file "
			"to disable the randomization.");

	}
}

#ifdef HAVE_PROC_SYS_KERNEL_SHMMAX
/*
 * Increase the maximum shared memory segment size till some large value.  We do
 * not restore it to the old value when the Filebench run is over. If we could
 * not change the value - we continue execution.
 */
void
fb_set_shmmax(void)
{
	FILE *f;
	int ret;

	f = fopen("/proc/sys/kernel/shmmax", "r+");
	if (!f) {
		filebench_log(LOG_FATAL, "WARNING: Could not open "
				"/proc/sys/kernel/shmmax file!\n"
				"It means that you probably ran Filebench not "
				"as a root. Filebench will not increase shared\n"
				"region limits in this case, which can lead "
				"to the failures on certain workloads.");
		return; 
	}

	/* writing new value */
#define SOME_LARGE_SHMAX "268435456" /* 256 MB */
	ret = fwrite(SOME_LARGE_SHMAX, sizeof(SOME_LARGE_SHMAX), 1, f);
	if (ret != 1) 
		filebench_log(LOG_ERROR, "Coud not write to "
				"/proc/sys/kernel/shmmax file!");
#undef SOME_LARGE_SHMAX

	return;
}
#else /* HAVE_PROC_SYS_KERNEL_SHMMAX */
void
fb_set_shmmax(void)
{
	return;
}
#endif /* HAVE_PROC_SYS_KERNEL_SHMMAX */

#ifdef HAVE_SETRLIMIT
/*
 * Increase the limit of opened files.
 *
 * We first set the limit to the hardlimit reported by the kernel; this call
 * will always succeed.  Then we try to set the limit to some large number of
 * files (unfortunately we can't set this ulimit to infinity), this will only
 * succeed if the process is ran by root.  Therefore, we always set the maximum
 * possible value for the limit for this given process (well, only if hardlimit
 * is greater then the large number of files defined by us, it is not true).
 *
 * Increasing this limit is especially important when we use thread model,
 * because opened files are accounted per-process, not per-thread.
 */
void
fb_set_rlimit(void)
{
	struct rlimit rlp;

	(void)getrlimit(RLIMIT_NOFILE, &rlp);
	rlp.rlim_cur = rlp.rlim_max;
	(void)setrlimit(RLIMIT_NOFILE, &rlp);
#define SOME_LARGE_NUMBER_OF_FILES 50000
	rlp.rlim_cur = rlp.rlim_max = SOME_LARGE_NUMBER_OF_FILES;
#undef SOME_LARGE_NUMBER_OF_FILES
	(void)setrlimit(RLIMIT_NOFILE, &rlp);
	return;
}
#else /* HAVE_SETRLIMIT */
void
fb_set_rlimit(void)
{
	return;
}
#endif /* HAVE_SETRLIMIT */

/*
 * Entry point for Filebench. Processes command line arguments. The -f option
 * will read in a workload file (the full name and extension must must be
 * given). The -a, -s, -m and -i options are used by the worker process to
 * receive the name, the base address of shared memory, its path, and the
 * process' instance number, respectively. This information is supplied by the
 * master process when it execs worker processes. If the worker process
 * arguments are passed then main will call the procflow_exec() routine which
 * creates worker threadflows and flowops and executes the procflow's portion of
 * the workload model until completion. If worker process arguments are not
 * passed to the process, then it becomes the master process for a filebench
 * run. It initializes the various filebench components and either executes the
 * supplied workload file, or enters interactive mode.
 */
int
main(int argc, char *argv[])
{
	const char cmd_options[] = "m:s:a:i:hf:";
	int opt;
	char *procname = NULL;
	int instance;
	void *shmaddr;
	char *cwdret;
	char *fscriptname = NULL;
#ifdef HAVE_LIBTECLA
	char *line;
#else /* HAVE_LIBTECLA */
	char line[1024];
#endif

	 /* parsing the parameters */
	while ((opt = getopt(argc, argv, cmd_options)) > 0) {
		switch (opt) {
		/* public parameters */
		case 'h':
			usage(2);
			break;
		case 'f':
			if (!optarg)
				usage(1);

			yyin = fopen(optarg, "r");
			if (!yyin) {
				filebench_log(LOG_FATAL,
				    "Cannot open file %s!", optarg);
				exit(1);
			}

			dofile = DOFILE_TRUE;
			fscriptname = optarg;
			break;
		/* private parameters: when filebench calls itself */
		case 'a':
			if (!optarg)
				usage(1);

			procname = optarg;
			break;
		case 's':
			if (!optarg)
				usage(1);
			sscanf(optarg, "%p", &shmaddr);
			break;
		case 'm':
			if (!optarg)
				usage(1);
			sscanf(optarg, "%s", shmpath);
			break;
		case 'i':
			if (!optarg)
				usage(1);
			sscanf(optarg, "%d", &instance);
			break;
		case '?':
		default:
			usage(1);
			break;
		}
	}

	/*
	 * Init things common to all processes: master and workers
	 */
	my_pid = getpid();
	fb_set_rlimit();
	fb_urandom_init();
	clock_init();

	if (procname) {
		/* A child Filebench instance */
		if (ipc_attach(shmaddr) < 0) {
			filebench_log(LOG_FATAL, "Cannot attach shm for %s",
			    procname);
			exit(1);
		}

		/* get correct function pointer for each working process */
		filebench_plugin_funcvecinit();

		/* execute corresponding procflow */
		if (procflow_exec(procname, instance) < 0) {
			filebench_log(LOG_FATAL, "Cannot startup process %s",
			    procname);
			exit(1);
		}

		exit(0);
	}

	/*
	 * Master process
	 */
	printf("Filebench Version %s\n", FILEBENCH_VERSION);
	check_va_randomization();

	/* saving executable name to exec it later as worker processes */
	execname = argv[0];

	/* saving current working directory */
	cwdret = getcwd(cwd, MAXPATHLEN);
	if (cwdret != cwd) {
		filebench_log(LOG_FATAL, "Cannot save current "
					 "working directory!");
		exit(1);
	}

	fb_set_shmmax();

	ipc_init();

	if (fscriptname)
		(void)strcpy(filebench_shm->shm_fscriptname, fscriptname);

	flowop_init();
	stats_init();
	eventgen_init();

	signal(SIGINT, parser_abort);

	if (dofile == DOFILE_TRUE)
		yyparse();
	else {
#ifdef HAVE_LIBTECLA
		if ((gl = new_GetLine(MAX_LINE_LEN, MAX_CMD_HIST)) == NULL) {
			filebench_log(LOG_ERROR,
			    "Failed to create GetLine object");
			filebench_shutdown(1);
		}

		if (gl_customize_completion(gl, NULL, command_complete)) {
			filebench_log(LOG_ERROR,
			    "Failed to register auto-completion function");
			filebench_shutdown(1);
		}

		while ((line = gl_get_line(gl, FILEBENCH_PROMPT, NULL, -1))) {
			arg_parse(line);
			yyparse();
		}

		del_GetLine(gl);
#else
		while (!feof(stdin)) {
			printf(FILEBENCH_PROMPT);
			fflush(stdout);
			if (fgets(line, sizeof (line), stdin) == NULL) {
				if (errno == EINTR)
					continue;
				else
					break;
			}
			arg_parse(line);
			yyparse();
		}
		printf("\n");
#endif	/* HAVE_LIBTECLA */
	}

	parser_filebench_shutdown((cmd_t *)0);

	return 0;
}

/*
 * arg_parse() puts the parser into command parsing mode. Create a tmpfile
 * and instruct the parser to read instructions from this location by setting
 * yyin to the value returned by tmpfile. Write the command into the file.
 * Then seek back to to the start of the file so that the parser can read
 * the instructions.
 */
static void
arg_parse(const char *command)
{
	if ((yyin = tmpfile()) == NULL) {
		filebench_log(LOG_FATAL,
		    "Exiting: Cannot create tmpfile: %s", strerror(errno));
		exit(1);
	}

	if (fwrite(command, strlen(command), 1, yyin) != 1)
		filebench_log(LOG_FATAL,
		    "Cannot write tmpfile: %s", strerror(errno));

	if (fseek(yyin, 0, SEEK_SET) != 0)
		filebench_log(LOG_FATAL,
		    "Cannot seek tmpfile: %s", strerror(errno));
}

/*
 * Converts a list of var_strings or ordinary strings to a single ordinary
 * string. It returns a pointer to the string (in malloc'd memory) if found,
 * or NULL otherwise.
 */
char *
parser_list2string(list_t *list)
{
	list_t *l;
	char *string;
	char *tmp;
	if ((string = malloc(MAXPATHLEN)) == NULL) {
		filebench_log(LOG_ERROR, "Failed to allocate memory");
		return (NULL);
	}

	*string = 0;

	/*	printf("parser_list2string: called\n"); */
	/* Format args */
	for (l = list; l != NULL; l = l->list_next) {
		char *lstr = avd_get_str(l->list_string);

		filebench_log(LOG_DEBUG_SCRIPT,
		    "converting string '%s'", lstr);

		/* see if it is a random variable */
		if (l->list_integer) {
			fbint_t param_name;

			tmp = NULL;
			param_name = avd_get_int(l->list_integer);
			switch (param_name) {
			case FSS_TYPE:
				tmp = var_randvar_to_string(lstr,
				    RAND_PARAM_TYPE);
				break;

			case FSS_SRC:
				tmp = var_randvar_to_string(lstr,
				    RAND_PARAM_SRC);
				break;

			case FSS_SEED:
				tmp = var_randvar_to_string(lstr,
				    RAND_PARAM_SEED);
				break;

			case FSS_MIN:
				tmp = var_randvar_to_string(lstr,
				    RAND_PARAM_MIN);
				break;

			case FSS_MEAN:
				tmp = var_randvar_to_string(lstr,
				    RAND_PARAM_MEAN);
				break;

			case FSS_GAMMA:
				tmp = var_randvar_to_string(lstr,
				    RAND_PARAM_GAMMA);
				break;

			case FSS_ROUND:
				tmp = var_randvar_to_string(lstr,
				    RAND_PARAM_ROUND);
				break;
			}

			if (tmp) {
				(void) strcat(string, tmp);
				free(tmp);
			} else {
				(void) strcat(string, lstr);
			}
		} else {
			/* perhaps a normal variable? */
			if ((tmp = var_to_string(lstr)) != NULL) {
				(void) strcat(string, tmp);
				free(tmp);
			} else {
				(void) strcat(string, lstr);
			}
		}
	}
	return (string);
}

/*
 * If the list just contains a single string starting with '$', then find
 * or create the named var and return the var's var_string component.
 * Otherwise, convert the list to a string, and allocate a var_string
 * containing a copy of that string. On failure either returns NULL
 * or shuts down the run.
 */
avd_t
parser_list2varstring(list_t *list)
{
	char *lstr = avd_get_str(list->list_string);

	/*	printf("parser_list2varstring: Called\n"); */
	/* Special case - variable name */
	if ((list->list_next == NULL) && (*lstr == '$'))
		return (var_ref_attr(lstr));

	return (avd_str_alloc(parser_list2string(list)));
}

/*
 * Looks for the var named in list_string of the first element of the
 * supplied list. If found, returns the var_val portion of the var in
 * an attribute value descriptor. If the var is not found, cannot be
 * allocated, the supplied list is NULL, or the list_string filed is
 * empty, returns NULL.
 */
avd_t
parser_list2avd(list_t *list)
{
	avd_t avd;
	char *lstr;

	if (list && ((lstr = avd_get_str(list->list_string)) != NULL)) {
		avd = var_ref_attr(lstr);
		return (avd);
	}

	return (NULL);
}

/*
 * Sets the event generator rate from the attribute supplied with the
 * command. If the attribute doesn't exist the routine does nothing.
 */
static void
parser_eventgen(cmd_t *cmd)
{
	attr_t *attr;

	/* Get the rate from attribute */
	if ((attr = get_attr_integer(cmd, FSA_RATE))) {
		if (attr->attr_avd) {
			eventgen_setrate(attr->attr_avd);
		}
	}
}

/*
 * Assigns the designated integer variable successive values from the
 * supplied comma seperated integer list. After each successive integer
 * assignment, it executes the bracket enclosed list of commands. For
 * example, repeated runs of a workload with increasing io sizes can
 * be done using the following command line:
 * 	foreach $iosize in 2k, 4k, 8k {run 60}
 */
static void
parser_foreach_integer(cmd_t *cmd)
{
	list_t *list = cmd->cmd_param_list;
	cmd_t *inner_cmd;

	for (; list != NULL; list = list->list_next) {
		fbint_t list_int = avd_get_int(list->list_integer);

		var_assign_integer(cmd->cmd_tgt1, list_int);
		filebench_log(LOG_VERBOSE, "Iterating %s=%llu",
		    cmd->cmd_tgt1, (u_longlong_t)list_int);
		for (inner_cmd = cmd->cmd_list; inner_cmd != NULL;
		    inner_cmd = inner_cmd->cmd_next) {
			inner_cmd->cmd(inner_cmd);
		}
	}
}

/*
 * Similar to parser_foreach_integer(), except takes a list of strings after
 * the "in" token. For example, to run twice using a different directory,
 * perhaps using a different filesystem, the following command line
 * could be used:
 * 	foreach $dir in "/ufs_top/fbt", "/zfs_top/fbt" {run 60)
 */
static void
parser_foreach_string(cmd_t *cmd)
{
	list_t *list = cmd->cmd_param_list;

	for (; list != NULL; list = list->list_next) {
		cmd_t *inner_cmd;
		char *lstr = avd_get_str(list->list_string);
		var_assign_string(cmd->cmd_tgt1, lstr);
		filebench_log(LOG_VERBOSE, "Iterating %s=%s",
		    cmd->cmd_tgt1, lstr);
		for (inner_cmd = cmd->cmd_list; inner_cmd != NULL;
		    inner_cmd = inner_cmd->cmd_next) {
			inner_cmd->cmd(inner_cmd);
		}
	}
}

/*
 * Lists the fileset name, path name and average size for all defined
 * filesets.
 */
static void
parser_list(cmd_t *cmd)
{
	(void) fileset_iter(fileset_print);
}

/*
 * Lists the flowop name and instance number for all flowops.
 */
static void
parser_flowop_list(cmd_t *cmd)
{
	flowop_printall();
}

/*
 * Calls procflow_define() to allocate "instances" number of  procflow(s)
 * (processes) with the supplied name. The default number of instances is
 * one. An optional priority level attribute can be supplied and is stored in
 * pf_nice. Finally the routine loops through the list of inner commands, if
 * any, which are defines for threadflows, and passes them one at a time to
 * parser_thread_define() to allocate threadflow entities for the process(es).
 */
static void
parser_proc_define(cmd_t *cmd)
{
	procflow_t *procflow;
	char *name = NULL;
	attr_t *attr;
	avd_t var_instances;
	fbint_t instances;
	cmd_t *inner_cmd;

	/* Get the name of the process */
	if ((attr = get_attr(cmd, FSA_NAME))) {
		name = avd_get_str(attr->attr_avd);
	} else {
		filebench_log(LOG_ERROR,
		    "define proc: proc specifies no name");
		filebench_shutdown(1);
	}

	/* Get the memory size from attribute */
	if ((attr = get_attr_integer(cmd, FSA_INSTANCES))) {
		if (AVD_IS_RANDOM(attr->attr_avd)) {
			filebench_log(LOG_ERROR,
			    "proc_define: Instances attr cannot be random");
			filebench_shutdown(1);
		}
		var_instances = attr->attr_avd;
		instances = avd_get_int(var_instances);
		filebench_log(LOG_DEBUG_IMPL,
		    "Setting instances = %llu", (u_longlong_t)instances);
	} else {
		filebench_log(LOG_DEBUG_IMPL,
		    "Defaulting to instances = 1");
		var_instances = avd_int_alloc(1);
		instances = 1;
	}

	if ((procflow = procflow_define(name, NULL, var_instances)) == NULL) {
		filebench_log(LOG_ERROR,
		    "Failed to instantiate %d %s process(es)\n",
		    instances, name);
		filebench_shutdown(1);
	}

	/* Get the pri from attribute */
	if ((attr = get_attr_integer(cmd, FSA_NICE))) {
		if (AVD_IS_RANDOM(attr->attr_avd)) {
			filebench_log(LOG_ERROR,
			    "proc_define: priority cannot be random");
			filebench_shutdown(1);
		}
		filebench_log(LOG_DEBUG_IMPL, "Setting pri = %llu",
		    (u_longlong_t)avd_get_int(attr->attr_avd));
		procflow->pf_nice = attr->attr_avd;
	} else
		procflow->pf_nice = avd_int_alloc(0);


	/* Create the list of threads for this process  */
	for (inner_cmd = cmd->cmd_list; inner_cmd != NULL;
	    inner_cmd = inner_cmd->cmd_next) {
		parser_thread_define(inner_cmd, procflow, instances);
	}
}

/*
 * Calls threadflow_define() to allocate "instances" number of  threadflow(s)
 * (threads) with the supplied name. The default number of instances is
 * one. Two other optional attributes may be supplied, one to set the memory
 * size, stored in tf_memsize, and to select the use of Interprocess Shared
 * Memory, which sets the THREADFLOW_USEISM flag in tf_attrs. Finally
 * the routine loops through the list of inner commands, if any, which are
 * defines for flowops, and passes them one at a time to
 * parser_flowop_define() to allocate flowop entities for the threadflows.
 */
static void
parser_thread_define(cmd_t *cmd, procflow_t *procflow, int procinstances)
{
	threadflow_t *threadflow, template;
	attr_t *attr;
	avd_t instances;
	cmd_t *inner_cmd;
	char *name = NULL;

	memset(&template, 0, sizeof (threadflow_t));

	/* Get the name of the thread */
	if ((attr = get_attr(cmd, FSA_NAME))) {
		name = avd_get_str(attr->attr_avd);
	} else {
		filebench_log(LOG_ERROR,
		    "define thread: thread in process %s specifies no name",
		    procflow->pf_name);
		filebench_shutdown(1);
	}

	/* Get the number of instances from attribute */
	if ((attr = get_attr_integer(cmd, FSA_INSTANCES))) {
		if (AVD_IS_RANDOM(attr->attr_avd)) {
			filebench_log(LOG_ERROR,
			    "define thread: Instances attr cannot be random");
			filebench_shutdown(1);
		}
		filebench_log(LOG_DEBUG_IMPL,
		    "define thread: Setting instances = %llu",
		    (u_longlong_t)avd_get_int(attr->attr_avd));
		instances = attr->attr_avd;
	} else
		instances = avd_int_alloc(1);

	/* Get the memory size from attribute */
	if ((attr = get_attr_integer(cmd, FSA_MEMSIZE))) {
		if (AVD_IS_RANDOM(attr->attr_avd)) {
			filebench_log(LOG_ERROR,
			    "define thread: Memory size cannot be random");
			filebench_shutdown(1);
		}
		filebench_log(LOG_DEBUG_IMPL,
		    "define thread: Setting memsize = %llu",
		    (u_longlong_t)avd_get_int(attr->attr_avd));
		template.tf_memsize = attr->attr_avd;
	} else
		template.tf_memsize = avd_int_alloc(0);

	/* Get ioprio parameters from attribute */
	if ((attr = get_attr_integer(cmd, FSA_IOPRIO))) {
		if (AVD_IS_RANDOM(attr->attr_avd)) {
			filebench_log(LOG_ERROR,
			    "define thread: ioprio cannot be random");
			filebench_shutdown(1);
		}
		filebench_log(LOG_DEBUG_IMPL,
		    "define thread: Setting ioprio = %llu",
		    (u_longlong_t)avd_get_int(attr->attr_avd));
		template.tf_ioprio = attr->attr_avd;
	} else
		template.tf_ioprio = avd_int_alloc(8);

	if ((threadflow = threadflow_define(procflow, name,
	    &template, instances)) == NULL) {
		filebench_log(LOG_ERROR,
		    "define thread: Failed to instantiate thread\n");
		filebench_shutdown(1);
	}

	/* Use ISM Memory? */
	if ((attr = get_attr(cmd, FSA_USEISM))) {
		threadflow->tf_attrs |= THREADFLOW_USEISM;
	}

	/* Create the list of flowops */
	for (inner_cmd = cmd->cmd_list; inner_cmd != NULL;
	    inner_cmd = inner_cmd->cmd_next) {
		parser_flowop_define(inner_cmd, threadflow,
		    &threadflow->tf_thrd_fops, FLOW_MASTER);
	}
}

/*
 * Fills in the attributes for a newly allocated flowop
 */
static void
parser_flowop_get_attrs(cmd_t *cmd, flowop_t *flowop)
{
	attr_t *attr;

	/* Get the filename from attribute */
	if ((attr = get_attr(cmd, FSA_FILE))) {
		flowop->fo_filename = attr->attr_avd;
		if (flowop->fo_filename == NULL) {
			filebench_log(LOG_ERROR,
			    "define flowop: no filename specfied");
			filebench_shutdown(1);
		}
	} else {
		/* no filename attribute specified */
		flowop->fo_filename = NULL;
	}

	/* Get the possetname from attribute */
	if ((attr = get_attr(cmd, FSA_POSSET))) {
		flowop->fo_possetname = attr->attr_avd;
		if (flowop->fo_possetname == NULL) {
			filebench_log(LOG_ERROR,
			    "parser_flowop_get_attrs: no "
				"posset name specfied");
			filebench_shutdown(1);
		}
	} else {
		/* no possetname attribute specified */
		flowop->fo_possetname = NULL;
	}

	/* Get the iosize of the op */
	if ((attr = get_attr_integer(cmd, FSA_IOSIZE)))
		flowop->fo_iosize = attr->attr_avd;
	else
		flowop->fo_iosize = avd_int_alloc(0);

	/* Get the working set size of the op */
	if ((attr = get_attr_integer(cmd, FSA_WSS)))
		flowop->fo_wss = attr->attr_avd;
	else
		flowop->fo_wss = avd_int_alloc(0);

	/* Random I/O? */
	if ((attr = get_attr_bool(cmd, FSA_RANDOM)))
		flowop->fo_random = attr->attr_avd;
	else
		flowop->fo_random = avd_bool_alloc(FALSE);

	/* Sync I/O? */
	if ((attr = get_attr_bool(cmd, FSA_DSYNC)))
		flowop->fo_dsync = attr->attr_avd;
	else
		flowop->fo_dsync = avd_bool_alloc(FALSE);

	/* Target, for wakeup etc */
	if ((attr = get_attr(cmd, FSA_TARGET)))
		(void) strcpy(flowop->fo_targetname,
		    avd_get_str(attr->attr_avd));

	/* Value */
	if ((attr = get_attr_integer(cmd, FSA_VALUE)))
		flowop->fo_value = attr->attr_avd;
	else
		flowop->fo_value = avd_int_alloc(0);

	/* FD */
	if ((attr = get_attr_integer(cmd, FSA_FD))) {
		flowop->fo_fdnumber = avd_get_int(attr->attr_avd);
		if (flowop->fo_filename != NULL)
			filebench_log(LOG_DEBUG_SCRIPT, "It is not "
			    "advisable to supply both an fd number "
			    "and a fileset name in most cases");
	}

	/* Rotatefd? */
	if ((attr = get_attr_bool(cmd, FSA_ROTATEFD)))
		flowop->fo_rotatefd = attr->attr_avd;
	else
		flowop->fo_rotatefd = avd_bool_alloc(FALSE);

	/* SRC FD, for copies etc... */
	if ((attr = get_attr_integer(cmd, FSA_SRCFD)))
		flowop->fo_srcfdnumber = avd_get_int(attr->attr_avd);

	/* Blocking operation? */
	if ((attr = get_attr_bool(cmd, FSA_BLOCKING)))
		flowop->fo_blocking = attr->attr_avd;
	else
		flowop->fo_blocking = avd_bool_alloc(FALSE);

	/* Direct I/O Operation */
	if ((attr = get_attr_bool(cmd, FSA_DIRECTIO)))
		flowop->fo_directio = attr->attr_avd;
	else
		flowop->fo_directio = avd_bool_alloc(FALSE);

	/* Highwater mark */
	if ((attr = get_attr_integer(cmd, FSA_HIGHWATER))) {
		flowop->fo_highwater = attr->attr_avd;
		if (AVD_IS_RANDOM(attr->attr_avd)) {
			filebench_log(LOG_ERROR,
			    "define flowop: Highwater attr cannot be random");
			filebench_shutdown(1);
		}
	} else {
		flowop->fo_highwater = avd_int_alloc(1);
	}

	/* find file or leaf directory by index number */
	if ((attr = get_attr_integer(cmd, FSA_INDEXED)))
		flowop->fo_fileindex = attr->attr_avd;
	else
		flowop->fo_fileindex = NULL;

	/* Read Ahead Diable */
	if ((attr = get_attr_bool(cmd, FSA_NOREADAHEAD)))
		flowop->fo_noreadahead = attr->attr_avd;
	else
		flowop->fo_noreadahead = avd_bool_alloc(FALSE);


}

/*
 * defines the FLOW_MASTER flowops within a FLOW_MASTER instance of
 * a composit flowop. Default attributes from the FLOW_INNER_DEF instances
 * of the composit flowop's inner flowops are used if set. Otherwise
 * default attributes from the FLOW_MASTER instance of the composit flowop
 * are used, which may include defaults from the original FLOW_DEFINITION
 * of the composit flowop.
 */
static void
parser_inner_flowop_define(threadflow_t *thread, flowop_t *comp0_flow,
			   flowop_t *comp_mstr_flow)
{
	flowop_t *inner_flowtype, *inner_flowop;

	/* follow flowop list, creating composit names */
	inner_flowtype = comp0_flow->fo_comp_fops;
	comp_mstr_flow->fo_comp_fops = NULL;

	while (inner_flowtype) {
		char fullname[MAXPATHLEN];

		/* create composite_name.name for new flowop */
		snprintf(fullname, MAXPATHLEN, "%s.%s",
		    comp_mstr_flow->fo_name, inner_flowtype->fo_name);

		if ((inner_flowop = flowop_define(thread, fullname,
		    inner_flowtype, &comp_mstr_flow->fo_comp_fops,
		    FLOW_MASTER, 0)) == NULL) {
			filebench_log(LOG_ERROR,
			    "define flowop: Failed to instantiate flowop %s\n",
			    fullname);
			filebench_shutdown(1);
		}

		/* if applicable, update filename attribute */
		if (inner_flowop->fo_filename) {
			char *name;

			/* fix up avd_t */
			avd_update(&inner_flowop->fo_filename,
			    comp_mstr_flow->fo_lvar_list);

			/* see if ready to get the file or fileset */
			name = avd_get_str(inner_flowop->fo_filename);
			if (name) {

				inner_flowop->fo_fileset = fileset_find(name);

				if (inner_flowop->fo_fileset == NULL) {
					filebench_log(LOG_ERROR,
					    "inr flowop %s: file %s not found",
					    inner_flowop->fo_name, name);
					filebench_shutdown(1);
				}
			}
		}

		/* update attributes from local variables */
		avd_update(&inner_flowop->fo_iters,
		    comp_mstr_flow->fo_lvar_list);

		/* if the inner flowop is a composit flowop, recurse */
		if (inner_flowtype->fo_type == FLOW_TYPE_COMPOSITE) {
			var_t *newlvar, *proto_lvars, *lvar_ptr;

			proto_lvars = inner_flowop->fo_lvar_list;
			inner_flowop->fo_lvar_list = 0;

			for (lvar_ptr = inner_flowtype->fo_lvar_list; lvar_ptr;
			    lvar_ptr = lvar_ptr->var_next) {

				if ((newlvar = var_lvar_alloc_local(
				    lvar_ptr->var_name)) != NULL) {

					add_lvar_to_list(newlvar,
					    &inner_flowop->fo_lvar_list);

					var_update_comp_lvars(newlvar,
					    proto_lvars,
					    comp_mstr_flow->fo_lvar_list);
				}
			}
		  
			parser_inner_flowop_define(thread,
			    inner_flowtype,
			    inner_flowop);

			inner_flowtype = inner_flowtype->fo_exec_next;
			continue;
		}

		avd_update(&inner_flowop->fo_iosize,
		    comp_mstr_flow->fo_lvar_list);
		avd_update(&inner_flowop->fo_wss,
		    comp_mstr_flow->fo_lvar_list);
		avd_update(&inner_flowop->fo_iters,
		    comp_mstr_flow->fo_lvar_list);
		avd_update(&inner_flowop->fo_value,
		    comp_mstr_flow->fo_lvar_list);
		avd_update(&inner_flowop->fo_random,
		    comp_mstr_flow->fo_lvar_list);
		avd_update(&inner_flowop->fo_dsync,
		    comp_mstr_flow->fo_lvar_list);
		avd_update(&inner_flowop->fo_rotatefd,
		    comp_mstr_flow->fo_lvar_list);
		avd_update(&inner_flowop->fo_blocking,
		    comp_mstr_flow->fo_lvar_list);
		avd_update(&inner_flowop->fo_directio,
		    comp_mstr_flow->fo_lvar_list);
		avd_update(&inner_flowop->fo_highwater,
		    comp_mstr_flow->fo_lvar_list);

		inner_flowtype = inner_flowtype->fo_exec_next;
	}
}

/*
 * Calls flowop_define() to allocate a flowop with the supplied name.
 * The allocated flowop inherits attributes from a base flowop of the
 * same type.  If the new flowop has a file or fileset attribute specified,
 * it must specify a defined fileobj or fileset or an error will be logged.
 * The new flowop may  also have the following attributes set by
 * the program:
 *  - file size (fo_iosize)
 *  - working set size (fo_wss)
 *  - do random io (fo_random)
 *  - do synchronous io (fo_dsync)
 *  - perform each operation multiple times before advancing (fo_iter)
 *  - target name (fo_targetname)
 *  - An integer value (fo_value)
 *  - a file descriptor (fo_fd)
 *  - specify to rotate file descriptors (fo_rotatefd)
 *  - a source fd (fo_srcfdnumber)
 *  - specify a blocking operation (fo_blocking)
 *  - specify a highwater mark (fo_highwater)
 *
 * After all the supplied attributes are stored in their respective locations
 * in the flowop object, the flowop's init function is called. No errors are
 * returned, but the filebench run will be terminated if the flowtype is not
 * specified, a name for the new flowop is not supplied, the flowop_define
 * call fails, or a file or fileset name is supplied but the corresponding
 * fileobj or fileset cannot be located.
 */
static void
parser_flowop_define(cmd_t *cmd, threadflow_t *thread,
    flowop_t **flowoplist_hdp, int category)
{
	flowop_t *flowop, *flowop_type;
	char *type = (char *)cmd->cmd_name;
	char *name = NULL;
	attr_t *attr;

	/* Get the inherited flowop */
	flowop_type = flowop_find(type);
	if (flowop_type == NULL) {
		filebench_log(LOG_ERROR,
		    "define flowop: flowop type %s not found",
		    type);
		filebench_shutdown(1);
	}

	/* Get the name of the flowop */
	if ((attr = get_attr(cmd, FSA_NAME))) {
		name = avd_get_str(attr->attr_avd);
	} else {
		filebench_log(LOG_ERROR,
		    "define flowop: flowop %s specifies no name",
		    flowop_type->fo_name);
		filebench_shutdown(1);
	}

	if ((flowop = flowop_define(thread, name,
	    flowop_type, flowoplist_hdp, category, 0)) == NULL) {
		filebench_log(LOG_ERROR,
		    "define flowop: Failed to instantiate flowop %s\n",
		    cmd->cmd_name);
		filebench_shutdown(1);
	}

	/* Iterations */
	if ((attr = get_attr_integer(cmd, FSA_ITERS)))
		flowop->fo_iters = attr->attr_avd;
	else
		flowop->fo_iters = avd_int_alloc(1);


	/* if this is a use of a composit flowop, create inner FLOW MASTERS */
	if (flowop_type->fo_type == FLOW_TYPE_COMPOSITE) {
		get_attr_lvars(cmd, flowop);
		if (category == FLOW_MASTER)
			parser_inner_flowop_define(thread,
			    flowop_type, flowop);
	}
	else {
		parser_flowop_get_attrs(cmd, flowop);
	}
}

static void
parser_composite_flowop_define(cmd_t *cmd)
{
	flowop_t *flowop;
	cmd_t *inner_cmd;
	char *name = NULL;
	attr_t *attr;

	/* Get the name of the flowop */
	if ((attr = get_attr(cmd, FSA_NAME))) {
		name = avd_get_str(attr->attr_avd);
	} else {
		filebench_log(LOG_ERROR,
		    "define flowop: Composit flowop specifies no name");

		filebench_shutdown(1);
	}

	if ((flowop = flowop_new_composite_define(name)) == NULL) {
		filebench_log(LOG_ERROR,
		    "define flowop: Failed to instantiate flowop %s\n",
		    cmd->cmd_name);
		filebench_shutdown(1);
	}

	/* place any local var_t variables on the flowop's local list */
	get_attr_lvars(cmd, flowop);

	/* Iterations */
	if ((attr = get_attr_integer(cmd, FSA_ITERS)))
		flowop->fo_iters = attr->attr_avd;
	else
		flowop->fo_iters = avd_int_alloc(1);

	/* define inner flowops */
	for (inner_cmd = cmd->cmd_list; inner_cmd != NULL;
	    inner_cmd = inner_cmd->cmd_next) {
		parser_flowop_define(inner_cmd, NULL,
		    &flowop->fo_comp_fops, FLOW_INNER_DEF);
	}
}


/*
 * Calls fileset_define() to allocate a fileset with the supplied name and
 * initializes the fileset's pathname attribute, and optionally the
 * fileset_cached, fileset_reuse, fileset_prealloc and fileset_size attributes.
 *
 */
static fileset_t *
parser_fileset_define_common(cmd_t *cmd)
{
	fileset_t *fileset;
	avd_t name;
	attr_t *attr;
	avd_t pathname;

	/*
	 * Make sure all plugin flowops are initialized.
	 * Defaults to local fs for now
	 */
	flowop_plugin_flowinit();

	/* Get the name of the file */
	if ((attr = get_attr_fileset(cmd, FSA_NAME))) {
		name = attr->attr_avd;
	} else {
		filebench_log(LOG_ERROR,
		    "define fileset: file or fileset specifies no name");
		return (NULL);
	}

	if ((fileset = fileset_define(name)) == NULL) {
		filebench_log(LOG_ERROR,
		    "define file: failed to instantiate file %s\n",
		    avd_get_str(name));
		return (NULL);
	}

	/* Get the pathname from attribute */
	if ((attr = get_attr(cmd, FSA_PATH)) == NULL) {
		filebench_log(LOG_ERROR, "define file: no pathname specified");
		return (NULL);
	}

	/* Expand variables in pathname */
	if ((pathname = parser_list2varstring(attr->attr_param_list))
	    == NULL) {
		filebench_log(LOG_ERROR, "Cannot interpret path");
		return (NULL);
	}

	fileset->fs_path = pathname;

	/* How much should we preallocate? */
	if ((attr = get_attr_integer(cmd, FSA_PREALLOC)) &&
	    attr->attr_avd) {
		if (AVD_IS_RANDOM(attr->attr_avd)) {
			filebench_log(LOG_ERROR,
			    "define fileset: Prealloc attr cannot be random");
			filebench_shutdown(1);
		}
		fileset->fs_preallocpercent = attr->attr_avd;
	} else if (attr && !attr->attr_avd) {
		fileset->fs_preallocpercent = avd_int_alloc(100);
	} else {
		fileset->fs_preallocpercent = avd_int_alloc(0);
	}

	/* Should we preallocate? */
	if ((attr = get_attr_bool(cmd, FSA_PREALLOC)))
		fileset->fs_prealloc = attr->attr_avd;
	else
		fileset->fs_prealloc = avd_bool_alloc(FALSE);

	/* Should we prealloc in parallel? */
	if ((attr = get_attr_bool(cmd, FSA_PARALLOC)))
		fileset->fs_paralloc = attr->attr_avd;
	else
		fileset->fs_paralloc = avd_bool_alloc(FALSE);

	/* Should we allow writes to the file? */
	if ((attr = get_attr_bool(cmd, FSA_READONLY)))
		fileset->fs_readonly = attr->attr_avd;
	else
		fileset->fs_readonly = avd_bool_alloc(FALSE);

	if ((attr = get_attr_bool(cmd, FSA_WRITEONLY)))
		fileset->fs_writeonly = attr->attr_avd;
	else
		fileset->fs_writeonly = avd_bool_alloc(FALSE);

	if ((avd_get_bool(fileset->fs_readonly) == TRUE) &&
		(avd_get_bool(fileset->fs_writeonly) == TRUE)) {
		filebench_log(LOG_ERROR, "fileset can't be read-only and "
					"write-only at the same time!");
		return NULL;
	}

	/* Should we reuse the existing file? */
	if ((attr = get_attr_bool(cmd, FSA_REUSE)))
		fileset->fs_reuse = attr->attr_avd;
	else
		fileset->fs_reuse = avd_bool_alloc(FALSE);

	/* Should we check for files actual existance? */
	if ((attr = get_attr_bool(cmd, FSA_TRUSTTREE)))
		fileset->fs_trust_tree = attr->attr_avd;
	else
		fileset->fs_trust_tree = avd_bool_alloc(FALSE);

	/* Should we leave in cache? */
	if ((attr = get_attr_bool(cmd, FSA_CACHED)))
		fileset->fs_cached = attr->attr_avd;
	else
		fileset->fs_cached = avd_bool_alloc(FALSE);

	/* Get the mean or absolute size of the file */
	if ((attr = get_attr_integer(cmd, FSA_SIZE)))
		fileset->fs_size = attr->attr_avd;
	else
		fileset->fs_size = avd_int_alloc(0);

	return (fileset);
}

/*
 * Calls parser_fileset_define_common() to allocate a fileset with
 * one entry and optionally the fileset_prealloc. sets the fileset_entries,
 * fileset_dirwidth, fileset_dirgamma, and fileset_sizegamma attributes
 * to appropriate values for emulating the old "fileobj" entity
 */
static void
parser_file_define(cmd_t *cmd)
{
	fileset_t *fileset;

	if ((fileset = parser_fileset_define_common(cmd)) == NULL) {
		filebench_log(LOG_ERROR,
		    "define file: failed to instantiate file");
		filebench_shutdown(1);
		return;
	}

	/* fileset is emulating a single file */
	fileset->fs_attrs = FILESET_IS_FILE;

	/* Set the size of the fileset to 1 */
	fileset->fs_entries = avd_int_alloc(1);

	/* Set the mean dir width to more than 1 */
	fileset->fs_dirwidth = avd_int_alloc(10);

	/* Set the dir and size gammas to 0 */
	fileset->fs_dirgamma = avd_int_alloc(0);
	fileset->fs_sizegamma = avd_int_alloc(0);
}

/*
 * Calls parser_fileset_define_common() to allocate a fileset with the
 * supplied name and initializes the fileset's fileset_preallocpercent,
 * fileset_prealloc, fileset_entries, fileset_dirwidth, fileset_dirgamma,
 * and fileset_sizegamma attributes.
 */
static void
parser_fileset_define(cmd_t *cmd)
{
	fileset_t *fileset;
	attr_t *attr;

	if ((fileset = parser_fileset_define_common(cmd)) == NULL) {
		filebench_log(LOG_ERROR,
		    "define fileset: failed to instantiate fileset");
		filebench_shutdown(1);
		return;
	}
	/* Get the number of files in the fileset */
	if ((attr = get_attr_integer(cmd, FSA_ENTRIES))) {
		fileset->fs_entries = attr->attr_avd;
	} else {
		fileset->fs_entries = avd_int_alloc(0);
	}

	/* Get the number of leafdirs in the fileset */
	if ((attr = get_attr_integer(cmd, FSA_LEAFDIRS))) {
		fileset->fs_leafdirs = attr->attr_avd;
	} else {
		fileset->fs_leafdirs = avd_int_alloc(0);
	}

	if ((avd_get_int(fileset->fs_entries) == 0) &&
	    (avd_get_int(fileset->fs_leafdirs) == 0)) {
		filebench_log(LOG_ERROR, "Fileset has no files or leafdirs");
	}

	/* Get the mean dir width of the fileset */
	if ((attr = get_attr_integer(cmd, FSA_DIRWIDTH))) {
		fileset->fs_dirwidth = attr->attr_avd;
	} else {
		filebench_log(LOG_ERROR, "Fileset has zero directory width");
		fileset->fs_dirwidth = avd_int_alloc(0);
	}

	/* Get the random variable for dir depth, if supplied */
	if ((attr = get_attr_integer(cmd, FSA_DIRDEPTHRV))) {
		if (!AVD_IS_RANDOM(attr->attr_avd)) {
			filebench_log(LOG_ERROR,
			    "Define fileset: dirdepthrv must be random var");
			filebench_shutdown(1);
		}
		fileset->fs_dirdepthrv = attr->attr_avd;
	} else {
		fileset->fs_dirdepthrv = NULL;
	}

	/* Get the gamma value for dir depth distributions */
	if ((attr = get_attr_integer(cmd, FSA_DIRGAMMA))) {
		if (AVD_IS_RANDOM(attr->attr_avd)) {
			filebench_log(LOG_ERROR,
			    "Define fileset: dirgamma attr cannot be random");
			filebench_shutdown(1);
		}
		fileset->fs_dirgamma = attr->attr_avd;
	} else
		fileset->fs_dirgamma = avd_int_alloc(1500);

	/* Get the gamma value for dir width distributions */
	if ((attr = get_attr_integer(cmd, FSA_FILESIZEGAMMA))) {
		if (AVD_IS_RANDOM(attr->attr_avd)) {
			filebench_log(LOG_ERROR,
			    "Define fileset: filesizegamma cannot be random");
			filebench_shutdown(1);
		}
		fileset->fs_sizegamma = attr->attr_avd;
	} else
		fileset->fs_sizegamma = avd_int_alloc(1500);
}

static void
parser_posset_define(cmd_t *cmd)
{
	attr_t *attr;
	avd_t name;
	avd_t type;
	avd_t seed;
	avd_t max;
	avd_t entries;
	int i;

	struct posset *ps;
	
	if ((attr = get_attr(cmd, FSA_NAME))) {
		name = attr->attr_avd;
	} else {
		filebench_log(LOG_ERROR,
		   "parser_posset_define: no name specified for the posset");
		filebench_shutdown(1);
		return;
	}

	if ((attr = get_attr(cmd, FSA_TYPE))) {
		type = attr->attr_avd;
	} else {
		filebench_log(LOG_ERROR,
		   "parser_posset_define: no type specified for the posset");
		filebench_shutdown(1);
		return;
	}

	if ((attr = get_attr_integer(cmd, FSA_ENTRIES))) {
		entries = attr->attr_avd;
	} else {
		filebench_log(LOG_ERROR,
		   "parser_posset_define: no entries number "
					"specified for the posset");
		filebench_shutdown(1);
		return;
	}

	/*
	 * two attributes below are mandatory only for specific types
	 * of possets, so postpone their validation for posset type
	 * specific functions.
	 */
	if ((attr = get_attr_integer(cmd, FSA_RANDSEED)))
		seed = attr->attr_avd;
	else
		seed = avd_int_alloc(0);

	if ((attr = get_attr_integer(cmd, FSA_RANDMAX)))
		max = attr->attr_avd;
	else
		max = avd_int_alloc(0);

	filebench_log(LOG_INFO,"Defining position set: %s type: %s",
				 avd_get_str(name), avd_get_str(type));

	ps = posset_alloc(name, type, seed, max, entries);
	if (!ps) {
		filebench_log(LOG_ERROR, "Cannot define fileset");
		filebench_shutdown(1);
		/* NOT REACHABLE */
		return;
	}

	/* printing all positions */
	for (i = 0; i < avd_get_int(ps->ps_entries); i++)
		filebench_log(LOG_INFO,"pos %d: %llu", i, ps->ps_positions[i]);

}

/*
 * Creates and starts all defined procflow processes. The call to
 * procflow_init() results in creation of the requested number of
 * process instances for each previously defined procflow. The
 * child processes exec() a new instance of filebench, passing it
 * the instance number and address of the shared memory region.
 * The child processes will then create their threads and flowops.
 * The routine then unlocks the run_lock to allow all the processes'
 * threads to start and  waits for all of them to begin execution.
 * Finally, it records the start time and resets the event generation
 * system.
 */
static void
parser_proc_create(cmd_t *cmd)
{
	filebench_shm->shm_1st_err = 0;
	filebench_shm->shm_f_abort = FILEBENCH_OK;

	if (procflow_init() != 0) {
		filebench_log(LOG_ERROR, "Failed to create processes\n");
		filebench_shutdown(1);
	}

	/* Wait for all threads to start */
	if (procflow_allstarted() != 0) {
		filebench_log(LOG_ERROR, "Could not start run");
		return;
	}

	/* Release the read lock, allowing threads to start */
	(void) pthread_rwlock_unlock(&filebench_shm->shm_run_lock);

	if (filebench_shm->shm_required &&
	    (ipc_ismcreate(filebench_shm->shm_required) < 0)) {
		filebench_log(LOG_ERROR, "Could not allocate shared memory");
		return;
	}

	filebench_shm->shm_starttime = gethrtime();
	eventgen_reset();
}

/*
 * Calls fileset_createset() to populate all files and filesets and
 * create all associated, initially existant,  files and subdirectories.
 * If errors are encountered, calls filebench_shutdown()
 * to exit filebench.
 */
static void
parser_fileset_create(cmd_t *cmd)
{
	if (!filecreate_done) {
		filecreate_done = 1;

		/* initialize the random number system first */
		randdist_init();

		/* create all the filesets */
		if (fileset_createset(NULL) != 0) {
			filebench_log(LOG_ERROR, "Failed to create filesets");
			filebench_shutdown(1);
		}
	} else {
		filebench_log(LOG_INFO,
		    "Attempting to create fileset more than once, ignoring");
	}

}

/*
 * Deletes the files and directories that represent files and filesets on the
 * storage medium.
 */
static void
parser_fileset_shutdown(cmd_t *cmd)
{
	filebench_log(LOG_INFO, "Shutting down filesets");
	fileset_delete_all_filesets();
}

/*
 * Shuts down all processes and their associated threads. When finished
 * it deletes interprocess shared memory and resets the event generator.
 * It does not exit the filebench program though.
 */
static void
parser_proc_shutdown(cmd_t *cmd)
{
	filebench_log(LOG_INFO, "Shutting down processes");
	filecreate_done = 0;
	procflow_shutdown();
	if (filebench_shm->shm_required)
		ipc_ismdelete();
	eventgen_reset();
}

/*
 * Ends filebench run after first destoring any interprocess
 * shared memory. The call to filebench_shutdown()
 * also causes filebench to exit.
 */
static void
parser_filebench_shutdown(cmd_t *cmd)
{
	int f_abort = filebench_shm->shm_f_abort;

	ipc_fini();

	if (f_abort == FILEBENCH_ABORT_ERROR)
		filebench_shutdown(1);
	else
		filebench_shutdown(0);
}

/*
 * This is Used for timing runs.Pauses the master thread in one second
 * intervals until the supplied ptime runs out or the f_abort flag
 * is raised. If given a time of zero or less, or the mode is stop on
 * lack of resources, it will pause until f_abort is raised.
 */
static int
parser_pause(int ptime)
{
	int timeslept = 0;

	if ((filebench_shm->shm_rmode == FILEBENCH_MODE_TIMEOUT) &&
	    (ptime > 0)) {
		while (timeslept < ptime) {
			(void) sleep(1);
			timeslept++;
			if (filebench_shm->shm_f_abort)
				break;
		}
	} else {
		/* initial runtime of 0 means run till abort */
		/* CONSTCOND */
		while (1) {
			(void) sleep(1);
			timeslept++;
			if (filebench_shm->shm_f_abort)
				break;
		}
	}
	return (timeslept);
}

/*
 * Do a file bench run. Calls routines to create file sets, files, and
 * processes. It resets the statistics counters, then sleeps for the runtime
 * passed as an argument to it on the command line in 1 second increments.
 * When it is finished sleeping, it collects a snapshot of the statistics
 * and ends the run.
 */
static void
parser_run(cmd_t *cmd)
{
	int runtime;
	int timeslept;

	runtime = cmd->cmd_qty;

	parser_fileset_create(cmd);
	parser_proc_create(cmd);

	/* check for startup errors */
	if (filebench_shm->shm_f_abort)
		return;

	filebench_log(LOG_INFO, "Running...");
	stats_clear();

	timeslept = parser_pause(runtime);

	filebench_log(LOG_INFO, "Run took %d seconds...", timeslept);
	parser_statssnap(cmd);
	parser_proc_shutdown(cmd);
	parser_filebench_shutdown((cmd_t *)0);
}

/*
 * Similar to parser_run, but gets the sleep time from a variable
 * whose name is supplied as an argument to the command.
 */
static void
parser_run_variable(cmd_t *cmd)
{
	avd_t integer = var_ref_attr(cmd->cmd_tgt1);
	int runtime;
	int timeslept;

	if (integer == NULL) {
		filebench_log(LOG_ERROR, "Unknown variable %s",
		cmd->cmd_tgt1);
		return;
	}

	runtime = avd_get_int(integer);

	/* check for startup errors */
	if (filebench_shm->shm_f_abort)
		return;

	filebench_log(LOG_INFO, "Running...");
	stats_clear();

	timeslept = parser_pause(runtime);

	filebench_log(LOG_INFO, "Run took %d seconds...", timeslept);
	parser_statssnap(cmd);
	parser_proc_shutdown(cmd);
}

char *usagestr = NULL;

/*
 * Prints usage string if defined, else just a message requesting load of a
 * personality.
 */
static void
parser_help(cmd_t *cmd)
{
	if (usagestr) {
		filebench_log(LOG_INFO, "%s", usagestr);
	} else {
		filebench_log(LOG_INFO,
		    "load <personality> (ls "
		    "%s/workloads for list)", fbbasepath);
	}
}

char *varstr = NULL;

/*
 * Prints the string of all var definitions, if there is one.
 */
static void
parser_printvars(cmd_t *cmd)
{
	char *str, *c;

	if (varstr) {
		str = strdup(varstr);
		for (c = str; *c != '\0'; c++) {
			if ((char)*c == '$')
				*c = ' ';
		}
		filebench_log(LOG_INFO, "%s", str);
		free(str);
	}
}

/*
 * Establishes multi-client synchronization socket with synch server.
 */
static void
parser_enable_mc(cmd_t *cmd)
{
	attr_t *attr;
	char *master;
	char *client;

	if ((attr= get_attr(cmd, FSA_MASTER))) {
		master = avd_get_str(attr->attr_avd);
	} else {
		filebench_log(LOG_ERROR,
		    "enable multi: no master specified");
		return;
	}

	if ((attr= get_attr(cmd, FSA_CLIENT))) {
		client = avd_get_str(attr->attr_avd);
	} else {
		filebench_log(LOG_ERROR,
		    "enable multi: no client specified");
		return;
	}

	mc_sync_open_sock(master, 8001, client);
}

/*
 * Exchanges multi-client synchronization message with synch server.
 */
static void
parser_domultisync(cmd_t *cmd)
{
	attr_t *attr;
	fbint_t value;

	if ((attr = get_attr(cmd, FSA_VALUE)))
		value = avd_get_int(attr->attr_avd);
	else
		value = 1;

	mc_sync_synchronize((int)value);
}

/*
 * Used by the SET command to add a var and default value string to the
 * varstr string. It allocates a new, larger varstr string, copies the
 * old contents of varstr into it, then adds the new var string on the end.
 */
static void
parser_vars(cmd_t *cmd)
{
	char *string = cmd->cmd_tgt1;
	char *newvars;

	if (string == NULL)
		return;

	if (dofile == DOFILE_TRUE)
		return;

	if (varstr == NULL) {
		newvars = malloc(strlen(string) + 2);
		*newvars = 0;
	} else {
		newvars = malloc(strlen(varstr) + strlen(string) + 2);
		(void) strcpy(newvars, varstr);
	}
	(void) strcat(newvars, string);
	(void) strcat(newvars, " ");

	if (varstr)
		free(varstr);

	varstr = newvars;
}

/*
 * used by the set command to set the integer part of a regular
 * variable, or the appropriate field of a random variable
 */
static void
parser_set_integer(cmd_t *cmd)
{
	var_assign_integer(cmd->cmd_tgt1, cmd->cmd_qty);
}

/*
 * used by the set command to set the integer part of a regular
 * variable from another variable, or the appropriate field of a
 * random variable from another variable
 */
static void
parser_set_var(cmd_t *cmd)
{
	var_assign_var(cmd->cmd_tgt1, cmd->cmd_tgt2);
}

/*
 * Used by the set command to set up for a binary operation of a
 * variable from a var, with an integer
 */
static void
parser_set_var_op_int(cmd_t *cmd)
{
	printf("parser_set_var_op_int: Called\n");
	switch (cmd->cmd_subtype) {
	case FSK_PLUS:
		var_assign_op_var_int(cmd->cmd_tgt1, VAR_IND_INT_SUM_IV,
		    cmd->cmd_tgt2, cmd->cmd_qty);
		break;

	case FSK_MINUS:
		var_assign_op_var_int(cmd->cmd_tgt1, VAR_IND_IV_DIF_INT,
		    cmd->cmd_tgt2, cmd->cmd_qty);
		break;

	case FSK_MULTIPLY:
		var_assign_op_var_int(cmd->cmd_tgt1, VAR_IND_INT_MUL_IV,
		    cmd->cmd_tgt2, cmd->cmd_qty);
		break;

	case FSK_DIVIDE:
		var_assign_op_var_int(cmd->cmd_tgt1, VAR_IND_IV_DIV_INT,
		    cmd->cmd_tgt2, cmd->cmd_qty);
		break;
	}
}

/*
 * Used by the set command to set up for a binary operation of an
 * integer with a variable from a var
 */
static void
parser_set_int_op_var(cmd_t *cmd)
{
	switch (cmd->cmd_subtype) {
	case FSK_PLUS:
		var_assign_op_var_int(cmd->cmd_tgt1, VAR_IND_INT_SUM_IV,
		    cmd->cmd_tgt3, cmd->cmd_qty);
		break;

	case FSK_MINUS:
		var_assign_op_var_int(cmd->cmd_tgt1, VAR_IND_INT_DIF_IV,
		    cmd->cmd_tgt3, cmd->cmd_qty);
		break;

	case FSK_MULTIPLY:
		var_assign_op_var_int(cmd->cmd_tgt1, VAR_IND_INT_MUL_IV,
		    cmd->cmd_tgt3, cmd->cmd_qty);
		break;

	case FSK_DIVIDE:
		var_assign_op_var_int(cmd->cmd_tgt1, VAR_IND_INT_DIV_IV,
		    cmd->cmd_tgt3, cmd->cmd_qty);
		break;
	}
}

/*
 * Used by the set command to set up for a binary operation of two
 * variables from other vars.
 */
static void
parser_set_var_op_var(cmd_t *cmd)
{
	switch (cmd->cmd_subtype) {
	case FSK_PLUS:
		var_assign_op_var_var(cmd->cmd_tgt1, VAR_IND_IV_SUM_IV,
		    cmd->cmd_tgt2, cmd->cmd_tgt3);
		break;

	case FSK_MINUS:
		var_assign_op_var_var(cmd->cmd_tgt1, VAR_IND_IV_DIF_IV,
		    cmd->cmd_tgt2, cmd->cmd_tgt3);
		break;

	case FSK_MULTIPLY:
		var_assign_op_var_var(cmd->cmd_tgt1, VAR_IND_IV_MUL_IV,
		    cmd->cmd_tgt2, cmd->cmd_tgt3);
		break;

	case FSK_DIVIDE:
		var_assign_op_var_var(cmd->cmd_tgt1, VAR_IND_IV_DIV_IV,
		    cmd->cmd_tgt2, cmd->cmd_tgt3);
		break;
	}
}


/*
 * Sleeps for cmd->cmd_qty seconds, one second at a time.
 */
static void
parser_warmup(cmd_t *cmd)
{
	int sleeptime;

	/* check for startup errors */
	if (filebench_shm->shm_f_abort)
		return;

	sleeptime = cmd->cmd_qty;
	filebench_log(LOG_INFO, "Warming up...");

	(void) parser_pause(sleeptime);
}

/*
 * Same as parser_sleep, except the sleep time is obtained from a variable
 * whose name is passed to it as an argument on the command line.
 */
static void
parser_warmup_variable(cmd_t *cmd)
{
	avd_t integer = var_ref_attr(cmd->cmd_tgt1);
	int sleeptime;

	if (integer == NULL) {
		filebench_log(LOG_ERROR, "Unknown variable %s",
		cmd->cmd_tgt1);
		return;
	}

	sleeptime = avd_get_int(integer);

	/* check for startup errors */
	if (filebench_shm->shm_f_abort)
		return;

	filebench_log(LOG_INFO, "Warming up...");

	(void) parser_pause(sleeptime);
}

/*
 * Sleeps for cmd->cmd_qty seconds, one second at a time.
 */
static void
parser_sleep(cmd_t *cmd)
{
	int sleeptime;
	int timeslept;

	/* check for startup errors */
	if (filebench_shm->shm_f_abort)
		return;

	sleeptime = cmd->cmd_qty;
	filebench_log(LOG_INFO, "Running...");

	timeslept = parser_pause(sleeptime);

	filebench_log(LOG_INFO, "Run took %d seconds...", timeslept);
}

/*
 * Same as parser_sleep, except the sleep time is obtained from a variable
 * whose name is passed to it as an argument on the command line.
 */
static void
parser_sleep_variable(cmd_t *cmd)
{
	avd_t integer = var_ref_attr(cmd->cmd_tgt1);
	int sleeptime;
	int timeslept;

	if (integer == NULL) {
		filebench_log(LOG_ERROR, "Unknown variable %s",
		cmd->cmd_tgt1);
		return;
	}

	sleeptime = avd_get_int(integer);

	/* check for startup errors */
	if (filebench_shm->shm_f_abort)
		return;

	filebench_log(LOG_INFO, "Running...");

	timeslept = parser_pause(sleeptime);

	filebench_log(LOG_INFO, "Run took %d seconds...", timeslept);
}

/*
 * Parser log prints the values of a list of variables to the log file.
 * The list of variables is placed on the command line, separated
 * by comas and the entire list is enclosed in quotes.
 * For example, if $dir contains "/export/home/tmp" and $filesize = 1048576,
 * then typing: log "$dir, $filesize" prints: log /export/home/tmp, 1048576
 */
static void
parser_log(cmd_t *cmd)
{
	char *string;

	if (cmd->cmd_param_list == NULL)
		return;

	string = parser_list2string(cmd->cmd_param_list);

	if (string == NULL)
		return;

	filebench_log(LOG_VERBOSE, "log %s", string);
	filebench_log(LOG_LOG, "%s", string);
}

/*
 * Implements the stats directory command. changes the directory for
 * dumping statistics to supplied directory path. For example:
 * 	stats directory /tmp
 * changes the stats directory to "/tmp".
 */
static void
parser_directory(cmd_t *cmd)
{
	char newdir[MAXPATHLEN];
	char *dir;
	int ret;

	if ((dir = parser_list2string(cmd->cmd_param_list)) == NULL) {
		filebench_log(LOG_ERROR, "Cannot interpret directory");
		return;
	}

	*newdir = 0;
	/* Change dir relative to cwd if path not fully qualified */
	if (*dir != '/') {
		(void) strcat(newdir, cwd);
		(void) strcat(newdir, "/");
	}
	(void) strcat(newdir, dir);
	(void) mkdir(newdir, 0755);
	filebench_log(LOG_VERBOSE, "Change dir to %s", newdir);
	ret = chdir(newdir);
	free(dir);
}

#define	PIPE_PARENT 1
#define	PIPE_CHILD  0

/*
 * Runs the quoted unix command as a background process. Intended for
 * running statistics gathering utilities such as mpstat while the filebench
 * workload is running. Also records the pid's of the background processes
 * so that parser_statssnap() can terminate them when the run completes.
 */
static void
parser_statscmd(cmd_t *cmd)
{
	char *string;
	pid_t pid;
	pidlist_t *pidlistent;
	int pipe_fd[2];
	int newstdout;

	if (cmd->cmd_param_list == NULL)
		return;

	string = parser_list2string(cmd->cmd_param_list);

	if (string == NULL)
		return;

	if ((pipe(pipe_fd)) < 0) {
		filebench_log(LOG_ERROR, "statscmd pipe failed");
		return;
	}

#ifdef HAVE_FORK1
	if ((pid = fork1()) < 0) {
		filebench_log(LOG_ERROR, "statscmd fork failed");
		return;
	}
#elif HAVE_FORK
	if ((pid = fork()) < 0) {
		filebench_log(LOG_ERROR, "statscmd fork failed");
		return;
	}
#else
	Crash! - Need code to deal with no fork1!
#endif /* HAVE_FORK1 */

	if (pid == 0) {

		setsid();

		filebench_log(LOG_VERBOSE,
		    "Backgrounding %s", string);
		/*
		 * Child
		 * - close stdout
		 * - dup to create new stdout
		 * - close pipe fds
		 */
		(void) close(1);

		if ((newstdout = dup(pipe_fd[PIPE_CHILD])) < 0) {
			filebench_log(LOG_ERROR,
			    "statscmd dup failed: %s",
			    strerror(errno));
		}

		(void) close(pipe_fd[PIPE_PARENT]);
		(void) close(pipe_fd[PIPE_CHILD]);

		if (system(string) < 0) {
			filebench_log(LOG_ERROR,
			    "statscmd exec failed: %s",
			    strerror(errno));
		}
		/* Failed! */
		exit(1);

	} else {

		/* Record pid in pidlist for subsequent reaping by stats snap */
		if ((pidlistent = (pidlist_t *)malloc(sizeof (pidlist_t)))
		    == NULL) {
			filebench_log(LOG_ERROR, "pidlistent malloc failed");
			return;
		}

		pidlistent->pl_pid = pid;
		pidlistent->pl_fd = pipe_fd[PIPE_PARENT];
		(void) close(pipe_fd[PIPE_CHILD]);

		/* Add fileobj to global list */
		if (pidlist == NULL) {
			pidlist = pidlistent;
			pidlistent->pl_next = NULL;
		} else {
			pidlistent->pl_next = pidlist;
			pidlist = pidlistent;
		}
	}
}

/*
 * Launches a shell to run the unix command supplied in the argument.
 * The command should be enclosed in quotes, as in:
 * 	system "rm xyz"
 * which would run the "rm" utility to delete the file "xyz".
 */
static void
parser_system(cmd_t *cmd)
{
	char *string;

	if (cmd->cmd_param_list == NULL)
		return;

	string = parser_list2string(cmd->cmd_param_list);

	if (string == NULL)
		return;

	filebench_log(LOG_VERBOSE,
	    "Running '%s'", string);

	if (system(string) < 0) {
		filebench_log(LOG_ERROR,
		    "system exec failed: %s",
		    strerror(errno));
		filebench_shutdown(1);
	}
	free(string);
}

/*
 * Echos string supplied with command to the log.
 */
static void
parser_echo(cmd_t *cmd)
{
	char *string;

	if (cmd->cmd_param_list == NULL)
		return;

	string = parser_list2string(cmd->cmd_param_list);

	if (string == NULL)
		return;

	filebench_log(LOG_INFO, "%s", string);
}

static void parser_fscheck(cmd_t *cmd) {
	return;
}

#if 0
/* XXX: do not support this command for now */
/*
 * Checks to see if the specified data directory exists and it's mounted file
 * system is the correct type.
 */
static void
parser_fscheck(cmd_t *cmd)
{
	int fstype_idx;
	char *pathname = NULL;
	char *filesys = "tmpfs";
	char string[MAXPATHLEN];
	struct statvfs64 statbuf;
	attr_t *attr;

	if (cmd->cmd_attr_list == NULL)
		return;

	for (attr = cmd->cmd_attr_list; attr; attr = attr->attr_next) {

		switch(attr->attr_name) {
		case FSA_PATH:
			pathname = avd_get_str(attr->attr_avd);
			break;
		case FSA_FSTYPE:
			filesys = avd_get_str(attr->attr_avd);
			break;
		}
	}

	if (pathname == NULL)
		return;

	if (statvfs64(pathname, &statbuf) < 0) {
		filebench_log(LOG_ERROR,
		    "%s error with supplied data path name: %s; exiting",
		    strerror(errno), pathname);
		filebench_shutdown(1);
		return;
	}

	if (strncmp(filesys, statbuf.f_basetype, FSTYPSZ) != 0) {
		filebench_log(LOG_ERROR,
		    "File System is of type %s, NOT %s as indicated",
		    statbuf.f_basetype, filesys);
		filebench_shutdown(1);
		return;
	}
}
#endif

/*
 * Checks to see if any filesets need to have their caches flushed, and
 * if so invokes the fs_flush script.
 */
static void
parser_fsflush(cmd_t *cmd)
{
	fileset_t *fileset;
	char **fspathlist;
	char *pathname = NULL;
	char *filesys = NULL;
	char string[MAXPATHLEN];
	attr_t *attr;
	int fsidx;

	if ((attr = cmd->cmd_attr_list) == NULL)
		return;

	/* Get supplied file system type */
	if (attr->attr_name == FSA_FSTYPE)
		filesys = avd_get_str(attr->attr_avd);

	if (filesys == NULL) {
		filebench_log(LOG_ERROR,
		    "FSFLUSH command lacks file system type");
		return;
	}

	/* Check all filesets for any that remain cached and count them*/
	fsidx = 0;
	for (fileset = filebench_shm->shm_filesetlist; fileset != NULL;
	     fileset = fileset->fs_next) {

		if (avd_get_bool(fileset->fs_cached))
			return;

		fsidx++;
	}

	/* allocated space for fileset path pointers */
	fspathlist = (char **)malloc(fsidx * sizeof(char *));

	/* If flushing still required, flush all filesets */
	fsidx = 0;
	for (fileset = filebench_shm->shm_filesetlist; fileset != NULL;
	     fileset = fileset->fs_next) {
		int idx;

		if ((pathname = avd_get_str(fileset->fs_path)) == NULL)
			return;

		for (idx = 0; idx < fsidx; idx++) {
			if (strcmp(pathname, fspathlist[idx]) == 0)
				break;
		}

		if (fsidx == idx) {

			/* found a new path */
			fspathlist[fsidx++] = pathname;

			/* now flush it */
			snprintf(string, MAXPATHLEN,
			    "%s/scripts/fs_flush %s %s", fbbasepath,
			    filesys, pathname);

			if (system(string) < 0) {
				filebench_log(LOG_ERROR,
				    "exec of fs_flush script failed: %s",
				    strerror(errno));
				filebench_shutdown(1);
			}
		}
	}
}

/*
 * Prints out the version of Filebench.
 */
static void
parser_version(cmd_t *cmd)
{
	filebench_log(LOG_INFO, "Filebench Version: %s", FILEBENCH_VERSION);
}

/*
 * Sets osprof_enabled option
 */
static void
parser_osprof_enable(cmd_t *cmd)
{
	filebench_shm->osprof_enabled = 1;
	filebench_log(LOG_INFO, "OSprof enabled");
}

/*
 * Resets osprof_enabled option
 */
static void
parser_osprof_disable(cmd_t *cmd)
{
	filebench_shm->osprof_enabled = 0;
	filebench_log(LOG_INFO, "OSprof disabled");
}

/*
 * Adds the string supplied as the argument to the usage command
 * to the end of the string printed by the help command.
 */
static void
parser_usage(cmd_t *cmd)
{
	char *string;
	char *newusage;

	if (cmd->cmd_param_list == NULL)
		return;

	string = parser_list2string(cmd->cmd_param_list);

	if (string == NULL)
		return;

	if (dofile == DOFILE_TRUE)
		return;

	if (usagestr == NULL) {
		newusage = malloc(strlen(string) + 2);
		*newusage = 0;
	} else {
		newusage = malloc(strlen(usagestr) + strlen(string) + 2);
		(void) strcpy(newusage, usagestr);
	}
	(void) strcat(newusage, "\n");
	(void) strcat(newusage, string);

	if (usagestr)
		free(usagestr);

	usagestr = newusage;

	filebench_log(LOG_INFO, "%s", string);
}

/*
 * Updates the global dump filename with the filename supplied
 * as the command's argument. Then dumps the statistics of each
 * worker flowop into the dump file, followed by a summary of
 * overall totals.
 */
static void
parser_statsdump(cmd_t *cmd)
{
	char *string;

	if (cmd->cmd_param_list == NULL)
		return;

	string = parser_list2string(cmd->cmd_param_list);

	if (string == NULL)
		return;

	filebench_log(LOG_VERBOSE,
	    "Stats dump to file '%s'", string);

	stats_dump(string);

	free(string);
}

/*
 * Same as statsdump, but outputs in a computer friendly format.
 */
static void
parser_statsmultidump(cmd_t *cmd)
{
	char *string;

	if (cmd->cmd_param_list == NULL)
		return;

	string = parser_list2string(cmd->cmd_param_list);

	if (string == NULL)
		return;

	filebench_log(LOG_VERBOSE,
	    "Stats dump to file '%s'", string);

	stats_multidump(string);

	free(string);
}

/*
 * Same as parser_statsdump, but in xml format.
 */
static void
parser_statsxmldump(cmd_t *cmd)
{
	char *string;

	if (cmd->cmd_param_list == NULL)
		return;

	string = parser_list2string(cmd->cmd_param_list);

	if (string == NULL)
		return;

	filebench_log(LOG_VERBOSE,
	    "Stats dump to file '%s'", string);

	stats_xmldump(string);

	free(string);
}

/*
 * Kills off background statistics collection processes, then takes a snapshot
 * of the filebench run's collected statistics using stats_snap() from
 * stats.c.
 */
static void
parser_statssnap(cmd_t *cmd)
{
	pidlist_t *pidlistent;
	int stat;
	pid_t pid;

	for (pidlistent = pidlist; pidlistent != NULL;
	    pidlistent = pidlistent->pl_next) {
		filebench_log(LOG_VERBOSE, "Killing session %d for pid %d",
		    getsid(pidlistent->pl_pid),
		    pidlistent->pl_pid);
		if (pidlistent->pl_fd)
			(void) close(pidlistent->pl_fd);
#ifdef HAVE_SIGSEND
		sigsend(P_SID, getsid(pidlistent->pl_pid), SIGTERM);
#else
		(void) kill(-1, SIGTERM);
#endif

		/* Close pipe */
		if (pidlistent->pl_fd)
			(void) close(pidlistent->pl_fd);

		/* Wait for cmd and all its children */
		while ((pid = waitpid(pidlistent->pl_pid * -1, &stat, 0)) > 0)
			filebench_log(LOG_DEBUG_IMPL,
			"Waited for pid %d", (int)pid);
	}

	for (pidlistent = pidlist; pidlistent != NULL;
	    pidlistent = pidlistent->pl_next) {
		free(pidlistent);
	}

	pidlist = NULL;
	stats_snap();
}

/*
 * Shutdown filebench.
 */
static void
parser_abort(int arg)
{
	(void) sigignore(SIGINT);
	filebench_log(LOG_INFO, "Aborting...");
	filebench_shutdown(1);
}

/*
 * define a random variable and initialize the distribution parameters
 */
static void
parser_randvar_define(cmd_t *cmd)
{
	var_t		*var;
	randdist_t	*rndp;
	attr_t		*attr;
	char		*name;

	/* Get the name for the random variable */
	if ((attr = get_attr(cmd, FSA_NAME))) {
		name = avd_get_str(attr->attr_avd);
	} else {
		filebench_log(LOG_ERROR,
		    "define randvar: no name specified");
		return;
	}

	if ((var = var_define_randvar(name)) == NULL) {
		filebench_log(LOG_ERROR,
		    "define randvar: failed for random variable %s",
		    name);
		return;
	}

	rndp = var->var_val.randptr;
	rndp->rnd_type = 0;

	/* Get the source of the random numbers */
	if ((attr = get_attr_integer(cmd, FSA_RANDSRC))) {
		int randsrc = (int)avd_get_int(attr->attr_avd);

		switch (randsrc) {
		case FSV_URAND:
			rndp->rnd_type |= RAND_SRC_URANDOM;
			break;
		case FSV_RAND48:
			rndp->rnd_type |= RAND_SRC_GENERATOR;
			break;
		}
	} else {
		/* default to rand48 random number generator */
		rndp->rnd_type |= RAND_SRC_GENERATOR;
	}

	/* Get the min value of the random distribution */
	if ((attr = get_attr_integer(cmd, FSA_RANDMIN)))
		rndp->rnd_min = attr->attr_avd;
	else
		rndp->rnd_min = avd_int_alloc(0);

	/* Get the roundoff value for the random distribution */
	if ((attr = get_attr_integer(cmd, FSA_RANDROUND)))
		rndp->rnd_round = attr->attr_avd;
	else
		rndp->rnd_round = avd_int_alloc(0);

	/* Get a tablular probablility distribution if there is one */
	if ((attr = get_attr(cmd, FSA_RANDTABLE))) {
		rndp->rnd_probtabs = (probtabent_t *)(attr->attr_obj);
		rndp->rnd_type |= RAND_TYPE_TABLE;

		/* no need for the rest of the attributes */
		return;
	} else {
		rndp->rnd_probtabs = NULL;
	}

	/* Get the type for the random variable */
	if ((attr = get_attr(cmd, FSA_TYPE))) {
		int disttype = (int)avd_get_int(attr->attr_avd);

		switch (disttype) {
		case FSV_RANDUNI:
			rndp->rnd_type |= RAND_TYPE_UNIFORM;
			break;
		case FSA_RANDGAMMA:
			rndp->rnd_type |= RAND_TYPE_GAMMA;
			break;
		case FSV_RANDTAB:
			filebench_log(LOG_ERROR,
			    "Table distribution type without prob table");
			break;
		}
	} else {
		/* default to gamma distribution type */
		rndp->rnd_type |= RAND_TYPE_GAMMA;
	}

	/* Get the seed for the random variable */
	if ((attr = get_attr_integer(cmd, FSA_RANDSEED)))
		rndp->rnd_seed = attr->attr_avd;
	else
		rndp->rnd_seed = avd_int_alloc(0);

	/* Get the gamma value of the random distribution */
	if ((attr = get_attr_integer(cmd, FSA_RANDGAMMA)))
		rndp->rnd_gamma = attr->attr_avd;
	else
		rndp->rnd_gamma = avd_int_alloc(1500);

	/* Get the mean value of the random distribution */
	if ((attr = get_attr_integer(cmd, FSA_RANDMEAN))) {
		rndp->rnd_mean = attr->attr_avd;
	} else if ((rndp->rnd_type & RAND_TYPE_MASK) == RAND_TYPE_GAMMA) {
		rndp->rnd_mean = NULL;
	} else {
		rndp->rnd_mean = avd_int_alloc(0);
	}
}

/*
 * Set a specified random distribution parameter in a random variable.
 */
static void
parser_randvar_set(cmd_t *cmd)
{
	var_t		*randvar;
	randdist_t	*rndp;
	avd_t	value;

	if ((randvar = var_find_randvar(cmd->cmd_tgt1)) == NULL) {
		filebench_log(LOG_ERROR,
		    "set randvar: failed",
		    cmd->cmd_tgt1);
		return;
	}

	rndp = randvar->var_val.randptr;
	value = cmd->cmd_attr_list->attr_avd;

	switch (cmd->cmd_qty) {
	case FSS_TYPE:
		{
			int disttype = (int)avd_get_int(value);

			rndp->rnd_type &= (~RAND_TYPE_MASK);

			switch (disttype) {
			case FSV_RANDUNI:
				rndp->rnd_type |= RAND_TYPE_UNIFORM;
				break;
			case FSA_RANDGAMMA:
				rndp->rnd_type |= RAND_TYPE_GAMMA;
				break;
			case FSV_RANDTAB:
				rndp->rnd_type |= RAND_TYPE_TABLE;
				break;
			}
			break;
		}

	case FSS_SRC:
		{
			int randsrc = (int)avd_get_int(value);

			rndp->rnd_type &=
			    (~(RAND_SRC_URANDOM | RAND_SRC_GENERATOR));

			switch (randsrc) {
			case FSV_URAND:
				rndp->rnd_type |= RAND_SRC_URANDOM;
				break;
			case FSV_RAND48:
				rndp->rnd_type |= RAND_SRC_GENERATOR;
				break;
			}
			break;
		}

	case FSS_SEED:
		rndp->rnd_seed = value;
		break;

	case FSS_GAMMA:
		rndp->rnd_gamma = value;
		break;

	case FSS_MEAN:
		rndp->rnd_mean = value;
		break;

	case FSS_MIN:
		rndp->rnd_min = value;
		break;

	case FSS_ROUND:
		rndp->rnd_round = value;
		break;

	default:
		filebench_log(LOG_ERROR, "setrandvar: undefined attribute");
	}
}

/*
 * alloc_cmd() allocates the required resources for a cmd_t. On failure, a
 * filebench_log is issued and NULL is returned.
 */
static cmd_t *
alloc_cmd(void)
{
	cmd_t *cmd;

	if ((cmd = malloc(sizeof (cmd_t))) == NULL) {
		filebench_log(LOG_ERROR, "Alloc cmd failed");
		return (NULL);
	}

	(void) memset(cmd, 0, sizeof (cmd_t));

	return (cmd);
}

/*
 * Allocates an attr_t structure and zeros it. Returns NULL on failure, or
 * a pointer to the attr_t.
 */
static attr_t *
alloc_attr(void)
{
	attr_t *attr;

	if ((attr = malloc(sizeof (attr_t))) == NULL) {
		return (NULL);
	}

	(void) memset(attr, 0, sizeof (attr_t));
	return (attr);
}

/*
 * Allocates a probtabent_t structure and zeros it. Returns NULL on failure, or
 * a pointer to the probtabent_t.
 */
static probtabent_t *
alloc_probtabent(void)
{
	probtabent_t *rte;

	if ((rte = malloc(sizeof (probtabent_t))) == NULL) {
		return (NULL);
	}

	(void) memset(rte, 0, sizeof (probtabent_t));
	return (rte);
}

/*
 * Allocates an attr_t structure and puts the supplied var_t into
 * its attr_avd location, and sets its name to FSA_LVAR_ASSIGN
 */
static attr_t *
alloc_lvar_attr(var_t *var)
{
	attr_t *attr;

	if ((attr = alloc_attr()) == NULL)
		return (NULL);

	attr->attr_name = FSA_LVAR_ASSIGN;
	attr->attr_avd = (avd_t)var;

	return (attr);
}


/*
 * Searches the attribute list for the command for the named attribute type.
 * The attribute list is created by the parser from the list of attributes
 * supplied with certain commands, such as the define and flowop commands.
 * Returns a pointer to the attribute structure if the named attribute is
 * found, otherwise returns NULL. If the attribute includes a parameter list,
 * the list is converted to a string and stored in the attr_avd field of
 * the returned attr_t struct.
 */
static attr_t *
get_attr_fileset(cmd_t *cmd, int64_t name)
{
	attr_t *attr;
	attr_t *rtn = NULL;

	for (attr = cmd->cmd_attr_list; attr != NULL;
	    attr = attr->attr_next) {
		filebench_log(LOG_DEBUG_IMPL,
		    "attr %d = %d %llx?",
		    attr->attr_name,
		    name,
		    attr->attr_avd);

		if (attr->attr_name == name)
			rtn = attr;
	}

	if (rtn == NULL)
		return (NULL);

	if (rtn->attr_param_list) {
		filebench_log(LOG_DEBUG_SCRIPT, "attr is param list");
		rtn->attr_avd = parser_list2varstring(rtn->attr_param_list);
	}

	return (rtn);
}


/*
 * Searches the attribute list for the command for the named attribute type.
 * The attribute list is created by the parser from the list of attributes
 * supplied with certain commands, such as the define and flowop commands.
 * Returns a pointer to the attribute structure if the named attribute is
 * found, otherwise returns NULL. If the attribute includes a parameter list,
 * the list is converted to a string and stored in the attr_avd field of
 * the returned attr_t struct.
 */
static attr_t *
get_attr(cmd_t *cmd, int64_t name)
{
	attr_t *attr;
	attr_t *rtn = NULL;
	char *string;

	for (attr = cmd->cmd_attr_list; attr != NULL;
	    attr = attr->attr_next) {
		filebench_log(LOG_DEBUG_IMPL,
		    "attr %d = %d %llx?",
		    attr->attr_name,
		    name,
		    attr->attr_avd);

		if (attr->attr_name == name)
			rtn = attr;
	}

	if (rtn == NULL)
		return (NULL);

	if (rtn->attr_param_list) {
		filebench_log(LOG_DEBUG_SCRIPT, "attr is param list");
		string = parser_list2string(rtn->attr_param_list);
		if (string != NULL) {
			rtn->attr_avd = avd_str_alloc(string);
			filebench_log(LOG_DEBUG_SCRIPT,
			    "attr string %s", string);
		}
	}

	return (rtn);
}

/*
 * Similar to get_attr, but converts the parameter string supplied with the
 * named attribute to an integer and stores the integer in the attr_avd
 * portion of the returned attr_t struct.
 */
static attr_t *
get_attr_integer(cmd_t *cmd, int64_t name)
{
	attr_t *attr;
	attr_t *rtn = NULL;

	for (attr = cmd->cmd_attr_list; attr != NULL;
	    attr = attr->attr_next) {
		if (attr->attr_name == name)
			rtn = attr;
	}

	if (rtn == NULL)
		return (NULL);

	if (rtn->attr_param_list)
		rtn->attr_avd = parser_list2avd(rtn->attr_param_list);

	return (rtn);
}

/*
 * Similar to get_attr, but converts the parameter string supplied with the
 * named attribute to an integer and stores the integer in the attr_avd
 * portion of the returned attr_t struct. If no parameter string is supplied
 * then it defaults to TRUE (1).
 */
static attr_t *
get_attr_bool(cmd_t *cmd, int64_t name)
{
	attr_t *attr;
	attr_t *rtn = NULL;

	for (attr = cmd->cmd_attr_list; attr != NULL;
	    attr = attr->attr_next) {
		if (attr->attr_name == name)
			rtn = attr;
	}

	if (rtn == NULL)
		return (NULL);

	if (rtn->attr_param_list) {
		rtn->attr_avd = parser_list2avd(rtn->attr_param_list);

	} else if (rtn->attr_avd == NULL) {
		rtn->attr_avd = avd_bool_alloc(TRUE);
	}

	/* boolean attributes cannot point to random variables */
	if (AVD_IS_RANDOM(rtn->attr_avd)) {
		filebench_log(LOG_ERROR,
		    "define flowop: Boolean attr %s cannot be random", name);
		filebench_shutdown(1);
		return (NULL);
	}

	return (rtn);
}

/*
 * removes the newly allocated local var from the shared local var
 * list, then puts it at the head of the private local var list
 * supplied as the second argument.
 */
static void
add_lvar_to_list(var_t *newlvar, var_t **lvar_list)
{
	var_t *prev;

	/* remove from shared local list, if there */
	if (newlvar == filebench_shm->shm_var_loc_list) {
		/* on top of list, just grap */
		filebench_shm->shm_var_loc_list = newlvar->var_next;
	} else {
		/* find newvar on list and remove */
		for (prev = filebench_shm->shm_var_loc_list; prev;
		    prev = prev->var_next) {
			if (prev->var_next == newlvar)
				prev->var_next = newlvar->var_next;
		}
	}
	newlvar->var_next = NULL;

	/* add to flowop private local list at head */
	newlvar->var_next = *lvar_list;
	*lvar_list = newlvar;
}

/*
 * Searches the attribute list for the command for any allocated local
 * variables. The attribute list is created by the parser from the list of
 * attributes supplied with certain commands, such as the define and flowop
 * commands. Places all found local vars onto the flowop's local variable
 * list. 
 */
static void
get_attr_lvars(cmd_t *cmd, flowop_t *flowop)
{
	attr_t *attr;
	var_t *orig_lvar_list;

	/* save the local var list */
	orig_lvar_list = flowop->fo_lvar_list;

	for (attr = cmd->cmd_attr_list; attr != NULL;
	    attr = attr->attr_next) {

		if (attr->attr_name == FSA_LVAR_ASSIGN) {
			var_t *newvar;

			if ((newvar = (var_t *)attr->attr_avd) == NULL)
				continue;

			add_lvar_to_list(newvar, &flowop->fo_lvar_list);
			var_update_comp_lvars(newvar, orig_lvar_list, NULL);
		}
	}
}

/*
 * Allocates memory for a list_t structure, initializes it to zero, and
 * returns a pointer to it. On failure, returns NULL.
 */
static list_t *
alloc_list()
{
	list_t *list;

	if ((list = malloc(sizeof (list_t))) == NULL) {
		return (NULL);
	}

	(void) memset(list, 0, sizeof (list_t));
	return (list);
}

int
yywrap()
{
	if (parentscript) {
		yyin = parentscript;
		yy_switchfilescript(yyin);
		parentscript = NULL;
		return (0);
	} else
		return (1);
}
