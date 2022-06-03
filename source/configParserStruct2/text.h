
#pragma once

// =====================================================

#include "configParserStruct2/command.h"

#include <vector>
#include <iosfwd>

// =====================================================

namespace configParserStruct
{
  
  // =====================================================

  class text
  {
    public:
      typedef const command* const_iterator;

    private:
      static const int SuccesssErrorLine;
      static const char PlaceholderByte;

    private:
      std::vector< char > Text;
      std::vector< char* > Strings;
      int ErrorLine;

    private:
      command* commandPointer( size_t Index );
      const command* commandPointer( size_t Index ) const;

    private:
      text& operator=( const text& );

    public:
      text();
      text( const text &Other );
      ~text();

      void push( const command &Command );
      void pushPlaceholder();
      void replacePlaceholder( const command &Command );
      size_t indexOfPlaceholder() const;
      void replaceLastCommand( const command &Command );
      const char *constStringRef( const char *String );
      void setErrorLine( int Line );

      void clear();

      size_t size() const;
      bool empty() const;

      bool isError() const;
      int errorLine() const;
      
      const command& operator[]( size_t Index ) const;

      const_iterator begin() const;
      const_iterator end() const;
  };

  // =====================================================
  
}

