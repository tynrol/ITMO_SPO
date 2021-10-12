/* A Bison parser, made by GNU Bison 3.7.5.  */

/* Bison interface for Yacc-like parsers in C

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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
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
    LINE = 258,                    /* LINE  */
    SELECT = 259,                  /* SELECT  */
    INSERT = 260,                  /* INSERT  */
    UPDATE = 261,                  /* UPDATE  */
    CREATE = 262,                  /* CREATE  */
    DELETE = 263,                  /* DELETE  */
    FROM = 264,                    /* FROM  */
    IDENTIFIER = 265,              /* IDENTIFIER  */
    IDENTIFIERS = 266,             /* IDENTIFIERS  */
    WHERE = 267,                   /* WHERE  */
    AND = 268,                     /* AND  */
    VALUES = 269,                  /* VALUES  */
    INTO = 270,                    /* INTO  */
    SET = 271,                     /* SET  */
    TABLE = 272,                   /* TABLE  */
    STATEMENT = 273,               /* STATEMENT  */
    TABLE_DEF = 274,               /* TABLE_DEF  */
    ALL = 275,                     /* ALL  */
    TEXT_TYPE = 276,               /* TEXT_TYPE  */
    NUMBER_TYPE = 277,             /* NUMBER_TYPE  */
    COLUMN_NAME = 278,             /* COLUMN_NAME  */
    SET_VALUE = 279,               /* SET_VALUE  */
    SET_VALUES = 280,              /* SET_VALUES  */
    CONDITION = 281,               /* CONDITION  */
    COLUMN_TYPE = 282,             /* COLUMN_TYPE  */
    ITEMS = 283,                   /* ITEMS  */
    COLUMNS = 284,                 /* COLUMNS  */
    CMD = 285                      /* CMD  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define LINE 258
#define SELECT 259
#define INSERT 260
#define UPDATE 261
#define CREATE 262
#define DELETE 263
#define FROM 264
#define IDENTIFIER 265
#define IDENTIFIERS 266
#define WHERE 267
#define AND 268
#define VALUES 269
#define INTO 270
#define SET 271
#define TABLE 272
#define STATEMENT 273
#define TABLE_DEF 274
#define ALL 275
#define TEXT_TYPE 276
#define NUMBER_TYPE 277
#define COLUMN_NAME 278
#define SET_VALUE 279
#define SET_VALUES 280
#define CONDITION 281
#define COLUMN_TYPE 282
#define ITEMS 283
#define COLUMNS 284
#define CMD 285

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 40 "sql_parser.yacc"

    struct ast_node * node;
    int number;
    char *text;

#line 133 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
