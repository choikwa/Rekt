/*
 * Lexer.cpp
 *
 *  Created on: Mar 2, 2015
 *      Author: thinkdoge
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include "Lexer.hpp"
#include "Opt.hpp"
#include "Lexeme.hpp"
#include "Rekt.hpp"

using namespace LexemeEnums;

using namespace std;


int Lexer::Process(const Opt &opt)
{
  cout << "Lexing..." << endl;
  if (opt.src.empty())
  {
    cerr << "Source filename empty!" << endl;
    return FAIL::LEXER;
  }

  fstream fss(opt.src, ios::in);

  return 0;
}

