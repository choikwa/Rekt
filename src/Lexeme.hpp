/*
 * Lexeme.hpp
 *
 *  Created on: Mar 3, 2015
 *      Author: thinkdoge
 */

#ifndef LEXEME_HPP_
#define LEXEME_HPP_
#include <iostream>
#include <cassert>
#include <string>
#include <unordered_map>
namespace LexemeEnums
{
enum
{
#include "LexemeEnums.hpp"
};
extern std::unordered_map<int, const char*> NameMap;
}

using namespace std;
class Lexeme
{
public:
  Lexeme();
  Lexeme(int in);
  Lexeme(const char *in);
  ~Lexeme(){}
  string getstr();
  int val;
  string ss;
  bool isInt;
};

string operator<<(const ostream &out, const Lexeme &in);
#endif /* LEXEME_HPP_ */
