
#pragma once

// =====================================================

#include "configParserStruct2/command.h"

// =====================================================

namespace configParserStruct
{

  // =====================================================
  
  class numEqCommand : public command
  {
    public:
      void exec( memory *Memory ) const;
      std::string toString() const;
      numEqCommand* clone( void *Memory = NULL ) const;
  };

  // =====================================================
  
  class numNeCommand : public command
  {
    public:
      void exec( memory *Memory ) const;
      std::string toString() const;
      numNeCommand* clone( void *Memory = NULL ) const;
  };

  // =====================================================
  
  class numLtCommand : public command
  {
    public:
      void exec( memory *Memory ) const;
      std::string toString() const;
      numLtCommand* clone( void *Memory = NULL ) const;
  };

  // =====================================================
  
  class numLeCommand : public command
  {
    public:
      void exec( memory *Memory ) const;
      std::string toString() const;
      numLeCommand* clone( void *Memory = NULL ) const;
  };

  // =====================================================
  
  class numGtCommand : public command
  {
    public:
      void exec( memory *Memory ) const;
      std::string toString() const;
      numGtCommand* clone( void *Memory = NULL ) const;
  };

  // =====================================================
  
  class numGeCommand : public command
  {
    public:
      void exec( memory *Memory ) const;
      std::string toString() const;
      numGeCommand* clone( void *Memory = NULL ) const;
  };


  // =====================================================
  
  class strEqCommand : public command
  {
    public:
      void exec( memory *Memory ) const;
      std::string toString() const;
      strEqCommand* clone( void *Memory = NULL ) const;
  };

  // =====================================================
  
  class strNeCommand : public command
  {
    public:
      void exec( memory *Memory ) const;
      std::string toString() const;
      strNeCommand* clone( void *Memory = NULL ) const;
  };

  // =====================================================
  
  class strLtCommand : public command
  {
    public:
      void exec( memory *Memory ) const;
      std::string toString() const;
      strLtCommand* clone( void *Memory = NULL ) const;
  };

  // =====================================================
  
  class strLeCommand : public command
  {
    public:
      void exec( memory *Memory ) const;
      std::string toString() const;
      strLeCommand* clone( void *Memory = NULL ) const;
  };

  // =====================================================
  
  class strGtCommand : public command
  {
    public:
      void exec( memory *Memory ) const;
      std::string toString() const;
      strGtCommand* clone( void *Memory = NULL ) const;
  };

  // =====================================================
  
  class strGeCommand : public command
  {
    public:
      void exec( memory *Memory ) const;
      std::string toString() const;
      strGeCommand* clone( void *Memory = NULL ) const;
  };

  // =====================================================

}

