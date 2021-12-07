/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_SPL_TAB_HPP_INCLUDED
# define YY_YY_SPL_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 6 "spl.ypp"

#include "splc.hpp"
#include "ast.hpp"

// definitions needed from flex
int yylex(void); // get the next token
int yylex_destroy(); // deallocate flex stuff
extern FILE* yyin; // file that Flex reads from

// this function is called by Bison when there is a parsing error
void yyerror(const char *p);


#line 62 "spl.tab.hpp"

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    BOP = 258,
    NOTTOK = 259,
    COMP = 260,
    OPA = 261,
    OPM = 262,
    POSNEG = 263,
    FUNARG = 264,
    LC = 265,
    RC = 266,
    LP = 267,
    RP = 268,
    LAMBDA = 269,
    IF = 270,
    IFELSE = 271,
    WHILE = 272,
    READ = 273,
    WRITE = 274,
    NEW = 275,
    ASN = 276,
    STOP = 277,
    ID = 278,
    NUM = 279,
    BOOL = 280,
    DEBUG = 281
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 23 "spl.ypp"

  Block* block;
  Stmt* stmt;
  Exp* exp;
  Id* id;
  Oper op;

#line 108 "spl.tab.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SPL_TAB_HPP_INCLUDED  */
