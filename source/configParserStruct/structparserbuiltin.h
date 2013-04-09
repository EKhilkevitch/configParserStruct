
#pragma once

// =====================================================

#include <cmath>

#include "configParserStruct/structparservars.h"

// =====================================================

namespace configParserStruct
{
  namespace structParserUtil
  {
    
    // =====================================================
    
    class mathBuiltIn : public builtinFunctionValue
    {
      protected:
        virtual double calculate( double Argument ) const = 0;

      public:
        const variable execute( const program &Program ) const;
    };
    
    // -----------------------------------------------------
    
    class piBuiltIn : public mathBuiltIn
    {
      protected:
        double calculate( double ) const;

      public: 
        variableValue* clone() const { return new piBuiltIn(); }
        const std::string string() const { return "builtin:pi"; }
    };
    
    // -----------------------------------------------------
 
    class expBuiltIn : public mathBuiltIn
    {
      protected:
        double calculate( double Argument ) const { return std::exp( Argument ); }

      public: 
        variableValue* clone() const { return new expBuiltIn(); }
        const std::string string() const { return "builtin:exp"; }
    };
    
    // -----------------------------------------------------
    
    class sinBuiltIn : public mathBuiltIn
    {
      protected:
        double calculate( double Argument ) const { return std::sin( Argument ); }

      public: 
        variableValue* clone() const { return new sinBuiltIn(); }
        const std::string string() const { return "builtin:sin"; }
    };
    
    // -----------------------------------------------------
    
    class cosBuiltIn : public mathBuiltIn
    {
      protected:
        double calculate( double Argument ) const { return std::cos( Argument ); }

      public: 
        variableValue* clone() const { return new cosBuiltIn(); }
        const std::string string() const { return "builtin:cos"; }
    };

    // -----------------------------------------------------

    class printBuiltIn : public builtinFunctionValue
    {
      public: 
        const variable execute( const program &Program ) const;
        variableValue* clone() const { return new printBuiltIn(); }
        const std::string string() const { return "builtin:print"; }
    };

    // =====================================================
   
  }
}

