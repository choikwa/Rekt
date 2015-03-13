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
#include <deque>
#include "Lexeme.hpp"
#include "Node.h"
class Opt;
using namespace std;
using namespace AST;
class Lexer
{
public:
  int Process(const Opt &opt);
  void processComments(const string &str, size_t &it, size_t &ln);
  void processStrLit(const string &str, size_t &it, const size_t ln);
  void processNumLit(const string &str, size_t &it, const size_t ln);
  void processIden(const string &str, size_t &it, const size_t ln);
  vector<Node*> lexemes;
  static const char DN = '*';

};

#endif /* LEXER_HPP_ */
