/*
 * Lexer.hpp
 *
 *  Created on: Mar 2, 2015
 *      Author: thinkdoge
 */

#ifndef LEXER_HPP_
#define LEXER_HPP_

#include <unordered_map>
#include <vector>
class Lexeme;
class Opt;
using namespace std;
class Lexer
{
public:
  int Process(const Opt &opt);
  static const unordered_map<int, vector<Lexeme>> grammar;
};

#endif /* LEXER_HPP_ */
