#include <cstdio>

extern char *yytext;
extern int yyleng;

/* The file from which lex will read input tokens (default is stdin). */
extern FILE *yyin;

/* Function to set yyin to another file */
void yyrestart( FILE *new_file );

/* The lexing function generated by lex: a token should be lexed with
   each call and its id returned. */
extern int yylex();
