/*
 * CodeGenerator.cpp
 *
 *  Created on: Nov 2, 2015
 *      Author: flamedoge
 */

#include "CodeGenerator.h"
#include "Opt.h"
#include "Parser.h"
#include "Node.h"
#include <stack>
#include <string>

using namespace std;
namespace CodeGenWS
{
string prog;
void blockEval(Node *n)
{
  prog.append("{ ");
  defaultEval(n);
  prog.append(" }");
}
void defaultEval(Node *n)
{
  for(auto &c : n->children)
  {
    evaluate(c);
  }
}
void evaluate(Node *n)
{
  cout << "evaluate: " << *n << endl;
  switch(n->id)
  {
  case DECL: prog.append(n->children[0]->str).append(" ").append(n->children[1]->str); break;
  case BLOCK: blockEval(n); break;
  default: defaultEval(n); break;
  }
}

}

int CodeGen::Process(Opt &opt, Parser &parser)
{
  auto top = parser.root;
  cout << endl << "======== CODEGEN ========" << endl;
  CodeGenWS::evaluate(top);

  cout << "C output: \"" << CodeGenWS::prog << "\"" << endl;
  return 0;
}


