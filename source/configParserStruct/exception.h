
#pragma once

// =====================================================

#include <stdexcept>

// =====================================================

namespace configParserStruct
{

  // =====================================================
  
  class exception : public std::runtime_error
  {
    public:
      exception( const std::string &What );
      ~exception() throw();
  };
  
  // =====================================================

}

// =====================================================

