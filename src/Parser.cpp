/*
 * Parser.cpp
 *
 *  Created on: Mar 4, 2015
 *      Author: thinkdoge
 */

#include <deque>
#include <vector>
#include "Parser.hpp"
#include "Lexeme.hpp"
#include "Lexer.hpp"
#include "Rekt.hpp"
#include "Node.h"

using namespace Lexeme;
Parser::Parser() : root(NULL)
{
  // TODO Auto-generated constructor stub

}

Parser::~Parser()
{
  // TODO Auto-generated destructor stub
}

using namespace std;

using namespace AST;
static vector<Node*> *ss = NULL;
namespace ParseWS
{
static size_t idx = 0;
static int tab = -2;
static bool trace = getenv("traceParser");
#define TRACE() if(trace) { tab+=2; for(int d=tab; d>0; d--){ cout << " "; } \
                 cout << __func__ << endl; cout.flush(); }
#define RET(x) tab-=2; return x
void expect(Node n)
{
  cout << "!!! Expected " << n << " at ln" << curNode().ln << endl;
}
void expect(Node n1, Node n2)
{
  cout << "!!! Expected " << n1 << " or " << n2 << " at ln" << curNode().ln << endl;
}
Node &curNode() { return *ss->at(idx); }
Node *getNode()
{
  if (idx >= ss->size())
  {
    cout << "end of lexeme stream" << endl;
    exit(0);
  }
  //cout << "ss[" << idx << "]=" << *ss->at(idx) << endl;
  Node *tmp = ss->at(idx);
  return tmp;
}
Node *lexMatch(int id)
{
  Node *tmp = getNode();
  if (tmp->id == id)
  {
    idx++;
    return tmp;
  }
  return NULL;
}
Node *match(Node n)
{
  Node *tmp = getNode();
  if (*tmp == n)
  {
    idx++;
    return tmp;
  }
  return NULL;
}
Node *program()
{
  TRACE();
  vector<Node*> stmts;
  while(Node *type = stmt()) { stmts.push_back(type); }
  RET(new Node(PROGRAM, stmts));
}
Node *stmt()
{
  TRACE();
  if(Node *fn = func())
  {
    RET(new Node(STMT, 1, fn));
  }
  else if (Node *n_if = f_if())
  {
    RET(new Node(STMT, 1, n_if));
  }
  RET(NULL);
}
Node *func()
{
  TRACE();
  // drastic measures to abuse decl in cond...
  if(Node *type = lexMatch(TYPE))
  {
    if(Node *iden = lexMatch(IDEN))
    {
      if (Node *n_args = args())
      {
        if (Node *n_block = block())
        {
          Node *fn = new Node(FUNC, 2, n_args, n_block);
          fn->str = type->str + " " + iden->str;
          RET(fn);
        } expect(BLOCK);
      }
    }
  }
  RET(NULL);
}
Node *args()
{
  TRACE();
  vector<Node*> children;
  if(match(Node(BRACKET, "(")))
  {
    if (Node *decl1 = decl())
    {
      cout << "matched decl" << endl;
      children.push_back(decl1);
      Node *comma, *decl2;
      do
      {
        comma = match(COMMA);
        decl2 = decl();
        if (comma && decl2)
        {
          children.push_back(comma);
          children.push_back(decl2);
        }
      } while (comma && decl2);
    }
    if(match(Node(BRACKET, ")")))
    {
      RET(new Node(ARGS, children));
    } expect(Node(BRACKET, ")"));
  }
  RET(NULL);
}
Node *decl()
{
  TRACE();
  if (Node *type = lexMatch(TYPE))
  {
    if (Node *iden = lexMatch(IDEN))
    {
      RET(new Node(DECL, 2, type, iden));
    } expect(DECL);
  }
  RET(NULL);
}
Node *block()
{
  TRACE();
  if(match(Node(BRACKET, "{")))
  {
    vector<Node *> stmts;
    while(Node *s = stmt())
    {
      stmts.push_back(s);
    }
    if(match(Node(BRACKET, "}")))
      RET(new Node(BLOCK, stmts));
    expect(Node(BRACKET, "}"));
  }
  RET(NULL);
}
Node *f_if()
{
  TRACE();
  if(match(IF))
  {
    vector<Node*> chl;
    if(match(Node(BRACKET, "(")))
    {
      if(Node *e = exp())
      {
        chl.push_back(e);
        if(match(Node(BRACKET, ")")))
        {
          if(Node *s = stmt())
          {
            chl.push_back(s);
          }
          else if (Node *b = block())
          {
            chl.push_back(b);
          } expect(STMT, BLOCK);

          if(Node *n_else = f_else())
          {
            chl.push_back(n_else);
          }
          RET(new Node(IF, chl));
        } expect(Node(BRACKET, ")"));
      } expect(EXP);
    } expect(Node(BRACKET, ")"));
  }
  RET(NULL);
}
Node *f_else()
{
  RET(new Node(ELSE));
}
Node *exp()
{
  RET(new Node(EXP));
}
}

int Parser::Process(Lexer &lex)
{
  cout << "======== PARSER ========" << endl;
  for (auto it : lex.lexemes)
  {
    cout << it->getval() << " ";
  }
  cout << endl;

  cout << "======= Lexemes ========" << endl;
  for (auto it : lex.lexemes)
  {
    cout << *it << " ";
  }
  cout << endl;
  cout << "======== Recursive Descent ========" << endl;
  ss = &lex.lexemes;
  root = ParseWS::program();  // recursive descent parsing!

  cout << "======== AST TREE ========" << endl;
  if (root)
  {
    root->printTree();
  }
  if (ParseWS::idx != ss->size())
    return FAIL::PARSER;
  return 0;
}
