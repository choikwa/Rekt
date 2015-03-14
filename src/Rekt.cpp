//============================================================================
// Name        : Rekt.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Rekt.hpp"
#include "Lexer.hpp"
#include "Opt.hpp"
#include "Parser.hpp"

using namespace std;

int main(int argc, char **argv)
{
  int ret = 0;

  // 1) Process Options
  Opt opt; if (!ret) ret = opt.Process(argc, argv);

  // 2) Lexer
  Lexer lex; if (!ret) ret = lex.Process(opt);

  // 3) Parser
  Parser pars; if (!ret) ret = pars.Process(lex);

  // 4) Optimizer

  // 5) CodeGenerator

  if (ret)
  {
    cout << "Failed in ";
    if (ret >= 50)
      cout << "CODEGEN ";
    else if (ret >= 40)
      cout << "OPTIMIZER ";
    else if (ret >= 30)
      cout << "PARSER ";
    else if (ret >= 20)
      cout << "LEXER ";
    else if (ret >= 10)
      cout << "OPTION ";
    else
      cout << "UNKNOWN ";
    cout << "with return " << ret << endl;
    return ret;
  }
  cout << "\nSuccess!" << endl;
  return 0;
}
