
// =====================================================

#include "configParserStruct/buildinfunc.h"
#include "configParserStruct/variable.h"
#include "configParserStruct/reference.h"
#include "configParserStruct/commandstack.h"
#include "configParserStruct/memory.h"

#include <stdexcept>
#include <cstdio>
#include <cstdlib>
#include <cassert>

// =====================================================

configParserStruct::buildInFunction::~buildInFunction()
{
}

// -----------------------------------------------------
      
size_t configParserStruct::buildInFunction::countOfArguments( const memory &Memory )
{
  return derefCommand::extractArgsCount(Memory);
}

// -----------------------------------------------------
      
const configParserStruct::variable& configParserStruct::buildInFunction::argumentValue( const memory &Memory, size_t Index, size_t ArgsCount )
{
  return derefCommand::extractArgValue( Memory, Index, ArgsCount );
}

// =====================================================

configParserStruct::zeroargMathBuildInFunction::zeroargMathBuildInFunction( double V ) :
  Value(V)
{
}

// -----------------------------------------------------

configParserStruct::variable configParserStruct::zeroargMathBuildInFunction::call( const memory& ) const
{
  return variable(Value);
}

// =====================================================

configParserStruct::oneargMathBuildInFunction::oneargMathBuildInFunction( function F ) :
  Function(F)
{
  if ( Function == NULL )
    throw std::invalid_argument( "Function must be not NULL" );
}

// -----------------------------------------------------
      
configParserStruct::variable configParserStruct::oneargMathBuildInFunction::call( const memory &Memory ) const
{
  const size_t ArgsCount = countOfArguments(Memory);
  if ( ArgsCount < 1 )
    return variable();

  const double Argument = argumentValue( Memory, 0, ArgsCount ).real();
  const double Result = Function( Argument );
  
  return variable( Result );
}

// =====================================================

configParserStruct::twoargMathBuildInFunction::twoargMathBuildInFunction( function F ) :
  Function(F)
{
  if ( Function == NULL )
    throw std::invalid_argument( "Function must be not NULL" );
}

// -----------------------------------------------------
      
configParserStruct::variable configParserStruct::twoargMathBuildInFunction::call( const memory &Memory ) const
{
  const size_t ArgsCount = countOfArguments(Memory);
  if ( ArgsCount < 2 )
    return variable();

  const double Argument1 = argumentValue( Memory, 0, ArgsCount ).real();
  const double Argument2 = argumentValue( Memory, 1, ArgsCount ).real();
  const double Result = Function( Argument1, Argument2 );
  
  return variable( Result );
}

// =====================================================

configParserStruct::variable configParserStruct::printBuildInFunction::call( const memory &Memory ) const
{
  const size_t ArgsCount = countOfArguments(Memory);
  for ( size_t i = 0; i < ArgsCount; i++ )
  {
    const variable &Arg = argumentValue( Memory, i, ArgsCount );
    std::fprintf( stdout, "%s", Arg.string().c_str() );
  }

  return variable( static_cast<int>(ArgsCount) );
}

// =====================================================

configParserStruct::variable configParserStruct::printlnBuildInFunction::call( const memory &Memory ) const
{
  const size_t ArgsCount = countOfArguments(Memory);
  for ( size_t i = 0; i < ArgsCount; i++ )
  {
    const variable &Arg = argumentValue( Memory, i, ArgsCount );
    std::fprintf( stdout, "%s", Arg.string().c_str() );
  }
  std::fprintf( stdout, "\n" );

  return variable( static_cast<int>(ArgsCount) );
}

// =====================================================

configParserStruct::variable configParserStruct::envBuildInFunction::call( const memory &Memory ) const
{
  const size_t ArgsCount = countOfArguments(Memory);
  if ( ArgsCount < 1 )
    return variable();

  const std::string Argument = argumentValue( Memory, 0, ArgsCount ).string();
  const char *EnvValue = std::getenv( Argument.c_str() );
  
  if ( EnvValue == NULL )
    return variable();
  return variable( EnvValue );
}

// =====================================================

