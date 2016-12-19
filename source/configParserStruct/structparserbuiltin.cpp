
// =====================================================

#define _USE_MATH_DEFINES 1

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <sstream>

#include "configParserStruct/structparservars.h"
#include "configParserStruct/structparserbuiltin.h"
#include "configParserStruct/structparserprogram.h"

// =====================================================

configParserStruct::structParserUtil::namedBuiltIn::namedBuiltIn( const std::string &N ) :
  Name(N)
{
  if ( Name.empty() )
    throw std::invalid_argument("Name of builtin function must be not empty");
}

// -----------------------------------------------------
        
configParserStruct::structParserUtil::namedBuiltIn::~namedBuiltIn()
{
}

// -----------------------------------------------------

const std::string configParserStruct::structParserUtil::namedBuiltIn::string() const 
{ 
  return "builtin:" + Name; 
}

// =====================================================

const configParserStruct::structParserUtil::variable configParserStruct::structParserUtil::printBuiltIn::execute( const program &Program ) const
{
  size_t NumberOfArgs = getNumberOfArguments(Program);
  for ( size_t i = 1; i <= NumberOfArgs; i++ )
    std::cout << getArgument( i, Program ).string();
  std::cout << std::flush;
  return createVariable( static_cast<int>(NumberOfArgs) );
}

// -----------------------------------------------------

const configParserStruct::structParserUtil::variable configParserStruct::structParserUtil::printlnBuiltIn::execute( const program &Program ) const
{
//  std::cout << "--- println ---- " << std::endl;
  size_t NumberOfArgs = getNumberOfArguments(Program);
  for ( size_t i = 1; i <= NumberOfArgs; i++ )
  {
    std::cout << getArgument( i, Program ).string();
//    std::cout << "(arg " << i << ") " << getArgument( i, Program ).string() << "()";
  }
  std::cout << std::endl;
//  std::cout << "--- end ---- " << std::endl;
  return createVariable( static_cast<int>(NumberOfArgs) );
}

// -----------------------------------------------------

const configParserStruct::structParserUtil::variable configParserStruct::structParserUtil::definedBuiltIn::execute( const program &Program ) const
{
  size_t NumberOfArgs = getNumberOfArguments(Program);
  if ( NumberOfArgs <= 0 )
    return createVariable( false );

  bool Result = true;
  for ( size_t i = 1; i <= NumberOfArgs; i++ )
    Result = Result && getArgument( i, Program ).isDefined();
  return createVariable( Result );
}

// -----------------------------------------------------
        
const configParserStruct::structParserUtil::variable configParserStruct::structParserUtil::debugProgramTextBuildIn::execute( const program &Program ) const
{
  std::stringstream Stream;
  size_t NumberOfCommands = Program.numberOfCommands();
  for ( size_t i = 0; i < NumberOfCommands; i++ )
    Stream << std::setw(12) << i << " " << Program.getCommand(i).toString() << std::endl;
  std::cout << Stream.str() << std::flush;
  return createVariable( Stream.str() );
}

// -----------------------------------------------------
        
const configParserStruct::structParserUtil::variable configParserStruct::structParserUtil::debugProgramStackBuildIn::execute( const program &Program ) const
{
  std::stringstream Stream;
  size_t NumberOfStacks = Program.stackSize();
  for ( size_t i = 0; i < NumberOfStacks; i++ )
  {
    Stream << "-- " << "#" << i << std::endl;
    std::set<std::string> VariableNames = Program.frameVariableNames(i);
    for ( std::set<std::string>::const_iterator Name = VariableNames.begin(); Name != VariableNames.end(); ++Name )
      Stream << "    " << *Name << " = " << Program.getNamedVariableFromFrame( i, *Name ).string() << std::endl;
    Stream << std::endl;
  }
  std::cout << Stream.str() << std::flush;
  return createVariable( Stream.str() );
}

// -----------------------------------------------------

const configParserStruct::structParserUtil::variable configParserStruct::structParserUtil::envBuiltIn::execute( const program &Program ) const
{
  std::string EnvName = getArgument( 1, Program ).string();
  const char *EnvValue = std::getenv( EnvName.c_str() );
  if ( EnvValue == NULL )
    EnvValue = "";
  return createVariable( std::string(EnvValue) );
}

// =====================================================


