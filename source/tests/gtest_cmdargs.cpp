
// =========================================================

#include <gtest/gtest.h>

#include "configParserStruct/cmdargs.h"

using namespace configParserStruct;

// =========================================================

static commandLineArgumentsParser createParser()
{
  commandLineArgumentsParser Parser;
  Parser.addOption( "help", false, 'h' );
  Parser.addOption( "input", true );
  Parser.addOption( "output", true, 'O' );
  Parser.addOption( "verbose", false );
  Parser.addOption( "integer", true, 'I' );
  Parser.addOption( "double", true, 'D' );
  return Parser;
}

// =========================================================

TEST( commandLineArgumentsParser, constructor ) 
{
  commandLineArgumentsParser *Parser = new commandLineArgumentsParser();
  Parser->addOption( "first", true );
  Parser->addOption( "second", false );
  Parser->addOption( "third", true, 'T' );

  commandLineArgumentsParser Parser2 = *Parser;
  delete Parser;
  Parser = NULL;

  ASSERT_EQ( 3, Parser2.numberOfOptions() );
  ASSERT_TRUE( Parser2.optionFullName(0)    == "first" );
  ASSERT_TRUE( Parser2.optionHasArgument(1) == false );
  ASSERT_TRUE( Parser2.optionFullName(2)    == "third" );
  ASSERT_TRUE( Parser2.optionShortName(2)   == 'T' );
  ASSERT_TRUE( Parser2.optionHasArgument(2) == true );
}

// ---------------------------------------------------------

TEST( commandLineArgumentsParser, isOptionExist )
{
  commandLineArgumentsParser Parser = createParser();

  ASSERT_TRUE( Parser.isOptionExist('i') );
  ASSERT_TRUE( Parser.isOptionExist("input") );
  ASSERT_TRUE( Parser.isOptionExist('O') );
  
  ASSERT_TRUE( ! Parser.isOptionExist("ixxx") );
  ASSERT_TRUE( ! Parser.isOptionExist('Z') );
}

// ---------------------------------------------------------

TEST( commandLineArgumentsParser, addOption ) 
{
  commandLineArgumentsParser Parser = createParser();

  ASSERT_TRUE( Parser.numberOfOptions()    == 6 );
  ASSERT_TRUE( Parser.optionFullName(0)    == "help" );
  ASSERT_TRUE( Parser.optionHasArgument(0) == false );
  ASSERT_TRUE( Parser.optionShortName(0)   == 'h' );

  ASSERT_TRUE( Parser.optionHasArgument(1) == true );
  ASSERT_TRUE( Parser.optionShortName(1)   == 'i' );

  ASSERT_TRUE( Parser.optionFullName(2)  == "output" );
  ASSERT_TRUE( Parser.optionShortName(2) == 'O' );
  
  ASSERT_TRUE( Parser.optionShortName(3) == 'v' );

  ASSERT_TRUE( Parser.optionFullName(7) == "" );

  try
  {
    Parser.addOption( "input", true );
    FAIL() << "Existing option added";;
  } catch ( std::runtime_error ) {}

  try
  {
    Parser.addOption( "ixxxx", true );
    FAIL() << "Option with first letter 'i' added";;
  } catch ( std::runtime_error ) {}
    
  Parser.addOption( "ixxxx", true, 'X' );
}

// ---------------------------------------------------------

TEST( commandLineArgumentsParser, parseOneOptionWithoutArg )
{
  commandLineArgumentsParser Parser = createParser();

  std::list< std::string > CommandLineArguments;
  CommandLineArguments.push_back("program_name");
  CommandLineArguments.push_back( "--help" );

  commandLineArgumentsParser::parsedArguments ParsedArguments = Parser( CommandLineArguments );
  ASSERT_TRUE( ParsedArguments.programName() == "program_name" );
  ASSERT_TRUE( ParsedArguments.exist('h') );
  ASSERT_TRUE( ParsedArguments.arg('h') == "" );
  ASSERT_TRUE( ! ParsedArguments.exist('v') );
  ASSERT_TRUE( ! ParsedArguments.exist('i') );

  ASSERT_TRUE( ! ParsedArguments.exist('Z') );
  
  ASSERT_TRUE( ParsedArguments.arg('Z')      == "" );
  ASSERT_TRUE( ParsedArguments.arg('Z',"aa") == "aa" );
  try
  {
    ParsedArguments.arg('Z',"aa",true);
    FAIL() << "No exception";;
  } catch ( std::runtime_error ) {}
  ASSERT_TRUE( ParsedArguments.arg('Z',"aa",false) == "aa" );

  ASSERT_EQ( '\0', ParsedArguments.foundUnknownOption() );
  ASSERT_TRUE( ParsedArguments.listOfFileArguments().empty() );

}

// ---------------------------------------------------------

TEST( commandLineArgumentsParser, parseOneOptionWithArg )
{
  commandLineArgumentsParser Parser = createParser();

  std::list< std::string > CommandLineArguments;
  CommandLineArguments.push_back("program_name");
  CommandLineArguments.push_back( "--input" );
  CommandLineArguments.push_back( "filename" );

  commandLineArgumentsParser::parsedArguments ParsedArguments = Parser( CommandLineArguments );
  ASSERT_TRUE( ! ParsedArguments.exist('h') );
  ASSERT_TRUE( ParsedArguments.exist('i') );
  ASSERT_TRUE( ParsedArguments.arg('i')           == "filename" );
  ASSERT_TRUE( ParsedArguments.arg('i',"aa")      == "filename" );
  ASSERT_TRUE( ParsedArguments.arg('i',"aa",true) == "filename" );
  
  ASSERT_EQ( '\0', ParsedArguments.foundUnknownOption() );
  ASSERT_TRUE( ParsedArguments.listOfFileArguments().empty() );
}

// ---------------------------------------------------------

TEST( commandLineArgumentsParser, parseShortOption )
{
  commandLineArgumentsParser Parser = createParser();

  std::list< std::string > CommandLineArguments;
  CommandLineArguments.push_back("program_name");
  CommandLineArguments.push_back( "-i" );
  CommandLineArguments.push_back( "filename" );
  CommandLineArguments.push_back( "-h" );

  commandLineArgumentsParser::parsedArguments ParsedArguments = Parser( CommandLineArguments );
  ASSERT_TRUE( ParsedArguments.exist('h') );
  ASSERT_TRUE( ParsedArguments.exist('i') );
  ASSERT_EQ( "filename", ParsedArguments.arg('i') );
  ASSERT_FALSE( ParsedArguments.exist('v') );

  auto OptionsSet = ParsedArguments.existingArguments();
  ASSERT_EQ( 2, OptionsSet.size() );
  ASSERT_TRUE( OptionsSet.find('i') != OptionsSet.end() );
  ASSERT_TRUE( OptionsSet.find('h') != OptionsSet.end() );
  ASSERT_TRUE( OptionsSet.find('v') == OptionsSet.end() );
}

// ---------------------------------------------------------

TEST( commandLineArgumentsParser, parseLongOption )
{
  commandLineArgumentsParser Parser = createParser();

  std::list< std::string > CommandLineArguments;
  CommandLineArguments.push_back("program_name");
  CommandLineArguments.push_back( "--input" );
  CommandLineArguments.push_back( "filename" );
  CommandLineArguments.push_back( "--help" );

  commandLineArgumentsParser::parsedArguments ParsedArguments = Parser( CommandLineArguments );
  ASSERT_TRUE( ParsedArguments.exist('h') );
  ASSERT_TRUE( ParsedArguments.exist('i') );
  ASSERT_EQ( "filename", ParsedArguments.arg('i') );
  ASSERT_FALSE( ParsedArguments.exist('v') );
}

// ---------------------------------------------------------

TEST( commandLineArgumentsParser, parseInvalidOption )
{
  commandLineArgumentsParser Parser = createParser();
  
  std::list< std::string > CommandLineArguments;
  CommandLineArguments.push_back("program_name");
  CommandLineArguments.push_back( "-X" );

  commandLineArgumentsParser::parsedArguments ParsedArguments = Parser( CommandLineArguments );
  ASSERT_EQ( 'X', ParsedArguments.foundUnknownOption() );
  ASSERT_TRUE( ParsedArguments.listOfFileArguments().empty() );

  CommandLineArguments.clear();
  CommandLineArguments.push_back("program_name");
  CommandLineArguments.push_back( "--verbose" );
  CommandLineArguments.push_back( "-X" );
  CommandLineArguments.push_back( "xvalue" );

  ParsedArguments = Parser( CommandLineArguments );
  ASSERT_EQ( 'X', ParsedArguments.foundUnknownOption() );
  ASSERT_TRUE( ParsedArguments.exist('v') );
  ASSERT_EQ( 1, ParsedArguments.listOfFileArguments().size() );
  ASSERT_EQ( "xvalue", ParsedArguments.listOfFileArguments().front() );
}

// ---------------------------------------------------------

TEST( commandLineArgumentsParser, parseOptionWithFile )
{
  commandLineArgumentsParser Parser = createParser();

  std::list< std::string > CommandLineArguments;
  CommandLineArguments.push_back("program_name");
  CommandLineArguments.push_back( "-i" );
  CommandLineArguments.push_back( "inputname" );
  CommandLineArguments.push_back( "filename1" );
  CommandLineArguments.push_back( "filename2" );

  commandLineArgumentsParser::parsedArguments ParsedArguments = Parser( CommandLineArguments );
  ASSERT_TRUE( ParsedArguments.exist('i') );
  ASSERT_EQ( 2, ParsedArguments.listOfFileArguments().size() );
  ASSERT_EQ( "filename1", ParsedArguments.listOfFileArguments().front() );
  ASSERT_EQ( "filename2", ParsedArguments.listOfFileArguments().back()  );
  ASSERT_EQ( '\0', ParsedArguments.foundUnknownOption() );
}

// ---------------------------------------------------------

TEST( commandLineArgumentsParser, parseIntegerOption )
{
  auto Parser = createParser();
  
  std::list< std::string > CommandLineArguments;
  CommandLineArguments.push_back("program_name");
  CommandLineArguments.push_back( "-I" );
  CommandLineArguments.push_back( "5" );

  auto ParsedArguments = Parser( CommandLineArguments );
  ASSERT_EQ( "5", ParsedArguments.arg('I') );
  ASSERT_EQ( 5, ParsedArguments.argInt('I') );
}

// ---------------------------------------------------------

TEST( commandLineArgumentsParser, parseDoubleOption )
{
  auto Parser = createParser();
  
  std::list< std::string > CommandLineArguments;
  CommandLineArguments.push_back("program_name");
  CommandLineArguments.push_back( "-D" );
  CommandLineArguments.push_back( "5.3" );

  auto ParsedArguments = Parser( CommandLineArguments );
  ASSERT_TRUE( ParsedArguments.arg('D') == "5.3" );
  ASSERT_NEAR( 5.3, ParsedArguments.argDouble('D'), 1e-5 );
}

// ---------------------------------------------------------

TEST( commandLineArgumentsParser, parseInvalidIngegerOption )
{
  auto Parser = createParser();
  
  std::list< std::string > CommandLineArguments;
  CommandLineArguments.push_back("program_name");
  CommandLineArguments.push_back( "-I" );
  CommandLineArguments.push_back( "Text" );

  auto ParsedArguments = Parser( CommandLineArguments );
  ASSERT_EQ( 0, ParsedArguments.argInt('I') );
  ASSERT_EQ( 1, ParsedArguments.argInt('I',1) );
  try
  {
    ParsedArguments.argInt('I',1,true);
    FAIL() << "No exception";;
  } catch ( std::exception& ) {}
}

// ---------------------------------------------------------

TEST( commandLineArgumentsParser, parseInvalidDoubleOption )
{
  auto Parser = createParser();
  
  std::list< std::string > CommandLineArguments;
  CommandLineArguments.push_back("program_name");
  CommandLineArguments.push_back( "-D" );
  CommandLineArguments.push_back( "Text" );

  commandLineArgumentsParser::parsedArguments ParsedArguments = Parser( CommandLineArguments );
  ASSERT_NEAR( 0,   ParsedArguments.argDouble('D'),     1e-5 );
  ASSERT_NEAR( 1.5, ParsedArguments.argDouble('D',1.5), 1e-5 );
  try
  {
    ParsedArguments.argDouble('D',1.5,true);
    FAIL() << "No exception";;
  } catch ( std::exception& ) {}
}

// ---------------------------------------------------------

TEST( commandLineArgumentsParser, twoParsingWithOneParser )
{
  auto Parser = createParser();
  
  std::list< std::string > CommandLineArguments;
  CommandLineArguments.push_back("program_name");
  CommandLineArguments.push_back( "-i" );
  CommandLineArguments.push_back( "file1" );
  CommandLineArguments.push_back( "filename" );
  
  auto ParsedArguments = Parser( CommandLineArguments );

  CommandLineArguments.clear();
  CommandLineArguments.push_back("program_name2");
  CommandLineArguments.push_back( "-O" );
  CommandLineArguments.push_back( "file2" );
  
  ParsedArguments = Parser( CommandLineArguments );

  ASSERT_TRUE( ! ParsedArguments.exist('i') );
  ASSERT_TRUE(   ParsedArguments.exist('O') );
  ASSERT_TRUE(   ParsedArguments.arg('O') == "file2" );
  ASSERT_EQ( '\0', ParsedArguments.foundUnknownOption() );
  ASSERT_TRUE( ParsedArguments.listOfFileArguments().empty() );
}

// ---------------------------------------------------------

TEST( commandLineArgumentsParser, parseWithPointers )
{
  auto Parser = createParser();

  char argv1[] = "program_name", argv2[] = "-i", argv3[] = "file1", argv4[] = "-v";

  char* argv[] = { argv1, argv2, argv3, argv4, NULL };
  const int argc = sizeof(argv)/sizeof(argv[0]) - 1;
  
  auto ParsedArguments = Parser( argc, argv );
  
  ASSERT_TRUE( ParsedArguments.exist('i') );
  ASSERT_TRUE( ParsedArguments.arg('i') == "file1" );
  ASSERT_TRUE( ParsedArguments.exist('v') );
  ASSERT_TRUE( ! ParsedArguments.exist('h') );
  ASSERT_EQ( '\0', ParsedArguments.foundUnknownOption() );
  ASSERT_TRUE( ParsedArguments.listOfFileArguments().empty() );
}

// ---------------------------------------------------------

TEST( commandLineArgumentsParser, parse ) 
{
  auto Parser = createParser();

  std::list< std::string > CommandLineArguments;

  CommandLineArguments.clear();
  CommandLineArguments.push_back("program_name");
  CommandLineArguments.push_back( "--input" );
  CommandLineArguments.push_back( "in_file" );
  CommandLineArguments.push_back( "-O" );
  CommandLineArguments.push_back( "out_file" );
  CommandLineArguments.push_back( "-D" );
  CommandLineArguments.push_back( "12.4" );
  CommandLineArguments.push_back( "ext_file1" );
  CommandLineArguments.push_back( "ext_file2" );
  CommandLineArguments.push_back( "ext_file3" );

  auto ParsedArguments = Parser( CommandLineArguments );

  ASSERT_EQ( '\0', ParsedArguments.foundUnknownOption() );
  ASSERT_TRUE( ! ParsedArguments.exist('h') );
  ASSERT_TRUE( ! ParsedArguments.exist('v') );
  ASSERT_TRUE( ParsedArguments.exist('i') );
  ASSERT_TRUE( ParsedArguments.arg('i') == "in_file" );
  ASSERT_TRUE( ParsedArguments.arg('O') == "out_file" );
  ASSERT_NEAR( 12.4, ParsedArguments.argDouble('D'), 1e-5 );
  ASSERT_EQ( 0, ParsedArguments.argInt('I') );
  ASSERT_EQ( 4, ParsedArguments.argInt('I',4) );
  ASSERT_EQ( 3, ParsedArguments.listOfFileArguments().size() );
  ASSERT_TRUE( ParsedArguments.listOfFileArguments().back() == "ext_file3" );
}

// =========================================================

