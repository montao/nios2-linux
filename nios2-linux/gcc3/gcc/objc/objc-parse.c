/* A Bison parser, made by GNU Bison 1.875.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software Foundation, Inc.

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
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

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
     IDENTIFIER = 258,
     TYPENAME = 259,
     SCSPEC = 260,
     STATIC = 261,
     TYPESPEC = 262,
     TYPE_QUAL = 263,
     CONSTANT = 264,
     STRING = 265,
     ELLIPSIS = 266,
     SIZEOF = 267,
     ENUM = 268,
     STRUCT = 269,
     UNION = 270,
     IF = 271,
     ELSE = 272,
     WHILE = 273,
     DO = 274,
     FOR = 275,
     SWITCH = 276,
     CASE = 277,
     DEFAULT = 278,
     BREAK = 279,
     CONTINUE = 280,
     RETURN = 281,
     GOTO = 282,
     ASM_KEYWORD = 283,
     TYPEOF = 284,
     ALIGNOF = 285,
     ATTRIBUTE = 286,
     EXTENSION = 287,
     LABEL = 288,
     REALPART = 289,
     IMAGPART = 290,
     VA_ARG = 291,
     CHOOSE_EXPR = 292,
     TYPES_COMPATIBLE_P = 293,
     PTR_VALUE = 294,
     PTR_BASE = 295,
     PTR_EXTENT = 296,
     FUNC_NAME = 297,
     ASSIGN = 298,
     OROR = 299,
     ANDAND = 300,
     EQCOMPARE = 301,
     ARITHCOMPARE = 302,
     RSHIFT = 303,
     LSHIFT = 304,
     MINUSMINUS = 305,
     PLUSPLUS = 306,
     UNARY = 307,
     HYPERUNARY = 308,
     POINTSAT = 309,
     INTERFACE = 310,
     IMPLEMENTATION = 311,
     END = 312,
     SELECTOR = 313,
     DEFS = 314,
     ENCODE = 315,
     CLASSNAME = 316,
     PUBLIC = 317,
     PRIVATE = 318,
     PROTECTED = 319,
     PROTOCOL = 320,
     OBJECTNAME = 321,
     CLASS = 322,
     ALIAS = 323,
     AT_THROW = 324,
     AT_TRY = 325,
     AT_CATCH = 326,
     AT_FINALLY = 327,
     AT_SYNCHRONIZED = 328,
     OBJC_STRING = 329
   };
#endif
#define IDENTIFIER 258
#define TYPENAME 259
#define SCSPEC 260
#define STATIC 261
#define TYPESPEC 262
#define TYPE_QUAL 263
#define CONSTANT 264
#define STRING 265
#define ELLIPSIS 266
#define SIZEOF 267
#define ENUM 268
#define STRUCT 269
#define UNION 270
#define IF 271
#define ELSE 272
#define WHILE 273
#define DO 274
#define FOR 275
#define SWITCH 276
#define CASE 277
#define DEFAULT 278
#define BREAK 279
#define CONTINUE 280
#define RETURN 281
#define GOTO 282
#define ASM_KEYWORD 283
#define TYPEOF 284
#define ALIGNOF 285
#define ATTRIBUTE 286
#define EXTENSION 287
#define LABEL 288
#define REALPART 289
#define IMAGPART 290
#define VA_ARG 291
#define CHOOSE_EXPR 292
#define TYPES_COMPATIBLE_P 293
#define PTR_VALUE 294
#define PTR_BASE 295
#define PTR_EXTENT 296
#define FUNC_NAME 297
#define ASSIGN 298
#define OROR 299
#define ANDAND 300
#define EQCOMPARE 301
#define ARITHCOMPARE 302
#define RSHIFT 303
#define LSHIFT 304
#define MINUSMINUS 305
#define PLUSPLUS 306
#define UNARY 307
#define HYPERUNARY 308
#define POINTSAT 309
#define INTERFACE 310
#define IMPLEMENTATION 311
#define END 312
#define SELECTOR 313
#define DEFS 314
#define ENCODE 315
#define CLASSNAME 316
#define PUBLIC 317
#define PRIVATE 318
#define PROTECTED 319
#define PROTOCOL 320
#define OBJECTNAME 321
#define CLASS 322
#define ALIAS 323
#define AT_THROW 324
#define AT_TRY 325
#define AT_CATCH 326
#define AT_FINALLY 327
#define AT_SYNCHRONIZED 328
#define OBJC_STRING 329




/* Copy the first part of user declarations.  */
#line 33 "objc/objc-parse.y"

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "input.h"
#include "cpplib.h"
#include "intl.h"
#include "timevar.h"
#include "c-pragma.h"		/* For YYDEBUG definition, and parse_in.  */
#include "c-tree.h"
#include "flags.h"
#include "varray.h"
#include "output.h"
#include "toplev.h"
#include "ggc.h"

#include "objc-act.h"

/* Like YYERROR but do call yyerror.  */
#define YYERROR1 { yyerror ("syntax error"); YYERROR; }

/* Like the default stack expander, except (1) use realloc when possible,
   (2) impose no hard maxiumum on stack size, (3) REALLY do not use alloca.

   Irritatingly, YYSTYPE is defined after this %{ %} block, so we cannot
   give malloced_yyvs its proper type.  This is ok since all we need from
   it is to be able to free it.  */

static short *malloced_yyss;
static void *malloced_yyvs;

#define yyoverflow(MSG, SS, SSSIZE, VS, VSSIZE, YYSSZ)			\
do {									\
  size_t newsize;							\
  short *newss;								\
  YYSTYPE *newvs;							\
  newsize = *(YYSSZ) *= 2;						\
  if (malloced_yyss)							\
    {									\
      newss = really_call_realloc (*(SS), newsize * sizeof (short));	\
      newvs = really_call_realloc (*(VS), newsize * sizeof (YYSTYPE));	\
    }									\
  else									\
    {									\
      newss = really_call_malloc (newsize * sizeof (short));		\
      newvs = really_call_malloc (newsize * sizeof (YYSTYPE));		\
      if (newss)							\
        memcpy (newss, *(SS), (SSSIZE));				\
      if (newvs)							\
        memcpy (newvs, *(VS), (VSSIZE));				\
    }									\
  if (!newss || !newvs)							\
    {									\
      yyerror (MSG);							\
      return 2;								\
    }									\
  *(SS) = newss;							\
  *(VS) = newvs;							\
  malloced_yyss = newss;						\
  malloced_yyvs = (void *) newvs;					\
} while (0)


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 100 "objc/objc-parse.y"
typedef union YYSTYPE {long itype; tree ttype; enum tree_code code;
	location_t location; } YYSTYPE;
/* Line 191 of yacc.c.  */
#line 291 "objc/objc-parse.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */
#line 248 "objc/objc-parse.y"

/* Number of statements (loosely speaking) and compound statements
   seen so far.  */
static int stmt_count;
static int compstmt_count;

/* Input location of the end of the body of last simple_if;
   used by the stmt-rule immediately after simple_if returns.  */
static location_t if_stmt_locus;


/* List of types and structure classes of the current declaration.  */
static GTY(()) tree current_declspecs;
static GTY(()) tree prefix_attributes;

/* List of all the attributes applying to the identifier currently being
   declared; includes prefix_attributes and possibly some more attributes
   just after a comma.  */
static GTY(()) tree all_prefix_attributes;

/* Stack of saved values of current_declspecs, prefix_attributes and
   all_prefix_attributes.  */
static GTY(()) tree declspec_stack;

/* PUSH_DECLSPEC_STACK is called from setspecs; POP_DECLSPEC_STACK
   should be called from the productions making use of setspecs.  */
#define PUSH_DECLSPEC_STACK						 \
  do {									 \
    declspec_stack = tree_cons (build_tree_list (prefix_attributes,	 \
						 all_prefix_attributes), \
				current_declspecs,			 \
				declspec_stack);			 \
  } while (0)

#define POP_DECLSPEC_STACK						\
  do {									\
    current_declspecs = TREE_VALUE (declspec_stack);			\
    prefix_attributes = TREE_PURPOSE (TREE_PURPOSE (declspec_stack));	\
    all_prefix_attributes = TREE_VALUE (TREE_PURPOSE (declspec_stack));	\
    declspec_stack = TREE_CHAIN (declspec_stack);			\
  } while (0)

/* For __extension__, save/restore the warning flags which are
   controlled by __extension__.  */
#define SAVE_EXT_FLAGS()		\
	(pedantic			\
	 | (warn_pointer_arith << 1)	\
	 | (warn_traditional << 2)	\
	 | (flag_iso << 3))

#define RESTORE_EXT_FLAGS(val)			\
  do {						\
    pedantic = val & 1;				\
    warn_pointer_arith = (val >> 1) & 1;	\
    warn_traditional = (val >> 2) & 1;		\
    flag_iso = (val >> 3) & 1;			\
  } while (0)

/* Objective-C specific parser/lexer information */

static enum tree_code objc_inherit_code;
static int objc_pq_context = 0, objc_public_flag = 0;

/* The following flag is needed to contextualize ObjC lexical analysis.
   In some cases (e.g., 'int NSObject;'), it is undesirable to bind
   an identifier to an ObjC class, even if a class with that name
   exists.  */
static int objc_need_raw_identifier;
#define OBJC_NEED_RAW_IDENTIFIER(VAL)	objc_need_raw_identifier = VAL


static bool parsing_iso_function_signature;

/* Tell yyparse how to print a token's value, if yydebug is set.  */

#define YYPRINT(FILE,YYCHAR,YYLVAL) yyprint(FILE,YYCHAR,YYLVAL)

static void yyprint (FILE *, int, YYSTYPE);
static void yyerror (const char *);
static int yylexname (void);
static inline int _yylex (void);
static int  yylex (void);
static void init_reswords (void);

  /* Initialisation routine for this file.  */
void
c_parse_init (void)
{
  init_reswords ();
}



/* Line 214 of yacc.c.  */
#line 395 "objc/objc-parse.c"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
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
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   5175

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  97
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  269
/* YYNRULES -- Number of rules. */
#define YYNRULES  725
/* YYNRULES -- Number of states. */
#define YYNSTATES  1153

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   329

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    93,     2,     2,     2,    60,    51,     2,
      66,    89,    58,    56,    94,    57,    65,    59,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    46,    90,
       2,    43,     2,    45,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    67,     2,    96,    50,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    95,    49,    91,    92,     2,     2,     2,
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
      35,    36,    37,    38,    39,    40,    41,    42,    44,    47,
      48,    52,    53,    54,    55,    61,    62,    63,    64,    68,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short yyprhs[] =
{
       0,     0,     3,     4,     6,     7,    10,    11,    15,    17,
      19,    21,    23,    29,    32,    36,    41,    46,    49,    52,
      55,    57,    58,    59,    68,    73,    74,    75,    84,    89,
      90,    91,    99,   103,   105,   107,   109,   111,   113,   115,
     117,   119,   121,   123,   125,   127,   128,   130,   132,   136,
     138,   141,   144,   147,   150,   153,   158,   161,   166,   169,
     172,   174,   176,   178,   180,   185,   187,   191,   195,   199,
     203,   207,   211,   215,   219,   223,   227,   231,   235,   236,
     241,   242,   247,   248,   249,   257,   258,   264,   268,   272,
     274,   276,   278,   280,   281,   289,   293,   297,   301,   305,
     310,   317,   326,   333,   338,   342,   346,   349,   352,   354,
     356,   358,   360,   362,   364,   365,   367,   370,   372,   374,
     377,   380,   385,   390,   393,   396,   399,   400,   402,   407,
     412,   416,   420,   423,   426,   428,   431,   434,   437,   440,
     443,   445,   448,   450,   453,   456,   459,   462,   465,   468,
     470,   473,   476,   479,   482,   485,   488,   491,   494,   497,
     500,   503,   506,   509,   512,   515,   518,   520,   523,   526,
     529,   532,   535,   538,   541,   544,   547,   550,   553,   556,
     559,   562,   565,   568,   571,   574,   577,   580,   583,   586,
     589,   592,   595,   598,   601,   604,   607,   610,   613,   616,
     619,   622,   625,   628,   631,   634,   637,   640,   643,   646,
     649,   652,   654,   656,   658,   660,   662,   664,   666,   668,
     670,   672,   674,   676,   678,   680,   682,   684,   686,   688,
     690,   692,   694,   696,   698,   700,   702,   704,   706,   708,
     710,   712,   714,   716,   718,   720,   722,   724,   726,   728,
     730,   732,   734,   736,   738,   740,   742,   744,   746,   748,
     750,   752,   754,   756,   758,   760,   762,   764,   765,   767,
     769,   771,   773,   775,   777,   779,   781,   784,   787,   789,
     794,   799,   801,   806,   808,   813,   814,   819,   820,   827,
     831,   832,   839,   843,   844,   846,   848,   851,   858,   860,
     864,   865,   867,   872,   879,   884,   886,   888,   890,   892,
     894,   896,   898,   899,   904,   906,   907,   910,   912,   916,
     920,   923,   924,   929,   931,   932,   937,   939,   941,   943,
     946,   949,   955,   959,   960,   961,   968,   969,   970,   977,
     979,   981,   986,   990,   993,   997,   999,  1001,  1003,  1005,
    1009,  1012,  1014,  1016,  1020,  1023,  1027,  1031,  1036,  1040,
    1045,  1049,  1052,  1054,  1056,  1059,  1061,  1064,  1066,  1069,
    1070,  1078,  1084,  1085,  1093,  1099,  1100,  1109,  1110,  1118,
    1121,  1124,  1127,  1128,  1130,  1131,  1133,  1135,  1138,  1139,
    1143,  1146,  1151,  1155,  1158,  1162,  1164,  1166,  1169,  1171,
    1176,  1178,  1183,  1186,  1191,  1195,  1198,  1203,  1207,  1209,
    1213,  1215,  1217,  1221,  1222,  1226,  1227,  1229,  1230,  1232,
    1235,  1237,  1239,  1241,  1245,  1248,  1252,  1257,  1261,  1264,
    1267,  1269,  1274,  1278,  1283,  1289,  1295,  1297,  1299,  1301,
    1303,  1305,  1308,  1311,  1314,  1317,  1319,  1322,  1325,  1328,
    1330,  1333,  1336,  1339,  1342,  1344,  1347,  1349,  1351,  1353,
    1355,  1358,  1359,  1360,  1361,  1362,  1363,  1365,  1367,  1370,
    1374,  1376,  1379,  1381,  1383,  1389,  1391,  1393,  1396,  1399,
    1402,  1405,  1406,  1412,  1413,  1418,  1419,  1421,  1424,  1428,
    1431,  1434,  1435,  1440,  1442,  1446,  1447,  1448,  1456,  1462,
    1465,  1466,  1467,  1468,  1469,  1482,  1483,  1490,  1493,  1495,
    1497,  1500,  1504,  1507,  1510,  1513,  1517,  1524,  1533,  1544,
    1557,  1561,  1566,  1568,  1572,  1575,  1576,  1580,  1581,  1588,
    1589,  1593,  1595,  1596,  1600,  1603,  1605,  1606,  1613,  1616,
    1617,  1621,  1627,  1630,  1635,  1636,  1638,  1639,  1641,  1642,
    1644,  1646,  1650,  1655,  1663,  1665,  1669,  1670,  1674,  1677,
    1678,  1679,  1686,  1689,  1690,  1692,  1694,  1698,  1700,  1704,
    1709,  1714,  1718,  1723,  1727,  1732,  1737,  1741,  1746,  1750,
    1752,  1753,  1757,  1759,  1762,  1764,  1768,  1770,  1774,  1776,
    1778,  1780,  1782,  1784,  1786,  1788,  1790,  1794,  1798,  1803,
    1806,  1807,  1811,  1812,  1813,  1814,  1824,  1825,  1831,  1832,
    1842,  1848,  1849,  1856,  1860,  1861,  1863,  1867,  1871,  1873,
    1875,  1877,  1879,  1880,  1884,  1887,  1891,  1895,  1897,  1898,
    1900,  1905,  1907,  1911,  1914,  1916,  1918,  1919,  1920,  1921,
    1929,  1930,  1933,  1934,  1938,  1940,  1942,  1943,  1944,  1950,
    1955,  1957,  1963,  1966,  1967,  1970,  1971,  1973,  1975,  1977,
    1980,  1983,  1988,  1991,  1994,  1996,  2000,  2003,  2006,  2008,
    2009,  2012,  2013,  2017,  2019,  2021,  2024,  2026,  2028,  2030,
    2032,  2034,  2036,  2038,  2040,  2042,  2044,  2046,  2048,  2050,
    2052,  2054,  2056,  2058,  2060,  2062,  2064,  2066,  2068,  2070,
    2072,  2074,  2076,  2083,  2087,  2093,  2096,  2098,  2100,  2102,
    2105,  2107,  2111,  2114,  2116,  2118,  2120,  2125,  2127,  2129,
    2131,  2134,  2137,  2139,  2144,  2149
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short yyrhs[] =
{
      98,     0,    -1,    -1,    99,    -1,    -1,   100,   102,    -1,
      -1,    99,   101,   102,    -1,   103,    -1,   105,    -1,   104,
      -1,   308,    -1,    28,    66,   114,    89,    90,    -1,   307,
     102,    -1,   136,   170,    90,    -1,   156,   136,   170,    90,
      -1,   155,   136,   169,    90,    -1,   162,    90,    -1,     1,
      90,    -1,     1,    91,    -1,    90,    -1,    -1,    -1,   155,
     136,   199,   106,   130,   259,   107,   248,    -1,   155,   136,
     199,     1,    -1,    -1,    -1,   156,   136,   204,   108,   130,
     259,   109,   248,    -1,   156,   136,   204,     1,    -1,    -1,
      -1,   136,   204,   110,   130,   259,   111,   248,    -1,   136,
     204,     1,    -1,     3,    -1,     4,    -1,    80,    -1,    75,
      -1,    51,    -1,    57,    -1,    56,    -1,    62,    -1,    61,
      -1,    92,    -1,    93,    -1,   116,    -1,    -1,   116,    -1,
     122,    -1,   116,    94,   122,    -1,   128,    -1,    58,   121,
      -1,   307,   121,    -1,   113,   121,    -1,    48,   112,    -1,
     118,   117,    -1,   118,    66,   225,    89,    -1,   119,   117,
      -1,   119,    66,   225,    89,    -1,    34,   121,    -1,    35,
     121,    -1,    12,    -1,    30,    -1,    29,    -1,   117,    -1,
      66,   225,    89,   121,    -1,   121,    -1,   122,    56,   122,
      -1,   122,    57,   122,    -1,   122,    58,   122,    -1,   122,
      59,   122,    -1,   122,    60,   122,    -1,   122,    55,   122,
      -1,   122,    54,   122,    -1,   122,    53,   122,    -1,   122,
      52,   122,    -1,   122,    51,   122,    -1,   122,    49,   122,
      -1,   122,    50,   122,    -1,    -1,   122,    48,   123,   122,
      -1,    -1,   122,    47,   124,   122,    -1,    -1,    -1,   122,
      45,   125,   114,    46,   126,   122,    -1,    -1,   122,    45,
     127,    46,   122,    -1,   122,    43,   122,    -1,   122,    44,
     122,    -1,     3,    -1,     9,    -1,    10,    -1,    42,    -1,
      -1,    66,   225,    89,    95,   129,   185,    91,    -1,    66,
     114,    89,    -1,    66,     1,    89,    -1,   252,   250,    89,
      -1,   252,     1,    89,    -1,   128,    66,   115,    89,    -1,
      36,    66,   122,    94,   225,    89,    -1,    37,    66,   122,
      94,   122,    94,   122,    89,    -1,    38,    66,   225,    94,
     225,    89,    -1,   128,    67,   114,    96,    -1,   128,    65,
     112,    -1,   128,    68,   112,    -1,   128,    62,    -1,   128,
      61,    -1,   359,    -1,   363,    -1,   364,    -1,   365,    -1,
      88,    -1,   131,    -1,    -1,   133,    -1,   259,   134,    -1,
     132,    -1,   240,    -1,   133,   132,    -1,   132,   240,    -1,
     157,   136,   169,    90,    -1,   158,   136,   170,    90,    -1,
     157,    90,    -1,   158,    90,    -1,   259,   138,    -1,    -1,
     176,    -1,   155,   136,   169,    90,    -1,   156,   136,   170,
      90,    -1,   155,   136,   193,    -1,   156,   136,   196,    -1,
     162,    90,    -1,   307,   138,    -1,     8,    -1,   139,     8,
      -1,   140,     8,    -1,   139,   177,    -1,   141,     8,    -1,
     142,     8,    -1,   177,    -1,   141,   177,    -1,   164,    -1,
     143,     8,    -1,   144,     8,    -1,   143,   166,    -1,   144,
     166,    -1,   139,   164,    -1,   140,   164,    -1,   165,    -1,
     143,   177,    -1,   143,   167,    -1,   144,   167,    -1,   139,
     165,    -1,   140,   165,    -1,   145,     8,    -1,   146,     8,
      -1,   145,   166,    -1,   146,   166,    -1,   141,   164,    -1,
     142,   164,    -1,   145,   177,    -1,   145,   167,    -1,   146,
     167,    -1,   141,   165,    -1,   142,   165,    -1,   182,    -1,
     147,     8,    -1,   148,     8,    -1,   139,   182,    -1,   140,
     182,    -1,   147,   182,    -1,   148,   182,    -1,   147,   177,
      -1,   149,     8,    -1,   150,     8,    -1,   141,   182,    -1,
     142,   182,    -1,   149,   182,    -1,   150,   182,    -1,   149,
     177,    -1,   151,     8,    -1,   152,     8,    -1,   151,   166,
      -1,   152,   166,    -1,   147,   164,    -1,   148,   164,    -1,
     143,   182,    -1,   144,   182,    -1,   151,   182,    -1,   152,
     182,    -1,   151,   177,    -1,   151,   167,    -1,   152,   167,
      -1,   147,   165,    -1,   148,   165,    -1,   153,     8,    -1,
     154,     8,    -1,   153,   166,    -1,   154,   166,    -1,   149,
     164,    -1,   150,   164,    -1,   145,   182,    -1,   146,   182,
      -1,   153,   182,    -1,   154,   182,    -1,   153,   177,    -1,
     153,   167,    -1,   154,   167,    -1,   149,   165,    -1,   150,
     165,    -1,   143,    -1,   144,    -1,   145,    -1,   146,    -1,
     151,    -1,   152,    -1,   153,    -1,   154,    -1,   139,    -1,
     140,    -1,   141,    -1,   142,    -1,   147,    -1,   148,    -1,
     149,    -1,   150,    -1,   143,    -1,   144,    -1,   151,    -1,
     152,    -1,   139,    -1,   140,    -1,   147,    -1,   148,    -1,
     143,    -1,   144,    -1,   145,    -1,   146,    -1,   139,    -1,
     140,    -1,   141,    -1,   142,    -1,   143,    -1,   144,    -1,
     145,    -1,   146,    -1,   139,    -1,   140,    -1,   141,    -1,
     142,    -1,   139,    -1,   140,    -1,   141,    -1,   142,    -1,
     143,    -1,   144,    -1,   145,    -1,   146,    -1,   147,    -1,
     148,    -1,   149,    -1,   150,    -1,   151,    -1,   152,    -1,
     153,    -1,   154,    -1,    -1,   160,    -1,   166,    -1,   168,
      -1,   167,    -1,     7,    -1,   213,    -1,   208,    -1,     4,
      -1,    75,   321,    -1,    80,   321,    -1,   322,    -1,   120,
      66,   114,    89,    -1,   120,    66,   225,    89,    -1,   172,
      -1,   169,    94,   137,   172,    -1,   174,    -1,   170,    94,
     137,   174,    -1,    -1,    28,    66,    10,    89,    -1,    -1,
     199,   171,   176,    43,   173,   183,    -1,   199,   171,   176,
      -1,    -1,   204,   171,   176,    43,   175,   183,    -1,   204,
     171,   176,    -1,    -1,   177,    -1,   178,    -1,   177,   178,
      -1,    31,    66,    66,   179,    89,    89,    -1,   180,    -1,
     179,    94,   180,    -1,    -1,   181,    -1,   181,    66,     3,
      89,    -1,   181,    66,     3,    94,   116,    89,    -1,   181,
      66,   115,    89,    -1,   112,    -1,   182,    -1,     7,    -1,
       8,    -1,     6,    -1,     5,    -1,   122,    -1,    -1,    95,
     184,   185,    91,    -1,     1,    -1,    -1,   186,   214,    -1,
     187,    -1,   186,    94,   187,    -1,   191,    43,   189,    -1,
     192,   189,    -1,    -1,   112,    46,   188,   189,    -1,   189,
      -1,    -1,    95,   190,   185,    91,    -1,   122,    -1,     1,
      -1,   192,    -1,   191,   192,    -1,    65,   112,    -1,    67,
     122,    11,   122,    96,    -1,    67,   122,    96,    -1,    -1,
      -1,   199,   194,   130,   259,   195,   253,    -1,    -1,    -1,
     204,   197,   130,   259,   198,   253,    -1,   200,    -1,   204,
      -1,    66,   176,   200,    89,    -1,   200,    66,   302,    -1,
     200,   233,    -1,    58,   163,   200,    -1,     4,    -1,    80,
      -1,   202,    -1,   203,    -1,   202,    66,   302,    -1,   202,
     233,    -1,     4,    -1,    80,    -1,   203,    66,   302,    -1,
     203,   233,    -1,    58,   163,   202,    -1,    58,   163,   203,
      -1,    66,   176,   203,    89,    -1,   204,    66,   302,    -1,
      66,   176,   204,    89,    -1,    58,   163,   204,    -1,   204,
     233,    -1,     3,    -1,    14,    -1,    14,   177,    -1,    15,
      -1,    15,   177,    -1,    13,    -1,    13,   177,    -1,    -1,
     205,   112,    95,   209,   216,    91,   176,    -1,   205,    95,
     216,    91,   176,    -1,    -1,   206,   112,    95,   210,   216,
      91,   176,    -1,   206,    95,   216,    91,   176,    -1,    -1,
     207,   112,    95,   211,   223,   215,    91,   176,    -1,    -1,
     207,    95,   212,   223,   215,    91,   176,    -1,   205,   112,
      -1,   206,   112,    -1,   207,   112,    -1,    -1,    94,    -1,
      -1,    94,    -1,   217,    -1,   217,   218,    -1,    -1,   217,
     218,    90,    -1,   217,    90,    -1,    73,    66,    75,    89,
      -1,   159,   136,   219,    -1,   159,   136,    -1,   160,   136,
     220,    -1,   160,    -1,     1,    -1,   307,   218,    -1,   221,
      -1,   219,    94,   137,   221,    -1,   222,    -1,   220,    94,
     137,   222,    -1,   199,   176,    -1,   199,    46,   122,   176,
      -1,    46,   122,   176,    -1,   204,   176,    -1,   204,    46,
     122,   176,    -1,    46,   122,   176,    -1,   224,    -1,   223,
      94,   224,    -1,     1,    -1,   112,    -1,   112,    43,   122,
      -1,    -1,   161,   226,   227,    -1,    -1,   229,    -1,    -1,
     229,    -1,   230,   177,    -1,   231,    -1,   230,    -1,   232,
      -1,    58,   163,   230,    -1,    58,   163,    -1,    58,   163,
     231,    -1,    66,   176,   229,    89,    -1,   232,    66,   292,
      -1,   232,   233,    -1,    66,   292,    -1,   233,    -1,    67,
     163,   122,    96,    -1,    67,   163,    96,    -1,    67,   163,
      58,    96,    -1,    67,     6,   163,   122,    96,    -1,    67,
     160,     6,   122,    96,    -1,   235,    -1,   236,    -1,   237,
      -1,   238,    -1,   262,    -1,   235,   262,    -1,   236,   262,
      -1,   237,   262,    -1,   238,   262,    -1,   135,    -1,   235,
     135,    -1,   236,   135,    -1,   238,   135,    -1,   263,    -1,
     235,   263,    -1,   236,   263,    -1,   237,   263,    -1,   238,
     263,    -1,   240,    -1,   239,   240,    -1,   235,    -1,   236,
      -1,   237,    -1,   238,    -1,     1,    90,    -1,    -1,    -1,
      -1,    -1,    -1,   246,    -1,   247,    -1,   246,   247,    -1,
      33,   306,    90,    -1,   253,    -1,     1,   253,    -1,    95,
      -1,    91,    -1,   241,   245,   251,    91,   242,    -1,   234,
      -1,     1,    -1,    66,    95,    -1,   249,   250,    -1,   255,
     261,    -1,   255,     1,    -1,    -1,    16,   256,    66,   114,
      89,    -1,    -1,    19,   258,   261,    18,    -1,    -1,   262,
      -1,   263,   260,    -1,   243,   260,   244,    -1,   259,   274,
      -1,   259,   285,    -1,    -1,   254,    17,   265,   261,    -1,
     254,    -1,   254,    17,     1,    -1,    -1,    -1,    18,   266,
      66,   114,    89,   267,   261,    -1,   257,    66,   114,    89,
      90,    -1,   257,     1,    -1,    -1,    -1,    -1,    -1,    20,
     268,    66,   273,   269,   287,    90,   270,   287,    89,   271,
     261,    -1,    -1,    21,    66,   114,    89,   272,   261,    -1,
     287,    90,    -1,   138,    -1,   253,    -1,   114,    90,    -1,
     243,   264,   244,    -1,    24,    90,    -1,    25,    90,    -1,
      26,    90,    -1,    26,   114,    90,    -1,    28,   286,    66,
     114,    89,    90,    -1,    28,   286,    66,   114,    46,   288,
      89,    90,    -1,    28,   286,    66,   114,    46,   288,    46,
     288,    89,    90,    -1,    28,   286,    66,   114,    46,   288,
      46,   288,    46,   291,    89,    90,    -1,    27,   112,    90,
      -1,    27,    58,   114,    90,    -1,    90,    -1,    83,   114,
      90,    -1,    83,    90,    -1,    -1,   277,   275,   284,    -1,
      -1,    87,    66,   114,    89,   276,   253,    -1,    -1,   279,
     278,   281,    -1,   279,    -1,    -1,    84,   280,   253,    -1,
     281,   282,    -1,   282,    -1,    -1,    85,    66,   299,    89,
     283,   253,    -1,    86,   253,    -1,    -1,    22,   122,    46,
      -1,    22,   122,    11,   122,    46,    -1,    23,    46,    -1,
     112,   259,    46,   176,    -1,    -1,     8,    -1,    -1,   114,
      -1,    -1,   289,    -1,   290,    -1,   289,    94,   290,    -1,
      10,    66,   114,    89,    -1,    67,   112,    96,    10,    66,
     114,    89,    -1,    10,    -1,   291,    94,    10,    -1,    -1,
     176,   293,   294,    -1,   297,    89,    -1,    -1,    -1,   298,
      90,   295,   176,   296,   294,    -1,     1,    89,    -1,    -1,
      11,    -1,   298,    -1,   298,    94,    11,    -1,   300,    -1,
     298,    94,   299,    -1,   155,   136,   201,   176,    -1,   155,
     136,   204,   176,    -1,   155,   136,   228,    -1,   156,   136,
     204,   176,    -1,   156,   136,   228,    -1,   157,   301,   201,
     176,    -1,   157,   301,   204,   176,    -1,   157,   301,   228,
      -1,   158,   301,   204,   176,    -1,   158,   301,   228,    -1,
     136,    -1,    -1,   176,   303,   304,    -1,   294,    -1,   305,
      89,    -1,     3,    -1,   305,    94,     3,    -1,   112,    -1,
     306,    94,   112,    -1,    32,    -1,   314,    -1,   310,    -1,
     311,    -1,   319,    -1,   330,    -1,    71,    -1,   112,    -1,
     309,    94,   112,    -1,    81,   309,    90,    -1,    82,   112,
     112,    90,    -1,    46,   112,    -1,    -1,    95,   323,    91,
      -1,    -1,    -1,    -1,    69,   112,   312,   321,   315,   313,
     316,   334,    71,    -1,    -1,    70,   112,   312,   317,   313,
      -1,    -1,    69,   112,    66,   112,    89,   321,   318,   334,
      71,    -1,    70,   112,    66,   112,    89,    -1,    -1,    79,
     112,   321,   320,   334,    71,    -1,    79,   309,    90,    -1,
      -1,   322,    -1,    53,   309,    53,    -1,   323,   324,   325,
      -1,   325,    -1,    77,    -1,    78,    -1,    76,    -1,    -1,
     325,   326,    90,    -1,   325,    90,    -1,   159,   136,   327,
      -1,   160,   136,   327,    -1,     1,    -1,    -1,   328,    -1,
     327,    94,   137,   328,    -1,   199,    -1,   199,    46,   122,
      -1,    46,   122,    -1,    56,    -1,    57,    -1,    -1,    -1,
      -1,   329,   331,   340,   332,   341,   333,   248,    -1,    -1,
     334,   337,    -1,    -1,   334,   335,   104,    -1,    90,    -1,
       1,    -1,    -1,    -1,   329,   338,   340,   339,   336,    -1,
      66,   225,    89,   349,    -1,   349,    -1,    66,   225,    89,
     350,   347,    -1,   350,   347,    -1,    -1,    90,   342,    -1,
      -1,   343,    -1,   344,    -1,   240,    -1,   343,   344,    -1,
     344,   240,    -1,   155,   136,   345,    90,    -1,   155,    90,
      -1,   156,    90,    -1,   346,    -1,   345,    94,   346,    -1,
     201,   176,    -1,   204,   176,    -1,   228,    -1,    -1,    94,
      11,    -1,    -1,    94,   348,   297,    -1,   351,    -1,   353,
      -1,   350,   353,    -1,     3,    -1,     4,    -1,    75,    -1,
      80,    -1,   352,    -1,    13,    -1,    14,    -1,    15,    -1,
      16,    -1,    17,    -1,    18,    -1,    19,    -1,    20,    -1,
      21,    -1,    22,    -1,    23,    -1,    24,    -1,    25,    -1,
      26,    -1,    27,    -1,    28,    -1,    12,    -1,    29,    -1,
      30,    -1,     7,    -1,     8,    -1,   351,    46,    66,   225,
      89,   112,    -1,   351,    46,   112,    -1,    46,    66,   225,
      89,   112,    -1,    46,   112,    -1,   351,    -1,   355,    -1,
     357,    -1,   355,   357,    -1,   116,    -1,   351,    46,   356,
      -1,    46,   356,    -1,   114,    -1,    75,    -1,     4,    -1,
      67,   358,   354,    96,    -1,   351,    -1,   361,    -1,   362,
      -1,   361,   362,    -1,   351,    46,    -1,    46,    -1,    72,
      66,   360,    89,    -1,    79,    66,   112,    89,    -1,    74,
      66,   225,    89,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,   343,   343,   346,   354,   354,   355,   355,   359,   364,
     365,   366,   367,   375,   380,   387,   389,   391,   393,   394,
     395,   402,   407,   401,   412,   415,   420,   414,   425,   428,
     433,   427,   438,   443,   444,   445,   446,   449,   451,   453,
     456,   458,   460,   462,   466,   472,   473,   477,   479,   484,
     485,   488,   491,   495,   497,   503,   506,   509,   512,   514,
     519,   523,   527,   531,   532,   537,   538,   540,   542,   544,
     546,   548,   550,   552,   554,   556,   558,   560,   563,   562,
     570,   569,   577,   581,   576,   587,   586,   597,   604,   616,
     622,   623,   624,   627,   626,   639,   644,   646,   660,   666,
     668,   671,   681,   691,   693,   700,   709,   711,   713,   715,
     717,   719,   721,   726,   734,   743,   754,   759,   760,   761,
     762,   770,   772,   774,   777,   786,   795,   805,   810,   812,
     814,   816,   818,   820,   877,   880,   883,   889,   895,   898,
     904,   907,   913,   916,   919,   922,   925,   928,   931,   937,
     940,   943,   946,   949,   952,   958,   961,   964,   967,   970,
     973,   979,   982,   985,   988,   991,   997,  1000,  1003,  1006,
    1012,  1018,  1024,  1033,  1039,  1042,  1045,  1051,  1057,  1063,
    1072,  1078,  1081,  1084,  1087,  1090,  1093,  1096,  1102,  1108,
    1114,  1123,  1126,  1129,  1132,  1135,  1141,  1144,  1147,  1150,
    1153,  1156,  1159,  1165,  1171,  1177,  1186,  1189,  1192,  1195,
    1198,  1205,  1206,  1207,  1208,  1209,  1210,  1211,  1212,  1216,
    1217,  1218,  1219,  1220,  1221,  1222,  1223,  1227,  1228,  1229,
    1230,  1234,  1235,  1236,  1237,  1241,  1242,  1243,  1244,  1248,
    1249,  1250,  1251,  1255,  1256,  1257,  1258,  1259,  1260,  1261,
    1262,  1266,  1267,  1268,  1269,  1270,  1271,  1272,  1273,  1274,
    1275,  1276,  1277,  1278,  1279,  1280,  1281,  1287,  1288,  1314,
    1315,  1319,  1323,  1325,  1329,  1333,  1337,  1339,  1344,  1346,
    1352,  1359,  1360,  1364,  1365,  1370,  1371,  1377,  1376,  1384,
    1393,  1392,  1400,  1409,  1410,  1415,  1417,  1422,  1427,  1429,
    1435,  1436,  1438,  1440,  1442,  1450,  1451,  1452,  1453,  1457,
    1458,  1464,  1466,  1465,  1469,  1476,  1478,  1482,  1483,  1489,
    1492,  1496,  1495,  1501,  1506,  1505,  1509,  1511,  1515,  1516,
    1520,  1522,  1526,  1532,  1545,  1531,  1563,  1576,  1562,  1596,
    1597,  1603,  1605,  1610,  1612,  1614,  1615,  1623,  1624,  1628,
    1633,  1635,  1636,  1640,  1645,  1647,  1649,  1651,  1659,  1664,
    1666,  1668,  1670,  1674,  1676,  1681,  1683,  1688,  1690,  1702,
    1701,  1708,  1713,  1712,  1717,  1722,  1721,  1727,  1726,  1734,
    1736,  1738,  1746,  1748,  1751,  1753,  1771,  1773,  1779,  1780,
    1782,  1786,  1791,  1794,  1804,  1807,  1812,  1814,  1820,  1821,
    1826,  1827,  1832,  1836,  1840,  1847,  1851,  1855,  1865,  1866,
    1871,  1877,  1879,  1885,  1884,  1893,  1894,  1899,  1902,  1906,
    1913,  1914,  1918,  1919,  1924,  1926,  1931,  1933,  1935,  1937,
    1939,  1946,  1948,  1950,  1952,  1955,  1966,  1967,  1968,  1972,
    1976,  1977,  1978,  1979,  1980,  1984,  1985,  1991,  1992,  1996,
    1997,  1998,  1999,  2000,  2004,  2005,  2009,  2010,  2011,  2012,
    2015,  2019,  2026,  2035,  2051,  2065,  2067,  2073,  2074,  2078,
    2092,  2094,  2097,  2101,  2103,  2111,  2112,  2116,  2133,  2141,
    2146,  2159,  2158,  2172,  2171,  2194,  2200,  2201,  2206,  2212,
    2226,  2236,  2235,  2243,  2255,  2266,  2269,  2265,  2277,  2280,
    2283,  2287,  2290,  2294,  2282,  2300,  2299,  2309,  2311,  2317,
    2319,  2322,  2326,  2335,  2344,  2347,  2350,  2354,  2358,  2363,
    2367,  2379,  2385,  2387,  2391,  2396,  2395,  2400,  2399,  2407,
    2406,  2410,  2417,  2416,  2422,  2423,  2428,  2427,  2434,  2437,
    2444,  2447,  2450,  2453,  2470,  2471,  2477,  2478,  2484,  2485,
    2489,  2490,  2495,  2497,  2504,  2506,  2517,  2516,  2525,  2527,
    2529,  2526,  2533,  2540,  2541,  2552,  2556,  2563,  2565,  2572,
    2577,  2582,  2585,  2591,  2599,  2604,  2609,  2612,  2618,  2624,
    2634,  2633,  2642,  2643,  2661,  2663,  2669,  2671,  2676,  2687,
    2688,  2689,  2690,  2691,  2692,  2707,  2709,  2714,  2721,  2728,
    2729,  2733,  2734,  2739,  2745,  2738,  2755,  2754,  2767,  2766,
    2778,  2789,  2788,  2803,  2811,  2814,  2818,  2828,  2829,  2833,
    2834,  2835,  2840,  2843,  2844,  2862,  2865,  2868,  2874,  2875,
    2876,  2880,  2887,  2893,  2903,  2905,  2911,  2917,  2925,  2910,
    2938,  2940,  2941,  2941,  2945,  2946,  2951,  2956,  2950,  2967,
    2972,  2977,  2982,  2991,  2993,  2999,  3001,  3005,  3006,  3007,
    3008,  3012,  3014,  3016,  3021,  3023,  3031,  3035,  3039,  3045,
    3048,  3054,  3053,  3066,  3070,  3072,  3079,  3080,  3081,  3082,
    3083,  3087,  3087,  3087,  3087,  3087,  3087,  3087,  3087,  3088,
    3088,  3088,  3088,  3088,  3088,  3089,  3089,  3089,  3089,  3089,
    3090,  3090,  3094,  3099,  3104,  3109,  3116,  3117,  3121,  3122,
    3130,  3142,  3146,  3153,  3154,  3158,  3165,  3170,  3171,  3175,
    3176,  3183,  3187,  3194,  3201,  3210
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENTIFIER", "TYPENAME", "SCSPEC", 
  "STATIC", "TYPESPEC", "TYPE_QUAL", "CONSTANT", "STRING", "ELLIPSIS", 
  "SIZEOF", "ENUM", "STRUCT", "UNION", "IF", "ELSE", "WHILE", "DO", "FOR", 
  "SWITCH", "CASE", "DEFAULT", "BREAK", "CONTINUE", "RETURN", "GOTO", 
  "ASM_KEYWORD", "TYPEOF", "ALIGNOF", "ATTRIBUTE", "EXTENSION", "LABEL", 
  "REALPART", "IMAGPART", "VA_ARG", "CHOOSE_EXPR", "TYPES_COMPATIBLE_P", 
  "PTR_VALUE", "PTR_BASE", "PTR_EXTENT", "FUNC_NAME", "'='", "ASSIGN", 
  "'?'", "':'", "OROR", "ANDAND", "'|'", "'^'", "'&'", "EQCOMPARE", 
  "ARITHCOMPARE", "RSHIFT", "LSHIFT", "'+'", "'-'", "'*'", "'/'", "'%'", 
  "MINUSMINUS", "PLUSPLUS", "UNARY", "HYPERUNARY", "'.'", "'('", "'['", 
  "POINTSAT", "INTERFACE", "IMPLEMENTATION", "END", "SELECTOR", "DEFS", 
  "ENCODE", "CLASSNAME", "PUBLIC", "PRIVATE", "PROTECTED", "PROTOCOL", 
  "OBJECTNAME", "CLASS", "ALIAS", "AT_THROW", "AT_TRY", "AT_CATCH", 
  "AT_FINALLY", "AT_SYNCHRONIZED", "OBJC_STRING", "')'", "';'", "'}'", 
  "'~'", "'!'", "','", "'{'", "']'", "$accept", "program", "extdefs", 
  "@1", "@2", "extdef", "extdef_1", "datadef", "fndef", "@3", "@4", "@5", 
  "@6", "@7", "@8", "identifier", "unop", "expr", "exprlist", 
  "nonnull_exprlist", "unary_expr", "sizeof", "alignof", "typeof", 
  "cast_expr", "expr_no_commas", "@9", "@10", "@11", "@12", "@13", 
  "primary", "@14", "old_style_parm_decls", "old_style_parm_decls_1", 
  "lineno_datadecl", "datadecls", "datadecl", "lineno_decl", "setspecs", 
  "maybe_resetattrs", "decl", "declspecs_nosc_nots_nosa_noea", 
  "declspecs_nosc_nots_nosa_ea", "declspecs_nosc_nots_sa_noea", 
  "declspecs_nosc_nots_sa_ea", "declspecs_nosc_ts_nosa_noea", 
  "declspecs_nosc_ts_nosa_ea", "declspecs_nosc_ts_sa_noea", 
  "declspecs_nosc_ts_sa_ea", "declspecs_sc_nots_nosa_noea", 
  "declspecs_sc_nots_nosa_ea", "declspecs_sc_nots_sa_noea", 
  "declspecs_sc_nots_sa_ea", "declspecs_sc_ts_nosa_noea", 
  "declspecs_sc_ts_nosa_ea", "declspecs_sc_ts_sa_noea", 
  "declspecs_sc_ts_sa_ea", "declspecs_ts", "declspecs_nots", 
  "declspecs_ts_nosa", "declspecs_nots_nosa", "declspecs_nosc_ts", 
  "declspecs_nosc_nots", "declspecs_nosc", "declspecs", 
  "maybe_type_quals_attrs", "typespec_nonattr", "typespec_attr", 
  "typespec_reserved_nonattr", "typespec_reserved_attr", 
  "typespec_nonreserved_nonattr", "initdecls", "notype_initdecls", 
  "maybeasm", "initdcl", "@15", "notype_initdcl", "@16", 
  "maybe_attribute", "attributes", "attribute", "attribute_list", 
  "attrib", "any_word", "scspec", "init", "@17", "initlist_maybe_comma", 
  "initlist1", "initelt", "@18", "initval", "@19", "designator_list", 
  "designator", "nested_function", "@20", "@21", "notype_nested_function", 
  "@22", "@23", "declarator", "after_type_declarator", "parm_declarator", 
  "parm_declarator_starttypename", "parm_declarator_nostarttypename", 
  "notype_declarator", "struct_head", "union_head", "enum_head", 
  "structsp_attr", "@24", "@25", "@26", "@27", "structsp_nonattr", 
  "maybecomma", "maybecomma_warn", "component_decl_list", 
  "component_decl_list2", "component_decl", "components", 
  "components_notype", "component_declarator", 
  "component_notype_declarator", "enumlist", "enumerator", "typename", 
  "@28", "absdcl", "absdcl_maybe_attribute", "absdcl1", "absdcl1_noea", 
  "absdcl1_ea", "direct_absdcl1", "array_declarator", "stmts_and_decls", 
  "lineno_stmt_decl_or_labels_ending_stmt", 
  "lineno_stmt_decl_or_labels_ending_decl", 
  "lineno_stmt_decl_or_labels_ending_label", 
  "lineno_stmt_decl_or_labels_ending_error", "lineno_stmt_decl_or_labels", 
  "errstmt", "pushlevel", "poplevel", "c99_block_start", "c99_block_end", 
  "maybe_label_decls", "label_decls", "label_decl", "compstmt_or_error", 
  "compstmt_start", "compstmt_nostart", "compstmt_contents_nonempty", 
  "compstmt_primary_start", "compstmt", "simple_if", "if_prefix", "@29", 
  "do_stmt_start", "@30", "save_location", "lineno_labeled_stmt", 
  "c99_block_lineno_labeled_stmt", "lineno_stmt", "lineno_label", 
  "select_or_iter_stmt", "@31", "@32", "@33", "@34", "@35", "@36", "@37", 
  "@38", "for_init_stmt", "stmt", "@39", "@40", "objc_try_catch_stmt", 
  "@41", "objc_try_stmt", "@42", "objc_catch_list", "objc_catch_block", 
  "@43", "objc_finally_block", "label", "maybe_type_qual", "xexpr", 
  "asm_operands", "nonnull_asm_operands", "asm_operand", "asm_clobbers", 
  "parmlist", "@44", "parmlist_1", "@45", "@46", "parmlist_2", "parms", 
  "parm", "firstparm", "setspecs_fp", "parmlist_or_identifiers", "@47", 
  "parmlist_or_identifiers_1", "identifiers", "identifiers_or_typenames", 
  "extension", "objcdef", "identifier_list", "classdecl", "aliasdecl", 
  "superclass", "class_ivars", "classdef", "@48", "@49", "@50", "@51", 
  "protocoldef", "@52", "protocolrefs", "non_empty_protocolrefs", 
  "ivar_decl_list", "visibility_spec", "ivar_decls", "ivar_decl", "ivars", 
  "ivar_declarator", "methodtype", "methoddef", "@53", "@54", "@55", 
  "methodprotolist", "@56", "semi_or_error", "methodproto", "@57", "@58", 
  "methoddecl", "optarglist", "myxdecls", "mydecls", "mydecl", "myparms", 
  "myparm", "optparmlist", "@59", "unaryselector", "keywordselector", 
  "selector", "reservedwords", "keyworddecl", "messageargs", 
  "keywordarglist", "keywordexpr", "keywordarg", "receiver", 
  "objcmessageexpr", "selectorarg", "keywordnamelist", "keywordname", 
  "objcselectorexpr", "objcprotocolexpr", "objcencodeexpr", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,    61,   298,    63,    58,   299,   300,   124,
      94,    38,   301,   302,   303,   304,    43,    45,    42,    47,
      37,   305,   306,   307,   308,    46,    40,    91,   309,   310,
     311,   312,   313,   314,   315,   316,   317,   318,   319,   320,
     321,   322,   323,   324,   325,   326,   327,   328,   329,    41,
      59,   125,   126,    33,    44,   123,    93
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned short yyr1[] =
{
       0,    97,    98,    98,   100,    99,   101,    99,   102,   103,
     103,   103,   103,   103,   104,   104,   104,   104,   104,   104,
     104,   106,   107,   105,   105,   108,   109,   105,   105,   110,
     111,   105,   105,   112,   112,   112,   112,   113,   113,   113,
     113,   113,   113,   113,   114,   115,   115,   116,   116,   117,
     117,   117,   117,   117,   117,   117,   117,   117,   117,   117,
     118,   119,   120,   121,   121,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   123,   122,
     124,   122,   125,   126,   122,   127,   122,   122,   122,   128,
     128,   128,   128,   129,   128,   128,   128,   128,   128,   128,
     128,   128,   128,   128,   128,   128,   128,   128,   128,   128,
     128,   128,   128,   130,   131,   131,   132,   133,   133,   133,
     133,   134,   134,   134,   134,   135,   136,   137,   138,   138,
     138,   138,   138,   138,   139,   139,   139,   140,   141,   141,
     142,   142,   143,   143,   143,   143,   143,   143,   143,   144,
     144,   144,   144,   144,   144,   145,   145,   145,   145,   145,
     145,   146,   146,   146,   146,   146,   147,   147,   147,   147,
     147,   147,   147,   148,   149,   149,   149,   149,   149,   149,
     150,   151,   151,   151,   151,   151,   151,   151,   151,   151,
     151,   152,   152,   152,   152,   152,   153,   153,   153,   153,
     153,   153,   153,   153,   153,   153,   154,   154,   154,   154,
     154,   155,   155,   155,   155,   155,   155,   155,   155,   156,
     156,   156,   156,   156,   156,   156,   156,   157,   157,   157,
     157,   158,   158,   158,   158,   159,   159,   159,   159,   160,
     160,   160,   160,   161,   161,   161,   161,   161,   161,   161,
     161,   162,   162,   162,   162,   162,   162,   162,   162,   162,
     162,   162,   162,   162,   162,   162,   162,   163,   163,   164,
     164,   165,   166,   166,   167,   168,   168,   168,   168,   168,
     168,   169,   169,   170,   170,   171,   171,   173,   172,   172,
     175,   174,   174,   176,   176,   177,   177,   178,   179,   179,
     180,   180,   180,   180,   180,   181,   181,   181,   181,   182,
     182,   183,   184,   183,   183,   185,   185,   186,   186,   187,
     187,   188,   187,   187,   190,   189,   189,   189,   191,   191,
     192,   192,   192,   194,   195,   193,   197,   198,   196,   199,
     199,   200,   200,   200,   200,   200,   200,   201,   201,   202,
     202,   202,   202,   203,   203,   203,   203,   203,   204,   204,
     204,   204,   204,   205,   205,   206,   206,   207,   207,   209,
     208,   208,   210,   208,   208,   211,   208,   212,   208,   213,
     213,   213,   214,   214,   215,   215,   216,   216,   217,   217,
     217,   217,   218,   218,   218,   218,   218,   218,   219,   219,
     220,   220,   221,   221,   221,   222,   222,   222,   223,   223,
     223,   224,   224,   226,   225,   227,   227,   228,   228,   228,
     229,   229,   230,   230,   231,   231,   232,   232,   232,   232,
     232,   233,   233,   233,   233,   233,   234,   234,   234,   234,
     235,   235,   235,   235,   235,   236,   236,   236,   236,   237,
     237,   237,   237,   237,   238,   238,   239,   239,   239,   239,
     240,   241,   242,   243,   244,   245,   245,   246,   246,   247,
     248,   248,   249,   250,   250,   251,   251,   252,   253,   254,
     254,   256,   255,   258,   257,   259,   260,   260,   261,   262,
     263,   265,   264,   264,   264,   266,   267,   264,   264,   264,
     268,   269,   270,   271,   264,   272,   264,   273,   273,   274,
     274,   274,   274,   274,   274,   274,   274,   274,   274,   274,
     274,   274,   274,   274,   274,   275,   274,   276,   274,   278,
     277,   277,   280,   279,   281,   281,   283,   282,   284,   284,
     285,   285,   285,   285,   286,   286,   287,   287,   288,   288,
     289,   289,   290,   290,   291,   291,   293,   292,   294,   295,
     296,   294,   294,   297,   297,   297,   297,   298,   298,   299,
     299,   299,   299,   299,   300,   300,   300,   300,   300,   301,
     303,   302,   304,   304,   305,   305,   306,   306,   307,   308,
     308,   308,   308,   308,   308,   309,   309,   310,   311,   312,
     312,   313,   313,   315,   316,   314,   317,   314,   318,   314,
     314,   320,   319,   319,   321,   321,   322,   323,   323,   324,
     324,   324,   325,   325,   325,   326,   326,   326,   327,   327,
     327,   328,   328,   328,   329,   329,   331,   332,   333,   330,
     334,   334,   335,   334,   336,   336,   338,   339,   337,   340,
     340,   340,   340,   341,   341,   342,   342,   343,   343,   343,
     343,   344,   344,   344,   345,   345,   346,   346,   346,   347,
     347,   348,   347,   349,   350,   350,   351,   351,   351,   351,
     351,   352,   352,   352,   352,   352,   352,   352,   352,   352,
     352,   352,   352,   352,   352,   352,   352,   352,   352,   352,
     352,   352,   353,   353,   353,   353,   354,   354,   355,   355,
     356,   357,   357,   358,   358,   358,   359,   360,   360,   361,
     361,   362,   362,   363,   364,   365
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     0,     1,     0,     2,     0,     3,     1,     1,
       1,     1,     5,     2,     3,     4,     4,     2,     2,     2,
       1,     0,     0,     8,     4,     0,     0,     8,     4,     0,
       0,     7,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     1,     1,     3,     1,
       2,     2,     2,     2,     2,     4,     2,     4,     2,     2,
       1,     1,     1,     1,     4,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     0,     4,
       0,     4,     0,     0,     7,     0,     5,     3,     3,     1,
       1,     1,     1,     0,     7,     3,     3,     3,     3,     4,
       6,     8,     6,     4,     3,     3,     2,     2,     1,     1,
       1,     1,     1,     1,     0,     1,     2,     1,     1,     2,
       2,     4,     4,     2,     2,     2,     0,     1,     4,     4,
       3,     3,     2,     2,     1,     2,     2,     2,     2,     2,
       1,     2,     1,     2,     2,     2,     2,     2,     2,     1,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     1,     4,
       4,     1,     4,     1,     4,     0,     4,     0,     6,     3,
       0,     6,     3,     0,     1,     1,     2,     6,     1,     3,
       0,     1,     4,     6,     4,     1,     1,     1,     1,     1,
       1,     1,     0,     4,     1,     0,     2,     1,     3,     3,
       2,     0,     4,     1,     0,     4,     1,     1,     1,     2,
       2,     5,     3,     0,     0,     6,     0,     0,     6,     1,
       1,     4,     3,     2,     3,     1,     1,     1,     1,     3,
       2,     1,     1,     3,     2,     3,     3,     4,     3,     4,
       3,     2,     1,     1,     2,     1,     2,     1,     2,     0,
       7,     5,     0,     7,     5,     0,     8,     0,     7,     2,
       2,     2,     0,     1,     0,     1,     1,     2,     0,     3,
       2,     4,     3,     2,     3,     1,     1,     2,     1,     4,
       1,     4,     2,     4,     3,     2,     4,     3,     1,     3,
       1,     1,     3,     0,     3,     0,     1,     0,     1,     2,
       1,     1,     1,     3,     2,     3,     4,     3,     2,     2,
       1,     4,     3,     4,     5,     5,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     1,     2,     2,     2,     1,
       2,     2,     2,     2,     1,     2,     1,     1,     1,     1,
       2,     0,     0,     0,     0,     0,     1,     1,     2,     3,
       1,     2,     1,     1,     5,     1,     1,     2,     2,     2,
       2,     0,     5,     0,     4,     0,     1,     2,     3,     2,
       2,     0,     4,     1,     3,     0,     0,     7,     5,     2,
       0,     0,     0,     0,    12,     0,     6,     2,     1,     1,
       2,     3,     2,     2,     2,     3,     6,     8,    10,    12,
       3,     4,     1,     3,     2,     0,     3,     0,     6,     0,
       3,     1,     0,     3,     2,     1,     0,     6,     2,     0,
       3,     5,     2,     4,     0,     1,     0,     1,     0,     1,
       1,     3,     4,     7,     1,     3,     0,     3,     2,     0,
       0,     6,     2,     0,     1,     1,     3,     1,     3,     4,
       4,     3,     4,     3,     4,     4,     3,     4,     3,     1,
       0,     3,     1,     2,     1,     3,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     4,     2,
       0,     3,     0,     0,     0,     9,     0,     5,     0,     9,
       5,     0,     6,     3,     0,     1,     3,     3,     1,     1,
       1,     1,     0,     3,     2,     3,     3,     1,     0,     1,
       4,     1,     3,     2,     1,     1,     0,     0,     0,     7,
       0,     2,     0,     3,     1,     1,     0,     0,     5,     4,
       1,     5,     2,     0,     2,     0,     1,     1,     1,     2,
       2,     4,     2,     2,     1,     3,     2,     2,     1,     0,
       2,     0,     3,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     6,     3,     5,     2,     1,     1,     1,     2,
       1,     3,     2,     1,     1,     1,     4,     1,     1,     1,
       2,     2,     1,     4,     4,     4
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned short yydefact[] =
{
       4,     0,     6,     0,     1,     0,     0,   275,   310,   309,
     272,   134,   367,   363,   365,     0,    62,     0,   588,     0,
     634,   635,     0,     0,   594,   614,     0,   614,     0,     0,
      20,     5,     8,    10,     9,     0,     0,   219,   220,   221,
     222,   211,   212,   213,   214,   223,   224,   225,   226,   215,
     216,   217,   218,   126,   126,     0,   142,   149,   269,   271,
     270,   140,   295,   166,     0,     0,     0,   274,   273,     0,
      11,   590,   591,   589,   592,   278,   636,   593,     7,    18,
      19,   368,   364,   366,     0,     0,    33,    34,    36,    35,
     595,     0,   600,   600,   276,   615,   614,     0,   277,     0,
       0,     0,   362,   267,   293,     0,   283,     0,   135,   147,
     153,   137,   169,   136,   148,   154,   170,   138,   159,   164,
     141,   176,   139,   160,   165,   177,   143,   145,   151,   150,
     187,   144,   146,   152,   188,   155,   157,   162,   161,   202,
     156,   158,   163,   203,   167,   185,   194,   173,   171,   168,
     186,   195,   172,   174,   200,   209,   180,   178,   175,   201,
     210,   179,   181,   183,   192,   191,   189,   182,   184,   193,
     190,   196,   198,   207,   206,   204,   197,   199,   208,   205,
       0,     0,    17,   296,   388,   379,   388,   380,   377,   381,
      13,     0,    89,    90,    91,    60,    61,     0,     0,     0,
       0,     0,    92,     0,    37,    39,    38,     0,    41,    40,
       0,     0,     0,     0,     0,   112,    42,    43,     0,     0,
      44,    63,     0,     0,    65,    47,    49,     0,     0,   108,
     109,   110,   111,   300,   616,     0,     0,     0,   614,     0,
     606,   611,   613,   597,     0,     0,   247,   248,   249,   250,
     243,   244,   245,   246,   413,     0,   239,   240,   241,   242,
     268,     0,     0,   294,    14,   293,    32,     0,   293,   267,
       0,   293,   361,   345,   267,   293,   346,     0,   281,     0,
     339,   340,     0,     0,     0,     0,     0,   369,     0,   372,
       0,   375,   676,   677,   700,   701,   697,   681,   682,   683,
     684,   685,   686,   687,   688,   689,   690,   691,   692,   693,
     694,   695,   696,   698,   699,     0,     0,   678,   679,   637,
     650,   669,   673,   680,   674,    58,    59,     0,     0,     0,
      53,    50,     0,   477,     0,     0,   715,   714,   713,     0,
       0,     0,     0,    52,     0,     0,     0,    54,     0,    56,
       0,     0,    82,    80,    78,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   107,   106,     0,
      45,     0,     0,     0,   473,   465,     0,    51,   307,   308,
     305,     0,   298,   301,   306,   596,   599,     0,   603,     0,
     602,   640,   598,   279,   415,   280,   360,     0,     0,   127,
       0,   580,   358,   267,   268,     0,     0,   485,   113,     0,
     485,   118,     0,   292,     0,     0,    16,   293,    24,     0,
     293,   293,   343,    15,    28,     0,     0,   293,   396,   390,
     239,   240,   241,   242,   235,   236,   237,   238,   126,   126,
     387,     0,   388,   293,   388,   410,   411,   384,   408,     0,
       0,   705,     0,   653,   671,   652,     0,   675,     0,     0,
       0,     0,    96,    95,     0,     0,   706,     0,   707,   708,
     722,   717,     0,   718,   719,     0,     0,    12,    48,     0,
       0,    87,    88,     0,     0,     0,     0,    76,    77,    75,
      74,    73,    72,    71,    66,    67,    68,    69,    70,   104,
       0,    46,     0,   105,    98,     0,     0,   466,   467,    97,
       0,   300,    45,   614,   602,   610,   622,   607,   642,   267,
     293,   414,   416,   421,   420,   422,   430,   359,   284,   285,
       0,     0,     0,     0,     0,   432,     0,   460,    30,   120,
     119,   116,   231,   232,   227,   228,   233,   234,   229,   230,
     126,   126,   290,   344,     0,     0,   485,   289,   342,   485,
       0,   371,   393,     0,   389,   397,     0,   374,     0,     0,
     385,     0,   384,     0,     0,     0,   638,   670,   563,     0,
     703,     0,     0,     0,    93,    64,   710,   712,     0,   716,
       0,   709,   721,   723,     0,   720,   725,   724,    55,    57,
       0,     0,    81,    79,    99,   103,   586,     0,   476,   445,
     475,   485,   485,   485,   485,     0,   454,     0,   463,   440,
     449,   468,   297,   299,    89,     0,   608,   604,     0,     0,
     612,   646,     0,   641,   424,   556,   429,   293,   428,   286,
       0,   584,   564,   126,   126,   582,     0,   565,   567,   581,
       0,     0,     0,   433,   431,     0,   123,     0,   124,     0,
       0,   341,   282,   285,    22,   287,    26,   391,     0,   293,
     392,   398,     0,   293,   394,   400,   293,   293,   412,   409,
     293,     0,     0,   649,   669,   219,   220,   221,   222,   211,
     212,   213,   214,   223,   224,   225,   226,   215,   216,   217,
     218,   126,     0,   658,   654,   656,     0,     0,   672,   565,
       0,     0,     0,     0,     0,   711,    83,    86,   469,     0,
     446,   441,   450,   447,   442,   451,   463,   443,   452,   448,
     444,   453,   455,   462,    89,   275,     0,     0,     0,     0,
       0,     0,   544,   614,   614,     0,   532,     0,   522,   472,
     485,     0,   125,   126,   126,     0,     0,   461,   509,   489,
     525,   531,   490,     0,   302,     0,   304,   640,   640,   621,
     619,   620,   601,   622,   627,   624,   126,   126,     0,     0,
     643,     0,   126,   126,   423,   425,     0,     0,   556,   427,
     562,   579,   417,   417,   558,   559,     0,   583,     0,   434,
     435,     0,    31,   470,     0,     0,   314,   312,   311,   291,
       0,     0,     0,   293,     0,   402,   293,   293,     0,   405,
     293,   370,   373,   378,   293,   704,   651,   662,   417,   663,
     659,   660,   639,     0,   100,     0,   102,   327,     0,     0,
     324,     0,   326,     0,   382,   317,   323,     0,     0,     0,
     587,   474,     0,   542,   512,   513,   514,     0,     0,     0,
     545,     0,   524,     0,     0,     0,     0,   510,     0,     0,
     132,   481,   495,   483,   500,     0,   493,     0,     0,   464,
     478,   539,     0,   133,     0,   642,   642,     0,   628,   628,
     623,   647,     0,     0,   426,   557,   351,   267,   293,   352,
     293,   347,   348,   293,   576,   418,   421,   267,   293,   293,
     578,   293,   566,   126,   126,   568,   585,   471,   121,   122,
       0,    23,   288,    27,   404,   293,     0,   407,   293,     0,
     376,   293,   293,   668,     0,   664,   702,     0,   330,    47,
       0,   321,    94,     0,   316,     0,     0,   329,   320,    84,
       0,   540,   515,     0,   520,     0,   523,   533,     0,   293,
       0,   130,   333,     0,   131,   336,     0,     0,   463,     0,
       0,     0,   480,   485,   479,   499,     0,   511,     0,   526,
       0,   530,   535,   303,   609,   605,     0,   631,   625,   629,
     626,     0,   424,   556,   574,   293,   350,   293,   354,   575,
     419,   424,   556,   577,   560,   417,   417,     0,   403,   399,
     406,   401,   666,   667,   661,   417,   101,     0,   332,     0,
       0,   318,   319,     0,     0,   521,     0,   527,   543,   128,
       0,   129,     0,     0,     0,     0,   546,     0,   494,   463,
     464,   486,   485,     0,   538,     0,   534,   633,     0,   293,
     645,   644,   648,   355,   356,     0,   349,   353,     0,   293,
     293,   571,   293,   573,   313,   665,     0,   325,   322,   541,
     548,     0,     0,   485,   485,     0,     0,   484,   547,   508,
     501,     0,   505,   492,   488,   487,     0,     0,   632,     0,
     357,   561,   569,   570,   572,   331,     0,     0,     0,   549,
     550,   516,   528,   334,   337,   482,   496,   546,   507,   463,
     498,   536,   630,     0,     0,   548,     0,     0,     0,     0,
     463,     0,   506,     0,     0,     0,     0,   517,   551,   335,
     338,   497,   502,   537,   552,     0,     0,     0,   546,     0,
     554,     0,   518,     0,     0,     0,     0,   503,   553,   519,
     555,   463,   504
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short yydefgoto[] =
{
      -1,     1,     2,     3,     5,    31,    32,    33,    34,   419,
     810,   425,   812,   270,   655,   841,   218,   334,   500,   220,
     221,   222,   223,    35,   224,   225,   486,   485,   483,   849,
     484,   226,   714,   407,   408,   409,   410,   541,   609,    36,
     398,   752,   246,   247,   248,   249,   250,   251,   252,   253,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
     643,   644,   438,   260,   254,    55,   261,    56,    57,    58,
      59,    60,   277,   105,   271,   278,   811,   106,   660,   399,
     263,    62,   381,   382,   383,    63,   809,   920,   843,   844,
     845,  1020,   846,   940,   847,   848,   961,  1030,  1118,   964,
    1032,  1119,   663,   280,   931,   901,   902,   281,    64,    65,
      66,    67,   442,   444,   449,   290,    68,   944,   571,   285,
     286,   440,   670,   674,   671,   675,   447,   448,   255,   394,
     521,   933,   905,   906,   524,   525,   272,   610,   611,   612,
     613,   614,   615,   411,   375,   851,   973,   977,   506,   507,
     508,   802,   757,   376,   617,   227,   803,   876,   877,   966,
     878,   968,   412,  1040,   974,  1041,  1042,   879,  1039,   967,
    1120,   969,  1107,  1138,  1151,  1109,  1080,   759,   881,  1072,
     760,   882,   761,   864,   981,   982,  1123,   979,   762,   861,
    1081,  1098,  1099,  1100,  1141,   636,   787,   645,   911,  1058,
     646,   647,   915,   648,   792,   402,   531,   649,   650,   607,
     228,    70,    91,    71,    72,   238,   517,    73,   514,   768,
     390,   767,    74,   391,    94,    75,   628,   773,   629,   778,
     988,   989,    76,    77,   191,   453,   707,   518,   632,  1052,
     633,   779,   991,   319,   576,   704,   705,   706,   934,   935,
     455,   578,   320,   321,   322,   323,   324,   467,   468,   587,
     469,   339,   229,   472,   473,   474,   230,   231,   232
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1007
static const short yypact[] =
{
     107,   110,   126,  4185, -1007,  4185,   352, -1007, -1007, -1007,
   -1007, -1007,   145,   145,   145,   113, -1007,   127, -1007,   467,
   -1007, -1007,   467,   467, -1007,   173,   467,   173,   467,   467,
   -1007, -1007, -1007, -1007, -1007,   200,   178,  1090,  3074,  4197,
    4306,   448,   356,   499,   870,  4275,  4318,  4287,  4396,   736,
     961,   779,  1119, -1007, -1007,    49, -1007, -1007, -1007, -1007,
   -1007,   145, -1007, -1007,    70,   204,   271, -1007, -1007,  4185,
   -1007, -1007, -1007, -1007, -1007, -1007, -1007, -1007, -1007, -1007,
   -1007,   145,   145,   145,  3889,   226, -1007, -1007, -1007, -1007,
   -1007,    63,    59,    95, -1007, -1007,   171,   342, -1007,   378,
     467,  3616, -1007,   128,   145,   394, -1007,  2965, -1007, -1007,
   -1007,   145, -1007, -1007, -1007, -1007, -1007, -1007, -1007, -1007,
     145, -1007, -1007, -1007, -1007, -1007, -1007, -1007, -1007,   145,
   -1007, -1007, -1007, -1007, -1007, -1007, -1007, -1007,   145, -1007,
   -1007, -1007, -1007, -1007, -1007, -1007, -1007,   145, -1007, -1007,
   -1007, -1007, -1007, -1007, -1007, -1007,   145, -1007, -1007, -1007,
   -1007, -1007, -1007, -1007, -1007,   145, -1007, -1007, -1007, -1007,
   -1007, -1007, -1007, -1007,   145, -1007, -1007, -1007, -1007, -1007,
     490,   178, -1007, -1007,   230,   259,   230,   270, -1007,   278,
   -1007,  4487, -1007, -1007, -1007, -1007, -1007,  3889,  3889,   310,
     313,   324, -1007,   467, -1007, -1007, -1007,  3889, -1007, -1007,
    2492,  3689,   359,   368,   386, -1007, -1007, -1007,  3889,   377,
     328, -1007,  3955,  4021, -1007,  5104,   762,  2064,  3889, -1007,
   -1007, -1007, -1007,   722, -1007,   467,   467,   467,   173,   467,
   -1007, -1007, -1007, -1007,   370,   434,  1287,  2120,  3699,  4856,
     840,   601,   957,   639, -1007,   440,   255,   479,   351,   533,
   -1007,   178,   178,   145, -1007,   145, -1007,   486,   145,   372,
    1702,   145, -1007, -1007,   128,   145, -1007,   421, -1007,  1447,
     465,   477,   468,  2996,   519,   526,  2177, -1007,   529, -1007,
     333, -1007, -1007, -1007, -1007, -1007, -1007, -1007, -1007, -1007,
   -1007, -1007, -1007, -1007, -1007, -1007, -1007, -1007, -1007, -1007,
   -1007, -1007, -1007, -1007, -1007,   398,  4376, -1007, -1007, -1007,
   -1007,  3499,   549, -1007, -1007, -1007, -1007,  3889,  3889,  4376,
   -1007, -1007,   508, -1007,   534,   542, -1007, -1007, -1007,  4522,
    4557,  4376,   467, -1007,   538,  3889,  2492, -1007,  2492, -1007,
    3889,  3889,   590, -1007, -1007,  3889,  3889,  3889,  3889,  3889,
    3889,  3889,  3889,  3889,  3889,  3889,  3889, -1007, -1007,   467,
    3889,  3889,   467,   550, -1007,   609,   562, -1007, -1007, -1007,
   -1007,   251, -1007,   593, -1007, -1007, -1007,   567, -1007,   580,
     576, -1007, -1007, -1007,   163, -1007,   477,    42,   178, -1007,
     668, -1007, -1007,   128,   680,  3063,   612, -1007, -1007,  1938,
      68, -1007,  4683,   648,   490,   490, -1007,   145, -1007,  1702,
     145,   145, -1007, -1007, -1007,  1702,   634,   145, -1007, -1007,
    1287,  2120,  3699,  4856,   840,   601,   957,   639, -1007,   459,
     622,  4413,   230,   145,   230, -1007,   678,   630, -1007,   333,
    4376, -1007,   643,   645,   728, -1007,   549, -1007,   444,  4990,
    5008,   651, -1007, -1007,  3406,  3889,   701,   661,  4522, -1007,
   -1007,   730,   685,  4557, -1007,   688,   689, -1007,  5104,   691,
     692,  5104,  5104,  3889,   737,  3889,  3889,  1818,  1097,  1616,
    1575,   865,   705,   705,   339,   339, -1007, -1007, -1007, -1007,
     699,   328,   702, -1007, -1007,   467,  2213,   609, -1007, -1007,
     711,   722,  4087,   173,   576, -1007, -1007, -1007,   504,   128,
     145, -1007, -1007, -1007, -1007,   536, -1007, -1007, -1007,    51,
     714,  3416,  3889,  3889,  3131, -1007,  4890, -1007, -1007, -1007,
   -1007, -1007,  4620,  4761,   826,  1245,  4639,  4787,   991,  1384,
     716,   717, -1007,   465,   410,   490, -1007,   766, -1007, -1007,
     727, -1007,   371,   124, -1007, -1007,   720, -1007,   726,  3889,
     467,   729,   630,   732,  4592,  1006, -1007, -1007,  4651,  4376,
   -1007,  4376,  3889,  4376, -1007, -1007,   328, -1007,  3889, -1007,
     701, -1007, -1007, -1007,   730, -1007, -1007, -1007,   740,   740,
     792,  3889,  1670,  1842, -1007, -1007, -1007,   475,   612, -1007,
   -1007,    58,    81,    83,    96,   824, -1007,   751,  3225, -1007,
   -1007, -1007, -1007, -1007,   322,   754, -1007, -1007,   404,  1902,
   -1007, -1007,  2595, -1007,   163,   163, -1007,   145, -1007, -1007,
     755, -1007, -1007, -1007, -1007, -1007,   756,   506, -1007, -1007,
     334,  4908,  4940, -1007, -1007,    74, -1007,   490, -1007,   178,
    2821, -1007, -1007,   822, -1007, -1007, -1007, -1007,  3889,   239,
     758, -1007,  3889,   301,   764, -1007,   145,   145,  5104, -1007,
     145,   769,   467, -1007,  3499,  4620,  4761,  4670,  4813,   826,
    1245,  1207,  1411,  4639,  4787,  4703,  4839,   991,  1384,  1300,
    1525,   771,   780, -1007, -1007,  4733,  1607,    74, -1007,   787,
     784,   785,  5038,   797,  2585, -1007, -1007,  2644, -1007,   467,
   -1007, -1007, -1007, -1007, -1007, -1007,  3318, -1007, -1007, -1007,
   -1007, -1007, -1007, -1007,   843,   845,  3889,   851,   808,   809,
    3757,   513,   897,   158,   335,  3823, -1007,   841, -1007, -1007,
   -1007,   818, -1007, -1007, -1007,   819,   846,   821, -1007, -1007,
   -1007,   829, -1007,  3529, -1007,  3889, -1007, -1007, -1007, -1007,
   -1007, -1007, -1007, -1007, -1007, -1007, -1007, -1007,   837,  4487,
   -1007,   178, -1007, -1007, -1007, -1007,   842,  3965, -1007, -1007,
   -1007, -1007,   277,   180, -1007, -1007,  4097, -1007,   926, -1007,
   -1007,   838, -1007, -1007,   528,   539, -1007, -1007,  5104, -1007,
      74,  2821,    74,  1972,  3889, -1007,   145,  1972,  3889, -1007,
     145, -1007, -1007, -1007,   145, -1007, -1007, -1007,   277, -1007,
   -1007, -1007, -1007,   467, -1007,  3889, -1007, -1007,   467,  3689,
   -1007,   893,  5104,   844,   847, -1007, -1007,   160,  2753,  3889,
   -1007, -1007,  5060, -1007, -1007, -1007, -1007,   853,  3889,   854,
   -1007,   879, -1007,   857,   838,  3889,   902, -1007,   490,   178,
   -1007, -1007, -1007, -1007, -1007,   883,   937,  2306,    69, -1007,
   -1007,   869,   872, -1007,   337,   520,   523,  4156,   487,   487,
   -1007, -1007,   490,   178, -1007, -1007, -1007,   128,   145, -1007,
     145,   545,   671,   188, -1007, -1007,   145,   128,   145,   188,
   -1007,   145, -1007, -1007, -1007, -1007, -1007, -1007, -1007, -1007,
    2585, -1007, -1007, -1007, -1007,  1972,   371, -1007,  1972,   124,
   -1007,   145,   188, -1007,   589, -1007, -1007,  5086, -1007,  1760,
    2585, -1007, -1007,  2678, -1007,  2889,  3889, -1007, -1007,  2644,
    3889, -1007, -1007,   871, -1007,  3889, -1007, -1007,   874,   145,
     595, -1007,   262,   604, -1007,   582,   911,   912, -1007,   915,
    3889,  2399, -1007, -1007, -1007, -1007,  3889, -1007,   838, -1007,
     918,   872, -1007, -1007, -1007, -1007,  3889,   913,   891, -1007,
     891,    61,   277,   327, -1007,   145, -1007,   145, -1007, -1007,
     145,   180,   180, -1007, -1007,   277,   180,   895, -1007, -1007,
   -1007, -1007, -1007, -1007, -1007,   277, -1007,  3889, -1007,   896,
    2889, -1007, -1007,  1760,  1996, -1007,    41, -1007, -1007, -1007,
    1702, -1007,  1702,  3889,  3889,   971,  3529,   905, -1007, -1007,
   -1007, -1007, -1007,   906, -1007,  4733, -1007,  5104,  3889,   145,
   -1007, -1007, -1007,   545,   671,   431, -1007, -1007,  3965,   145,
     188, -1007,   188, -1007, -1007, -1007,  4958, -1007, -1007, -1007,
      73,   901,   838, -1007, -1007,   914,   928, -1007, -1007, -1007,
   -1007,   910, -1007, -1007, -1007, -1007,   933,   929,  5104,   487,
   -1007, -1007, -1007, -1007, -1007, -1007,   935,   467,    53,   908,
   -1007, -1007, -1007, -1007, -1007, -1007, -1007,  3889, -1007, -1007,
   -1007, -1007, -1007,  3889,   930,    73,   939,    73,   838,   838,
   -1007,   942, -1007,   838,   936,  1017,    54, -1007, -1007, -1007,
   -1007, -1007, -1007, -1007, -1007,   967,  1024,   946,  3889,  3889,
   -1007,   432, -1007,   949,   951,   952,  1031, -1007, -1007, -1007,
   -1007, -1007, -1007
};

/* YYPGOTO[NTERM-NUM].  */
static const short yypgoto[] =
{
   -1007, -1007, -1007, -1007, -1007,   108, -1007,   411, -1007, -1007,
   -1007, -1007, -1007, -1007, -1007,   -19, -1007,   -83,   535,  -290,
     350, -1007, -1007, -1007,  -157,  1018, -1007, -1007, -1007, -1007,
   -1007, -1007, -1007,  -331, -1007,   636, -1007, -1007,  -379,   611,
    -375,  -730,     3,     9,    55,    86,     8,    26,   141,   183,
    -378,  -377,  -545,  -527,  -363,  -322,  -520,  -509,  -499,  -438,
     646,   652,  -577,  -242, -1007,  -596,  -237,  1194,  1312,  1230,
    1513, -1007,  -618,  -176,  -259,   502, -1007,   662, -1007,   588,
     889,   -18, -1007,   552, -1007,  1152,   254, -1007,  -629, -1007,
     125, -1007,  -797, -1007, -1007,   222, -1007, -1007, -1007, -1007,
   -1007, -1007,  -178,   338,  -756,    80,  -238,    87, -1007, -1007,
   -1007, -1007, -1007, -1007, -1007, -1007, -1007, -1007,   501,  -129,
   -1007,   633, -1007, -1007,   151,   153,   638,   515,   -15, -1007,
   -1007,  -689,  -327,  -296,  -606, -1007,   283, -1007, -1007, -1007,
   -1007, -1007, -1007,  -384, -1007, -1007,  -562,    43, -1007, -1007,
     583,  -639, -1007,   349, -1007, -1007,  -564, -1007, -1007, -1007,
   -1007, -1007,  -354,    65,  -847,  -372,  -293, -1007, -1007, -1007,
   -1007, -1007, -1007, -1007, -1007, -1007, -1007, -1007, -1007, -1007,
   -1007, -1007, -1007, -1007, -1007,   129, -1007, -1007, -1007, -1007,
   -1006,    -6, -1007,    -5, -1007,   476, -1007,  -749, -1007, -1007,
     537,   540,    71, -1007,   470,  -405, -1007, -1007, -1007, -1007,
      16, -1007,   379, -1007, -1007,  1027,   603, -1007, -1007, -1007,
   -1007, -1007, -1007, -1007,    -4,   -10, -1007, -1007,   355, -1007,
     234,    40,  -494, -1007, -1007, -1007, -1007,     1, -1007, -1007,
   -1007, -1007, -1007,   357, -1007, -1007, -1007,   425, -1007,   123,
     456, -1007,   568,   570,  -225, -1007,  -295, -1007, -1007,   558,
     696, -1007, -1007, -1007, -1007,   686, -1007, -1007, -1007
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -658
static const short yytable[] =
{
      90,   219,   279,    92,    93,   282,    37,    96,    37,    90,
     100,    41,    38,    41,    38,    95,   558,    95,   245,    69,
     420,    69,   755,    98,   631,   539,   457,   404,   785,    42,
     695,    42,   405,   883,   546,   547,   900,   414,   895,   804,
     325,   326,   555,   183,   439,   185,   187,   189,   696,   548,
     331,   948,   776,   538,   758,   699,   756,   288,    39,  -456,
      39,   343,  1050,   183,   183,   183,   700,   522,   832,  -115,
     975,   377,    37,    86,    87,   801,   701,    41,    38,   267,
     501,   244,  -457,  1096,  -458,    69,    95,  1070,   556,    40,
     549,    40,   241,   183,   559,    42,   456,  -459,   523,  1115,
    1136,  1121,   183,   904,   910,   236,   256,    -2,   268,   269,
       4,   183,   257,    78,   466,   471,   234,   268,   269,   753,
     183,  1035,   616,   107,    39,   237,    -3,   102,   338,   183,
    1071,   527,  1143,   782,   619,   976,    11,   702,   183,   182,
    1097,   236,  1116,  1137,    43,    88,    43,   183,  1022,  -436,
      89,  1051,   618,   546,   547,    40,   183,   235,   258,    17,
     695,   239,   758,  -115,   756,   184,   532,   755,   548,   749,
     672,   921,  -437,   923,  -438,   586,    17,   190,   696,    84,
     754,   102,   103,   102,   330,   699,    44,  -439,    44,   259,
     104,   703,  1083,    85,   783,   335,   700,   693,   694,   439,
     546,   547,   664,   945,   -36,   666,   701,    86,    87,   549,
      43,    19,   697,   620,   380,   548,   385,   386,   387,    17,
     389,   519,   501,  1068,    19,   838,    19,   946,    95,   520,
     269,   732,   720,   723,   388,   729,   103,   917,   907,   721,
     724,   727,   730,   590,   104,   183,   908,   269,   594,  1059,
     960,   695,    44,   698,   268,   269,   549,   618,   618,   726,
     618,  -595,  1122,   108,   753,  -595,   101,   702,   283,   696,
      17,   446,   256,  1131,    86,    87,   699,   256,   257,    88,
     102,   896,   634,   257,    89,   814,    17,   700,   502,   430,
     267,  1007,   233,  -285,   434,   431,   451,   913,   586,   186,
     957,   452,   441,   284,  1152,  -285,  1079,   585,   786,  1091,
     776,  1019,   435,   566,   461,   568,  1061,  1063,   722,   725,
     728,   731,   831,   476,   258,   754,   475,   693,   694,   258,
     102,   479,    17,   480,   445,   897,    86,    87,   784,   523,
     510,   432,   697,   898,   269,   511,    88,   818,   396,   397,
     499,    89,  -285,   503,   287,   259,  -285,   899,   914,   117,
     259,     8,     9,    10,   131,   289,   188,   268,   269,    12,
      13,    14,   433,   291,   102,   273,   327,   331,   403,   328,
      11,   -35,    17,   698,   669,   897,   785,   777,    19,   457,
     329,   631,   631,   898,   269,   785,   866,   364,   365,   366,
     600,    86,    87,    17,   420,    97,   256,    99,    88,   546,
     547,   764,   257,    89,  1044,   542,   765,   668,   693,   694,
     544,   543,   345,   797,   548,   340,   983,   436,   798,   274,
     446,   345,   242,   697,   341,   573,   235,   275,   545,   580,
     755,   926,    79,    80,   430,   929,  -256,    86,    87,   434,
     431,   276,   342,     8,     9,    10,   126,   441,   258,   456,
     392,    12,    13,    14,   450,   549,   344,   435,   243,   437,
      86,    87,   235,    88,   698,   884,   421,   269,    89,    17,
     769,   770,   771,   805,   264,   529,   606,   113,   265,   259,
     102,   273,   380,   102,   273,   772,   432,   997,   269,   661,
     695,   396,   397,    95,     8,     9,    10,   135,  1102,   626,
     579,   416,    12,    13,    14,   417,    86,    87,   696,    88,
    1090,  1145,   256,   393,    89,   699,  1146,   433,   257,   395,
      17,   421,   269,   986,   542,   751,   700,   753,  -255,   544,
     543,   122,    88,   268,   269,   274,   913,    89,   274,  -395,
    -395,   446,   400,   275,  1129,  1130,   275,   545,   423,  1133,
      20,    21,   265,   422,   710,   718,   711,   276,   713,   719,
     276,   858,   347,   349,   258,   630,    20,    21,   685,    20,
      21,   542,   436,   689,   686,   426,   544,   543,    88,  -257,
    1056,   984,  1057,    89,   985,   458,   795,   462,   754,   750,
     796,   690,   637,   269,   545,   259,   377,   914,    10,   131,
     267,   995,   269,  -285,    12,    13,    14,   427,   918,   726,
     443,    37,   417,   463,   437,  -285,    41,    38,   477,   919,
     687,   464,   430,   265,   763,    37,   -85,   434,   431,   504,
      41,    38,   505,   751,    42,   777,    10,   140,   268,   269,
     673,   509,    12,    13,    14,   435,   513,   857,    42,   512,
     992,   688,   863,   825,   180,   181,   786,   693,   694,   515,
    1001,   516,  -285,    39,  1089,   786,  -285,   526,   530,  1014,
     546,   547,   697,  1015,   432,  1029,   533,    39,   726,   417,
     962,   552,   262,   963,  1031,   548,   784,   523,   265,  1073,
     850,  1074,   537,   420,    40,   784,   523,   750,   685,   560,
     987,   987,   564,   689,   686,   433,   691,   282,    40,  1103,
    1104,   569,   859,   698,   570,    86,    87,     8,     9,   378,
     379,   690,   574,    95,    95,   575,   549,   997,   269,   577,
      98,     8,     9,    10,   162,   583,   529,   588,   669,    12,
      13,    14,   553,   554,  1054,  1055,   338,   589,   692,    43,
     687,   362,   363,   364,   365,   366,    37,    17,   885,   886,
     436,    41,    38,    43,   593,   953,   592,   596,   597,   763,
     598,   599,   958,   601,     8,     9,    10,   171,   604,    42,
     542,   688,    12,    13,    14,   544,   543,    88,   605,   685,
     622,    44,    89,   639,   689,   686,   656,   658,   638,   665,
      17,   676,   437,   545,   936,    44,   667,   677,    39,   938,
     680,   682,   690,   367,   368,   406,  -263,   369,   370,   371,
     372,     8,     9,    10,   126,   584,   422,   422,   716,    12,
      13,    14,   733,   766,   790,   794,   691,    10,   126,    40,
     267,   687,   816,    12,    13,    14,   401,    17,   820,   413,
     824,   827,   871,   415,   872,   873,   874,   875,   529,  -265,
     829,    17,  1026,   833,   834,     8,     9,    10,   140,   903,
     909,   796,   688,    12,    13,    14,   836,  1037,   692,   -33,
     430,   -34,    61,  1043,    61,   434,   431,   853,   854,   855,
     256,    81,    82,    83,    43,   860,   257,   865,   867,   870,
     256,   987,   374,   435,  -529,   932,   257,   526,   526,   360,
     361,   362,   363,   364,   365,   366,   111,   890,   120,   916,
     129,   894,   138,   749,   147,   942,   156,   691,   165,   941,
     174,   943,   432,   952,   954,   955,    44,   956,   959,   970,
    1075,  1076,   258,  1078,   971,   978,   965,   980,    61,  1048,
    -258,  1025,   258,  1027,    10,   135,     8,     9,    10,   167,
      12,    13,    14,   433,    12,    13,    14,  1033,  1034,   692,
     529,  1036,   183,   259,  1045,  1049,  1064,  1067,    17,  1077,
      61,  1101,    61,   259,  1082,  1086,     8,     9,    10,   162,
    1108,  1113,  1117,  1105,    12,    13,    14,   406,   557,   401,
       7,     8,     9,    10,    11,   561,   673,  1106,  1111,    12,
      13,    14,    17,  1110,  1078,  1134,  1125,  1135,   436,  1127,
    1124,   567,  1132,  1139,  1140,    16,  1142,    17,  1147,    37,
    1148,  1150,  1149,   780,    41,    38,   540,   625,   685,   562,
     563,  -264,   763,   689,   686,  1078,  1144,   662,   550,    19,
     528,   542,    42,   623,   551,   922,   544,   543,  1021,   947,
     437,   690,  1053,   681,   565,   526,   526,  1009,  1114,   396,
     397,    25,  1011,  1084,   545,   679,    27,   572,   396,   397,
     621,    39,  1060,  1062,     7,     8,     9,    10,   108,    61,
     687,  -655,   932,    12,    13,    14,   880,  1085,   635,  1126,
    1046,   526,  1128,   789,   793,   708,  1087,   627,   709,    16,
     240,    17,    40,   990,     8,     9,    10,   176,   887,  1112,
     830,   688,    12,    13,    14,   111,   891,   120,  1065,   129,
     826,   138,   683,    19,   684,   111,   715,   120,   357,   358,
     359,   360,   361,   362,   363,   364,   365,   366,    61,   595,
       0,   657,   659,    61,   591,    25,     0,     0,     0,     0,
      27,     0,     0,     0,     0,    61,     0,    43,     0,     0,
    -251,     0,     0,     0,   996,   998,   691,     0,     0,   112,
     116,   121,   125,   130,   134,   139,   143,   148,   152,   157,
     161,   166,   170,   175,   179,    61,     0,     0,     0,  -266,
       0,     0,     8,     9,    10,   135,     0,     0,    61,    44,
      12,    13,    14,     0,     0,   788,     0,     0,   692,     0,
      61,   109,   114,   118,   123,    61,     0,    61,    17,   145,
     150,   154,   159,   781,     0,     0,     0,     0,     0,     0,
       8,     9,    10,   131,   791,   791,     0,   815,    12,    13,
      14,   819,     0,     0,   821,   822,     0,     0,   823,     0,
       0,   127,   132,   136,   141,   526,   526,     0,     0,   163,
     168,   172,   177,     0,   526,   526,     0,     0,   526,   526,
       0,     7,    61,     0,    10,   108,     0,     0,   526,     0,
      12,    13,    14,     0,     0,     8,     9,    10,   171,     0,
       0,     0,   828,    12,    13,    14,    16,     0,    17,   111,
       0,   120,     0,   129,     0,   138,     0,     0,     0,     0,
      61,    17,     0,     0,     0,     0,   996,   998,   998,    61,
      19,     0,     0,     0,     0,   459,   460,     0,     0,   110,
     115,   119,   124,     0,     0,     0,     0,   146,   151,   155,
     160,     0,    25,   478,   868,   869,     0,    27,   481,   482,
       0,     0,     0,   487,   488,   489,   490,   491,   492,   493,
     494,   495,   496,   497,   498,   384,     0,   888,   889,     8,
       9,    10,   167,   892,   893,     0,     0,    12,    13,    14,
       0,   924,     0,     0,     0,   927,     0,     0,    61,     0,
       0,     0,   930,     0,     0,     0,     8,     9,    10,   140,
       0,     0,     0,   536,    12,    13,    14,     0,     0,     0,
       0,   111,     0,   129,     0,   147,     0,   165,     0,     0,
     109,   114,   118,   123,     0,     0,     0,     0,   418,     0,
       0,   -21,   -21,   -21,   -21,   -21,     0,     0,     0,     0,
     -21,   -21,   -21,     0,    61,     0,     0,     0,    61,     0,
      61,     0,    61,     0,     0,   267,   -21,     0,  -285,     0,
     127,   132,   136,   141,     0,     0,   993,     0,   994,     0,
    -285,   999,     0,     0,     0,     0,  1002,  1003,     0,  1004,
     -21,     0,     0,   602,   603,     0,     0,    61,     0,     0,
       0,     0,     0,  1008,     0,     0,  1010,     0,    61,  1012,
    1013,    61,   -21,     0,  1005,  1006,     0,   -21,     0,     0,
       8,     9,    10,   176,     0,     0,     0,  -285,    12,    13,
      14,  -285,   -21,     0,     0,     0,     0,  1028,     0,     0,
     651,   652,     0,     0,   128,   133,   137,   142,   110,   115,
     119,   124,   164,   169,   173,   178,     0,     0,     0,     0,
       0,     0,     0,     0,   111,     0,   120,     0,   129,     0,
     138,     0,   147,   401,   156,   401,   165,   678,   174,     0,
       0,     0,     0,     0,    61,     0,     0,     0,     0,     0,
     712,     0,     0,     0,     0,     0,     0,     0,   406,     0,
       0,  -657,  -657,  -657,  -657,  -657,     0,     0,     0,   717,
    -657,  -657,  -657,     0,   109,   114,   118,   123,   359,   360,
     361,   362,   363,   364,   365,   366,  -657,     0,  -657,     0,
       0,     0,     0,     0,     0,     0,     0,  1092,  1093,     0,
    1094,     0,    61,     0,     0,     0,     0,     0,     0,     0,
    -657,     0,     0,   384,   127,   132,   136,   141,   358,   359,
     360,   361,   362,   363,   364,   365,   366,     0,   808,     0,
       0,     0,  -657,     0,     0,    61,   813,  -657,     0,     0,
     817,     0,     0,     0,   112,   116,   130,   134,   148,   152,
     166,   170,  -657,   406,     0,     0,  -485,  -485,  -485,  -485,
    -485,     0,     0,     0,     0,  -485,  -485,  -485,   354,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,   365,
     366,  -485,   842,     0,     0,     0,   109,   114,     0,     0,
     145,   150,   110,   115,   119,   124,     0,     0,     0,     0,
       0,     0,     0,     0,   852,  -485,     0,     0,     0,     0,
       0,     0,     0,   128,   133,   137,   142,     0,     0,     0,
       0,  1017,     0,     0,   127,   132,    61,  -485,   163,   168,
       0,     0,  -485,     0,     0,     0,    61,     0,     0,     0,
       0,     0,     0,     0,     0,  1000,    61,  -114,     0,     0,
       0,     0,     0,   350,   351,   352,     0,   353,   354,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,   365,
     366,     0,     0,     0,     0,     0,     0,     0,     0,   808,
       0,     0,   925,     0,     0,     0,   928,   112,   116,   121,
     125,   130,   134,   139,   143,   148,   152,   157,   161,   166,
     170,   175,   179,   937,   110,   115,  1018,   939,   146,   151,
       0,     0,     0,     0,     0,     0,   842,   949,   356,   357,
     358,   359,   360,   361,   362,   363,   364,   365,   366,   109,
     114,   118,   123,     0,     0,     0,     0,   145,   150,   154,
     159,   355,   356,   357,   358,   359,   360,   361,   362,   363,
     364,   365,   366,   774,     0,     0,     7,     0,     0,    10,
      11,     0,     0,     0,     0,    12,    13,    14,     0,   127,
     132,   136,   141,     0,     0,    61,     0,   163,   168,   172,
     177,    16,     0,    17,    61,     0,     0,     0,   842,   406,
       0,     0,  -117,  -117,  -117,  -117,  -117,   128,   133,   137,
     142,  -117,  -117,  -117,     0,    19,     0,     0,   842,     0,
       0,   842,     0,   842,  1023,     0,     0,  -117,  1024,     0,
       0,     0,     0,     0,     0,     0,     0,    25,  -618,  -618,
    -618,     0,    27,     0,     0,     0,     0,     0,     0,     0,
       0,  -117,   775,  -618,     0,     0,     0,   110,   115,   119,
     124,     0,     0,    17,  1047,   146,   151,   155,   160,     0,
       0,     0,     0,  -117,     0,   350,   351,   352,  -117,   353,
     354,   355,   356,   357,   358,   359,   360,   361,   362,   363,
     364,   365,   366,  -117,     0,  1066,     0,     0,   842,   350,
     351,   352,  1069,   353,   354,   355,   356,   357,   358,   359,
     360,   361,   362,   363,   364,   365,   366,   128,   133,     0,
       0,   164,   169,     0,     0,   373,  1088,  -461,  -461,  -461,
    -461,  -461,  -461,  -461,  -461,     0,  -461,  -461,  -461,  -461,
    -461,     0,  -461,  -461,  -461,  -461,  -461,  -461,  -461,  -461,
    -461,  -461,  -461,  -461,  -461,  -461,  -461,  -461,  -461,  -461,
    -461,  -461,  -461,     0,     0,     0,  -461,     0,     0,     0,
       0,     0,  -461,     0,     0,  -461,     0,  -461,     0,     0,
    -461,  -461,  -461,     0,     7,  -461,  -461,    10,   113,     0,
    -461,  -461,     0,    12,    13,    14,  -461,     0,  -461,  -461,
       0,     0,     0,  -461,  -461,     0,     0,  -461,  -461,    16,
       0,  -461,  -461,     0,  -461,   374,  -461,  -461,     0,  -461,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    19,     0,     0,     0,     0,   428,     0,
       0,     7,     0,     0,    10,    11,     0,     0,     0,     0,
      12,    13,    14,     0,     0,    25,     0,     0,     0,     0,
      27,     0,   128,   133,   137,   142,    16,     0,    17,    18,
     164,   169,   173,   178,   608,     0,  -485,  -485,  -485,  -485,
    -485,  -485,  -485,  -485,     0,  -485,  -485,  -485,  -485,  -485,
      19,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,  -485,
    -485,  -485,  -485,  -485,  -485,  -485,     0,  -485,  -485,  -485,
    -485,  -485,    25,     0,     0,  -485,     0,    27,     0,     0,
       0,  -485,     0,     0,  -485,     0,  -485,   429,  -386,  -485,
    -485,  -485,     0,     0,  -485,  -485,     0,     0,     0,  -485,
    -485,     0,     0,     0,     0,  -485,     0,  -485,  -485,     0,
       0,     0,  -485,  -485,     0,     0,  -485,  -485,     0,     0,
    -485,  -485,     0,  -485,     0,  -485,  -485,   972,  -485,  -463,
    -463,     0,     0,     0,     0,  -463,  -463,     0,  -463,     0,
       0,     0,  -463,     0,  -463,  -463,  -463,  -463,  -463,  -463,
    -463,  -463,  -463,  -463,  -463,     0,  -463,     0,  -463,     0,
    -463,  -463,  -463,  -463,  -463,     0,     0,     0,  -463,     0,
       0,     0,     0,     0,  -463,     0,     0,  -463,     0,     0,
       0,     0,  -463,  -463,  -463,     0,     0,  -463,  -463,     0,
       0,     0,  -463,  -463,     0,     0,     0,     0,  -463,     0,
    -463,  -463,     0,     0,     0,  -463,  -463,     0,     0,  -463,
    -463,     0,     0,  -463,  -463,     0,  -463,     0,  -463,  -463,
    1038,  -463,  -491,  -491,     0,     0,     0,     0,  -491,  -491,
       0,  -491,     0,     0,     0,  -491,     0,  -491,  -491,  -491,
    -491,  -491,  -491,  -491,  -491,  -491,  -491,  -491,     0,  -491,
       0,  -491,     0,  -491,  -491,  -491,  -491,  -491,     0,     0,
       0,  -491,     0,     0,     0,     0,     0,  -491,     0,     0,
    -491,     0,     0,     0,     0,  -491,  -491,  -491,     0,     0,
    -491,  -491,     0,     0,     0,  -491,  -491,     0,     0,     0,
       0,  -491,     0,  -491,  -491,     0,     0,     0,  -491,  -491,
       0,     0,  -491,  -491,     0,     0,  -491,  -491,     0,  -491,
       0,  -491,  -491,   332,  -491,   192,     7,     0,     0,    10,
      11,   193,   194,     0,   195,    12,    13,    14,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    16,   196,    17,    18,     0,   197,   198,   199,   200,
     201,     0,     0,     0,   202,     0,     0,     0,     0,     0,
     203,     0,     0,   204,     0,    19,     0,     0,   205,   206,
     207,     0,     0,   208,   209,     0,     0,     0,   210,   211,
       0,     0,     0,     0,   212,     0,   213,    25,     0,     0,
       0,   214,    27,     0,     0,     0,     0,     0,     0,     0,
     215,     0,     0,     0,   216,   217,   837,   333,   734,    87,
       0,     0,     0,     0,   193,   194,     6,   195,  -126,     7,
       8,     9,    10,    11,     0,     0,     0,     0,    12,    13,
      14,     0,     0,     0,     0,   196,     0,    18,     0,   197,
     198,   199,   200,   201,    16,     0,    17,   202,     0,     0,
       0,     0,     0,   203,     0,     0,   204,     0,     0,     0,
       0,   205,   206,   207,     0,     0,   208,   209,    19,     0,
     838,   210,   839,  -126,     0,     0,     0,   212,     0,   213,
      88,  -126,     0,     0,   214,    89,     0,     0,     0,     0,
      25,     0,     0,   215,     0,    27,  -315,   216,   217,   837,
     840,   734,    87,     0,     0,    30,     0,   193,   194,   352,
     195,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,     0,     0,     0,   196,     0,
      18,     0,   197,   198,   199,   200,   201,     0,     0,     0,
     202,     0,     0,     0,     0,     0,   203,     0,     0,   204,
       0,     0,     0,     0,   205,   206,   207,     0,     0,   208,
     209,     0,     0,   838,   210,   839,     0,     0,     0,     0,
     212,     0,   213,    88,   837,     0,   192,   214,    89,     0,
       0,     0,   193,   194,     0,   195,   215,     0,     0,  -383,
     216,   217,     0,   840,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   196,     0,    18,     0,   197,   198,   199,
     200,   201,     0,     0,     0,   202,  -328,     0,     0,     0,
       0,   203,     0,     0,   204,     0,     0,     0,     0,   205,
     206,   207,     0,     0,   208,   209,     0,     0,  -328,   210,
     211,     0,   806,     0,   192,   212,     0,   213,     0,     0,
     193,   194,   214,   195,     0,     0,     0,     0,     0,     0,
       0,   215,     0,     0,     0,   216,   217,     0,   840,     0,
       0,   196,     0,    18,     0,   197,   198,   199,   200,   201,
       0,     0,     0,   202,     0,     0,     0,     0,     0,   203,
       0,     0,   204,     0,     0,     0,     0,   205,   206,   207,
       0,     0,   208,   209,     0,     0,     0,   210,   211,     0,
     837,     0,   192,   212,     0,   213,     0,     0,   193,   194,
     214,   195,     0,     0,     0,     0,     0,     0,     0,   215,
       0,     0,     0,   216,   217,     0,   807,     0,     0,   196,
       0,    18,     0,   197,   198,   199,   200,   201,     0,     0,
       0,   202,     0,     0,     0,     0,     0,   203,     0,     0,
     204,     0,     0,     0,     0,   205,   206,   207,     0,     0,
     208,   209,     0,     0,     0,   210,   211,     0,     0,     0,
       0,   212,     0,   213,     0,     0,   266,     0,   214,   -29,
     -29,   -29,   -29,   -29,     0,     0,     0,   215,   -29,   -29,
     -29,   216,   217,     0,   840,     0,     0,     0,     0,     0,
       0,     0,     0,   267,   -29,     0,  -285,   424,     0,     0,
     -25,   -25,   -25,   -25,   -25,     0,     0,     0,  -285,   -25,
     -25,   -25,     0,     0,     0,     0,     0,     0,   -29,     0,
       0,     0,     0,     0,   267,   -25,     0,  -285,     0,     0,
       0,   268,   269,     0,     0,     0,     0,     0,     0,  -285,
     -29,     0,     0,     0,     0,   -29,     0,     0,     0,   -25,
       0,     0,     0,     0,     0,  -285,     0,     0,     0,  -285,
     -29,     0,   268,   269,     0,     0,   192,     0,     0,     0,
       0,   -25,   193,   194,     0,   195,   -25,     0,     7,     8,
       9,    10,   113,     0,     0,     0,  -285,    12,    13,    14,
    -285,   -25,     0,   196,     0,    18,     0,   197,   198,   199,
     200,   201,     0,    16,     0,   202,     0,     0,     0,     0,
       0,   203,     0,     0,   204,     0,     0,     0,     0,   205,
     206,   534,     0,     0,   208,   209,     0,    19,     0,   210,
     211,     0,     0,     0,   192,   212,     0,   213,     0,     0,
     193,   194,   214,   195,     0,     0,     0,     0,     0,    25,
       0,   215,     0,     0,    27,   216,   217,     0,     0,   535,
       0,   196,     0,    18,  -252,   197,   198,   199,   200,   201,
       0,     0,     0,   202,     0,     0,     0,     0,     0,   203,
       0,     0,   204,     0,     0,     0,     0,   205,   206,   207,
       0,     0,   208,   209,     0,     0,     0,   210,   211,     0,
       0,     0,     0,   212,     0,   213,     0,     0,     0,     0,
     214,     0,     0,     0,     0,     0,     0,     0,     0,   215,
       0,     0,     0,   216,   217,     0,     0,   653,   734,   735,
       8,     9,    10,    11,   193,   194,     0,   195,    12,    13,
      14,     0,     0,     0,     0,     0,     0,   736,   737,   738,
     739,   740,   741,   742,    16,   196,    17,    18,     0,   197,
     198,   199,   200,   201,     0,     0,     0,   202,     0,     0,
       0,     0,     0,   203,     0,     0,   204,     0,    19,     0,
       0,   205,   206,   207,     0,     0,   208,   209,     0,     0,
       0,   210,   211,     0,     0,     0,     0,   212,     0,   213,
     743,     0,     0,     0,   214,   744,     0,     0,   745,   746,
       0,     0,   747,   215,     0,   748,     0,   216,   217,     0,
     749,   734,    87,     0,     0,     0,     0,   193,   194,     0,
     195,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     736,   737,   738,   739,   740,   741,   742,     0,   196,     0,
      18,     0,   197,   198,   199,   200,   201,     0,     0,     0,
     202,     0,     0,     0,     0,     0,   203,     0,     0,   204,
       0,     0,     0,     0,   205,   206,   207,     0,     0,   208,
     209,     0,     0,     0,   210,   211,     0,     0,     0,     0,
     212,     0,   213,    88,     0,     0,     0,   214,    89,     0,
       0,   745,   746,     0,     0,   747,   215,     0,   748,   192,
     216,   217,     0,   749,     0,   193,   194,   640,   195,   641,
       7,     8,     9,    10,    11,     0,     0,   642,     0,    12,
      13,    14,     0,     0,     0,     0,   196,     0,    18,     0,
     197,   198,   199,   200,   201,    16,     0,     0,   202,     0,
       0,     0,     0,     0,   203,     0,     0,   204,     0,     0,
       0,     0,   205,   206,   207,     0,     0,   208,   209,    19,
       0,     0,   210,   211,     0,     0,     0,     0,   212,     0,
     213,     0,     0,     0,     0,   214,     0,     0,     0,     0,
       0,    25,     0,     0,   215,     0,    27,     0,   216,   217,
       0,   584,   292,   293,     0,  -563,   294,   295,     0,     0,
       0,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
       0,     0,   192,     7,     8,     9,    10,    11,   193,   194,
       0,   195,    12,    13,    14,   315,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    16,   196,
      17,    18,     0,   197,   198,   199,   200,   201,     0,     0,
       0,   202,     0,     0,   317,     0,     0,   203,     0,   318,
     204,     0,    19,     0,     0,   205,   206,   207,     0,     0,
     208,   209,     0,   454,     0,   210,   211,     0,     0,     0,
       0,   212,     0,   213,    25,     0,     0,     0,   214,    27,
       0,     0,     0,     0,     0,     0,     0,   215,     0,   192,
       7,   216,   217,    10,    11,   193,   194,     0,   195,    12,
      13,    14,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    16,   196,    17,    18,     0,
     197,   198,   199,   200,   201,     0,     0,     0,   202,     0,
       0,     0,     0,     0,   203,     0,     0,   204,     0,    19,
       0,     0,   205,   206,   207,     0,     0,   208,   209,     0,
       0,     0,   210,   211,     0,     0,     0,     0,   212,     0,
     213,    25,   192,   336,     0,   214,    27,     0,   193,   194,
       0,   195,     0,     7,   215,     0,    10,   117,   216,   217,
       0,     0,    12,    13,    14,     0,     0,     0,     0,   196,
       0,    18,     0,   197,   198,   199,   200,   201,    16,     0,
      17,   202,     0,     0,     0,     0,     0,   203,     0,     0,
     204,     0,     0,     0,     0,   205,   206,   207,     0,     0,
     208,   209,    19,     0,     0,   210,   211,     0,     0,     0,
     192,   212,     0,   213,   337,     0,   193,   194,   214,   195,
       0,     0,     0,     0,    25,     0,     0,   215,     0,    27,
       0,   216,   217,     0,     0,     0,     0,   196,     0,    18,
       0,   197,   198,   199,   200,   201,     0,     0,     0,   202,
       0,     0,     0,     0,     0,   203,     0,     0,   204,     0,
       0,     0,     0,   205,   206,   207,     0,     0,   208,   209,
       0,     0,     0,   210,   211,     0,   192,     0,     0,   212,
       0,   213,   193,   194,     0,   195,   214,     0,     0,     0,
       0,     0,     0,     0,     0,   215,     0,   856,     0,   216,
     217,     0,     0,   196,     0,    18,     0,   197,   198,   199,
     200,   201,     0,     0,     0,   202,     0,     0,     0,     0,
       0,   203,     0,     0,   204,     0,     0,     0,     0,   205,
     206,   207,     0,     0,   208,   209,     0,     0,     0,   210,
     211,     0,   192,     0,     0,   212,     0,   213,   193,   194,
       0,   195,   214,     0,     0,     0,     0,     0,     0,     0,
       0,   215,     0,   862,     0,   216,   217,     0,     0,   196,
       0,    18,     0,   197,   198,   199,   200,   201,     0,     0,
       0,   202,     0,     0,     0,     0,     0,   203,     0,     0,
     204,     0,     0,     0,     0,   205,   206,   207,     0,     0,
     208,   209,     0,     0,     0,   210,   211,     0,   192,     0,
       0,   212,     0,   213,   193,   194,   640,   195,   214,     7,
       8,     9,    10,    11,     0,     0,   642,   215,    12,    13,
      14,   216,   217,     0,     0,   196,     0,    18,     0,   197,
     198,   199,   200,   201,    16,     0,     0,   202,     0,     0,
       0,     0,     0,   203,     0,     0,   204,     0,     0,     0,
       0,   205,   206,   207,     0,     0,   208,   209,    19,     0,
       0,   346,   211,     0,   192,     0,     0,   212,     0,   213,
     193,   194,     0,   195,   214,     0,     0,     0,     0,     0,
      25,     0,     0,   215,     0,    27,     0,   216,   217,     0,
       0,   196,     0,    18,  -563,   197,   198,   199,   200,   201,
       0,     0,     0,   202,     0,     0,     0,     0,     0,   203,
       0,     0,   204,     0,     0,     0,     0,   205,   206,   207,
       0,     0,   208,   209,     0,     0,     0,   348,   211,     0,
     624,     0,     0,   212,     0,   213,   193,   194,     0,   195,
     214,     7,     8,     9,    10,    11,     0,     0,   912,   215,
      12,    13,    14,   216,   217,     0,     0,   196,     0,    18,
       0,   197,   198,   199,   200,   201,    16,     0,    17,   202,
       0,     0,     0,     0,     0,   203,     0,     0,   204,     0,
       0,     0,     0,   205,   206,   207,     0,     0,   208,   209,
      19,     0,     0,   210,   211,     0,     0,   774,     0,   212,
       7,   213,     0,    10,    11,     0,   214,     0,     0,    12,
      13,    14,    25,     0,     0,   215,     0,    27,     0,   216,
     217,     0,     0,     0,     0,    16,     6,    17,  -126,     7,
       8,     9,    10,    11,     0,     0,     0,     0,    12,    13,
      14,     7,     8,     9,    10,   117,     0,     0,     0,    19,
      12,    13,    14,    15,    16,     0,    17,    18,     0,     0,
       0,     0,     0,     0,     0,     0,    16,     0,    17,     0,
       0,    25,  -617,  -617,  -617,     0,    27,     0,    19,     0,
       0,    20,    21,  -126,     0,     0,   775,  -617,     0,     0,
      19,  -126,     0,     0,    22,    23,    24,     0,     0,     0,
      25,     0,     0,     0,    26,    27,    28,    29,     0,     0,
       0,     0,    25,     0,     0,    30,     0,    27,     0,     7,
       8,     9,    10,   144,     0,     0,     0,  -253,    12,    13,
      14,     7,     8,     9,    10,   153,     0,     0,     0,     0,
      12,    13,    14,     0,    16,     0,    17,     0,     0,     0,
       7,     8,     9,    10,   122,     0,    16,     0,    17,    12,
      13,    14,     7,     8,     9,    10,   149,     0,    19,     0,
       0,    12,    13,    14,     0,    16,     0,     0,     0,     0,
      19,     0,     0,     0,     0,     0,     0,    16,     0,     0,
      25,     0,     0,     0,     0,    27,     0,     0,     0,    19,
       0,     0,    25,     0,     0,  -259,     0,    27,     0,     0,
       0,    19,     0,     0,     0,     0,     0,  -261,     0,     0,
       7,    25,     0,    10,    11,     0,    27,     0,     0,    12,
      13,    14,     0,    25,     0,     0,  -254,     0,    27,     0,
       7,     8,     9,    10,   158,    16,     0,    17,  -260,    12,
      13,    14,     0,     0,   428,     0,     0,     7,     0,     0,
      10,    11,     0,     0,     0,    16,    12,    13,    14,    19,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    16,     0,    17,    18,     0,     0,     0,    19,
       0,    25,     0,     0,     0,     0,    27,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    19,     0,     0,     0,
       0,    25,     0,     0,     0,     0,    27,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  -262,     0,    25,     0,
     292,   293,     0,    27,   294,   295,     0,     0,     0,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,     0,     0,
       0,     0,     0,     0,     0,   292,   293,     0,     0,   294,
     295,     0,     0,   315,   296,   297,   298,   299,   300,   301,
     302,   303,   304,   305,   306,   307,   308,   309,   310,   311,
     312,   313,   314,   316,     0,     0,     0,     0,     0,     0,
     292,   293,   317,     0,   294,   295,     0,   318,   465,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,     0,     0,
       0,     0,     0,     0,     0,   292,   293,   317,     0,   294,
     295,     0,   318,   470,   296,   297,   298,   299,   300,   301,
     302,   303,   304,   305,   306,   307,   308,   309,   310,   311,
     312,   313,   314,     0,     7,     8,     9,    10,   108,     0,
       0,     0,   317,    12,    13,    14,     0,   318,   315,     0,
       0,     0,     0,     7,     8,     9,    10,   144,     0,    16,
       0,    17,    12,    13,    14,     7,     8,     9,    10,    11,
       0,     0,   642,     0,    12,    13,    14,   317,    16,     0,
      17,     0,   318,    19,     7,     8,     9,    10,   117,     0,
      16,     0,     0,    12,    13,    14,     0,     7,     8,     9,
      10,    11,    19,     0,     0,    25,    12,    13,    14,    16,
      27,    17,     0,     0,    19,     0,     0,     7,     8,     9,
      10,   153,    16,     0,    25,     0,    12,    13,    14,    27,
       0,     0,     0,    19,     0,     0,    25,     0,     0,     0,
       0,    27,    16,     0,    17,     0,    19,     7,     8,     9,
      10,    11,     0,     0,     0,    25,    12,    13,    14,     0,
      27,     0,     0,     0,     0,     0,    19,     0,    25,     0,
       0,     0,    16,    27,    17,     7,     8,     9,    10,   113,
       0,     0,     0,     0,    12,    13,    14,     0,    25,     0,
       0,     0,     0,    27,     0,     0,    19,     0,     0,     0,
      16,     7,     8,     9,    10,   149,     0,     0,     0,     0,
      12,    13,    14,     0,     0,     0,     0,     0,    25,     0,
       0,     0,     0,    27,    19,     0,    16,     7,     8,     9,
      10,   122,     0,     0,     0,     0,    12,    13,    14,     0,
       0,     0,     0,     0,     0,     0,    25,     0,     0,     0,
      19,    27,    16,     7,     8,     9,    10,   158,     0,     0,
       0,     0,    12,    13,    14,     0,     0,     0,     0,     0,
       7,     0,    25,    10,   122,     0,    19,    27,    16,    12,
      13,    14,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    16,     0,     0,    25,     0,
       0,     0,    19,    27,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    19,
       0,     0,     0,     0,    25,     0,     0,     0,     0,    27,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    25,     0,   350,   351,   352,    27,   353,   354,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,   365,
     366,   350,   351,   352,     0,   353,   354,   355,   356,   357,
     358,   359,   360,   361,   362,   363,   364,   365,   366,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   350,   351,   352,   654,   353,   354,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,   365,
     366,   350,   351,   352,   799,   353,   354,   355,   356,   357,
     358,   359,   360,   361,   362,   363,   364,   365,   366,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   350,   351,   352,   800,   353,   354,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,   365,
     366,   350,   351,   352,  1095,   353,   354,   355,   356,   357,
     358,   359,   360,   361,   362,   363,   364,   365,   366,     0,
       0,   950,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   350,   351,   352,   581,   353,   354,   355,   356,   357,
     358,   359,   360,   361,   362,   363,   364,   365,   366,     0,
       0,     0,   582,   350,   351,   352,   951,   353,   354,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,   365,
     366,     0,     0,     0,     0,     0,     0,     0,     0,   350,
     351,   352,   835,   353,   354,   355,   356,   357,   358,   359,
     360,   361,   362,   363,   364,   365,   366,   350,   351,   352,
       0,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1016
};

static const short yycheck[] =
{
      19,    84,   180,    22,    23,   181,     3,    26,     5,    28,
      29,     3,     3,     5,     5,    25,   421,    27,   101,     3,
     279,     5,   618,    27,   518,   409,   321,   269,   634,     3,
     575,     5,   269,   763,   412,   412,   792,   274,   787,   657,
     197,   198,   417,    61,   286,    64,    65,    66,   575,   412,
     207,   848,   629,   407,   618,   575,   618,   186,     3,     1,
       5,   218,     1,    81,    82,    83,   575,   394,   707,     1,
       1,   228,    69,     3,     4,     1,   575,    69,    69,    28,
     370,   100,     1,    10,     1,    69,    96,    46,   419,     3,
     412,     5,    96,   111,   425,    69,   321,     1,   394,    46,
      46,  1107,   120,   792,   793,    46,   103,     0,    66,    67,
       0,   129,   103,     5,   339,   340,    53,    66,    67,   618,
     138,   968,   506,    36,    69,    66,     0,     3,   211,   147,
      89,    89,  1138,   632,   506,    66,     8,   575,   156,    90,
      67,    46,    89,    89,     3,    75,     5,   165,   945,    91,
      80,    90,   506,   531,   531,    69,   174,    94,   103,    31,
     705,    66,   726,    95,   726,    95,   403,   763,   531,    95,
      46,   810,    91,   812,    91,   465,    31,    69,   705,    66,
     618,     3,    58,     3,   203,   705,     3,    91,     5,   103,
      66,   575,  1039,    66,   632,   210,   705,   575,   575,   441,
     578,   578,   556,    43,    46,   559,   705,     3,     4,   531,
      69,    53,   575,   506,   233,   578,   235,   236,   237,    31,
     239,    58,   512,  1020,    53,    65,    53,    67,   238,    66,
      67,   615,   611,   612,   238,   614,    58,   801,    58,   611,
     612,   613,   614,   468,    66,   263,    66,    67,   473,  1005,
     868,   796,    69,   575,    66,    67,   578,   611,   612,   613,
     614,    90,  1109,     8,   763,    94,    66,   705,   181,   796,
      31,   290,   269,  1120,     3,     4,   796,   274,   269,    75,
       3,     4,   519,   274,    80,    46,    31,   796,   371,   286,
      28,   920,    66,    31,   286,   286,   315,   796,   588,    95,
     864,   316,   286,    73,  1151,    43,  1036,   464,   635,  1058,
     887,   940,   286,   442,   329,   444,  1005,  1006,   611,   612,
     613,   614,   706,   342,   269,   763,   341,   705,   705,   274,
       3,   346,    31,   348,     1,    58,     3,     4,   634,   635,
      89,   286,   705,    66,    67,    94,    75,    46,   261,   262,
     369,    80,    90,   372,    95,   269,    94,    80,   796,     8,
     274,     5,     6,     7,     8,    95,    95,    66,    67,    13,
      14,    15,   286,    95,     3,     4,    66,   534,     6,    66,
       8,    46,    31,   705,   562,    58,   992,   629,    53,   684,
      66,   885,   886,    66,    67,  1001,   750,    58,    59,    60,
     483,     3,     4,    31,   663,    26,   403,    28,    75,   787,
     787,    89,   403,    80,   978,   412,    94,    46,   796,   796,
     412,   412,    94,    89,   787,    66,    89,   286,    94,    58,
     449,    94,    90,   796,    66,   450,    94,    66,   412,   458,
    1036,   816,    90,    91,   441,   820,    90,     3,     4,   441,
     441,    80,    66,     5,     6,     7,     8,   441,   403,   684,
      90,    13,    14,    15,    66,   787,    89,   441,    90,   286,
       3,     4,    94,    75,   796,   765,    66,    67,    80,    31,
      76,    77,    78,   659,    90,   398,   505,     8,    94,   403,
       3,     4,   511,     3,     4,    91,   441,    66,    67,    89,
    1045,   414,   415,   513,     5,     6,     7,     8,  1072,   513,
      66,    90,    13,    14,    15,    94,     3,     4,  1045,    75,
      89,    89,   519,    89,    80,  1045,    94,   441,   519,    89,
      31,    66,    67,    46,   531,   618,  1045,  1036,    90,   531,
     531,     8,    75,    66,    67,    58,  1045,    80,    58,    90,
      91,   570,    66,    66,  1118,  1119,    66,   531,    90,  1123,
      56,    57,    94,   280,   579,    90,   581,    80,   583,    94,
      80,    58,   222,   223,   519,    71,    56,    57,   575,    56,
      57,   578,   441,   575,   575,    66,   578,   578,    75,    90,
     995,    71,   997,    80,    71,    46,    90,    89,  1036,   618,
      94,   575,    66,    67,   578,   519,   763,  1045,     7,     8,
      28,    66,    67,    31,    13,    14,    15,    91,    90,   973,
      91,   618,    94,    89,   441,    43,   618,   618,    90,    90,
     575,    89,   629,    94,   618,   632,    46,   629,   629,    89,
     632,   632,    33,   726,   618,   887,     7,     8,    66,    67,
     563,    89,    13,    14,    15,   629,    89,   740,   632,    66,
     897,   575,   745,   682,    53,    54,   993,  1045,  1045,    89,
     907,    95,    90,   618,  1049,  1002,    94,   394,    10,    90,
    1058,  1058,  1045,    94,   629,    90,     6,   632,  1042,    94,
     868,    43,   104,   869,    90,  1058,   992,   993,    94,  1030,
     719,  1032,    90,   962,   618,  1001,  1002,   726,   705,    75,
     888,   889,    90,   705,   705,   629,   575,   893,   632,  1073,
    1074,    43,   741,  1045,    94,     3,     4,     5,     6,     7,
       8,   705,    89,   743,   744,    90,  1058,    66,    67,    11,
     744,     5,     6,     7,     8,    94,   659,    46,   926,    13,
      14,    15,   414,   415,   992,   993,   839,    96,   575,   618,
     705,    56,    57,    58,    59,    60,   763,    31,   767,   768,
     629,   763,   763,   632,    89,   858,    46,    89,    89,   763,
      89,    89,   865,    46,     5,     6,     7,     8,    89,   763,
     787,   705,    13,    14,    15,   787,   787,    75,    96,   796,
      89,   618,    80,    89,   796,   796,    90,    90,   525,    43,
      31,    91,   629,   787,   833,   632,    89,    91,   763,   838,
      91,    89,   796,    61,    62,     1,    90,    65,    66,    67,
      68,     5,     6,     7,     8,    95,   553,   554,    46,    13,
      14,    15,    91,    89,    89,    89,   705,     7,     8,   763,
      28,   796,    94,    13,    14,    15,   268,    31,    94,   271,
      91,    90,    16,   275,    18,    19,    20,    21,   781,    90,
      90,    31,   955,    89,    89,     5,     6,     7,     8,   792,
     793,    94,   796,    13,    14,    15,    89,   970,   705,    46,
     887,    46,     3,   976,     5,   887,   887,    46,    90,    90,
     897,    12,    13,    14,   763,     8,   897,    66,    90,    90,
     907,  1089,    91,   887,    85,   828,   907,   634,   635,    54,
      55,    56,    57,    58,    59,    60,    37,    90,    39,     3,
      41,    89,    43,    95,    45,    91,    47,   796,    49,    46,
      51,    94,   887,    90,    90,    66,   763,    90,    46,    66,
    1033,  1034,   897,  1036,    17,    86,   869,    85,    69,    46,
      90,    90,   907,    89,     7,     8,     5,     6,     7,     8,
      13,    14,    15,   887,    13,    14,    15,    66,    66,   796,
     893,    66,  1000,   897,    66,    94,    91,    91,    31,    18,
     101,    90,   103,   907,    89,    89,     5,     6,     7,     8,
      90,    66,    94,    89,    13,    14,    15,     1,   420,   421,
       4,     5,     6,     7,     8,   427,   929,    89,    89,    13,
      14,    15,    31,    90,  1107,    89,    96,    10,   887,    90,
    1113,   443,    90,    66,    10,    29,    90,    31,    89,  1036,
      89,    10,    90,   632,  1036,  1036,   410,   512,  1045,   438,
     439,    90,  1036,  1045,  1045,  1138,  1139,   555,   412,    53,
     398,  1058,  1036,   511,   412,   811,  1058,  1058,   943,   847,
     887,  1045,   992,   572,   441,   792,   793,   926,  1097,   992,
     993,    75,   929,  1040,  1058,   570,    80,   449,  1001,  1002,
     507,  1036,  1005,  1006,     4,     5,     6,     7,     8,   210,
    1045,    95,  1015,    13,    14,    15,   757,  1042,   520,  1115,
     981,   828,  1117,   637,   644,   578,  1045,   514,   578,    29,
      93,    31,  1036,   889,     5,     6,     7,     8,   773,  1089,
     705,  1045,    13,    14,    15,   246,   779,   248,  1015,   250,
     684,   252,   574,    53,   574,   256,   588,   258,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,   269,   473,
      -1,   550,   551,   274,   468,    75,    -1,    -1,    -1,    -1,
      80,    -1,    -1,    -1,    -1,   286,    -1,  1036,    -1,    -1,
      90,    -1,    -1,    -1,   901,   902,  1045,    -1,    -1,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,   316,    -1,    -1,    -1,    90,
      -1,    -1,     5,     6,     7,     8,    -1,    -1,   329,  1036,
      13,    14,    15,    -1,    -1,   637,    -1,    -1,  1045,    -1,
     341,    37,    38,    39,    40,   346,    -1,   348,    31,    45,
      46,    47,    48,   632,    -1,    -1,    -1,    -1,    -1,    -1,
       5,     6,     7,     8,   643,   644,    -1,   669,    13,    14,
      15,   673,    -1,    -1,   676,   677,    -1,    -1,   680,    -1,
      -1,    41,    42,    43,    44,   992,   993,    -1,    -1,    49,
      50,    51,    52,    -1,  1001,  1002,    -1,    -1,  1005,  1006,
      -1,     4,   403,    -1,     7,     8,    -1,    -1,  1015,    -1,
      13,    14,    15,    -1,    -1,     5,     6,     7,     8,    -1,
      -1,    -1,   701,    13,    14,    15,    29,    -1,    31,   430,
      -1,   432,    -1,   434,    -1,   436,    -1,    -1,    -1,    -1,
     441,    31,    -1,    -1,    -1,    -1,  1053,  1054,  1055,   450,
      53,    -1,    -1,    -1,    -1,   327,   328,    -1,    -1,    37,
      38,    39,    40,    -1,    -1,    -1,    -1,    45,    46,    47,
      48,    -1,    75,   345,   753,   754,    -1,    80,   350,   351,
      -1,    -1,    -1,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   233,    -1,   776,   777,     5,
       6,     7,     8,   782,   783,    -1,    -1,    13,    14,    15,
      -1,   813,    -1,    -1,    -1,   817,    -1,    -1,   519,    -1,
      -1,    -1,   824,    -1,    -1,    -1,     5,     6,     7,     8,
      -1,    -1,    -1,   405,    13,    14,    15,    -1,    -1,    -1,
      -1,   542,    -1,   544,    -1,   546,    -1,   548,    -1,    -1,
     246,   247,   248,   249,    -1,    -1,    -1,    -1,     1,    -1,
      -1,     4,     5,     6,     7,     8,    -1,    -1,    -1,    -1,
      13,    14,    15,    -1,   575,    -1,    -1,    -1,   579,    -1,
     581,    -1,   583,    -1,    -1,    28,    29,    -1,    31,    -1,
     250,   251,   252,   253,    -1,    -1,   898,    -1,   900,    -1,
      43,   903,    -1,    -1,    -1,    -1,   908,   909,    -1,   911,
      53,    -1,    -1,   485,   486,    -1,    -1,   618,    -1,    -1,
      -1,    -1,    -1,   925,    -1,    -1,   928,    -1,   629,   931,
     932,   632,    75,    -1,   913,   914,    -1,    80,    -1,    -1,
       5,     6,     7,     8,    -1,    -1,    -1,    90,    13,    14,
      15,    94,    95,    -1,    -1,    -1,    -1,   959,    -1,    -1,
     532,   533,    -1,    -1,    41,    42,    43,    44,   246,   247,
     248,   249,    49,    50,    51,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   685,    -1,   687,    -1,   689,    -1,
     691,    -1,   693,   995,   695,   997,   697,   569,   699,    -1,
      -1,    -1,    -1,    -1,   705,    -1,    -1,    -1,    -1,    -1,
     582,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,
      -1,     4,     5,     6,     7,     8,    -1,    -1,    -1,   601,
      13,    14,    15,    -1,   430,   431,   432,   433,    53,    54,
      55,    56,    57,    58,    59,    60,    29,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1059,  1060,    -1,
    1062,    -1,   763,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      53,    -1,    -1,   511,   434,   435,   436,   437,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    -1,   660,    -1,
      -1,    -1,    75,    -1,    -1,   796,   668,    80,    -1,    -1,
     672,    -1,    -1,    -1,   542,   543,   544,   545,   546,   547,
     548,   549,    95,     1,    -1,    -1,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    -1,    13,    14,    15,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    29,   714,    -1,    -1,    -1,   542,   543,    -1,    -1,
     546,   547,   430,   431,   432,   433,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   736,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   250,   251,   252,   253,    -1,    -1,    -1,
      -1,    11,    -1,    -1,   544,   545,   887,    75,   548,   549,
      -1,    -1,    80,    -1,    -1,    -1,   897,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   906,   907,    95,    -1,    -1,
      -1,    -1,    -1,    43,    44,    45,    -1,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   811,
      -1,    -1,   814,    -1,    -1,    -1,   818,   685,   686,   687,
     688,   689,   690,   691,   692,   693,   694,   695,   696,   697,
     698,   699,   700,   835,   542,   543,    96,   839,   546,   547,
      -1,    -1,    -1,    -1,    -1,    -1,   848,   849,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,   685,
     686,   687,   688,    -1,    -1,    -1,    -1,   693,   694,   695,
     696,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,     1,    -1,    -1,     4,    -1,    -1,     7,
       8,    -1,    -1,    -1,    -1,    13,    14,    15,    -1,   689,
     690,   691,   692,    -1,    -1,  1036,    -1,   697,   698,   699,
     700,    29,    -1,    31,  1045,    -1,    -1,    -1,   920,     1,
      -1,    -1,     4,     5,     6,     7,     8,   434,   435,   436,
     437,    13,    14,    15,    -1,    53,    -1,    -1,   940,    -1,
      -1,   943,    -1,   945,   946,    -1,    -1,    29,   950,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    75,    76,    77,
      78,    -1,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    53,    90,    91,    -1,    -1,    -1,   685,   686,   687,
     688,    -1,    -1,    31,   986,   693,   694,   695,   696,    -1,
      -1,    -1,    -1,    75,    -1,    43,    44,    45,    80,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    95,    -1,  1017,    -1,    -1,  1020,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,   544,   545,    -1,
      -1,   548,   549,    -1,    -1,     1,  1048,     3,     4,     5,
       6,     7,     8,     9,    10,    -1,    12,    13,    14,    15,
      16,    -1,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    -1,    -1,    -1,    42,    -1,    -1,    -1,
      -1,    -1,    48,    -1,    -1,    51,    -1,    53,    -1,    -1,
      56,    57,    58,    -1,     4,    61,    62,     7,     8,    -1,
      66,    67,    -1,    13,    14,    15,    72,    -1,    74,    75,
      -1,    -1,    -1,    79,    80,    -1,    -1,    83,    84,    29,
      -1,    87,    88,    -1,    90,    91,    92,    93,    -1,    95,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,     1,    -1,
      -1,     4,    -1,    -1,     7,     8,    -1,    -1,    -1,    -1,
      13,    14,    15,    -1,    -1,    75,    -1,    -1,    -1,    -1,
      80,    -1,   689,   690,   691,   692,    29,    -1,    31,    32,
     697,   698,   699,   700,     1,    -1,     3,     4,     5,     6,
       7,     8,     9,    10,    -1,    12,    13,    14,    15,    16,
      53,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    -1,    34,    35,    36,
      37,    38,    75,    -1,    -1,    42,    -1,    80,    -1,    -1,
      -1,    48,    -1,    -1,    51,    -1,    53,    90,    91,    56,
      57,    58,    -1,    -1,    61,    62,    -1,    -1,    -1,    66,
      67,    -1,    -1,    -1,    -1,    72,    -1,    74,    75,    -1,
      -1,    -1,    79,    80,    -1,    -1,    83,    84,    -1,    -1,
      87,    88,    -1,    90,    -1,    92,    93,     1,    95,     3,
       4,    -1,    -1,    -1,    -1,     9,    10,    -1,    12,    -1,
      -1,    -1,    16,    -1,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    -1,    30,    -1,    32,    -1,
      34,    35,    36,    37,    38,    -1,    -1,    -1,    42,    -1,
      -1,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    56,    57,    58,    -1,    -1,    61,    62,    -1,
      -1,    -1,    66,    67,    -1,    -1,    -1,    -1,    72,    -1,
      74,    75,    -1,    -1,    -1,    79,    80,    -1,    -1,    83,
      84,    -1,    -1,    87,    88,    -1,    90,    -1,    92,    93,
       1,    95,     3,     4,    -1,    -1,    -1,    -1,     9,    10,
      -1,    12,    -1,    -1,    -1,    16,    -1,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    -1,    30,
      -1,    32,    -1,    34,    35,    36,    37,    38,    -1,    -1,
      -1,    42,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    56,    57,    58,    -1,    -1,
      61,    62,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,
      -1,    72,    -1,    74,    75,    -1,    -1,    -1,    79,    80,
      -1,    -1,    83,    84,    -1,    -1,    87,    88,    -1,    90,
      -1,    92,    93,     1,    95,     3,     4,    -1,    -1,     7,
       8,     9,    10,    -1,    12,    13,    14,    15,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    29,    30,    31,    32,    -1,    34,    35,    36,    37,
      38,    -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,
      48,    -1,    -1,    51,    -1,    53,    -1,    -1,    56,    57,
      58,    -1,    -1,    61,    62,    -1,    -1,    -1,    66,    67,
      -1,    -1,    -1,    -1,    72,    -1,    74,    75,    -1,    -1,
      -1,    79,    80,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      88,    -1,    -1,    -1,    92,    93,     1,    95,     3,     4,
      -1,    -1,    -1,    -1,     9,    10,     1,    12,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    -1,    13,    14,
      15,    -1,    -1,    -1,    -1,    30,    -1,    32,    -1,    34,
      35,    36,    37,    38,    29,    -1,    31,    42,    -1,    -1,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,
      -1,    56,    57,    58,    -1,    -1,    61,    62,    53,    -1,
      65,    66,    67,    58,    -1,    -1,    -1,    72,    -1,    74,
      75,    66,    -1,    -1,    79,    80,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    88,    -1,    80,    91,    92,    93,     1,
      95,     3,     4,    -1,    -1,    90,    -1,     9,    10,    45,
      12,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    -1,    -1,    -1,    30,    -1,
      32,    -1,    34,    35,    36,    37,    38,    -1,    -1,    -1,
      42,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    56,    57,    58,    -1,    -1,    61,
      62,    -1,    -1,    65,    66,    67,    -1,    -1,    -1,    -1,
      72,    -1,    74,    75,     1,    -1,     3,    79,    80,    -1,
      -1,    -1,     9,    10,    -1,    12,    88,    -1,    -1,    91,
      92,    93,    -1,    95,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    30,    -1,    32,    -1,    34,    35,    36,
      37,    38,    -1,    -1,    -1,    42,    43,    -1,    -1,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    -1,    56,
      57,    58,    -1,    -1,    61,    62,    -1,    -1,    65,    66,
      67,    -1,     1,    -1,     3,    72,    -1,    74,    -1,    -1,
       9,    10,    79,    12,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    -1,    -1,    92,    93,    -1,    95,    -1,
      -1,    30,    -1,    32,    -1,    34,    35,    36,    37,    38,
      -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    -1,    56,    57,    58,
      -1,    -1,    61,    62,    -1,    -1,    -1,    66,    67,    -1,
       1,    -1,     3,    72,    -1,    74,    -1,    -1,     9,    10,
      79,    12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    92,    93,    -1,    95,    -1,    -1,    30,
      -1,    32,    -1,    34,    35,    36,    37,    38,    -1,    -1,
      -1,    42,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    56,    57,    58,    -1,    -1,
      61,    62,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,
      -1,    72,    -1,    74,    -1,    -1,     1,    -1,    79,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    88,    13,    14,
      15,    92,    93,    -1,    95,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    28,    29,    -1,    31,     1,    -1,    -1,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    43,    13,
      14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,
      -1,    -1,    -1,    -1,    28,    29,    -1,    31,    -1,    -1,
      -1,    66,    67,    -1,    -1,    -1,    -1,    -1,    -1,    43,
      75,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,    -1,    94,
      95,    -1,    66,    67,    -1,    -1,     3,    -1,    -1,    -1,
      -1,    75,     9,    10,    -1,    12,    80,    -1,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    90,    13,    14,    15,
      94,    95,    -1,    30,    -1,    32,    -1,    34,    35,    36,
      37,    38,    -1,    29,    -1,    42,    -1,    -1,    -1,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    -1,    56,
      57,    58,    -1,    -1,    61,    62,    -1,    53,    -1,    66,
      67,    -1,    -1,    -1,     3,    72,    -1,    74,    -1,    -1,
       9,    10,    79,    12,    -1,    -1,    -1,    -1,    -1,    75,
      -1,    88,    -1,    -1,    80,    92,    93,    -1,    -1,    96,
      -1,    30,    -1,    32,    90,    34,    35,    36,    37,    38,
      -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    -1,    56,    57,    58,
      -1,    -1,    61,    62,    -1,    -1,    -1,    66,    67,    -1,
      -1,    -1,    -1,    72,    -1,    74,    -1,    -1,    -1,    -1,
      79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,
      -1,    -1,    -1,    92,    93,    -1,    -1,    96,     3,     4,
       5,     6,     7,     8,     9,    10,    -1,    12,    13,    14,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    -1,    34,
      35,    36,    37,    38,    -1,    -1,    -1,    42,    -1,    -1,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    53,    -1,
      -1,    56,    57,    58,    -1,    -1,    61,    62,    -1,    -1,
      -1,    66,    67,    -1,    -1,    -1,    -1,    72,    -1,    74,
      75,    -1,    -1,    -1,    79,    80,    -1,    -1,    83,    84,
      -1,    -1,    87,    88,    -1,    90,    -1,    92,    93,    -1,
      95,     3,     4,    -1,    -1,    -1,    -1,     9,    10,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      22,    23,    24,    25,    26,    27,    28,    -1,    30,    -1,
      32,    -1,    34,    35,    36,    37,    38,    -1,    -1,    -1,
      42,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,    51,
      -1,    -1,    -1,    -1,    56,    57,    58,    -1,    -1,    61,
      62,    -1,    -1,    -1,    66,    67,    -1,    -1,    -1,    -1,
      72,    -1,    74,    75,    -1,    -1,    -1,    79,    80,    -1,
      -1,    83,    84,    -1,    -1,    87,    88,    -1,    90,     3,
      92,    93,    -1,    95,    -1,     9,    10,     1,    12,     3,
       4,     5,     6,     7,     8,    -1,    -1,    11,    -1,    13,
      14,    15,    -1,    -1,    -1,    -1,    30,    -1,    32,    -1,
      34,    35,    36,    37,    38,    29,    -1,    -1,    42,    -1,
      -1,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,
      -1,    -1,    56,    57,    58,    -1,    -1,    61,    62,    53,
      -1,    -1,    66,    67,    -1,    -1,    -1,    -1,    72,    -1,
      74,    -1,    -1,    -1,    -1,    79,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    -1,    88,    -1,    80,    -1,    92,    93,
      -1,    95,     3,     4,    -1,    89,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      -1,    -1,     3,     4,     5,     6,     7,     8,     9,    10,
      -1,    12,    13,    14,    15,    46,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,    30,
      31,    32,    -1,    34,    35,    36,    37,    38,    -1,    -1,
      -1,    42,    -1,    -1,    75,    -1,    -1,    48,    -1,    80,
      51,    -1,    53,    -1,    -1,    56,    57,    58,    -1,    -1,
      61,    62,    -1,    94,    -1,    66,    67,    -1,    -1,    -1,
      -1,    72,    -1,    74,    75,    -1,    -1,    -1,    79,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    88,    -1,     3,
       4,    92,    93,     7,     8,     9,    10,    -1,    12,    13,
      14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    29,    30,    31,    32,    -1,
      34,    35,    36,    37,    38,    -1,    -1,    -1,    42,    -1,
      -1,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    53,
      -1,    -1,    56,    57,    58,    -1,    -1,    61,    62,    -1,
      -1,    -1,    66,    67,    -1,    -1,    -1,    -1,    72,    -1,
      74,    75,     3,     4,    -1,    79,    80,    -1,     9,    10,
      -1,    12,    -1,     4,    88,    -1,     7,     8,    92,    93,
      -1,    -1,    13,    14,    15,    -1,    -1,    -1,    -1,    30,
      -1,    32,    -1,    34,    35,    36,    37,    38,    29,    -1,
      31,    42,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    56,    57,    58,    -1,    -1,
      61,    62,    53,    -1,    -1,    66,    67,    -1,    -1,    -1,
       3,    72,    -1,    74,    75,    -1,     9,    10,    79,    12,
      -1,    -1,    -1,    -1,    75,    -1,    -1,    88,    -1,    80,
      -1,    92,    93,    -1,    -1,    -1,    -1,    30,    -1,    32,
      -1,    34,    35,    36,    37,    38,    -1,    -1,    -1,    42,
      -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    56,    57,    58,    -1,    -1,    61,    62,
      -1,    -1,    -1,    66,    67,    -1,     3,    -1,    -1,    72,
      -1,    74,     9,    10,    -1,    12,    79,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    88,    -1,    90,    -1,    92,
      93,    -1,    -1,    30,    -1,    32,    -1,    34,    35,    36,
      37,    38,    -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,
      -1,    48,    -1,    -1,    51,    -1,    -1,    -1,    -1,    56,
      57,    58,    -1,    -1,    61,    62,    -1,    -1,    -1,    66,
      67,    -1,     3,    -1,    -1,    72,    -1,    74,     9,    10,
      -1,    12,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    88,    -1,    90,    -1,    92,    93,    -1,    -1,    30,
      -1,    32,    -1,    34,    35,    36,    37,    38,    -1,    -1,
      -1,    42,    -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,
      51,    -1,    -1,    -1,    -1,    56,    57,    58,    -1,    -1,
      61,    62,    -1,    -1,    -1,    66,    67,    -1,     3,    -1,
      -1,    72,    -1,    74,     9,    10,     1,    12,    79,     4,
       5,     6,     7,     8,    -1,    -1,    11,    88,    13,    14,
      15,    92,    93,    -1,    -1,    30,    -1,    32,    -1,    34,
      35,    36,    37,    38,    29,    -1,    -1,    42,    -1,    -1,
      -1,    -1,    -1,    48,    -1,    -1,    51,    -1,    -1,    -1,
      -1,    56,    57,    58,    -1,    -1,    61,    62,    53,    -1,
      -1,    66,    67,    -1,     3,    -1,    -1,    72,    -1,    74,
       9,    10,    -1,    12,    79,    -1,    -1,    -1,    -1,    -1,
      75,    -1,    -1,    88,    -1,    80,    -1,    92,    93,    -1,
      -1,    30,    -1,    32,    89,    34,    35,    36,    37,    38,
      -1,    -1,    -1,    42,    -1,    -1,    -1,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    -1,    56,    57,    58,
      -1,    -1,    61,    62,    -1,    -1,    -1,    66,    67,    -1,
       3,    -1,    -1,    72,    -1,    74,     9,    10,    -1,    12,
      79,     4,     5,     6,     7,     8,    -1,    -1,    11,    88,
      13,    14,    15,    92,    93,    -1,    -1,    30,    -1,    32,
      -1,    34,    35,    36,    37,    38,    29,    -1,    31,    42,
      -1,    -1,    -1,    -1,    -1,    48,    -1,    -1,    51,    -1,
      -1,    -1,    -1,    56,    57,    58,    -1,    -1,    61,    62,
      53,    -1,    -1,    66,    67,    -1,    -1,     1,    -1,    72,
       4,    74,    -1,     7,     8,    -1,    79,    -1,    -1,    13,
      14,    15,    75,    -1,    -1,    88,    -1,    80,    -1,    92,
      93,    -1,    -1,    -1,    -1,    29,     1,    31,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    -1,    13,    14,
      15,     4,     5,     6,     7,     8,    -1,    -1,    -1,    53,
      13,    14,    15,    28,    29,    -1,    31,    32,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    29,    -1,    31,    -1,
      -1,    75,    76,    77,    78,    -1,    80,    -1,    53,    -1,
      -1,    56,    57,    58,    -1,    -1,    90,    91,    -1,    -1,
      53,    66,    -1,    -1,    69,    70,    71,    -1,    -1,    -1,
      75,    -1,    -1,    -1,    79,    80,    81,    82,    -1,    -1,
      -1,    -1,    75,    -1,    -1,    90,    -1,    80,    -1,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    90,    13,    14,
      15,     4,     5,     6,     7,     8,    -1,    -1,    -1,    -1,
      13,    14,    15,    -1,    29,    -1,    31,    -1,    -1,    -1,
       4,     5,     6,     7,     8,    -1,    29,    -1,    31,    13,
      14,    15,     4,     5,     6,     7,     8,    -1,    53,    -1,
      -1,    13,    14,    15,    -1,    29,    -1,    -1,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,
      75,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,    53,
      -1,    -1,    75,    -1,    -1,    90,    -1,    80,    -1,    -1,
      -1,    53,    -1,    -1,    -1,    -1,    -1,    90,    -1,    -1,
       4,    75,    -1,     7,     8,    -1,    80,    -1,    -1,    13,
      14,    15,    -1,    75,    -1,    -1,    90,    -1,    80,    -1,
       4,     5,     6,     7,     8,    29,    -1,    31,    90,    13,
      14,    15,    -1,    -1,     1,    -1,    -1,     4,    -1,    -1,
       7,     8,    -1,    -1,    -1,    29,    13,    14,    15,    53,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    29,    -1,    31,    32,    -1,    -1,    -1,    53,
      -1,    75,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    53,    -1,    -1,    -1,
      -1,    75,    -1,    -1,    -1,    -1,    80,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    90,    -1,    75,    -1,
       3,     4,    -1,    80,     7,     8,    -1,    -1,    -1,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,    -1,    -1,     7,
       8,    -1,    -1,    46,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    66,    -1,    -1,    -1,    -1,    -1,    -1,
       3,     4,    75,    -1,     7,     8,    -1,    80,    46,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,     3,     4,    75,    -1,     7,
       8,    -1,    80,    46,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,     4,     5,     6,     7,     8,    -1,
      -1,    -1,    75,    13,    14,    15,    -1,    80,    46,    -1,
      -1,    -1,    -1,     4,     5,     6,     7,     8,    -1,    29,
      -1,    31,    13,    14,    15,     4,     5,     6,     7,     8,
      -1,    -1,    11,    -1,    13,    14,    15,    75,    29,    -1,
      31,    -1,    80,    53,     4,     5,     6,     7,     8,    -1,
      29,    -1,    -1,    13,    14,    15,    -1,     4,     5,     6,
       7,     8,    53,    -1,    -1,    75,    13,    14,    15,    29,
      80,    31,    -1,    -1,    53,    -1,    -1,     4,     5,     6,
       7,     8,    29,    -1,    75,    -1,    13,    14,    15,    80,
      -1,    -1,    -1,    53,    -1,    -1,    75,    -1,    -1,    -1,
      -1,    80,    29,    -1,    31,    -1,    53,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    75,    13,    14,    15,    -1,
      80,    -1,    -1,    -1,    -1,    -1,    53,    -1,    75,    -1,
      -1,    -1,    29,    80,    31,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    -1,    13,    14,    15,    -1,    75,    -1,
      -1,    -1,    -1,    80,    -1,    -1,    53,    -1,    -1,    -1,
      29,     4,     5,     6,     7,     8,    -1,    -1,    -1,    -1,
      13,    14,    15,    -1,    -1,    -1,    -1,    -1,    75,    -1,
      -1,    -1,    -1,    80,    53,    -1,    29,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    -1,    13,    14,    15,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,
      53,    80,    29,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    -1,    13,    14,    15,    -1,    -1,    -1,    -1,    -1,
       4,    -1,    75,     7,     8,    -1,    53,    80,    29,    13,
      14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    29,    -1,    -1,    75,    -1,
      -1,    -1,    53,    80,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    53,
      -1,    -1,    -1,    -1,    75,    -1,    -1,    -1,    -1,    80,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    -1,    43,    44,    45,    80,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    43,    44,    45,    -1,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    43,    44,    45,    96,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    43,    44,    45,    96,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    43,    44,    45,    96,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    43,    44,    45,    96,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    -1,
      -1,    11,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    43,    44,    45,    94,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    -1,
      -1,    -1,    94,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    43,
      44,    45,    94,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    43,    44,    45,
      -1,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    89
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned short yystos[] =
{
       0,    98,    99,   100,     0,   101,     1,     4,     5,     6,
       7,     8,    13,    14,    15,    28,    29,    31,    32,    53,
      56,    57,    69,    70,    71,    75,    79,    80,    81,    82,
      90,   102,   103,   104,   105,   120,   136,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   162,   164,   165,   166,   167,
     168,   177,   178,   182,   205,   206,   207,   208,   213,   307,
     308,   310,   311,   314,   319,   322,   329,   330,   102,    90,
      91,   177,   177,   177,    66,    66,     3,     4,    75,    80,
     112,   309,   112,   112,   321,   322,   112,   309,   321,   309,
     112,    66,     3,    58,    66,   170,   174,   204,     8,   164,
     165,   177,   182,     8,   164,   165,   182,     8,   164,   165,
     177,   182,     8,   164,   165,   182,     8,   166,   167,   177,
     182,     8,   166,   167,   182,     8,   166,   167,   177,   182,
       8,   166,   167,   182,     8,   164,   165,   177,   182,     8,
     164,   165,   182,     8,   164,   165,   177,   182,     8,   164,
     165,   182,     8,   166,   167,   177,   182,     8,   166,   167,
     182,     8,   166,   167,   177,   182,     8,   166,   167,   182,
     136,   136,    90,   178,    95,   112,    95,   112,    95,   112,
     102,   331,     3,     9,    10,    12,    30,    34,    35,    36,
      37,    38,    42,    48,    51,    56,    57,    58,    61,    62,
      66,    67,    72,    74,    79,    88,    92,    93,   113,   114,
     116,   117,   118,   119,   121,   122,   128,   252,   307,   359,
     363,   364,   365,    66,    53,    94,    46,    66,   312,    66,
     312,   321,    90,    90,   112,   114,   139,   140,   141,   142,
     143,   144,   145,   146,   161,   225,   139,   140,   141,   142,
     160,   163,   176,   177,    90,    94,     1,    28,    66,    67,
     110,   171,   233,     4,    58,    66,    80,   169,   172,   199,
     200,   204,   170,   204,    73,   216,   217,    95,   216,    95,
     212,    95,     3,     4,     7,     8,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    46,    66,    75,    80,   340,
     349,   350,   351,   352,   353,   121,   121,    66,    66,    66,
     112,   121,     1,    95,   114,   225,     4,    75,   114,   358,
      66,    66,    66,   121,    89,    94,    66,   117,    66,   117,
      43,    44,    45,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    65,
      66,    67,    68,     1,    91,   241,   250,   121,     7,     8,
     112,   179,   180,   181,   182,   112,   112,   112,   321,   112,
     317,   320,    90,    89,   226,    89,   204,   204,   137,   176,
      66,   176,   302,     6,   160,   163,     1,   130,   131,   132,
     133,   240,   259,   176,   163,   176,    90,    94,     1,   106,
     171,    66,   233,    90,     1,   108,    66,    91,     1,    90,
     139,   140,   141,   142,   143,   144,   145,   146,   159,   160,
     218,   307,   209,    91,   210,     1,   112,   223,   224,   211,
      66,   112,   225,   332,    94,   347,   351,   353,    46,   122,
     122,   225,    89,    89,    89,    46,   351,   354,   355,   357,
      46,   351,   360,   361,   362,   225,   112,    90,   122,   225,
     225,   122,   122,   125,   127,   124,   123,   122,   122,   122,
     122,   122,   122,   122,   122,   122,   122,   122,   122,   112,
     115,   116,   114,   112,    89,    33,   245,   246,   247,    89,
      89,    94,    66,    89,   315,    89,    95,   313,   334,    58,
      66,   227,   229,   230,   231,   232,   233,    89,   174,   204,
      10,   303,   163,     6,    58,    96,   122,    90,   259,   240,
     132,   134,   139,   140,   143,   144,   147,   148,   151,   152,
     157,   158,    43,   200,   200,   137,   130,   176,   302,   130,
      75,   176,   136,   136,    90,   218,   216,   176,   216,    43,
      94,   215,   223,   225,    89,    90,   341,    11,   348,    66,
     112,    94,    94,    94,    95,   121,   116,   356,    46,    96,
     351,   357,    46,    89,   351,   362,    89,    89,    89,    89,
     114,    46,   122,   122,    89,    96,   112,   306,     1,   135,
     234,   235,   236,   237,   238,   239,   240,   251,   259,   262,
     263,   247,    89,   180,     3,   115,   321,   313,   323,   325,
      71,   329,   335,   337,   163,   176,   292,    66,   233,    89,
       1,     3,    11,   157,   158,   294,   297,   298,   300,   304,
     305,   122,   122,    96,    96,   111,    90,   136,    90,   136,
     175,    89,   172,   199,   259,    43,   259,    89,    46,   199,
     219,   221,    46,   204,   220,   222,    91,    91,   122,   224,
      91,   215,    89,   349,   350,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   240,   342,   343,   344,   333,   297,   298,
     225,   225,   122,   225,   129,   356,    46,   122,    90,    94,
     135,   262,   263,   135,   262,   263,   259,   262,   263,   135,
     262,   263,   240,    91,     3,     4,    22,    23,    24,    25,
      26,    27,    28,    75,    80,    83,    84,    87,    90,    95,
     112,   114,   138,   155,   156,   162,   243,   249,   253,   274,
     277,   279,   285,   307,    89,    94,    89,   318,   316,    76,
      77,    78,    91,   324,     1,    90,   159,   160,   326,   338,
     104,   136,   155,   156,   230,   231,   229,   293,   176,   292,
      89,   136,   301,   301,    89,    90,    94,    89,    94,    96,
      96,     1,   248,   253,   169,   170,     1,    95,   122,   183,
     107,   173,   109,   122,    46,   176,    94,   122,    46,   176,
      94,   176,   176,   176,    91,   112,   347,    90,   136,    90,
     344,   240,   248,    89,    89,    94,    89,     1,    65,    67,
      95,   112,   122,   185,   186,   187,   189,   191,   192,   126,
     112,   242,   122,    46,    90,    90,    90,   114,    58,   112,
       8,   286,    90,   114,   280,    66,   259,    90,   136,   136,
      90,    16,    18,    19,    20,    21,   254,   255,   257,   264,
     250,   275,   278,   138,   116,   334,   334,   325,   136,   136,
      90,   340,   136,   136,    89,   294,     4,    58,    66,    80,
     201,   202,   203,   204,   228,   229,   230,    58,    66,   204,
     228,   295,    11,   155,   156,   299,     3,   253,    90,    90,
     184,   248,   183,   248,   176,   122,   137,   176,   122,   137,
     176,   201,   204,   228,   345,   346,   112,   122,   112,   122,
     190,    46,    91,    94,   214,    43,    67,   192,   189,   122,
      11,    46,    90,   114,    90,    66,    90,   253,   114,    46,
     169,   193,   199,   170,   196,   204,   256,   266,   258,   268,
      66,    17,     1,   243,   261,     1,    66,   244,    86,   284,
      85,   281,   282,    89,    71,    71,    46,   199,   327,   328,
     327,   339,   163,   176,   176,    66,   233,    66,   233,   176,
     177,   163,   176,   176,   176,   136,   136,   185,   176,   221,
     176,   222,   176,   176,    90,    94,    89,    11,    96,   185,
     188,   187,   189,   122,   122,    90,   114,    89,   176,    90,
     194,    90,   197,    66,    66,   261,    66,   114,     1,   265,
     260,   262,   263,   114,   253,    66,   282,   122,    46,    94,
       1,    90,   336,   202,   203,   203,   302,   302,   296,   201,
     204,   228,   204,   228,    91,   346,   122,    91,   189,    46,
      46,    89,   276,   130,   130,   114,   114,    18,   114,   138,
     273,   287,    89,   261,   244,   260,    89,   299,   122,   137,
      89,   294,   176,   176,   176,    96,    10,    67,   288,   289,
     290,    90,   253,   259,   259,    89,    89,   269,    90,   272,
      90,    89,   328,    66,   112,    46,    89,    94,   195,   198,
     267,   287,   261,   283,   114,    96,   288,    90,   290,   253,
     253,   261,    90,   253,    89,    10,    46,    89,   270,    66,
      10,   291,    90,   287,   114,    89,    94,    89,    89,    90,
      10,   271,   261
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrlab1

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
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)         \
  Current.first_line   = Rhs[1].first_line;      \
  Current.first_column = Rhs[1].first_column;    \
  Current.last_line    = Rhs[N].last_line;       \
  Current.last_column  = Rhs[N].last_column;
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
} while (0)

# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Token, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (cinluded).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short *bottom, short *top)
#else
static void
yy_stack_print (bottom, top)
    short *bottom;
    short *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned int yylineno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylineno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
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
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  register const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yytype, yyvaluep)
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

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
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
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

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
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
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
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

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %s, ", yytname[yytoken]));

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
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
        case 2:
#line 343 "objc/objc-parse.y"
    { if (pedantic)
		    pedwarn ("ISO C forbids an empty source file");
		;}
    break;

  case 4:
#line 354 "objc/objc-parse.y"
    {yyval.ttype = NULL_TREE; ;}
    break;

  case 6:
#line 355 "objc/objc-parse.y"
    {yyval.ttype = NULL_TREE; ggc_collect(); ;}
    break;

  case 8:
#line 360 "objc/objc-parse.y"
    { parsing_iso_function_signature = false; ;}
    break;

  case 12:
#line 368 "objc/objc-parse.y"
    { STRIP_NOPS (yyvsp[-2].ttype);
		  if ((TREE_CODE (yyvsp[-2].ttype) == ADDR_EXPR
		       && TREE_CODE (TREE_OPERAND (yyvsp[-2].ttype, 0)) == STRING_CST)
		      || TREE_CODE (yyvsp[-2].ttype) == STRING_CST)
		    assemble_asm (yyvsp[-2].ttype);
		  else
		    error ("argument of `asm' is not a constant string"); ;}
    break;

  case 13:
#line 376 "objc/objc-parse.y"
    { RESTORE_EXT_FLAGS (yyvsp[-1].itype); ;}
    break;

  case 14:
#line 381 "objc/objc-parse.y"
    { if (pedantic)
		    error ("ISO C forbids data definition with no type or storage class");
		  else
		    warning ("data definition has no type or storage class");

		  POP_DECLSPEC_STACK; ;}
    break;

  case 15:
#line 388 "objc/objc-parse.y"
    { POP_DECLSPEC_STACK; ;}
    break;

  case 16:
#line 390 "objc/objc-parse.y"
    { POP_DECLSPEC_STACK; ;}
    break;

  case 17:
#line 392 "objc/objc-parse.y"
    { shadow_tag (yyvsp[-1].ttype); ;}
    break;

  case 20:
#line 396 "objc/objc-parse.y"
    { if (pedantic)
		    pedwarn ("ISO C does not allow extra `;' outside of a function"); ;}
    break;

  case 21:
#line 402 "objc/objc-parse.y"
    { if (! start_function (current_declspecs, yyvsp[0].ttype,
					all_prefix_attributes))
		    YYERROR1;
		;}
    break;

  case 22:
#line 407 "objc/objc-parse.y"
    { DECL_SOURCE_LOCATION (current_function_decl) = yyvsp[0].location;
		  store_parm_decls (); ;}
    break;

  case 23:
#line 410 "objc/objc-parse.y"
    { finish_function ();
		  POP_DECLSPEC_STACK; ;}
    break;

  case 24:
#line 413 "objc/objc-parse.y"
    { POP_DECLSPEC_STACK; ;}
    break;

  case 25:
#line 415 "objc/objc-parse.y"
    { if (! start_function (current_declspecs, yyvsp[0].ttype,
					all_prefix_attributes))
		    YYERROR1;
		;}
    break;

  case 26:
#line 420 "objc/objc-parse.y"
    { DECL_SOURCE_LOCATION (current_function_decl) = yyvsp[0].location;
		  store_parm_decls (); ;}
    break;

  case 27:
#line 423 "objc/objc-parse.y"
    { finish_function ();
		  POP_DECLSPEC_STACK; ;}
    break;

  case 28:
#line 426 "objc/objc-parse.y"
    { POP_DECLSPEC_STACK; ;}
    break;

  case 29:
#line 428 "objc/objc-parse.y"
    { if (! start_function (NULL_TREE, yyvsp[0].ttype,
					all_prefix_attributes))
		    YYERROR1;
		;}
    break;

  case 30:
#line 433 "objc/objc-parse.y"
    { DECL_SOURCE_LOCATION (current_function_decl) = yyvsp[0].location;
		  store_parm_decls (); ;}
    break;

  case 31:
#line 436 "objc/objc-parse.y"
    { finish_function ();
		  POP_DECLSPEC_STACK; ;}
    break;

  case 32:
#line 439 "objc/objc-parse.y"
    { POP_DECLSPEC_STACK; ;}
    break;

  case 37:
#line 450 "objc/objc-parse.y"
    { yyval.code = ADDR_EXPR; ;}
    break;

  case 38:
#line 452 "objc/objc-parse.y"
    { yyval.code = NEGATE_EXPR; ;}
    break;

  case 39:
#line 454 "objc/objc-parse.y"
    { yyval.code = CONVERT_EXPR;
		;}
    break;

  case 40:
#line 457 "objc/objc-parse.y"
    { yyval.code = PREINCREMENT_EXPR; ;}
    break;

  case 41:
#line 459 "objc/objc-parse.y"
    { yyval.code = PREDECREMENT_EXPR; ;}
    break;

  case 42:
#line 461 "objc/objc-parse.y"
    { yyval.code = BIT_NOT_EXPR; ;}
    break;

  case 43:
#line 463 "objc/objc-parse.y"
    { yyval.code = TRUTH_NOT_EXPR; ;}
    break;

  case 44:
#line 467 "objc/objc-parse.y"
    { yyval.ttype = build_compound_expr (yyvsp[0].ttype); ;}
    break;

  case 45:
#line 472 "objc/objc-parse.y"
    { yyval.ttype = NULL_TREE; ;}
    break;

  case 47:
#line 478 "objc/objc-parse.y"
    { yyval.ttype = build_tree_list (NULL_TREE, yyvsp[0].ttype); ;}
    break;

  case 48:
#line 480 "objc/objc-parse.y"
    { chainon (yyvsp[-2].ttype, build_tree_list (NULL_TREE, yyvsp[0].ttype)); ;}
    break;

  case 50:
#line 486 "objc/objc-parse.y"
    { yyval.ttype = build_indirect_ref (yyvsp[0].ttype, "unary *"); ;}
    break;

  case 51:
#line 489 "objc/objc-parse.y"
    { yyval.ttype = yyvsp[0].ttype;
		  RESTORE_EXT_FLAGS (yyvsp[-1].itype); ;}
    break;

  case 52:
#line 492 "objc/objc-parse.y"
    { yyval.ttype = build_unary_op (yyvsp[-1].code, yyvsp[0].ttype, 0);
		  overflow_warning (yyval.ttype); ;}
    break;

  case 53:
#line 496 "objc/objc-parse.y"
    { yyval.ttype = finish_label_address_expr (yyvsp[0].ttype); ;}
    break;

  case 54:
#line 498 "objc/objc-parse.y"
    { skip_evaluation--;
		  if (TREE_CODE (yyvsp[0].ttype) == COMPONENT_REF
		      && DECL_C_BIT_FIELD (TREE_OPERAND (yyvsp[0].ttype, 1)))
		    error ("`sizeof' applied to a bit-field");
		  yyval.ttype = c_sizeof (TREE_TYPE (yyvsp[0].ttype)); ;}
    break;

  case 55:
#line 504 "objc/objc-parse.y"
    { skip_evaluation--;
		  yyval.ttype = c_sizeof (groktypename (yyvsp[-1].ttype)); ;}
    break;

  case 56:
#line 507 "objc/objc-parse.y"
    { skip_evaluation--;
		  yyval.ttype = c_alignof_expr (yyvsp[0].ttype); ;}
    break;

  case 57:
#line 510 "objc/objc-parse.y"
    { skip_evaluation--;
		  yyval.ttype = c_alignof (groktypename (yyvsp[-1].ttype)); ;}
    break;

  case 58:
#line 513 "objc/objc-parse.y"
    { yyval.ttype = build_unary_op (REALPART_EXPR, yyvsp[0].ttype, 0); ;}
    break;

  case 59:
#line 515 "objc/objc-parse.y"
    { yyval.ttype = build_unary_op (IMAGPART_EXPR, yyvsp[0].ttype, 0); ;}
    break;

  case 60:
#line 519 "objc/objc-parse.y"
    { skip_evaluation++; ;}
    break;

  case 61:
#line 523 "objc/objc-parse.y"
    { skip_evaluation++; ;}
    break;

  case 62:
#line 527 "objc/objc-parse.y"
    { skip_evaluation++; ;}
    break;

  case 64:
#line 533 "objc/objc-parse.y"
    { yyval.ttype = c_cast_expr (yyvsp[-2].ttype, yyvsp[0].ttype); ;}
    break;

  case 66:
#line 539 "objc/objc-parse.y"
    { yyval.ttype = parser_build_binary_op (yyvsp[-1].code, yyvsp[-2].ttype, yyvsp[0].ttype); ;}
    break;

  case 67:
#line 541 "objc/objc-parse.y"
    { yyval.ttype = parser_build_binary_op (yyvsp[-1].code, yyvsp[-2].ttype, yyvsp[0].ttype); ;}
    break;

  case 68:
#line 543 "objc/objc-parse.y"
    { yyval.ttype = parser_build_binary_op (yyvsp[-1].code, yyvsp[-2].ttype, yyvsp[0].ttype); ;}
    break;

  case 69:
#line 545 "objc/objc-parse.y"
    { yyval.ttype = parser_build_binary_op (yyvsp[-1].code, yyvsp[-2].ttype, yyvsp[0].ttype); ;}
    break;

  case 70:
#line 547 "objc/objc-parse.y"
    { yyval.ttype = parser_build_binary_op (yyvsp[-1].code, yyvsp[-2].ttype, yyvsp[0].ttype); ;}
    break;

  case 71:
#line 549 "objc/objc-parse.y"
    { yyval.ttype = parser_build_binary_op (yyvsp[-1].code, yyvsp[-2].ttype, yyvsp[0].ttype); ;}
    break;

  case 72:
#line 551 "objc/objc-parse.y"
    { yyval.ttype = parser_build_binary_op (yyvsp[-1].code, yyvsp[-2].ttype, yyvsp[0].ttype); ;}
    break;

  case 73:
#line 553 "objc/objc-parse.y"
    { yyval.ttype = parser_build_binary_op (yyvsp[-1].code, yyvsp[-2].ttype, yyvsp[0].ttype); ;}
    break;

  case 74:
#line 555 "objc/objc-parse.y"
    { yyval.ttype = parser_build_binary_op (yyvsp[-1].code, yyvsp[-2].ttype, yyvsp[0].ttype); ;}
    break;

  case 75:
#line 557 "objc/objc-parse.y"
    { yyval.ttype = parser_build_binary_op (yyvsp[-1].code, yyvsp[-2].ttype, yyvsp[0].ttype); ;}
    break;

  case 76:
#line 559 "objc/objc-parse.y"
    { yyval.ttype = parser_build_binary_op (yyvsp[-1].code, yyvsp[-2].ttype, yyvsp[0].ttype); ;}
    break;

  case 77:
#line 561 "objc/objc-parse.y"
    { yyval.ttype = parser_build_binary_op (yyvsp[-1].code, yyvsp[-2].ttype, yyvsp[0].ttype); ;}
    break;

  case 78:
#line 563 "objc/objc-parse.y"
    { yyvsp[-1].ttype = c_common_truthvalue_conversion
		    (default_conversion (yyvsp[-1].ttype));
		  skip_evaluation += yyvsp[-1].ttype == truthvalue_false_node; ;}
    break;

  case 79:
#line 567 "objc/objc-parse.y"
    { skip_evaluation -= yyvsp[-3].ttype == truthvalue_false_node;
		  yyval.ttype = parser_build_binary_op (TRUTH_ANDIF_EXPR, yyvsp[-3].ttype, yyvsp[0].ttype); ;}
    break;

  case 80:
#line 570 "objc/objc-parse.y"
    { yyvsp[-1].ttype = c_common_truthvalue_conversion
		    (default_conversion (yyvsp[-1].ttype));
		  skip_evaluation += yyvsp[-1].ttype == truthvalue_true_node; ;}
    break;

  case 81:
#line 574 "objc/objc-parse.y"
    { skip_evaluation -= yyvsp[-3].ttype == truthvalue_true_node;
		  yyval.ttype = parser_build_binary_op (TRUTH_ORIF_EXPR, yyvsp[-3].ttype, yyvsp[0].ttype); ;}
    break;

  case 82:
#line 577 "objc/objc-parse.y"
    { yyvsp[-1].ttype = c_common_truthvalue_conversion
		    (default_conversion (yyvsp[-1].ttype));
		  skip_evaluation += yyvsp[-1].ttype == truthvalue_false_node; ;}
    break;

  case 83:
#line 581 "objc/objc-parse.y"
    { skip_evaluation += ((yyvsp[-4].ttype == truthvalue_true_node)
				      - (yyvsp[-4].ttype == truthvalue_false_node)); ;}
    break;

  case 84:
#line 584 "objc/objc-parse.y"
    { skip_evaluation -= yyvsp[-6].ttype == truthvalue_true_node;
		  yyval.ttype = build_conditional_expr (yyvsp[-6].ttype, yyvsp[-3].ttype, yyvsp[0].ttype); ;}
    break;

  case 85:
#line 587 "objc/objc-parse.y"
    { if (pedantic)
		    pedwarn ("ISO C forbids omitting the middle term of a ?: expression");
		  /* Make sure first operand is calculated only once.  */
		  yyvsp[0].ttype = save_expr (yyvsp[-1].ttype);
		  yyvsp[-1].ttype = c_common_truthvalue_conversion
		    (default_conversion (yyvsp[0].ttype));
		  skip_evaluation += yyvsp[-1].ttype == truthvalue_true_node; ;}
    break;

  case 86:
#line 595 "objc/objc-parse.y"
    { skip_evaluation -= yyvsp[-4].ttype == truthvalue_true_node;
		  yyval.ttype = build_conditional_expr (yyvsp[-4].ttype, yyvsp[-3].ttype, yyvsp[0].ttype); ;}
    break;

  case 87:
#line 598 "objc/objc-parse.y"
    { char class;
		  yyval.ttype = build_modify_expr (yyvsp[-2].ttype, NOP_EXPR, yyvsp[0].ttype);
		  class = TREE_CODE_CLASS (TREE_CODE (yyval.ttype));
		  if (IS_EXPR_CODE_CLASS (class))
		    C_SET_EXP_ORIGINAL_CODE (yyval.ttype, MODIFY_EXPR);
		;}
    break;

  case 88:
#line 605 "objc/objc-parse.y"
    { char class;
		  yyval.ttype = build_modify_expr (yyvsp[-2].ttype, yyvsp[-1].code, yyvsp[0].ttype);
		  /* This inhibits warnings in
		     c_common_truthvalue_conversion.  */
		  class = TREE_CODE_CLASS (TREE_CODE (yyval.ttype));
		  if (IS_EXPR_CODE_CLASS (class))
		    C_SET_EXP_ORIGINAL_CODE (yyval.ttype, ERROR_MARK);
		;}
    break;

  case 89:
#line 617 "objc/objc-parse.y"
    {
		  if (yychar == YYEMPTY)
		    yychar = YYLEX;
		  yyval.ttype = build_external_ref (yyvsp[0].ttype, yychar == '(');
		;}
    break;

  case 92:
#line 625 "objc/objc-parse.y"
    { yyval.ttype = fname_decl (C_RID_CODE (yyval.ttype), yyval.ttype); ;}
    break;

  case 93:
#line 627 "objc/objc-parse.y"
    { start_init (NULL_TREE, NULL, 0);
		  yyvsp[-2].ttype = groktypename (yyvsp[-2].ttype);
		  really_start_incremental_init (yyvsp[-2].ttype); ;}
    break;

  case 94:
#line 631 "objc/objc-parse.y"
    { tree constructor = pop_init_level (0);
		  tree type = yyvsp[-5].ttype;
		  finish_init ();

		  if (pedantic && ! flag_isoc99)
		    pedwarn ("ISO C90 forbids compound literals");
		  yyval.ttype = build_compound_literal (type, constructor);
		;}
    break;

  case 95:
#line 640 "objc/objc-parse.y"
    { char class = TREE_CODE_CLASS (TREE_CODE (yyvsp[-1].ttype));
		  if (IS_EXPR_CODE_CLASS (class))
		    C_SET_EXP_ORIGINAL_CODE (yyvsp[-1].ttype, ERROR_MARK);
		  yyval.ttype = yyvsp[-1].ttype; ;}
    break;

  case 96:
#line 645 "objc/objc-parse.y"
    { yyval.ttype = error_mark_node; ;}
    break;

  case 97:
#line 647 "objc/objc-parse.y"
    { tree saved_last_tree;

		   if (pedantic)
		     pedwarn ("ISO C forbids braced-groups within expressions");
		  saved_last_tree = COMPOUND_BODY (yyvsp[-2].ttype);
		  RECHAIN_STMTS (yyvsp[-2].ttype, COMPOUND_BODY (yyvsp[-2].ttype));
		  last_tree = saved_last_tree;
		  TREE_CHAIN (last_tree) = NULL_TREE;
		  if (!last_expr_type)
		    last_expr_type = void_type_node;
		  yyval.ttype = build1 (STMT_EXPR, last_expr_type, yyvsp[-2].ttype);
		  TREE_SIDE_EFFECTS (yyval.ttype) = 1;
		;}
    break;

  case 98:
#line 661 "objc/objc-parse.y"
    {
		  last_tree = COMPOUND_BODY (yyvsp[-2].ttype);
		  TREE_CHAIN (last_tree) = NULL_TREE;
		  yyval.ttype = error_mark_node;
		;}
    break;

  case 99:
#line 667 "objc/objc-parse.y"
    { yyval.ttype = build_function_call (yyvsp[-3].ttype, yyvsp[-1].ttype); ;}
    break;

  case 100:
#line 669 "objc/objc-parse.y"
    { yyval.ttype = build_va_arg (yyvsp[-3].ttype, groktypename (yyvsp[-1].ttype)); ;}
    break;

  case 101:
#line 672 "objc/objc-parse.y"
    {
                  tree c;

                  c = fold (yyvsp[-5].ttype);
                  STRIP_NOPS (c);
                  if (TREE_CODE (c) != INTEGER_CST)
                    error ("first argument to __builtin_choose_expr not a constant");
                  yyval.ttype = integer_zerop (c) ? yyvsp[-1].ttype : yyvsp[-3].ttype;
		;}
    break;

  case 102:
#line 682 "objc/objc-parse.y"
    {
		  tree e1, e2;

		  e1 = TYPE_MAIN_VARIANT (groktypename (yyvsp[-3].ttype));
		  e2 = TYPE_MAIN_VARIANT (groktypename (yyvsp[-1].ttype));

		  yyval.ttype = comptypes (e1, e2, COMPARE_STRICT)
		    ? build_int_2 (1, 0) : build_int_2 (0, 0);
		;}
    break;

  case 103:
#line 692 "objc/objc-parse.y"
    { yyval.ttype = build_array_ref (yyvsp[-3].ttype, yyvsp[-1].ttype); ;}
    break;

  case 104:
#line 694 "objc/objc-parse.y"
    {
		    if (!is_public (yyvsp[-2].ttype, yyvsp[0].ttype))
		      yyval.ttype = error_mark_node;
		    else
		      yyval.ttype = build_component_ref (yyvsp[-2].ttype, yyvsp[0].ttype);
		;}
    break;

  case 105:
#line 701 "objc/objc-parse.y"
    {
                  tree expr = build_indirect_ref (yyvsp[-2].ttype, "->");

		      if (!is_public (expr, yyvsp[0].ttype))
			yyval.ttype = error_mark_node;
		      else
			yyval.ttype = build_component_ref (expr, yyvsp[0].ttype);
		;}
    break;

  case 106:
#line 710 "objc/objc-parse.y"
    { yyval.ttype = build_unary_op (POSTINCREMENT_EXPR, yyvsp[-1].ttype, 0); ;}
    break;

  case 107:
#line 712 "objc/objc-parse.y"
    { yyval.ttype = build_unary_op (POSTDECREMENT_EXPR, yyvsp[-1].ttype, 0); ;}
    break;

  case 108:
#line 714 "objc/objc-parse.y"
    { yyval.ttype = build_message_expr (yyvsp[0].ttype); ;}
    break;

  case 109:
#line 716 "objc/objc-parse.y"
    { yyval.ttype = build_selector_expr (yyvsp[0].ttype); ;}
    break;

  case 110:
#line 718 "objc/objc-parse.y"
    { yyval.ttype = build_protocol_expr (yyvsp[0].ttype); ;}
    break;

  case 111:
#line 720 "objc/objc-parse.y"
    { yyval.ttype = build_encode_expr (yyvsp[0].ttype); ;}
    break;

  case 112:
#line 722 "objc/objc-parse.y"
    { yyval.ttype = build_objc_string_object (yyvsp[0].ttype); ;}
    break;

  case 113:
#line 727 "objc/objc-parse.y"
    {
	  parsing_iso_function_signature = false; /* Reset after decls.  */
	;}
    break;

  case 114:
#line 734 "objc/objc-parse.y"
    {
	  if (warn_traditional && !in_system_header
	      && parsing_iso_function_signature)
	    warning ("traditional C rejects ISO C style function definitions");
	  if (warn_old_style_definition && !in_system_header
	      && !parsing_iso_function_signature)
	    warning ("old-style parameter declaration");
	  parsing_iso_function_signature = false; /* Reset after warning.  */
	;}
    break;

  case 115:
#line 744 "objc/objc-parse.y"
    {
	  if (warn_old_style_definition && !in_system_header)
	    warning ("old-style parameter declaration");
	;}
    break;

  case 116:
#line 755 "objc/objc-parse.y"
    { ;}
    break;

  case 121:
#line 771 "objc/objc-parse.y"
    { POP_DECLSPEC_STACK; ;}
    break;

  case 122:
#line 773 "objc/objc-parse.y"
    { POP_DECLSPEC_STACK; ;}
    break;

  case 123:
#line 775 "objc/objc-parse.y"
    { shadow_tag_warned (yyvsp[-1].ttype, 1);
		  pedwarn ("empty declaration"); ;}
    break;

  case 124:
#line 778 "objc/objc-parse.y"
    { pedwarn ("empty declaration"); ;}
    break;

  case 125:
#line 787 "objc/objc-parse.y"
    { ;}
    break;

  case 126:
#line 795 "objc/objc-parse.y"
    { pending_xref_error ();
		  PUSH_DECLSPEC_STACK;
		  split_specs_attrs (yyvsp[0].ttype,
				     &current_declspecs, &prefix_attributes);
		  all_prefix_attributes = prefix_attributes; ;}
    break;

  case 127:
#line 806 "objc/objc-parse.y"
    { all_prefix_attributes = chainon (yyvsp[0].ttype, prefix_attributes); ;}
    break;

  case 128:
#line 811 "objc/objc-parse.y"
    { POP_DECLSPEC_STACK; ;}
    break;

  case 129:
#line 813 "objc/objc-parse.y"
    { POP_DECLSPEC_STACK; ;}
    break;

  case 130:
#line 815 "objc/objc-parse.y"
    { POP_DECLSPEC_STACK; ;}
    break;

  case 131:
#line 817 "objc/objc-parse.y"
    { POP_DECLSPEC_STACK; ;}
    break;

  case 132:
#line 819 "objc/objc-parse.y"
    { shadow_tag (yyvsp[-1].ttype); ;}
    break;

  case 133:
#line 821 "objc/objc-parse.y"
    { RESTORE_EXT_FLAGS (yyvsp[-1].itype); ;}
    break;

  case 134:
#line 878 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, NULL_TREE);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 135:
#line 881 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 136:
#line 884 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 137:
#line 890 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (yyvsp[0].ttype, NULL_TREE, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = TREE_STATIC (yyvsp[-1].ttype); ;}
    break;

  case 138:
#line 896 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 139:
#line 899 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 140:
#line 905 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (yyvsp[0].ttype, NULL_TREE, NULL_TREE);
		  TREE_STATIC (yyval.ttype) = 0; ;}
    break;

  case 141:
#line 908 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (yyvsp[0].ttype, NULL_TREE, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = TREE_STATIC (yyvsp[-1].ttype); ;}
    break;

  case 142:
#line 914 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, NULL_TREE);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 143:
#line 917 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 144:
#line 920 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 145:
#line 923 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 146:
#line 926 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 147:
#line 929 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 148:
#line 932 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 149:
#line 938 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, NULL_TREE);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 150:
#line 941 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (yyvsp[0].ttype, NULL_TREE, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = TREE_STATIC (yyvsp[-1].ttype); ;}
    break;

  case 151:
#line 944 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 152:
#line 947 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 153:
#line 950 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 154:
#line 953 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 155:
#line 959 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 156:
#line 962 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 157:
#line 965 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 158:
#line 968 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 159:
#line 971 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 160:
#line 974 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 161:
#line 980 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (yyvsp[0].ttype, NULL_TREE, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = TREE_STATIC (yyvsp[-1].ttype); ;}
    break;

  case 162:
#line 983 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 163:
#line 986 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 164:
#line 989 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 165:
#line 992 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 166:
#line 998 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, NULL_TREE);
		  TREE_STATIC (yyval.ttype) = 0; ;}
    break;

  case 167:
#line 1001 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 168:
#line 1004 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 169:
#line 1007 "objc/objc-parse.y"
    { if (extra_warnings && TREE_STATIC (yyvsp[-1].ttype))
		    warning ("`%s' is not at beginning of declaration",
			     IDENTIFIER_POINTER (yyvsp[0].ttype));
		  yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = TREE_STATIC (yyvsp[-1].ttype); ;}
    break;

  case 170:
#line 1013 "objc/objc-parse.y"
    { if (extra_warnings && TREE_STATIC (yyvsp[-1].ttype))
		    warning ("`%s' is not at beginning of declaration",
			     IDENTIFIER_POINTER (yyvsp[0].ttype));
		  yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = TREE_STATIC (yyvsp[-1].ttype); ;}
    break;

  case 171:
#line 1019 "objc/objc-parse.y"
    { if (extra_warnings && TREE_STATIC (yyvsp[-1].ttype))
		    warning ("`%s' is not at beginning of declaration",
			     IDENTIFIER_POINTER (yyvsp[0].ttype));
		  yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = TREE_STATIC (yyvsp[-1].ttype); ;}
    break;

  case 172:
#line 1025 "objc/objc-parse.y"
    { if (extra_warnings && TREE_STATIC (yyvsp[-1].ttype))
		    warning ("`%s' is not at beginning of declaration",
			     IDENTIFIER_POINTER (yyvsp[0].ttype));
		  yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = TREE_STATIC (yyvsp[-1].ttype); ;}
    break;

  case 173:
#line 1034 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (yyvsp[0].ttype, NULL_TREE, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = TREE_STATIC (yyvsp[-1].ttype); ;}
    break;

  case 174:
#line 1040 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 175:
#line 1043 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 176:
#line 1046 "objc/objc-parse.y"
    { if (extra_warnings && TREE_STATIC (yyvsp[-1].ttype))
		    warning ("`%s' is not at beginning of declaration",
			     IDENTIFIER_POINTER (yyvsp[0].ttype));
		  yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = TREE_STATIC (yyvsp[-1].ttype); ;}
    break;

  case 177:
#line 1052 "objc/objc-parse.y"
    { if (extra_warnings && TREE_STATIC (yyvsp[-1].ttype))
		    warning ("`%s' is not at beginning of declaration",
			     IDENTIFIER_POINTER (yyvsp[0].ttype));
		  yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = TREE_STATIC (yyvsp[-1].ttype); ;}
    break;

  case 178:
#line 1058 "objc/objc-parse.y"
    { if (extra_warnings && TREE_STATIC (yyvsp[-1].ttype))
		    warning ("`%s' is not at beginning of declaration",
			     IDENTIFIER_POINTER (yyvsp[0].ttype));
		  yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = TREE_STATIC (yyvsp[-1].ttype); ;}
    break;

  case 179:
#line 1064 "objc/objc-parse.y"
    { if (extra_warnings && TREE_STATIC (yyvsp[-1].ttype))
		    warning ("`%s' is not at beginning of declaration",
			     IDENTIFIER_POINTER (yyvsp[0].ttype));
		  yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = TREE_STATIC (yyvsp[-1].ttype); ;}
    break;

  case 180:
#line 1073 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (yyvsp[0].ttype, NULL_TREE, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = TREE_STATIC (yyvsp[-1].ttype); ;}
    break;

  case 181:
#line 1079 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 182:
#line 1082 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 183:
#line 1085 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 184:
#line 1088 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 185:
#line 1091 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 186:
#line 1094 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 187:
#line 1097 "objc/objc-parse.y"
    { if (extra_warnings && TREE_STATIC (yyvsp[-1].ttype))
		    warning ("`%s' is not at beginning of declaration",
			     IDENTIFIER_POINTER (yyvsp[0].ttype));
		  yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = TREE_STATIC (yyvsp[-1].ttype); ;}
    break;

  case 188:
#line 1103 "objc/objc-parse.y"
    { if (extra_warnings && TREE_STATIC (yyvsp[-1].ttype))
		    warning ("`%s' is not at beginning of declaration",
			     IDENTIFIER_POINTER (yyvsp[0].ttype));
		  yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = TREE_STATIC (yyvsp[-1].ttype); ;}
    break;

  case 189:
#line 1109 "objc/objc-parse.y"
    { if (extra_warnings && TREE_STATIC (yyvsp[-1].ttype))
		    warning ("`%s' is not at beginning of declaration",
			     IDENTIFIER_POINTER (yyvsp[0].ttype));
		  yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = TREE_STATIC (yyvsp[-1].ttype); ;}
    break;

  case 190:
#line 1115 "objc/objc-parse.y"
    { if (extra_warnings && TREE_STATIC (yyvsp[-1].ttype))
		    warning ("`%s' is not at beginning of declaration",
			     IDENTIFIER_POINTER (yyvsp[0].ttype));
		  yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = TREE_STATIC (yyvsp[-1].ttype); ;}
    break;

  case 191:
#line 1124 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (yyvsp[0].ttype, NULL_TREE, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = TREE_STATIC (yyvsp[-1].ttype); ;}
    break;

  case 192:
#line 1127 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 193:
#line 1130 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 194:
#line 1133 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 195:
#line 1136 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 196:
#line 1142 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 197:
#line 1145 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 198:
#line 1148 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 199:
#line 1151 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 200:
#line 1154 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 201:
#line 1157 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 202:
#line 1160 "objc/objc-parse.y"
    { if (extra_warnings && TREE_STATIC (yyvsp[-1].ttype))
		    warning ("`%s' is not at beginning of declaration",
			     IDENTIFIER_POINTER (yyvsp[0].ttype));
		  yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = TREE_STATIC (yyvsp[-1].ttype); ;}
    break;

  case 203:
#line 1166 "objc/objc-parse.y"
    { if (extra_warnings && TREE_STATIC (yyvsp[-1].ttype))
		    warning ("`%s' is not at beginning of declaration",
			     IDENTIFIER_POINTER (yyvsp[0].ttype));
		  yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = TREE_STATIC (yyvsp[-1].ttype); ;}
    break;

  case 204:
#line 1172 "objc/objc-parse.y"
    { if (extra_warnings && TREE_STATIC (yyvsp[-1].ttype))
		    warning ("`%s' is not at beginning of declaration",
			     IDENTIFIER_POINTER (yyvsp[0].ttype));
		  yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = TREE_STATIC (yyvsp[-1].ttype); ;}
    break;

  case 205:
#line 1178 "objc/objc-parse.y"
    { if (extra_warnings && TREE_STATIC (yyvsp[-1].ttype))
		    warning ("`%s' is not at beginning of declaration",
			     IDENTIFIER_POINTER (yyvsp[0].ttype));
		  yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = TREE_STATIC (yyvsp[-1].ttype); ;}
    break;

  case 206:
#line 1187 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (yyvsp[0].ttype, NULL_TREE, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = TREE_STATIC (yyvsp[-1].ttype); ;}
    break;

  case 207:
#line 1190 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 208:
#line 1193 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 209:
#line 1196 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 210:
#line 1199 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-1].ttype);
		  TREE_STATIC (yyval.ttype) = 1; ;}
    break;

  case 267:
#line 1287 "objc/objc-parse.y"
    { yyval.ttype = NULL_TREE; ;}
    break;

  case 268:
#line 1289 "objc/objc-parse.y"
    { yyval.ttype = yyvsp[0].ttype; ;}
    break;

  case 272:
#line 1324 "objc/objc-parse.y"
    { OBJC_NEED_RAW_IDENTIFIER (1);	;}
    break;

  case 275:
#line 1334 "objc/objc-parse.y"
    { /* For a typedef name, record the meaning, not the name.
		     In case of `foo foo, bar;'.  */
		  yyval.ttype = lookup_name (yyvsp[0].ttype); ;}
    break;

  case 276:
#line 1338 "objc/objc-parse.y"
    { yyval.ttype = get_static_reference (yyvsp[-1].ttype, yyvsp[0].ttype); ;}
    break;

  case 277:
#line 1340 "objc/objc-parse.y"
    { yyval.ttype = get_object_reference (yyvsp[0].ttype); ;}
    break;

  case 278:
#line 1345 "objc/objc-parse.y"
    { yyval.ttype = get_object_reference (yyvsp[0].ttype); ;}
    break;

  case 279:
#line 1347 "objc/objc-parse.y"
    { skip_evaluation--;
		  if (TREE_CODE (yyvsp[-1].ttype) == COMPONENT_REF
		      && DECL_C_BIT_FIELD (TREE_OPERAND (yyvsp[-1].ttype, 1)))
		    error ("`typeof' applied to a bit-field");
		  yyval.ttype = TREE_TYPE (yyvsp[-1].ttype); ;}
    break;

  case 280:
#line 1353 "objc/objc-parse.y"
    { skip_evaluation--; yyval.ttype = groktypename (yyvsp[-1].ttype); ;}
    break;

  case 285:
#line 1370 "objc/objc-parse.y"
    { yyval.ttype = NULL_TREE; ;}
    break;

  case 286:
#line 1372 "objc/objc-parse.y"
    { yyval.ttype = yyvsp[-1].ttype; ;}
    break;

  case 287:
#line 1377 "objc/objc-parse.y"
    { yyval.ttype = start_decl (yyvsp[-3].ttype, current_declspecs, 1,
					  chainon (yyvsp[-1].ttype, all_prefix_attributes));
		  start_init (yyval.ttype, yyvsp[-2].ttype, global_bindings_p ()); ;}
    break;

  case 288:
#line 1382 "objc/objc-parse.y"
    { finish_init ();
		  finish_decl (yyvsp[-1].ttype, yyvsp[0].ttype, yyvsp[-4].ttype); ;}
    break;

  case 289:
#line 1385 "objc/objc-parse.y"
    { tree d = start_decl (yyvsp[-2].ttype, current_declspecs, 0,
				       chainon (yyvsp[0].ttype, all_prefix_attributes));
		  finish_decl (d, NULL_TREE, yyvsp[-1].ttype);
                ;}
    break;

  case 290:
#line 1393 "objc/objc-parse.y"
    { yyval.ttype = start_decl (yyvsp[-3].ttype, current_declspecs, 1,
					  chainon (yyvsp[-1].ttype, all_prefix_attributes));
		  start_init (yyval.ttype, yyvsp[-2].ttype, global_bindings_p ()); ;}
    break;

  case 291:
#line 1398 "objc/objc-parse.y"
    { finish_init ();
		  finish_decl (yyvsp[-1].ttype, yyvsp[0].ttype, yyvsp[-4].ttype); ;}
    break;

  case 292:
#line 1401 "objc/objc-parse.y"
    { tree d = start_decl (yyvsp[-2].ttype, current_declspecs, 0,
				       chainon (yyvsp[0].ttype, all_prefix_attributes));
		  finish_decl (d, NULL_TREE, yyvsp[-1].ttype); ;}
    break;

  case 293:
#line 1409 "objc/objc-parse.y"
    { yyval.ttype = NULL_TREE; ;}
    break;

  case 294:
#line 1411 "objc/objc-parse.y"
    { yyval.ttype = yyvsp[0].ttype; ;}
    break;

  case 295:
#line 1416 "objc/objc-parse.y"
    { yyval.ttype = yyvsp[0].ttype; ;}
    break;

  case 296:
#line 1418 "objc/objc-parse.y"
    { yyval.ttype = chainon (yyvsp[-1].ttype, yyvsp[0].ttype); ;}
    break;

  case 297:
#line 1423 "objc/objc-parse.y"
    { yyval.ttype = yyvsp[-2].ttype; ;}
    break;

  case 298:
#line 1428 "objc/objc-parse.y"
    { yyval.ttype = yyvsp[0].ttype; ;}
    break;

  case 299:
#line 1430 "objc/objc-parse.y"
    { yyval.ttype = chainon (yyvsp[-2].ttype, yyvsp[0].ttype); ;}
    break;

  case 300:
#line 1435 "objc/objc-parse.y"
    { yyval.ttype = NULL_TREE; ;}
    break;

  case 301:
#line 1437 "objc/objc-parse.y"
    { yyval.ttype = build_tree_list (yyvsp[0].ttype, NULL_TREE); ;}
    break;

  case 302:
#line 1439 "objc/objc-parse.y"
    { yyval.ttype = build_tree_list (yyvsp[-3].ttype, build_tree_list (NULL_TREE, yyvsp[-1].ttype)); ;}
    break;

  case 303:
#line 1441 "objc/objc-parse.y"
    { yyval.ttype = build_tree_list (yyvsp[-5].ttype, tree_cons (NULL_TREE, yyvsp[-3].ttype, yyvsp[-1].ttype)); ;}
    break;

  case 304:
#line 1443 "objc/objc-parse.y"
    { yyval.ttype = build_tree_list (yyvsp[-3].ttype, yyvsp[-1].ttype); ;}
    break;

  case 312:
#line 1466 "objc/objc-parse.y"
    { really_start_incremental_init (NULL_TREE); ;}
    break;

  case 313:
#line 1468 "objc/objc-parse.y"
    { yyval.ttype = pop_init_level (0); ;}
    break;

  case 314:
#line 1470 "objc/objc-parse.y"
    { yyval.ttype = error_mark_node; ;}
    break;

  case 315:
#line 1476 "objc/objc-parse.y"
    { if (pedantic)
		    pedwarn ("ISO C forbids empty initializer braces"); ;}
    break;

  case 319:
#line 1490 "objc/objc-parse.y"
    { if (pedantic && ! flag_isoc99)
		    pedwarn ("ISO C90 forbids specifying subobject to initialize"); ;}
    break;

  case 320:
#line 1493 "objc/objc-parse.y"
    { if (pedantic)
		    pedwarn ("obsolete use of designated initializer without `='"); ;}
    break;

  case 321:
#line 1496 "objc/objc-parse.y"
    { set_init_label (yyvsp[-1].ttype);
		  if (pedantic)
		    pedwarn ("obsolete use of designated initializer with `:'"); ;}
    break;

  case 322:
#line 1500 "objc/objc-parse.y"
    {;}
    break;

  case 324:
#line 1506 "objc/objc-parse.y"
    { push_init_level (0); ;}
    break;

  case 325:
#line 1508 "objc/objc-parse.y"
    { process_init_element (pop_init_level (0)); ;}
    break;

  case 326:
#line 1510 "objc/objc-parse.y"
    { process_init_element (yyvsp[0].ttype); ;}
    break;

  case 330:
#line 1521 "objc/objc-parse.y"
    { set_init_label (yyvsp[0].ttype); ;}
    break;

  case 331:
#line 1523 "objc/objc-parse.y"
    { set_init_index (yyvsp[-3].ttype, yyvsp[-1].ttype);
		  if (pedantic)
		    pedwarn ("ISO C forbids specifying range of elements to initialize"); ;}
    break;

  case 332:
#line 1527 "objc/objc-parse.y"
    { set_init_index (yyvsp[-1].ttype, NULL_TREE); ;}
    break;

  case 333:
#line 1532 "objc/objc-parse.y"
    { if (pedantic)
		    pedwarn ("ISO C forbids nested functions");

		  push_function_context ();
		  if (! start_function (current_declspecs, yyvsp[0].ttype,
					all_prefix_attributes))
		    {
		      pop_function_context ();
		      YYERROR1;
		    }
		  parsing_iso_function_signature = false; /* Don't warn about nested functions.  */
		;}
    break;

  case 334:
#line 1545 "objc/objc-parse.y"
    { tree decl = current_function_decl;
		  DECL_SOURCE_LOCATION (decl) = yyvsp[0].location;
		  store_parm_decls (); ;}
    break;

  case 335:
#line 1555 "objc/objc-parse.y"
    { tree decl = current_function_decl;
		  finish_function ();
		  pop_function_context ();
		  add_decl_stmt (decl); ;}
    break;

  case 336:
#line 1563 "objc/objc-parse.y"
    { if (pedantic)
		    pedwarn ("ISO C forbids nested functions");

		  push_function_context ();
		  if (! start_function (current_declspecs, yyvsp[0].ttype,
					all_prefix_attributes))
		    {
		      pop_function_context ();
		      YYERROR1;
		    }
		  parsing_iso_function_signature = false; /* Don't warn about nested functions.  */
		;}
    break;

  case 337:
#line 1576 "objc/objc-parse.y"
    { tree decl = current_function_decl;
		  DECL_SOURCE_LOCATION (decl) = yyvsp[0].location;
		  store_parm_decls (); ;}
    break;

  case 338:
#line 1586 "objc/objc-parse.y"
    { tree decl = current_function_decl;
		  finish_function ();
		  pop_function_context ();
		  add_decl_stmt (decl); ;}
    break;

  case 341:
#line 1604 "objc/objc-parse.y"
    { yyval.ttype = yyvsp[-2].ttype ? tree_cons (yyvsp[-2].ttype, yyvsp[-1].ttype, NULL_TREE) : yyvsp[-1].ttype; ;}
    break;

  case 342:
#line 1606 "objc/objc-parse.y"
    { yyval.ttype = build_nt (CALL_EXPR, yyvsp[-2].ttype, yyvsp[0].ttype, NULL_TREE); ;}
    break;

  case 343:
#line 1611 "objc/objc-parse.y"
    { yyval.ttype = set_array_declarator_type (yyvsp[0].ttype, yyvsp[-1].ttype, 0); ;}
    break;

  case 344:
#line 1613 "objc/objc-parse.y"
    { yyval.ttype = make_pointer_declarator (yyvsp[-1].ttype, yyvsp[0].ttype); ;}
    break;

  case 349:
#line 1629 "objc/objc-parse.y"
    { yyval.ttype = build_nt (CALL_EXPR, yyvsp[-2].ttype, yyvsp[0].ttype, NULL_TREE); ;}
    break;

  case 350:
#line 1634 "objc/objc-parse.y"
    { yyval.ttype = set_array_declarator_type (yyvsp[0].ttype, yyvsp[-1].ttype, 0); ;}
    break;

  case 353:
#line 1641 "objc/objc-parse.y"
    { yyval.ttype = build_nt (CALL_EXPR, yyvsp[-2].ttype, yyvsp[0].ttype, NULL_TREE); ;}
    break;

  case 354:
#line 1646 "objc/objc-parse.y"
    { yyval.ttype = set_array_declarator_type (yyvsp[0].ttype, yyvsp[-1].ttype, 0); ;}
    break;

  case 355:
#line 1648 "objc/objc-parse.y"
    { yyval.ttype = make_pointer_declarator (yyvsp[-1].ttype, yyvsp[0].ttype); ;}
    break;

  case 356:
#line 1650 "objc/objc-parse.y"
    { yyval.ttype = make_pointer_declarator (yyvsp[-1].ttype, yyvsp[0].ttype); ;}
    break;

  case 357:
#line 1652 "objc/objc-parse.y"
    { yyval.ttype = yyvsp[-2].ttype ? tree_cons (yyvsp[-2].ttype, yyvsp[-1].ttype, NULL_TREE) : yyvsp[-1].ttype; ;}
    break;

  case 358:
#line 1660 "objc/objc-parse.y"
    { yyval.ttype = build_nt (CALL_EXPR, yyvsp[-2].ttype, yyvsp[0].ttype, NULL_TREE); ;}
    break;

  case 359:
#line 1665 "objc/objc-parse.y"
    { yyval.ttype = yyvsp[-2].ttype ? tree_cons (yyvsp[-2].ttype, yyvsp[-1].ttype, NULL_TREE) : yyvsp[-1].ttype; ;}
    break;

  case 360:
#line 1667 "objc/objc-parse.y"
    { yyval.ttype = make_pointer_declarator (yyvsp[-1].ttype, yyvsp[0].ttype); ;}
    break;

  case 361:
#line 1669 "objc/objc-parse.y"
    { yyval.ttype = set_array_declarator_type (yyvsp[0].ttype, yyvsp[-1].ttype, 0); ;}
    break;

  case 363:
#line 1675 "objc/objc-parse.y"
    { yyval.ttype = NULL_TREE; ;}
    break;

  case 364:
#line 1677 "objc/objc-parse.y"
    { yyval.ttype = yyvsp[0].ttype; ;}
    break;

  case 365:
#line 1682 "objc/objc-parse.y"
    { yyval.ttype = NULL_TREE; ;}
    break;

  case 366:
#line 1684 "objc/objc-parse.y"
    { yyval.ttype = yyvsp[0].ttype; ;}
    break;

  case 367:
#line 1689 "objc/objc-parse.y"
    { yyval.ttype = NULL_TREE; ;}
    break;

  case 368:
#line 1691 "objc/objc-parse.y"
    { yyval.ttype = yyvsp[0].ttype; ;}
    break;

  case 369:
#line 1702 "objc/objc-parse.y"
    { yyval.ttype = start_struct (RECORD_TYPE, yyvsp[-1].ttype);
		  /* Start scope of tag before parsing components.  */
		;}
    break;

  case 370:
#line 1706 "objc/objc-parse.y"
    { yyval.ttype = finish_struct (yyvsp[-3].ttype, nreverse (yyvsp[-2].ttype),
				      chainon (yyvsp[-6].ttype, yyvsp[0].ttype)); ;}
    break;

  case 371:
#line 1709 "objc/objc-parse.y"
    { yyval.ttype = finish_struct (start_struct (RECORD_TYPE, NULL_TREE),
				      nreverse (yyvsp[-2].ttype), chainon (yyvsp[-4].ttype, yyvsp[0].ttype));
		;}
    break;

  case 372:
#line 1713 "objc/objc-parse.y"
    { yyval.ttype = start_struct (UNION_TYPE, yyvsp[-1].ttype); ;}
    break;

  case 373:
#line 1715 "objc/objc-parse.y"
    { yyval.ttype = finish_struct (yyvsp[-3].ttype, nreverse (yyvsp[-2].ttype),
				      chainon (yyvsp[-6].ttype, yyvsp[0].ttype)); ;}
    break;

  case 374:
#line 1718 "objc/objc-parse.y"
    { yyval.ttype = finish_struct (start_struct (UNION_TYPE, NULL_TREE),
				      nreverse (yyvsp[-2].ttype), chainon (yyvsp[-4].ttype, yyvsp[0].ttype));
		;}
    break;

  case 375:
#line 1722 "objc/objc-parse.y"
    { yyval.ttype = start_enum (yyvsp[-1].ttype); ;}
    break;

  case 376:
#line 1724 "objc/objc-parse.y"
    { yyval.ttype = finish_enum (yyvsp[-4].ttype, nreverse (yyvsp[-3].ttype),
				    chainon (yyvsp[-7].ttype, yyvsp[0].ttype)); ;}
    break;

  case 377:
#line 1727 "objc/objc-parse.y"
    { yyval.ttype = start_enum (NULL_TREE); ;}
    break;

  case 378:
#line 1729 "objc/objc-parse.y"
    { yyval.ttype = finish_enum (yyvsp[-4].ttype, nreverse (yyvsp[-3].ttype),
				    chainon (yyvsp[-6].ttype, yyvsp[0].ttype)); ;}
    break;

  case 379:
#line 1735 "objc/objc-parse.y"
    { yyval.ttype = xref_tag (RECORD_TYPE, yyvsp[0].ttype); ;}
    break;

  case 380:
#line 1737 "objc/objc-parse.y"
    { yyval.ttype = xref_tag (UNION_TYPE, yyvsp[0].ttype); ;}
    break;

  case 381:
#line 1739 "objc/objc-parse.y"
    { yyval.ttype = xref_tag (ENUMERAL_TYPE, yyvsp[0].ttype);
		  /* In ISO C, enumerated types can be referred to
		     only if already defined.  */
		  if (pedantic && !COMPLETE_TYPE_P (yyval.ttype))
		    pedwarn ("ISO C forbids forward references to `enum' types"); ;}
    break;

  case 385:
#line 1754 "objc/objc-parse.y"
    { if (pedantic && ! flag_isoc99)
		    pedwarn ("comma at end of enumerator list"); ;}
    break;

  case 386:
#line 1772 "objc/objc-parse.y"
    { yyval.ttype = yyvsp[0].ttype; ;}
    break;

  case 387:
#line 1774 "objc/objc-parse.y"
    { yyval.ttype = chainon (yyvsp[0].ttype, yyvsp[-1].ttype);
		  pedwarn ("no semicolon at end of struct or union"); ;}
    break;

  case 388:
#line 1779 "objc/objc-parse.y"
    { yyval.ttype = NULL_TREE; ;}
    break;

  case 389:
#line 1781 "objc/objc-parse.y"
    { yyval.ttype = chainon (yyvsp[-1].ttype, yyvsp[-2].ttype); ;}
    break;

  case 390:
#line 1783 "objc/objc-parse.y"
    { if (pedantic)
		    pedwarn ("extra semicolon in struct or union specified"); ;}
    break;

  case 391:
#line 1787 "objc/objc-parse.y"
    { yyval.ttype = nreverse (get_class_ivars_from_name (yyvsp[-1].ttype)); ;}
    break;

  case 392:
#line 1792 "objc/objc-parse.y"
    { yyval.ttype = yyvsp[0].ttype;
		  POP_DECLSPEC_STACK; ;}
    break;

  case 393:
#line 1795 "objc/objc-parse.y"
    {
		  /* Support for unnamed structs or unions as members of
		     structs or unions (which is [a] useful and [b] supports
		     MS P-SDK).  */
		  if (pedantic)
		    pedwarn ("ISO C doesn't support unnamed structs/unions");

		  yyval.ttype = grokfield(NULL, current_declspecs, NULL_TREE);
		  POP_DECLSPEC_STACK; ;}
    break;

  case 394:
#line 1805 "objc/objc-parse.y"
    { yyval.ttype = yyvsp[0].ttype;
		  POP_DECLSPEC_STACK; ;}
    break;

  case 395:
#line 1808 "objc/objc-parse.y"
    { if (pedantic)
		    pedwarn ("ISO C forbids member declarations with no members");
		  shadow_tag_warned (yyvsp[0].ttype, pedantic);
		  yyval.ttype = NULL_TREE; ;}
    break;

  case 396:
#line 1813 "objc/objc-parse.y"
    { yyval.ttype = NULL_TREE; ;}
    break;

  case 397:
#line 1815 "objc/objc-parse.y"
    { yyval.ttype = yyvsp[0].ttype;
		  RESTORE_EXT_FLAGS (yyvsp[-1].itype); ;}
    break;

  case 399:
#line 1822 "objc/objc-parse.y"
    { TREE_CHAIN (yyvsp[0].ttype) = yyvsp[-3].ttype; yyval.ttype = yyvsp[0].ttype; ;}
    break;

  case 401:
#line 1828 "objc/objc-parse.y"
    { TREE_CHAIN (yyvsp[0].ttype) = yyvsp[-3].ttype; yyval.ttype = yyvsp[0].ttype; ;}
    break;

  case 402:
#line 1833 "objc/objc-parse.y"
    { yyval.ttype = grokfield (yyvsp[-1].ttype, current_declspecs, NULL_TREE);
		  decl_attributes (&yyval.ttype,
				   chainon (yyvsp[0].ttype, all_prefix_attributes), 0); ;}
    break;

  case 403:
#line 1837 "objc/objc-parse.y"
    { yyval.ttype = grokfield (yyvsp[-3].ttype, current_declspecs, yyvsp[-1].ttype);
		  decl_attributes (&yyval.ttype,
				   chainon (yyvsp[0].ttype, all_prefix_attributes), 0); ;}
    break;

  case 404:
#line 1841 "objc/objc-parse.y"
    { yyval.ttype = grokfield (NULL_TREE, current_declspecs, yyvsp[-1].ttype);
		  decl_attributes (&yyval.ttype,
				   chainon (yyvsp[0].ttype, all_prefix_attributes), 0); ;}
    break;

  case 405:
#line 1848 "objc/objc-parse.y"
    { yyval.ttype = grokfield (yyvsp[-1].ttype, current_declspecs, NULL_TREE);
		  decl_attributes (&yyval.ttype,
				   chainon (yyvsp[0].ttype, all_prefix_attributes), 0); ;}
    break;

  case 406:
#line 1852 "objc/objc-parse.y"
    { yyval.ttype = grokfield (yyvsp[-3].ttype, current_declspecs, yyvsp[-1].ttype);
		  decl_attributes (&yyval.ttype,
				   chainon (yyvsp[0].ttype, all_prefix_attributes), 0); ;}
    break;

  case 407:
#line 1856 "objc/objc-parse.y"
    { yyval.ttype = grokfield (NULL_TREE, current_declspecs, yyvsp[-1].ttype);
		  decl_attributes (&yyval.ttype,
				   chainon (yyvsp[0].ttype, all_prefix_attributes), 0); ;}
    break;

  case 409:
#line 1867 "objc/objc-parse.y"
    { if (yyvsp[-2].ttype == error_mark_node)
		    yyval.ttype = yyvsp[-2].ttype;
		  else
		    TREE_CHAIN (yyvsp[0].ttype) = yyvsp[-2].ttype, yyval.ttype = yyvsp[0].ttype; ;}
    break;

  case 410:
#line 1872 "objc/objc-parse.y"
    { yyval.ttype = error_mark_node; ;}
    break;

  case 411:
#line 1878 "objc/objc-parse.y"
    { yyval.ttype = build_enumerator (yyvsp[0].ttype, NULL_TREE); ;}
    break;

  case 412:
#line 1880 "objc/objc-parse.y"
    { yyval.ttype = build_enumerator (yyvsp[-2].ttype, yyvsp[0].ttype); ;}
    break;

  case 413:
#line 1885 "objc/objc-parse.y"
    { pending_xref_error ();
		  yyval.ttype = yyvsp[0].ttype; ;}
    break;

  case 414:
#line 1888 "objc/objc-parse.y"
    { yyval.ttype = build_tree_list (yyvsp[-1].ttype, yyvsp[0].ttype); ;}
    break;

  case 415:
#line 1893 "objc/objc-parse.y"
    { yyval.ttype = NULL_TREE; ;}
    break;

  case 417:
#line 1899 "objc/objc-parse.y"
    { yyval.ttype = build_tree_list (build_tree_list (current_declspecs,
							 NULL_TREE),
					all_prefix_attributes); ;}
    break;

  case 418:
#line 1903 "objc/objc-parse.y"
    { yyval.ttype = build_tree_list (build_tree_list (current_declspecs,
							 yyvsp[0].ttype),
					all_prefix_attributes); ;}
    break;

  case 419:
#line 1907 "objc/objc-parse.y"
    { yyval.ttype = build_tree_list (build_tree_list (current_declspecs,
							 yyvsp[-1].ttype),
					chainon (yyvsp[0].ttype, all_prefix_attributes)); ;}
    break;

  case 423:
#line 1920 "objc/objc-parse.y"
    { yyval.ttype = make_pointer_declarator (yyvsp[-1].ttype, yyvsp[0].ttype); ;}
    break;

  case 424:
#line 1925 "objc/objc-parse.y"
    { yyval.ttype = make_pointer_declarator (yyvsp[0].ttype, NULL_TREE); ;}
    break;

  case 425:
#line 1927 "objc/objc-parse.y"
    { yyval.ttype = make_pointer_declarator (yyvsp[-1].ttype, yyvsp[0].ttype); ;}
    break;

  case 426:
#line 1932 "objc/objc-parse.y"
    { yyval.ttype = yyvsp[-2].ttype ? tree_cons (yyvsp[-2].ttype, yyvsp[-1].ttype, NULL_TREE) : yyvsp[-1].ttype; ;}
    break;

  case 427:
#line 1934 "objc/objc-parse.y"
    { yyval.ttype = build_nt (CALL_EXPR, yyvsp[-2].ttype, yyvsp[0].ttype, NULL_TREE); ;}
    break;

  case 428:
#line 1936 "objc/objc-parse.y"
    { yyval.ttype = set_array_declarator_type (yyvsp[0].ttype, yyvsp[-1].ttype, 1); ;}
    break;

  case 429:
#line 1938 "objc/objc-parse.y"
    { yyval.ttype = build_nt (CALL_EXPR, NULL_TREE, yyvsp[0].ttype, NULL_TREE); ;}
    break;

  case 430:
#line 1940 "objc/objc-parse.y"
    { yyval.ttype = set_array_declarator_type (yyvsp[0].ttype, NULL_TREE, 1); ;}
    break;

  case 431:
#line 1947 "objc/objc-parse.y"
    { yyval.ttype = build_array_declarator (yyvsp[-1].ttype, yyvsp[-2].ttype, 0, 0); ;}
    break;

  case 432:
#line 1949 "objc/objc-parse.y"
    { yyval.ttype = build_array_declarator (NULL_TREE, yyvsp[-1].ttype, 0, 0); ;}
    break;

  case 433:
#line 1951 "objc/objc-parse.y"
    { yyval.ttype = build_array_declarator (NULL_TREE, yyvsp[-2].ttype, 0, 1); ;}
    break;

  case 434:
#line 1953 "objc/objc-parse.y"
    { yyval.ttype = build_array_declarator (yyvsp[-1].ttype, yyvsp[-2].ttype, 1, 0); ;}
    break;

  case 435:
#line 1956 "objc/objc-parse.y"
    { yyval.ttype = build_array_declarator (yyvsp[-1].ttype, yyvsp[-3].ttype, 1, 0); ;}
    break;

  case 438:
#line 1969 "objc/objc-parse.y"
    {
		  error ("label at end of compound statement");
		;}
    break;

  case 446:
#line 1986 "objc/objc-parse.y"
    {
		  if ((pedantic && !flag_isoc99)
		      || warn_declaration_after_statement)
		    pedwarn_c90 ("ISO C90 forbids mixed declarations and code");
		;}
    break;

  case 461:
#line 2019 "objc/objc-parse.y"
    { pushlevel (0);
		  clear_last_expr ();
		  add_scope_stmt (/*begin_p=*/1, /*partial_p=*/0);
		;}
    break;

  case 462:
#line 2026 "objc/objc-parse.y"
    {
		  if (c_dialect_objc ())
		    objc_clear_super_receiver ();
		  yyval.ttype = add_scope_stmt (/*begin_p=*/0, /*partial_p=*/0);
		;}
    break;

  case 463:
#line 2035 "objc/objc-parse.y"
    { if (flag_isoc99)
		    {
		      yyval.ttype = c_begin_compound_stmt ();
		      pushlevel (0);
		      clear_last_expr ();
		      add_scope_stmt (/*begin_p=*/1, /*partial_p=*/0);
		    }
		  else
		    yyval.ttype = NULL_TREE;
		;}
    break;

  case 464:
#line 2051 "objc/objc-parse.y"
    { if (flag_isoc99)
		    {
		      tree scope_stmt = add_scope_stmt (/*begin_p=*/0, /*partial_p=*/0);
		      yyval.ttype = poplevel (KEEP_MAYBE, 0, 0);
		      SCOPE_STMT_BLOCK (TREE_PURPOSE (scope_stmt))
			= SCOPE_STMT_BLOCK (TREE_VALUE (scope_stmt))
			= yyval.ttype;
		    }
		  else
		    yyval.ttype = NULL_TREE; ;}
    break;

  case 466:
#line 2068 "objc/objc-parse.y"
    { if (pedantic)
		    pedwarn ("ISO C forbids label declarations"); ;}
    break;

  case 469:
#line 2079 "objc/objc-parse.y"
    { tree link;
		  for (link = yyvsp[-1].ttype; link; link = TREE_CHAIN (link))
		    {
		      tree label = declare_label (TREE_VALUE (link));
		      C_DECLARED_LABEL_FLAG (label) = 1;
		      add_decl_stmt (label);
		    }
		;}
    break;

  case 470:
#line 2093 "objc/objc-parse.y"
    {;}
    break;

  case 472:
#line 2097 "objc/objc-parse.y"
    { compstmt_count++;
                      yyval.ttype = c_begin_compound_stmt (); ;}
    break;

  case 473:
#line 2102 "objc/objc-parse.y"
    { yyval.ttype = convert (void_type_node, integer_zero_node); ;}
    break;

  case 474:
#line 2104 "objc/objc-parse.y"
    { yyval.ttype = poplevel (KEEP_MAYBE, 0, 0);
		  SCOPE_STMT_BLOCK (TREE_PURPOSE (yyvsp[0].ttype))
		    = SCOPE_STMT_BLOCK (TREE_VALUE (yyvsp[0].ttype))
		    = yyval.ttype; ;}
    break;

  case 477:
#line 2117 "objc/objc-parse.y"
    { if (last_tree == NULL)
		    {
		      error ("braced-group within expression allowed only inside a function");
		      YYERROR;
		    }
		  /* We must force a BLOCK for this level
		     so that, if it is not expanded later,
		     there is a way to turn off the entire subtree of blocks
		     that are contained in it.  */
		  keep_next_level ();
		  compstmt_count++;
		  yyval.ttype = add_stmt (build_stmt (COMPOUND_STMT, last_tree));
		  last_expr_type = NULL_TREE;
		;}
    break;

  case 478:
#line 2134 "objc/objc-parse.y"
    { RECHAIN_STMTS (yyvsp[-1].ttype, COMPOUND_BODY (yyvsp[-1].ttype));
		  last_expr_type = NULL_TREE;
                  yyval.ttype = yyvsp[-1].ttype; ;}
    break;

  case 479:
#line 2142 "objc/objc-parse.y"
    { c_finish_then (); ;}
    break;

  case 481:
#line 2159 "objc/objc-parse.y"
    { yyval.ttype = c_begin_if_stmt (); ;}
    break;

  case 482:
#line 2161 "objc/objc-parse.y"
    { c_expand_start_cond (c_common_truthvalue_conversion (yyvsp[-1].ttype),
				       compstmt_count,yyvsp[-3].ttype);
		  yyval.itype = stmt_count;
		  if_stmt_locus = yyvsp[-6].location; ;}
    break;

  case 483:
#line 2172 "objc/objc-parse.y"
    { stmt_count++;
		  compstmt_count++;
		  c_in_iteration_stmt++;
		  yyval.ttype
		    = add_stmt (build_stmt (DO_STMT, NULL_TREE,
					    NULL_TREE));
		  /* In the event that a parse error prevents
		     parsing the complete do-statement, set the
		     condition now.  Otherwise, we can get crashes at
		     RTL-generation time.  */
		  DO_COND (yyval.ttype) = error_mark_node; ;}
    break;

  case 484:
#line 2184 "objc/objc-parse.y"
    { yyval.ttype = yyvsp[-2].ttype;
		  RECHAIN_STMTS (yyval.ttype, DO_BODY (yyval.ttype));
		  c_in_iteration_stmt--; ;}
    break;

  case 485:
#line 2194 "objc/objc-parse.y"
    { if (yychar == YYEMPTY)
		    yychar = YYLEX;
		  yyval.location = input_location; ;}
    break;

  case 488:
#line 2207 "objc/objc-parse.y"
    { if (flag_isoc99)
		    RECHAIN_STMTS (yyvsp[-2].ttype, COMPOUND_BODY (yyvsp[-2].ttype)); ;}
    break;

  case 489:
#line 2213 "objc/objc-parse.y"
    { if (yyvsp[0].ttype)
		    {
		      STMT_LINENO (yyvsp[0].ttype) = yyvsp[-1].location.line;
		      /* ??? We currently have no way of recording
			 the filename for a statement.  This probably
			 matters little in practice at the moment,
			 but I suspect that problems will occur when
			 doing inlining at the tree level.  */
		    }
		;}
    break;

  case 490:
#line 2227 "objc/objc-parse.y"
    { if (yyvsp[0].ttype)
		    {
		      STMT_LINENO (yyvsp[0].ttype) = yyvsp[-1].location.line;
		    }
		;}
    break;

  case 491:
#line 2236 "objc/objc-parse.y"
    { c_expand_start_else ();
		  yyvsp[-1].itype = stmt_count; ;}
    break;

  case 492:
#line 2239 "objc/objc-parse.y"
    { c_finish_else ();
		  c_expand_end_cond ();
		  if (extra_warnings && stmt_count == yyvsp[-3].itype)
		    warning ("empty body in an else-statement"); ;}
    break;

  case 493:
#line 2244 "objc/objc-parse.y"
    { c_expand_end_cond ();
		  /* This warning is here instead of in simple_if, because we
		     do not want a warning if an empty if is followed by an
		     else statement.  Increment stmt_count so we don't
		     give a second error if this is a nested `if'.  */
		  if (extra_warnings && stmt_count++ == yyvsp[0].itype)
		    warning ("%Hempty body in an if-statement",
                             &if_stmt_locus); ;}
    break;

  case 494:
#line 2256 "objc/objc-parse.y"
    { c_expand_end_cond (); ;}
    break;

  case 495:
#line 2266 "objc/objc-parse.y"
    { stmt_count++;
		  yyval.ttype = c_begin_while_stmt (); ;}
    break;

  case 496:
#line 2269 "objc/objc-parse.y"
    { c_in_iteration_stmt++;
		  yyvsp[-1].ttype = c_common_truthvalue_conversion (yyvsp[-1].ttype);
		  c_finish_while_stmt_cond
		    (c_common_truthvalue_conversion (yyvsp[-1].ttype), yyvsp[-3].ttype);
		  yyval.ttype = add_stmt (yyvsp[-3].ttype); ;}
    break;

  case 497:
#line 2275 "objc/objc-parse.y"
    { c_in_iteration_stmt--;
		  RECHAIN_STMTS (yyvsp[-1].ttype, WHILE_BODY (yyvsp[-1].ttype)); ;}
    break;

  case 498:
#line 2279 "objc/objc-parse.y"
    { DO_COND (yyvsp[-4].ttype) = c_common_truthvalue_conversion (yyvsp[-2].ttype); ;}
    break;

  case 499:
#line 2281 "objc/objc-parse.y"
    { ;}
    break;

  case 500:
#line 2283 "objc/objc-parse.y"
    { yyval.ttype = build_stmt (FOR_STMT, NULL_TREE, NULL_TREE,
					  NULL_TREE, NULL_TREE);
		  add_stmt (yyval.ttype); ;}
    break;

  case 501:
#line 2287 "objc/objc-parse.y"
    { stmt_count++;
		  RECHAIN_STMTS (yyvsp[-2].ttype, FOR_INIT_STMT (yyvsp[-2].ttype)); ;}
    break;

  case 502:
#line 2290 "objc/objc-parse.y"
    { if (yyvsp[-1].ttype)
		    FOR_COND (yyvsp[-5].ttype)
		      = c_common_truthvalue_conversion (yyvsp[-1].ttype); ;}
    break;

  case 503:
#line 2294 "objc/objc-parse.y"
    { c_in_iteration_stmt++;
		  FOR_EXPR (yyvsp[-8].ttype) = yyvsp[-1].ttype; ;}
    break;

  case 504:
#line 2297 "objc/objc-parse.y"
    { RECHAIN_STMTS (yyvsp[-10].ttype, FOR_BODY (yyvsp[-10].ttype));
		  c_in_iteration_stmt--;;}
    break;

  case 505:
#line 2300 "objc/objc-parse.y"
    { stmt_count++;
		  yyval.ttype = c_start_case (yyvsp[-1].ttype);
		  c_in_case_stmt++; ;}
    break;

  case 506:
#line 2304 "objc/objc-parse.y"
    { c_finish_case ();
		  c_in_case_stmt--; ;}
    break;

  case 507:
#line 2310 "objc/objc-parse.y"
    { add_stmt (build_stmt (EXPR_STMT, yyvsp[-1].ttype)); ;}
    break;

  case 508:
#line 2312 "objc/objc-parse.y"
    { check_for_loop_decls (); ;}
    break;

  case 509:
#line 2318 "objc/objc-parse.y"
    { stmt_count++; yyval.ttype = yyvsp[0].ttype; ;}
    break;

  case 510:
#line 2320 "objc/objc-parse.y"
    { stmt_count++;
		  yyval.ttype = c_expand_expr_stmt (yyvsp[-1].ttype); ;}
    break;

  case 511:
#line 2323 "objc/objc-parse.y"
    { if (flag_isoc99)
		    RECHAIN_STMTS (yyvsp[-2].ttype, COMPOUND_BODY (yyvsp[-2].ttype));
		  yyval.ttype = NULL_TREE; ;}
    break;

  case 512:
#line 2327 "objc/objc-parse.y"
    { stmt_count++;
		if (!(c_in_iteration_stmt || c_in_case_stmt))
		  {
		    error ("break statement not within loop or switch");
		    yyval.ttype = NULL_TREE;
		  }
		else
		  yyval.ttype = add_stmt (build_break_stmt ()); ;}
    break;

  case 513:
#line 2336 "objc/objc-parse.y"
    { stmt_count++;
		if (!c_in_iteration_stmt)
		  {
		    error ("continue statement not within a loop");
		    yyval.ttype = NULL_TREE;
		  }
		else
		  yyval.ttype = add_stmt (build_continue_stmt ()); ;}
    break;

  case 514:
#line 2345 "objc/objc-parse.y"
    { stmt_count++;
		  yyval.ttype = c_expand_return (NULL_TREE); ;}
    break;

  case 515:
#line 2348 "objc/objc-parse.y"
    { stmt_count++;
		  yyval.ttype = c_expand_return (yyvsp[-1].ttype); ;}
    break;

  case 516:
#line 2351 "objc/objc-parse.y"
    { stmt_count++;
		  yyval.ttype = simple_asm_stmt (yyvsp[-2].ttype); ;}
    break;

  case 517:
#line 2355 "objc/objc-parse.y"
    { stmt_count++;
		  yyval.ttype = build_asm_stmt (yyvsp[-6].ttype, yyvsp[-4].ttype, yyvsp[-2].ttype, NULL_TREE, NULL_TREE); ;}
    break;

  case 518:
#line 2360 "objc/objc-parse.y"
    { stmt_count++;
		  yyval.ttype = build_asm_stmt (yyvsp[-8].ttype, yyvsp[-6].ttype, yyvsp[-4].ttype, yyvsp[-2].ttype, NULL_TREE); ;}
    break;

  case 519:
#line 2365 "objc/objc-parse.y"
    { stmt_count++;
		  yyval.ttype = build_asm_stmt (yyvsp[-10].ttype, yyvsp[-8].ttype, yyvsp[-6].ttype, yyvsp[-4].ttype, yyvsp[-2].ttype); ;}
    break;

  case 520:
#line 2368 "objc/objc-parse.y"
    { tree decl;
		  stmt_count++;
		  decl = lookup_label (yyvsp[-1].ttype);
		  if (decl != 0)
		    {
		      TREE_USED (decl) = 1;
		      yyval.ttype = add_stmt (build_stmt (GOTO_STMT, decl));
		    }
		  else
		    yyval.ttype = NULL_TREE;
		;}
    break;

  case 521:
#line 2380 "objc/objc-parse.y"
    { if (pedantic)
		    pedwarn ("ISO C forbids `goto *expr;'");
		  stmt_count++;
		  yyvsp[-1].ttype = convert (ptr_type_node, yyvsp[-1].ttype);
		  yyval.ttype = add_stmt (build_stmt (GOTO_STMT, yyvsp[-1].ttype)); ;}
    break;

  case 522:
#line 2386 "objc/objc-parse.y"
    { yyval.ttype = NULL_TREE; ;}
    break;

  case 523:
#line 2388 "objc/objc-parse.y"
    { stmt_count++;
		  yyval.ttype = objc_build_throw_stmt (yyvsp[-1].ttype);
		;}
    break;

  case 524:
#line 2392 "objc/objc-parse.y"
    { stmt_count++;
		  yyval.ttype = objc_build_throw_stmt (NULL_TREE);
		;}
    break;

  case 525:
#line 2396 "objc/objc-parse.y"
    { objc_build_finally_prologue (); ;}
    break;

  case 526:
#line 2398 "objc/objc-parse.y"
    { yyval.ttype = objc_build_try_catch_finally_stmt (yyvsp[-2].itype, yyvsp[0].itype); ;}
    break;

  case 527:
#line 2400 "objc/objc-parse.y"
    { objc_build_synchronized_prologue (yyvsp[-1].ttype); ;}
    break;

  case 528:
#line 2402 "objc/objc-parse.y"
    { yyval.ttype = objc_build_synchronized_epilogue (); ;}
    break;

  case 529:
#line 2407 "objc/objc-parse.y"
    { objc_build_try_epilogue (1); ;}
    break;

  case 530:
#line 2409 "objc/objc-parse.y"
    { objc_build_catch_epilogue (); yyval.itype = 1; ;}
    break;

  case 531:
#line 2411 "objc/objc-parse.y"
    { objc_build_try_epilogue (0); yyval.itype = 0; ;}
    break;

  case 532:
#line 2417 "objc/objc-parse.y"
    { objc_build_try_prologue (); ;}
    break;

  case 536:
#line 2428 "objc/objc-parse.y"
    { objc_build_catch_stmt (yyvsp[-1].ttype); ;}
    break;

  case 537:
#line 2430 "objc/objc-parse.y"
    { stmt_count++; ;}
    break;

  case 538:
#line 2435 "objc/objc-parse.y"
    { yyval.itype = 1; ;}
    break;

  case 539:
#line 2437 "objc/objc-parse.y"
    { yyval.itype = 0; ;}
    break;

  case 540:
#line 2445 "objc/objc-parse.y"
    { stmt_count++;
		  yyval.ttype = do_case (yyvsp[-1].ttype, NULL_TREE); ;}
    break;

  case 541:
#line 2448 "objc/objc-parse.y"
    { stmt_count++;
		  yyval.ttype = do_case (yyvsp[-3].ttype, yyvsp[-1].ttype); ;}
    break;

  case 542:
#line 2451 "objc/objc-parse.y"
    { stmt_count++;
		  yyval.ttype = do_case (NULL_TREE, NULL_TREE); ;}
    break;

  case 543:
#line 2454 "objc/objc-parse.y"
    { tree label = define_label (yyvsp[-2].location, yyvsp[-3].ttype);
		  stmt_count++;
		  if (label)
		    {
		      decl_attributes (&label, yyvsp[0].ttype, 0);
		      yyval.ttype = add_stmt (build_stmt (LABEL_STMT, label));
		    }
		  else
		    yyval.ttype = NULL_TREE;
		;}
    break;

  case 544:
#line 2470 "objc/objc-parse.y"
    { yyval.ttype = NULL_TREE; ;}
    break;

  case 545:
#line 2472 "objc/objc-parse.y"
    { ;}
    break;

  case 546:
#line 2477 "objc/objc-parse.y"
    { yyval.ttype = NULL_TREE; ;}
    break;

  case 548:
#line 2484 "objc/objc-parse.y"
    { yyval.ttype = NULL_TREE; ;}
    break;

  case 551:
#line 2491 "objc/objc-parse.y"
    { yyval.ttype = chainon (yyvsp[-2].ttype, yyvsp[0].ttype); ;}
    break;

  case 552:
#line 2496 "objc/objc-parse.y"
    { yyval.ttype = build_tree_list (build_tree_list (NULL_TREE, yyvsp[-3].ttype), yyvsp[-1].ttype); ;}
    break;

  case 553:
#line 2498 "objc/objc-parse.y"
    { yyvsp[-5].ttype = build_string (IDENTIFIER_LENGTH (yyvsp[-5].ttype),
				     IDENTIFIER_POINTER (yyvsp[-5].ttype));
		  yyval.ttype = build_tree_list (build_tree_list (yyvsp[-5].ttype, yyvsp[-3].ttype), yyvsp[-1].ttype); ;}
    break;

  case 554:
#line 2505 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, NULL_TREE); ;}
    break;

  case 555:
#line 2507 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, yyvsp[0].ttype, yyvsp[-2].ttype); ;}
    break;

  case 556:
#line 2517 "objc/objc-parse.y"
    { pushlevel (0);
		  declare_parm_level (); ;}
    break;

  case 557:
#line 2520 "objc/objc-parse.y"
    { yyval.ttype = yyvsp[0].ttype;
		  poplevel (0, 0, 0); ;}
    break;

  case 559:
#line 2527 "objc/objc-parse.y"
    { mark_forward_parm_decls (); ;}
    break;

  case 560:
#line 2529 "objc/objc-parse.y"
    { /* Dummy action so attributes are in known place
		     on parser stack.  */ ;}
    break;

  case 561:
#line 2532 "objc/objc-parse.y"
    { yyval.ttype = yyvsp[0].ttype; ;}
    break;

  case 562:
#line 2534 "objc/objc-parse.y"
    { yyval.ttype = tree_cons (NULL_TREE, NULL_TREE, NULL_TREE); ;}
    break;

  case 563:
#line 2540 "objc/objc-parse.y"
    { yyval.ttype = get_parm_info (0); ;}
    break;

  case 564:
#line 2542 "objc/objc-parse.y"
    { yyval.ttype = get_parm_info (0);
		  /* Gcc used to allow this as an extension.  However, it does
		     not work for all targets, and thus has been disabled.
		     Also, since func (...) and func () are indistinguishable,
		     it caused problems with the code in expand_builtin which
		     tries to verify that BUILT_IN_NEXT_ARG is being used
		     correctly.  */
		  error ("ISO C requires a named argument before `...'");
		  parsing_iso_function_signature = true;
		;}
    break;

  case 565:
#line 2553 "objc/objc-parse.y"
    { yyval.ttype = get_parm_info (1);
		  parsing_iso_function_signature = true;
		;}
    break;

  case 566:
#line 2557 "objc/objc-parse.y"
    { yyval.ttype = get_parm_info (0);
		  parsing_iso_function_signature = true;
		;}
    break;

  case 567:
#line 2564 "objc/objc-parse.y"
    { push_parm_decl (yyvsp[0].ttype); ;}
    break;

  case 568:
#line 2566 "objc/objc-parse.y"
    { push_parm_decl (yyvsp[0].ttype); ;}
    break;

  case 569:
#line 2573 "objc/objc-parse.y"
    { yyval.ttype = build_tree_list (build_tree_list (current_declspecs,
							 yyvsp[-1].ttype),
					chainon (yyvsp[0].ttype, all_prefix_attributes));
		  POP_DECLSPEC_STACK; ;}
    break;

  case 570:
#line 2578 "objc/objc-parse.y"
    { yyval.ttype = build_tree_list (build_tree_list (current_declspecs,
							 yyvsp[-1].ttype),
					chainon (yyvsp[0].ttype, all_prefix_attributes));
		  POP_DECLSPEC_STACK; ;}
    break;

  case 571:
#line 2583 "objc/objc-parse.y"
    { yyval.ttype = yyvsp[0].ttype;
		  POP_DECLSPEC_STACK; ;}
    break;

  case 572:
#line 2586 "objc/objc-parse.y"
    { yyval.ttype = build_tree_list (build_tree_list (current_declspecs,
							 yyvsp[-1].ttype),
					chainon (yyvsp[0].ttype, all_prefix_attributes));
		  POP_DECLSPEC_STACK; ;}
    break;

  case 573:
#line 2592 "objc/objc-parse.y"
    { yyval.ttype = yyvsp[0].ttype;
		  POP_DECLSPEC_STACK; ;}
    break;

  case 574:
#line 2600 "objc/objc-parse.y"
    { yyval.ttype = build_tree_list (build_tree_list (current_declspecs,
							 yyvsp[-1].ttype),
					chainon (yyvsp[0].ttype, all_prefix_attributes));
		  POP_DECLSPEC_STACK; ;}
    break;

  case 575:
#line 2605 "objc/objc-parse.y"
    { yyval.ttype = build_tree_list (build_tree_list (current_declspecs,
							 yyvsp[-1].ttype),
					chainon (yyvsp[0].ttype, all_prefix_attributes));
		  POP_DECLSPEC_STACK; ;}
    break;

  case 576:
#line 2610 "objc/objc-parse.y"
    { yyval.ttype = yyvsp[0].ttype;
		  POP_DECLSPEC_STACK; ;}
    break;

  case 577:
#line 2613 "objc/objc-parse.y"
    { yyval.ttype = build_tree_list (build_tree_list (current_declspecs,
							 yyvsp[-1].ttype),
					chainon (yyvsp[0].ttype, all_prefix_attributes));
		  POP_DECLSPEC_STACK; ;}
    break;

  case 578:
#line 2619 "objc/objc-parse.y"
    { yyval.ttype = yyvsp[0].ttype;
		  POP_DECLSPEC_STACK; ;}
    break;

  case 579:
#line 2625 "objc/objc-parse.y"
    { prefix_attributes = chainon (prefix_attributes, yyvsp[-3].ttype);
		  all_prefix_attributes = prefix_attributes; ;}
    break;

  case 580:
#line 2634 "objc/objc-parse.y"
    { pushlevel (0);
		  declare_parm_level (); ;}
    break;

  case 581:
#line 2637 "objc/objc-parse.y"
    { yyval.ttype = yyvsp[0].ttype;
		  poplevel (0, 0, 0); ;}
    break;

  case 583:
#line 2644 "objc/objc-parse.y"
    { tree t;
		  for (t = yyvsp[-1].ttype; t; t = TREE_CHAIN (t))
		    if (TREE_VALUE (t) == NULL_TREE)
		      error ("`...' in old-style identifier list");
		  yyval.ttype = tree_cons (NULL_TREE, NULL_TREE, yyvsp[-1].ttype);

		  /* Make sure we have a parmlist after attributes.  */
		  if (yyvsp[-3].ttype != 0
		      && (TREE_CODE (yyval.ttype) != TREE_LIST
			  || TREE_PURPOSE (yyval.ttype) == 0
			  || TREE_CODE (TREE_PURPOSE (yyval.ttype)) != PARM_DECL))
		    YYERROR1;
		;}
    break;

  case 584:
#line 2662 "objc/objc-parse.y"
    { yyval.ttype = build_tree_list (NULL_TREE, yyvsp[0].ttype); ;}
    break;

  case 585:
#line 2664 "objc/objc-parse.y"
    { yyval.ttype = chainon (yyvsp[-2].ttype, build_tree_list (NULL_TREE, yyvsp[0].ttype)); ;}
    break;

  case 586:
#line 2670 "objc/objc-parse.y"
    { yyval.ttype = build_tree_list (NULL_TREE, yyvsp[0].ttype); ;}
    break;

  case 587:
#line 2672 "objc/objc-parse.y"
    { yyval.ttype = chainon (yyvsp[-2].ttype, build_tree_list (NULL_TREE, yyvsp[0].ttype)); ;}
    break;

  case 588:
#line 2677 "objc/objc-parse.y"
    { yyval.itype = SAVE_EXT_FLAGS();
		  pedantic = 0;
		  warn_pointer_arith = 0;
		  warn_traditional = 0;
		  flag_iso = 0; ;}
    break;

  case 594:
#line 2693 "objc/objc-parse.y"
    {
		  if (objc_implementation_context)
                    {
		      finish_class (objc_implementation_context);
		      objc_ivar_chain = NULL_TREE;
		      objc_implementation_context = NULL_TREE;
		    }
		  else
		    warning ("`@end' must appear in an implementation context");
		;}
    break;

  case 595:
#line 2708 "objc/objc-parse.y"
    { yyval.ttype = build_tree_list (NULL_TREE, yyvsp[0].ttype); ;}
    break;

  case 596:
#line 2710 "objc/objc-parse.y"
    { yyval.ttype = chainon (yyvsp[-2].ttype, build_tree_list (NULL_TREE, yyvsp[0].ttype)); ;}
    break;

  case 597:
#line 2715 "objc/objc-parse.y"
    {
		  objc_declare_class (yyvsp[-1].ttype);
		;}
    break;

  case 598:
#line 2722 "objc/objc-parse.y"
    {
		  objc_declare_alias (yyvsp[-2].ttype, yyvsp[-1].ttype);
		;}
    break;

  case 599:
#line 2728 "objc/objc-parse.y"
    { yyval.ttype = yyvsp[0].ttype; ;}
    break;

  case 600:
#line 2729 "objc/objc-parse.y"
    { yyval.ttype = NULL_TREE; ;}
    break;

  case 603:
#line 2739 "objc/objc-parse.y"
    {
		  objc_interface_context = objc_ivar_context
		    = start_class (CLASS_INTERFACE_TYPE, yyvsp[-2].ttype, yyvsp[-1].ttype, yyvsp[0].ttype);
                  objc_public_flag = 0;
		;}
    break;

  case 604:
#line 2745 "objc/objc-parse.y"
    {
                  continue_class (objc_interface_context);
		;}
    break;

  case 605:
#line 2749 "objc/objc-parse.y"
    {
		  finish_class (objc_interface_context);
		  objc_interface_context = NULL_TREE;
		;}
    break;

  case 606:
#line 2755 "objc/objc-parse.y"
    {
		  objc_implementation_context = objc_ivar_context
		    = start_class (CLASS_IMPLEMENTATION_TYPE, yyvsp[-1].ttype, yyvsp[0].ttype, NULL_TREE);
                  objc_public_flag = 0;
		;}
    break;

  case 607:
#line 2761 "objc/objc-parse.y"
    {
                  objc_ivar_chain
		    = continue_class (objc_implementation_context);
		;}
    break;

  case 608:
#line 2767 "objc/objc-parse.y"
    {
		  objc_interface_context
		    = start_class (CATEGORY_INTERFACE_TYPE, yyvsp[-4].ttype, yyvsp[-2].ttype, yyvsp[0].ttype);
                  continue_class (objc_interface_context);
		;}
    break;

  case 609:
#line 2773 "objc/objc-parse.y"
    {
		  finish_class (objc_interface_context);
		  objc_interface_context = NULL_TREE;
		;}
    break;

  case 610:
#line 2779 "objc/objc-parse.y"
    {
		  objc_implementation_context
		    = start_class (CATEGORY_IMPLEMENTATION_TYPE, yyvsp[-3].ttype, yyvsp[-1].ttype, NULL_TREE);
                  objc_ivar_chain
		    = continue_class (objc_implementation_context);
		;}
    break;

  case 611:
#line 2789 "objc/objc-parse.y"
    {
		  objc_pq_context = 1;
		  objc_interface_context
		    = start_protocol(PROTOCOL_INTERFACE_TYPE, yyvsp[-1].ttype, yyvsp[0].ttype);
		;}
    break;

  case 612:
#line 2795 "objc/objc-parse.y"
    {
		  objc_pq_context = 0;
		  finish_protocol(objc_interface_context);
		  objc_interface_context = NULL_TREE;
		;}
    break;

  case 613:
#line 2804 "objc/objc-parse.y"
    {
		  objc_declare_protocols (yyvsp[-1].ttype);
		;}
    break;

  case 614:
#line 2811 "objc/objc-parse.y"
    {
		  yyval.ttype = NULL_TREE;
		;}
    break;

  case 616:
#line 2819 "objc/objc-parse.y"
    {
		  if (yyvsp[-2].code == LT_EXPR && yyvsp[0].code == GT_EXPR)
		    yyval.ttype = yyvsp[-1].ttype;
		  else
		    YYERROR1;
		;}
    break;

  case 619:
#line 2833 "objc/objc-parse.y"
    { objc_public_flag = 2; ;}
    break;

  case 620:
#line 2834 "objc/objc-parse.y"
    { objc_public_flag = 0; ;}
    break;

  case 621:
#line 2835 "objc/objc-parse.y"
    { objc_public_flag = 1; ;}
    break;

  case 622:
#line 2840 "objc/objc-parse.y"
    {
                  yyval.ttype = NULL_TREE;
                ;}
    break;

  case 624:
#line 2845 "objc/objc-parse.y"
    {
                  if (pedantic)
		    pedwarn ("extra semicolon in struct or union specified");
                ;}
    break;

  case 625:
#line 2863 "objc/objc-parse.y"
    { yyval.ttype = yyvsp[0].ttype;
		  POP_DECLSPEC_STACK; ;}
    break;

  case 626:
#line 2866 "objc/objc-parse.y"
    { yyval.ttype = yyvsp[0].ttype;
		  POP_DECLSPEC_STACK; ;}
    break;

  case 627:
#line 2869 "objc/objc-parse.y"
    { yyval.ttype = NULL_TREE; ;}
    break;

  case 628:
#line 2874 "objc/objc-parse.y"
    { yyval.ttype = NULL_TREE; ;}
    break;

  case 631:
#line 2881 "objc/objc-parse.y"
    {
		  yyval.ttype = add_instance_variable (objc_ivar_context,
					      objc_public_flag,
					      yyvsp[0].ttype, current_declspecs,
					      NULL_TREE);
                ;}
    break;

  case 632:
#line 2888 "objc/objc-parse.y"
    {
		  yyval.ttype = add_instance_variable (objc_ivar_context,
					      objc_public_flag,
					      yyvsp[-2].ttype, current_declspecs, yyvsp[0].ttype);
                ;}
    break;

  case 633:
#line 2894 "objc/objc-parse.y"
    {
		  yyval.ttype = add_instance_variable (objc_ivar_context,
					      objc_public_flag,
					      NULL_TREE,
					      current_declspecs, yyvsp[0].ttype);
                ;}
    break;

  case 634:
#line 2904 "objc/objc-parse.y"
    { objc_inherit_code = CLASS_METHOD_DECL; ;}
    break;

  case 635:
#line 2906 "objc/objc-parse.y"
    { objc_inherit_code = INSTANCE_METHOD_DECL; ;}
    break;

  case 636:
#line 2911 "objc/objc-parse.y"
    {
		  objc_pq_context = 1;
		  if (!objc_implementation_context)
		    fatal_error ("method definition not in class context");
		;}
    break;

  case 637:
#line 2917 "objc/objc-parse.y"
    {
		  objc_pq_context = 0;
		  objc_add_method (objc_implementation_context,
				   yyvsp[0].ttype,
				   objc_inherit_code == CLASS_METHOD_DECL);
		  start_method_def (yyvsp[0].ttype);
		;}
    break;

  case 638:
#line 2925 "objc/objc-parse.y"
    {
		  continue_method_def ();
		;}
    break;

  case 639:
#line 2929 "objc/objc-parse.y"
    {
		  finish_method_def ();
		;}
    break;

  case 642:
#line 2941 "objc/objc-parse.y"
    { yyval.ttype = NULL_TREE; ;}
    break;

  case 646:
#line 2951 "objc/objc-parse.y"
    {
		  /* Remember protocol qualifiers in prototypes.  */
		  objc_pq_context = 1;
		;}
    break;

  case 647:
#line 2956 "objc/objc-parse.y"
    {
		  /* Forget protocol qualifiers here.  */
		  objc_pq_context = 0;
		  objc_add_method (objc_interface_context,
				   yyvsp[0].ttype,
				   objc_inherit_code == CLASS_METHOD_DECL);
		;}
    break;

  case 649:
#line 2968 "objc/objc-parse.y"
    {
		  yyval.ttype = build_method_decl (objc_inherit_code, yyvsp[-2].ttype, yyvsp[0].ttype, NULL_TREE);
		;}
    break;

  case 650:
#line 2973 "objc/objc-parse.y"
    {
		  yyval.ttype = build_method_decl (objc_inherit_code, NULL_TREE, yyvsp[0].ttype, NULL_TREE);
		;}
    break;

  case 651:
#line 2978 "objc/objc-parse.y"
    {
		  yyval.ttype = build_method_decl (objc_inherit_code, yyvsp[-3].ttype, yyvsp[-1].ttype, yyvsp[0].ttype);
		;}
    break;

  case 652:
#line 2983 "objc/objc-parse.y"
    {
		  yyval.ttype = build_method_decl (objc_inherit_code, NULL_TREE, yyvsp[-1].ttype, yyvsp[0].ttype);
		;}
    break;

  case 661:
#line 3013 "objc/objc-parse.y"
    { POP_DECLSPEC_STACK; ;}
    break;

  case 662:
#line 3015 "objc/objc-parse.y"
    { shadow_tag (yyvsp[-1].ttype); ;}
    break;

  case 663:
#line 3017 "objc/objc-parse.y"
    { pedwarn ("empty declaration"); ;}
    break;

  case 664:
#line 3022 "objc/objc-parse.y"
    { push_parm_decl (yyvsp[0].ttype); ;}
    break;

  case 665:
#line 3024 "objc/objc-parse.y"
    { push_parm_decl (yyvsp[0].ttype); ;}
    break;

  case 666:
#line 3032 "objc/objc-parse.y"
    { yyval.ttype = build_tree_list (build_tree_list (current_declspecs,
							 yyvsp[-1].ttype),
					chainon (yyvsp[0].ttype, all_prefix_attributes)); ;}
    break;

  case 667:
#line 3036 "objc/objc-parse.y"
    { yyval.ttype = build_tree_list (build_tree_list (current_declspecs,
							 yyvsp[-1].ttype),
					chainon (yyvsp[0].ttype, all_prefix_attributes)); ;}
    break;

  case 668:
#line 3040 "objc/objc-parse.y"
    { yyval.ttype = yyvsp[0].ttype; ;}
    break;

  case 669:
#line 3045 "objc/objc-parse.y"
    {
		  yyval.ttype = NULL_TREE;
		;}
    break;

  case 670:
#line 3049 "objc/objc-parse.y"
    {
		  /* oh what a kludge! */
		  yyval.ttype = objc_ellipsis_node;
		;}
    break;

  case 671:
#line 3054 "objc/objc-parse.y"
    {
		  pushlevel (0);
		;}
    break;

  case 672:
#line 3058 "objc/objc-parse.y"
    {
		  /* returns a tree list node generated by get_parm_info */
		  yyval.ttype = yyvsp[0].ttype;
		  poplevel (0, 0, 0);
		;}
    break;

  case 675:
#line 3073 "objc/objc-parse.y"
    {
		  yyval.ttype = chainon (yyvsp[-1].ttype, yyvsp[0].ttype);
		;}
    break;

  case 702:
#line 3095 "objc/objc-parse.y"
    {
		  yyval.ttype = build_keyword_decl (yyvsp[-5].ttype, yyvsp[-2].ttype, yyvsp[0].ttype);
		;}
    break;

  case 703:
#line 3100 "objc/objc-parse.y"
    {
		  yyval.ttype = build_keyword_decl (yyvsp[-2].ttype, NULL_TREE, yyvsp[0].ttype);
		;}
    break;

  case 704:
#line 3105 "objc/objc-parse.y"
    {
		  yyval.ttype = build_keyword_decl (NULL_TREE, yyvsp[-2].ttype, yyvsp[0].ttype);
		;}
    break;

  case 705:
#line 3110 "objc/objc-parse.y"
    {
		  yyval.ttype = build_keyword_decl (NULL_TREE, NULL_TREE, yyvsp[0].ttype);
		;}
    break;

  case 709:
#line 3123 "objc/objc-parse.y"
    {
		  yyval.ttype = chainon (yyvsp[-1].ttype, yyvsp[0].ttype);
		;}
    break;

  case 710:
#line 3131 "objc/objc-parse.y"
    {
		  if (TREE_CHAIN (yyvsp[0].ttype) == NULL_TREE)
		    /* just return the expr., remove a level of indirection */
		    yyval.ttype = TREE_VALUE (yyvsp[0].ttype);
                  else
		    /* we have a comma expr., we will collapse later */
		    yyval.ttype = yyvsp[0].ttype;
		;}
    break;

  case 711:
#line 3143 "objc/objc-parse.y"
    {
		  yyval.ttype = build_tree_list (yyvsp[-2].ttype, yyvsp[0].ttype);
		;}
    break;

  case 712:
#line 3147 "objc/objc-parse.y"
    {
		  yyval.ttype = build_tree_list (NULL_TREE, yyvsp[0].ttype);
		;}
    break;

  case 714:
#line 3155 "objc/objc-parse.y"
    {
		  yyval.ttype = get_class_reference (yyvsp[0].ttype);
		;}
    break;

  case 715:
#line 3159 "objc/objc-parse.y"
    {
		  yyval.ttype = get_class_reference (yyvsp[0].ttype);
		;}
    break;

  case 716:
#line 3166 "objc/objc-parse.y"
    { yyval.ttype = build_tree_list (yyvsp[-2].ttype, yyvsp[-1].ttype); ;}
    break;

  case 720:
#line 3177 "objc/objc-parse.y"
    {
		  yyval.ttype = chainon (yyvsp[-1].ttype, yyvsp[0].ttype);
		;}
    break;

  case 721:
#line 3184 "objc/objc-parse.y"
    {
		  yyval.ttype = build_tree_list (yyvsp[-1].ttype, NULL_TREE);
		;}
    break;

  case 722:
#line 3188 "objc/objc-parse.y"
    {
		  yyval.ttype = build_tree_list (NULL_TREE, NULL_TREE);
		;}
    break;

  case 723:
#line 3195 "objc/objc-parse.y"
    {
		  yyval.ttype = yyvsp[-1].ttype;
		;}
    break;

  case 724:
#line 3202 "objc/objc-parse.y"
    {
		  yyval.ttype = yyvsp[-1].ttype;
		;}
    break;

  case 725:
#line 3211 "objc/objc-parse.y"
    {
		  yyval.ttype = groktypename (yyvsp[-1].ttype);
		;}
    break;


    }

/* Line 991 of yacc.c.  */
#line 6565 "objc/objc-parse.c"

  yyvsp -= yylen;
  yyssp -= yylen;


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
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("syntax error, unexpected ") + 1;
	  yysize += yystrlen (yytname[yytype]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			const char *yyq = ! yycount ? ", expecting " : " or ";
			yyp = yystpcpy (yyp, yyq);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yycount++;
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* Return failure if at end of input.  */
      if (yychar == YYEOF)
        {
	  /* Pop the error token.  */
          YYPOPSTACK;
	  /* Pop the rest of the stack.  */
	  while (yyss < yyssp)
	    {
	      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
	      yydestruct (yystos[*yyssp], yyvsp);
	      YYPOPSTACK;
	    }
	  YYABORT;
        }

      YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
      yydestruct (yytoken, &yylval);
      yychar = YYEMPTY;

    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab2;


/*----------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
yyerrlab1:

  /* Suppress GCC warning that yyerrlab1 is unused when no action
     invokes YYERROR.  */
#if defined (__GNUC_MINOR__) && 2093 <= (__GNUC__ * 1000 + __GNUC_MINOR__) \
    && !defined __cplusplus
  __attribute__ ((__unused__))
#endif


  goto yyerrlab2;


/*---------------------------------------------------------------.
| yyerrlab2 -- pop states until the error token can be shifted.  |
`---------------------------------------------------------------*/
yyerrlab2:
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

      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp);
      yyvsp--;
      yystate = *--yyssp;

      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;


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
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 3216 "objc/objc-parse.y"


/* yylex() is a thin wrapper around c_lex(), all it does is translate
   cpplib.h's token codes into yacc's token codes.  */

static enum cpp_ttype last_token;

/* The reserved keyword table.  */
struct resword
{
  const char *word;
  ENUM_BITFIELD(rid) rid : 16;
  unsigned int disable   : 16;
};

/* Disable mask.  Keywords are disabled if (reswords[i].disable & mask) is
   _true_.  */
#define D_C89	0x01	/* not in C89 */
#define D_EXT	0x02	/* GCC extension */
#define D_EXT89	0x04	/* GCC extension incorporated in C99 */
#define D_OBJC	0x08	/* Objective C only */

static const struct resword reswords[] =
{
  { "_Bool",		RID_BOOL,	0 },
  { "_Complex",		RID_COMPLEX,	0 },
  { "__FUNCTION__",	RID_FUNCTION_NAME, 0 },
  { "__PRETTY_FUNCTION__", RID_PRETTY_FUNCTION_NAME, 0 },
  { "__alignof",	RID_ALIGNOF,	0 },
  { "__alignof__",	RID_ALIGNOF,	0 },
  { "__asm",		RID_ASM,	0 },
  { "__asm__",		RID_ASM,	0 },
  { "__attribute",	RID_ATTRIBUTE,	0 },
  { "__attribute__",	RID_ATTRIBUTE,	0 },
  { "__builtin_choose_expr", RID_CHOOSE_EXPR, 0 },
  { "__builtin_types_compatible_p", RID_TYPES_COMPATIBLE_P, 0 },
  { "__builtin_va_arg",	RID_VA_ARG,	0 },
  { "__complex",	RID_COMPLEX,	0 },
  { "__complex__",	RID_COMPLEX,	0 },
  { "__const",		RID_CONST,	0 },
  { "__const__",	RID_CONST,	0 },
  { "__extension__",	RID_EXTENSION,	0 },
  { "__func__",		RID_C99_FUNCTION_NAME, 0 },
  { "__imag",		RID_IMAGPART,	0 },
  { "__imag__",		RID_IMAGPART,	0 },
  { "__inline",		RID_INLINE,	0 },
  { "__inline__",	RID_INLINE,	0 },
  { "__label__",	RID_LABEL,	0 },
  { "__ptrbase",	RID_PTRBASE,	0 },
  { "__ptrbase__",	RID_PTRBASE,	0 },
  { "__ptrextent",	RID_PTREXTENT,	0 },
  { "__ptrextent__",	RID_PTREXTENT,	0 },
  { "__ptrvalue",	RID_PTRVALUE,	0 },
  { "__ptrvalue__",	RID_PTRVALUE,	0 },
  { "__real",		RID_REALPART,	0 },
  { "__real__",		RID_REALPART,	0 },
  { "__restrict",	RID_RESTRICT,	0 },
  { "__restrict__",	RID_RESTRICT,	0 },
  { "__signed",		RID_SIGNED,	0 },
  { "__signed__",	RID_SIGNED,	0 },
  { "__thread",		RID_THREAD,	0 },
  { "__typeof",		RID_TYPEOF,	0 },
  { "__typeof__",	RID_TYPEOF,	0 },
  { "__volatile",	RID_VOLATILE,	0 },
  { "__volatile__",	RID_VOLATILE,	0 },
  { "asm",		RID_ASM,	D_EXT },
  { "auto",		RID_AUTO,	0 },
  { "break",		RID_BREAK,	0 },
  { "case",		RID_CASE,	0 },
  { "char",		RID_CHAR,	0 },
  { "const",		RID_CONST,	0 },
  { "continue",		RID_CONTINUE,	0 },
  { "default",		RID_DEFAULT,	0 },
  { "do",		RID_DO,		0 },
  { "double",		RID_DOUBLE,	0 },
  { "else",		RID_ELSE,	0 },
  { "enum",		RID_ENUM,	0 },
  { "extern",		RID_EXTERN,	0 },
  { "float",		RID_FLOAT,	0 },
  { "for",		RID_FOR,	0 },
  { "goto",		RID_GOTO,	0 },
  { "if",		RID_IF,		0 },
  { "inline",		RID_INLINE,	D_EXT89 },
  { "int",		RID_INT,	0 },
  { "long",		RID_LONG,	0 },
  { "register",		RID_REGISTER,	0 },
  { "restrict",		RID_RESTRICT,	D_C89 },
  { "return",		RID_RETURN,	0 },
  { "short",		RID_SHORT,	0 },
  { "signed",		RID_SIGNED,	0 },
  { "sizeof",		RID_SIZEOF,	0 },
  { "static",		RID_STATIC,	0 },
  { "struct",		RID_STRUCT,	0 },
  { "switch",		RID_SWITCH,	0 },
  { "typedef",		RID_TYPEDEF,	0 },
  { "typeof",		RID_TYPEOF,	D_EXT },
  { "union",		RID_UNION,	0 },
  { "unsigned",		RID_UNSIGNED,	0 },
  { "void",		RID_VOID,	0 },
  { "volatile",		RID_VOLATILE,	0 },
  { "while",		RID_WHILE,	0 },
  { "id",		RID_ID,			D_OBJC },

  /* These objc keywords are recognized only immediately after
     an '@'.  */
  { "class",		RID_AT_CLASS,		D_OBJC },
  { "compatibility_alias", RID_AT_ALIAS,	D_OBJC },
  { "defs",		RID_AT_DEFS,		D_OBJC },
  { "encode",		RID_AT_ENCODE,		D_OBJC },
  { "end",		RID_AT_END,		D_OBJC },
  { "implementation",	RID_AT_IMPLEMENTATION,	D_OBJC },
  { "interface",	RID_AT_INTERFACE,	D_OBJC },
  { "private",		RID_AT_PRIVATE,		D_OBJC },
  { "protected",	RID_AT_PROTECTED,	D_OBJC },
  { "protocol",		RID_AT_PROTOCOL,	D_OBJC },
  { "public",		RID_AT_PUBLIC,		D_OBJC },
  { "selector",		RID_AT_SELECTOR,	D_OBJC },
  { "throw",		RID_AT_THROW,		D_OBJC },
  { "try",		RID_AT_TRY,		D_OBJC },
  { "catch",		RID_AT_CATCH,		D_OBJC },
  { "finally",		RID_AT_FINALLY,		D_OBJC },
  { "synchronized",	RID_AT_SYNCHRONIZED,	D_OBJC },
  /* These are recognized only in protocol-qualifier context
     (see above) */
  { "bycopy",		RID_BYCOPY,		D_OBJC },
  { "byref",		RID_BYREF,		D_OBJC },
  { "in",		RID_IN,			D_OBJC },
  { "inout",		RID_INOUT,		D_OBJC },
  { "oneway",		RID_ONEWAY,		D_OBJC },
  { "out",		RID_OUT,		D_OBJC },
};
#define N_reswords (sizeof reswords / sizeof (struct resword))

/* Table mapping from RID_* constants to yacc token numbers.
   Unfortunately we have to have entries for all the keywords in all
   three languages.  */
static const short rid_to_yy[RID_MAX] =
{
  /* RID_STATIC */	STATIC,
  /* RID_UNSIGNED */	TYPESPEC,
  /* RID_LONG */	TYPESPEC,
  /* RID_CONST */	TYPE_QUAL,
  /* RID_EXTERN */	SCSPEC,
  /* RID_REGISTER */	SCSPEC,
  /* RID_TYPEDEF */	SCSPEC,
  /* RID_SHORT */	TYPESPEC,
  /* RID_INLINE */	SCSPEC,
  /* RID_VOLATILE */	TYPE_QUAL,
  /* RID_SIGNED */	TYPESPEC,
  /* RID_AUTO */	SCSPEC,
  /* RID_RESTRICT */	TYPE_QUAL,

  /* C extensions */
  /* RID_COMPLEX */	TYPESPEC,
  /* RID_THREAD */	SCSPEC,

  /* C++ */
  /* RID_FRIEND */	0,
  /* RID_VIRTUAL */	0,
  /* RID_EXPLICIT */	0,
  /* RID_EXPORT */	0,
  /* RID_MUTABLE */	0,

  /* ObjC */
  /* RID_IN */		TYPE_QUAL,
  /* RID_OUT */		TYPE_QUAL,
  /* RID_INOUT */	TYPE_QUAL,
  /* RID_BYCOPY */	TYPE_QUAL,
  /* RID_BYREF */	TYPE_QUAL,
  /* RID_ONEWAY */	TYPE_QUAL,

  /* C */
  /* RID_INT */		TYPESPEC,
  /* RID_CHAR */	TYPESPEC,
  /* RID_FLOAT */	TYPESPEC,
  /* RID_DOUBLE */	TYPESPEC,
  /* RID_VOID */	TYPESPEC,
  /* RID_ENUM */	ENUM,
  /* RID_STRUCT */	STRUCT,
  /* RID_UNION */	UNION,
  /* RID_IF */		IF,
  /* RID_ELSE */	ELSE,
  /* RID_WHILE */	WHILE,
  /* RID_DO */		DO,
  /* RID_FOR */		FOR,
  /* RID_SWITCH */	SWITCH,
  /* RID_CASE */	CASE,
  /* RID_DEFAULT */	DEFAULT,
  /* RID_BREAK */	BREAK,
  /* RID_CONTINUE */	CONTINUE,
  /* RID_RETURN */	RETURN,
  /* RID_GOTO */	GOTO,
  /* RID_SIZEOF */	SIZEOF,

  /* C extensions */
  /* RID_ASM */		ASM_KEYWORD,
  /* RID_TYPEOF */	TYPEOF,
  /* RID_ALIGNOF */	ALIGNOF,
  /* RID_ATTRIBUTE */	ATTRIBUTE,
  /* RID_VA_ARG */	VA_ARG,
  /* RID_EXTENSION */	EXTENSION,
  /* RID_IMAGPART */	IMAGPART,
  /* RID_REALPART */	REALPART,
  /* RID_LABEL */	LABEL,
  /* RID_PTRBASE */	PTR_BASE,
  /* RID_PTREXTENT */	PTR_EXTENT,
  /* RID_PTRVALUE */	PTR_VALUE,

  /* RID_CHOOSE_EXPR */			CHOOSE_EXPR,
  /* RID_TYPES_COMPATIBLE_P */		TYPES_COMPATIBLE_P,

  /* RID_FUNCTION_NAME */		FUNC_NAME,
  /* RID_PRETTY_FUNCTION_NAME */	FUNC_NAME,
  /* RID_C99_FUNCTION_NAME */		FUNC_NAME,

  /* C++ */
  /* RID_BOOL */	TYPESPEC,
  /* RID_WCHAR */	0,
  /* RID_CLASS */	0,
  /* RID_PUBLIC */	0,
  /* RID_PRIVATE */	0,
  /* RID_PROTECTED */	0,
  /* RID_TEMPLATE */	0,
  /* RID_NULL */	0,
  /* RID_CATCH */	0,
  /* RID_DELETE */	0,
  /* RID_FALSE */	0,
  /* RID_NAMESPACE */	0,
  /* RID_NEW */		0,
  /* RID_OFFSETOF */    0,
  /* RID_OPERATOR */	0,
  /* RID_THIS */	0,
  /* RID_THROW */	0,
  /* RID_TRUE */	0,
  /* RID_TRY */		0,
  /* RID_TYPENAME */	0,
  /* RID_TYPEID */	0,
  /* RID_USING */	0,

  /* casts */
  /* RID_CONSTCAST */	0,
  /* RID_DYNCAST */	0,
  /* RID_REINTCAST */	0,
  /* RID_STATCAST */	0,

  /* Objective C */
  /* RID_ID */			OBJECTNAME,
  /* RID_AT_ENCODE */		ENCODE,
  /* RID_AT_END */		END,
  /* RID_AT_CLASS */		CLASS,
  /* RID_AT_ALIAS */		ALIAS,
  /* RID_AT_DEFS */		DEFS,
  /* RID_AT_PRIVATE */		PRIVATE,
  /* RID_AT_PROTECTED */	PROTECTED,
  /* RID_AT_PUBLIC */		PUBLIC,
  /* RID_AT_PROTOCOL */		PROTOCOL,
  /* RID_AT_SELECTOR */		SELECTOR,
  /* RID_AT_THROW */		AT_THROW,
  /* RID_AT_TRY */		AT_TRY,
  /* RID_AT_CATCH */		AT_CATCH,
  /* RID_AT_FINALLY */		AT_FINALLY,
  /* RID_AT_SYNCHRONIZED */	AT_SYNCHRONIZED,
  /* RID_AT_INTERFACE */	INTERFACE,
  /* RID_AT_IMPLEMENTATION */	IMPLEMENTATION
};

static void
init_reswords (void)
{
  unsigned int i;
  tree id;
  int mask = (flag_isoc99 ? 0 : D_C89)
	      | (flag_no_asm ? (flag_isoc99 ? D_EXT : D_EXT|D_EXT89) : 0);

  if (!c_dialect_objc ())
     mask |= D_OBJC;

  ridpointers = ggc_calloc ((int) RID_MAX, sizeof (tree));
  for (i = 0; i < N_reswords; i++)
    {
      /* If a keyword is disabled, do not enter it into the table
	 and so create a canonical spelling that isn't a keyword.  */
      if (reswords[i].disable & mask)
	continue;

      id = get_identifier (reswords[i].word);
      C_RID_CODE (id) = reswords[i].rid;
      C_IS_RESERVED_WORD (id) = 1;
      ridpointers [(int) reswords[i].rid] = id;
    }
}

#define NAME(type) cpp_type2name (type)

static void
yyerror (const char *msgid)
{
  c_parse_error (msgid, last_token, yylval.ttype);
}

static int
yylexname (void)
{
  tree decl;

  int objc_force_identifier = objc_need_raw_identifier;
  OBJC_NEED_RAW_IDENTIFIER (0);

  if (C_IS_RESERVED_WORD (yylval.ttype))
    {
      enum rid rid_code = C_RID_CODE (yylval.ttype);

      /* Turn non-typedefed refs to "id" into plain identifiers; this
	 allows constructs like "void foo(id id);" to work.  */
      if (rid_code == RID_ID)
      {
	decl = lookup_name (yylval.ttype);
	if (decl == NULL_TREE || TREE_CODE (decl) != TYPE_DECL)
	  return IDENTIFIER;
      }

      if (!OBJC_IS_AT_KEYWORD (rid_code)
	  && (!OBJC_IS_PQ_KEYWORD (rid_code) || objc_pq_context))
      {
	/* Return the canonical spelling for this keyword.  */
	yylval.ttype = ridpointers[(int) rid_code];
	return rid_to_yy[(int) rid_code];
      }
    }

  decl = lookup_name (yylval.ttype);
  if (decl)
    {
      if (TREE_CODE (decl) == TYPE_DECL)
	return TYPENAME;
    }
  else
    {
      tree objc_interface_decl = is_class_name (yylval.ttype);
      /* ObjC class names are in the same namespace as variables and
	 typedefs, and hence are shadowed by local declarations.  */
      if (objc_interface_decl
	  && (global_bindings_p ()
	      || (!objc_force_identifier && !decl)))
	{
	  yylval.ttype = objc_interface_decl;
	  return CLASSNAME;
	}
    }

  return IDENTIFIER;
}

static inline int
_yylex (void)
{
 get_next:
  last_token = c_lex (&yylval.ttype);
  switch (last_token)
    {
    case CPP_EQ:					return '=';
    case CPP_NOT:					return '!';
    case CPP_GREATER:	yylval.code = GT_EXPR;		return ARITHCOMPARE;
    case CPP_LESS:	yylval.code = LT_EXPR;		return ARITHCOMPARE;
    case CPP_PLUS:	yylval.code = PLUS_EXPR;	return '+';
    case CPP_MINUS:	yylval.code = MINUS_EXPR;	return '-';
    case CPP_MULT:	yylval.code = MULT_EXPR;	return '*';
    case CPP_DIV:	yylval.code = TRUNC_DIV_EXPR;	return '/';
    case CPP_MOD:	yylval.code = TRUNC_MOD_EXPR;	return '%';
    case CPP_AND:	yylval.code = BIT_AND_EXPR;	return '&';
    case CPP_OR:	yylval.code = BIT_IOR_EXPR;	return '|';
    case CPP_XOR:	yylval.code = BIT_XOR_EXPR;	return '^';
    case CPP_RSHIFT:	yylval.code = RSHIFT_EXPR;	return RSHIFT;
    case CPP_LSHIFT:	yylval.code = LSHIFT_EXPR;	return LSHIFT;

    case CPP_COMPL:					return '~';
    case CPP_AND_AND:					return ANDAND;
    case CPP_OR_OR:					return OROR;
    case CPP_QUERY:					return '?';
    case CPP_OPEN_PAREN:				return '(';
    case CPP_EQ_EQ:	yylval.code = EQ_EXPR;		return EQCOMPARE;
    case CPP_NOT_EQ:	yylval.code = NE_EXPR;		return EQCOMPARE;
    case CPP_GREATER_EQ:yylval.code = GE_EXPR;		return ARITHCOMPARE;
    case CPP_LESS_EQ:	yylval.code = LE_EXPR;		return ARITHCOMPARE;

    case CPP_PLUS_EQ:	yylval.code = PLUS_EXPR;	return ASSIGN;
    case CPP_MINUS_EQ:	yylval.code = MINUS_EXPR;	return ASSIGN;
    case CPP_MULT_EQ:	yylval.code = MULT_EXPR;	return ASSIGN;
    case CPP_DIV_EQ:	yylval.code = TRUNC_DIV_EXPR;	return ASSIGN;
    case CPP_MOD_EQ:	yylval.code = TRUNC_MOD_EXPR;	return ASSIGN;
    case CPP_AND_EQ:	yylval.code = BIT_AND_EXPR;	return ASSIGN;
    case CPP_OR_EQ:	yylval.code = BIT_IOR_EXPR;	return ASSIGN;
    case CPP_XOR_EQ:	yylval.code = BIT_XOR_EXPR;	return ASSIGN;
    case CPP_RSHIFT_EQ:	yylval.code = RSHIFT_EXPR;	return ASSIGN;
    case CPP_LSHIFT_EQ:	yylval.code = LSHIFT_EXPR;	return ASSIGN;

    case CPP_OPEN_SQUARE:				return '[';
    case CPP_CLOSE_SQUARE:				return ']';
    case CPP_OPEN_BRACE:				return '{';
    case CPP_CLOSE_BRACE:				return '}';
    case CPP_ELLIPSIS:					return ELLIPSIS;

    case CPP_PLUS_PLUS:					return PLUSPLUS;
    case CPP_MINUS_MINUS:				return MINUSMINUS;
    case CPP_DEREF:					return POINTSAT;
    case CPP_DOT:					return '.';

      /* The following tokens may affect the interpretation of any
	 identifiers following, if doing Objective-C.  */
    case CPP_COLON:		OBJC_NEED_RAW_IDENTIFIER (0);	return ':';
    case CPP_COMMA:		OBJC_NEED_RAW_IDENTIFIER (0);	return ',';
    case CPP_CLOSE_PAREN:	OBJC_NEED_RAW_IDENTIFIER (0);	return ')';
    case CPP_SEMICOLON:		OBJC_NEED_RAW_IDENTIFIER (0);	return ';';

    case CPP_EOF:
      return 0;

    case CPP_NAME:
      return yylexname ();

    case CPP_AT_NAME:
      /* This only happens in Objective-C; it must be a keyword.  */
      return rid_to_yy [(int) C_RID_CODE (yylval.ttype)];

    case CPP_NUMBER:
    case CPP_CHAR:
    case CPP_WCHAR:
      return CONSTANT;

    case CPP_STRING:
    case CPP_WSTRING:
      return STRING;

    case CPP_OBJC_STRING:
      return OBJC_STRING;

      /* These tokens are C++ specific (and will not be generated
         in C mode, but let's be cautious).  */
    case CPP_SCOPE:
    case CPP_DEREF_STAR:
    case CPP_DOT_STAR:
    case CPP_MIN_EQ:
    case CPP_MAX_EQ:
    case CPP_MIN:
    case CPP_MAX:
      /* These tokens should not survive translation phase 4.  */
    case CPP_HASH:
    case CPP_PASTE:
      error ("syntax error at '%s' token", NAME(last_token));
      goto get_next;

    default:
      abort ();
    }
  /* NOTREACHED */
}

static int
yylex (void)
{
  int r;
  timevar_push (TV_LEX);
  r = _yylex();
  timevar_pop (TV_LEX);
  return r;
}

/* Function used when yydebug is set, to print a token in more detail.  */

static void
yyprint (FILE *file, int yychar, YYSTYPE yyl)
{
  tree t = yyl.ttype;

  fprintf (file, " [%s]", NAME(last_token));

  switch (yychar)
    {
    case IDENTIFIER:
    case TYPENAME:
    case OBJECTNAME:
    case TYPESPEC:
    case TYPE_QUAL:
    case SCSPEC:
    case STATIC:
      if (IDENTIFIER_POINTER (t))
	fprintf (file, " `%s'", IDENTIFIER_POINTER (t));
      break;

    case CONSTANT:
      fprintf (file, " %s", GET_MODE_NAME (TYPE_MODE (TREE_TYPE (t))));
      if (TREE_CODE (t) == INTEGER_CST)
	{
	  fputs (" ", file);
	  fprintf (file, HOST_WIDE_INT_PRINT_DOUBLE_HEX,
		   TREE_INT_CST_HIGH (t), TREE_INT_CST_LOW (t));
	}
      break;
    }
}

/* This is not the ideal place to put these, but we have to get them out
   of c-lex.c because cp/lex.c has its own versions.  */

/* Parse the file.  */
void
c_parse_file (void)
{
  yyparse ();
  /* In case there were missing closebraces, get us back to the global
     binding level.  */
  while (! global_bindings_p ())
    poplevel (0, 0, 0);
  /* __FUNCTION__ is defined at file scope ("").  This
     call may not be necessary as my tests indicate it
     still works without it.  */
  finish_fname_decls ();

  if (malloced_yyss)
    {
      free (malloced_yyss);
      free (malloced_yyvs);
      malloced_yyss = 0;
    }
}

#include "gt-c-parse.h"


