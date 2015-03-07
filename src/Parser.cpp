/*
 * Parser.cpp
 *
 *  Created on: Mar 4, 2015
 *      Author: thinkdoge
 */

#include "Parser.hpp"
#include "Lexeme.hpp"
#include "Lexer.hpp"
#include "Rekt.hpp"

using namespace LexemeEnums;
/*const unordered_map<int, vector<Lexeme>> Parser::grammar =
{
  {single_input, {NEWLINE, rex_or, simple_stmt, rex_or, compound_stmt, NEWLINE}},
  {file_input, {parenop, NEWLINE, rex_or, stmt, parencl, rex_star, ENDMARKER}},
  {eval_input, {testlist, NEWLINE, rex_star, ENDMARKER}},
  {decorator, {"@", dotted_name, sqbrop, "(", sqbrop, arglist, sqbrcl, ")", NEWLINE}},
  {decorators, {decorator, rex_plus}},
  {decorated, {decorators, funcdef}},
  {parameters, {"(", sqbrop, typedargslist, sqbrcl, ")"}},
  {typedargslist, {parenop, tfpdef, sqbrop, "=", test, sqbrcl, parenop, ",", tfpdef, sqbrop, "=", test, sqbrcl, parencl, rex_star, sqbrop, ",",
      sqbrop, "*", sqbrop, tfpdef, sqbrcl, parenop, ",", tfpdef, sqbrop, "=", test, sqbrcl, parencl, rex_star, sqbrop, ",", "**", tfpdef, sqbrcl, rex_or, "**", tfpdef, sqbrcl, sqbrcl,
      rex_or, "*", sqbrop, tfpdef, sqbrcl, parenop, ",", tfpdef, sqbrop, "=", test, sqbrcl, parencl, rex_star, sqbrop, ",", "**", tfpdef, sqbrcl, rex_or, "**", tfpdef, parencl}},
  {tfpdef, {NAME, sqbrop, ":", test, sqbrcl}},
  {varargslist, {parenop, vfpdef, sqbrop, "=", test, sqbrcl, parenop, ",", vfpdef, sqbrop, "=", test, sqbrcl, parencl, rex_star, sqbrop, ",",
      sqbrop, "*", sqbrop, vfpdef, sqbrcl, parenop, ",", vfpdef, sqbrop, "=", test, sqbrcl, parencl, rex_star, sqbrop, ",", "**", vfpdef, sqbrcl, rex_or, "**", vfpdef, sqbrcl, sqbrcl,
      rex_or, "*", sqbrop, vfpdef, sqbrcl, parenop, ",", vfpdef, sqbrop, "=", test, sqbrcl, parencl, rex_star, sqbrop, ",", "**", vfpdef, sqbrcl, rex_or, "**", vfpdef, parencl}},
  {vfpdef, {NAME}},
  {stmt, {simple_stmt, rex_or, compound_stmt}},
  {simple_stmt, {small_stmt, parenop, ";", small_stmt, parencl, rex_star, sqbrop, ";", sqbrcl, NEWLINE}},
  {small_stmt, {expr_stmt, rex_or, del_stmt, pass_stmt, rex_or, flow_stmt, rex_or,
      import_stmt, rex_or, global_stmt, rex_or, nonlocal_stmt, rex_or, assert_stmt}},
  {expr_stmt, {testlist_star_expr, parenop, augassign, parenop, yield_expr, rex_or, testlist, parencl, rex_or,
      parenop, "=", parenop, yield_expr, rex_or, testlist_star_expr, parencl, parencl, rex_star, parencl}},
  {testlist_star_expr, {parenop, test, rex_or, star_expr, parencl, parenop, ",", parencl, test, rex_or, star_expr, parencl, parencl, rex_star, sqbrop, ",", sqbrcl}},
  {augassign, {parenop, "+=", rex_or, "-=", rex_or, "*=", rex_or, "/=", rex_or, "%=", rex_or, "&=", rex_or, "|=", rex_or, "^=", rex_or,
      "<<=", rex_or, ">>=", rex_or, "**=", rex_or, "//=", parencl}},
  {del_stmt, {"del", exprlist}},
  {pass_stmt, {"pass"}},
  {flow_stmt, {break_stmt, rex_or, continue_stmt, rex_or, return_stmt, rex_or, raise_stmt, rex_or, yield_stmt}},
  {break_stmt, {"break"}},
  {continue_stmt, {"continue"}},
  {return_stmt, {"return", sqbrop, testlist, sqbrcl}},
  {yield_stmt, {yield_expr}},
  {raise_stmt, {"raise", sqbrop, test, sqbrop, "from", test, sqbrcl, sqbrcl}},
  {import_stmt, {import_name, rex_or, import_from}},
  {import_name, {"import", dotted_as_names}},
  {import_from, {parenop, "from", parenop, parenop, ".", rex_or, "...", parencl, dotted_name, rex_or, parenop, ".", rex_or, "...", parencl, rex_plus, parencl}},
  {import_as_name, {NAME, sqbrop, "as", NAME, sqbrcl}},
  {dotted_as_name, {dotted_name, sqbrop, "as", NAME, sqbrcl}},
  {import_as_names, {import_as_name, parenop, ",", import_as_name, parencl, rex_star, sqbrop, ",", sqbrcl}},
  {dotted_as_names, {dotted_as_name, parenop, ",", dotted_as_name, parencl, rex_star}},
  {dotted_name, {NAME, parenop, ".", NAME, parencl, rex_star}},
  {global_stmt, {"global", NAME, parenop, ",", NAME, parencl, rex_star}},
  {nonlocal_stmt, {"nonlocal", NAME, parenop, ",", NAME, parencl, rex_star}},
  {assert_stmt, {"assert", test, sqbrop, ",", test, sqbrcl}},
  {compound_stmt, {if_stmt, rex_or, while_stmt, rex_or, for_stmt, rex_or, with_stmt, rex_or, funcdef, rex_or, decorated}},
  {if_stmt, {"if", test, ":", suite, parenop, "elif", test, ":", suite, parencl, rex_star, sqbrop, "else", ":", suite, sqbrcl}},
  {while_stmt, {"while", test, ":", suite, sqbrop, "else", ":", suite, sqbrcl}},

};
*/
const unordered_map<int, vector<Lexeme>> Parser::grammar =
{
    {0,{0}},
};
Parser::Parser()
{
  // TODO Auto-generated constructor stub

}

Parser::~Parser()
{
  // TODO Auto-generated destructor stub
}

using namespace std;

void Parser::printGrammar()
{
  cout << "\n<<< Rekt Lang BNF Rules >>>" << endl;
  for(auto &it : grammar) //
  {
    cout << LexemeEnums::NameMap[it.first] << " : ";
    for (auto lex : it.second)
    {
      cout << lex.getstr() << " ";
    }
    cout << endl;
  }
  cout << endl;
}
int Parser::Process(const Lexer &lex)
{

  return FAIL::PARSER;
}
