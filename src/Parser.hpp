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
using namespace AST;
namespace ParseWS
{
void error(Node n);
void error(Node n1, Node n2);
Node *expect(Node n);
Node *expect(Node n1, Node n2);
Node *lexExpect(int id);
Node &curNode();
Node *getNode();
Node *lexMatch(int id);
Node *match(Node n);
Node *program();
Node *stmt();
Node *func();
Node *args();
Node *decl();
Node *type();
Node *block();
Node *f_if();
Node *f_else();
Node *exp();
Node *f_for();
Node *iterator();
Node *call();
Node *f_while();
Node *f_switch();
}
using namespace std;
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
