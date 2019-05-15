
// =====================================================

#include "configParserStruct/cmdargs.h"

#include <cstring>
#include <cassert>
#include <cstdlib>
#include <stdexcept>
#include <iostream>
#include <getopt.h>

// =====================================================

namespace 
{

  // -----------------------------------------------------
  
  // Replacement of strdup becouse of icc problems on Win32 with this.
  char* stringCopy( const char *String )
  {
    if ( String == NULL )
      return NULL;

    size_t Length = strlen( String );
    char *CopyOfString = static_cast<char*>( malloc( Length + 1 ) );
    
    if ( CopyOfString == NULL )
      throw std::bad_alloc();

    std::strcpy( CopyOfString, String );
    return CopyOfString;
  }
  
  // -----------------------------------------------------

  option createOption( const configParserStruct::commandLineArgumentsParser::getoptOption &Option )
  {
    option Result = 
    {
      Option.Name.empty() ? NULL : stringCopy( Option.Name.c_str() ),
      Option.HasArg,
      NULL,
      Option.ShortName
    };

    return Result;
  }
  
  // -----------------------------------------------------

  void destroyOption( option *Option )
  {
    assert( Option != NULL );
    std::free( const_cast<char*>(Option->name) );
    std::memset( Option, 0, sizeof(*Option) );
  }
  
  // -----------------------------------------------------

  std::vector< option > createOptionsVector( const std::vector< configParserStruct::commandLineArgumentsParser::getoptOption > &Options )
  {
    std::vector< option > Result;
    for ( size_t i = 0; i < Options.size(); i++ )
      Result.push_back( createOption(Options[i]) );
    return Result;
  }
  
  // -----------------------------------------------------

  void destroyOptionsVector( std::vector<option> *Options )
  {
    assert( Options != NULL );
    for ( size_t i = 0; i < Options->size(); i++ )
      destroyOption( &( ( *Options )[i] ) );
    Options->clear();
  }

}

// =====================================================

configParserStruct::commandLineArgumentsParser::exception::exception( const std::string &What ) : 
  ::configParserStruct::exception(What) 
{
}
        
// -----------------------------------------------------

configParserStruct::commandLineArgumentsParser::invalidArgumentNameException::invalidArgumentNameException( const std::string &ArgName ) : 
  exception( "Invalid argument name '" + ArgName + "'" ) 
{
}

// -----------------------------------------------------
        
configParserStruct::commandLineArgumentsParser::invalidArgumentValueException::invalidArgumentValueException( const std::string &ArgName ) : 
  exception( "Value of argument '" + ArgName + "' must be integer" ) 
{
}

// -----------------------------------------------------
        
configParserStruct::commandLineArgumentsParser::doubleInsertingOptionException::doubleInsertingOptionException( const std::string &Name ) : 
  exception( "Option with name '" + Name + "' already exisis in list of options" ) 
{
}

// =====================================================
      
configParserStruct::commandLineArgumentsParser::parsedArguments::parsedArguments( const std::string &Name ) : 
  ProgramName(Name), 
  FoundUnknownOption('\0') 
{
}

// -----------------------------------------------------
          
const std::string& configParserStruct::commandLineArgumentsParser::parsedArguments::programName() const 
{ 
  return ProgramName; 
}

// -----------------------------------------------------
                    
void configParserStruct::commandLineArgumentsParser::parsedArguments::insert( char ArgShortName, const char *Value ) 
{ 
  std::string String = ( Value == NULL ? "" : Value );
  Arguments[ ArgShortName ] = String;
}

// -----------------------------------------------------

void configParserStruct::commandLineArgumentsParser::parsedArguments::setFoundUnknownOption( char Char ) 
{ 
  FoundUnknownOption = Char;
}

// -----------------------------------------------------

void configParserStruct::commandLineArgumentsParser::parsedArguments::pushFileArgument( const std::string &Arg ) 
{ 
  ListOfFileArguments.push_back(Arg); 
}

// -----------------------------------------------------

bool configParserStruct::commandLineArgumentsParser::parsedArguments::exist( char ArgShortName ) const 
{ 
  return Arguments.find(ArgShortName) != Arguments.end(); 
}
          
// -----------------------------------------------------

char configParserStruct::commandLineArgumentsParser::parsedArguments::foundUnknownOption() const 
{ 
  return FoundUnknownOption; 
}

// -----------------------------------------------------

const std::list<std::string>& configParserStruct::commandLineArgumentsParser::parsedArguments::listOfFileArguments() const 
{ 
  return ListOfFileArguments; 
}

// -----------------------------------------------------

std::set<char> configParserStruct::commandLineArgumentsParser::parsedArguments::existingArguments() const
{
  std::set<char> Set;
  for ( std::map< char, std::string >::const_iterator a = Arguments.begin(); a != Arguments.end(); ++a )
    Set.insert( a->first );
  return Set;
}

// -----------------------------------------------------

std::string configParserStruct::commandLineArgumentsParser::parsedArguments::arg( char ArgShortName, const std::string &DefaultValue, bool EnableException ) const
{
  std::map< char, std::string >::const_iterator i = Arguments.find(ArgShortName); 
  if ( i == Arguments.end() )
  {
    if ( EnableException )
      throw invalidArgumentNameException( std::string() + ArgShortName );
    return DefaultValue;
  }
  return i->second; 
}

// -----------------------------------------------------

int configParserStruct::commandLineArgumentsParser::parsedArguments::argInt( char ArgShortName, const int DefaultValue, bool EnableException ) const
{
  char *End = NULL;
  const std::string ArgValue = arg(ArgShortName,std::string(),EnableException);
  int Result = strtol( ArgValue.c_str(), &End, 10 );
  assert( End != NULL );
  if ( *End != '\0' || ArgValue.empty() )
  {
    if ( EnableException )
      throw invalidArgumentValueException( std::string() + ArgShortName );
    return DefaultValue;
  }
  return Result;
}

// -----------------------------------------------------

double configParserStruct::commandLineArgumentsParser::parsedArguments::argDouble( char ArgShortName, const double DefaultValue, bool EnableException ) const
{
  char *End = NULL;
  const std::string ArgValue = arg(ArgShortName,std::string(),EnableException);
  double Result = strtod( ArgValue.c_str(), &End );
  assert( End != NULL );
  if ( *End != '\0' || ArgValue.empty() )
  {
    if ( EnableException )
      throw invalidArgumentValueException( std::string() + ArgShortName );
    return DefaultValue;
  }
  return Result;
}

// =====================================================

configParserStruct::commandLineArgumentsParser::commandLineArgumentsParser() 
{
}

// -----------------------------------------------------

void configParserStruct::commandLineArgumentsParser::addOption( const std::string &FullName, const bool NeedParameter, const char ShortName )
{
  if ( FullName.empty() )
    return;

  if ( ShortName <= 0 )
    throw std::invalid_argument( "Value of ShortName must be > 0" );

  if ( isOptionExist(ShortName) )
    throw doubleInsertingOptionException( std::string() + ShortName);

  if ( isOptionExist(FullName) )
    throw doubleInsertingOptionException( FullName );

  getoptOption NextOption = { FullName, NeedParameter, ShortName };
  Options.push_back( NextOption );
}

// -----------------------------------------------------

void configParserStruct::commandLineArgumentsParser::addOption( const std::string &FullName, const bool NeedParameter ) 
{ 
  if ( ! FullName.empty() ) 
    addOption( FullName, NeedParameter, FullName[0] ); 
}

// -----------------------------------------------------

bool configParserStruct::commandLineArgumentsParser::isOptionExist( char ShortName )
{
  for ( std::vector< getoptOption >::const_iterator o = Options.begin(); o != Options.end(); ++o )
    if ( configParserStruct::commandLineArgumentsParser::optionShortName(*o) == ShortName )
      return true;
  return false;
}

// -----------------------------------------------------

bool configParserStruct::commandLineArgumentsParser::isOptionExist( const std::string &FullName )
{
  for ( std::vector< getoptOption >::const_iterator o = Options.begin(); o != Options.end(); ++o )
    if ( configParserStruct::commandLineArgumentsParser::optionFullName(*o) == FullName )
      return true;
  return false;
}

// -----------------------------------------------------

size_t configParserStruct::commandLineArgumentsParser::numberOfOptions() const 
{ 
  return Options.size(); 
}

// -----------------------------------------------------

void configParserStruct::commandLineArgumentsParser::clearOptions() 
{ 
  Options.clear(); 
}

// -----------------------------------------------------

std::string configParserStruct::commandLineArgumentsParser::shortOptions() const
{
  std::string Result;
  for ( std::vector< getoptOption >::const_iterator o = Options.begin(); o != Options.end(); ++o )
  {
    Result += optionShortName(*o);
    if ( optionHasArgument(*o) )
      Result += ':';
  }
  return Result;
}

// -----------------------------------------------------

std::vector<configParserStruct::commandLineArgumentsParser::getoptOption> configParserStruct::commandLineArgumentsParser::longOptions() const
{
  std::vector< getoptOption > Result;
 
  for ( std::vector< getoptOption >::const_iterator opt = Options.begin(); opt != Options.end(); ++opt )
    Result.push_back( *opt );

  getoptOption LastOption = { std::string(), false, '\0' };
  Result.push_back( LastOption );

  return Result;
}

// -----------------------------------------------------

void configParserStruct::commandLineArgumentsParser::clearGetoptInternalIndexes() const
{
  optind = 0;
  opterr = 0;
}

// -----------------------------------------------------

void configParserStruct::commandLineArgumentsParser::setArgumentsFromParameters( parsedArguments *ParsingResults, int argc, char *argv[] ) const
{
  assert( ParsingResults != NULL );

  if ( argv == NULL )
    throw std::invalid_argument( "argv == NULL" );
  
  std::string ShortOptions = shortOptions();
  std::vector<option> LongOptions = createOptionsVector( longOptions() );

  while ( true )
  {
    char *ShortOptsCStr = stringCopy( ShortOptions.c_str() );
    int NextOption = getopt_long( argc, argv, ShortOptsCStr, &LongOptions[0], NULL );
    free( ShortOptsCStr );

    if ( NextOption == -1 )
      break;

    if ( NextOption == '?' )
    {
      ParsingResults->setFoundUnknownOption( static_cast<char>(optopt) );
      continue;
    }

    ParsingResults->insert( NextOption, optarg );
  };

  destroyOptionsVector( &LongOptions );
}

// -----------------------------------------------------

void configParserStruct::commandLineArgumentsParser::setArgumentsFromFileList( parsedArguments *ParsingResults, int argc, char *argv[] ) const
{
  assert( ParsingResults != NULL );
  assert( argv != NULL );

  for ( int i = optind; i < argc; i++ )
  {
    if ( argv[i] != NULL )
      ParsingResults->pushFileArgument( argv[i] );
  }
}

// -----------------------------------------------------

std::string configParserStruct::commandLineArgumentsParser::optionFullName( size_t i ) const 
{ 
  return i < numberOfOptions() ? optionFullName(Options[i]) : ""; 
}

// -----------------------------------------------------

bool configParserStruct::commandLineArgumentsParser::optionHasArgument( size_t i ) const 
{ 
  return i < numberOfOptions() ? optionHasArgument(Options[i]) : false; 
} 

// -----------------------------------------------------

char configParserStruct::commandLineArgumentsParser::optionShortName( size_t i ) const 
{ 
  return i < numberOfOptions() ? optionShortName(Options[i]) : '\0'; 
}

// -----------------------------------------------------

configParserStruct::commandLineArgumentsParser::parsedArguments configParserStruct::commandLineArgumentsParser::parse( int argc, char *argv[] ) const
{
  if ( argv == NULL || argc <= 0 )
    throw std::invalid_argument( "argv == NULL || argc <= 0" );
  
  parsedArguments Result( argv[0] );
  clearGetoptInternalIndexes();

#if _WIN32
  argc -= 1;
  argv += 1;
#endif

  setArgumentsFromParameters( &Result, argc, argv );
  setArgumentsFromFileList( &Result, argc, argv );
  return Result;
}

// -----------------------------------------------------

configParserStruct::commandLineArgumentsParser::parsedArguments configParserStruct::commandLineArgumentsParser::parse( const std::list< std::string > &Arguments ) const
{
  size_t argc = Arguments.size();
  char **argv = new char*[ argc + 1 ];

  size_t i = 0;
  std::list< std::string >::const_iterator a = Arguments.begin();
  while ( a != Arguments.end() )
  {
    argv[i] = new char[ a->size() + 1 ];
    std::strcpy( argv[i], a->c_str() );
    ++a;
    ++i;
  }
  assert( i == argc );
  argv[ argc ] = NULL;

  parsedArguments Result = parse( static_cast<int>(argc), argv );

  for ( size_t i = 0; i < argc; i++ )
    delete [] argv[i];
  delete [] argv;

  return Result;
}

// -----------------------------------------------------
      
configParserStruct::commandLineArgumentsParser::parsedArguments configParserStruct::commandLineArgumentsParser::operator()( int argc, char *argv[] ) const 
{ 
  return parse(argc,argv); 
}

// -----------------------------------------------------

configParserStruct::commandLineArgumentsParser::parsedArguments configParserStruct::commandLineArgumentsParser::operator()(  const std::list< std::string > &Arguments ) const 
{ 
  return parse(Arguments); 
}

// =====================================================

