
#ifndef _STRUCTPARSERUTIL_H_
#define _STRUCTPARSERUTIL_H_ 1

// =====================================================

#if __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#define STRUCTPARSER_MAX_STRING_LENGTH ( 4096 )
#define STRUCTPARSER_MAX_ID_LENGTH     (  128 )

void structParserSetInputString( const char *String );
size_t getNextPartFromInputString( char *const Buffer, const size_t MaxSize );

#if __cplusplus
}
#endif

// =====================================================

#if __cplusplus

#include <string>

namespace configParserStruct
{
  class structParserInputString
  {
    private:
      std::string String;
      unsigned Position;

    public:
      structParserInputString( const char *S = NULL );

      void set( const char *Str );
      void reset() { Position = 0; }
      const std::string get() const { return String; }

      const std::string takeNextPart( size_t Size );
      size_t putNextPartToBuffer( char *Buffer, size_t Size );
  };
}
#endif

// =====================================================

#endif

