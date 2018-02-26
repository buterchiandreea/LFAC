/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

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

#ifndef YY_YY_WORK_GOOD_TAB_H_INCLUDED
# define YY_YY_WORK_GOOD_TAB_H_INCLUDED
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
#line 206 "work_good.y" /* yacc.c:1909  */
 float floatNumberValue; int intNumberValue; char *StringValue; struct declaration *var; struct list *vars; 

#line 91 "work_good.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_WORK_GOOD_TAB_H_INCLUDED  */
