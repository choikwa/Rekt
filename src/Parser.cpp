/*
 * Parser.cpp
 *
 *  Created on: Mar 4, 2015
 *      Author: thinkdoge
 */

#include "Parser.h"

#include <deque>
#include <vector>
#include "Lexeme.h"
#include "Lexer.h"
#include "Node.h"
#include "Rekt.h"

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
#define ENT() auto old = idx; \
  { if(idx >= ss->size()) { cout << "end of lex stream" << endl; return NULL; } \
    if(trace) { tab+=2; for(int d=tab; d>0; d--){ cout << " "; } \
    cout << __func__ << ": " << *ss->at(idx) << endl; }}
#define RET(x) { if(x==NULL) idx = old; tab-=2; return x; }
#define ERR(n) do{ cout << __FILE__ << ":" << __LINE__ << ": !!! Error: Expected " << Node(n) << " at ln" << \
		curNode().ln << endl; exit(0); } while(false);
inline void error(Node n1, Node n2) { cout << "!!! Error: Expected " << n1 << " or " << n2 <<
  " at ln" << curNode().ln << endl; }
inline void error(Node n1, Node n2, Node n3) { cout << "!!! Error: Expected " << n1 << " or " << n2 <<
  " or " << n3 << " at ln" << curNode().ln << endl; }

Node *expect(Node n)	// throw error if not matched
{
  Node *ret;
  if((ret = match(n)))
    return ret;
  else
    ERR(n);
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
void printTape()
{
	cout << "Tape: ";
	for(auto i = idx; i < idx + 5 && i < ss->size(); i++)
	{
		cout << *ss->at(i) << " ";
	}
	cout << endl;
}
Node &curNode() { return *ss->at(idx); }
Node *getNode()
{
  if (idx >= ss->size())
  {
    //cout << "end of lexeme stream" << endl;
    return NULL;
  }
//  cout << "ss[" << idx << "]=" << *ss->at(idx) << endl;
  Node *tmp = ss->at(idx);
  return tmp;
}
Node *lexMatch(int id)	// Only match the id and advance
{
  Node *tmp = getNode();
  if (tmp && tmp->id == id)
  {
    idx++;
    return tmp;
  }
  return NULL;
}

Node *match(Node n)	// Full match unless keyword and advance
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
     (n = f_while()) ||
     (n = f_switch()) ||
     ((n = call()) && lexMatch(SEMICOLON)))
  {
    RET(new Node(STMT, 1, n));
  }
  else if((n = decl()) ||
          (n = exp()))
  {
    if(n->id == DECL && lexMatch(SEMICOLON))
      RET(new Node(STMT, 1, n));
    if((assign = lexExpect(ASSIGN)))
    {
      if((e = exp()))
      {
        if(lexExpect(SEMICOLON))
          RET(new Node(STMT, 3, n, assign, e));
      } else ERR(EXP);
    }
  }
  else if ((n = lexMatch(RETURN)))
  {
    if((e = exp()))
    {
      if(lexExpect(SEMICOLON))
      {
        RET(new Node(STMT, 2, n, e));
      }
    } else ERR(EXP);
  }
  RET(NULL);
}
Node *func()
{
  ENT();
  if(Node *n_decl = decl())
  {
    if (Node *n_args = parms())
    {
      if (Node *n_block = block())
      {
        Node *fn = new Node(FUNC, 3, n_decl, n_args, n_block);
        RET(fn);
      } else ERR(BLOCK);
    }
  }
  RET(NULL);
}
Node *parms()
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
      RET(new Node(PARMS, children));
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
  Node *n = NULL, *op = NULL, *e = NULL;
	op = NULL; e = NULL;
	if(match(Node(BRACKET, "(")))
	{
		if((e = exp()))
		{
			expect(Node(BRACKET, ")"));
		} else error(EXP, BINOP, UNOP);

		n = new Node(BRACKET, 1, e);
	}
	else if((e = call()) ||
		 (e = lexMatch(IDEN)) ||
		 (e = lexMatch(INT)) ||
		 (e = lexMatch(FLOAT)) ||
		 (e = lexMatch(STR)))
	{
		n = e;
	}
	else if((op = lexMatch(UNOP)) ||
					(op = lexMatch(MINUS)))
	{
		if((e = exp()))
		{
			n = new Node(op->id, 1, e);
		} else ERR(EXP);
	}
  // Operator precedence

  if (n)
  {
  	n = r_exp(n);
  	RET(n);
  }
  RET(NULL);
}

Node *r_exp(Node *n)
{
	ENT();
	Node *op = NULL, *e = NULL;
	while(true)
	{
		bool found = false;
		if(n->id == CALL || n->id == IDEN)
		{
			found = true;
			if((op = lexMatch(DOT)))
			{
				if((e = exp()))
				{
					n = new Node(EXP, 2, n, e);
					n->str = op->str;
				} else ERR(EXP);
			}
			else if(match(Node(BRACKET, "[")))
			{
				idx--;
				while(match(Node(BRACKET, "[")))
				{
					if((e = exp()))
					{
						// todo: match type of call/iden to exp inside '[]'
						if(expect(Node(BRACKET, "]")))
						{
							n = new Node(EXP, 2, n, e);
							n->str = "[]";
						}
					} else ERR(EXP);
				}
			}
		}

		if((op = lexMatch(BINOP)) ||
			 (op = lexMatch(MINUS)))
		{
			found = true;
			if((e = exp()))
			{
				n = new Node(op->id, 2, n, e);
				n->str = op->str;
			} else ERR(EXP);
		}

		if(!found)
			break;
	}
	if(n)
		RET(n);
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
      } else ERR(BLOCK);
    } else ERR(ITERATOR);
  }
  RET(NULL);
}
Node *iterator()
{
  ENT();
  if(match(Node(BRACKET, "{")))
  {
    vector<Node*> chl;
    if(Node *d = decl())
    {
      chl.push_back(d);
      Node *comma;
      do
      {
        comma = lexMatch(COMMA);
        d = decl();
        if (comma && d)
        {
          chl.push_back(d);
        }
      } while(comma && d);
      if(expect(Node(BINOP, "|")))
      {
        Node *cond;
        if((cond = exp()))
        {
          chl.push_back(cond);
          do
          {
            comma = lexMatch(COMMA);
            cond = exp();
            if (comma && cond)
            {
              chl.push_back(cond);
            }
          } while(comma && cond);
        } else ERR(EXP);  // Need at least one exp for iterator
        if(expect(Node(BRACKET, "}")))
        {
          RET(new Node(ITERATOR, chl));
        }
      }
    } else ERR(DECL);
  }
  RET(NULL);
}
Node *call()
{
  ENT();
  auto oldIdx = idx;
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
  idx = oldIdx;
  RET(NULL);
}
Node *f_while()
{
  ENT();
  if(lexMatch(WHILE))
  {
    if(expect(Node(BRACKET, "(")))
    {
      if(Node *e = exp())
      {
        if(expect(Node(BRACKET, ")")))
        {
          if(Node *b = block())
          {
            RET(new Node(WHILE, 2, e, b));
          } else ERR(BLOCK);
        }
      } else ERR(EXP);
    }
  }
  RET(NULL);
}
Node *f_switch()
{
  ENT();
  if(lexMatch(SWITCH))
  {
    if(expect(Node(BRACKET, "(")))
    {
      if(Node *e = exp())
      {
        vector<Node *> chl;
        chl.push_back(e);
        if(expect(Node(BRACKET, ")")) &&
           expect(Node(BRACKET, "{")))
        {
          Node *test;
          while((test = exp()) ||
                (test = lexMatch(ALL)) ||
                (test = lexMatch(NONE)))
          {
            if(lexExpect(COLON))
            {
              vector<Node*> tmp;
              while(Node *s = stmt())
              {
                tmp.push_back(s);
              }
              Node *mycase = new Node(CASE, 1, test);
              mycase->children.insert(mycase->children.end(), tmp.begin(), tmp.end());
              chl.push_back(mycase);
            }
          }
          if(chl.size() < 2) error(EXP, ALL, NONE); // need at least one case
          if(expect(Node(BRACKET, "}")))
            RET(new Node(SWITCH, chl));
        }
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
