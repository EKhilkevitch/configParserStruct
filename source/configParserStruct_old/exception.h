
#pragma once

// =====================================================

#include <exception>
#include <string>

// =====================================================

namespace configParserStruct
{

  // =====================================================
  
  class exception : public std::exception
  {
    private:
      std::string What;

    public:
      explicit exception( const std::string &What );
      const char* what() const throw();
      ~exception() throw();
  };
  
  // =====================================================

}

// =====================================================

