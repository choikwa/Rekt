/*
 * Opt.cpp
 *
 *  Created on: Mar 2, 2015
 *      Author: thinkdoge
 */

#include "Opt.hpp"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <unordered_map>
#include <vector>
#include <string>


using namespace std;

Opt::Opt()
{
  opts =
  {
      {Preprocessor, {"Preprocessor", 0}},
      {Assembly,     {"Assembly", 0}},
      {Object,       {"Object", 0}},
      {Link,         {"Link", 0}},
  };
}

int Opt::Process(int argc, char **argv)
{
  int c = 0;
  while ((c = getopt(argc, argv, "ESco:")) != -1)
  {
    switch (c)
    {
    case 'E':
      opts[Preprocessor].on = 1;
      break;
    case 'S':
      opts[Assembly].on = 1;
      break;
    case 'c':
      opts[Object].on = 1;
      break;
    case 'o':
      opts[Link].on = 1;
      subopts[Link] = optarg;
      break;
    default:
      break;
    }
  }
  for(const auto &it : opts)
  {
    cout << it.second.optName << " " << it.second.on << " ";
    if (!subopts[it.first].empty())
      cout << subopts[it.first];
    cout << endl;
  }

  for(int idx = optind; idx < argc; idx++)
  {
    if (src.empty() && string(argv[idx]).rfind(".cpp") != string::npos)
    {
      src = argv[idx];
      cout << "src: " << src << endl;
      continue;
    }
    cout << "Non-option arg: " << argv[idx] << endl;
    return 1;
  }
  if ((src.empty() && !opts[Link].on) ||
      (!opts[Preprocessor].on && !opts[Assembly].on && !opts[Object].on && !opts[Link].on))
  {
    cout << "Wrong arguments given" << endl;
    return 2;
  }

  return 0;
}
