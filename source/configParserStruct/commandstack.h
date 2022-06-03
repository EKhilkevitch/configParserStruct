
#pragma once

// =====================================================

#include "configParserStruct/command.h"
#include "configParserStruct/variable.h"

// =====================================================

namespace configParserStruct
{

  // =====================================================
  
  class pushValueCommand : public command
  {
    private:
      explicit pushValueCommand( const variable &Variable );

    public:
      pushValueCommand();
      explicit pushValueCommand( int Integer );
      explicit pushValueCommand( double Real );
      explicit pushValueCommand( const char *String );
      explicit pushValueCommand( variable::collectionType Type );

      void exec( memory *Memory ) const;
      std::string toString() const;
      pushValueCommand* clone( void *Memory = NULL ) const;
  };

  // =====================================================
  
  class pushLocalDataRefCommand : public command
  {
    private:
      explicit pushLocalDataRefCommand( const variable &Variable );

    public:
      explicit pushLocalDataRefCommand( const char *Name );
      explicit pushLocalDataRefCommand( size_t Index );

      void exec( memory *Memory ) const;
      std::string toString() const;
      pushLocalDataRefCommand* clone( void *Memory = NULL ) const;
  };

  // =====================================================
  
  class pushGlobalDataRefCommand : public command
  {
    private:
      explicit pushGlobalDataRefCommand( const variable &Variable );

    public:
      explicit pushGlobalDataRefCommand( const char *Name );

      void exec( memory *Memory ) const;
      std::string toString() const;
      pushGlobalDataRefCommand* clone( void *Memory = NULL ) const;
  };

  // =====================================================
  
  class pushInstructionRefCommand : public command
  {
    private:
      explicit pushInstructionRefCommand( const variable &Variable );

    public:
      explicit pushInstructionRefCommand( size_t Index );

      void exec( memory *Memory ) const;
      std::string toString() const;
      pushInstructionRefCommand* clone( void *Memory = NULL ) const;
  };

  // =====================================================
 
  class assignCommand : public command
  {
    private:
      static void assign( memory *Memory, const reference &Reference, const variable &Value );
      static variable createEmpty( const reference &Reference );

    public:
      void exec( memory *Memory ) const;
      std::string toString() const;
      assignCommand* clone( void *Memory = NULL ) const;
  };

  // =====================================================
 
  class derefCommand : public command
  {
    private:
      static variable extract( memory *Memory, const reference &Reference );

    public:
      void exec( memory *Memory ) const;
      std::string toString() const;
      derefCommand* clone( void *Memory = NULL ) const;
  };

  // =====================================================
  
  class joinrefCommand : public command
  {
    public:
      enum type
      {
        Array,
        Dict
      };

    public:
      explicit joinrefCommand( type Type );
      void exec( memory *Memory ) const;
      std::string toString() const;
      joinrefCommand* clone( void *Memory = NULL ) const;
  };

  // =====================================================
  
  class duptopCommand : public command
  {
    public:
      void exec( memory *Memory ) const;
      std::string toString() const;
      duptopCommand* clone( void *Memory = NULL ) const;
  };

  // =====================================================
  
  class popAndAssignLastCommand : public command
  {
    public:
      void exec( memory *Memory ) const;
      std::string toString() const;
      popAndAssignLastCommand* clone( void *Memory = NULL ) const;
  };

  // =====================================================
  
  class pushStackSizeCommand : public command
  {
    public:
      void exec( memory *Memory ) const;
      std::string toString() const;
      pushStackSizeCommand* clone( void *Memory = NULL ) const;  
  };

  // =====================================================

}

