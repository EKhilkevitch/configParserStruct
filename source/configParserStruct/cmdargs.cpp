
// =====================================================

#include "configParserStruct/cmdargs.h"

#include <cstring>
#include <cassert>
#include <cstdlib>
#include <stdexcept>
#include <iostream>

// =====================================================
                    
void configParserStruct::commandLineArgumentsParser::parsedArguments::insert( char ArgShortName, const char *Value ) 
{ 
  std::string String = ( Value == NULL ? "" : Value );
  Arguments[ ArgShortName ] = String;
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
      throw std::runtime_error( std::string("Invalid argument name ") + ArgShortName );
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
  if ( *End != '\0' )
  {
    if ( EnableException )
      throw std::runtime_error( std::string("Invalid value of argument '") + ArgShortName + "' : must be integer" );
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
  if ( *End != '\0' )
  {
    if ( EnableException )
      throw std::runtime_error( std::string("Invalid value of argument '") + ArgShortName + "' : must be integer" );
    return DefaultValue;
  }
  return Result;
}

// =====================================================
      
configParserStruct::commandLineArgumentsParser::commandLineArgumentsParser( const commandLineArgumentsParser &Parser )
{
  copyOptions( Parser.Options );
}

// -----------------------------------------------------

void configParserStruct::commandLineArgumentsParser::clearOptions()
{
  for ( std::vector< option >::const_iterator o = Options.begin(); o != Options.end(); ++o )
    delete o->name;
  Options.clear();
}

// -----------------------------------------------------

void configParserStruct::commandLineArgumentsParser::copyOptions( const std::vector< option > &Source )
{
  clearOptions();
  for ( std::vector< option >::const_iterator o = Source.begin(); o != Source.end(); ++o )
  {
    option NewOption = *o;
    NewOption.name = strdup( o->name );
    Options.push_back( NewOption );
  }
}

// -----------------------------------------------------

void configParserStruct::commandLineArgumentsParser::addOption( const std::string &FullName, const bool NeedParameter, const char ShortName )
{
  if ( FullName.empty() )
    return;

  if ( ShortName <= 0 )
    throw std::invalid_argument( "Value of ShortName must be > 0" );

  if ( isOptionExist(ShortName) )
    throw std::runtime_error( std::string() + "Option with short name '" + ShortName + "' already exist in list" );

  if ( isOptionExist(FullName) )
    throw std::runtime_error( std::string() + "Option with full name '" + FullName + "' already exist in list" );

  option NextOption = { strdup( FullName.c_str() ), NeedParameter, NULL, ShortName };
  Options.push_back( NextOption );
}

// -----------------------------------------------------

bool configParserStruct::commandLineArgumentsParser::isOptionExist( char ShortName )
{
  for ( std::vector< option >::const_iterator o = Options.begin(); o != Options.end(); ++o )
    if ( configParserStruct::commandLineArgumentsParser::optionShortName(*o) == ShortName )
      return true;
  return false;
}

// -----------------------------------------------------

bool configParserStruct::commandLineArgumentsParser::isOptionExist( const std::string &FullName )
{
  for ( std::vector< option >::const_iterator o = Options.begin(); o != Options.end(); ++o )
    if ( configParserStruct::commandLineArgumentsParser::optionFullName(*o) == FullName )
      return true;
  return false;
}

// -----------------------------------------------------

std::string configParserStruct::commandLineArgumentsParser::shortOptions() const
{
  std::string Result;
  for ( std::vector< option >::const_iterator o = Options.begin(); o != Options.end(); ++o )
  {
    Result += optionShortName(*o);
    if ( optionHasArgument(*o) )
      Result += ':';
  }
  return Result;
}

// -----------------------------------------------------

std::vector<configParserStruct::commandLineArgumentsParser::option> configParserStruct::commandLineArgumentsParser::longOptions() const
{
  std::vector< option > Result;
 
  for ( std::vector< option >::const_iterator opt = Options.begin(); opt != Options.end(); ++opt )
    Result.push_back( *opt );

  option LastOption = { NULL, false, NULL, '\0' };
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
    throw std::invalid_argument("argv == NULL");
  
  std::string ShortOptions = shortOptions();
  std::vector<option> LongOptions = longOptions();

  while ( true )
  {
    char *ShortOptsCStr = strdup( ShortOptions.c_str() );
    int NextOption = getopt_long( argc, argv, ShortOptsCStr, LongOptions.data(), NULL );
    free( ShortOptsCStr );

    if ( NextOption == -1 )
      break;

    if ( NextOption == '?' )
    {
      ParsingResults->setFoundUnknownOption();
      continue;
    }

    ParsingResults->insert( NextOption, optarg );
  };
}

// -----------------------------------------------------

void configParserStruct::commandLineArgumentsParser::setArgumentsFromFileList( parsedArguments *ParsingResults, int argc, char *argv[] ) const
{
  assert( ParsingResults != NULL );
  assert( argv != NULL );

  for ( int i = optind; i < argc; i++ )
    if ( argv[i] != NULL )
      ParsingResults->pushFileArgument( argv[i] );
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

  unsigned i = 0;
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

  parsedArguments Result = parse( argc, argv );

  for ( unsigned i = 0; i < argc; i++ )
    delete [] argv[i];
  delete [] argv;

  return Result;
}

// =====================================================

