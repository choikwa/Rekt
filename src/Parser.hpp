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
namespace AST
{
class Node;
}
class Lexer;
namespace ParseWS
{
using namespace AST;
void expect(Node n);
void expect(Node n1, Node n2);
Node &curNode();
Node *getNode();
Node *lexMatch();
Node *match();
Node *program();
Node *stmt();
Node *func();
Node *args();
Node *decl();
Node *block();
Node *f_if();
Node *f_else();
Node *exp();
}
using namespace std;
using namespace AST;
class Parser
{
public:
  Parser();
  virtual ~Parser();
  int Process(Lexer &lex);

  void printGrammar();
  Node *root;
};

#endif /* PARSER_HPP_ */
