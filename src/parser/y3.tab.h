/* A Bison parser, made by GNU Bison 3.7.4.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_SRC_PARSER_Y3_TAB_H_INCLUDED
# define YY_YY_SRC_PARSER_Y3_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    PRINTVAR = 258,                /* PRINTVAR  */
    PRINTSTRING = 259,             /* PRINTSTRING  */
    LET = 260,                     /* LET  */
    IF = 261,                      /* IF  */
    ELSE = 262,                    /* ELSE  */
    WHILE = 263,                   /* WHILE  */
    MUT = 264,                     /* MUT  */
    COM = 265,                     /* COM  */
    TYPE = 266,                    /* TYPE  */
    FALSE = 267,                   /* FALSE  */
    TRUE = 268,                    /* TRUE  */
    FN = 269,                      /* FN  */
    CONTINUE = 270,                /* CONTINUE  */
    BREAK = 271,                   /* BREAK  */
    RETURN = 272,                  /* RETURN  */
    GT = 273,                      /* GT  */
    LT = 274,                      /* LT  */
    EQ = 275,                      /* EQ  */
    EQEQ = 276,                    /* EQEQ  */
    NE = 277,                      /* NE  */
    OROR = 278,                    /* OROR  */
    ANDAND = 279,                  /* ANDAND  */
    LE = 280,                      /* LE  */
    GE = 281,                      /* GE  */
    PLUSEQ = 282,                  /* PLUSEQ  */
    MINUSEQ = 283,                 /* MINUSEQ  */
    ARROW = 284,                   /* ARROW  */
    IDENTIFIER = 285,              /* IDENTIFIER  */
    DEC_LITERAL = 286,             /* DEC_LITERAL  */
    COLON = 287,                   /* COLON  */
    AMPERSAND = 288,               /* AMPERSAND  */
    PLUS = 289,                    /* PLUS  */
    MINUS = 290,                   /* MINUS  */
    STAR = 291,                    /* STAR  */
    SLASH = 292,                   /* SLASH  */
    NOT = 293,                     /* NOT  */
    SEMICOLON = 294,               /* SEMICOLON  */
    LBRACE = 295,                  /* LBRACE  */
    RBRACE = 296,                  /* RBRACE  */
    LPAREN = 297,                  /* LPAREN  */
    RPAREN = 298,                  /* RPAREN  */
    UNARY = 299                    /* UNARY  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 16 "src/parser/y-exp.y"

double num;
bool boolean;   
char *id;
struct Exp_ *exp;
struct funcParam *param;
struct funcParamList *paramList;
struct ExpList *callParamList;
struct ItemExp *prog;

#line 119 "src/parser/y3.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int yyparse (void);

#endif /* !YY_YY_SRC_PARSER_Y3_TAB_H_INCLUDED  */
