
#pragma once

// =====================================================

#include <string>
#include <vector>
#include <iosfwd>

// =====================================================

namespace configParserStruct
{

  // =====================================================
  
  class reference;

  // =====================================================
  
  class variable
  {
    public:
      enum collectionType
      {
        ArrayCollection,
        DictCollection
      };

    private:

#define SIZEMAX( A, B ) ( (A) > (B) ? (A) : (B) )
      struct valueMemory
      {
        char _[ sizeof(void*) + 
          SIZEMAX( sizeof(int), 
            SIZEMAX( sizeof(double), 
              SIZEMAX( sizeof(std::vector<variable>), 
                SIZEMAX( sizeof(size_t) + sizeof(void*) + sizeof(void*), sizeof(void*) )
            ) ) )
              ];
      } ValueMemory;

    private:
      template < class T > inline T* variableValuePointer();
      template < class T > inline const T* variableValuePointer() const;

    public:
      variable();
      variable( const variable &Variable );
      variable& operator=( const variable &Variable );
      explicit variable( int Value );
      explicit variable( double Value );
      explicit variable( const char *Value );
      explicit variable( const std::string &Value );
      explicit variable( const reference &Reference );
      explicit variable( collectionType Type );
      ~variable();

      void swap( variable &Other );

      int integer() const;
      double real() const;
      bool boolean() const;
      std::string string() const;
      
      const reference& ref() const;
      variable* getByRef( const reference &Reference ) const;
      variable* setByRef( const reference &Reference, const variable &Variable );

      const char* type() const;
  };

  // =====================================================
  
  std::ostream& operator<<( std::ostream &Stream, const variable &Variable );

  // =====================================================
  
}

