
#pragma once

// =====================================================

//#include "configParserStruct/command.h"

#include <vector>
#include <set>
#include <string>
#include <iosfwd>

// =====================================================

namespace configParserStruct
{
  
  // =====================================================
  
  class command;

  // =====================================================

  class text
  {
    public:
      typedef const command* const_iterator;

    public:
      static const int SuccesssErrorLine;

    private:
      static const char PlaceholderByte;
      static const size_t InvalidPlaceholderIndex;

    private:
      std::vector< char > Text;
      std::vector< std::vector<char> > Strings;

      struct parseTimeStatus
      {
        int ErrorLine;
        int FunctionLevel;
        std::vector< size_t > CallArgsCount;
        std::set< const char* > Variables;
      } ParseTimeStatus;

    private:
      inline size_t sizeOfText() const;
      inline command* commandPointer( size_t Index );
      inline const command* commandPointer( size_t Index ) const;
      static bool isCommandPointerPlaceholder( const command *Command );

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
      void replaceCommand( size_t Index, const command &Command );
      const char *constStringRef( const char *String );
      void setErrorLine( int Line );

      void parseTimeIncFunctionLevel();
      void parseTimeDecFunctionLevel();
      void parseTimePushCallArgs();
      void parseTimeIncCallArgs();
      size_t parseTimeCountOfCallArgs() const;
      void parseTimePopCallArgs();

      void addToVariablesSet( const char *Name, bool Force = false );
      void eraseFromVariablesSet( const char *Name );

      void clear();

      size_t size() const;
      bool empty() const;

      bool isError() const;
      int errorLine() const;

      std::set<std::string> variables() const;
      
      const command& operator[]( size_t Index ) const;

      const_iterator begin() const;
      const_iterator end() const;

      std::string toDebugString() const;
  };

  // =====================================================
  
}

