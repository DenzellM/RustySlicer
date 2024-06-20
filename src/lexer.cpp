// #include "lexer.h"

// #include "test_lex.yy.c"
#include "y3.tab.c"
#include "expresion.h"
// #include "y2.tab.h"

#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>

/* Array with tokens such that index = tokenid - 250*/

std::string tokens[] = {
    "IDENTIFIER",
    "PRINTVAR",
    "PRINTSTRING",
    "DEC_LITERAL",
    "TRUE",
    "FALSE",
    "FN",
    "LPAREN",
    "RPAREN",
    "ARROW",
    "LBRACE",
    "RBRACE",
    "SEMICOLON",
    "COLON",
    "LET",
    "EQ",
    "PLUS",
    "MINUS",
    "STAR",
    "SLASH",
    "PLUSEQ",
    "MINUSEQ",
    "ANDAND",
    "OROR",
    "NOT",
    "GT",
    "GE",
    "LT",
    "LE",
    "EQEQ",
    "NE",
    "AMPERSAND",
    "IF",
    "ELSE",
    "WHILE",
    "MUT",
    "COM"};

// void parse(std::ostream &out, bool fileMode)
// {
//   int tokenid;
//   std::string end = "\n";
//   if (fileMode)
//   {
//     end = " ";
//   }

//   while ((tokenid = yylex()))
//   {
//     /* Token codes start from 250 */
//     out << tokens[tokenid - 258] << end;

//     /* Append value */
//     if ((tokenid == IDENTIFIER) || (tokenid == DEC_LITERAL) ||
//         (tokenid == PRINTSTRING) || (tokenid == PRINTVAR))
//     {
//       out << yytext << end;
//     }
//   }
// }

int main(int argc, char *argv[])
{
  
  // yyFlexLexer lex;
  bool fileMode = false;
  std::string inFile = "";
  std::string outFile = "";

  std::ofstream ostream;
  std::ifstream istream;


  switch (argc)
  {
  case 2:
    inFile = argv[1];
    fileMode = true;
    break;

  case 3:
    inFile = argv[1];
    yydebug = 1;
    fileMode = true;
    break;

  default:
    break;
  }
  
  if (outFile == "" && fileMode)
  {
    outFile = inFile;
    std::size_t pos = outFile.find(".rs");
    outFile.replace(pos, 11, "_tokens.txt");
  }
  if (fileMode)
  {
    // istream.open(inFile);
    // lex.yyrestart(istream);
    FILE *test = fopen(inFile.c_str(), "r");    
    yyin = test;
    // ostream.open(outFile);
    // parse(std::cout, true);
    yyparse();
    // parse(ostream, true);
  }

  return 0;
}
