/*
 * Lexer.cpp
 *
 *  Created on: Mar 2, 2015
 *      Author: thinkdoge
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <cctype>
#include "Lexer.hpp"
#include "Opt.hpp"
#include "Lexeme.hpp"
#include "Rekt.hpp"

using namespace LexemeEnums;

using namespace std;


int Lexer::Process(const Opt &opt)
{
  cout << "======== LEXER ========" << endl;
  if (opt.src.empty())
  {
    cerr << "Source filename empty!" << endl;
    return FAIL::LEXER;
  }

  ifstream ifs(opt.src, ios::in);

  // read entire file into string
  std::string str;
  if (ifs)
  {
    // get length of file:
    ifs.seekg(0, ifs.end);
    int length = ifs.tellg();
    ifs.seekg(0, ifs.beg);

    str.resize(length, ' '); // reserve space
    char* begin = &*str.begin();

    ifs.read(begin, length);
    ifs.close();
  } else {
    cout << "Could not open source file" << endl;
    return FAIL::LEXER;
  }

  size_t it = 0;
  size_t ln = 1;
#define ADDLEXEME(x,y) cout << DN; lexemes.push_back(Lexeme(x, y, ln))
  while (str[it] != *str.end())
  {
    char c = str[it];
    switch(c)
    {
    case '#': processComments(str, it, ln); break;
    case '\n': ln++;  // discard whitespace, increment ln for \n
    case ' ':
    case '\t':
    case '\v':
    case '\f':
    case '\r': it++; cout << DN; break;
    case '\'':
    case '\"': processStrLit(str, it, ln); break;
    case ',': ADDLEXEME(COMMA, c); it++; break;
    case ';': ADDLEXEME(SEMICOLON, c); it++; break;
    case ':': ADDLEXEME(COLON, c); it++; break;
    case '-': ADDLEXEME(MINUS, c); it++; break;
    default:
      if (string("[]{}()").find(c) != string::npos) { ADDLEXEME(BRACKET, c); it++; break; }
      // '-' is to be fixed up in parser as it can be unary as well
      if (string("+*/").find(c) != string::npos) { ADDLEXEME(BINOP, c); it++; break; }
      if (string("<>=").find(c) != string::npos)
      {
        if (str[it+1] == '=') { ADDLEXEME(BINOP, c); it+=2; break; }
        else if (c == '=') { ADDLEXEME(ASSIGN, c); it++; break; }
        else { ADDLEXEME(BINOP, c); it++; break; }
      }
      if (string("&|").find(c) != string::npos)
      {
        if (str[it+1] == c) { ADDLEXEME(BINOP, str.substr(it, 2)); it+=2; break; }
        else { ADDLEXEME(BINOP, c); it++; break; }
      }
      if (isdigit(c)) { processNumLit(str, it, ln); break; }
      //else { processIden

      // unconsumed
      cout << "Unrecognized Lexeme \'" << c << "\' in l:" << ln << endl;
      it++;
      break;
    }
    cout.flush();
  }
#ifdef ADDLEXEME
#undef ADDLEXEME
#endif

  cout << "\n\n====[Enum] [val] [lineno]====" << endl;
  for (auto &it: lexemes)
  {
    cout << '[' << NameMap[it.id] << "] [" << it.getval() << "] [l:" << it.ln << ']' << endl;
  }
  cout << endl;
  return 0;
}

void Lexer::processComments(const string &str, size_t &it, size_t &ln)
{
  //multi-line comment
  if(0 && str[it+1] == '#')
  {
    //consume until '##'
    it += 2;
    while(!(str[it] == '#' && str[it+1] == '#'))
    {
      if(str[it] == '\n')
        ln++;
      if(str[it+1] == *str.end())
      {
        cout << "unterminated multi-line string in line " << ln << endl;
        return;
      }
      it++;
    }
    it++;
    return;
  }
  //single-line comment
  while(str[it+1] != '\n')
    it++;
  it++;
}
void Lexer::processStrLit(const string &str, size_t &it, const size_t ln)
{
  //read until unescaped ' or " is found.
  size_t start = it+1;
  char quote = str[it];
  while(true)
  {
    it++;
    if (str[it] == *str.end())
    {
      cout << "Unterminated string" << endl;
      return;
    }
    if (str[it] == quote &&
        str[it-1] != '\\')
      break;
  }
  lexemes.push_back(Lexeme(STR, str.substr(start, it - start), ln));
  it++;
  cout << DN;
}

void Lexer::processNumLit(const string &str, size_t &it, const size_t ln)
{
  size_t start = it;
  bool foundDecPoint = false;
  bool foundHex = false;
  while(isdigit(str[it]) || str[it] == '.' || str[it] == 'x')
  {
    if(str[it] == '.')
    {
      if(foundDecPoint)
        { cout << "l:" << ln << ": Found another decimal point while processing float literal" << endl; break; }
      else
        { foundDecPoint = true; }
    }
    else if(str[it] == 'x')
    {
      if(foundHex)
        { cout << "l:" << ln << ": Found another 'x' while processing hex literal" << endl; break; }
      else
        { foundHex = true; }
    }
    it++;
  }
  if(foundDecPoint)
    lexemes.push_back(Lexeme(FLOAT, stod(str.substr(start, it - start), NULL), ln));
  else
    lexemes.push_back(Lexeme(INT, stol(str.substr(start, it - start), NULL), ln));
}
