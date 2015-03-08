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
#include "Lexemes.txt"
};
extern std::unordered_map<int, const char*> NameMap;
}

using namespace std;
class Lexeme
{
public:
  Lexeme(){ cout << "Empty lexeme!" << endl; }
  Lexeme(int id) : id(id) {}
  Lexeme(int id, long in, int ln) : id(id), val(in), ln(ln) {}
  Lexeme(int id, double in, int ln) : id(id), dval(in), ln(ln) {}
  Lexeme(int id, char in, int ln) : id(id), str(1,in), ln(ln) {}
  Lexeme(int id, const char *in, int ln) : id(id), str(in), ln(ln) {}
  Lexeme(int id, const string &in, int ln) : id(id), str(in), ln(ln) {}
  string getval();
  int id;  // lexeme enum
  union
  {
    long val; // int
    double dval;  //floating point
  };
  string str;
  int ln;  // line number
};

#endif /* LEXEME_HPP_ */
