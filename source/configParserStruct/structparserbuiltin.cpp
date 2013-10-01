
// =====================================================

#define _USE_MATH_DEFINES 1

#include <cmath>
#include <iostream>
#include <stdexcept>

#include "configParserStruct/structparservars.h"
#include "configParserStruct/structparserbuiltin.h"

// =====================================================

configParserStruct::structParserUtil::namedBuiltIn::namedBuiltIn( const std::string &N ) :
  Name(N)
{
  if ( Name.empty() )
    throw std::invalid_argument("Name of builtin function must be not empty");
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

// -----------------------------------------------------

const configParserStruct::structParserUtil::variable configParserStruct::structParserUtil::definedBuiltIn::execute( const program &Program ) const
{
  int NumberOfArgs = getNumberOfArguments(Program);
  if ( NumberOfArgs <= 0 )
    return createVariable( false );
  bool Result = true;
  for ( int i = 1; i <= NumberOfArgs; i++ )
    Result = Result && getArgument( i, Program ).isDefined();
  return createVariable( Result );
}

// =====================================================


