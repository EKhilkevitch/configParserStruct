
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
        PresetScope,
        LocalScope,
        GlobalScope
      };

      enum clearMode
      {
        ClearAll,
        KeepPreset
      };

    private:
      struct namedFrame
      {
        std::map< std::string, variable > Map;
        mutable std::map< const char*, variable* > ReferenceMap;
      };

      namedFrame Preset;
      namedFrame Global;
      std::list< namedFrame > Locals;

    private:
      static inline variable* setValueByName( namedFrame *Frame, const std::string &Name, const variable &Value );
      static inline variable* findValueByName( const namedFrame &Frame, const std::string &Name );
      static inline variable* setValueByReference( namedFrame *Frame, const char *Name, const variable &Value );
      static inline variable* findValueByReference( const namedFrame &Frame, const char *Name );
      static inline void clear( namedFrame *Frame );
      static std::string toDebugString( const namedFrame &Frame, const std::string &Prefix );
      static void copy( namedFrame *To, const namedFrame &From );

      inline const namedFrame* frameForScope( scope Scope ) const;
      inline namedFrame* frameForScope( scope Scope );

    public:
      named();
      named( const named &Other );
      named& operator=( const named &Other );

      void swap( named &Other );

      void pushLocal();
      void popLocal();

      inline variable* setValueByName( const std::string &Name, const variable &Value, scope Scope );
      inline variable* findValueByName( const std::string &Name, scope Scope ) const;
      
      inline variable* setValueByReference( const char *Name, const variable &Value, scope Scope );
      inline variable* findValueByReference( const char *Name, scope Scope ) const;

      void clear( clearMode Mode );
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

      inline void push( const variable &Variable );
      inline const variable pop();

      inline variable* valueByShift( size_t Index ) const;
      inline variable* top() const;

      inline bool empty() const;
      inline size_t size() const;
      inline void truncate( size_t Size );

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
      
      inline void setLastResult( const variable &Variable );
      inline const variable& lastResult() const;
      
      inline void jumpToNextCommand();
      inline void jumpToCommand( size_t Index );
      inline size_t instructionPointer() const;

      void setBaseStackPointer( size_t Size );
      size_t baseStackPointer() const;
      bool useBaseStackPointer() const;
      void setUseBaseStackPointer( bool Set );

      void setHaltFlag();
      inline bool isHalted() const;
      
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

      inline const buildInFunction* findFunctionByName( const std::string &Name ) const;
      inline const buildInFunction* findFunctionByReference( const char *Name ) const;
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

      void clear( named::clearMode Mode );

      std::string toDebugString() const;
  };

  // =====================================================

}

