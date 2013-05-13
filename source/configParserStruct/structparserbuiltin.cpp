
// =====================================================

#define _USE_MATH_DEFINES 1
#include <cmath>
#include <iostream>

#include "configParserStruct/structparservars.h"
#include "configParserStruct/structparserbuiltin.h"

// =====================================================

const configParserStruct::structParserUtil::variable configParserStruct::structParserUtil::mathBuiltIn::execute( const program &Program ) const
{
  double Arg = getArgument( 1, Program ).number();
  double Result = calculate(Arg);
  return createVariable(Result);
}

// -----------------------------------------------------
        
double configParserStruct::structParserUtil::piBuiltIn::calculate( double ) const 
{ 
  return M_PI; 
}

// =====================================================

const configParserStruct::structParserUtil::variable configParserStruct::structParserUtil::powBuiltIn::execute( const program &Program ) const
{
  double Arg1 = getArgument( 1, Program ).number();
  double Arg2 = getArgument( 2, Program ).number();
  double Result = calculate(Arg1,Arg2);
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


