

#pragma once

// =====================================================

#include <string>
#include <sstream>

// =====================================================

namespace configParserStruct
{

  template <class T> inline std::string convertToString( const T &Value )
  {
    std::stringstream Stream;
    Stream << Value;
    return Stream.str();
  }

  template <class T> inline const T convertFromString( const std::string &String )
  {
    std::stringstream Stream;
    Stream << String;
    T Result = T();
    Stream >> Result;
    return Result;
  }

}

// =====================================================

