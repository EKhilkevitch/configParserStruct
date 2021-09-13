

#pragma once

// =====================================================

#include <string>
#include <sstream>
#include <cstdio>

// =====================================================

namespace configParserStruct
{

  template <class T> inline std::string convertToString( const T &Value )
  {
    std::ostringstream Stream;
    Stream << Value;
    return Stream.str();
  }

  template <> inline std::string convertToString( const int &Value )
  {
    char Buffer[ 64 ];
    std::sprintf( Buffer, "%i", Value );
    return Buffer;
  }
  
  template <> inline std::string convertToString( const unsigned &Value )
  {
    char Buffer[ 64 ];
    std::sprintf( Buffer, "%u", Value );
    return Buffer;
  }
  
  template <> inline std::string convertToString( const double &Value )
  {
    char Buffer[ 64 ];
    std::sprintf( Buffer, "%g", Value );
    return Buffer;
  }

  template <class T> inline const T convertFromString( const std::string &String )
  {
    std::istringstream Stream(String);
    T Result = T();
    Stream >> Result;
    return Result;
  }
}

// =====================================================

