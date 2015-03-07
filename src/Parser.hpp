/*
 * Parser.hpp
 *
 *  Created on: Mar 4, 2015
 *      Author: thinkdoge
 */

#ifndef PARSER_HPP_
#define PARSER_HPP_
#include <unordered_map>
#include <vector>
class Lexeme;
class Lexer;
using namespace std;

class Parser
{
public:
  Parser();
  virtual ~Parser();
  int Process(const Lexer &lex);
  void printGrammar();
  static const unordered_map<int, vector<Lexeme>> grammar;
};

#endif /* PARSER_HPP_ */
