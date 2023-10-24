
#pragma once

// =====================================================

#include "configParserStruct/memory.h"
#include "configParserStruct/text.h"

// =====================================================

namespace configParserStruct
{
  // =====================================================
 
  class program
  {
    private:
      text *Text;
      int *TextRefCount;

      memory Memory;

    private:
      void detachAndClearTextIfNeed();
      void detachAndCopyTextIfNeed();

    public:
      program();
      program( const program &Program );
      program& operator=( const program &Program );
      ~program();
      void swap( program &Other );

      void clear();
      void build( const std::string &SourceCode );
      void optimize();
      void run();

      const text& programText() const;
      const memory& programMemory() const;
      memory& programMemory();
  };

  // =====================================================
}

// =====================================================

