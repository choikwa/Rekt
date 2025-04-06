/*
 * Parser.hpp
 *
 *  Created on: Mar 4, 2015
 *      Author: thinkdoge
 */

#ifndef PARSER_H_
#define PARSER_H_
#include <unordered_map>
#include <vector>
#include <string>
namespace AST
{
class Node;
}
class Lexer;
using namespace AST;
namespace ParseWS
{
void ERR(Node n);
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
Node *parms();
Node *decl();
Node *type();
Node *block();
Node *f_if();
Node *f_else();
Node *exp();
Node *r_exp(Node *n);
Node *f_for();
Node *iterator();
Node *call();
Node *f_while();
Node *f_switch();
bool hasHigherOpPrecedence(const std::string &s1, const std::string &s2);
Node *fixUpOpPrec(Node *n);
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

#endif /* PARSER_H_ */
