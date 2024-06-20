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
struct ItemExp *prog;
}

//keywords
%token PRINTVAR 
%token PRINTSTRING
%token LET 
%token IF 
%token ELSE 
%token WHILE 
%token MUT
%token COM
%token TYPE
%token FALSE 
%token TRUE
%token FN 
%token CONTINUE
%token BREAK
%token RETURN

//symbols
%token GT  // >
%token LT  // <
%token EQ  // =
%token EQEQ  // ==
%token NE  // !=
%token OROR  // ||
%token ANDAND  // &&
%token LE  // <=
%token GE  // >=
%token PLUSEQ  // +=
%token MINUSEQ // -=
%token ARROW  // ->
%token IDENTIFIER 
%token DEC_LITERAL 
%token COLON  // :
%token AMPERSAND  // &
%token PLUS 
%token MINUS 
%token STAR  // * gebruikt voor vermenigvuldiging en dereference
%token SLASH 
%token NOT  // !
%token SEMICOLON  // ;
%token LBRACE  // {
%token RBRACE 
%token LPAREN  // (
%token RPAREN 

%precedence MUT
%precedence IDENTIFIER
%precedence ARROW COLON
%right EQ MINUSEQ PLUSEQ 
%left OROR ANDAND

%left EQEQ NE
%left LT GT LE GE
%left PLUS MINUS
%left STAR SLASH
%precedence NOT UNARY
%precedence LBRACE LPAREN

%type <callParamList> callParams
%type <exp> callExp

%type <exp> idExp
%type <exp> expr
%type <exp> litExp
%type <exp> groupExp
%type <exp> opValueDecl
%type <exp> patNTA
%type <exp> ifExpr
%type <exp> blockOrIf

%type <exp> opExp_B
%type <exp> arithExp_B
%type <exp> compOp_B
%type <exp> boolOpExp_B
%type <exp> assExp_B
%type <exp> compAssExp_B
%type <exp> negExp_B
%type <exp> derefExp_B
%type <exp> borrowExp_B

%type <exp> exprWOBlock
%type <exp> loopExpr
%type <exp> exprWBlock

%type <exp> blockExpr
%type <exp> funcBody

%type <exp> stmnts
%type <exp> macro

%type <exp> stmnt
%type <exp> letExpn
%type <exp> contExp
%type <exp> breakExp
%type <exp> returnExp


%type <exp> loopExpnt

%type <exp> func
%type <exp> item
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
        | item items {$$ = new ItemExp($1, $2);}
        ;

item:   func {$$ = $1;};

//checked: 6
derefExp_B: STAR expr %prec UNARY {$$ = new derefExp($2);};

//checked: 6
borrowExp_B:      AMPERSAND optMut expr %prec UNARY    {$$ = new borrowExp($3, $2);}
                | ANDAND  optMut expr %prec UNARY      {$$ = new borrowExp(new borrowExp($3, $2), $2);}
                ;

 //checked: 8           
negExp_B:     MINUS expr %prec UNARY   {$$ = new NegExp($2, Type::Double);}
            | NOT expr %prec UNARY     {$$ = new NegExp($2, Type::Bool);}
            ;

//checked: 2
arithExp_B:   expr PLUS expr    {$$ = new OpExp($1, Plus, $3);}
            | expr MINUS expr   {$$ = new OpExp($1, Minus, $3);}
            | expr STAR expr    {$$ = new OpExp($1, Times, $3);}
            | expr SLASH expr   {$$ = new OpExp($1, Div, $3);}
            ;

//checked: 8
boolOpExp_B:  expr ANDAND expr  {$$ = new BoolOpExp($1, And, $3);}
            | expr OROR expr    {$$ = new BoolOpExp($1, Or, $3);};
            ;

//checked: 8
compOp_B:     expr EQEQ expr    {$$ = new compOpExp($1, Eq, $3);}
            | expr NE expr      {$$ = new compOpExp($1, Ne, $3);}
            | expr GT expr      {$$ = new compOpExp($1, Gt, $3);}
            | expr LT expr      {$$ = new compOpExp($1, Lt, $3);}
            | expr GE expr      {$$ = new compOpExp($1, Ge, $3);}
            | expr LE expr      {$$ = new compOpExp($1, Le, $3);}
            ;
//checked: trivial
assExp_B: expr EQ expr {$$ = new AssExp($1, $3);};

//checked: 1
compAssExp_B: expr PLUSEQ expr   {$$ = new CompAssExp($1, $3, CompOp::PlusEq);}
            | expr MINUSEQ expr  {$$ = new CompAssExp($1, $3, CompOp::MinEq);}
            | expr STAR EQ expr  {$$ = new CompAssExp($1, $4, CompOp::TimeEq);}
            | expr SLASH EQ expr {$$ = new CompAssExp($1, $4, CompOp::DivEq);}
            ;
//checked: trivial
groupExp: LPAREN expr RPAREN {$$ = $2;};

//checked: 4
callParams:                         {$$ = new ExpList();}
            | expr COM callParams   {$$ = new ExpList($1, $3);}
            | expr                  {$$ = new ExpList($1);};
//checked: 4
callExp:    exprWOBlock LPAREN callParams RPAREN {$$ = new funCallExp($1, $3);};

//checked: 8
breakExp: BREAK {$$ = new BreakExp();};

//checked: trivial
idExp: IDENTIFIER {$$ = new IdExp(yylval.id);};

//checked: 7
returnExp:    
              RETURN expr %prec UNARY{$$ = new ReturnExp($2);}
            |  RETURN  {$$ = new ReturnExp(nullptr);}
            ;

//checked: trivial
optMut: {$$ = false;} | MUT {$$ = true;};


//checked: trivial
opExp_B:  negExp_B        {$$ = $1;}
        | derefExp_B      {$$ = $1;}
        | arithExp_B      {$$ = $1;}
        | boolOpExp_B     {$$ = $1;}
        | compOp_B        {$$ = $1;}
        | compAssExp_B    {$$ = $1;}
        | assExp_B        {$$ = $1;}
        | borrowExp_B     {$$ = $1;}
        ;
//checked: 8
contExp: CONTINUE {$$ = new ContExp();};

//checked: trivial          
expr:     exprWBlock {$$ = $1;}
        | exprWOBlock{$$ = $1;}
        | stmnt {$$ = $1;}
        ;

//checked: trivial
exprWOBlock:  idExp     {$$ = $1;}
            | opExp_B   {$$ = $1;}
            | litExp    {$$ = $1;}
            | groupExp  {$$ = $1;}
            | callExp   {$$ = $1;}
            ;

//checked: trivial
loopExpnt:    contExp   {$$ = $1;}
            | returnExp {$$ = $1;}
            | breakExp  {$$ = $1;}
            ;

//checked: trivial
letExpn:    LET optMut identifier opTypeDecl opValueDecl SEMICOLON {$$ = new LetExp($2, $3, $4, $5);}
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
func:   FN identifier LPAREN funcParams RPAREN returnType funcBody {$$ = new funcExp($2, $4, $6, $7);};

//checked: trivial
macro:  PRINTVAR{$$ = new PrintVarExp(yylval.id);} | PRINTSTRING{$$ = new PrintStringExp(yylval.id);};


//checked: 5            
blockExpr:    LBRACE expr RBRACE        {$$ = new BlockExp(nullptr, $2);}
            | LBRACE stmnt RBRACE       {$$ = new BlockExp($2, nullptr);}
            | LBRACE stmnts expr RBRACE {$$ = new BlockExp($2, $3);}
            | LBRACE RBRACE             {$$ = new BlockExp(nullptr, nullptr);}
            ;

//checked: trivial
stmnts:   expr          {$$ = $1;}
        | stmnts expr   {$$ = new CompExp($1, $2);}
        ;


//checked: trivial
exprWBlock:   loopExpr  {$$ = $1;}
            | ifExpr    {$$ = $1;}
            | blockExpr {$$ = $1;}
            ;

//checked: 7
ifExpr:   IF expr blockExpr {$$ = new ifExp($2, $3, nullptr);}
        | IF expr blockExpr ELSE blockOrIf {$$ = new ifExp($2, $3, $5);};

//checked: 7
blockOrIf:    blockExpr {$$ = $1;}
            | ifExpr{$$ = $1;}
            ;

//checked: trivial
stmnt:    SEMICOLON {$$ = nullptr;}
        | letExpn   {$$ = $1;}
        | item          {$$ = $1;}
        | macro         {$$ = $1;}
        | loopExpnt     {$$ = $1;}
        ;

//checked: 7
loopExpr: WHILE exprWOBlock blockExpr {$$ = new whileExp($2, $3);};

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
