/*
 * Lexeme.hpp
 *
 *  Created on: Mar 3, 2015
 *      Author: thinkdoge
 */

#ifndef LEXEME_HPP_
#define LEXEME_HPP_
#include <iostream>
#include <cassert>
#include <string>
#include <unordered_map>
#include <unordered_set>
namespace Lexeme
{
using namespace std;
enum
{
#include "Lexemes.txt"
};
const unordered_set<string> keywords =
{
#include "Keywords.txt"
};
const unordered_set<string> types =
{
    "int",
    "float",
    "char",
};
const unordered_map<int, const char*> idToNameMap =
{
#include "idToNameMap.txt"
};
const unordered_map<string, int> nameToIdMap =
{
#include "nameToIdMap.txt"
};
}
#endif /* LEXEME_HPP_ */
