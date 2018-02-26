/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "work_good.y" /* yacc.c:339  */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "declaration.c"    

extern FILE* yyin;
extern char* yytext;
extern int yylineno;
extern int yylex();

#define MAX_STRUCTS 1000
#define MAX_NR_IDENTIFIERS 1000
#define MAX_NR_FUNCTIONS 1000
#define MAX_NR_STACKS 255



struct declaration *identifierStruct[MAX_NR_STACKS][MAX_NR_IDENTIFIERS],
                   *functionStruct[MAX_NR_FUNCTIONS];


int yyerror(char *s){
   printf("The Program is bad defined!!! Error: %s at line:%d\n", s, yylineno);
}

void handleError(int code) {
    
    if(code == DIFFERENT_TYPES) {
        yyerror("The variables have not the same type!");
    }
    else if (code == DIV_BY_ZERO) {
        yyerror("Warning! Division by zero!");
    }
    else if (code == DST_NOT_INITIALIZED) {
        yyerror("Destination variable is not initialized!");
    }
    else if (code == SRC_NOT_INITIALIZED) {
        yyerror("Source variable is not initialized!");
    }
    else if (code == OPERATION_NOT_SUPPORTED_ON_TYPE) {
        yyerror("Operation is not supported on this type of variable!");
    }
    if(code != 0) {
        exit(code);
    }
}


int nrStacks;
int nrIdentifiers[MAX_NR_STACKS];

void increaseStack() {
  nrStacks++;
}

void decreaseStacks() {
  for(int i=0; i<nrIdentifiers[nrStacks-1]; i++){
      free (identifierStruct[nrStacks-1][i]);
      free (identifierStruct[nrStacks-1][i]);
      identifierStruct[nrStacks-1][i] = NULL;
  }
  nrStacks--;
}

struct declaration* find(struct declaration** vector, int numberElements, char *name) {
    for(int i = 0; i < numberElements; i++) {
        if(compare(vector[i]->name, name) == 0){
            return vector[i];
        }
    }
    
    return NULL;
}

struct declaration* findInList(struct list* theList, char *name) {
    if(theList == NULL) {
        return NULL;
    }
    
    for(struct list *item = theList->head; item != NULL; item = item->next) {
        if(compare(item->value->name, name) == 0){
            return item->value;
        }
    }
   
    return NULL;
}

struct declaration* getMember(struct declaration* var, struct declaration *idTypeForms) {
    struct declaration *member = findInList(var->vars, idTypeForms->name);
    
    if (member == NULL) {
        yyerror("Member not found!");
        exit(1);
    }
    
    return member;
}


// # ## IDENTIFIERS ## #

struct declaration* findIdentifier(char *name) {
    struct declaration *elem = find(identifierStruct[nrStacks-1], nrIdentifiers[nrStacks-1], name);
    if(elem == NULL) {
        yyerror("Variable is not defined!");
        exit(0);
    }
    return elem;
}

void copyValue(char *dstName, char *srcName){
    
    struct declaration *dst = findIdentifier(dstName);
    struct declaration *src = findIdentifier(srcName);
    handleError(copyTo(dst, src));
}

// # ## END ## #


struct list* cloneVars(struct list *theList) {
    
    if(theList == NULL){
        return NULL;
    }
    
    
    struct list *newLst = newList(cloneVariable(theList->head->value));
    for(struct list *item = newLst->head->next; item != NULL; item = item->next) {         
        newLst = pushBack(newLst, cloneVariable(item->value));
    }
    return newLst;
}


// # ## STRUCTS ## # 

struct declaration *structs[MAX_STRUCTS];
int nrStructs;

struct declaration* findStruct (char *name) {
    struct declaration *elem = find(structs, nrStructs, name);
    if(elem == NULL) {
        yyerror("Struct is not defined!");
        exit(0);
    }
    return elem;
}

void addStructToStack(struct declaration *var) {
    struct declaration *stackStruct = find(structs, nrStructs, var->name);
    if(stackStruct != NULL) {
        yyerror("Struct already defined!");
        exit(0);
    }
    
    structs[nrStructs] = var;
    nrStructs++;
}


struct declaration* addNewStruct(char *structName, struct list *members) {
    struct declaration *strct;
    strct = newDeclaration(NULL, structName, false);
    strct->vars = cloneVars(members);
    
    addStructToStack(strct);
    return strct;
}

void setStructSingle(struct declaration *strct, struct declaration *var) {
    var->type = strdup(strct->name);
    var->vars = cloneVars(strct->vars);
}

void setStruct(struct declaration *strct, struct list *vars) {
    for(struct list *item = vars->head; item != NULL; item = item->next) {
        setStructSingle(strct, item->value);
    }
}

// # ## END ## #

void addToStack(struct declaration *var) {
    struct declaration *stackVar = find(identifierStruct[nrStacks-1], nrIdentifiers[nrStacks-1], var->name);

    if(stackVar != NULL) {
        if(compare(var->type, stackVar->type) != 0) {
            yyerror("Variable already defined with another type!");
            exit(0);
        }
        else {
            yyerror("Variable already defined!");
            exit(0);
        }
    }
    identifierStruct[nrStacks-1][nrIdentifiers[nrStacks-1]] = var;
    nrIdentifiers[nrStacks-1]++;
}

void addListToStack(struct list *vars) {
    for(struct list *item = vars->head; item != NULL; item = item->next){
        addToStack(item->value);
    }
}

// # ## FUNCTION ## # 

int nrFunctions;

void addFuncToStack(struct declaration *func) {
    struct declaration *stackFunc = find(functionStruct, nrFunctions, func->name);
    if(stackFunc != NULL) {
        yyerror("Function already defined!");
        exit(0);
    }
    
    functionStruct[nrFunctions] = func;
    nrFunctions++;
}

struct declaration* addNewFunction(char *functionType, char *functionName, struct list *params) {
    struct declaration *func;
    func = newDeclaration(functionType, functionName, false);
    func->vars = cloneVars(params);
    
    addFuncToStack(func);
    return func;
}

struct declaration* findFunction (char *name) {
    struct declaration *elem = find(functionStruct, nrFunctions, name);
    if(elem == NULL) {
        yyerror("Function is not defined!");
        exit(0);
    }
    return elem;
}



// # ## END ## #




#line 317 "work_good.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif


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
    ID = 258,
    VAR_TYPE = 259,
    INTEGER = 260,
    FLOAT = 261,
    CONST = 262,
    VOID_TYPE = 263,
    PRINT = 264,
    STRUCT = 265,
    RETURN = 266,
    TEOF = 267,
    ASSIGN_PLUS = 268,
    ASSIGN_MINUS = 269,
    ASSIGN_PROD = 270,
    ASSIGN_DIV = 271,
    ASSIGN_MOD = 272,
    ASSIGN = 273,
    MINUS = 274,
    PLUS = 275,
    DIVISION = 276,
    MULTIPLY = 277,
    MODULO = 278,
    CHAR = 279,
    STRING = 280,
    TRUE = 281,
    FALSE = 282,
    BOOL_OP = 283,
    FOR = 284,
    WHILE = 285,
    DO = 286,
    IF = 287,
    ELSE = 288
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 252 "work_good.y" /* yacc.c:355  */
 float floatNumberValue; int intNumberValue; char *StringValue; struct declaration *var; struct list *vars; 

#line 391 "work_good.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);



/* Copy the second part of user declarations.  */

#line 408 "work_good.tab.c" /* yacc.c:358  */

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
#else
typedef signed char yytype_int8;
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
# elif ! defined YYSIZE_T
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
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
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
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
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
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  25
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   402

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  46
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  37
/* YYNRULES -- Number of rules.  */
#define YYNRULES  108
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  213

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   289

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      41,    42,     2,     2,    38,     2,    43,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    45,    35,
       2,     2,     2,    44,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    36,     2,    37,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    39,     2,    40,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   294,   294,   297,   298,   300,   301,   303,   304,   305,
     306,   310,   316,   326,   327,   328,   332,   333,   334,   335,
     338,   340,   346,   352,   359,   360,   362,   363,   366,   375,
     386,   389,   392,   395,   400,   404,   408,   412,   418,   424,
     430,   436,   443,   444,   448,   462,   463,   465,   466,   468,
     469,   470,   471,   472,   473,   474,   477,   483,   488,   492,
     498,   499,   500,   501,   502,   506,   510,   514,   518,   522,
     525,   528,   533,   536,   539,   542,   545,   550,   551,   556,
     557,   561,   562,   577,   578,   579,   582,   587,   592,   593,
     595,   596,   598,   599,   600,   602,   603,   605,   608,   609,
     610,   612,   613,   615,   619,   623,   624,   625,   626
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ID", "VAR_TYPE", "INTEGER", "FLOAT",
  "CONST", "VOID_TYPE", "PRINT", "STRUCT", "RETURN", "TEOF", "ASSIGN_PLUS",
  "ASSIGN_MINUS", "ASSIGN_PROD", "ASSIGN_DIV", "ASSIGN_MOD", "ASSIGN",
  "MINUS", "PLUS", "DIVISION", "MULTIPLY", "MODULO", "CHAR", "STRING",
  "TRUE", "FALSE", "BOOL_OP", "FOR", "WHILE", "DO", "IF", "ELSE",
  "\"simple_if\"", "';'", "'['", "']'", "','", "'{'", "'}'", "'('", "')'",
  "'.'", "'?'", "':'", "$accept", "RunProgram", "CodeProgram",
  "ProgramMainBody", "Declarations", "VariableDeclaration", "IdentTypes",
  "VarDeclList", "DataTypeDecl", "DataTypeBlock", "StructStmt",
  "StructVariableDeclaration", "FunctionPrototype", "DeclParameters",
  "FunctionDecl", "printFunction", "BlockStatement", "Statements",
  "SimpleStmt", "AtomicValues", "AExp", "AssignementDeclID",
  "IdAssignement", "Assignement", "CallParameters", "FunctionCall",
  "SingleStatement", "BExp", "ValueTypes", "CondExp", "IfStatement",
  "IfCond", "StepFor1", "StepFor2_3", "ForStatement", "WhileStatement",
  "ControlStmt", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,    59,    91,    93,    44,   123,
     125,    40,    41,    46,    63,    58
};
# endif

#define YYPACT_NINF -107

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-107)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     281,     8,    22,   121,    43,    51,  -107,    23,    -2,   281,
    -107,    35,    65,    25,  -107,    50,    -3,    40,  -107,    20,
      40,     8,     8,    61,    83,  -107,  -107,  -107,  -107,  -107,
    -107,   194,  -107,   337,   225,     8,     8,    10,    40,    40,
      29,    15,     8,    92,     8,  -107,  -107,    77,   300,  -107,
    -107,  -107,  -107,    86,   100,   101,  -107,  -107,   312,  -107,
      32,  -107,  -107,  -107,   233,  -107,  -107,   150,  -107,  -107,
    -107,  -107,   139,  -107,   155,   141,  -107,   149,  -107,  -107,
    -107,    36,   337,   143,   360,   225,   155,   173,    -3,  -107,
       8,     8,   132,  -107,    72,  -107,    78,     8,   189,  -107,
      58,   161,    40,   337,   337,   337,   337,   337,    42,   337,
    -107,     9,   151,   325,   325,   331,   140,   160,   166,   155,
       8,  -107,  -107,   337,   337,   337,   337,   337,  -107,   225,
     325,  -107,   140,     8,   175,   225,  -107,  -107,     8,     8,
      84,  -107,  -107,    40,     8,  -107,   174,  -107,   150,   150,
     150,   150,   150,  -107,    79,   155,   335,  -107,   208,  -107,
    -107,   178,   172,   180,  -107,  -107,  -107,   173,    71,    71,
    -107,  -107,  -107,  -107,   170,  -107,   337,   155,  -107,  -107,
       8,     8,   136,    40,  -107,   225,  -107,   181,   325,   272,
     272,   325,   365,  -107,  -107,     8,     8,   155,  -107,  -107,
     190,  -107,   199,  -107,  -107,  -107,  -107,   325,   272,   185,
    -107,   272,  -107
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     0,     0,     0,     0,    10,     0,     0,     4,
       5,     0,     0,     0,     8,    13,    16,    22,    18,    13,
      11,     0,     0,     0,     0,     1,     2,     6,     7,     9,
      42,     0,    43,     0,     0,     0,     0,     0,    23,    12,
       0,     0,    20,    13,     0,    56,    57,     0,     0,    58,
      59,    86,    87,     0,     0,     0,    55,    45,     0,    83,
      60,    84,    52,    50,     0,    47,    62,    91,    77,    78,
      93,    63,     0,    90,    92,    85,   105,     0,   107,   108,
      51,    13,     0,    60,     0,     0,    70,     0,    17,    19,
       0,     0,     0,    31,     0,    33,     0,     0,     0,    24,
       0,     0,    21,     0,     0,     0,     0,     0,     0,     0,
      54,     0,    98,     0,     0,    13,    91,     0,    90,     0,
       0,    46,    48,     0,     0,     0,     0,     0,    49,     0,
       0,   106,     0,     0,    14,     0,    38,    34,     0,     0,
       0,    30,    32,    28,     0,    25,     0,    26,    72,    73,
      74,    75,    76,    81,     0,    79,     0,    53,     0,    99,
     100,     0,     0,     0,    69,    94,    88,    61,    65,    64,
      67,    66,    68,    89,     0,    61,     0,    71,    40,    36,
       0,     0,     0,    29,    27,     0,    82,     0,   101,     0,
       0,     0,     0,    39,    35,     0,     0,    80,    44,   102,
       0,   104,    95,    97,    15,    41,    37,   101,     0,     0,
      96,     0,   103
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -107,  -107,  -107,  -107,   232,     3,    -1,     7,    18,  -107,
    -107,   145,  -107,   206,  -107,  -107,   234,  -107,   -59,  -107,
      41,    37,  -107,   195,  -107,  -107,  -107,   -54,   -32,  -106,
    -107,  -107,  -107,    47,  -107,  -107,  -107
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     7,     8,     9,    10,    59,    83,    17,    61,    42,
     100,   101,    13,    94,    14,    62,    63,    64,    65,    66,
      67,    68,    69,    70,   154,    71,    72,    73,    74,    75,
      76,    77,   161,   200,    78,    79,    80
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      16,    16,    86,    11,   118,   122,   160,   162,   163,    20,
      26,    15,    11,    90,    91,    34,   111,    92,    12,    97,
      16,    16,    98,    25,   174,    19,   119,    12,    38,    39,
      60,   118,    90,    91,    87,    88,    92,   129,    18,    18,
      35,    16,    16,    16,   157,    81,    23,    45,    46,   102,
      34,    20,    93,   119,    24,    99,    33,    60,    18,    18,
      30,    37,    97,    60,    31,    98,    49,    50,    51,    52,
      28,    95,    33,    89,    84,   120,   155,   108,    36,    18,
      18,    18,   199,    85,   153,   203,    33,   180,   181,   136,
     137,   182,   125,   126,   127,    15,    16,   173,   145,   116,
      29,   199,    40,   177,   143,   103,   104,   105,   106,   107,
     140,    60,    60,    60,   141,   159,   140,   185,   109,   167,
     142,   186,    41,   132,    21,    22,   116,   112,    33,    60,
     201,   202,   175,   108,    18,   138,   139,   178,   179,   195,
     196,   113,   114,    16,   148,   149,   150,   151,   152,   210,
     156,   183,   212,   197,   115,    44,    45,    46,   158,   123,
     124,   125,   126,   127,   168,   169,   170,   171,   172,   123,
     124,   125,   126,   127,   128,    49,    50,    51,    52,   193,
     194,    18,   164,   129,   131,   130,   133,    60,    60,    60,
      60,   135,    58,   144,   205,   206,   147,    43,    44,    45,
      46,     3,   165,    47,     5,    48,    60,    60,   166,   184,
      60,   176,    22,   188,   189,   191,   198,   192,    49,    50,
      51,    52,   190,    53,    54,   207,    55,   211,    81,    56,
      45,    46,   208,    31,    57,    58,    43,    44,    45,    46,
       3,    27,    47,     5,    48,   146,    96,    32,     0,    49,
      50,    51,    52,   117,   209,     0,     0,    49,    50,    51,
      52,     0,    53,    54,     0,    55,    85,     0,    56,     0,
       0,     0,    31,   121,    58,    43,    44,    45,    46,     3,
       0,    47,     5,    48,     1,     2,     0,     0,     3,     4,
       0,     5,     0,     0,     0,     0,    49,    50,    51,    52,
       0,    53,    54,    81,    55,    45,    46,    56,     0,     0,
       0,    31,     0,    58,     0,   115,     6,    45,    46,     0,
       0,     0,     0,     0,    49,    50,    51,    52,   115,     0,
      45,    46,     0,     0,     0,   110,    49,    50,    51,    52,
      81,    85,    45,    46,   103,   104,   105,   106,   107,    49,
      50,    51,    52,    85,   123,   124,   125,   126,   127,     0,
       0,    49,    50,     0,     0,     0,    58,    33,     0,     0,
       0,     0,   108,     0,     0,     0,     0,   187,    82,   123,
     124,   125,   126,   127,   123,   124,   125,   126,   127,     0,
       0,     0,     0,     0,     0,     0,     0,   134,     0,     0,
       0,     0,   204
};

static const yytype_int16 yycheck[] =
{
       1,     2,    34,     0,    58,    64,   112,   113,   114,     2,
      12,     3,     9,     3,     4,    18,    48,     7,     0,     4,
      21,    22,     7,     0,   130,     3,    58,     9,    21,    22,
      31,    85,     3,     4,    35,    36,     7,    28,     1,     2,
      43,    42,    43,    44,    35,     3,     3,     5,     6,    42,
      18,    44,    42,    85,     3,    40,    36,    58,    21,    22,
      35,    41,     4,    64,    39,     7,    24,    25,    26,    27,
      35,    42,    36,    36,    33,    43,   108,    41,    38,    42,
      43,    44,   188,    41,    42,   191,    36,     3,     4,    90,
      91,     7,    21,    22,    23,     3,    97,   129,    40,    58,
      35,   207,    41,   135,    97,    13,    14,    15,    16,    17,
      38,   112,   113,   114,    42,   112,    38,    38,    41,   120,
      42,    42,    39,    82,     3,     4,    85,    41,    36,   130,
     189,   190,   133,    41,    97,     3,     4,   138,   139,     3,
       4,    41,    41,   144,   103,   104,   105,   106,   107,   208,
     109,   144,   211,   185,     3,     4,     5,     6,     7,    19,
      20,    21,    22,    23,   123,   124,   125,   126,   127,    19,
      20,    21,    22,    23,    35,    24,    25,    26,    27,   180,
     181,   144,    42,    28,    35,    44,    43,   188,   189,   190,
     191,    18,    41,     4,   195,   196,    35,     3,     4,     5,
       6,     7,    42,     9,    10,    11,   207,   208,    42,    35,
     211,    36,     4,    35,    42,    45,    35,   176,    24,    25,
      26,    27,    42,    29,    30,    35,    32,    42,     3,    35,
       5,     6,    33,    39,    40,    41,     3,     4,     5,     6,
       7,     9,     9,    10,    11,   100,    40,    13,    -1,    24,
      25,    26,    27,    58,   207,    -1,    -1,    24,    25,    26,
      27,    -1,    29,    30,    -1,    32,    41,    -1,    35,    -1,
      -1,    -1,    39,    40,    41,     3,     4,     5,     6,     7,
      -1,     9,    10,    11,     3,     4,    -1,    -1,     7,     8,
      -1,    10,    -1,    -1,    -1,    -1,    24,    25,    26,    27,
      -1,    29,    30,     3,    32,     5,     6,    35,    -1,    -1,
      -1,    39,    -1,    41,    -1,     3,    35,     5,     6,    -1,
      -1,    -1,    -1,    -1,    24,    25,    26,    27,     3,    -1,
       5,     6,    -1,    -1,    -1,    35,    24,    25,    26,    27,
       3,    41,     5,     6,    13,    14,    15,    16,    17,    24,
      25,    26,    27,    41,    19,    20,    21,    22,    23,    -1,
      -1,    24,    25,    -1,    -1,    -1,    41,    36,    -1,    -1,
      -1,    -1,    41,    -1,    -1,    -1,    -1,    42,    41,    19,
      20,    21,    22,    23,    19,    20,    21,    22,    23,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    37,    -1,    -1,
      -1,    -1,    37
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     7,     8,    10,    35,    47,    48,    49,
      50,    51,    54,    58,    60,     3,    52,    53,    67,     3,
      53,     3,     4,     3,     3,     0,    12,    50,    35,    35,
      35,    39,    62,    36,    18,    43,    38,    41,    53,    53,
      41,    39,    55,     3,     4,     5,     6,     9,    11,    24,
      25,    26,    27,    29,    30,    32,    35,    40,    41,    51,
      52,    54,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    71,    72,    73,    74,    75,    76,    77,    80,    81,
      82,     3,    41,    52,    66,    41,    74,    52,    52,    67,
       3,     4,     7,    42,    59,    42,    59,     4,     7,    40,
      56,    57,    53,    13,    14,    15,    16,    17,    41,    41,
      35,    74,    41,    41,    41,     3,    66,    69,    73,    74,
      43,    40,    64,    19,    20,    21,    22,    23,    35,    28,
      44,    35,    66,    43,    37,    18,    52,    52,     3,     4,
      38,    42,    42,    53,     4,    40,    57,    35,    66,    66,
      66,    66,    66,    42,    70,    74,    66,    35,     7,    51,
      75,    78,    75,    75,    42,    42,    42,    52,    66,    66,
      66,    66,    66,    74,    75,    52,    36,    74,    52,    52,
       3,     4,     7,    53,    35,    38,    42,    42,    35,    42,
      42,    45,    66,    52,    52,     3,     4,    74,    35,    75,
      79,    64,    64,    75,    37,    52,    52,    35,    33,    79,
      64,    42,    64
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    46,    47,    48,    48,    49,    49,    50,    50,    50,
      50,    51,    51,    52,    52,    52,    53,    53,    53,    53,
      54,    54,    54,    54,    55,    55,    56,    56,    57,    57,
      58,    58,    58,    58,    59,    59,    59,    59,    59,    59,
      59,    59,    60,    60,    61,    62,    62,    63,    63,    64,
      64,    64,    64,    64,    64,    64,    65,    65,    65,    65,
      66,    66,    66,    66,    66,    66,    66,    66,    66,    66,
      67,    67,    68,    68,    68,    68,    68,    69,    69,    70,
      70,    71,    71,    72,    72,    72,    73,    73,    73,    73,
      74,    74,    75,    75,    75,    76,    76,    77,    78,    78,
      78,    79,    79,    80,    81,    82,    82,    82,    82
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     0,     1,     1,     2,     2,     1,     2,
       1,     2,     3,     1,     4,     7,     1,     3,     1,     3,
       3,     4,     2,     3,     2,     3,     2,     3,     2,     3,
       5,     4,     5,     4,     2,     4,     3,     5,     2,     4,
       3,     5,     2,     2,     5,     2,     3,     1,     2,     2,
       1,     1,     1,     3,     2,     1,     1,     1,     1,     1,
       1,     3,     1,     1,     3,     3,     3,     3,     3,     3,
       3,     5,     3,     3,     3,     3,     3,     1,     1,     1,
       3,     3,     4,     1,     1,     1,     1,     1,     3,     3,
       1,     1,     1,     1,     3,     5,     7,     5,     0,     1,
       1,     0,     1,     9,     5,     1,     2,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
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
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
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

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

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
      int yyn = yypact[*yyssp];
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
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
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
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
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
          yyp++;
          yyformat++;
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
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

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
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
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
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
#line 294 "work_good.y" /* yacc.c:1646  */
    { printf("The Program is Well Defined! \n"); exit(0); }
#line 1673 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 310 "work_good.y" /* yacc.c:1646  */
    {
        for(struct list *elem = (yyvsp[0].vars)->head ; elem !=NULL; elem=elem->next) {
            elem->value->type = strdup((yyvsp[-1].StringValue));
            addToStack(elem->value);
        }
}
#line 1684 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 316 "work_good.y" /* yacc.c:1646  */
    {
                 
        for(struct list *elem = (yyvsp[0].vars)->head ; elem !=NULL; elem=elem->next) {
            elem->value->type = strdup((yyvsp[-1].StringValue));
            addToStack(elem->value);
        }   
    }
#line 1696 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 326 "work_good.y" /* yacc.c:1646  */
    { (yyval.var) = newDeclaration(NULL, (yyvsp[0].StringValue), false); }
#line 1702 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 327 "work_good.y" /* yacc.c:1646  */
    { (yyval.var) = newDeclaration(NULL, (yyvsp[-3].StringValue), true); }
#line 1708 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 328 "work_good.y" /* yacc.c:1646  */
    { (yyval.var) = newDeclaration(NULL, (yyvsp[-6].StringValue), true); }
#line 1714 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 332 "work_good.y" /* yacc.c:1646  */
    { (yyval.vars) = newList((yyvsp[0].var)); }
#line 1720 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 333 "work_good.y" /* yacc.c:1646  */
    { (yyval.vars) = pushBack((yyvsp[-2].vars), (yyvsp[0].var));}
#line 1726 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 334 "work_good.y" /* yacc.c:1646  */
    { (yyval.vars) = newList((yyvsp[0].var)); }
#line 1732 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 335 "work_good.y" /* yacc.c:1646  */
    { (yyval.vars) = pushBack((yyvsp[-2].vars), (yyvsp[0].var));}
#line 1738 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 339 "work_good.y" /* yacc.c:1646  */
    { addNewStruct((yyvsp[-1].StringValue), (yyvsp[0].vars)); }
#line 1744 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 341 "work_good.y" /* yacc.c:1646  */
    { 
                struct declaration *strct = addNewStruct((yyvsp[-2].StringValue), (yyvsp[-1].vars));
                setStruct(strct, (yyvsp[0].vars)); //set the type for structs
                addListToStack((yyvsp[0].vars)); //add the variables in stack
             }
#line 1754 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 347 "work_good.y" /* yacc.c:1646  */
    { 
                struct declaration *strct = findStruct((yyvsp[-1].StringValue)); 
                setStruct(strct, (yyvsp[0].vars));
                addListToStack((yyvsp[0].vars));
             }
#line 1764 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 353 "work_good.y" /* yacc.c:1646  */
    {  
                struct declaration *strct = findStruct((yyvsp[-1].StringValue)); 
                setStruct(strct, (yyvsp[0].vars));
                addListToStack((yyvsp[0].vars));
             }
#line 1774 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 359 "work_good.y" /* yacc.c:1646  */
    { (yyval.vars) = NULL; }
#line 1780 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 360 "work_good.y" /* yacc.c:1646  */
    { (yyval.vars) = (yyvsp[-1].vars); }
#line 1786 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 362 "work_good.y" /* yacc.c:1646  */
    { (yyval.vars) = (yyvsp[-1].vars); }
#line 1792 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 363 "work_good.y" /* yacc.c:1646  */
    { (yyval.vars) = extendBack((yyvsp[-2].vars), (yyvsp[-1].vars)); }
#line 1798 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 366 "work_good.y" /* yacc.c:1646  */
    {
   
        for(struct list *elem = (yyvsp[0].vars)->head ; elem !=NULL; elem=elem->next) {
            
            elem->value->type = strdup((yyvsp[-1].StringValue));
 
        }
        (yyval.vars) = (yyvsp[0].vars);
}
#line 1812 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 375 "work_good.y" /* yacc.c:1646  */
    {
                 
        for(struct list *elem = (yyvsp[0].vars)->head ; elem !=NULL; elem=elem->next) {
            elem->value->type = strdup((yyvsp[-1].StringValue));
        }   
        
        (yyval.vars) = (yyvsp[0].vars);
    }
#line 1825 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 386 "work_good.y" /* yacc.c:1646  */
    {
                    (yyval.var) = addNewFunction((yyvsp[-4].StringValue), (yyvsp[-3].StringValue), (yyvsp[-1].vars));
                  }
#line 1833 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 389 "work_good.y" /* yacc.c:1646  */
    {
                    (yyval.var) = addNewFunction((yyvsp[-3].StringValue), (yyvsp[-2].StringValue), NULL);
                  }
#line 1841 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 392 "work_good.y" /* yacc.c:1646  */
    {
                    (yyval.var) = addNewFunction(NULL, (yyvsp[-3].StringValue), (yyvsp[-1].vars));
                  }
#line 1849 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 395 "work_good.y" /* yacc.c:1646  */
    {
                    (yyval.var) = addNewFunction(NULL, (yyvsp[-2].StringValue), NULL);
                  }
#line 1857 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 400 "work_good.y" /* yacc.c:1646  */
    {
                (yyvsp[0].var)->type = (yyvsp[-1].StringValue);
                (yyval.vars) = newList((yyvsp[0].var)); //lista de declaratii
              }
#line 1866 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 404 "work_good.y" /* yacc.c:1646  */
    {
                (yyvsp[0].var)->type = (yyvsp[-1].StringValue);
                (yyval.vars) = pushBack((yyvsp[-3].vars), (yyvsp[0].var)); //lista de declaratii
              }
#line 1875 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 408 "work_good.y" /* yacc.c:1646  */
    {
                (yyvsp[0].var)->type = (yyvsp[-1].StringValue);
                (yyval.vars) = newList((yyvsp[0].var)); //lista de declaratii
              }
#line 1884 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 413 "work_good.y" /* yacc.c:1646  */
    {
                (yyvsp[0].var)->type = (yyvsp[-1].StringValue);
                (yyval.vars) = pushBack((yyvsp[-4].vars), (yyvsp[0].var)); //lista de declaratii
              }
#line 1893 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 418 "work_good.y" /* yacc.c:1646  */
    {
                    struct declaration *strct = findStruct((yyvsp[-1].StringValue));                    
                    setStructSingle(strct, (yyvsp[0].var));
                    (yyval.vars) = newList((yyvsp[0].var)); //lista de declaratii
                }
#line 1903 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 424 "work_good.y" /* yacc.c:1646  */
    { 
                  struct declaration *strct = findStruct((yyvsp[-1].StringValue));
                  setStructSingle(strct, (yyvsp[0].var));
                  (yyval.vars) = pushBack((yyvsp[-3].vars), (yyvsp[0].var)); //lista de declaratii
                }
#line 1913 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 430 "work_good.y" /* yacc.c:1646  */
    { 
                  struct declaration *strct = findStruct((yyvsp[-1].StringValue));
                  setStructSingle(strct, (yyvsp[0].var));
                  (yyval.vars) = newList((yyvsp[0].var)); //lista de declaratii
              }
#line 1923 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 436 "work_good.y" /* yacc.c:1646  */
    { 
                  struct declaration *strct = findStruct((yyvsp[-1].StringValue)); 
                  setStructSingle(strct, (yyvsp[0].var));
                  (yyval.vars) = pushBack((yyvsp[-4].vars), (yyvsp[0].var)); //lista de declaratii  
              }
#line 1933 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 448 "work_good.y" /* yacc.c:1646  */
    {
                if(compare((yyvsp[-2].var)->type, "int") == 0) {
                    printf("%d \n", (yyvsp[-2].var)->intValue);
                }
                if(compare((yyvsp[-2].var)->type, "float") == 0) {
                    printf("%f \n", (yyvsp[-2].var)->floatValue);
                }
                
                /*if(compare($3->type, "char") == 0 || compare($3->type, "string") == 0) {
                    yyerror("The types of the print parameters are not numeric!!!");
                }*/
}
#line 1950 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 477 "work_good.y" /* yacc.c:1646  */
    {
                (yyval.var) = newDeclaration("int", NULL, false);
                (yyval.var)->isInitialized = true;
                (yyval.var)->intValue = (yyvsp[0].intNumberValue);
                
             }
#line 1961 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 483 "work_good.y" /* yacc.c:1646  */
    {
                (yyval.var) = newDeclaration("float", NULL, false);
                (yyval.var)->isInitialized = true;
                (yyval.var)->floatValue = (yyvsp[0].floatNumberValue);
             }
#line 1971 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 488 "work_good.y" /* yacc.c:1646  */
    {
                (yyval.var) = newDeclaration("char", NULL, false);
                (yyval.var)->isInitialized = true;
             }
#line 1980 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 492 "work_good.y" /* yacc.c:1646  */
    {
                (yyval.var) = newDeclaration("string", NULL, false);
                (yyval.var)->isInitialized = true;
             }
#line 1989 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 498 "work_good.y" /* yacc.c:1646  */
    { (yyval.var) = findIdentifier((yyvsp[0].var)->name); }
#line 1995 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 499 "work_good.y" /* yacc.c:1646  */
    { (yyval.var) = getMember(findIdentifier((yyvsp[-2].var)->name), (yyvsp[0].var)); }
#line 2001 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 500 "work_good.y" /* yacc.c:1646  */
    { (yyval.var) = (yyvsp[0].var); }
#line 2007 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 501 "work_good.y" /* yacc.c:1646  */
    { (yyval.var) = (yyvsp[0].var); }
#line 2013 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 502 "work_good.y" /* yacc.c:1646  */
    {
        (yyval.var) = cloneVariable((yyvsp[-2].var));
        handleError(addOperation((yyval.var), (yyvsp[0].var)));
     }
#line 2022 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 506 "work_good.y" /* yacc.c:1646  */
    { 
        (yyval.var) = cloneVariable((yyvsp[-2].var));
        handleError(subOperation((yyval.var), (yyvsp[0].var)));
     }
#line 2031 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 510 "work_good.y" /* yacc.c:1646  */
    { 
        (yyval.var) = cloneVariable((yyvsp[-2].var));
        handleError(mulOperation((yyval.var), (yyvsp[0].var)));
     }
#line 2040 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 514 "work_good.y" /* yacc.c:1646  */
    { 
        (yyval.var) = cloneVariable((yyvsp[-2].var));
        handleError(divOperation((yyval.var), (yyvsp[0].var)));
     }
#line 2049 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 518 "work_good.y" /* yacc.c:1646  */
    { 
        (yyval.var) = cloneVariable((yyvsp[-2].var));
        handleError(modOperation((yyval.var), (yyvsp[0].var)));
     }
#line 2058 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 522 "work_good.y" /* yacc.c:1646  */
    { (yyval.var) = (yyvsp[-1].var); }
#line 2064 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 525 "work_good.y" /* yacc.c:1646  */
    { (yyval.var) = findIdentifier((yyvsp[-2].var)->name);
                                       handleError(copyTo((yyval.var), (yyvsp[0].var)));
                                     }
#line 2072 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 528 "work_good.y" /* yacc.c:1646  */
    { (yyval.var) = getMember(findIdentifier((yyvsp[-4].var)->name), (yyvsp[-2].var));
                                       handleError(copyTo((yyval.var), (yyvsp[0].var)));
                                     }
#line 2080 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 533 "work_good.y" /* yacc.c:1646  */
    {
                handleError(addOperation(findIdentifier((yyvsp[-2].StringValue)), (yyvsp[0].var)));
              }
#line 2088 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 536 "work_good.y" /* yacc.c:1646  */
    {
                handleError(subOperation(findIdentifier((yyvsp[-2].StringValue)), (yyvsp[0].var)));
              }
#line 2096 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 539 "work_good.y" /* yacc.c:1646  */
    {
                handleError(mulOperation(findIdentifier((yyvsp[-2].StringValue)), (yyvsp[0].var)));
              }
#line 2104 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 542 "work_good.y" /* yacc.c:1646  */
    {
                handleError(divOperation(findIdentifier((yyvsp[-2].StringValue)), (yyvsp[0].var)));  
              }
#line 2112 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 545 "work_good.y" /* yacc.c:1646  */
    {
                handleError(addOperation(findIdentifier((yyvsp[-2].StringValue)), (yyvsp[0].var)));
              }
#line 2120 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 556 "work_good.y" /* yacc.c:1646  */
    { (yyval.vars) = newList((yyvsp[0].var)); }
#line 2126 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 557 "work_good.y" /* yacc.c:1646  */
    { (yyval.vars) = pushBack((yyvsp[-2].vars), (yyvsp[0].var)); }
#line 2132 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 561 "work_good.y" /* yacc.c:1646  */
    { (yyval.var) = findFunction((yyvsp[-2].StringValue)); }
#line 2138 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 562 "work_good.y" /* yacc.c:1646  */
    {  //here we check if the functionCallParameters have the same type with the declFunctionParameters
                (yyval.var) = findFunction((yyvsp[-3].StringValue));
                struct list *first, *second;
                for(first = (yyval.var)->vars->head, second =(yyvsp[-1].vars)->head; first != NULL && second != NULL; first = first->next, second = second->next) {
                    if(compare((yyval.var)->vars->value->type, (yyvsp[-1].vars)->value->type) != 0) {
                        yyerror("Different parameters type!!!");
                    }     
                }
                
                /*if (first != second) {
                        yyerror("Too many parameters!");
                }*/
                
                }
#line 2157 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 582 "work_good.y" /* yacc.c:1646  */
    {
            (yyval.var) = newDeclaration("bool", NULL, false);
            (yyval.var)->isInitialized = true;
            (yyval.var)->intValue = 1;
        }
#line 2167 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 587 "work_good.y" /* yacc.c:1646  */
    {
            (yyval.var) = newDeclaration("bool", NULL, false);
            (yyval.var)->isInitialized = true;
            (yyval.var)->intValue = 0;
        }
#line 2177 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 592 "work_good.y" /* yacc.c:1646  */
    {(yyval.var) = (yyvsp[-1].var);}
#line 2183 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 593 "work_good.y" /* yacc.c:1646  */
    {(yyval.var) = (yyvsp[-2].var);}
#line 2189 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 595 "work_good.y" /* yacc.c:1646  */
    { (yyval.var) = (yyvsp[0].var); }
#line 2195 "work_good.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 596 "work_good.y" /* yacc.c:1646  */
    { (yyval.var) = (yyvsp[0].var); }
#line 2201 "work_good.tab.c" /* yacc.c:1646  */
    break;


#line 2205 "work_good.tab.c" /* yacc.c:1646  */
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

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

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
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

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
  return yyresult;
}
#line 629 "work_good.y" /* yacc.c:1906  */


int main(int argc, char** argv){
  yyin=fopen(argv[1],"r");
  yyparse();
}
