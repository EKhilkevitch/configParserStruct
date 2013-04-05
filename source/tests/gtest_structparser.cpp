
// =========================================================

#include <gtest/gtest.h>

#include <cstdio>
#include <algorithm>
#include <iostream>

#include "configParserStruct/structparser.h"

using namespace configParserStruct;

// =========================================================

TEST( structParser, structParser )
{
  const std::string TestProgram = 
    "x = 3+1;\n"
    "";

  structParser Parser;
  Parser.exec(TestProgram);
}

// =========================================================


