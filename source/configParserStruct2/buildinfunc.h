
#pragma once

// =====================================================

#include <cstddef>

// =====================================================

namespace configParserStruct
{
  // =====================================================
  
  class memory;
  class variable;

  // =====================================================
 
  class buildInFunction
  {
    protected:
      static size_t countOfArguments( const memory &Memory );
      static const variable& argumentValue( const memory &Memory, size_t Index );

    public:
      virtual variable call( const memory &Memory ) const = 0; 
      virtual ~buildInFunction();
  };

  // =====================================================
  
  class zeroargMathBuildInFunction : public buildInFunction
  {
    private:
      double Value;

    public:
      explicit zeroargMathBuildInFunction( double Value );
      variable call( const memory &Memory ) const;
  };

  // =====================================================
  
  class oneargMathBuildInFunction : public buildInFunction 
  {
    public:
      typedef double (*function)( double );

    private:
      function Function;

    public:
      explicit oneargMathBuildInFunction( function Function );
      variable call( const memory &Memory ) const;
  };

  // =====================================================
  
  class twoargMathBuildInFunction : public buildInFunction 
  {
    public:
      typedef double (*function)( double, double );

    private:
      function Function;

    public:
      explicit twoargMathBuildInFunction( function Function );
      variable call( const memory &Memory ) const;
  };

  // =====================================================
  
  class printBuildInFunction : public buildInFunction
  {
    public:
      variable call( const memory &Memory ) const;
  };

  // =====================================================
  
  class printlnBuildInFunction : public buildInFunction
  {
    public:
      variable call( const memory &Memory ) const;
  };

  // =====================================================
  
  class envBuildInFunction : public buildInFunction
  {
    public:
      variable call( const memory &Memory ) const;
  };

  // =====================================================
 
}

