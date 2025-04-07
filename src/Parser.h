/*
 * Parser.hpp
 *
 *  Created on: Mar 4, 2015
 *      Author: thinkdoge
 */

#ifndef PARSER_H_
#define PARSER_H_
#include "Node.h"
#include <unordered_set>
#include <vector>
#include <string>
#include <memory>
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
Node *iterable();
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
  class SymTabEnt
  {
  public:
    SymTabEnt(Node *F, Node *T, Node *I, int ln) : 
      Func(F), Type(T), Iden(I), lineno(ln) {}
    Node *Func;
    Node *Type;
    Node *Iden;
    int lineno;
    bool operator==(const SymTabEnt &p) const
    {
      return *Func == *p.Func &&
             *Type == *p.Type &&
             *Iden == *p.Iden;
    }
  };
  unique_ptr<unordered_set<SymTabEnt>> SymbolTable;
};

namespace std
{
  template<>
  struct hash<Parser::SymTabEnt>
  {
    typedef Parser::SymTabEnt argument_type;
    typedef size_t result_type;
    size_t operator()(const Parser::SymTabEnt& x) const
    {
      return x.Func->id ^ x.Type->id ^ x.Iden->id;
    }
  };
}
#endif /* PARSER_H_ */
