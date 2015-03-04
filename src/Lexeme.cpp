/*
 * Lexeme.cpp
 *
 *  Created on: Mar 3, 2015
 *      Author: thinkdoge
 */

#include <iostream>
#include <cstring>
#include "Lexeme.hpp"

namespace LexemeEnums
{
std::unordered_map<int, const char*> NameMap =
{
#include "LexemeNameMap.hpp"
};
}
using namespace std;
Lexeme::Lexeme()
{
  cout << "Empty Lexeme!" << endl;
}

Lexeme::Lexeme(int in)
: val(in),
  ss(LexemeEnums::NameMap[in]),
  isInt(true)
{
}

Lexeme::Lexeme(const char *in)
: ss(in),
  isInt(false)
{
}

string Lexeme::getstr()
{
  return ss;
}
