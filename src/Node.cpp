/*
 * Node.cpp
 *
 *  Created on: Mar 12, 2015
 *      Author: thinkdoge
 */


#include "Node.h"
#include <cstdarg>
#include <iostream>

namespace AST
{
using namespace std;
using namespace Lexeme;
bool Node::operator==(const Node &n)
{
  if (id != n.id) return false;
  if (id == INT && val != n.val) return false;
  if (id == FLOAT && dval != n.dval) return false;
  if (getval() != n.getval()) return false;
  return true;
}
ostream& operator<<(ostream& os, const Node& n)
{
    os << "[" << idToNameMap.at(n.id) << "," << n.getval() << "]";
    return os;
}
Node::Node(int id, int numToAdd, Node *n, ...) : id(id)
{
  va_list vl;
  va_start(vl, n);
  children.push_back(n);
  for(int i=0; i<numToAdd-1; i++)
  {
    children.push_back(va_arg(vl, Node*));
  }
  va_end(vl);
}
static int tab = 0;
void Node::printTree()
{
  for(int i=0; i<tab; i++)
    cout << " ";
  cout << *this << endl;
  tab+=2;
  if (children.size() > 0)
  {
    for(auto c : children)
    {
      c->printTree();
    }
  }
  tab-=2;
}

}
