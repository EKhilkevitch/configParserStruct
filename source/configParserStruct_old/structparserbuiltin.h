
#pragma once

// =====================================================

#include <stdexcept>
#include <string>

#include "configParserStruct/structparservars.h"

// =====================================================

namespace configParserStruct
{
  namespace structParserUtil
  {
    
    // =====================================================
    
    class namedBuiltIn : public builtinFunctionValue
    {
      private:
        std::string Name;

      public:
        explicit namedBuiltIn( const std::string &Name );
        ~namedBuiltIn();
        const std::string string() const;
    };
    
    // -----------------------------------------------------
    
    template <class T> class zeroArgumentBuiltIn : public namedBuiltIn
    {
      public:
        typedef T type;

      private:
        type Value;
      
      public: 
        zeroArgumentBuiltIn( const std::string &N, const type &V );
        zeroArgumentBuiltIn<T>* clone() const { return new zeroArgumentBuiltIn<type>(*this); }
        const variable execute( const program &Program ) const;
    };

    // -----------------------------------------------------
    
    template <class retT, class argT> class oneArgumentBuiltIn : public namedBuiltIn
    {
      public:
        typedef retT (*function)( argT );

      private:
        function Function;

      public:
        oneArgumentBuiltIn( const std::string &N, function F );
        oneArgumentBuiltIn<retT,argT>* clone() const { return new oneArgumentBuiltIn<retT,argT>(*this); }
        const variable execute( const program &Program ) const;
    };
    
    // -----------------------------------------------------
    
    template <class retT, class arg1T, class arg2T> class twoArgumentsBuiltIn : public namedBuiltIn
    {
      public:
        typedef retT (*function)( arg1T, arg2T );

      private:
        function Function;

      public:
        twoArgumentsBuiltIn( const std::string &N, function F );
        twoArgumentsBuiltIn<retT,arg1T,arg2T>* clone() const { return new twoArgumentsBuiltIn<retT,arg1T,arg2T>(*this); }
        const variable execute( const program &Program ) const;
    };

    // =====================================================

    class printBuiltIn : public namedBuiltIn
    {
      public: 
        printBuiltIn() : namedBuiltIn("print") {}
        const variable execute( const program &Program ) const;
        printBuiltIn* clone() const { return new printBuiltIn(*this); }
    };
    
    // -----------------------------------------------------
    
    class printlnBuiltIn : public namedBuiltIn
    {
      public: 
        printlnBuiltIn() : namedBuiltIn("println") {}
        const variable execute( const program &Program ) const;
        printlnBuiltIn* clone() const { return new printlnBuiltIn(*this); }
    };
    
    // -----------------------------------------------------
    
    class definedBuiltIn : public namedBuiltIn
    {
      public:
        definedBuiltIn() : namedBuiltIn("defined") {}
        const variable execute( const program &Program ) const;
        definedBuiltIn* clone() const { return new definedBuiltIn(*this); }
    };
    
    // -----------------------------------------------------
    
    class envBuiltIn : public namedBuiltIn
    {
      public:
        envBuiltIn() : namedBuiltIn("env") {}
        const variable execute( const program &Program ) const;
        envBuiltIn* clone() const { return new envBuiltIn(*this); }
    };
    
    // -----------------------------------------------------
    
    class debugProgramTextBuildIn : public namedBuiltIn
    {
      public:
        debugProgramTextBuildIn() : namedBuiltIn("debug_text") {}
        const variable execute( const program &Program ) const;
        debugProgramTextBuildIn* clone() const { return new debugProgramTextBuildIn(*this); }
    };
    
    // -----------------------------------------------------
    
    class debugProgramStackBuildIn : public namedBuiltIn
    {
      public:
        debugProgramStackBuildIn() : namedBuiltIn("debug_stack") {}
        const variable execute( const program &Program ) const;
        debugProgramStackBuildIn* clone() const { return new debugProgramStackBuildIn(*this); }
    };

    // =====================================================
   
    typedef zeroArgumentBuiltIn<double>                 mathZeroArgumentBuiltIn;
    typedef oneArgumentBuiltIn<double,double>           mathOneArgumentBuiltIn;
    typedef twoArgumentsBuiltIn<double,double,double>   mathTwoArgumentsBuiltIn;

    template <class builtIn> builtIn makeBuiltIn( const std::string &Name, typename builtIn::function Function );
    template <class builtIn> builtIn makeBuiltIn( const std::string &Name, const typename builtIn::type &Value );

    // =====================================================
        
    template <class type> zeroArgumentBuiltIn<type>::zeroArgumentBuiltIn( const std::string &N, const type &V ) : 
      namedBuiltIn(N), 
      Value(V) 
    {
    }
    
    // -----------------------------------------------------
    
    template <class type> const variable zeroArgumentBuiltIn<type>::execute( const program& ) const
    {
      return createVariable( Value );
    }
    
    // -----------------------------------------------------
    
    template <class retT, class argT> oneArgumentBuiltIn<retT,argT>::oneArgumentBuiltIn( const std::string &N, function F ) :
      namedBuiltIn(N),
      Function(F)
    {
      if ( Function == NULL )
        throw std::invalid_argument("Function must be not NULL");
    }
    
    // -----------------------------------------------------
    
    template <class retT, class argT> const variable oneArgumentBuiltIn<retT,argT>::execute( const program &Program ) const
    {
      const variable &ArgVariable = getArgument( 1, Program );
      argT Arg = extractValueScalar<argT>( ArgVariable );
      retT Ret = Function( Arg );
      return createVariable<retT>( Ret );
    }
    
    // -----------------------------------------------------

    template <class retT, class arg1T, class arg2T> twoArgumentsBuiltIn<retT,arg1T,arg2T>::twoArgumentsBuiltIn( const std::string &N, function F ) :
      namedBuiltIn(N),
      Function(F)
    {
      if ( Function == NULL )
        throw std::invalid_argument("Function must be not NULL");
    }
    
    // -----------------------------------------------------

    template <class retT, class arg1T, class arg2T> const variable twoArgumentsBuiltIn<retT,arg1T,arg2T>::execute( const program &Program ) const
    {
      const variable &Arg1Variable = getArgument( 1, Program );
      arg1T Arg1 = extractValueScalar<arg1T>( Arg1Variable );
      
      const variable &Arg2Variable = getArgument( 2, Program );
      arg2T Arg2 = extractValueScalar<arg2T>( Arg2Variable );

      retT Ret = Function( Arg1, Arg2 );
      return createVariable<retT>( Ret );
    }

    // =====================================================
       
    template <class builtIn> builtIn makeBuiltIn( const std::string &Name, typename builtIn::function Function )
    {
      return builtIn( Name, Function );
    }
    
    // -----------------------------------------------------
    
    template <class builtIn> builtIn makeBuiltIn( const std::string &Name, const typename builtIn::type &Value )
    {
      return builtIn( Name, Value );
    }
    
    // =====================================================
    
  }
}

