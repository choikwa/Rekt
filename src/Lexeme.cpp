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
unordered_map<int, const char*> NameMap =
{
#include "LexemeNameMap.hpp"
};
}

using namespace std;
using namespace LexemeEnums;
string Lexeme::getval()
{
  switch (id)
  {
  case INT:
    return to_string(val);
  case FLOAT:
    return to_string(dval);
  default:
    return str;
  }
}
