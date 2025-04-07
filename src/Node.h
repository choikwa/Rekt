/*
 * Node.h
 *
 *  Created on: Mar 8, 2015
 *      Author: thinkdoge
 */

#ifndef NODE_H_
#define NODE_H_
#include <vector>

#include "Lexeme.h"
namespace AST
{
using namespace std;
using namespace Lexeme;
class Node
{
public:
  Node(){ cout << "Empty lexeme!" << endl; }
  Node(string str) : id(IDEN), str(str) {}
  Node(int id) : id(id) {}
  Node(int id, long in, int ln=0) : id(id), val(in), ln(ln) {}
  Node(int id, double in, int ln=0) : id(id), dval(in), ln(ln) {}
  Node(int id, char in, int ln=0) : id(id), str(1,in), ln(ln) {}
  Node(int id, const char *in, int ln=0) : id(id), str(in), ln(ln) {}
  Node(int id, const string &in, int ln=0) : id(id), str(in), ln(ln) {}
  bool operator==(const Node &n);
  friend ostream& operator<<(ostream& os, const Node& dt);
  Node(int id, int numToAdd, Node *n, ...);
  Node(int id, const vector<Node*> &in) : id(id), children(in) {}

  void printTree();
  string getval() const
  {
    switch (id)
    {
    case INT:
      return to_string(val);
    case FLOAT:
      return to_string(dval);
    default:
      return str;
    }
  }
  int id;  // lexeme enum
  union
  {
    long val; // int
    double dval;  //floating point
  };
  string str;
  int ln;  // line number
  vector<Node*> children;
};

} /* namespace AST */

#endif /* NODE_H_ */
