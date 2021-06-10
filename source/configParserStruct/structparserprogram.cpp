
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
#include <iomanip>
#include <cassert>
#include <stdexcept>

// =====================================================

extern "C" int CPSSPU_parse();
extern "C" void CPSSPU_lexResetLineNumber();
extern "C" void CPSSPU_lexSetNewLineEnabled( int Enabled );

// =====================================================

namespace
{

  // =====================================================

  configParserStruct::mutex Mutex;

  // -----------------------------------------------------

  double doubleSqr( double X ) { return X * X; }
  double doubleMax( double A, double B ) { return A > B ? A : B; } 
  double doubleMin( double A, double B ) { return A < B ? A : B; } 

  // =====================================================
}

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

  SET_STD_BUILTIN_FUNCTION( mathOneArgumentBuiltIn, sqrt );
  SET_STD_BUILTIN_FUNCTION( mathOneArgumentBuiltIn, sin );
  SET_STD_BUILTIN_FUNCTION( mathOneArgumentBuiltIn, cos );
  SET_STD_BUILTIN_FUNCTION( mathOneArgumentBuiltIn, tan );
  SET_STD_BUILTIN_FUNCTION( mathOneArgumentBuiltIn, exp );
  SET_STD_BUILTIN_FUNCTION( mathOneArgumentBuiltIn, asin );
  SET_STD_BUILTIN_FUNCTION( mathOneArgumentBuiltIn, acos );
  SET_STD_BUILTIN_FUNCTION( mathOneArgumentBuiltIn, atan );
  SET_STD_BUILTIN_FUNCTION( mathOneArgumentBuiltIn, log );
  SET_STD_BUILTIN_FUNCTION( mathOneArgumentBuiltIn, floor );
  SET_STD_BUILTIN_FUNCTION( mathOneArgumentBuiltIn, ceil );
  
  SET_STD_BUILTIN_FUNCTION( mathTwoArgumentsBuiltIn, pow );
  SET_STD_BUILTIN_FUNCTION( mathTwoArgumentsBuiltIn, atan2 );

  SET_BUILTIN_FUNCTION( mathOneArgumentBuiltIn, "abs", std::fabs );
  SET_BUILTIN_FUNCTION( mathOneArgumentBuiltIn, "erf", erf );
  SET_BUILTIN_FUNCTION( mathOneArgumentBuiltIn, "erfc", erfc );
  SET_BUILTIN_FUNCTION( mathOneArgumentBuiltIn, "sqr", doubleSqr );
  SET_BUILTIN_FUNCTION( mathTwoArgumentsBuiltIn, "max", doubleMax );
  SET_BUILTIN_FUNCTION( mathTwoArgumentsBuiltIn, "min", doubleMin );

  setNamedVariable( "print", printBuiltIn() );
  setNamedVariable( "println", printlnBuiltIn() );
  setNamedVariable( "defined", definedBuiltIn() );
  setNamedVariable( "env", envBuiltIn() );
  setNamedVariable( "debug_text", debugProgramTextBuildIn() );
  setNamedVariable( "debug_stack", debugProgramStackBuildIn() );

#undef SET_BUILTIN_FUNCTION 
#undef SET_STD_BUILTIN_FUNCTION
}

// -----------------------------------------------------
        
size_t configParserStruct::structParserUtil::program::pushCommand( const commandAction &Action ) 
{ 
  return Commands.push(Action); 
}

// -----------------------------------------------------
        
size_t configParserStruct::structParserUtil::program::numberOfCommands() const 
{ 
  return Commands.size(); 
}

// -----------------------------------------------------
        
const configParserStruct::structParserUtil::command& configParserStruct::structParserUtil::program::getCommand( size_t Index ) const
{
  return Commands.getCommand(Index);
}

// -----------------------------------------------------
        
void configParserStruct::structParserUtil::program::executeOneCommand() 
{ 
  Commands.executeOneCommand(this); 
}

// -----------------------------------------------------
        
size_t configParserStruct::structParserUtil::program::replaceCommandMarkerToNop() 
{ 
  return Commands.replaceMarker( nopCommand() ); 
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::program::setCurrentCommandIndex( size_t Index ) 
{ 
  Commands.setCurrentCommandIndex(Index); 
}

// -----------------------------------------------------
        
size_t configParserStruct::structParserUtil::program::currentCommandIndex()  const 
{ 
  return Commands.currentCommandIndex(); 
}

// -----------------------------------------------------

size_t configParserStruct::structParserUtil::program::stackSize() const 
{ 
  return Stack.size(); 
}

// -----------------------------------------------------


std::set<std::string> configParserStruct::structParserUtil::program::frameVariableNames( size_t StackIndex ) const
{
  return Variables.listOfNamesInStack( StackIndex );
}

// -----------------------------------------------------

const configParserStruct::structParserUtil::variable configParserStruct::structParserUtil::program::getNamedVariableFromFrame( size_t StackIndex, const std::string &Name ) const
{
  return Variables.getFromStack( StackIndex, Name );
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::program::pushFunctionArgument( const variable &Value )
{
  variable NumberOfArgs = Variables.getFromTopOfStack( "$N" );
  Variables.set( "$" + convertToString( NumberOfArgs.integer() + 1 ), Value );
  Variables.set( "$N", createVariable( NumberOfArgs.integer() + 1 ) );
}

// -----------------------------------------------------
        
void configParserStruct::structParserUtil::program::pushVariableFrame() 
{ 
  Variables.pushNewList();

#if 0
  int NumberOfArgs = Variables.getFromLastByOneOfStack( "$N" ).integer();
  for ( int i = 1; i <= NumberOfArgs; i++ )
  {
    std::string Name = "$" + convertToString(i);
    Variables.set( Name, Variables.getFromLastByOneOfStack(Name) ); 
  }
  Variables.set( "$N", createVariable(NumberOfArgs) );
#endif
}

// -----------------------------------------------------

void configParserStruct::structParserUtil::program::popVariableFrame() 
{ 
  Variables.popList();
#if 0 
  int NumberOfArgs = Variables.getFromTopOfStack( "$N" ).integer();
  for ( int i = 1; i <= NumberOfArgs; i++ )
  {
    std::string Name = "$" + convertToString(i);
    Variables.removeFromTopOfStack( Name );
  }
  Variables.removeFromTopOfStack( "$N" );


  NumberOfArgs = Variables.getFromLastByOneOfStack( "$N" ).integer();
  for ( int i = 1; i <= NumberOfArgs; i++ )
  {
    std::string Name = "$" + convertToString(i);
    Variables.set( Name, Variables.getFromLastByOneOfStack(Name) ); 
  }
  Variables.set( "$N", createVariable(NumberOfArgs) );
#endif
}

// -----------------------------------------------------
        
size_t configParserStruct::structParserUtil::program::functionLevel() const 
{
  return Variables.size(); 
}

// -----------------------------------------------------

std::set<std::string> configParserStruct::structParserUtil::program::onBuildVariableNames() const
{
  std::set<std::string> VariableNames;
  for ( commandsList::const_iterator c = Commands.begin(); c != Commands.end(); ++c )
  {
    const pushValueCommand *PushCommand = dynamic_cast< const pushValueCommand* > ( &c->action() );
    if ( PushCommand == NULL )
      continue;
    const variable &ReferenceVariable = PushCommand->pushedVariable();
    if ( ! ReferenceVariable.isValueDerivedFrom<referenceVariableValue>() )
      continue;
    const referenceVariableValue &ReferenceVariableValue = ReferenceVariable.value<referenceVariableValue>();
    const std::string &PushedName = ReferenceVariableValue.name();
    VariableNames.insert( PushedName );
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
  {
    Commands.clear();
    Result = false;
  }

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

std::string configParserStruct::structParserUtil::program::lastResultVariableName() 
{ 
  return "::LAST_EXPRESSION::"; 
}

// -----------------------------------------------------
        
std::string configParserStruct::structParserUtil::program::toString() const
{
  std::stringstream Stream;

  Stream << "Program: " << Commands.size() << " commands, " << Variables.size() << " named variabled" << std::endl;

  Stream << std::endl << "Commands:" << std::endl;
  for ( size_t i = 0; i < Commands.size(); i++ )
    Stream << "  " << std::setw(4) << i << " " << Commands.getCommand(i).toString() << std::endl;
  
  Stream << std::endl << "Variables:" << std::endl;
  const std::set<std::string> &Names = Variables.listOfNames();
  for ( std::set<std::string>::const_iterator i = Names.begin(); i != Names.end(); ++i )
    Stream << "  " << *i << " = " << Variables.get( *i ).string() << std::endl;

  return Stream.str();
}

// =====================================================

