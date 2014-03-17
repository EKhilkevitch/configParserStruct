
// =====================================================

#include <string>
#include <cstdio>
#include <cstdlib>

#include <configParserStruct/structparser.h>

// =====================================================

void die( const std::string &Message )
{
  std::fprintf( stderr, "Error: %s\n", Message.c_str() );
  std::exit(1);
}

// -----------------------------------------------------

FILE* openInputFile( const std::string &FileName )
{
  if ( FileName.empty() )
    return stdin;

  FILE *File = std::fopen( FileName.c_str(), "r" );
  if ( File == NULL )
    die( std::string() + "Can not open file " + FileName );

  return File;
}

// -----------------------------------------------------

void closeInputFile( FILE *File )
{
  if ( File == NULL )
    return;
  if ( File == stdin )
    return;
  std::fclose(File);
}

// -----------------------------------------------------

std::string readFileContent( FILE *File )
{
  std::string Content;

  while ( true )
  {
    char Buffer[ 4096 ];
    Buffer[0] = Buffer[ sizeof(Buffer)-1 ] = '\0';
    char *Result = std::fgets( Buffer, sizeof(Buffer)-1, File );
    if ( Result == NULL )
      break;

    Content += Buffer;
  }

  return Content;
}

// =====================================================

int main( int argc, char *argv[] )
{
  FILE *File = openInputFile( argc == 2 ? argv[1] : "" );

  std::string FileContent = readFileContent( File );

  configParserStruct::structParser Parser;
  Parser.build( FileContent );
  Parser.run();

  closeInputFile( File );

  return 0;
}

// =====================================================

