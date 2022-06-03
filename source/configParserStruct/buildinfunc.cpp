
// =====================================================

#include "configParserStruct/buildinfunc.h"
#include "configParserStruct/variable.h"
#include "configParserStruct/reference.h"
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
  size_t PrevStackSize = Memory.baseStackPointer();
  if ( ! Memory.useBaseStackPointer() )
  {
    const variable *SavedStackPointer = Memory.findStackValueByShift( PrevStackSize - 1 );
    if ( SavedStackPointer != NULL )
      PrevStackSize = SavedStackPointer->ref().asStackPointer();
  }
 
  return static_cast<size_t>( Memory.findStackValueByShift( PrevStackSize )->integer() );
}

// -----------------------------------------------------
      
const configParserStruct::variable& configParserStruct::buildInFunction::argumentValue( const memory &Memory, size_t Index )
{
  assert( Index < countOfArguments(Memory) );

  size_t PrevStackSize = Memory.baseStackPointer();
  if ( ! Memory.useBaseStackPointer() )
  {
    const variable *SavedStackPointer = Memory.findStackValueByShift( PrevStackSize - 1 );
    if ( SavedStackPointer != NULL )
      PrevStackSize = SavedStackPointer->ref().asStackPointer();
  }

  return *Memory.findStackValueByShift( PrevStackSize + Index + 1 );
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
  if ( countOfArguments(Memory) < 1 )
    return variable();

  const double Argument = argumentValue( Memory, 0 ).real();
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
  if ( countOfArguments(Memory) < 2 )
    return variable();

  const double Argument1 = argumentValue( Memory, 0 ).real();
  const double Argument2 = argumentValue( Memory, 1 ).real();
  const double Result = Function( Argument1, Argument2 );
  
  return variable( Result );
}

// =====================================================

configParserStruct::variable configParserStruct::printBuildInFunction::call( const memory &Memory ) const
{
  const size_t CountOfArguments = countOfArguments(Memory);

  for ( size_t i = 0; i < CountOfArguments; i++ )
  {
    const variable &Arg = argumentValue( Memory, i );
    std::fprintf( stdout, "%s", Arg.string().c_str() );
  }

  return variable( static_cast<int>(CountOfArguments) );
}

// =====================================================

configParserStruct::variable configParserStruct::printlnBuildInFunction::call( const memory &Memory ) const
{
  const size_t CountOfArguments = countOfArguments(Memory);

  for ( size_t i = 0; i < CountOfArguments; i++ )
  {
    const variable &Arg = argumentValue( Memory, i );
    std::fprintf( stdout, "%s", Arg.string().c_str() );
  }
  std::fprintf( stdout, "\n" );

  return variable( static_cast<int>(CountOfArguments) );
}

// =====================================================

configParserStruct::variable configParserStruct::envBuildInFunction::call( const memory &Memory ) const
{
  if ( countOfArguments(Memory) < 1 )
    return variable();

  const std::string Argument = argumentValue( Memory, 0 ).string();
  const char *EnvValue = std::getenv( Argument.c_str() );
  
  if ( EnvValue == NULL )
    return variable();
  return variable( EnvValue );
}

// =====================================================

