
#pragma once

// =====================================================

#include "configParserStruct/command.h"

// =====================================================

namespace configParserStruct
{

  // =====================================================
  
  class addCommand : public command
  {
    public:
      void exec( memory *Memory ) const;
      std::string toString() const;
      addCommand* clone( void *Memory = NULL ) const;
  };

  // =====================================================
  
  class subCommand : public command
  {
    public:
      void exec( memory *Memory ) const;
      std::string toString() const;
      subCommand* clone( void *Memory = NULL ) const;
  };

  // =====================================================
  
  class mulCommand : public command
  {
    public:
      void exec( memory *Memory ) const;
      std::string toString() const;
      mulCommand* clone( void *Memory = NULL ) const;
  };

  // =====================================================
  
  class divCommand : public command
  {
    public:
      void exec( memory *Memory ) const;
      std::string toString() const;
      divCommand* clone( void *Memory = NULL ) const;
  };

  // =====================================================
  
  class modCommand : public command
  {
    public:
      void exec( memory *Memory ) const;
      std::string toString() const;
      modCommand* clone( void *Memory = NULL ) const;
  };

  // =====================================================
  
  class unminusCommand : public command
  {
    public:
      void exec( memory *Memory ) const;
      std::string toString() const;
      unminusCommand* clone( void *Memory = NULL ) const;
  };

  // =====================================================
  
  class boolAndCommand : public command
  {
    public:
      void exec( memory *Memory ) const;
      std::string toString() const;
      boolAndCommand* clone( void *Memory = NULL ) const;
  };

  // =====================================================
  
  class boolOrCommand : public command
  {
    public:
      void exec( memory *Memory ) const;
      std::string toString() const;
      boolOrCommand* clone( void *Memory = NULL ) const;
  };

  // =====================================================
  
  class boolNotCommand : public command
  {
    public:
      void exec( memory *Memory ) const;
      std::string toString() const;
      boolNotCommand* clone( void *Memory = NULL ) const;
  };

  // =====================================================
  
  class strAddCommand : public command
  {
    public:
      void exec( memory *Memory ) const;
      std::string toString() const;
      strAddCommand* clone( void *Memory = NULL ) const;
  };

  // =====================================================
  
  class joinArrayCommand : public command
  {
    public:
      void exec( memory *Memory ) const;
      std::string toString() const;
      joinArrayCommand* clone( void *Memory = NULL ) const;
  };
  
  // =====================================================
  
  class joinDictCommand : public command
  {
    private:
      explicit joinDictCommand( const variable &Variable );

    public:
      explicit joinDictCommand( const char *Name );
      void exec( memory *Memory ) const;
      std::string toString() const;
      joinDictCommand* clone( void *Memory = NULL ) const;
  };

  // =====================================================

}

