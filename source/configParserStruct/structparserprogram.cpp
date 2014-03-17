
// =====================================================

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES 1
#endif

#include "configParserStruct/structparserprogram.h"
#include "configParserStruct/structparsercompiler.h"
#include "configParserStruct/structparserbuiltin.h"
#include "configParserStruct/structparserinput.h"
#include "configParserStruct/structparserextmath.h"
#include "configParserStruct/stringcast.h"
#include "configParserStruct/mutex.h"

#include <cmath>
#include <sstream>
#include <cassert>
#include <stdexcept>

// =====================================================

extern "C" int CPSSPU_parse();
extern "C" void CPSSPU_lexResetLineNumber();
extern "C" void CPSSPU_lexSetNewLineEnabled( int Enabled );

// =====================================================

static configParserStruct::mutex Mutex;

// =====================================================

configParserStruct::structParserUtil::program::program()
{
  clear();
}

// -----------------------------------------------------

configParserStruct::structParserUtil::program::~program()
{
  clear();
}

// -----------------------------------------------------
        
void configParserStruct::structParserUtil::program::initBuiltInVariables()
{

#define SET_BUILTIN_FUNCTION( Type, Name, Value ) \
  do { setNamedVariable( Name, makeBuiltIn<Type>( Name, Value ) ); } while (false)

#define SET_STD_BUILTIN_FUNCTION( Type, Name ) \
  SET_BUILTIN_FUNCTION( Type, #Name, std::Name )

  SET_BUILTIN_FUNCTION( mathZeroArgumentBuiltIn, "pi", M_PI );

  SET_STD_BUILTIN_FUNCTION( mathOneArgumentBuiltIn, sin );
  SET_STD_BUILTIN_FUNCTION( mathOneArgumentBuiltIn, cos );
  SET_STD_BUILTIN_FUNCTION( mathOneArgumentBuiltIn, tan );
  SET_STD_BUILTIN_FUNCTION( mathOneArgumentBuiltIn, exp );
  SET_STD_BUILTIN_FUNCTION( mathOneArgumentBuiltIn, asin );
  SET_STD_BUILTIN_FUNCTION( mathOneArgumentBuiltIn, acos );
  SET_STD_BUILTIN_FUNCTION( mathOneArgumentBuiltIn, atan );
  SET_STD_BUILTIN_FUNCTION( mathOneArgumentBuiltIn, log );
  
  SET_STD_BUILTIN_FUNCTION( mathTwoArgumentsBuiltIn, pow );
  SET_STD_BUILTIN_FUNCTION( mathTwoArgumentsBuiltIn, atan2 );

  SET_BUILTIN_FUNCTION( mathOneArgumentBuiltIn, "abs", std::fabs );
  SET_BUILTIN_FUNCTION( mathOneArgumentBuiltIn, "erf", erf );
  SET_BUILTIN_FUNCTION( mathOneArgumentBuiltIn, "erfc", erfc );

  setNamedVariable( "print", printBuiltIn() );
  setNamedVariable( "println", printlnBuiltIn() );
  setNamedVariable( "defined", definedBuiltIn() );

#undef SET_BUILTIN_FUNCTION 
#undef SET_STD_BUILTIN_FUNCTION
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::program::pushFunctionArgument( const variable &V )
{
  variable NumberOfArgs = Variables.getFromTopOfStack( "$N" );
  Variables.set( "$" + convertToString( NumberOfArgs.integer() + 1 ), V );
  Variables.set( "$N", createVariable( NumberOfArgs.integer() + 1 ) );
}

// -----------------------------------------------------

std::set<std::string> configParserStruct::structParserUtil::program::onBuildVariableNames() const
{
  std::set<std::string> VariableNames;
  for ( commandsList::const_iterator c = Commands.begin(); c != Commands.end(); ++c )
  {
    try
    {
      const pushValueCommand &PushCommand = dynamic_cast< const pushValueCommand& > ( c->action() );
      const variable &ReferenceVariable = PushCommand.pushedVariable();
      const referenceVariableValue &ReferenceVariableValue = ReferenceVariable.value<referenceVariableValue>();
      const std::string &PushedName = ReferenceVariableValue.name();
      VariableNames.insert( PushedName );
    } catch ( std::bad_cast& ) {}
  }
  return VariableNames;
}

// -----------------------------------------------------
        
void configParserStruct::structParserUtil::program::clear()
{
  Commands.clear();
  Variables.clear();
  Stack.clear();
  
  ErrorLine = -1;
}

// -----------------------------------------------------
        
bool configParserStruct::structParserUtil::program::build( const std::string &ProgramText )
{
  mutexLocker MutexLocker( &Mutex );

  bool Result = true;
  
  setStructPrserProgram( this );
  setInputString( ProgramText ); 
  CPSSPU_lexResetLineNumber();
  CPSSPU_lexSetNewLineEnabled(false);
  
  int ParseResult = CPSSPU_parse();
  if ( ErrorLine >= 0 || ParseResult != 0 )
    Result = false;

  setStructPrserProgram( NULL );
  
  return Result;
}

// -----------------------------------------------------
        
void configParserStruct::structParserUtil::program::execute() 
{ 
  Commands.resetCurrentCommandIndex();
  Commands.execute( this ); 
}

// -----------------------------------------------------

bool configParserStruct::structParserUtil::program::rebuildAndExecute( const std::string &ProgramText )
{
  clear();
  initBuiltInVariables();
  bool Ok = build( ProgramText );
  if ( ! Ok )
    return false;
  execute();
  return true;
}

// -----------------------------------------------------
        
std::string configParserStruct::structParserUtil::program::toString() const
{
  std::stringstream Stream;

  Stream << "Program: " << Commands.size() << " commands, " << Variables.size() << " named variabled" << std::endl;

  Stream << std::endl << "Commands:" << std::endl;
  for ( unsigned i = 0; i < Commands.size(); i++ )
    Stream << "  " << Commands.getCommand(i).toString() << std::endl;
  
  Stream << std::endl << "Variables:" << std::endl;
  const std::set<std::string> &Names = Variables.listOfNames();
  for ( std::set<std::string>::const_iterator i = Names.begin(); i != Names.end(); ++i )
    Stream << "  " << *i << " = " << Variables.get( *i ).string() << std::endl;

  return Stream.str();
}

// =====================================================

