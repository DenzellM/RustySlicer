%{
#include <stdio.h>

#include "lex.yy.c"
#include "expStructs.h"
#include "statementStructs.h"
#include "program.h"



void yyerror(const char* str);
%}

%define api.pure

%union{
double num;
bool boolean;   
char *id;
struct Exp_ *exp;
struct funcParam *param;
struct funcParamList *paramList;
struct ExpList *callParamList;
struct Stm_ *stm;
struct ItemStm *prog;
}
//most of these are guessed and could be wrong
%token PRINTVAR 
%token PRINTSTRING

%token ARROW  // ->

%token COLON  // :
%token LET 
%left IF 
%token ELSE 
%token WHILE 
%token MUT
%token COM
%token TYPE
%left IDENTIFIER 
%token DEC_LITERAL 
%token TRUE
%token FALSE 
%token FN 

%token CONTINUE
%token BREAK
%token RETURN


%left EQ  // =
%right PLUSEQ  // +=
%right MINUSEQ // -=
%right ASSIGN

%left OROR  // ||
%left ANDAND  // &&

%left GT  // >
%left GE  // >=
%left LT  // <
%left LE  // <=
%left EQEQ  // ==
%left NE  // !=

%left AMPERSAND  // &

%left PLUS 
%left MINUS 

%left STAR  // * gebruikt voor vermenigvuldiging en dereference
%left SLASH 


%right UNARY
%right NOT  // !



%left SEMICOLON  // ;
%right LBRACE  // {
%token RBRACE 
%right LPAREN  // (
%token RPAREN 

%type <callParamList> callParams
%type <exp> callExp


%type <exp> idExp
%type <exp> derefExp
%type <exp> boolOpExp
%type <exp> negExp
%type <exp> arithExp
%type <exp> compOp
%type <exp> expr
%type <exp> litExp
%type <exp> groupExp
%type <exp> opValueDecl
%type <exp> patNTA
%type <exp> ifExpr
%type <exp> optElseExpr

%type <exp> opExp
%type <exp> exprWOBlock
%type <exp> loopExpr
%type <exp> exprWBlock
%type <exp> borrowExp


%type <exp> blockExpr
%type <exp> funcBody


%type <exp> assExp
%type <exp> compAssExp

%type <stm> stmnts
%type <stm> macro

%type <stm> stmnt
%type <stm> letStmn
%type <stm> contExp
%type <stm> breakExp
%type <stm> returnExp


%type <stm> loopStmnt

%type <stm> func
%type <stm> item
%type <prog> items

%type <id> opTypeDecl //its just a string
%type <id> returnType //its just a string
%type <id> typePath //its just a string
%type <id> typeNB //its just a string
%type <id> refType //its just a string
%type <id> identifier //its just a string



%type <boolean> optMut
%type <param> funcParam
%type <paramList> funcParams

%start program
%defines

%%
program: items {Program p($1); p.execute();}

items:    {$$ = nullptr;}
        | item items {$$ = new ItemStm($1, $2);}
        ;

item:   func {$$ = $1;};

//checked: 6
derefExp: STAR %prec UNARY expr {$$ = new derefExp($2);};

//checked: 6
borrowExp:    AMPERSAND %prec UNARY  optMut expr    {$$ = new borrowExp($3, $2);}
            | ANDAND %prec UNARY optMut expr        {$$ = new borrowExp(new borrowExp($3, $2), $2);}
            ;
 //checked: 8           
negExp:   MINUS %prec UNARY expr    {$$ = new NegExp($2, Type::Double);}
        | NOT %prec UNARY expr      {$$ = new NegExp($2, Type::Bool);}
        ;
//checked: 2
arithExp:     expr PLUS expr    {$$ = new OpExp($1, Plus, $3);}
            | expr MINUS expr   {$$ = new OpExp($1, Minus, $3);}
            | expr STAR expr    {$$ = new OpExp($1, Times, $3);}
            | expr SLASH expr   {$$ = new OpExp($1, Div, $3);}
            ;

//checked: 8
boolOpExp:    expr ANDAND expr  {$$ = new BoolOpExp($1, And, $3);}
            | expr OROR expr    {$$ = new BoolOpExp($1, Or, $3);};
            ;

//checked: 8
compOp:   expr EQEQ expr    {$$ = new compOpExp($1, Eq, $3);}
        | expr NE expr      {$$ = new compOpExp($1, Ne, $3);}
        | expr GT expr      {$$ = new compOpExp($1, Gt, $3);}
        | expr LT expr      {$$ = new compOpExp($1, Lt, $3);}
        | expr GE expr      {$$ = new compOpExp($1, Ge, $3);}
        | expr LE expr      {$$ = new compOpExp($1, Le, $3);}
        ;

//checked: trivial
groupExp: LPAREN expr RPAREN {$$ = $2;};

//checked: 4
callParams: {$$ = new ExpList();}
            | expr COM callParams   {$$ = new ExpList($1, $3);}
            | expr                  {$$ = new ExpList($1);};
//checked: 4
callExp:  groupExp LPAREN callParams RPAREN {$$ = new funCallExp($1, $3);}
        | idExp LPAREN callParams RPAREN {$$ = new funCallExp($1, $3);};

//checked: 8
breakExp: BREAK {$$ = new BreakStm();};

//checked: 7
returnExp:    RETURN expr {$$ = new ReturnStm($2);}
            | RETURN  {$$ = new ReturnStm(nullptr);}
            ;

//==========================================================================

//checked: trivial
idExp: IDENTIFIER {$$ = new IdExp(yylval.id);};

//checked: trivial
optMut: {$$ = false;} | MUT {$$ = true;};


//checked: trivial
opExp:    negExp        {$$ = $1;}
        | arithExp      {$$ = $1;}
        | boolOpExp     {$$ = $1;}
        | compOp        {$$ = $1;}
        | compAssExp    {$$ = $1;}
        | assExp        {$$ = $1;}
        | borrowExp     {$$ = $1;}
        | derefExp      {$$ = $1;}
        ;
//checked: 8
contExp: CONTINUE {$$ = new ContStm();};

//checked: trivial
exprWOBlock:  idExp     {$$ = $1;}
            | litExp    {$$ = $1;}
            | opExp     {$$ = $1;}
            | groupExp  {$$ = $1;}
            | callExp   {$$ = $1;}
            ;

//checked: trivial
loopStmnt:    contExp   {$$ = $1;}
            | returnExp {$$ = $1;}
            | breakExp  {$$ = $1;}
            ;

//checked: trivial
letStmn:    LET optMut identifier opTypeDecl opValueDecl SEMICOLON {$$ = new LetStm($2, $3, $4, $5);}
            ;

//checked: trivial
assExp: expr EQ expr {$$ = new AssExp($1, $3);};

//checked: 1
compAssExp:   expr PLUSEQ expr  {$$ = new CompAssExp($1, $3, CompOp::PlusEq);}
            | expr MINUSEQ expr {$$ = new CompAssExp($1, $3, CompOp::MinEq);}
            | expr STAR EQ %prec ASSIGN expr    {$$ = new CompAssExp($1, $4, CompOp::TimeEq);}
            | expr SLASH EQ %prec ASSIGN expr   {$$ = new CompAssExp($1, $4, CompOp::DivEq);}
            ;

//checked: trivial    
opTypeDecl:   {$$ = "None";}
            | COLON TYPE {$$ = yylval.id;}
            ;

//checked: trivial
opValueDecl:      {$$ = nullptr;}
                | EQ expr {$$ = $2;}
                ;

//checked: trivial
returnType:   {$$ = "None";}
            | ARROW TYPE {$$ = yylval.id;}
            ;

//checked: trivial
patNTA:   idExp {$$ = $1;}
        | litExp{$$ = $1;}
        ;

//checked: trivial
typePath: TYPE {$$ = yylval.id;};

//checked: 6
refType:      AMPERSAND optMut typeNB {char* mut = ""; if($2) mut = "mut"; char out[50] = ""; strcat(out , "&"); strcat(out, mut); strcat(out, $3); strcpy($$, out);};

//checked: trivial
typeNB:   typePath {$$ = $1;}
        | refType {$$ = $1;}
        ;

//checked: trivial
funcParam:  patNTA COLON typeNB {$$ = new funcParam($1, $3);};

//checked: trivial
funcParams:   {$$ = new funcParamList();}
            | funcParam COM funcParams {$$ = new funcParamList($1, $3);}
            | funcParam {$$ = new funcParamList($1);}
            ;

//checked: trivial
funcBody:     blockExpr {$$ = $1;}
            | SEMICOLON {$$ = nullptr;}
            ;

//checked: trivial
identifier: IDENTIFIER{$$ = yylval.id;};

//checked: trivial
func:   FN identifier LPAREN funcParams RPAREN returnType funcBody {$$ = new funcStm($2, $4, $6, $7);};

//checked: trivial
macro:  PRINTVAR {$$ = new PrintVarStm(yylval.id);} | PRINTSTRING {$$ = new PrintStringStm(yylval.id);};


//checked: 5            
blockExpr:    LBRACE stmnts exprWOBlock RBRACE {$$ = new BlockExp($2, $3);}
            | LBRACE exprWOBlock RBRACE {$$ = new BlockExp(nullptr, $2);}
            | LBRACE stmnts RBRACE {$$ = new BlockExp($2, nullptr);}
            | LBRACE RBRACE {$$ = new BlockExp(nullptr, nullptr);}
            ;

//checked: trivial
stmnts:   stmnt         {$$ = $1;} //collection
        | stmnts stmnt   {$$ = new CompStm($1, $2);}
        ;


//checked: trivial
exprWBlock:   loopExpr  {$$ = $1;}
            | ifExpr    {$$ = $1;}
            | blockExpr {$$ = $1;}
            ;

//checked: trivial          
expr:     exprWBlock {$$ = $1;}
        | exprWOBlock {$$ = $1;}
        ;

//checked: 7
ifExpr:   IF expr blockExpr optElseExpr {$$ = new ifExp($2, $3, $4);};

//checked: 7
optElseExpr:    {$$ = nullptr;}
                | ELSE blockExpr {$$ = $2;}
                | ELSE ifExpr{$$ = $2;}
                ;

//checked: trivial
stmnt:    SEMICOLON {$$ = nullptr;}
        | letStmn   {$$ = $1;}
        | expr SEMICOLON {$$ = new expStm($1);}
        | exprWBlock {$$ = new expStm($1);}
        | item          {$$ = $1;}
        | macro         {$$ = $1;}
        | loopStmnt     {$$ = $1;}
        ;

//checked: 7
loopExpr: WHILE expr blockExpr {$$ = new whileExp($2, $3);};

//checked: trivial
litExp:   TRUE {$$ = new BoolExp(true); } 
        | FALSE {$$ = new BoolExp(false);}
        | DEC_LITERAL {$$ = new NumExp(yylval.num);}
        ;

%%
void yyerror (const char *s)
{
    printf("Compiler error\n");
    return;
}
