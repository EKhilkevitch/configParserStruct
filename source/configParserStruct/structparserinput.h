
#ifndef _STRUCTPARSERINPUT_H_
#define _STRUCTPARSERINPUT_H_ 1

// =====================================================

#if __cplusplus
extern "C" {
#endif

#include <stdlib.h>

size_t getNextPartFromInputString( char *const Buffer, const size_t MaxSize );

#if __cplusplus
}
#endif

// =====================================================

#if __cplusplus

#include <string>

namespace configParserStruct
{
  namespace structParserUtil
  {
    
    // =====================================================

    void setInputString( const std::string &String );

    // =====================================================
    
    class inputString
    {
      private:
        std::string String;
        unsigned Position;

      public:
        inputString( const std::string &S = std::string() );

        void set( const std::string &Str );
        void reset() { Position = 0; }
        const std::string get() const { return String; }

        const std::string takeNextPart( size_t Size );
        size_t putNextPartToBuffer( char *Buffer, size_t Size );
    };
    
    // =====================================================
  }
}
#endif

// =====================================================

#endif

