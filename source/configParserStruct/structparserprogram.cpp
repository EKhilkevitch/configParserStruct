
// =====================================================

#include "configParserStruct/structparserprogram.h"
#include "configParserStruct/structparsercompiler.h"
#include "configParserStruct/structparserbuiltin.h"
#include "configParserStruct/structparserinput.h"
#include "configParserStruct/stringcast.h"
#include "configParserStruct/mutex.h"
#include "configParserStruct/mutex.h"

#include <sstream>
#include <cassert>

extern "C" int strprs_parse();
extern "C" void lexResetLineNumber();
extern "C" void lexSetNewLineEnabled( int Enabled );

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
  setNamedVariable( "pi", piBuiltIn() );
  setNamedVariable( "exp", expBuiltIn() );
  setNamedVariable( "sin", sinBuiltIn() );
  setNamedVariable( "cos", cosBuiltIn() );
  setNamedVariable( "print", printBuiltIn() );
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::program::pushFunctionArgument( const variable &V )
{
  variable NumberOfArgs = Variables.getFromTopOfStack( "$N" );
  Variables.set( "$" + convertToString( NumberOfArgs.integer() + 1 ), V );
  Variables.set( "$N", createVariable( NumberOfArgs.integer() + 1 ) );
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
  bool Result = true;

  Mutex.lock();
  try
  {
    setStructPrserProgram( this );
    setInputString( ProgramText ); 
    lexResetLineNumber();
    lexSetNewLineEnabled(false);
    
    int ParseResult = strprs_parse();
    if ( ErrorLine >= 0 || ParseResult != 0 )
      Result = false;

    setStructPrserProgram( NULL );
  } catch ( ... )
  {
    Mutex.unlock();
    throw;
  }
    
  Mutex.unlock();
  return Result;
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
  std::list<std::string> Names = Variables.listOfNames();
  for ( std::list<std::string>::const_iterator i = Names.begin(); i != Names.end(); ++i )
    Stream << "  " << *i << " = " << Variables.get( *i ).string() << std::endl;

  return Stream.str();
}

// =====================================================

