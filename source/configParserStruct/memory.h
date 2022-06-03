
#pragma once

// =====================================================

#include "configParserStruct/variable.h"

#include <map>
#include <vector>
#include <list>
#include <iosfwd>

// =====================================================

namespace configParserStruct
{
  
  // =====================================================
  
  class buildInFunction;

  // =====================================================
 
  class named 
  {
    public:
      enum scope
      {
        LocalScope,
        GlobalScope
      };

    private:
      struct namedFrame
      {
        std::map< std::string, variable > Map;
        mutable std::map< const char*, variable* > ReferenceMap;
      };

      namedFrame Global;
      std::list< namedFrame > Locals;

    private:
      static variable* setValueByName( namedFrame *Frame, const std::string &Name, const variable &Value );
      static variable* findValueByName( const namedFrame &Frame, const std::string &Name );
      static variable* setValueByReference( namedFrame *Frame, const char *Name, const variable &Value );
      static variable* findValueByReference( const namedFrame &Frame, const char *Name );
      static void clear( namedFrame *Frame );
      static std::string toDebugString( const namedFrame &Frame, const std::string &Prefix );
      static void copy( namedFrame *To, const namedFrame &From );

      const namedFrame* frameForScope( scope Scope ) const;
      namedFrame* frameForScope( scope Scope );

    public:
      named();
      named( const named &Other );
      named& operator=( const named &Other );

      void swap( named &Other );

      void pushLocal();
      void popLocal();

      variable* setValueByName( const std::string &Name, const variable &Value, scope Scope );
      variable* findValueByName( const std::string &Name, scope Scope ) const;
      
      variable* setValueByReference( const char *Name, const variable &Value, scope Scope );
      variable* findValueByReference( const char *Name, scope Scope ) const;

      void clear();
      std::string toDebugString() const;
  };

  // =====================================================
 
  class stack
  {
    private:
      mutable std::vector< variable > Stack;

    public:
      stack();
      void swap( stack &Other );

      void push( const variable &Variable );
      const variable pop();

      variable* valueByShift( size_t Index ) const;
      variable* top() const;

      bool empty() const;
      size_t size() const;
      void truncate( size_t Size );

      void clear();
      std::string toDebugString() const;
  }; 
  
  // =====================================================

  class registers
  {
    private:
      size_t InstructionPointer;
      size_t BaseStackPointer;
      bool UseBaseStackPointer;
      bool HaltFlag;
      variable LastResult;

    public:
      registers();

      void swap( registers &Other );
      
      void setLastResult( const variable &Variable );
      const variable& lastResult() const;
      
      void jumpToNextCommand();
      void jumpToCommand( size_t Index );
      size_t instructionPointer() const;

      void setBaseStackPointer( size_t Size );
      size_t baseStackPointer() const;
      bool useBaseStackPointer() const;
      void setUseBaseStackPointer( bool Set );

      void setHaltFlag();
      bool isHalted() const;
      
      void reset();
      std::string toDebugString() const;
  };

  // =====================================================
  
  class buildins
  {
    private:
      static double sqr( double x );
      static double min( double a, double b );
      static double max( double a, double b );

    private:
      std::map< std::string, const buildInFunction* > Map;
      mutable std::map< const char*, const buildInFunction* > ReferenceMap;

    private:
      void initMap();
      void clear();

    public:
      buildins();
      buildins( const buildins &Other );
      buildins& operator=( const buildins &Other );
      ~buildins();
      void swap( buildins &Other );

      const buildInFunction* findFunctionByName( const std::string &Name ) const;
      const buildInFunction* findFunctionByReference( const char *Name ) const;
  };

  // =====================================================
  
  class memory
  {
    public:
      typedef named::scope scope;

    private:
      stack Stack;
      registers Registers;
      named Named;
      buildins Buildins;

    public:
      void swap( memory &Other );

      void pushToStack( const variable &Variable );
      variable popFromStack();
      
      void setLastResult( const variable &Variable );
      const variable& lastResult() const;
      
      size_t stackSize() const;
      void truncateStack( size_t Size );
      
      void setBaseStackPointer( size_t Size );
      size_t baseStackPointer() const;
      bool useBaseStackPointer() const;
      void setUseBaseStackPointer( bool Set );
      void pushLocalNamedFrame();
      void popLocalNamedFrame();

      variable* findValueByName( const std::string &Name, scope Scope ) const;
      variable* findValueByReference( const char *Name, scope Scope ) const;
      variable* setValueByName( const std::string &Name, const variable &Value, scope Scope );
      variable* setValueByReference( const char *Name, const variable &Value, scope Scope );
      variable* findStackValueByShift( size_t Index ) const;
      variable* topStackValue() const;
      const buildInFunction* findFunctionByReference( const char *Name ) const;

      void jumpToNextCommand();
      void jumpToCommand( size_t Index );
      size_t instructionPointer() const;

      void setHaltFlag();
      bool isHalted() const;

      void clear();
      std::string toDebugString() const;
  };

  // =====================================================

}

