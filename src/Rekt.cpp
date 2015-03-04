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

using namespace std;

int main(int argc, char **argv)
{
  int ret = 0;

  // 1) Process Options
  Opt opt; if ((ret = opt.Process(argc, argv))) return ret;

  // 2) Lexer
  Lexer lex; if ((ret = lex.Process(opt))) return ret;

  // 3) Parser


  // 4) Semantic Checker


  // 5) Optimizer


  // 6) CodeGenerator


  cout << "Success!" << endl;
  return 0;
}
