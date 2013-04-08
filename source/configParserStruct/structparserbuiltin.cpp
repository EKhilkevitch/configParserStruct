
// =====================================================

#include "configParserStruct/structparservars.h"
#include "configParserStruct/structparserbuiltin.h"

#include <cmath>
#include <iostream>

// =====================================================

const configParserStruct::structParserUtil::variable configParserStruct::structParserUtil::mathBuiltIn::execute( const program &Program ) const
{
  double Arg = getArgument( 1, Program ).number();
  double Result = calculate(Arg);
  return createVariable(Result);
}

// =====================================================

const configParserStruct::structParserUtil::variable configParserStruct::structParserUtil::printBuiltIn::execute( const program &Program ) const
{
  int NumberOfArgs = getNumberOfArguments(Program);
  for ( int i = 1; i <= NumberOfArgs; i++ )
    std::cout << getArgument( i, Program ).string();
  std::cout << std::endl;
  return createVariable( NumberOfArgs );
}

// =====================================================


