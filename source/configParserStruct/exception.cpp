
// =====================================================

#include "configParserStruct/exception.h"

// =====================================================

configParserStruct::exception::exception( const std::string &What ) :
  std::runtime_error( What )
{
}

// -----------------------------------------------------

configParserStruct::exception::~exception() throw()
{
}

// =====================================================

