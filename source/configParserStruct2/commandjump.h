
#pragma once

// =====================================================

#include "configParserStruct2/command.h"

// =====================================================

namespace configParserStruct
{

  // =====================================================
   
  class nopCommand : public command
  {
    public:
      void exec( memory *Memory ) const;
      std::string toString() const;
      nopCommand* clone( void *Memory = NULL ) const;
  };

  // =====================================================
  
  class haltCommand : public command
  {
    public:
      void exec( memory *Memory ) const;
      std::string toString() const;
      haltCommand* clone( void *Memory = NULL ) const;
  }; 
  
  // =====================================================

  class jumpIfFalseCommand : public command
  {
    private:
      explicit jumpIfFalseCommand( const variable &Variable );

    public:
      explicit jumpIfFalseCommand( size_t Index );
      void exec( memory *Memory ) const;
      std::string toString() const;
      jumpIfFalseCommand* clone( void *Memory = NULL ) const;
  }; 

  
  // =====================================================

  class jumpCommand : public command
  {
    private:
      explicit jumpCommand( const variable &Variable );

    public:
      explicit jumpCommand( size_t Index );
      void exec( memory *Memory ) const;
      std::string toString() const;
      jumpCommand* clone( void *Memory = NULL ) const;
  }; 

  // =====================================================
  
  class callCommand : public command
  {
    private:
      explicit callCommand( const variable &Variable );

    public:
      explicit callCommand( const char *Name );
      void exec( memory *Memory ) const;
      std::string toString() const;
      callCommand* clone( void *Memory = NULL ) const;  
  };
  
  // =====================================================

  class returnCommand : public command
  {
    public:
      void exec( memory *Memory ) const;
      std::string toString() const;
      returnCommand* clone( void *Memory = NULL ) const;  
  };

  // =====================================================
  
}

