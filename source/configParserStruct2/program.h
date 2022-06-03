
#pragma once

// =====================================================

#include "configParserStruct2/memory.h"
#include "configParserStruct2/text.h"

// =====================================================

namespace configParserStruct
{
  // =====================================================
 
  class program
  {
    private:
      text Text;
      memory Memory;

    public:
      program();
      ~program();

      void clear();
      void build( const std::string &SourceCode );
      void run();

      const text& programText() const;
      const memory& programMemory() const;
      memory& programMemory();
  };

  // =====================================================
}

// =====================================================

