
#pragma once

// =====================================================

#include <vector>
#include <string>

// =====================================================

namespace configParserStruct
{

  // =====================================================
  
  class variable;

  // =====================================================
 
  class reference
  {
    friend std::ostream& operator<<( std::ostream &Stream, const reference &Reference );

    public:
      enum type
      {
        None,
        GlobalName,
        LocalName,
        ArgumentIndex,
        ArrayIndex,
        DictKey,
        InstructionPointer,
        StackPointer
      };

    private:
      type Type;

      union value
      {
        const char *ConstChar;
        size_t SizeT;
      } Value;

      reference *Next;

    public:
      reference();
      explicit reference( const char *Value, type Type );
      explicit reference( size_t Index, type Type );
      reference( const reference &Reference );
      reference& operator=( const reference &Reference );
      ~reference();
      void swap( reference &Other );

      bool hasType( type Type ) const;

      const reference* next() const;
      void setAsTail( const reference &Reference );

      const char* asGlobalName() const;
      const char* asLocalName() const;
      const char* asDictKey() const;
      size_t asArrayIndex() const;
      size_t asArgumentIndex() const;
      size_t asInstructionPointer() const;
      size_t asStackPointer() const;

      std::string toDebugString() const;
  };

  // =====================================================
  
}

