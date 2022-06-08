
// =====================================================

#include "configParserStruct/memory.h"
#include "configParserStruct/buildinfunc.h"

#include <sstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <cassert>

// =====================================================

configParserStruct::named::namedFrame::namedFrame() :
  ReferenceMap( ReferenceMapSize )
{
}

// -----------------------------------------------------
          
configParserStruct::named::namedFrame::namedFrame( const namedFrame &Other ) :
  Map( Other.Map ),
  ReferenceMap( ReferenceMapSize )
{
  for ( std::vector< std::vector< std::pair<const char*,variable*> > >::const_iterator bit = Other.ReferenceMap.begin(); bit != Other.ReferenceMap.end(); ++bit )
  {
    for ( std::vector< std::pair<const char*,variable*> >::const_iterator sit = bit->begin(); sit != bit->end(); ++sit )
    {
      const char *ConstChar = sit->first;
      variable *Pointer = NULL;
      std::map< std::string, variable >::iterator mit = Map.find( ConstChar );
      if ( mit != Map.end() )
        Pointer = &( mit->second );
      ReferenceMap[ indexOfPointer(ConstChar) ].push_back( std::make_pair( ConstChar, Pointer ) );
    }
  }
}

// -----------------------------------------------------

configParserStruct::named::namedFrame& configParserStruct::named::namedFrame::operator=( const namedFrame &Other )
{
  if ( &Other == this )
    return *this;

  namedFrame Copy(Other);
  Copy.swap( *this );

  return *this;
}

// -----------------------------------------------------
          
size_t configParserStruct::named::namedFrame::indexOfPointer( const void *Pointer )
{
  size_t PointerAsNumber = reinterpret_cast<size_t>(Pointer);
  PointerAsNumber = ( PointerAsNumber >> 4 );
  return PointerAsNumber % ReferenceMapSize;
}

// -----------------------------------------------------

void configParserStruct::named::namedFrame::swap( namedFrame &Other )
{
  Map.swap( Other.Map );
  ReferenceMap.swap( Other.ReferenceMap );
}

// -----------------------------------------------------

void configParserStruct::named::namedFrame::clear()
{
  Map.clear();
  for ( size_t i = 0; i < ReferenceMap.size(); i++ )
    ReferenceMap[i].clear();
}

// -----------------------------------------------------
          
void configParserStruct::named::namedFrame::clearReferenceMap()
{
  for ( size_t i = 0; i < ReferenceMap.size(); i++ )
    ReferenceMap[i].clear();
}

// -----------------------------------------------------

void configParserStruct::named::namedFrame::joinFrom( const namedFrame &Frame )
{
  for ( std::map< std::string, variable >::const_iterator mit = Frame.Map.begin(); mit != Frame.Map.end(); ++mit )
    setValueByName( mit->first, mit->second );
}

// -----------------------------------------------------
      
configParserStruct::variable* configParserStruct::named::namedFrame::setValueByName( const std::string &Name, const variable &Value )
{
  variable *Result = &( Map[ Name ] = Value );

  const char *NameString = Name.c_str();

  for ( std::vector< std::vector< std::pair<const char*,variable*> > >::iterator bit = ReferenceMap.begin(); bit != ReferenceMap.end(); ++bit )
  {
    for ( std::vector< std::pair<const char*,variable*> >::iterator sit = bit->begin(); sit != bit->end(); sit++ )
    {
      if ( std::strcmp( sit->first, NameString ) == 0 )
      {
        bit->erase( sit );
        goto End;
      }
    }
  }

End:

  return Result;
}

// -----------------------------------------------------

configParserStruct::variable* configParserStruct::named::namedFrame::setValueByReference( const char *Name, const variable &Value )
{
  if ( Name == NULL )
    throw std::invalid_argument( "Name must be not NULL" );

  const size_t RIndex = indexOfPointer(Name);
  std::vector< std::pair<const char*,variable*> >::iterator FoundRefMap = ReferenceMap[RIndex].end();
  for ( std::vector< std::pair<const char*,variable*> >::iterator si = ReferenceMap[RIndex].begin(); si != ReferenceMap[RIndex].end(); si++ ) 
  {
    if ( si->first == Name )
    {
      if ( si->second != NULL )
      {
        *si->second = Value;
        return si->second;
      } else {
        FoundRefMap = si;
        break;
      }
    }
  }
  
  std::map< std::string, variable >::iterator sit = Map.find(Name);
  variable *Pointer = NULL;
  if ( sit != Map.end() )
  {
    Pointer = &( sit->second );
    *Pointer = Value;
  } else {
    Pointer = &( Map[ Name ] = Value );
  }
   
  assert( Pointer != NULL );

  if ( FoundRefMap == ReferenceMap[RIndex].end() ) 
    ReferenceMap[RIndex].push_back( std::make_pair(Name,Pointer) );
  else
    FoundRefMap->second = Pointer;

  return Pointer;
}

// -----------------------------------------------------

configParserStruct::variable* configParserStruct::named::namedFrame::findValueByName( const std::string &Name ) const
{
  std::map< std::string, variable >::const_iterator it = Map.find(Name);
  if ( it != Map.end() )
    return const_cast<variable*>( &( it->second ) );
  return NULL;
}

// -----------------------------------------------------

configParserStruct::variable* configParserStruct::named::namedFrame::findValueByReference( const char *Name ) const
{
  if ( Name == NULL )
    throw std::invalid_argument( "Name must be not NULL" );

  const size_t RIndex = indexOfPointer(Name);
  for ( std::vector< std::pair<const char*,variable*> >::const_iterator si = ReferenceMap[RIndex].begin(); si != ReferenceMap[RIndex].end(); si++ ) 
  {
    if ( si->first == Name )
      return si->second;
  }

  std::map< std::string, variable >::const_iterator sit = Map.find(Name);
  if ( sit != Map.end() )
  {
    variable *Pointer = const_cast<variable*>( &( sit->second ) );
    ReferenceMap[RIndex].push_back( std::make_pair(Name,Pointer) );
    return Pointer;
  } else {
    ReferenceMap[RIndex].push_back( std::make_pair(Name,static_cast<variable*>(NULL)) );
    return NULL;
  }
}

// -----------------------------------------------------
      
std::string configParserStruct::named::namedFrame::toDebugString( const std::string &Prefix ) const
{
  std::ostringstream Stream;

  if ( Map.empty() )
    Stream << Prefix << " (empty)" << std::endl;

  for ( std::map< std::string, variable >::const_iterator it = Map.begin(); it != Map.end(); ++it )
    Stream << Prefix << " " << it->first << " = " << it->second << " (value ptr:" << &it->second << ")" << std::endl;
 

  for ( std::vector< std::vector< std::pair<const char*,variable*> > >::const_iterator bit = ReferenceMap.begin(); bit != ReferenceMap.end(); ++bit )
  {
    for ( std::vector< std::pair<const char*,variable*> >::const_iterator sit = bit->begin(); sit != bit->end(); sit++ )
    {
      Stream << Prefix << " " << std::setw(3) << ( bit - ReferenceMap.begin() ) << " " << sit->first << " (" << static_cast<const void*>( sit->first ) << ")" << " = " << sit->second << std::endl;
    }
  }

  return Stream.str();
}

// -----------------------------------------------------

configParserStruct::named::named()
{
}

// -----------------------------------------------------
      
configParserStruct::named::named( const named &Other ) :
  Preset( Other.Preset ),
  Global( Other.Global ),
  Locals( Other.Locals )
{
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
  Preset.swap( Other.Preset );
  Global.swap( Other.Global );
  Locals.swap( Other.Locals );
}

// -----------------------------------------------------

void configParserStruct::named::clear( clearMode Mode )
{
  switch ( Mode )
  {
    case ClearAll:
      Global.clear();
      Locals.clear();
      Preset.clear();
      break;

    case KeepPreset:
      Global.clear();
      Locals.clear();
      break;

    case ClearConstStrings:
      Global.clearReferenceMap();
      Preset.clearReferenceMap();
      for ( std::list< namedFrame >::iterator it = Locals.begin(); it != Locals.end(); ++it )
        it->clearReferenceMap();
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
  namedFrame *Frame = frameForScope(Scope);
  return Frame->setValueByName( Name, Value );
}

// -----------------------------------------------------

configParserStruct::variable* configParserStruct::named::findValueByName( const std::string &Name, scope Scope ) const
{
  variable *Result = NULL;
  const namedFrame *Frame = NULL;
  
  Frame = frameForScope(Scope);
  Result = Frame->findValueByName( Name );

  if ( Result == NULL && Frame == &Global )
  {
    Frame = frameForScope(PresetScope);
    Result = Frame->findValueByName( Name );
  }

  return Result;
}

// -----------------------------------------------------
      
configParserStruct::variable* configParserStruct::named::setValueByReference( const char *Name, const variable &Value, scope Scope )
{
  namedFrame *Frame = frameForScope(Scope);
  return Frame->setValueByReference( Name, Value );
}

// -----------------------------------------------------

configParserStruct::variable* configParserStruct::named::findValueByReference( const char *Name, scope Scope ) const
{
  variable *Result = NULL;
  const namedFrame *Frame = NULL;
  
  Frame = frameForScope(Scope);
  Result = Frame->findValueByReference( Name );

  if ( Result == NULL && Frame == &Global )
  {
    Frame = frameForScope(PresetScope);
    Result = Frame->findValueByReference( Name );
  }

  return Result;
}

// -----------------------------------------------------

std::string configParserStruct::named::toDebugString() const
{
  std::string Result;

  Result += Preset.toDebugString( "Preset" );
  Result += Global.toDebugString( "Global" );

  int Index = 0;
  for ( std::list< namedFrame >::const_iterator it = Locals.begin(); it != Locals.end(); ++it )
  {
    char NumberString[64];
    std::snprintf( NumberString, sizeof(NumberString), "Local %i", Index );
    Result += it->toDebugString( NumberString );
    Index++;
  }

  return Result;
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
  assert( ! Stack.empty() );
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
    deleteMap();
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
    deleteMap();
    throw;
  }
}

// -----------------------------------------------------
      
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
  deleteMap();
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
      
void configParserStruct::buildins::deleteMap()
{
  for ( std::map< std::string, const buildInFunction* >::iterator it = Map.begin(); it != Map.end(); ++it )
    delete it->second;

  Map.clear();
  ReferenceMap.clear();
}

// -----------------------------------------------------

void configParserStruct::buildins::clear( named::clearMode Mode )
{
  switch ( Mode )
  {
    case named::ClearConstStrings:
      ReferenceMap.clear();
      break;

    default:
      break;
  }
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
  Buildins.clear( Mode );
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


