
// =====================================================

#include "configParserStruct/memory.h"
#include "configParserStruct/buildinfunc.h"

#include <sstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <cstdio>
#include <cmath>
#include <cassert>

// =====================================================

configParserStruct::named::named()
{
}

// -----------------------------------------------------
      
configParserStruct::named::named( const named &Other )
{
  copy( &Preset, Other.Preset );
  copy( &Global, Other.Global );
  for ( std::list< namedFrame >::const_iterator it = Other.Locals.begin(); it != Other.Locals.end(); ++it )
  {
    Locals.push_back( namedFrame() );
    copy( &Locals.back(), *it );
  }
}

// -----------------------------------------------------

configParserStruct::named& configParserStruct::named::operator=( const named &Other )
{
  if ( &Other == this )
    return *this;

  named Copy( Other );
  swap( Copy );
  return *this;
}

// -----------------------------------------------------
      
void configParserStruct::named::swap( named &Other )
{
  Preset.Map.swap( Other.Preset.Map );
  Preset.ReferenceMap.swap( Other.Preset.ReferenceMap );
  Global.Map.swap( Other.Global.Map );
  Global.ReferenceMap.swap( Other.Global.ReferenceMap );
  Locals.swap( Other.Locals );
}

// -----------------------------------------------------

void configParserStruct::named::clear( clearMode Mode )
{
  clear( &Global );
  Locals.clear();
  switch ( Mode )
  {
    case ClearAll:
      clear( &Preset );
      break;

    case KeepPreset:
      break;

    default:
      assert( false );
  }
}

// -----------------------------------------------------

void configParserStruct::named::pushLocal()
{
  Locals.push_back( namedFrame() );
}

// -----------------------------------------------------

void configParserStruct::named::popLocal()
{
  assert( ! Locals.empty() );
  if ( ! Locals.empty() )
    Locals.pop_back();
}

// -----------------------------------------------------
      
const configParserStruct::named::namedFrame* configParserStruct::named::frameForScope( scope Scope ) const
{
  switch ( Scope )
  {
    case LocalScope:
      if ( Locals.empty() )
        return &Global;
      return &Locals.back();
    
    case GlobalScope:
      return &Global;
    
    case PresetScope:
      return &Preset;

    default:
      assert( false );
      return NULL;
  }
}

// -----------------------------------------------------

configParserStruct::named::namedFrame* configParserStruct::named::frameForScope( scope Scope )
{
  return const_cast< namedFrame* >( static_cast<const named*>(this)->frameForScope(Scope) );
}

// -----------------------------------------------------

configParserStruct::variable* configParserStruct::named::setValueByName( const std::string &Name, const variable &Value, scope Scope )
{
  return setValueByName( frameForScope(Scope), Name, Value );
}

// -----------------------------------------------------

configParserStruct::variable* configParserStruct::named::findValueByName( const std::string &Name, scope Scope ) const
{
  variable *Result = NULL;
  const namedFrame *Frame = NULL;
  
  Frame = frameForScope(Scope);
  Result = findValueByName( *Frame, Name );

  if ( Result == NULL && Frame == &Global )
  {
    Frame = frameForScope(PresetScope);
    Result = findValueByName( *Frame, Name );
  }

  return Result;
}

// -----------------------------------------------------
      
configParserStruct::variable* configParserStruct::named::setValueByReference( const char *Name, const variable &Value, scope Scope )
{
  return setValueByReference( frameForScope(Scope), Name, Value );
}

// -----------------------------------------------------

configParserStruct::variable* configParserStruct::named::findValueByReference( const char *Name, scope Scope ) const
{
  variable *Result = NULL;
  const namedFrame *Frame = NULL;
  
  Frame = frameForScope(Scope);
  Result = findValueByReference( *Frame, Name );

  if ( Result == NULL && Frame == &Global )
  {
    Frame = frameForScope(PresetScope);
    Result = findValueByReference( *Frame, Name );
  }

  return Result;
}

// -----------------------------------------------------

std::string configParserStruct::named::toDebugString() const
{
  std::string Result;

  Result += toDebugString( Preset, "Preset" );
  Result += toDebugString( Global, "Global" );

  int Index = 0;
  for ( std::list< namedFrame >::const_iterator it = Locals.begin(); it != Locals.end(); ++it )
  {
    char NumberString[64];
    std::snprintf( NumberString, sizeof(NumberString), "Local %i", Index );
    Result += toDebugString( *it, NumberString );
    Index++;
  }

  return Result;
}

// -----------------------------------------------------
      
configParserStruct::variable* configParserStruct::named::setValueByName( namedFrame *Frame, const std::string &Name, const variable &Value )
{
  assert( Frame != NULL );

  return &( Frame->Map[ Name ] = Value );
}

// -----------------------------------------------------

configParserStruct::variable* configParserStruct::named::setValueByReference( namedFrame *Frame, const char *Name, const variable &Value )
{
  assert( Frame != NULL );

  if ( Name == NULL )
    throw std::invalid_argument( "Name must be not NULL" );

  std::map< const char*, variable* >::iterator rit = Frame->ReferenceMap.find(Name);
  if ( rit != Frame->ReferenceMap.end() )
  {
    variable *Pointer = const_cast<variable*>( rit->second );
    if ( Pointer != NULL )
    {
      *Pointer = Value;
      return Pointer;
    }
  }
  
  std::map< std::string, variable >::iterator sit = Frame->Map.find(Name);
  if ( sit != Frame->Map.end() )
  {
    variable *Pointer = &( sit->second );
    Frame->ReferenceMap[Name] = Pointer;
    *Pointer = Value;
    return Pointer;
  }

  variable *Pointer = &( Frame->Map[ Name ] = Value );
  Frame->ReferenceMap[Name] = Pointer;
  return Pointer;
}

// -----------------------------------------------------

configParserStruct::variable* configParserStruct::named::findValueByName( const namedFrame &Frame, const std::string &Name ) 
{
  std::map< std::string, variable >::const_iterator it = Frame.Map.find(Name);
  if ( it != Frame.Map.end() )
    return const_cast<variable*>( &( it->second ) );
  return NULL;
}

// -----------------------------------------------------

configParserStruct::variable* configParserStruct::named::findValueByReference( const namedFrame &Frame, const char *Name ) 
{
  if ( Name == NULL )
    throw std::invalid_argument( "Name must be not NULL" );

  std::map< const char*, variable* >::const_iterator rit = Frame.ReferenceMap.find(Name);
  if ( rit != Frame.ReferenceMap.end() )
  {
    variable *Pointer = rit->second;
    return Pointer;
  }

  std::map< std::string, variable >::const_iterator sit = Frame.Map.find(Name);
  if ( sit != Frame.Map.end() )
  {
    variable *Pointer = const_cast<variable*>( &( sit->second ) );
    Frame.ReferenceMap[Name] = Pointer;
    return Pointer;
  } else {
    Frame.ReferenceMap[Name] = NULL;
  }
  
  return NULL;
}

// -----------------------------------------------------

void configParserStruct::named::clear( namedFrame *Frame )
{
  assert( Frame != NULL );
  Frame->Map.clear();
  Frame->ReferenceMap.clear();
}

// -----------------------------------------------------
      
void configParserStruct::named::copy( namedFrame *To, const namedFrame &From )
{
  assert( To != NULL );

  To->Map = From.Map;

  To->ReferenceMap.clear();
  for ( std::map< const char*, variable* >::const_iterator it = From.ReferenceMap.begin(); it != From.ReferenceMap.end(); ++it )
  {
    variable *Pointer = &( To->Map[ it->first ] );
    To->ReferenceMap[ it->first ] = Pointer;
  }
}

// -----------------------------------------------------
      
std::string configParserStruct::named::toDebugString( const namedFrame &Frame, const std::string &Prefix ) 
{
  std::ostringstream Stream;

  if ( Frame.Map.empty() )
    Stream << Prefix << " (empty)" << std::endl;

  for ( std::map< std::string, variable >::const_iterator it = Frame.Map.begin(); it != Frame.Map.end(); ++it )
    Stream << Prefix << " " << it->first << " = " << it->second << " (value ptr:" << &it->second << ")" << std::endl;
  
  for ( std::map< const char*, variable* >::const_iterator it = Frame.ReferenceMap.begin(); it != Frame.ReferenceMap.end(); ++it )
    Stream << Prefix << " " << it->first << " (" << static_cast<const void*>( it->first ) << ")" << " = " << it->second << std::endl;

  return Stream.str();
}

// =====================================================

configParserStruct::stack::stack()
{
  Stack.reserve( 32 );
}

// -----------------------------------------------------
      
void configParserStruct::stack::swap( stack &Other )
{
  Stack.swap( Other.Stack );
}

// -----------------------------------------------------

void configParserStruct::stack::push( const variable &Variable )
{
  Stack.push_back( Variable );
}

// -----------------------------------------------------

const configParserStruct::variable configParserStruct::stack::pop()
{
  assert( ! Stack.empty() );

  variable Result = Stack.back();
  Stack.pop_back();
  return Result;
}

// -----------------------------------------------------
      
configParserStruct::variable* configParserStruct::stack::valueByShift( size_t Index ) const
{
  if ( Index >= Stack.size() )
    return NULL;

  return &Stack[Index];
}

// -----------------------------------------------------

configParserStruct::variable* configParserStruct::stack::top() const
{
  if ( Stack.empty() )
    return NULL;

  return &Stack.back();
}

// -----------------------------------------------------

bool configParserStruct::stack::empty() const
{
  return Stack.empty();
}

// -----------------------------------------------------

size_t configParserStruct::stack::size() const
{
  return Stack.size();
}

// -----------------------------------------------------

void configParserStruct::stack::truncate( size_t Size )
{
  if ( Size < Stack.size() )
    Stack.resize(Size);
}

// -----------------------------------------------------

void configParserStruct::stack::clear()
{
  Stack.clear();
}

// -----------------------------------------------------
      
std::string configParserStruct::stack::toDebugString() const
{
  std::ostringstream Stream;
  
  if ( Stack.empty() )
    Stream << "(empty)" << std::endl;

  for ( size_t i = 0; i < Stack.size(); i++ )
  {
    Stream << std::setw(5) << i << " : " << Stack[i] << std::endl;
  }
  return Stream.str();
}

// =====================================================

configParserStruct::registers::registers() :
  InstructionPointer( 0 ),
  BaseStackPointer( 0 ),
  UseBaseStackPointer( true ),
  HaltFlag( false )
{
}

// -----------------------------------------------------

void configParserStruct::registers::swap( registers &Other )
{
  registers Tmp = Other;
  Other = *this;
  *this = Tmp;
}

// -----------------------------------------------------
      
void configParserStruct::registers::setLastResult( const variable &Variable )
{
  LastResult = Variable;
}

// -----------------------------------------------------
      
const configParserStruct::variable& configParserStruct::registers::lastResult() const
{
  return LastResult;
}

// -----------------------------------------------------
      
void configParserStruct::registers::setBaseStackPointer( size_t Size )
{
  BaseStackPointer = Size;
}

// -----------------------------------------------------

size_t configParserStruct::registers::baseStackPointer() const
{
  return BaseStackPointer;
}

// -----------------------------------------------------
      
bool configParserStruct::registers::useBaseStackPointer() const
{
  return UseBaseStackPointer;
}

// -----------------------------------------------------

void configParserStruct::registers::setUseBaseStackPointer( bool Set )
{
  UseBaseStackPointer = Set;
}

// -----------------------------------------------------

void configParserStruct::registers::jumpToNextCommand()
{
  InstructionPointer += 1;
}

// -----------------------------------------------------

void configParserStruct::registers::jumpToCommand( size_t Index )
{
  InstructionPointer = Index;
}

// -----------------------------------------------------
      
size_t configParserStruct::registers::instructionPointer() const
{
  return InstructionPointer;
}

// -----------------------------------------------------

void configParserStruct::registers::setHaltFlag()
{
  HaltFlag = true;
}

// -----------------------------------------------------

bool configParserStruct::registers::isHalted() const
{
  return HaltFlag;
}

// -----------------------------------------------------

void configParserStruct::registers::reset()
{
  InstructionPointer = 0;
  BaseStackPointer = 0;
  UseBaseStackPointer = true;
  HaltFlag = false;
  LastResult = variable();
}

// -----------------------------------------------------
      
std::string configParserStruct::registers::toDebugString() const
{
  std::ostringstream Stream;
  Stream << "IP = " << InstructionPointer << std::endl;
  Stream << "BASE SP = " << BaseStackPointer << std::endl;
  Stream << "USE BASE SP = " << UseBaseStackPointer << std::endl;
  Stream << "HALT = " << HaltFlag << std::endl;
  Stream << "LAST = " << LastResult << std::endl;
  return Stream.str();
}

// =====================================================

double configParserStruct::buildins::sqr( double x ) { return x*x; }
double configParserStruct::buildins::max( double a, double b ) { return a > b ? a : b; }
double configParserStruct::buildins::min( double a, double b ) { return a < b ? a : b; }

// -----------------------------------------------------

configParserStruct::buildins::buildins()
{
  try
  {
    initMap();
  } catch ( ... ) {
    clear();
    throw;
  }
}

// -----------------------------------------------------

configParserStruct::buildins::buildins( const buildins &Other )
{
  try
  {
    initMap();

    for ( std::map< const char*, const buildInFunction* >::const_iterator it = Other.ReferenceMap.begin(); it != Other.ReferenceMap.end(); ++it )
    {
      const buildInFunction* Pointer = Map[ it->first ];
      ReferenceMap[ it->first ] = Pointer;
    }
  } catch ( ... ) {
    clear();
    throw;
  }
}
      
configParserStruct::buildins& configParserStruct::buildins::operator=( const buildins &Other )
{
  if ( &Other == this )
    return *this;

  buildins Copy(Other);
  swap( Copy );

  return *this;
}

// -----------------------------------------------------

configParserStruct::buildins::~buildins()
{
  clear();
}

// -----------------------------------------------------

void configParserStruct::buildins::swap( buildins &Other )
{
  Map.swap( Other.Map );
  ReferenceMap.swap( Other.ReferenceMap );
}

// -----------------------------------------------------

void configParserStruct::buildins::initMap()
{
  Map[ "pi" ]    = new zeroargMathBuildInFunction( 4 * std::atan(1.0) );

  Map[ "sin" ]   = new oneargMathBuildInFunction( &std::sin );
  Map[ "cos" ]   = new oneargMathBuildInFunction( &std::cos );
  Map[ "tan" ]   = new oneargMathBuildInFunction( &std::tan );
  Map[ "exp" ]   = new oneargMathBuildInFunction( &std::exp );
  Map[ "sqrt" ]  = new oneargMathBuildInFunction( &std::sqrt );
  Map[ "asin" ]  = new oneargMathBuildInFunction( &std::asin );
  Map[ "acos" ]  = new oneargMathBuildInFunction( &std::acos );
  Map[ "atan" ]  = new oneargMathBuildInFunction( &std::atan );
  Map[ "log" ]   = new oneargMathBuildInFunction( &std::log );
  Map[ "floor" ] = new oneargMathBuildInFunction( &std::floor );
  Map[ "ceil" ]  = new oneargMathBuildInFunction( &std::ceil );
  Map[ "abs" ]   = new oneargMathBuildInFunction( &std::fabs );
  Map[ "sqr" ]   = new oneargMathBuildInFunction( &sqr );
  
  Map[ "min" ]    = new twoargMathBuildInFunction( &min );
  Map[ "max" ]    = new twoargMathBuildInFunction( &max );
  Map[ "pow" ]    = new twoargMathBuildInFunction( &std::pow );
  Map[ "atan2" ]  = new twoargMathBuildInFunction( &std::atan2 );
  
  Map[ "print" ]   = new printBuildInFunction();
  Map[ "println" ] = new printlnBuildInFunction();
  Map[ "env" ]     = new envBuildInFunction();
}

// -----------------------------------------------------
      
void configParserStruct::buildins::clear()
{
  for ( std::map< std::string, const buildInFunction* >::iterator it = Map.begin(); it != Map.end(); ++it )
    delete it->second;

  Map.clear();
  ReferenceMap.clear();
}

// -----------------------------------------------------
      
const configParserStruct::buildInFunction* configParserStruct::buildins::findFunctionByName( const std::string &Name ) const
{
  std::map< std::string, const buildInFunction* >::const_iterator it = Map.find(Name);
  if ( it == Map.end() )
    return NULL;
  return it->second;
}

// -----------------------------------------------------

const configParserStruct::buildInFunction* configParserStruct::buildins::findFunctionByReference( const char *Name ) const
{
  if ( Name == NULL )
    throw std::invalid_argument( "Name must be not NULL" );

  std::map< const char*, const buildInFunction* >::const_iterator rit = ReferenceMap.find(Name);
  if ( rit != ReferenceMap.end() )
    return rit->second;

  std::map< std::string, const buildInFunction* >::const_iterator bit = Map.find(Name);
  if ( bit == Map.end() )
    return NULL;

  const buildInFunction *Pointer = bit->second;
  ReferenceMap[Name] = Pointer;
  return Pointer;
}

// =====================================================

void configParserStruct::memory::swap( memory &Other )
{
  Stack.swap( Other.Stack );
  Registers.swap( Other.Registers );
  Named.swap( Other.Named );
  Buildins.swap( Other.Buildins );
}

// -----------------------------------------------------

void configParserStruct::memory::pushToStack( const variable &Variable )
{
  Stack.push( Variable );
}

// -----------------------------------------------------

configParserStruct::variable configParserStruct::memory::popFromStack()
{
  return Stack.pop();
}

// -----------------------------------------------------

void configParserStruct::memory::setLastResult( const variable &Variable )
{
  Registers.setLastResult( Variable );
}

// -----------------------------------------------------

const configParserStruct::variable& configParserStruct::memory::lastResult() const
{
  return Registers.lastResult();
}

// -----------------------------------------------------

void configParserStruct::memory::setBaseStackPointer( size_t Size )
{
  return Registers.setBaseStackPointer(Size);
}

// -----------------------------------------------------

size_t configParserStruct::memory::baseStackPointer() const
{
  return Registers.baseStackPointer();
}

// -----------------------------------------------------
      
bool configParserStruct::memory::useBaseStackPointer() const
{
  return Registers.useBaseStackPointer();
}

// -----------------------------------------------------

void configParserStruct::memory::setUseBaseStackPointer( bool Set )
{
  Registers.setUseBaseStackPointer( Set );
}

// -----------------------------------------------------

size_t configParserStruct::memory::stackSize() const
{
  return Stack.size();
}

// -----------------------------------------------------

void configParserStruct::memory::truncateStack( size_t Size )
{
  Stack.truncate(Size);
}

// -----------------------------------------------------
      
void configParserStruct::memory::pushLocalNamedFrame()
{
  Named.pushLocal();
}

// -----------------------------------------------------

void configParserStruct::memory::popLocalNamedFrame()
{
  Named.popLocal();
}

// -----------------------------------------------------
      
configParserStruct::variable* configParserStruct::memory::setValueByReference( const char *Name, const variable &Value, scope Scope )
{
  return Named.setValueByReference( Name, Value, Scope );
}

// -----------------------------------------------------
      
configParserStruct::variable* configParserStruct::memory::setValueByName( const std::string &Name, const variable &Value, scope Scope )
{
  return Named.setValueByName( Name, Value, Scope );
}

// -----------------------------------------------------

configParserStruct::variable* configParserStruct::memory::findValueByName( const std::string &Name, scope Scope ) const
{
  return Named.findValueByName( Name, Scope );
}

// -----------------------------------------------------

configParserStruct::variable* configParserStruct::memory::findValueByReference( const char *Name, scope Scope ) const
{
  return Named.findValueByReference( Name, Scope );
}

// -----------------------------------------------------

configParserStruct::variable* configParserStruct::memory::findStackValueByShift( size_t Index ) const
{
  return Stack.valueByShift(Index);
}

// -----------------------------------------------------
      
configParserStruct::variable* configParserStruct::memory::topStackValue() const
{
  return Stack.top();
}

// -----------------------------------------------------
      
const configParserStruct::buildInFunction* configParserStruct::memory::findFunctionByReference( const char *Name ) const
{
  return Buildins.findFunctionByReference(Name);
}

// -----------------------------------------------------

void configParserStruct::memory::jumpToNextCommand()
{
  Registers.jumpToNextCommand();
}

// -----------------------------------------------------

void configParserStruct::memory::jumpToCommand( size_t Index )
{
  Registers.jumpToCommand( Index );
}

// -----------------------------------------------------

size_t configParserStruct::memory::instructionPointer() const
{
  return Registers.instructionPointer();
}

// -----------------------------------------------------

void configParserStruct::memory::setHaltFlag()
{
  Registers.setHaltFlag();
}

// -----------------------------------------------------

bool configParserStruct::memory::isHalted() const
{
  return Registers.isHalted();
}

// -----------------------------------------------------
      
void configParserStruct::memory::clear( named::clearMode Mode )
{
  Stack.clear();
  Named.clear( Mode );
  Registers.reset();
}

// -----------------------------------------------------
      
std::string configParserStruct::memory::toDebugString() const
{
  return 
    "Memory:\n" 
    "====\n"
    "STACK:\n" +
    Stack.toDebugString() +
    "====\n"
    "NAMED:\n" +
    Named.toDebugString() +
    "====\n"
    "REGISTERS:\n" +
    Registers.toDebugString();
}

// =====================================================


