/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

extern int yylex();
void yyerror(const char *s);

ASTNode* root;

#line 83 "parser.tab.c"

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

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_ID = 3,                         /* ID  */
  YYSYMBOL_INT_LITERAL = 4,                /* INT_LITERAL  */
  YYSYMBOL_FLOAT_LITERAL = 5,              /* FLOAT_LITERAL  */
  YYSYMBOL_STR_LITERAL = 6,                /* STR_LITERAL  */
  YYSYMBOL_CHAR_LITERAL = 7,               /* CHAR_LITERAL  */
  YYSYMBOL_INT = 8,                        /* INT  */
  YYSYMBOL_FLOAT_T = 9,                    /* FLOAT_T  */
  YYSYMBOL_CHAR_T = 10,                    /* CHAR_T  */
  YYSYMBOL_IF = 11,                        /* IF  */
  YYSYMBOL_ELSE = 12,                      /* ELSE  */
  YYSYMBOL_WHILE = 13,                     /* WHILE  */
  YYSYMBOL_RETURN = 14,                    /* RETURN  */
  YYSYMBOL_BREAK = 15,                     /* BREAK  */
  YYSYMBOL_CONTINUE = 16,                  /* CONTINUE  */
  YYSYMBOL_PRINT = 17,                     /* PRINT  */
  YYSYMBOL_SCAN = 18,                      /* SCAN  */
  YYSYMBOL_EQ = 19,                        /* EQ  */
  YYSYMBOL_NE = 20,                        /* NE  */
  YYSYMBOL_LE = 21,                        /* LE  */
  YYSYMBOL_GE = 22,                        /* GE  */
  YYSYMBOL_AND = 23,                       /* AND  */
  YYSYMBOL_OR = 24,                        /* OR  */
  YYSYMBOL_NOT = 25,                       /* NOT  */
  YYSYMBOL_26_ = 26,                       /* '='  */
  YYSYMBOL_27_ = 27,                       /* '<'  */
  YYSYMBOL_28_ = 28,                       /* '>'  */
  YYSYMBOL_29_ = 29,                       /* '+'  */
  YYSYMBOL_30_ = 30,                       /* '-'  */
  YYSYMBOL_31_ = 31,                       /* '*'  */
  YYSYMBOL_32_ = 32,                       /* '/'  */
  YYSYMBOL_LOWER_THAN_ELSE = 33,           /* LOWER_THAN_ELSE  */
  YYSYMBOL_34_ = 34,                       /* '('  */
  YYSYMBOL_35_ = 35,                       /* ')'  */
  YYSYMBOL_36_ = 36,                       /* ','  */
  YYSYMBOL_37_ = 37,                       /* ';'  */
  YYSYMBOL_38_ = 38,                       /* '{'  */
  YYSYMBOL_39_ = 39,                       /* '}'  */
  YYSYMBOL_YYACCEPT = 40,                  /* $accept  */
  YYSYMBOL_program = 41,                   /* program  */
  YYSYMBOL_func_list = 42,                 /* func_list  */
  YYSYMBOL_func_def = 43,                  /* func_def  */
  YYSYMBOL_param_list = 44,                /* param_list  */
  YYSYMBOL_param = 45,                     /* param  */
  YYSYMBOL_stmt_list = 46,                 /* stmt_list  */
  YYSYMBOL_stmt = 47,                      /* stmt  */
  YYSYMBOL_block = 48,                     /* block  */
  YYSYMBOL_declaration = 49,               /* declaration  */
  YYSYMBOL_assignment = 50,                /* assignment  */
  YYSYMBOL_while_stmt = 51,                /* while_stmt  */
  YYSYMBOL_if_stmt = 52,                   /* if_stmt  */
  YYSYMBOL_expr = 53,                      /* expr  */
  YYSYMBOL_arg_list = 54                   /* arg_list  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




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

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
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
typedef yytype_uint8 yy_state_t;

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
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
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

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

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
#define YYFINAL  45
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   249

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  40
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  15
/* YYNRULES -- Number of rules.  */
#define YYNRULES  61
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  138

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   281


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      34,    35,    31,    29,    36,    30,     2,    32,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    37,
      27,    26,    28,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    38,     2,    39,     2,     2,     2,     2,
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
      25,    33
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    46,    46,    48,    50,    59,    68,    77,    79,    81,
      83,    85,    90,    92,   102,   104,   106,   111,   113,   118,
     119,   120,   121,   123,   126,   129,   135,   138,   141,   144,
     147,   150,   155,   160,   161,   162,   166,   174,   179,   181,
     186,   187,   188,   189,   190,   191,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   205,   207,
     212,   214
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "ID", "INT_LITERAL",
  "FLOAT_LITERAL", "STR_LITERAL", "CHAR_LITERAL", "INT", "FLOAT_T",
  "CHAR_T", "IF", "ELSE", "WHILE", "RETURN", "BREAK", "CONTINUE", "PRINT",
  "SCAN", "EQ", "NE", "LE", "GE", "AND", "OR", "NOT", "'='", "'<'", "'>'",
  "'+'", "'-'", "'*'", "'/'", "LOWER_THAN_ELSE", "'('", "')'", "','",
  "';'", "'{'", "'}'", "$accept", "program", "func_list", "func_def",
  "param_list", "param", "stmt_list", "stmt", "block", "declaration",
  "assignment", "while_stmt", "if_stmt", "expr", "arg_list", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-95)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      36,   -23,     1,     7,     9,   -17,   -15,   108,     4,    11,
      -3,    21,    72,    43,    36,   -95,   -95,    72,   -95,    31,
      35,   -95,   -95,   108,    66,    45,    50,    61,   108,   108,
      65,   -95,   -95,   -95,   108,   108,   116,   -95,   -95,   100,
     129,   138,   165,   166,    63,   -95,   -95,   -95,   -95,   -95,
     -95,   203,   140,   203,    -6,    -2,     5,    16,   135,   152,
      89,   217,   169,   108,   108,   108,   108,   108,   108,   108,
     108,   108,   108,   108,   108,   -95,   143,   186,   150,   -95,
     -95,   -95,   -95,   -95,   149,   108,   191,   192,   199,   173,
       0,   -95,   173,    41,    62,    72,   173,   -95,    73,   -95,
     120,   120,   120,   120,   217,    99,   120,   120,    85,    85,
     -95,   -95,   175,   182,   183,   -95,   203,   -95,   -95,   -95,
     -95,   173,    16,   -95,   173,   173,   216,   -95,   -95,   -95,
     -95,   -95,   -95,   -95,   -95,   -95,    72,   -95
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     3,     5,     2,    18,    29,     0,
       0,    21,    22,     0,     0,    33,    34,    35,     0,     0,
      54,    55,    56,    57,     0,     0,     0,    27,    28,     0,
       0,     0,     0,     0,     0,     1,     6,     4,    17,    19,
      20,    36,     0,    60,     0,     0,     0,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    26,     0,     0,     0,    33,
      34,    35,    32,    31,     0,     0,     0,     0,     0,     0,
       0,    12,     0,     0,     0,     0,     0,    59,     0,    53,
      48,    49,    46,    47,    50,    51,    44,    45,    40,    41,
      42,    43,     0,     0,     0,    30,    61,    14,    15,    16,
      10,     0,     0,    11,     0,     0,    38,    37,    58,    24,
      23,    25,     7,    13,     8,     9,     0,    39
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -95,   -95,   -95,   189,   104,   107,     6,   -94,   -87,   -95,
     -95,   -95,   -95,    -7,   180
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,    13,    14,    15,    90,    91,    16,    17,    18,    19,
      20,    21,    22,    53,    54
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      36,   126,   120,    23,    25,   123,    86,    87,    88,   127,
      26,    24,    27,    86,    87,    88,    51,    28,    44,    29,
      47,    58,    59,    48,    86,    87,    88,    61,    62,    84,
      85,    39,    77,    89,   132,   121,   122,   134,   135,     1,
      92,    37,   137,    45,     2,     3,     4,     5,    38,     6,
       7,     8,     9,    10,    11,    40,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,    49,    30,
      31,    32,    50,    33,    12,     1,   124,   122,   116,    55,
      41,    42,    43,     5,    56,     6,     7,     8,     9,    10,
      11,    34,    30,    31,    32,    57,    33,   125,   122,    60,
      35,    52,    82,    30,    31,    32,    76,    33,   128,    85,
      12,    30,    31,    32,    34,    33,    73,    74,    63,    64,
      65,    66,    67,    35,    97,    34,    69,    70,    71,    72,
      73,    74,    78,    34,    35,    63,    64,    65,    66,    67,
      68,    79,    35,    69,    70,    71,    72,    73,    74,    71,
      72,    73,    74,    75,    63,    64,    65,    66,    67,    68,
      93,    94,    69,    70,    71,    72,    73,    74,    80,    81,
      95,    63,    64,    65,    66,    67,    68,    83,   112,    69,
      70,    71,    72,    73,    74,   114,   115,    96,    63,    64,
      65,    66,    67,    68,   117,   118,    69,    70,    71,    72,
      73,    74,   119,    46,    99,    63,    64,    65,    66,    67,
      68,    12,   129,    69,    70,    71,    72,    73,    74,   130,
     131,   113,    63,    64,    65,    66,    67,    68,   136,   133,
      69,    70,    71,    72,    73,    74,    63,    64,    65,    66,
      98,     0,     0,     0,    69,    70,    71,    72,    73,    74
};

static const yytype_int16 yycheck[] =
{
       7,    95,    89,    26,     3,    92,     8,     9,    10,    96,
       3,    34,     3,     8,     9,    10,    23,    34,    12,    34,
      14,    28,    29,    17,     8,     9,    10,    34,    35,    35,
      36,    34,    39,    35,   121,    35,    36,   124,   125,     3,
      35,    37,   136,     0,     8,     9,    10,    11,    37,    13,
      14,    15,    16,    17,    18,    34,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    37,     3,
       4,     5,    37,     7,    38,     3,    35,    36,    85,    34,
       8,     9,    10,    11,    34,    13,    14,    15,    16,    17,
      18,    25,     3,     4,     5,    34,     7,    35,    36,    34,
      34,    35,    39,     3,     4,     5,     6,     7,    35,    36,
      38,     3,     4,     5,    25,     7,    31,    32,    19,    20,
      21,    22,    23,    34,    35,    25,    27,    28,    29,    30,
      31,    32,     3,    25,    34,    19,    20,    21,    22,    23,
      24,     3,    34,    27,    28,    29,    30,    31,    32,    29,
      30,    31,    32,    37,    19,    20,    21,    22,    23,    24,
      56,    57,    27,    28,    29,    30,    31,    32,     3,     3,
      35,    19,    20,    21,    22,    23,    24,    37,    35,    27,
      28,    29,    30,    31,    32,    35,    37,    35,    19,    20,
      21,    22,    23,    24,     3,     3,    27,    28,    29,    30,
      31,    32,     3,    14,    35,    19,    20,    21,    22,    23,
      24,    38,    37,    27,    28,    29,    30,    31,    32,    37,
      37,    35,    19,    20,    21,    22,    23,    24,    12,   122,
      27,    28,    29,    30,    31,    32,    19,    20,    21,    22,
      60,    -1,    -1,    -1,    27,    28,    29,    30,    31,    32
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     8,     9,    10,    11,    13,    14,    15,    16,
      17,    18,    38,    41,    42,    43,    46,    47,    48,    49,
      50,    51,    52,    26,    34,     3,     3,     3,    34,    34,
       3,     4,     5,     7,    25,    34,    53,    37,    37,    34,
      34,     8,     9,    10,    46,     0,    43,    46,    46,    37,
      37,    53,    35,    53,    54,    34,    34,    34,    53,    53,
      34,    53,    53,    19,    20,    21,    22,    23,    24,    27,
      28,    29,    30,    31,    32,    37,     6,    53,     3,     3,
       3,     3,    39,    37,    35,    36,     8,     9,    10,    35,
      44,    45,    35,    44,    44,    35,    35,    35,    54,    35,
      53,    53,    53,    53,    53,    53,    53,    53,    53,    53,
      53,    53,    35,    35,    35,    37,    53,     3,     3,     3,
      48,    35,    36,    48,    35,    35,    47,    48,    35,    37,
      37,    37,    48,    45,    48,    48,    12,    47
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    40,    41,    41,    41,    42,    42,    43,    43,    43,
      43,    43,    44,    44,    45,    45,    45,    46,    46,    47,
      47,    47,    47,    47,    47,    47,    47,    47,    47,    47,
      47,    47,    48,    49,    49,    49,    50,    51,    52,    52,
      53,    53,    53,    53,    53,    53,    53,    53,    53,    53,
      53,    53,    53,    53,    53,    53,    53,    53,    53,    53,
      54,    54
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,     1,     2,     6,     6,     6,
       5,     5,     1,     3,     2,     2,     2,     2,     1,     2,
       2,     1,     1,     5,     5,     5,     3,     2,     2,     1,
       5,     4,     3,     2,     2,     2,     3,     5,     5,     7,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     3,     1,     1,     1,     1,     4,     3,
       1,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


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

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


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




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
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
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
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
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
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
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

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
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
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
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
  case 2: /* program: stmt_list  */
#line 47 "parser.y"
        { root = (yyvsp[0].node); }
#line 1226 "parser.tab.c"
    break;

  case 3: /* program: func_list  */
#line 49 "parser.y"
        { }
#line 1232 "parser.tab.c"
    break;

  case 4: /* program: func_list stmt_list  */
#line 51 "parser.y"
        {
            ASTNode* cur = root;
            while (cur->next) cur = cur->next;
            cur->next = (yyvsp[0].node);
        }
#line 1242 "parser.tab.c"
    break;

  case 5: /* func_list: func_def  */
#line 60 "parser.y"
        {
            if (root == NULL) root = (yyvsp[0].node);
            else {
                ASTNode* cur = root;
                while (cur->next) cur = cur->next;
                cur->next = (yyvsp[0].node);
            }
        }
#line 1255 "parser.tab.c"
    break;

  case 6: /* func_list: func_list func_def  */
#line 69 "parser.y"
        {
            ASTNode* cur = root;
            while (cur->next) cur = cur->next;
            cur->next = (yyvsp[0].node);
        }
#line 1265 "parser.tab.c"
    break;

  case 7: /* func_def: INT ID '(' param_list ')' block  */
#line 78 "parser.y"
        { (yyval.node) = create_func_def_node("int", (yyvsp[-4].str), (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1271 "parser.tab.c"
    break;

  case 8: /* func_def: FLOAT_T ID '(' param_list ')' block  */
#line 80 "parser.y"
        { (yyval.node) = create_func_def_node("float", (yyvsp[-4].str), (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1277 "parser.tab.c"
    break;

  case 9: /* func_def: CHAR_T ID '(' param_list ')' block  */
#line 82 "parser.y"
        { (yyval.node) = create_func_def_node("char", (yyvsp[-4].str), (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1283 "parser.tab.c"
    break;

  case 10: /* func_def: INT ID '(' ')' block  */
#line 84 "parser.y"
        { (yyval.node) = create_func_def_node("int", (yyvsp[-3].str), NULL, (yyvsp[0].node)); }
#line 1289 "parser.tab.c"
    break;

  case 11: /* func_def: FLOAT_T ID '(' ')' block  */
#line 86 "parser.y"
        { (yyval.node) = create_func_def_node("float", (yyvsp[-3].str), NULL, (yyvsp[0].node)); }
#line 1295 "parser.tab.c"
    break;

  case 12: /* param_list: param  */
#line 91 "parser.y"
        { (yyval.node) = (yyvsp[0].node); }
#line 1301 "parser.tab.c"
    break;

  case 13: /* param_list: param_list ',' param  */
#line 93 "parser.y"
        {
            ASTNode* cur = (yyvsp[-2].node);
            while (cur->next) cur = cur->next;
            cur->next = (yyvsp[0].node);
            (yyval.node) = (yyvsp[-2].node);
        }
#line 1312 "parser.tab.c"
    break;

  case 14: /* param: INT ID  */
#line 103 "parser.y"
        { (yyval.node) = create_param_node("int",   (yyvsp[0].str)); }
#line 1318 "parser.tab.c"
    break;

  case 15: /* param: FLOAT_T ID  */
#line 105 "parser.y"
        { (yyval.node) = create_param_node("float", (yyvsp[0].str)); }
#line 1324 "parser.tab.c"
    break;

  case 16: /* param: CHAR_T ID  */
#line 107 "parser.y"
        { (yyval.node) = create_param_node("char",  (yyvsp[0].str)); }
#line 1330 "parser.tab.c"
    break;

  case 17: /* stmt_list: stmt stmt_list  */
#line 112 "parser.y"
        { (yyvsp[-1].node)->next = (yyvsp[0].node); (yyval.node) = (yyvsp[-1].node); }
#line 1336 "parser.tab.c"
    break;

  case 18: /* stmt_list: stmt  */
#line 114 "parser.y"
        { (yyval.node) = (yyvsp[0].node); }
#line 1342 "parser.tab.c"
    break;

  case 19: /* stmt: declaration ';'  */
#line 118 "parser.y"
                        { (yyval.node) = (yyvsp[-1].node); }
#line 1348 "parser.tab.c"
    break;

  case 20: /* stmt: assignment ';'  */
#line 119 "parser.y"
                        { (yyval.node) = (yyvsp[-1].node); }
#line 1354 "parser.tab.c"
    break;

  case 21: /* stmt: while_stmt  */
#line 120 "parser.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 1360 "parser.tab.c"
    break;

  case 22: /* stmt: if_stmt  */
#line 121 "parser.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 1366 "parser.tab.c"
    break;

  case 23: /* stmt: PRINT '(' expr ')' ';'  */
#line 124 "parser.y"
        { (yyval.node) = create_node(NODE_PRINT, NULL, (yyvsp[-2].node), NULL); }
#line 1372 "parser.tab.c"
    break;

  case 24: /* stmt: PRINT '(' STR_LITERAL ')' ';'  */
#line 127 "parser.y"
        { (yyval.node) = create_node(NODE_PRINT, (yyvsp[-2].str), NULL, NULL); }
#line 1378 "parser.tab.c"
    break;

  case 25: /* stmt: SCAN '(' ID ')' ';'  */
#line 130 "parser.y"
        {
            ASTNode* id = create_node(NODE_ID, (yyvsp[-2].str), NULL, NULL);
            (yyval.node) = create_node(NODE_SCAN, NULL, id, NULL);
        }
#line 1387 "parser.tab.c"
    break;

  case 26: /* stmt: RETURN expr ';'  */
#line 136 "parser.y"
        { (yyval.node) = create_node(NODE_RETURN, NULL, (yyvsp[-1].node), NULL); }
#line 1393 "parser.tab.c"
    break;

  case 27: /* stmt: BREAK ';'  */
#line 139 "parser.y"
        { (yyval.node) = create_node(NODE_BREAK, NULL, NULL, NULL); }
#line 1399 "parser.tab.c"
    break;

  case 28: /* stmt: CONTINUE ';'  */
#line 142 "parser.y"
        { (yyval.node) = create_node(NODE_CONTINUE, NULL, NULL, NULL); }
#line 1405 "parser.tab.c"
    break;

  case 29: /* stmt: block  */
#line 145 "parser.y"
        { (yyval.node) = (yyvsp[0].node); }
#line 1411 "parser.tab.c"
    break;

  case 30: /* stmt: ID '(' arg_list ')' ';'  */
#line 148 "parser.y"
        { (yyval.node) = create_func_call_node((yyvsp[-4].str), (yyvsp[-2].node)); }
#line 1417 "parser.tab.c"
    break;

  case 31: /* stmt: ID '(' ')' ';'  */
#line 151 "parser.y"
        { (yyval.node) = create_func_call_node((yyvsp[-3].str), NULL); }
#line 1423 "parser.tab.c"
    break;

  case 32: /* block: '{' stmt_list '}'  */
#line 156 "parser.y"
        { (yyval.node) = create_block_node((yyvsp[-1].node)); }
#line 1429 "parser.tab.c"
    break;

  case 33: /* declaration: INT ID  */
#line 160 "parser.y"
                 { (yyval.node) = create_var_node("int",   (yyvsp[0].str)); }
#line 1435 "parser.tab.c"
    break;

  case 34: /* declaration: FLOAT_T ID  */
#line 161 "parser.y"
                 { (yyval.node) = create_var_node("float", (yyvsp[0].str)); }
#line 1441 "parser.tab.c"
    break;

  case 35: /* declaration: CHAR_T ID  */
#line 162 "parser.y"
                 { (yyval.node) = create_var_node("char",  (yyvsp[0].str)); }
#line 1447 "parser.tab.c"
    break;

  case 36: /* assignment: ID '=' expr  */
#line 167 "parser.y"
        {
            ASTNode* id = create_node(NODE_ID, (yyvsp[-2].str), NULL, NULL);
            (yyval.node) = create_node(NODE_ASSIGN, "=", id, (yyvsp[0].node));
        }
#line 1456 "parser.tab.c"
    break;

  case 37: /* while_stmt: WHILE '(' expr ')' block  */
#line 175 "parser.y"
        { (yyval.node) = create_while_node((yyvsp[-2].node), (yyvsp[0].node)); }
#line 1462 "parser.tab.c"
    break;

  case 38: /* if_stmt: IF '(' expr ')' stmt  */
#line 180 "parser.y"
        { (yyval.node) = create_if_node((yyvsp[-2].node), (yyvsp[0].node), NULL); }
#line 1468 "parser.tab.c"
    break;

  case 39: /* if_stmt: IF '(' expr ')' stmt ELSE stmt  */
#line 182 "parser.y"
        { (yyval.node) = create_if_node((yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1474 "parser.tab.c"
    break;

  case 40: /* expr: expr '+' expr  */
#line 186 "parser.y"
                      { (yyval.node) = create_node(NODE_BIN_OP, "+",  (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1480 "parser.tab.c"
    break;

  case 41: /* expr: expr '-' expr  */
#line 187 "parser.y"
                      { (yyval.node) = create_node(NODE_BIN_OP, "-",  (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1486 "parser.tab.c"
    break;

  case 42: /* expr: expr '*' expr  */
#line 188 "parser.y"
                      { (yyval.node) = create_node(NODE_BIN_OP, "*",  (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1492 "parser.tab.c"
    break;

  case 43: /* expr: expr '/' expr  */
#line 189 "parser.y"
                      { (yyval.node) = create_node(NODE_BIN_OP, "/",  (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1498 "parser.tab.c"
    break;

  case 44: /* expr: expr '<' expr  */
#line 190 "parser.y"
                      { (yyval.node) = create_node(NODE_BIN_OP, "<",  (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1504 "parser.tab.c"
    break;

  case 45: /* expr: expr '>' expr  */
#line 191 "parser.y"
                      { (yyval.node) = create_node(NODE_BIN_OP, ">",  (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1510 "parser.tab.c"
    break;

  case 46: /* expr: expr LE expr  */
#line 192 "parser.y"
                      { (yyval.node) = create_node(NODE_BIN_OP, "<=", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1516 "parser.tab.c"
    break;

  case 47: /* expr: expr GE expr  */
#line 193 "parser.y"
                      { (yyval.node) = create_node(NODE_BIN_OP, ">=", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1522 "parser.tab.c"
    break;

  case 48: /* expr: expr EQ expr  */
#line 194 "parser.y"
                      { (yyval.node) = create_node(NODE_BIN_OP, "==", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1528 "parser.tab.c"
    break;

  case 49: /* expr: expr NE expr  */
#line 195 "parser.y"
                      { (yyval.node) = create_node(NODE_BIN_OP, "!=", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1534 "parser.tab.c"
    break;

  case 50: /* expr: expr AND expr  */
#line 196 "parser.y"
                      { (yyval.node) = create_node(NODE_LOGIC_OP, "&&", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1540 "parser.tab.c"
    break;

  case 51: /* expr: expr OR expr  */
#line 197 "parser.y"
                      { (yyval.node) = create_node(NODE_LOGIC_OP, "||", (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1546 "parser.tab.c"
    break;

  case 52: /* expr: NOT expr  */
#line 198 "parser.y"
                      { (yyval.node) = create_node(NODE_LOGIC_OP, "!",  (yyvsp[0].node), NULL); }
#line 1552 "parser.tab.c"
    break;

  case 53: /* expr: '(' expr ')'  */
#line 199 "parser.y"
                      { (yyval.node) = (yyvsp[-1].node); }
#line 1558 "parser.tab.c"
    break;

  case 54: /* expr: ID  */
#line 200 "parser.y"
                      { (yyval.node) = create_node(NODE_ID,    (yyvsp[0].str), NULL, NULL); }
#line 1564 "parser.tab.c"
    break;

  case 55: /* expr: INT_LITERAL  */
#line 201 "parser.y"
                      { (yyval.node) = create_node(NODE_INT,   (yyvsp[0].str), NULL, NULL); }
#line 1570 "parser.tab.c"
    break;

  case 56: /* expr: FLOAT_LITERAL  */
#line 202 "parser.y"
                      { (yyval.node) = create_node(NODE_FLOAT, (yyvsp[0].str), NULL, NULL); }
#line 1576 "parser.tab.c"
    break;

  case 57: /* expr: CHAR_LITERAL  */
#line 203 "parser.y"
                      { (yyval.node) = create_node(NODE_CHAR,  (yyvsp[0].str), NULL, NULL); }
#line 1582 "parser.tab.c"
    break;

  case 58: /* expr: ID '(' arg_list ')'  */
#line 206 "parser.y"
        { (yyval.node) = create_func_call_node((yyvsp[-3].str), (yyvsp[-1].node)); }
#line 1588 "parser.tab.c"
    break;

  case 59: /* expr: ID '(' ')'  */
#line 208 "parser.y"
        { (yyval.node) = create_func_call_node((yyvsp[-2].str), NULL); }
#line 1594 "parser.tab.c"
    break;

  case 60: /* arg_list: expr  */
#line 213 "parser.y"
        { (yyval.node) = (yyvsp[0].node); }
#line 1600 "parser.tab.c"
    break;

  case 61: /* arg_list: arg_list ',' expr  */
#line 215 "parser.y"
        {
            ASTNode* cur = (yyvsp[-2].node);
            while (cur->next) cur = cur->next;
            cur->next = (yyvsp[0].node);
            (yyval.node) = (yyvsp[-2].node);
        }
#line 1611 "parser.tab.c"
    break;


#line 1615 "parser.tab.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

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
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
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
  ++yynerrs;

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

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
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
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 223 "parser.y"


void yyerror(const char *s) {
    fprintf(stderr, "Parser error: %s\n", s);
}
