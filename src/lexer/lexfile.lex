%{
/* 
go.lex: lex-file for go basisniveau
*/

#include "y3.tab.h"


  /* Keep track of current position of lex for error messages, i.e. 
     the position just *after* the last token read */
  int line_nr = 1;
  int col_nr = 1;

%}

identifier [a-zA-Z]([a-zA-Z0-9])*
dec_lit [0-9](_|[0-9])*\.?([0-9](_|[0-9])*)?
comment ("//".*"\n")
%%
\n            {line_nr++; col_nr = 1;}
{comment}     {line_nr++; col_nr = 1;}
[ \t]         {col_nr++;}
true          {col_nr += strlen(yytext); return TRUE;}
false         {col_nr += strlen(yytext); return FALSE;}
fn            {col_nr += strlen(yytext); return FN;}
\(            {col_nr += strlen(yytext); return LPAREN;}
\)            {col_nr += strlen(yytext); return RPAREN;}
->            {col_nr += strlen(yytext); return ARROW;}
\{            {col_nr += strlen(yytext); return LBRACE;}
\}            {col_nr += strlen(yytext); return RBRACE;}
;             {col_nr += strlen(yytext); return SEMICOLON;}
:             {col_nr += strlen(yytext); return COLON;}
\,            {col_nr += strlen(yytext); return COM;}
let           {col_nr += strlen(yytext); return LET;}
\=            {col_nr += strlen(yytext); return EQ;}
\+            {col_nr += strlen(yytext); return PLUS;}
\-            {col_nr += strlen(yytext); return MINUS;}
\*            {col_nr += strlen(yytext); return STAR;}
\/            {col_nr += strlen(yytext); return SLASH;}
\+\=          {col_nr += strlen(yytext); return PLUSEQ;}
\-\=          {col_nr += strlen(yytext); return MINUSEQ;}
\&\&          {col_nr += strlen(yytext); return ANDAND;}
\|\|          {col_nr += strlen(yytext); return OROR;}
\!            {col_nr += strlen(yytext); return NOT;}
\>            {col_nr += strlen(yytext); return GT;}
\>\=          {col_nr += strlen(yytext); return GE;}
\<            {col_nr += strlen(yytext); return LT;}
\<\=          {col_nr += strlen(yytext); return LE;}
\=\=          {col_nr += strlen(yytext); return EQEQ;}
\!\=          {col_nr += strlen(yytext); return NE;}
\&            {col_nr += strlen(yytext); return AMPERSAND;}
if            {col_nr += strlen(yytext); return IF;}
else          {col_nr += strlen(yytext); return ELSE;}
while         {col_nr += strlen(yytext); return WHILE;}
mut           {col_nr += strlen(yytext); return MUT;}
continue      {col_nr += strlen(yytext); return CONTINUE;}
break         {col_nr += strlen(yytext); return BREAK;}
return        {col_nr += strlen(yytext); return RETURN;}

println!\(\"[a-zA-Z0-9_]+\"\)         {col_nr += strlen(yytext); yylval->id = strdup(yytext); return PRINTSTRING;}
println!\(\"\{[a-zA-Z0-9_]+\}\"\)   {col_nr += strlen(yytext); yylval->id = strdup(yytext); return PRINTVAR;}

bool          {col_nr += strlen(yytext); yylval->id = strdup(yytext); return TYPE;}
i32           {col_nr += strlen(yytext); yylval->id = strdup(yytext); return TYPE;}
float         {col_nr += strlen(yytext); yylval->id = strdup(yytext); return TYPE;}

{dec_lit}     {col_nr += strlen(yytext); yylval->num = strtod(yytext, NULL); return DEC_LITERAL;}
{identifier}  {col_nr += strlen(yytext); yylval->id = strdup(yytext); return IDENTIFIER;}





.      {
  if (yytext[0] < ' '){ /* non-printable char */
    /*yyerror*/ fprintf(stderr,"illegal character: ^%c",yytext[0] + '@'); 
  }
  else {
    if (yytext[0] > '~') {
      /* non-printable char printed as octal int padded with zeros, eg \012*/
      /*yyerror(*/fprintf(stderr,"illegal character: \\%03o", (int) yytext[0]);
    }
    else {
      /*yyerror(*/fprintf(stderr,"illegal character: %s",yytext);
    }
  }
  /* lex read exactly one char; the illegal one */
  fprintf(stderr," at line %d column %d\n", line_nr, (col_nr-1));
			       }
%%

/* Function called by (f)lex when EOF is read. If yywrap returns a
   true (non-zero) (f)lex will terminate and continue otherwise.*/
int yywrap(){
  return (1);
}