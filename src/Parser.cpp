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
#define ENT() auto old = idx; { if(trace) { tab+=2; for(int d=tab; d>0; d--){ cout << " "; } \
  cout << __func__ << endl; }}
#define RET(x) { if(x==NULL) idx = old; tab-=2; return x; }
void error(Node n) { cout << "!!! Expected " << n << " at ln" << curNode().ln << endl; }
void error(Node n1, Node n2) { cout << "!!! Expected " << n1 << " or " << n2 <<
  " at ln" << curNode().ln << endl; }

Node *expect(Node n)
{
  Node *ret;
  if((ret = match(n)))
    return ret;
  else
    error(n);
  return NULL;
}
Node *expect(Node n1, Node n2)
{
  Node *ret;
  if((ret = match(n1)) || (ret = match(n2)))
    return ret;
  else
    error(n1, n2);
  return NULL;
}
Node *lexExpect(int id)
{
  Node *ret;
  if((ret = lexMatch(id)))
    return ret;
  else
    cout << "!!! Expected " << idToNameMap.at(id) << " at ln" << curNode().ln << endl;
  return NULL;
}
Node &curNode() { return *ss->at(idx); }
Node *getNode()
{
  if (idx >= ss->size())
  {
    //cout << "end of lexeme stream" << endl;
    return NULL;
  }
  //cout << "ss[" << idx << "]=" << *ss->at(idx) << endl;
  Node *tmp = ss->at(idx);
  return tmp;
}
Node *lexMatch(int id)
{
  Node *tmp = getNode();
  if (tmp && tmp->id == id)
  {
    idx++;
    return tmp;
  }
  return NULL;
}
Node *match(Node n)
{
  if(isKeyword(n.id))
    return lexMatch(n.id);

  Node *tmp = getNode();
  if (tmp && *tmp == n)
  {
    idx++;
    return tmp;
  }
  return NULL;
}
Node *program()
{
  ENT();
  vector<Node*> stmts;
  while(Node *type = stmt()) { stmts.push_back(type); }
  RET(new Node(PROGRAM, stmts));
}
Node *stmt()
{
  ENT();
  Node *n, *e, *assign;
  if((n = func()) ||
     (n = f_if()) ||
     (n = f_for()) ||
     ((n = call()) && lexMatch(SEMICOLON)))
  {
    RET(new Node(STMT, 1, n));
  }
  else if((n = decl()))
  {
    if(lexMatch(SEMICOLON))
      RET(new Node(STMT, 1, n));
    if((assign = lexExpect(ASSIGN)))
    {
      if((e = exp()))
      {
        if(lexExpect(SEMICOLON))
          RET(new Node(STMT, 3, n, assign, e));
      } else error(EXP);
    }
  }
  else if((n = lexMatch(IDEN)))
  {
    if((assign = lexExpect(ASSIGN)))
    {
      if((e = exp()))
      {
        if(lexExpect(SEMICOLON))
          RET(new Node(STMT, 3, n, assign, e));
      } else error(EXP);
    }
  }
  RET(NULL);
}
Node *func()
{
  ENT();
  if(Node *n_decl = decl())
  {
    if (Node *n_args = args())
    {
      if (Node *n_block = block())
      {
        Node *fn = new Node(FUNC, 3, n_decl, n_args, n_block);
        RET(fn);
      } else error(BLOCK);
    }
  }
  RET(NULL);
}
Node *args()
{
  ENT();
  vector<Node*> children;
  if(match(Node(BRACKET, "(")))
  {
    if (Node *decl1 = decl())
    {
      children.push_back(decl1);
      Node *comma, *decl2;
      do
      {
        comma = lexMatch(COMMA);
        decl2 = decl();
        if (comma && decl2)
          children.push_back(decl2);
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
  ENT();
  if (Node *n_type = type())
  {
    if (Node *iden = lexExpect(IDEN))
    {
      RET(new Node(DECL, 2, n_type, iden));
    }
  }
  RET(NULL);
}
Node *type()
{
  ENT();
  if(Node *t = lexMatch(TYPE)) { RET(t); } // int, float, char
  if(match(Node(BRACKET, "[")) && match(Node(BRACKET, "]"))) RET(new Node(TYPE, "[]"));
  if(match(Node(BRACKET, "(")) && match(Node(BRACKET, ")"))) RET(new Node(TYPE, "()"));
  if(match(Node(BRACKET, "{")) && match(Node(BRACKET, "}"))) RET(new Node(TYPE, "{}"));
  RET(NULL);
}
Node *block()
{
  ENT();
  if(match(Node(BRACKET, "{")))
  {
    vector<Node *> stmts;
    while(Node *s = stmt())
    {
      stmts.push_back(s);
    }
    if(match(Node(BRACKET, "}")))
      RET(new Node(BLOCK, stmts));
  }
  RET(NULL);
}
Node *f_if()
{
  ENT();
  if(match(IF))
  {
    vector<Node*> chl;
    if(expect(Node(BRACKET, "(")))
    {
      if(Node *e = exp())
      {
        chl.push_back(e);
        if(expect(Node(BRACKET, ")")))
        {
          if(Node *s = stmt())
          {
            chl.push_back(s);
          }
          else if (Node *b = block())
          {
            chl.push_back(b);
          }
          else
            error(STMT, BLOCK);

          if(Node *n_else = f_else())
          {
            chl.push_back(n_else);
          }
          RET(new Node(IF, chl));
        }
      }
    }
  }
  RET(NULL);
}
Node *f_else()
{
  ENT();
  vector<Node *> chl;
  if(match(ELSE))
  {
    if(Node *n_if = f_if()) // else if case
    {
      chl.push_back(n_if);
      RET(new Node(ELSE, chl));
    }
    else
    {
      if(Node *s = stmt())
      {
        chl.push_back(s);
      }
      else if (Node *b = block())
      {
        chl.push_back(b);
      }
      else
      {
        error(STMT, BLOCK);
        RET(NULL);
      }
      RET(new Node(ELSE, chl));
    }
  }
  RET(NULL);
}
Node *exp()
{
  ENT();
  Node *n, *op, *e;
  if((n = call()) ||
     (n = lexMatch(IDEN)) ||
     (n = lexMatch(INT)) ||
     (n = lexMatch(CHAR)) ||
     (n = lexMatch(STR)))
  {
    if((op = lexMatch(BINOP)) ||
       (op = lexMatch(MINUS)))
    {
      if((e = exp()))
      {
        Node *nexp = new Node(EXP, 2, n, e);
        nexp->str = op->str;
        RET(nexp);
      } else error(EXP);
    }
    RET(n);
  }
  else if((op = lexMatch(UNOP)) ||
          (op = lexMatch(MINUS)))
  {
    if((e = exp()))
    {
      RET(new Node(EXP, 2, op, e))
    } else error(EXP);
  }
  RET(NULL);
}
Node *f_for()
{
  ENT();
  if(lexMatch(FOR))
  {
    if(Node *iter = iterator())
    {
      if(Node *b = block())
      {
        RET(new Node(FOR, 2, iter, b));
      } else error(BLOCK);
    } else error(ITERATOR);
  }
  RET(NULL);
}
Node *iterator()
{
  ENT();
  if(match(Node(BRACKET, "{")))
  {
    vector<Node*> chl;
    if(Node *n_decl = decl())
    {
      chl.push_back(n_decl);
      if(expect(Node(BINOP, "|")))
      {
        Node *cond;
        if((cond = exp()))
        {
          chl.push_back(cond);
          Node *comma;
          do
          {
            comma = lexMatch(COMMA);
            cond = exp();
            if (comma && cond)
            {
              chl.push_back(cond);
            }
          } while(comma && cond);
        } else error(EXP);  // Need at least one exp for iterator
        if(expect(Node(BRACKET, "}")))
        {
          RET(new Node(ITERATOR, chl));
        }
      }
    }
  }
  RET(NULL);
}
Node *call()
{
  ENT();
  if(Node *n = lexMatch(IDEN))
  {
    vector<Node*> chl;
    if(match(Node(BRACKET, "(")))
    {
      if(Node *e = exp())
      {
        chl.push_back(e);
        Node *comma;
        do
        {
          if((comma = lexMatch(COMMA)) && (e = exp()))
            chl.push_back(e);
        } while(comma && e);
      }
      if(expect(Node(BRACKET, ")")))
      {
        Node *call = new Node(CALL, chl);
        call->str = n->str;
        RET(call);
      }
    }
  }
  RET(NULL);
}
}

int Parser::Process(Lexer &lex)
{
  cout << endl << "======== PARSER ========" << endl;
  ss = &lex.lexemes;
  root = ParseWS::program();  // recursive descent parsing!

  cout << "-------- AST TREE --------" << endl;
  if (root)
  {
    root->printTree();
  }
  if (ParseWS::idx != ss->size())
  {
    Node *fail = ss->at(ParseWS::idx);
    cout << "Failed at parsing " << *fail << " at ln" << fail->ln << endl;
    return FAIL::PARSER;
  }
  return 0;
}
