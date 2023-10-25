
#pragma once

// =====================================================

#include <vector>
#include <cstddef>

// =====================================================

namespace configParserStruct
{
  
  // =====================================================
  
  class text;
  class command;

  // =====================================================
 
  class optimizer
  {
    private:
      struct jumpCommandInfo
      {
        size_t Index;
        size_t Destination;

        jumpCommandInfo();
        jumpCommandInfo( size_t Index, size_t Destination );
      };

    private:
      static std::vector< jumpCommandInfo > jumpCommands( const text &Text );
      static void optimizeUminus( text *Text );
      static void optimizeAssign( text *Text );
      static const command* nextEffectiveCommand( const text &Text, size_t *Index );

    public:
      optimizer();

      void optimize( text *Text ) const;
      void operator()( text *Text ) const;
  };

  // =====================================================

}

