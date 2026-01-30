/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     UNSIGNED_INTEGER = 258,
     REAL = 259,
     STRING = 260,
     HEX_STRING = 261,
     IDENTIFIER = 262,
     CHAR = 263,
     START_DC_FILE = 264,
     START_DC_VALUE = 265,
     KW_DCLASS = 266,
     KW_STRUCT = 267,
     KW_FROM = 268,
     KW_IMPORT = 269,
     KW_TYPEDEF = 270,
     KW_KEYWORD = 271,
     KW_INT8 = 272,
     KW_INT16 = 273,
     KW_INT32 = 274,
     KW_INT64 = 275,
     KW_UINT8 = 276,
     KW_UINT16 = 277,
     KW_UINT32 = 278,
     KW_UINT64 = 279,
     KW_FLOAT32 = 280,
     KW_FLOAT64 = 281,
     KW_STRING = 282,
     KW_BLOB = 283,
     KW_CHAR = 284
   };
#endif
/* Tokens.  */
#define UNSIGNED_INTEGER 258
#define REAL 259
#define STRING 260
#define HEX_STRING 261
#define IDENTIFIER 262
#define CHAR 263
#define START_DC_FILE 264
#define START_DC_VALUE 265
#define KW_DCLASS 266
#define KW_STRUCT 267
#define KW_FROM 268
#define KW_IMPORT 269
#define KW_TYPEDEF 270
#define KW_KEYWORD 271
#define KW_INT8 272
#define KW_INT16 273
#define KW_INT32 274
#define KW_INT64 275
#define KW_UINT8 276
#define KW_UINT16 277
#define KW_UINT32 278
#define KW_UINT64 279
#define KW_FLOAT32 280
#define KW_FLOAT64 281
#define KW_STRING 282
#define KW_BLOB 283
#define KW_CHAR 284




/* Copy the first part of user declarations.  */
#line 4 "parser.ypp"

	#include "file/lexerDefs.h"
	#include "file/parserDefs.h"
	#include "file/parser.h"
	#include "file/write.h" // format_type(Type);

	#include "dc/File.h"
	#include "dc/DistributedType.h"
	#include "dc/NumericRange.h"
	#include "dc/NumericType.h"
	#include "dc/ArrayType.h"
	#include "dc/Struct.h"
	#include "dc/Class.h"
	#include "dc/Field.h"
	#include "dc/Method.h"
	#include "dc/Parameter.h"
	#include "dc/MolecularField.h"

	#include "util/byteorder.h"

	#include <stdint.h> // Fixed width integer limits
	#include <math.h>   // Float INFINITY
	#include <stack>    // std::stack

	#define yyparse run_parser
	#define yylex run_lexer
	#define yyerror parser_error
	#define yywarning parser_warning
	#define yychar dcyychar
	#define yydebug dcyydebug
	#define yynerrs dcyynerrs

	// Because our token type contains objects of type string, which
	// require correct copy construction (and not simply memcpying), we
	// cannot use bison's built-in auto-stack-grow feature.  As an easy
	// solution, we ensure here that we have enough yacc stack to start
	// with, and that it doesn't ever try to grow.
	#define YYINITDEPTH 1000
	#define YYMAXDEPTH 1000

	using namespace std;
	namespace dclass   // open namespace dclass
	{


	// Parser output
	static File* parsed_file = nullptr;
	static string* parsed_value = nullptr;

	// Parser state
	static Class* current_class = nullptr;
	static Struct* current_struct = nullptr;

	// Stack of distributed types for parsing values
	struct TypeAndDepth
	{
		int depth;
		const DistributedType* type;
		TypeAndDepth(const DistributedType* t, int d) : depth(d), type(t) {}
	};
	static stack<TypeAndDepth> type_stack;
	static int current_depth;

	// These two types are really common types the parser doesn't need to make new
	//     duplicates of every time a string or blob is used.
	static ArrayType* basic_string = nullptr;
	static ArrayType* basic_blob = nullptr;

	/* Helper functions */
	static bool check_depth();
	static void depth_error(string what);
	static void depth_error(int depth, string what);
	static string number_value(Type type, double &number);
	static string number_value(Type type, int64_t &number);
	static string number_value(Type type, uint64_t &number);

////////////////////////////////////////////////////////////////////
// Defining the interface to the parser.
////////////////////////////////////////////////////////////////////

	void init_file_parser(istream& in, const string& filename, File& file)
	{
		parsed_file = &file;
		init_file_lexer(in, filename);
	}

	void init_value_parser(istream& in, const string& source,
	                       const DistributedType* type, string& output)
	{
		parsed_value = &output;
		current_depth = 0;
		type_stack.push(TypeAndDepth(type, 0));
		init_value_lexer(in, source);
	}

	void cleanup_parser()
	{
		current_depth = 0;
		type_stack = stack<TypeAndDepth>();
		parsed_file = nullptr;
		parsed_value = nullptr;
	}

	int parser_error_count()
	{
		return lexer_error_count();
	}
	int parser_warning_count()
	{
		return lexer_warning_count();
	}

	void parser_error(const string &msg)
	{
		lexer_error(msg);
	}
	void parser_warning(const string &msg)
	{
		lexer_warning(msg);
	}


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 286 "parser.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  23
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   289

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  46
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  79
/* YYNRULES -- Number of rules.  */
#define YYNRULES  171
/* YYNRULES -- Number of states.  */
#define YYNSTATES  266

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   284

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    43,     2,     2,
      41,    42,    32,    45,    33,    44,    31,    34,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    39,    30,
       2,    40,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    35,     2,    36,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    37,     2,    38,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     6,     9,    11,    14,    17,    20,    23,
      26,    29,    31,    33,    35,    38,    43,    45,    49,    51,
      53,    55,    59,    61,    65,    67,    70,    75,    78,    80,
      83,    85,    88,    89,    97,    99,   102,   104,   108,   110,
     112,   115,   119,   122,   124,   125,   132,   134,   137,   141,
     143,   145,   147,   149,   151,   153,   155,   156,   161,   163,
     168,   173,   174,   179,   180,   185,   186,   192,   195,   197,
     199,   201,   203,   205,   210,   215,   220,   225,   229,   233,
     235,   237,   242,   244,   246,   248,   250,   252,   257,   262,
     267,   271,   275,   279,   282,   285,   288,   292,   294,   296,
     298,   300,   301,   306,   309,   314,   319,   320,   325,   326,
     331,   333,   335,   339,   341,   343,   347,   349,   351,   353,
     355,   357,   359,   361,   363,   365,   367,   369,   371,   373,
     375,   377,   379,   380,   385,   387,   391,   392,   397,   399,
     401,   405,   409,   412,   413,   418,   420,   421,   426,   428,
     432,   436,   440,   444,   448,   451,   454,   456,   458,   460,
     462,   464,   466,   468,   470,   472,   474,   476,   478,   480,
     482,   485
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      47,     0,    -1,     9,    48,    -1,    10,    49,    -1,   124,
      -1,    48,    30,    -1,    48,    61,    -1,    48,    68,    -1,
      48,    50,    -1,    48,    55,    -1,    48,    59,    -1,   124,
      -1,   108,    -1,   109,    -1,    14,    51,    -1,    13,    51,
      14,    52,    -1,    54,    -1,    51,    31,    54,    -1,    53,
      -1,    32,    -1,    54,    -1,    53,    33,    54,    -1,     7,
      -1,    54,    34,     7,    -1,    56,    -1,    15,    57,    -1,
      56,    35,   103,    36,    -1,    82,     7,    -1,     7,    -1,
      16,    60,    -1,   124,    -1,    60,     7,    -1,    -1,    11,
       7,    62,    63,    37,    66,    38,    -1,   124,    -1,    39,
      64,    -1,    65,    -1,    64,    33,    65,    -1,     7,    -1,
     124,    -1,    66,    30,    -1,    66,    67,    30,    -1,    72,
     123,    -1,    85,    -1,    -1,    12,     7,    69,    37,    70,
      38,    -1,   124,    -1,    70,    30,    -1,    70,    71,    30,
      -1,    72,    -1,    73,    -1,    75,    -1,    76,    -1,    77,
      -1,    81,    -1,    82,    -1,    -1,    82,    40,    74,   108,
      -1,    57,    -1,    75,    35,   103,    36,    -1,    76,    35,
     103,    36,    -1,    -1,    75,    40,    78,   108,    -1,    -1,
      76,    40,    79,   108,    -1,    -1,    81,    40,   109,    80,
     108,    -1,     7,    93,    -1,    83,    -1,    84,    -1,    58,
      -1,    88,    -1,    87,    -1,    88,    35,   103,    36,    -1,
      58,    35,   103,    36,    -1,    87,    35,   103,    36,    -1,
      84,    35,   103,    36,    -1,     7,    39,    86,    -1,    85,
      33,    86,    -1,     7,    -1,   121,    -1,   121,    41,   103,
      42,    -1,    89,    -1,    91,    -1,    92,    -1,    90,    -1,
     122,    -1,    89,    41,   102,    42,    -1,    91,    41,   102,
      42,    -1,    92,    41,   102,    42,    -1,    89,    43,   106,
      -1,    89,    34,   105,    -1,    91,    34,   105,    -1,    41,
      42,    -1,    94,    42,    -1,    41,    95,    -1,    94,    33,
      95,    -1,    97,    -1,    98,    -1,    99,    -1,    82,    -1,
      -1,    82,    40,    96,   108,    -1,    83,     7,    -1,    97,
      35,   103,    36,    -1,    98,    35,   103,    36,    -1,    -1,
      97,    40,   100,   108,    -1,    -1,    98,    40,   101,   108,
      -1,   124,    -1,   107,    -1,   107,    44,   107,    -1,   124,
      -1,   104,    -1,   104,    44,   104,    -1,     8,    -1,   105,
      -1,     3,    -1,     3,    -1,   120,    -1,     4,    -1,     8,
      -1,   106,    -1,   120,    -1,     3,    -1,     4,    -1,     8,
      -1,     5,    -1,     6,    -1,   115,    -1,   112,    -1,    -1,
      41,   110,   111,    42,    -1,   108,    -1,   111,    33,   108,
      -1,    -1,    37,   113,   114,    38,    -1,   108,    -1,   109,
      -1,   114,    33,   108,    -1,   114,    33,   109,    -1,    35,
      36,    -1,    -1,    35,   116,   117,    36,    -1,   119,    -1,
      -1,   117,    33,   118,   119,    -1,   108,    -1,   120,    32,
     105,    -1,     3,    32,   105,    -1,     4,    32,   105,    -1,
       5,    32,   105,    -1,     6,    32,   105,    -1,    45,     3,
      -1,    44,     3,    -1,    27,    -1,    28,    -1,    29,    -1,
      17,    -1,    18,    -1,    19,    -1,    20,    -1,    21,    -1,
      22,    -1,    23,    -1,    24,    -1,    25,    -1,    26,    -1,
     124,    -1,   123,     7,    -1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   229,   229,   230,   235,   236,   237,   238,   239,   240,
     244,   248,   252,   257,   265,   270,   279,   280,   287,   288,
     295,   299,   307,   308,   315,   360,   364,   372,   382,   397,
     401,   402,   410,   409,   449,   450,   454,   461,   471,   502,
     503,   504,   542,   566,   574,   573,   613,   614,   615,   649,
     650,   654,   655,   656,   657,   661,   666,   665,   680,   687,
     692,   701,   700,   712,   711,   723,   722,   736,   743,   744,
     748,   766,   770,   774,   778,   782,   786,   793,   819,   851,
     873,   901,   924,   925,   926,   927,   931,   935,   944,   953,
     965,   977,   984,   994,   998,  1005,  1015,  1028,  1029,  1030,
    1031,  1036,  1035,  1049,  1056,  1061,  1070,  1069,  1081,  1080,
    1094,  1095,  1096,  1100,  1101,  1102,  1106,  1118,  1122,  1135,
    1136,  1137,  1141,  1153,  1157,  1172,  1187,  1202,  1230,  1267,
    1301,  1302,  1307,  1306,  1348,  1349,  1357,  1356,  1398,  1399,
    1400,  1404,  1411,  1456,  1455,  1528,  1530,  1529,  1550,  1555,
    1577,  1599,  1621,  1677,  1737,  1738,  1742,  1743,  1747,  1748,
    1749,  1750,  1751,  1752,  1753,  1754,  1755,  1756,  1757,  1761,
    1765,  1778
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "UNSIGNED_INTEGER", "REAL", "STRING",
  "HEX_STRING", "IDENTIFIER", "CHAR", "START_DC_FILE", "START_DC_VALUE",
  "KW_DCLASS", "KW_STRUCT", "KW_FROM", "KW_IMPORT", "KW_TYPEDEF",
  "KW_KEYWORD", "KW_INT8", "KW_INT16", "KW_INT32", "KW_INT64", "KW_UINT8",
  "KW_UINT16", "KW_UINT32", "KW_UINT64", "KW_FLOAT32", "KW_FLOAT64",
  "KW_STRING", "KW_BLOB", "KW_CHAR", "';'", "'.'", "'*'", "','", "'/'",
  "'['", "']'", "'{'", "'}'", "':'", "'='", "'('", "')'", "'%'", "'-'",
  "'+'", "$accept", "grammar", "file", "value", "import", "import_module",
  "import_symbols", "import_symbol_list", "import_alternatives", "typedef",
  "typedef_type", "nonmethod_type_with_name", "defined_type",
  "keyword_decl", "keyword_decl_list", "dclass", "@1", "class_inheritance",
  "class_parents", "defined_class", "class_fields", "class_field",
  "dstruct", "@2", "struct_fields", "struct_field", "named_field",
  "unnamed_field", "@3", "field_with_name", "field_with_name_as_array",
  "field_with_name_and_default", "@4", "@5", "@6", "method_as_field",
  "nonmethod_type", "nonmethod_type_no_array", "type_with_array",
  "molecular", "defined_field", "builtin_array_type", "numeric_type",
  "numeric_token_only", "numeric_with_range", "numeric_with_modulus",
  "numeric_with_divisor", "method", "method_body", "parameter", "@7",
  "param_with_name", "param_with_name_as_array",
  "param_with_name_and_default", "@8", "@9", "numeric_range",
  "array_range", "char_or_uint", "small_unsigned_integer", "number",
  "char_or_number", "type_value", "method_value", "@10",
  "parameter_values", "struct_value", "@11", "field_values", "array_value",
  "@12", "element_values", "@13", "array_expansion", "signed_integer",
  "array_type_token", "numeric_type_token", "keyword_list", "empty", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
      59,    46,    42,    44,    47,    91,    93,   123,   125,    58,
      61,    40,    41,    37,    45,    43
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    46,    47,    47,    48,    48,    48,    48,    48,    48,
      48,    49,    49,    49,    50,    50,    51,    51,    52,    52,
      53,    53,    54,    54,    55,    56,    56,    57,    58,    59,
      60,    60,    62,    61,    63,    63,    64,    64,    65,    66,
      66,    66,    67,    67,    69,    68,    70,    70,    70,    71,
      71,    72,    72,    72,    72,    73,    74,    73,    75,    76,
      76,    78,    77,    79,    77,    80,    77,    81,    82,    82,
      83,    83,    83,    84,    84,    84,    84,    85,    85,    86,
      87,    87,    88,    88,    88,    88,    89,    90,    90,    90,
      91,    92,    92,    93,    93,    94,    94,    95,    95,    95,
      95,    96,    95,    97,    98,    98,   100,    99,   101,    99,
     102,   102,   102,   103,   103,   103,   104,   104,   105,   106,
     106,   106,   107,   107,   108,   108,   108,   108,   108,   108,
     108,   108,   110,   109,   111,   111,   113,   112,   114,   114,
     114,   114,   115,   116,   115,   117,   118,   117,   119,   119,
     119,   119,   119,   119,   120,   120,   121,   121,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   123,
     123,   124
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     1,     2,     2,     2,     2,     2,
       2,     1,     1,     1,     2,     4,     1,     3,     1,     1,
       1,     3,     1,     3,     1,     2,     4,     2,     1,     2,
       1,     2,     0,     7,     1,     2,     1,     3,     1,     1,
       2,     3,     2,     1,     0,     6,     1,     2,     3,     1,
       1,     1,     1,     1,     1,     1,     0,     4,     1,     4,
       4,     0,     4,     0,     4,     0,     5,     2,     1,     1,
       1,     1,     1,     4,     4,     4,     4,     3,     3,     1,
       1,     4,     1,     1,     1,     1,     1,     4,     4,     4,
       3,     3,     3,     2,     2,     2,     3,     1,     1,     1,
       1,     0,     4,     2,     4,     4,     0,     4,     0,     4,
       1,     1,     3,     1,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     4,     1,     3,     0,     4,     1,     1,
       3,     3,     2,     0,     4,     1,     0,     4,     1,     3,
       3,     3,     3,     3,     2,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     0
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,   171,   171,     0,     2,     4,   125,   126,   128,   129,
     127,   143,   136,   132,     0,     0,     3,    12,    13,   131,
     130,   124,    11,     1,     0,     0,     0,     0,     0,   171,
       5,     8,     9,    24,    10,     6,     7,   142,     0,     0,
       0,   155,   154,    32,    44,    22,     0,    16,    14,    28,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     156,   157,   158,    25,    70,     0,    68,    69,    72,    71,
      82,    85,    83,    84,    80,    86,    29,    30,   171,   125,
     126,   128,   129,   148,     0,   145,   124,   138,   139,     0,
     134,     0,   171,     0,     0,     0,     0,   171,    27,   171,
     171,   171,     0,   171,     0,     0,   171,   171,   171,    31,
     118,   116,     0,   114,   117,   113,     0,     0,     0,     0,
     146,   144,     0,     0,   137,     0,   133,     0,     0,    34,
     171,    19,    15,    18,    20,    17,    23,     0,     0,     0,
       0,    91,   119,   121,   122,     0,   123,   111,   120,   110,
      90,    92,     0,     0,     0,    26,     0,   150,   151,   152,
     153,     0,   149,   140,   141,   135,    38,    35,    36,   171,
       0,    46,     0,    74,    76,    75,    73,    87,     0,    88,
      89,    81,   115,   147,     0,     0,    39,    28,    47,    45,
      58,     0,    49,    50,    51,    52,    53,    54,    55,    21,
     112,    37,    28,    40,    33,     0,   171,    43,     0,    67,
       0,    48,   171,    61,   171,    63,     0,    56,     0,    41,
      42,   169,     0,    93,   100,    68,    95,    97,    98,    99,
       0,    94,     0,     0,     0,     0,    65,     0,    79,    77,
     170,    78,   101,   103,   171,   106,   171,   108,    96,    59,
      62,    60,    64,     0,    57,     0,     0,     0,     0,     0,
      66,   102,   104,   107,   105,   109
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,    16,    31,    46,   132,   133,    47,    32,
      33,   190,    64,    34,    76,    35,    92,   128,   167,   168,
     185,   205,    36,    93,   170,   191,   192,   193,   237,   194,
     195,   196,   233,   235,   253,   197,    65,    66,    67,   207,
     239,    68,    69,    70,    71,    72,    73,   209,   210,   226,
     255,   227,   228,   229,   257,   259,   145,   112,   113,   114,
     146,   147,    83,    18,    40,    91,    19,    39,    89,    20,
      38,    84,   161,    85,    21,    74,    75,   220,   115
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -188
static const yytype_int16 yypact[] =
{
      76,  -188,    14,    31,   259,  -188,  -188,  -188,  -188,  -188,
    -188,    24,  -188,  -188,    68,    71,  -188,  -188,  -188,  -188,
    -188,  -188,  -188,  -188,    45,    83,    84,    84,   219,  -188,
    -188,  -188,  -188,    58,  -188,  -188,  -188,  -188,    73,    14,
      91,  -188,  -188,  -188,  -188,  -188,    11,    64,    69,  -188,
    -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,
    -188,  -188,  -188,  -188,    66,    95,  -188,    70,    74,    77,
      -8,  -188,    16,    63,    72,  -188,   104,  -188,    38,    82,
      87,    92,    93,  -188,    12,  -188,    97,  -188,  -188,    23,
    -188,    -6,    94,    78,     8,    84,   109,    38,  -188,    38,
      38,    38,   128,    20,     9,   128,    20,    20,    38,  -188,
    -188,  -188,    96,    90,  -188,  -188,   128,   128,   128,   128,
    -188,  -188,   128,    14,  -188,    91,  -188,   130,   101,  -188,
    -188,  -188,  -188,   106,    64,    64,  -188,   105,   107,   111,
     112,  -188,  -188,  -188,  -188,    98,  -188,   126,  -188,  -188,
    -188,  -188,   100,   103,   108,  -188,    38,  -188,  -188,  -188,
    -188,    73,  -188,  -188,  -188,  -188,  -188,   116,  -188,  -188,
     165,  -188,    84,  -188,  -188,  -188,  -188,  -188,    20,  -188,
    -188,  -188,  -188,  -188,   130,   190,  -188,   110,  -188,  -188,
    -188,   123,  -188,  -188,    27,    28,  -188,   115,     7,    64,
    -188,  -188,    -7,  -188,  -188,   141,  -188,   140,   139,  -188,
      -3,  -188,    38,  -188,    38,  -188,   133,  -188,   168,  -188,
     169,  -188,   168,  -188,   137,   171,  -188,    40,    49,  -188,
     219,  -188,   143,    91,   160,    91,  -188,    91,  -188,  -188,
    -188,  -188,  -188,  -188,    38,  -188,    38,  -188,  -188,  -188,
    -188,  -188,  -188,    91,  -188,    91,   162,    91,   163,    91,
    -188,  -188,  -188,  -188,  -188,  -188
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -188,  -188,  -188,  -188,  -188,   173,  -188,  -188,   -84,  -188,
    -188,   174,  -188,  -188,  -188,  -188,  -188,  -188,  -188,    17,
    -188,  -188,  -188,  -188,  -188,  -188,    19,  -188,  -188,  -188,
    -188,  -188,  -188,  -188,  -188,  -188,  -164,  -187,  -188,  -188,
     -17,  -188,  -188,  -188,  -188,  -188,  -188,  -188,  -188,    -9,
    -188,  -188,  -188,  -188,  -188,  -188,   -24,   -92,    67,   147,
     118,    46,    -2,   -36,  -188,  -188,  -188,  -188,  -188,  -188,
    -188,  -188,  -188,    89,   -34,  -188,  -188,  -188,     0
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint16 yytable[] =
{
      17,     5,    22,    88,    86,   137,   198,   138,   139,   140,
     134,   135,   142,   143,    98,    45,   154,     6,     7,     8,
       9,   225,    10,   142,   143,    94,   102,   125,   144,    77,
     230,    23,   218,   103,   208,   104,   126,    87,    90,   231,
     131,   110,    95,   225,   224,   120,   111,   217,   121,    11,
     105,    12,    43,    14,    15,    13,   123,   106,    14,    15,
      37,   124,   212,   214,    14,    15,   224,   213,   215,   148,
     148,    41,   148,   148,    42,   244,    79,    80,    81,    82,
     245,    10,   152,   153,   246,     1,     2,   164,   199,   247,
      44,    45,   129,    78,     6,     7,     8,     9,    96,    10,
      95,    97,    98,   149,   107,    99,   149,   149,    11,   100,
      12,   109,   101,   108,   116,   130,   136,    14,    15,   117,
     232,   163,   234,   165,   118,   119,    11,    86,    12,   122,
     171,   110,   155,   127,   156,    14,    15,   166,   169,   172,
     177,   173,   179,   174,   148,   180,    49,   175,   176,   184,
     181,   208,   256,   211,   258,   216,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,   186,
     178,   219,   187,   222,    13,   238,   240,   242,   243,   249,
     236,   223,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,   188,   251,   202,   262,   264,
      48,   201,    63,   189,   206,   241,   221,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
     203,   248,   150,   182,   200,     0,    49,     0,   204,     0,
       0,   250,     0,   252,     0,   254,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,   141,
     183,   260,   151,   261,     0,   263,     0,   265,     0,     0,
       0,     0,     0,   157,   158,   159,   160,     0,     0,   162,
      24,    25,    26,    27,    28,    29,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    30
};

static const yytype_int16 yycheck[] =
{
       2,     1,     2,    39,    38,    97,   170,    99,   100,   101,
      94,    95,     3,     4,     7,     7,   108,     3,     4,     5,
       6,   208,     8,     3,     4,    14,    34,    33,     8,    29,
      33,     0,    39,    41,    41,    43,    42,    39,    40,    42,
      32,     3,    31,   230,   208,    33,     8,    40,    36,    35,
      34,    37,     7,    44,    45,    41,    33,    41,    44,    45,
      36,    38,    35,    35,    44,    45,   230,    40,    40,   103,
     104,     3,   106,   107,     3,    35,     3,     4,     5,     6,
      40,     8,   106,   107,    35,     9,    10,   123,   172,    40,
       7,     7,    92,    35,     3,     4,     5,     6,    34,     8,
      31,    35,     7,   103,    41,    35,   106,   107,    35,    35,
      37,     7,    35,    41,    32,    37,     7,    44,    45,    32,
     212,   123,   214,   125,    32,    32,    35,   161,    37,    32,
     130,     3,    36,    39,    44,    44,    45,     7,    37,    33,
      42,    36,    42,    36,   178,    42,     7,    36,    36,    33,
      42,    41,   244,    30,   246,    40,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,   169,
      44,    30,     7,    33,    41,     7,     7,    40,     7,    36,
     216,    42,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    36,     7,    36,    36,
      27,   184,    28,    38,   185,   222,   206,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,   230,   104,   156,   178,    -1,     7,    -1,    38,    -1,
      -1,   233,    -1,   235,    -1,   237,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,   102,
     161,   253,   105,   255,    -1,   257,    -1,   259,    -1,    -1,
      -1,    -1,    -1,   116,   117,   118,   119,    -1,    -1,   122,
      11,    12,    13,    14,    15,    16,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    30
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     9,    10,    47,    48,   124,     3,     4,     5,     6,
       8,    35,    37,    41,    44,    45,    49,   108,   109,   112,
     115,   120,   124,     0,    11,    12,    13,    14,    15,    16,
      30,    50,    55,    56,    59,    61,    68,    36,   116,   113,
     110,     3,     3,     7,     7,     7,    51,    54,    51,     7,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    57,    58,    82,    83,    84,    87,    88,
      89,    90,    91,    92,   121,   122,    60,   124,    35,     3,
       4,     5,     6,   108,   117,   119,   120,   108,   109,   114,
     108,   111,    62,    69,    14,    31,    34,    35,     7,    35,
      35,    35,    34,    41,    43,    34,    41,    41,    41,     7,
       3,     8,   103,   104,   105,   124,    32,    32,    32,    32,
      33,    36,    32,    33,    38,    33,    42,    39,    63,   124,
      37,    32,    52,    53,    54,    54,     7,   103,   103,   103,
     103,   105,     3,     4,     8,   102,   106,   107,   120,   124,
     106,   105,   102,   102,   103,    36,    44,   105,   105,   105,
     105,   118,   105,   108,   109,   108,     7,    64,    65,    37,
      70,   124,    33,    36,    36,    36,    36,    42,    44,    42,
      42,    42,   104,   119,    33,    66,   124,     7,    30,    38,
      57,    71,    72,    73,    75,    76,    77,    81,    82,    54,
     107,    65,     7,    30,    38,    67,    72,    85,    41,    93,
      94,    30,    35,    40,    35,    40,    40,    40,    39,    30,
     123,   124,    33,    42,    82,    83,    95,    97,    98,    99,
      33,    42,   103,    78,   103,    79,   109,    74,     7,    86,
       7,    86,    40,     7,    35,    40,    35,    40,    95,    36,
     108,    36,   108,    80,   108,    96,   103,   100,   103,   101,
     108,   108,    36,   108,    36,   108
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

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
#ifndef	YYINITDEPTH
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
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
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
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
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
	    /* Fall through.  */
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

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 9:
#line 241 "parser.ypp"
    {

	;}
    break;

  case 11:
#line 249 "parser.ypp"
    {
		parsed_value->clear();
	;}
    break;

  case 12:
#line 253 "parser.ypp"
    {
		parsed_value->assign((yyvsp[(1) - (1)].str));
		if(!type_stack.empty()) depth_error(0, "type");
	;}
    break;

  case 13:
#line 258 "parser.ypp"
    {
		parsed_value->assign((yyvsp[(1) - (1)].str));
		if(!type_stack.empty()) depth_error(0, "method");
	;}
    break;

  case 14:
#line 266 "parser.ypp"
    {
		Import* import = new Import((yyvsp[(2) - (2)].str));
		parsed_file->add_import(import);
	;}
    break;

  case 15:
#line 271 "parser.ypp"
    {
		Import* import = new Import((yyvsp[(2) - (4)].str));
		import->symbols.assign((yyvsp[(4) - (4)].strings).begin(), (yyvsp[(4) - (4)].strings).end());
		parsed_file->add_import(import);
	;}
    break;

  case 16:
#line 279 "parser.ypp"
    { (yyval.str) = (yyvsp[(1) - (1)].str); ;}
    break;

  case 17:
#line 281 "parser.ypp"
    {
		(yyval.str) = (yyvsp[(1) - (3)].str) + string(".") + (yyvsp[(3) - (3)].str);
	;}
    break;

  case 18:
#line 287 "parser.ypp"
    { (yyval.strings) = (yyvsp[(1) - (1)].strings); ;}
    break;

  case 19:
#line 289 "parser.ypp"
    {
		(yyval.strings) = vector<string>();
	;}
    break;

  case 20:
#line 296 "parser.ypp"
    {
		(yyval.strings) = vector<string>(1, (yyvsp[(1) - (1)].str));
	;}
    break;

  case 21:
#line 300 "parser.ypp"
    {
		(yyvsp[(1) - (3)].strings).push_back((yyvsp[(3) - (3)].str));
		(yyval.strings) = (yyvsp[(1) - (3)].strings);
	;}
    break;

  case 22:
#line 307 "parser.ypp"
    { (yyval.str) = (yyvsp[(1) - (1)].str); ;}
    break;

  case 23:
#line 309 "parser.ypp"
    {
		(yyval.str) = (yyvsp[(1) - (3)].str) + string("/") + (yyvsp[(3) - (3)].str);
	;}
    break;

  case 24:
#line 316 "parser.ypp"
    {
		if((yyvsp[(1) - (1)].nametype).type == nullptr)
		{
			// Ignore this typedef, it should have already produced an error
			break;
		}

		// Set the type's typedef
		(yyvsp[(1) - (1)].nametype).type->set_alias((yyvsp[(1) - (1)].nametype).name);

		bool type_added	= parsed_file->add_typedef((yyvsp[(1) - (1)].nametype).name, (yyvsp[(1) - (1)].nametype).type);
		if(!type_added)
		{
			// Lets be really descriptive about why this failed
			DistributedType* dtype = parsed_file->get_type_by_name((yyvsp[(1) - (1)].nametype).name);
			if(dtype == nullptr)
			{
				parser_error("Unknown error adding typedef to file.");
				break;
			}

			Struct* dstruct = dtype->as_struct();
			if(dstruct == nullptr)
			{
				parser_error("Cannot add 'typedef " + (yyvsp[(1) - (1)].nametype).name
				             + "' to file because a typedef was already declared with that name.");
				break;
			}

			if(dstruct->as_class())
			{
				parser_error("Cannot add 'typedef " + (yyvsp[(1) - (1)].nametype).name
				             + "' to file because a class was already declared with that name.");
			}
			else
			{
				parser_error("Cannot add 'typedef " + (yyvsp[(1) - (1)].nametype).name
				             + "' to file because a struct was already declared with that name.");
			}
		}
	;}
    break;

  case 25:
#line 361 "parser.ypp"
    {
		(yyval.nametype) = (yyvsp[(2) - (2)].nametype);
	;}
    break;

  case 26:
#line 365 "parser.ypp"
    {
		(yyval.nametype) = (yyvsp[(1) - (4)].nametype);
		(yyval.nametype).type = new ArrayType((yyvsp[(1) - (4)].nametype).type, (yyvsp[(3) - (4)].range));
	;}
    break;

  case 27:
#line 373 "parser.ypp"
    {
		TokenType::NameType nt;
		nt.type = (yyvsp[(1) - (2)].u.dtype);
		nt.name = (yyvsp[(2) - (2)].str);
		(yyval.nametype) = nt;
	;}
    break;

  case 28:
#line 383 "parser.ypp"
    {
		DistributedType* dtype = parsed_file->get_type_by_name((yyvsp[(1) - (1)].str));
		if(dtype == nullptr)
		{
			parser_error("Type '" + string((yyvsp[(1) - (1)].str)) + "' has not been declared.");
			(yyval.u.dtype) = nullptr;
			break;
		}

		(yyval.u.dtype) = dtype;
	;}
    break;

  case 31:
#line 403 "parser.ypp"
    {
		parsed_file->add_keyword((yyvsp[(2) - (2)].str));
	;}
    break;

  case 32:
#line 410 "parser.ypp"
    {
		current_class = new Class(parsed_file, (yyvsp[(2) - (2)].str));
	;}
    break;

  case 33:
#line 414 "parser.ypp"
    {
		bool class_added = parsed_file->add_class(current_class);
		if(!class_added)
		{
			// Lets be really descriptive about why this failed
			DistributedType* dtype = parsed_file->get_type_by_name(current_class->get_name());
			if(dtype == nullptr)
			{
				parser_error("Unknown error adding class to file.");
				break;
			}

			Struct* dstruct = dtype->as_struct();
			if(dstruct == nullptr)
			{
				parser_error("Cannot add 'dclass " + current_class->get_name()
				             + "' to file because a typedef was already declared with that name.");
				break;
			}

			if(dstruct->as_class())
			{
				parser_error("Cannot add 'dclass " + current_class->get_name()
				             + "' to file because a class was already declared with that name.");
			}
			else
			{
				parser_error("Cannot add 'dclass " + current_class->get_name()
				             + "' to file because a struct was already declared with that name.");
			}
		}
	;}
    break;

  case 36:
#line 455 "parser.ypp"
    {
		if((yyvsp[(1) - (1)].u.dclass) != nullptr)
		{
			current_class->add_parent((yyvsp[(1) - (1)].u.dclass));
		}
	;}
    break;

  case 37:
#line 462 "parser.ypp"
    {
		if((yyvsp[(3) - (3)].u.dclass) != nullptr)
		{
			current_class->add_parent((yyvsp[(3) - (3)].u.dclass));
		}
	;}
    break;

  case 38:
#line 472 "parser.ypp"
    {
		DistributedType* dtype = parsed_file->get_type_by_name((yyvsp[(1) - (1)].str));
		if(dtype == nullptr)
		{
			parser_error("'dclass " + string((yyvsp[(1) - (1)].str)) + "' has not been declared.");
			(yyval.u.dclass) = nullptr;
			break;
		}

		Struct* dstruct = dtype->as_struct();
		if(dstruct == nullptr)
		{
			parser_error("class cannot inherit from non-class type '" + string((yyvsp[(1) - (1)].str)) + "'.");
			(yyval.u.dclass) = nullptr;
			break;
		}

		Class* dclass = dstruct->as_class();
		if(dclass == nullptr)
		{
			parser_error("class cannot inherit from struct type '" + string((yyvsp[(1) - (1)].str)) + "'.");
			(yyval.u.dclass) = nullptr;
			break;
		}

		(yyval.u.dclass) = dclass;
	;}
    break;

  case 41:
#line 505 "parser.ypp"
    {
		if((yyvsp[(2) - (3)].u.dfield) == nullptr)
		{
			// Ignore this field, it should have already generated a parser error
			break;
		}

		bool field_added = current_class->add_field((yyvsp[(2) - (3)].u.dfield));
		if(!field_added)
		{
			// Lets be really descriptive about why this failed
			if(current_class->get_field_by_name((yyvsp[(2) - (3)].u.dfield)->get_name()))
			{
				parser_error("Cannot add field '" + (yyvsp[(2) - (3)].u.dfield)->get_name()
				             + "', a field with that name already exists in 'dclass "
				             + current_class->get_name() + "'.");
			}
			else if(current_class->get_name() == (yyvsp[(2) - (3)].u.dfield)->get_name())
			{
				if((yyvsp[(2) - (3)].u.dfield)->as_molecular())
				{
					parser_error("Cannot use a molecular field as a constructor.");
				}
				else
				{
					parser_error("The constructor must be the first field in the class.");
				}
			}
			else
			{
				parser_error("Unknown error adding field to class.");
			}
		}
	;}
    break;

  case 42:
#line 543 "parser.ypp"
    {
		if((yyvsp[(1) - (2)].u.dfield) == nullptr)
		{
			// Ignore this field, it should have already generated a parser error
			(yyval.u.dfield) = nullptr;
			break;
		}

		if((yyvsp[(1) - (2)].u.dfield)->get_name().empty())
		{
			parser_error("An unnamed field can't be defined in a class.");
			(yyval.u.dfield) = nullptr;
			break;
		}

		// Add the keywords to the class
		for(auto it = (yyvsp[(2) - (2)].strings).begin(); it != (yyvsp[(2) - (2)].strings).end(); ++it)
		{
			(yyvsp[(1) - (2)].u.dfield)->add_keyword(*it);
		}

		(yyval.u.dfield) = (yyvsp[(1) - (2)].u.dfield);
	;}
    break;

  case 43:
#line 567 "parser.ypp"
    {
		(yyval.u.dfield) = (Field*)(yyvsp[(1) - (1)].u.dmolecule);
	;}
    break;

  case 44:
#line 574 "parser.ypp"
    {
		current_struct = new Struct(parsed_file, (yyvsp[(2) - (2)].str));
	;}
    break;

  case 45:
#line 578 "parser.ypp"
    {
		bool struct_added = parsed_file->add_struct(current_struct);
		if(!struct_added)
		{
			// Lets be really descriptive about why this failed
			DistributedType* dtype = parsed_file->get_type_by_name(current_struct->get_name());
			if(dtype == nullptr)
			{
				parser_error("Unknown error adding struct to file.");
				break;
			}

			Struct* dstruct = dtype->as_struct();
			if(dstruct == nullptr)
			{
				parser_error("Cannot add 'struct " + current_struct->get_name()
				             + "' to file because a typedef was already declared with that name.");
				break;
			}

			if(dstruct->as_class())
			{
				parser_error("Cannot add 'struct " + current_struct->get_name()
				             + "' to file because a class was already declared with that name.");
			}
			else
			{
				parser_error("Cannot add 'struct " + current_struct->get_name()
				             + "' to file because a struct was already declared with that name.");
			}
		}
	;}
    break;

  case 48:
#line 616 "parser.ypp"
    {
		if((yyvsp[(2) - (3)].u.dfield) == nullptr || (yyvsp[(2) - (3)].u.dfield)->get_type() == nullptr)
		{
			// Ignore this field, it should have already generated a parser error
			break;
		}

		if(!current_struct->add_field((yyvsp[(2) - (3)].u.dfield)))
		{
			// Lets be really descriptive about why this failed
			if(current_struct->get_field_by_name((yyvsp[(2) - (3)].u.dfield)->get_name()))
			{
				parser_error("Cannot add field '" + (yyvsp[(2) - (3)].u.dfield)->get_name()
				             + "', a field with that name already exists in 'struct "
				             + current_struct->get_name() + "'.");
			}
			else if(current_struct->get_name() == (yyvsp[(2) - (3)].u.dfield)->get_name())
			{
				parser_error("A constructor can't be defined in a struct.");
			}
			else if((yyvsp[(2) - (3)].u.dfield)->get_type()->as_method())
			{
				parser_error("A method can't be defined in a struct.");
			}
			else
			{
				parser_error("Unknown error adding field to struct.");
			}
		}
	;}
    break;

  case 55:
#line 662 "parser.ypp"
    {
		(yyval.u.dfield) = new Field((yyvsp[(1) - (1)].u.dtype));
	;}
    break;

  case 56:
#line 666 "parser.ypp"
    {
		current_depth = 0;
		type_stack.push(TypeAndDepth((yyvsp[(1) - (2)].u.dtype), 0));
	;}
    break;

  case 57:
#line 671 "parser.ypp"
    {
		Field* field = new Field((yyvsp[(1) - (4)].u.dtype));
		if(!type_stack.empty()) depth_error(0, "unnamed field");
		field->set_default_value((yyvsp[(4) - (4)].str));
		(yyval.u.dfield) = field;
	;}
    break;

  case 58:
#line 681 "parser.ypp"
    {
		(yyval.u.dfield) = new Field((yyvsp[(1) - (1)].nametype).type, (yyvsp[(1) - (1)].nametype).name);
	;}
    break;

  case 59:
#line 688 "parser.ypp"
    {
		(yyvsp[(1) - (4)].u.dfield)->set_type(new ArrayType((yyvsp[(1) - (4)].u.dfield)->get_type(), (yyvsp[(3) - (4)].range)));
		(yyval.u.dfield) = (yyvsp[(1) - (4)].u.dfield);
	;}
    break;

  case 60:
#line 693 "parser.ypp"
    {
		(yyvsp[(1) - (4)].u.dfield)->set_type(new ArrayType((yyvsp[(1) - (4)].u.dfield)->get_type(), (yyvsp[(3) - (4)].range)));
		(yyval.u.dfield) = (yyvsp[(1) - (4)].u.dfield);
	;}
    break;

  case 61:
#line 701 "parser.ypp"
    {
		current_depth = 0;
		type_stack.push(TypeAndDepth((yyvsp[(1) - (2)].u.dfield)->get_type(), 0));
	;}
    break;

  case 62:
#line 706 "parser.ypp"
    {
		if(!type_stack.empty()) depth_error(0, "field '" + (yyvsp[(1) - (4)].u.dfield)->get_name() + "'");
		(yyvsp[(1) - (4)].u.dfield)->set_default_value((yyvsp[(4) - (4)].str));
		(yyval.u.dfield) = (yyvsp[(1) - (4)].u.dfield);
	;}
    break;

  case 63:
#line 712 "parser.ypp"
    {
		current_depth = 0;
		type_stack.push(TypeAndDepth((yyvsp[(1) - (2)].u.dfield)->get_type(), 0));
	;}
    break;

  case 64:
#line 717 "parser.ypp"
    {
		if(!type_stack.empty()) depth_error(0, "field '" + (yyvsp[(1) - (4)].u.dfield)->get_name() + "'");
		(yyvsp[(1) - (4)].u.dfield)->set_default_value((yyvsp[(4) - (4)].str));
		(yyval.u.dfield) = (yyvsp[(1) - (4)].u.dfield);
	;}
    break;

  case 65:
#line 723 "parser.ypp"
    {
		current_depth = 0;
		type_stack.push(TypeAndDepth((yyvsp[(1) - (3)].u.dfield)->get_type(), 0));
	;}
    break;

  case 66:
#line 728 "parser.ypp"
    {
		if(!type_stack.empty()) depth_error(0, "method");
		(yyvsp[(1) - (5)].u.dfield)->set_default_value((yyvsp[(3) - (5)].str));
		(yyval.u.dfield) = (yyvsp[(1) - (5)].u.dfield);
	;}
    break;

  case 67:
#line 737 "parser.ypp"
    {
		(yyval.u.dfield) = new Field((yyvsp[(2) - (2)].u.dmethod), (yyvsp[(1) - (2)].str));
	;}
    break;

  case 70:
#line 749 "parser.ypp"
    {
		if((yyvsp[(1) - (1)].u.dtype) == nullptr)
		{
			// defined_type should have output an error, pass nullptr upstream
			(yyval.u.dtype) = nullptr;
			break;
		}

		if((yyvsp[(1) - (1)].u.dtype)->get_type() == T_METHOD)
		{
			parser_error("Cannot use a method type here.");
			(yyval.u.dtype) = nullptr;
			break;
		}

		(yyval.u.dtype) = (yyvsp[(1) - (1)].u.dtype);
	;}
    break;

  case 71:
#line 767 "parser.ypp"
    {
		(yyval.u.dtype) = (DistributedType*)(yyvsp[(1) - (1)].u.dnumeric);
	;}
    break;

  case 73:
#line 775 "parser.ypp"
    {
		(yyval.u.dtype) = new ArrayType((yyvsp[(1) - (4)].u.dnumeric), (yyvsp[(3) - (4)].range));
	;}
    break;

  case 74:
#line 779 "parser.ypp"
    {
		(yyval.u.dtype) = new ArrayType((yyvsp[(1) - (4)].u.dtype), (yyvsp[(3) - (4)].range));
	;}
    break;

  case 75:
#line 783 "parser.ypp"
    {
		(yyval.u.dtype) = new ArrayType((yyvsp[(1) - (4)].u.dtype), (yyvsp[(3) - (4)].range));
	;}
    break;

  case 76:
#line 787 "parser.ypp"
    {
		(yyval.u.dtype) = new ArrayType((yyvsp[(1) - (4)].u.dtype), (yyvsp[(3) - (4)].range));
	;}
    break;

  case 77:
#line 794 "parser.ypp"
    {
		MolecularField* mol = new MolecularField(current_class, (yyvsp[(1) - (3)].str));
		if((yyvsp[(3) - (3)].u.dfield) == nullptr)
		{
			// Ignore this field, it should have already generated an error
			(yyval.u.dmolecule) = mol;
			break;
		}

		bool field_added = mol->add_field((yyvsp[(3) - (3)].u.dfield));
		if(!field_added)
		{
			if((yyvsp[(3) - (3)].u.dfield)->as_molecular())
			{
				parser_error("Cannot add molecular '" + (yyvsp[(3) - (3)].u.dfield)->get_name() + "' to a molecular field.");
			}
			else
			{
				parser_error("Unkown error adding field " + (yyvsp[(3) - (3)].u.dfield)->get_name() + " to molecular '"
				             + (yyvsp[(1) - (3)].str) + "'.");
			}
		}

		(yyval.u.dmolecule) = mol;
	;}
    break;

  case 78:
#line 820 "parser.ypp"
    {
		if((yyvsp[(3) - (3)].u.dfield) == nullptr)
		{
			// Ignore this field, it should have already generated an error
			(yyval.u.dmolecule) = (yyvsp[(1) - (3)].u.dmolecule);
			break;
		}

		bool field_added = (yyvsp[(1) - (3)].u.dmolecule)->add_field((yyvsp[(3) - (3)].u.dfield));
		if(!field_added)
		{
			if((yyvsp[(3) - (3)].u.dfield)->as_molecular())
			{
				parser_error("Cannot add molecular '" + (yyvsp[(3) - (3)].u.dfield)->get_name() + "' to a molecular field.");
			}
			else if(!(yyvsp[(1) - (3)].u.dmolecule)->has_matching_keywords(*(yyvsp[(3) - (3)].u.dfield)))
			{
				parser_error("Mismatched keywords in molecular between " +
					(yyvsp[(1) - (3)].u.dmolecule)->get_field(0)->get_name() + " and " + (yyvsp[(3) - (3)].u.dfield)->get_name() + ".");
			}
			else
			{
				parser_error("Unkown error adding field " + (yyvsp[(3) - (3)].u.dfield)->get_name() + " to molecular '"
				             + (yyvsp[(1) - (3)].u.dmolecule)->get_name() + "'.");
			}
		}

		(yyval.u.dmolecule) = (yyvsp[(1) - (3)].u.dmolecule);
	;}
    break;

  case 79:
#line 852 "parser.ypp"
    {
		if(!current_class)
		{
			parser_error("Field '" + (yyvsp[(1) - (1)].str) + "' not defined in current class.");
			(yyval.u.dfield) = nullptr;
			break;
		}

		Field *field = current_class->get_field_by_name((yyvsp[(1) - (1)].str));
		if(field == nullptr)
		{
			parser_error("Field '" + (yyvsp[(1) - (1)].str) + "' not defined in current class.");
			(yyval.u.dfield) = nullptr;
			break;
		}

		(yyval.u.dfield) = field;
	;}
    break;

  case 80:
#line 874 "parser.ypp"
    {
		if((yyvsp[(1) - (1)].u.type) == T_STRING)
		{
			if(basic_string == nullptr)
			{
				basic_string = new ArrayType(new NumericType(T_CHAR));
				basic_string->set_alias("string");
			}

			(yyval.u.dtype) = basic_string;
		}
		else if((yyvsp[(1) - (1)].u.type) == T_BLOB)
		{
			if(basic_blob == nullptr)
			{
				basic_blob = new ArrayType(new NumericType(T_UINT8));
				basic_blob->set_alias("blob");
			}

			(yyval.u.dtype) = basic_blob;
		}
		else
		{
			parser_error("Found builtin ArrayType not handled by parser.");
			(yyval.u.dtype) = nullptr;
		}
	;}
    break;

  case 81:
#line 902 "parser.ypp"
    {
		if((yyvsp[(1) - (4)].u.type) == T_STRING)
		{
			ArrayType* arr = new ArrayType(new NumericType(T_CHAR), (yyvsp[(3) - (4)].range));
			arr->set_alias("string");
			(yyval.u.dtype) = arr;
		}
		else if((yyvsp[(1) - (4)].u.type) == T_BLOB)
		{
			ArrayType* arr = new ArrayType(new NumericType(T_UINT8), (yyvsp[(3) - (4)].range));
			arr->set_alias("blob");
			(yyval.u.dtype) = arr;
		}
		else
		{
			parser_error("Found builtin ArrayType not handled by parser.");
			(yyval.u.dtype) = nullptr;
		}
	;}
    break;

  case 86:
#line 931 "parser.ypp"
    { (yyval.u.dnumeric) = new NumericType((yyvsp[(1) - (1)].u.type)); ;}
    break;

  case 87:
#line 936 "parser.ypp"
    {
		if(!(yyvsp[(1) - (4)].u.dnumeric)->set_range((yyvsp[(3) - (4)].range)))
		{
			parser_error("Invalid range for type.");
		}

		(yyval.u.dnumeric) = (yyvsp[(1) - (4)].u.dnumeric);
	;}
    break;

  case 88:
#line 945 "parser.ypp"
    {
		if(!(yyvsp[(1) - (4)].u.dnumeric)->set_range((yyvsp[(3) - (4)].range)))
		{
			parser_error("Invalid range for type.");
		}

		(yyval.u.dnumeric) = (yyvsp[(1) - (4)].u.dnumeric);
	;}
    break;

  case 89:
#line 954 "parser.ypp"
    {
		if(!(yyvsp[(1) - (4)].u.dnumeric)->set_range((yyvsp[(3) - (4)].range)))
		{
			parser_error("Invalid range for type.");
		}

		(yyval.u.dnumeric) = (yyvsp[(1) - (4)].u.dnumeric);
	;}
    break;

  case 90:
#line 966 "parser.ypp"
    {
		if(!(yyvsp[(1) - (3)].u.dnumeric)->set_modulus((yyvsp[(3) - (3)].u.real)))
		{
			parser_error("Invalid modulus for type.");
		}

		(yyval.u.dnumeric) = (yyvsp[(1) - (3)].u.dnumeric);
	;}
    break;

  case 91:
#line 978 "parser.ypp"
    {
		if(!(yyvsp[(1) - (3)].u.dnumeric)->set_divisor((yyvsp[(3) - (3)].u.uint32)))
		{
			parser_error("Invalid divisor for type.");
		}
	;}
    break;

  case 92:
#line 985 "parser.ypp"
    {
		if(!(yyvsp[(1) - (3)].u.dnumeric)->set_divisor((yyvsp[(3) - (3)].u.uint32)))
		{
			parser_error("Invalid divisor for type.");
		}
	;}
    break;

  case 93:
#line 995 "parser.ypp"
    {
		(yyval.u.dmethod) = new Method();
	;}
    break;

  case 94:
#line 999 "parser.ypp"
    {
		(yyval.u.dmethod) = (yyvsp[(1) - (2)].u.dmethod);
	;}
    break;

  case 95:
#line 1006 "parser.ypp"
    {
		Method* fn = new Method();
		bool param_added = fn->add_parameter((yyvsp[(2) - (2)].u.dparam));
		if(!param_added)
		{
			parser_error("Unknown error adding parameter to method.");
		}
		(yyval.u.dmethod) = fn;
	;}
    break;

  case 96:
#line 1016 "parser.ypp"
    {
		bool param_added = (yyvsp[(1) - (3)].u.dmethod)->add_parameter((yyvsp[(3) - (3)].u.dparam));
		if(!param_added)
		{
			parser_error("Cannot add parameter '" + (yyvsp[(3) - (3)].u.dparam)->get_name()
			             + "', a parameter with that name is already used in this method.");
		}
		(yyval.u.dmethod) = (yyvsp[(1) - (3)].u.dmethod);
	;}
    break;

  case 100:
#line 1032 "parser.ypp"
    {
		(yyval.u.dparam) = new Parameter((yyvsp[(1) - (1)].u.dtype));
	;}
    break;

  case 101:
#line 1036 "parser.ypp"
    {
		current_depth = 0;
		type_stack.push(TypeAndDepth((yyvsp[(1) - (2)].u.dtype),0));
	;}
    break;

  case 102:
#line 1041 "parser.ypp"
    {
		Parameter* param = new Parameter((yyvsp[(1) - (4)].u.dtype));
		if(!type_stack.empty()) depth_error(0, "type");
		param->set_default_value((yyvsp[(4) - (4)].str));
		(yyval.u.dparam) = param;
	;}
    break;

  case 103:
#line 1050 "parser.ypp"
    {
		(yyval.u.dparam) = new Parameter((yyvsp[(1) - (2)].u.dtype), (yyvsp[(2) - (2)].str));
	;}
    break;

  case 104:
#line 1057 "parser.ypp"
    {
		(yyvsp[(1) - (4)].u.dparam)->set_type(new ArrayType((yyvsp[(1) - (4)].u.dparam)->get_type(), (yyvsp[(3) - (4)].range)));
		(yyval.u.dparam) = (yyvsp[(1) - (4)].u.dparam);
	;}
    break;

  case 105:
#line 1062 "parser.ypp"
    {
		(yyvsp[(1) - (4)].u.dparam)->set_type(new ArrayType((yyvsp[(1) - (4)].u.dparam)->get_type(), (yyvsp[(3) - (4)].range)));
		(yyval.u.dparam) = (yyvsp[(1) - (4)].u.dparam);
	;}
    break;

  case 106:
#line 1070 "parser.ypp"
    {
		current_depth = 0;
		type_stack.push(TypeAndDepth((yyvsp[(1) - (2)].u.dparam)->get_type(), 0));
	;}
    break;

  case 107:
#line 1075 "parser.ypp"
    {
		if(!type_stack.empty()) depth_error(0, "parameter '" + (yyvsp[(1) - (4)].u.dparam)->get_name() + "'");
		(yyvsp[(1) - (4)].u.dparam)->set_default_value((yyvsp[(4) - (4)].str));
		(yyval.u.dparam) = (yyvsp[(1) - (4)].u.dparam);
	;}
    break;

  case 108:
#line 1081 "parser.ypp"
    {
		current_depth = 0;
		type_stack.push(TypeAndDepth((yyvsp[(1) - (2)].u.dparam)->get_type(), 0));
	;}
    break;

  case 109:
#line 1086 "parser.ypp"
    {
		if(!type_stack.empty()) depth_error(0, "parameter '" + (yyvsp[(1) - (4)].u.dparam)->get_name() + "'");
		(yyvsp[(1) - (4)].u.dparam)->set_default_value((yyvsp[(4) - (4)].str));
		(yyval.u.dparam) = (yyvsp[(1) - (4)].u.dparam);
	;}
    break;

  case 110:
#line 1094 "parser.ypp"
    { (yyval.range) = NumericRange(); ;}
    break;

  case 111:
#line 1095 "parser.ypp"
    { (yyval.range) = NumericRange((yyvsp[(1) - (1)].u.real), (yyvsp[(1) - (1)].u.real)); ;}
    break;

  case 112:
#line 1096 "parser.ypp"
    { (yyval.range) = NumericRange((yyvsp[(1) - (3)].u.real), (yyvsp[(3) - (3)].u.real)); ;}
    break;

  case 113:
#line 1100 "parser.ypp"
    { (yyval.range) = NumericRange(); ;}
    break;

  case 114:
#line 1101 "parser.ypp"
    { (yyval.range) = NumericRange((yyvsp[(1) - (1)].u.uint32), (yyvsp[(1) - (1)].u.uint32)); ;}
    break;

  case 115:
#line 1102 "parser.ypp"
    { (yyval.range) = NumericRange((yyvsp[(1) - (3)].u.uint32), (yyvsp[(3) - (3)].u.uint32)); ;}
    break;

  case 116:
#line 1107 "parser.ypp"
    {
		if((yyvsp[(1) - (1)].str).length() != 1)
		{
			parser_error("Single character required.");
			(yyval.u.uint32) = 0;
		}
		else
		{
			(yyval.u.uint32) = (unsigned char)(yyvsp[(1) - (1)].str)[0];
		}
	;}
    break;

  case 118:
#line 1123 "parser.ypp"
    {
		unsigned int num = (unsigned int)(yyvsp[(1) - (1)].u.uint64);
		if(num != (yyvsp[(1) - (1)].u.uint64))
		{
			parser_error("Number out of range.");
			(yyval.u.uint32) = 1;
		}
		(yyval.u.uint32) = num;
	;}
    break;

  case 119:
#line 1135 "parser.ypp"
    { (yyval.u.real) = (double)(yyvsp[(1) - (1)].u.uint64); ;}
    break;

  case 120:
#line 1136 "parser.ypp"
    { (yyval.u.real) = (double)(yyvsp[(1) - (1)].u.int64); ;}
    break;

  case 122:
#line 1142 "parser.ypp"
    {
		if((yyvsp[(1) - (1)].str).length() != 1)
		{
			parser_error("Single character required.");
			(yyval.u.real) = 0;
		}
		else
		{
			(yyval.u.real) = (double)(unsigned char)(yyvsp[(1) - (1)].str)[0];
		}
	;}
    break;

  case 124:
#line 1158 "parser.ypp"
    {
		if(!check_depth()) depth_error("signed integer");

		const DistributedType* dtype = type_stack.top().type;
		if(dtype == nullptr)
		{
			// Ignore this field, it should have already generated an error
			(yyval.str) = "";
			break;
		}
		type_stack.pop(); // Remove numeric type from stack

		(yyval.str) = number_value(dtype->get_type(), (yyvsp[(1) - (1)].u.int64));
	;}
    break;

  case 125:
#line 1173 "parser.ypp"
    {
		if(!check_depth()) depth_error("unsigned integer");

		const DistributedType* dtype = type_stack.top().type;
		if(dtype == nullptr)
		{
			// Ignore this field, it should have already generated an error
			(yyval.str) = "";
			break;
		}
		type_stack.pop(); // Remove numeric type from stack

		(yyval.str) = number_value(dtype->get_type(), (yyvsp[(1) - (1)].u.uint64));
	;}
    break;

  case 126:
#line 1188 "parser.ypp"
    {
		if(!check_depth()) depth_error("floating point");

		const DistributedType* dtype = type_stack.top().type;
		if(dtype == nullptr)
		{
			// Ignore this field, it should have already generated an error
			(yyval.str) = "";
			break;
		}
		type_stack.pop(); // Remove numeric type from stack

		(yyval.str) = number_value(dtype->get_type(), (yyvsp[(1) - (1)].u.real));
	;}
    break;

  case 127:
#line 1203 "parser.ypp"
    {
		if(!check_depth()) depth_error("char");

		const DistributedType* dtype = type_stack.top().type;
		if(dtype == nullptr)
		{
			// Ignore this field, it should have already generated an error
			(yyval.str) = "";
			break;
		}
		type_stack.pop(); // Remove type from stack

		if(dtype->get_type() != T_CHAR) {
			parser_error("Cannot use char value for non-char type '"
			             + format_type(dtype->get_type()) + "'.");
		}

		if((yyvsp[(1) - (1)].str).length() != 1)
		{
			parser_error("Single character required.");
			(yyval.str) = "";
		}
		else
		{
			(yyval.str) = (yyvsp[(1) - (1)].str);
		}
	;}
    break;

  case 128:
#line 1231 "parser.ypp"
    {
		if(!check_depth()) depth_error("string");

		const DistributedType* dtype = type_stack.top().type;
		if(dtype == nullptr)
		{
			// Ignore this field, it should have already generated an error
			(yyval.str) = "";
			break;
		}
		type_stack.pop(); // Remove string type from stack

		if(dtype->get_type() == T_STRING || dtype->get_type() == T_BLOB)
		{
			if((yyvsp[(1) - (1)].str).length() != dtype->get_size())
			{
				parser_error("Value for fixed-length string has incorrect length.");
			}

			(yyval.str) = (yyvsp[(1) - (1)].str);
		}
		else if(dtype->get_type() == T_VARSTRING || dtype->get_type() == T_VARBLOB)
		{
			// TODO: Check for range limits
			// Prepend length tag
			sizetag_t length = (yyvsp[(1) - (1)].str).length();
			length = swap_le(length);
			(yyval.str) = string((char*)&length, sizeof(sizetag_t)) + (yyvsp[(1) - (1)].str);
		}
		else
		{
			parser_error("Cannot use string value for non-string type '"
			             + format_type(dtype->get_type()) + "'.");
			(yyval.str) = (yyvsp[(1) - (1)].str);
		}
	;}
    break;

  case 129:
#line 1268 "parser.ypp"
    {
		if(!check_depth()) depth_error("hex-string");

		const DistributedType* dtype = type_stack.top().type;
		if(dtype == nullptr)
		{
			// Ignore this field, it should have already generated an error
			(yyval.str) = "";
			break;
		}
		type_stack.pop(); // Remove type from stack

		if(dtype->get_type() == T_BLOB)
		{
			if((yyvsp[(1) - (1)].str).length() != dtype->get_size())
			{
				parser_error("Value for fixed-length blob has incorrect length.");
			}

			(yyval.str) = (yyvsp[(1) - (1)].str);
		}
		else if(dtype->get_type() == T_VARBLOB)
		{
			// TODO: Check for range limits
			(yyval.str) = (yyvsp[(1) - (1)].str);
		}
		else
		{
			parser_error("Cannot use hex value for non-blob type '"
			             + format_type(dtype->get_type()) + "'.");
			(yyval.str) = (yyvsp[(1) - (1)].str);
		}
	;}
    break;

  case 132:
#line 1307 "parser.ypp"
    {
		if(!check_depth()) depth_error("method");

		const DistributedType* dtype = type_stack.top().type;
		if(dtype == nullptr)
		{
			// Ignore this field, it should have already generated an error
			break;
		}
		if(dtype->as_method())
		{
			current_depth++;
			const Method* method = dtype->as_method();

			size_t num_params = method->get_num_parameters();
			for(unsigned int i = 1; i <= num_params; ++i)
			{
				// Reverse iteration
				const Parameter* param = method->get_parameter(num_params-i);
				// Add parameter types to stack such that the first is on top
				type_stack.push(TypeAndDepth(param->get_type(), current_depth));
			}
		}
		else
		{
			parser_error("Cannot use method-value for non-method type '"
			             + format_type(dtype->get_type()) + "'.");
		}
	;}
    break;

  case 133:
#line 1337 "parser.ypp"
    {
		if(type_stack.top().type->as_method())
		{
			current_depth--;
		}
		type_stack.pop(); // Remove method type from the stack
		(yyval.str) = (yyvsp[(3) - (4)].str);
	;}
    break;

  case 135:
#line 1350 "parser.ypp"
    {
		(yyval.str) = (yyvsp[(1) - (3)].str) + (yyvsp[(3) - (3)].str);
	;}
    break;

  case 136:
#line 1357 "parser.ypp"
    {
		if(!check_depth()) depth_error("struct");

		const DistributedType* dtype = type_stack.top().type;
		if(dtype == nullptr)
		{
			// Ignore this field, it should have already generated an error
			break;
		}
		if(dtype->as_struct())
		{
			current_depth++;
			const Struct* dstruct = dtype->as_struct();

			size_t num_fields = dstruct->get_num_fields();
			for(unsigned int i = 1; i <= num_fields; ++i)
			{
				// Reverse iteration
				const Field* field = dstruct->get_field(num_fields-i);
				// Add field types to stack such that the first is on top
				type_stack.push(TypeAndDepth(field->get_type(), current_depth));
			}
		}
		else
		{
			parser_error("Cannot use struct-composition for non-struct type '"
			             + format_type(dtype->get_type()) + "'.");
		}
	;}
    break;

  case 137:
#line 1387 "parser.ypp"
    {
		if(type_stack.top().type->as_struct())
		{
			current_depth--;
		}
		type_stack.pop(); // Remove method type from the stack
		(yyval.str) = (yyvsp[(3) - (4)].str);
	;}
    break;

  case 140:
#line 1401 "parser.ypp"
    {
		(yyval.str) = (yyvsp[(1) - (3)].str) + (yyvsp[(3) - (3)].str);
	;}
    break;

  case 141:
#line 1405 "parser.ypp"
    {
		(yyval.str) = (yyvsp[(1) - (3)].str) + (yyvsp[(3) - (3)].str);
	;}
    break;

  case 142:
#line 1412 "parser.ypp"
    {
		if(!check_depth()) depth_error("array");

		const DistributedType* dtype = type_stack.top().type;
		if(dtype == nullptr)
		{
			// Ignore this field, it should have already generated an error
			(yyval.str) = "";
			break;
		}
		type_stack.pop();
		if(!dtype->as_array())
		{
			parser_error("Cannot use array-composition for non-array type '"
			             + format_type(dtype->get_type()) + "'.");
			(yyval.str) = "";
			break;
		}
		const ArrayType* array = dtype->as_array();

		string val;

		if(array->get_array_size() > 0)
		{
			// For fixed size arrays, an empty array is an error
			parser_error("Fixed-sized array of size "
			             + to_string((unsigned long long)array->get_array_size())
			             + " can't have 0 elements.");
		}
		else if(array->has_range())
		{
			// If we have a range, 0 elements must be valid in the range
			if(0 < array->get_range().min.uinteger)
			{
				parser_error("Too few elements in array value, minimum "
				             + to_string(array->get_range().min.uinteger) + ".");
			}
		}

		// Since a fixed-size array can't have zero elements, this always
		// the default value for a varsize array, which is the length-tag 0.
		(yyval.str) = string(sizeof(sizetag_t), '\0');
	;}
    break;

  case 143:
#line 1456 "parser.ypp"
    {
		if(!check_depth()) depth_error("array");

		const DistributedType* dtype = type_stack.top().type;
		if(dtype == nullptr)
		{
			// Ignore this field, it should have already generated an error
			break;
		}
		if(dtype->as_array())
		{
			const ArrayType* array = dtype->as_array();

			// For arrays we're going to do something pretty hacky:
			//    For every element we had we are going to increment the depth,
			//    and after we finish the element_values production we will compare
			//    the current_depth to the depth of our original symbol to check
			//    if the array size is proper.
			type_stack.push(TypeAndDepth(array->get_element_type(), current_depth));
		}
		else
		{
			parser_error("Cannot use array-composition for non-array type '"
			             + format_type(dtype->get_type()) + "'.");
		}
	;}
    break;

  case 144:
#line 1483 "parser.ypp"
    {
		if(type_stack.top().type->as_array())
		{
			uint64_t actual_size = current_depth - type_stack.top().depth;

			const DistributedType* dtype = type_stack.top().type;
			if(dtype == nullptr)
			{
				// Ignore this field, it should have already generated an error
				(yyval.str) = "";
				break;
			}

			const ArrayType* array = dtype->as_array();
			if(array->has_range())
			{
				if(actual_size > array->get_range().max.uinteger)
				{
					parser_error("Too many elements in array value, maximum "
					             + to_string(array->get_range().max.uinteger) + ".");
				}
				else if(actual_size < array->get_range().min.uinteger)
				{
					parser_error("Too few elements in array value, minimum "
					             + to_string(array->get_range().min.uinteger) + ".");
				}
			}

			if(array->get_array_size() == 0)
			{
				sizetag_t length = (yyvsp[(3) - (4)].str).length();
				(yyval.str) = string((char*)&length, sizeof(sizetag_t)) + (yyvsp[(3) - (4)].str);
			}
			else
			{
				(yyval.str) = (yyvsp[(3) - (4)].str);
			}
			current_depth = type_stack.top().depth;
		}

		type_stack.pop(); // Remove array type from the stack
	;}
    break;

  case 146:
#line 1530 "parser.ypp"
    {
		// We popped off the only element we added, so we're back to the array
		// Don't increment the depth; the array_expansion will add to
		// the current_depth depending on the number of elements it adds.
		const DistributedType* dtype = type_stack.top().type;
		if(dtype == nullptr)
		{
			// Ignore this field, it should have already generated an error
			break;
		}
		const ArrayType* array = dtype->as_array();
		type_stack.push(TypeAndDepth(array->get_element_type(), current_depth));
	;}
    break;

  case 147:
#line 1544 "parser.ypp"
    {
		(yyval.str) = (yyvsp[(1) - (4)].str) + (yyvsp[(4) - (4)].str);
	;}
    break;

  case 148:
#line 1551 "parser.ypp"
    {
		current_depth++;
		(yyval.str) = (yyvsp[(1) - (1)].str);
	;}
    break;

  case 149:
#line 1556 "parser.ypp"
    {
		const DistributedType* dtype = type_stack.top().type;
		if(dtype == nullptr)
		{
			// Ignore this field, it should have already generated an error
			(yyval.str) = "";
			break;
		}
		type_stack.pop(); // Pop that array element type
		current_depth += (yyvsp[(3) - (3)].u.uint32); // For arrays, we add 1 to the depth per element

		string base = number_value(dtype->get_type(), (yyvsp[(1) - (3)].u.int64));

		string val;
		val.reserve(base.length() * (yyvsp[(3) - (3)].u.uint32));
		for(unsigned int i = 0; i < (yyvsp[(3) - (3)].u.uint32); ++i)
		{
			val += base;
		}
		(yyval.str) = val;
	;}
    break;

  case 150:
#line 1578 "parser.ypp"
    {
		const DistributedType* dtype = type_stack.top().type;
		if(dtype == nullptr)
		{
			// Ignore this field, it should have already generated an error
			(yyval.str) = "";
			break;
		}
		type_stack.pop(); // Pop that array element type
		current_depth += (yyvsp[(3) - (3)].u.uint32); // For arrays, we add 1 to the depth per element

		string base = number_value(dtype->get_type(), (yyvsp[(1) - (3)].u.uint64));

		string val;
		val.reserve(base.length() * (yyvsp[(3) - (3)].u.uint32));
		for(unsigned int i = 0; i < (yyvsp[(3) - (3)].u.uint32); ++i)
		{
			val += base;
		}
		(yyval.str) = val;
	;}
    break;

  case 151:
#line 1600 "parser.ypp"
    {
		const DistributedType* dtype = type_stack.top().type;
		if(dtype == nullptr)
		{
			// Ignore this field, it should have already generated an error
			(yyval.str) = "";
			break;
		}
		type_stack.pop(); // Pop that array element type
		current_depth += (yyvsp[(3) - (3)].u.uint32); // For arrays, we add 1 to the depth per element

		string base = number_value(dtype->get_type(), (yyvsp[(1) - (3)].u.real));

		string val;
		val.reserve(base.length() * (yyvsp[(3) - (3)].u.uint32));
		for(unsigned int i = 0; i < (yyvsp[(3) - (3)].u.uint32); ++i)
		{
			val += base;
		}
		(yyval.str) = val;
	;}
    break;

  case 152:
#line 1622 "parser.ypp"
    {
		const DistributedType* dtype = type_stack.top().type;
		if(dtype == nullptr)
		{
			// Ignore this field, it should have already generated an error
			(yyval.str) = "";
			break;
		}
		type_stack.pop(); // Pop that array element type
		current_depth += (yyvsp[(3) - (3)].u.uint32); // For arrays, we add 1 to the depth per element

		if(dtype->get_type() == T_STRING)
		{
			if((yyvsp[(1) - (3)].str).length() != dtype->get_size())
			{
				parser_error("Value for fixed-length string has incorrect length.");
			}

			string val;
			val.reserve((yyvsp[(1) - (3)].str).length() * (yyvsp[(3) - (3)].u.uint32));
			for(unsigned int i = 0; i < (yyvsp[(3) - (3)].u.uint32); ++i)
			{
				val += (yyvsp[(1) - (3)].str);
			}
			(yyval.str) = val;
		}
		else if(dtype->get_type() == T_VARSTRING)
		{
			// TODO: Check for range limits
			// Prepend length tag
			sizetag_t length = (yyvsp[(1) - (3)].str).length();
			string base = string((char*)&length, sizeof(sizetag_t)) + (yyvsp[(1) - (3)].str);

			string val;
			val.reserve(base.length() * (yyvsp[(3) - (3)].u.uint32));
			for(unsigned int i = 0; i < (yyvsp[(3) - (3)].u.uint32); ++i)
			{
				val += base;
			}
			(yyval.str) = val;
		}
		else
		{
			parser_error("Cannot use string value for non-string type '"
			             + format_type(dtype->get_type()) + "'.");

			string val;
			val.reserve((yyvsp[(1) - (3)].str).length() * (yyvsp[(3) - (3)].u.uint32));
			for(unsigned int i = 0; i < (yyvsp[(3) - (3)].u.uint32); ++i)
			{
				val += (yyvsp[(1) - (3)].str);
			}
			(yyval.str) = val;
		}
	;}
    break;

  case 153:
#line 1678 "parser.ypp"
    {
		const DistributedType* dtype = type_stack.top().type;
		if(dtype == nullptr)
		{
			// Ignore this field, it should have already generated an error
			(yyval.str) = "";
			break;
		}
		type_stack.pop(); // Pop that array element type
		current_depth += (yyvsp[(3) - (3)].u.uint32); // For arrays, we add 1 to the depth per element

		if(dtype->get_type() == T_BLOB)
		{
			if((yyvsp[(1) - (3)].str).length() != dtype->get_size())
			{
				parser_error("Value for fixed-length blob has incorrect length.");
			}

			string val;
			val.reserve((yyvsp[(1) - (3)].str).length() * (yyvsp[(3) - (3)].u.uint32));
			for(unsigned int i = 0; i < (yyvsp[(3) - (3)].u.uint32); ++i)
			{
				val += (yyvsp[(1) - (3)].str);
			}
			(yyval.str) = val;
		}
		else if(dtype->get_type() == T_VARBLOB)
		{
			// TODO: Check for range limits
			// Prepend length tag
			sizetag_t length = (yyvsp[(1) - (3)].str).length();
			string base = string((char*)&length, sizeof(sizetag_t)) + (yyvsp[(1) - (3)].str);

			string val;
			val.reserve(base.length() * (yyvsp[(3) - (3)].u.uint32));
			for(unsigned int i = 0; i < (yyvsp[(3) - (3)].u.uint32); ++i)
			{
				val += base;
			}
			(yyval.str) = val;

		}
		else
		{
			parser_error("Cannot use hex value for non-blob type '"
			             + format_type(dtype->get_type()) + "'.");

			string val;
			val.reserve((yyvsp[(1) - (3)].str).length() * (yyvsp[(3) - (3)].u.uint32));
			for(unsigned int i = 0; i < (yyvsp[(3) - (3)].u.uint32); ++i)
			{
				val += (yyvsp[(1) - (3)].str);
			}
			(yyval.str) = val;
		}
	;}
    break;

  case 154:
#line 1737 "parser.ypp"
    { (yyval.u.int64) = int64_t((yyvsp[(2) - (2)].u.uint64)); ;}
    break;

  case 155:
#line 1738 "parser.ypp"
    { (yyval.u.int64) = -int64_t((yyvsp[(2) - (2)].u.uint64)); ;}
    break;

  case 156:
#line 1742 "parser.ypp"
    { (yyval.u.type) = T_STRING; ;}
    break;

  case 157:
#line 1743 "parser.ypp"
    { (yyval.u.type) = T_BLOB; ;}
    break;

  case 158:
#line 1747 "parser.ypp"
    { (yyval.u.type) = T_CHAR; ;}
    break;

  case 159:
#line 1748 "parser.ypp"
    { (yyval.u.type) = T_INT8; ;}
    break;

  case 160:
#line 1749 "parser.ypp"
    { (yyval.u.type) = T_INT16; ;}
    break;

  case 161:
#line 1750 "parser.ypp"
    { (yyval.u.type) = T_INT32; ;}
    break;

  case 162:
#line 1751 "parser.ypp"
    { (yyval.u.type) = T_INT64; ;}
    break;

  case 163:
#line 1752 "parser.ypp"
    { (yyval.u.type) = T_UINT8; ;}
    break;

  case 164:
#line 1753 "parser.ypp"
    { (yyval.u.type) = T_UINT16; ;}
    break;

  case 165:
#line 1754 "parser.ypp"
    { (yyval.u.type) = T_UINT32; ;}
    break;

  case 166:
#line 1755 "parser.ypp"
    { (yyval.u.type) = T_UINT64; ;}
    break;

  case 167:
#line 1756 "parser.ypp"
    { (yyval.u.type) = T_FLOAT32; ;}
    break;

  case 168:
#line 1757 "parser.ypp"
    { (yyval.u.type) = T_FLOAT64; ;}
    break;

  case 169:
#line 1762 "parser.ypp"
    {
		(yyval.strings) = vector<string>();
	;}
    break;

  case 170:
#line 1766 "parser.ypp"
    {
		if(!parsed_file->has_keyword((yyvsp[(2) - (2)].str)))
		{
			parser_error("Keyword '" + (yyvsp[(2) - (2)].str) + "' has not been declared.");
			break;
		}

		(yyvsp[(1) - (2)].strings).push_back((yyvsp[(2) - (2)].str));
		(yyval.strings) = (yyvsp[(1) - (2)].strings);
	;}
    break;


/* Line 1267 of yacc.c.  */
#line 3489 "parser.cpp"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
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

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
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

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


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

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
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
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 1782 "parser.ypp"
 /* Start helper function section */


bool check_depth()
{
	return (!type_stack.empty() && current_depth == type_stack.top().depth);
}

void depth_error(string what)
{
	if(type_stack.empty() || current_depth < type_stack.top().depth)
	{
		parser_error("Too many nested values while parsing value for " + what + ".");
	}
	else
	{
		parser_error("Too few nested values while parsing value for " + what + ".");
	}
}

void depth_error(int depth, string what)
{
	if(current_depth > depth)
	{
		parser_error("Too few nested values before this " + what + " value.");
	}
	else
	{
		parser_error("Too many nested values before this " + what + " value.");
	}
}

string number_value(Type type, double &number)
{
	switch(type)
	{
		case T_FLOAT32:
		{
			float v = number;
			if(v == std::numeric_limits<float>::infinity() ||
			   v == -std::numeric_limits<float>::infinity())
			{
				parser_error("Value is out of range for type 'float32'.");
			}

			v = swap_le(v);
			return string((char*)&v, sizeof(float));
		}
		case T_FLOAT64:
		{
			double v = number;
			v = swap_le(v);
			return string((char*)&v, sizeof(double));
		}
		case T_INT8:
		case T_INT16:
		case T_INT32:
		case T_INT64:
		case T_CHAR:
		case T_UINT8:
		case T_UINT16:
		case T_UINT32:
		case T_UINT64:
		{
			parser_error("Cannot use floating-point value for integer datatype.");
			return string();
		}
		default:
		{
			parser_error("Cannot use floating-point value for non-numeric datatype.");
			return string();
		}
	}
}

string number_value(Type type, int64_t &number)
{
	switch(type)
	{
		case T_INT8:
		{
			if(INT8_MIN > number || number > INT8_MAX)
			{
				parser_error("Signed integer out of range for type 'int8'.");
			}

			int8_t v = number;
			return string((char*)&v, sizeof(int8_t));
		}
		case T_INT16:
		{
			if(INT16_MIN > number || number > INT16_MAX)
			{
				parser_error("Signed integer out of range for type 'int16'.");
			}

			uint16_t v = number;
			v = swap_le(v);
			return string((char*)&v, sizeof(int16_t));
		}
		case T_INT32:
		{
			if(INT32_MIN > number || number > INT32_MAX)
			{
				parser_error("Signed integer out of range for type 'int32'.");
			}

			int32_t v = number;
			v = swap_le(v);
			return string((char*)&v, sizeof(int32_t));
		}
		case T_INT64:
		{
			int64_t v = number;
			v = swap_le(v);
			return string((char*)&v, sizeof(int64_t));
		}
		case T_CHAR:
		case T_UINT8:
		case T_UINT16:
		case T_UINT32:
		case T_UINT64:
		{
			if(number < 0)
			{
				parser_error("Can't use negative value for unsigned integer datatype.");
			}
			uint64_t v = number;
			return number_value(type, v);
		}
		case T_FLOAT32:
		case T_FLOAT64:
		{
			// Note: Expecting number to be converted to a double by value (ie. -1 becomes -1.0)
			double v = number;
			return number_value(type, v);
		}
		default:
		{
			parser_error("Cannot use signed integer value for non-numeric datatype.");
			return string();
		}
	}
}

string number_value(Type type, uint64_t &number)
{
	switch(type)
	{
		case T_CHAR:
		case T_UINT8:
		{
			if(number > UINT8_MAX)
			{
				parser_error("Unsigned integer out of range for type 'uint8'.");
			}

			uint8_t v = number;
			return string((char*)&v, sizeof(uint8_t));
		}
		case T_UINT16:
		{
			if(number > UINT16_MAX)
			{
				parser_error("Unsigned integer out of range for type 'uint16'.");
			}

			uint16_t v = number;
			v = swap_le(v);
			return string((char*)&v, sizeof(uint16_t));
		}
		case T_UINT32:
		{
			if(number > UINT32_MAX)
			{
				parser_error("Unsigned integer out of range for type 'uint32'.");
			}

			uint32_t v = number;
			v = swap_le(v);
			return string((char*)&v, sizeof(uint32_t));
		}
		case T_UINT64:
		{
			uint64_t v = number;
			v = swap_le(v);
			return string((char*)&number, sizeof(uint64_t));
		}
		case T_INT8:
		case T_INT16:
		case T_INT32:
		case T_INT64:
		{
			if(number > INT64_MAX)
			{
				parser_error("Unsigned integer out of range for signed integer datatype.");
			}
			int64_t v = number;
			return number_value(type, v);
		}
		case T_FLOAT32:
		case T_FLOAT64:
		{
			// Note: Expecting number to be converted to a double by value (ie. 3 becomes 3.0)
			double v = number;
			return number_value(type, v);
		}
		default:
		{
			parser_error("Cannot use unsigned integer value for non-numeric datatype.");
			return string();
		}
	}
}


} // close namespace dclass

