
// =====================================================

#include "configParserStruct2/exception.h"

// =====================================================

configParserStruct::exception::exception( const std::string &W ) :
  What(W)
{
}

// -----------------------------------------------------

const char* configParserStruct::exception::what() const throw()
{
  return What.c_str();
}

// -----------------------------------------------------

configParserStruct::exception::~exception() throw()
{
}

// =====================================================

