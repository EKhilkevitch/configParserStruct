
#pragma once

// =====================================================

#include <vector>
#include <typeinfo>

#include "configParserStruct/clonablepointer.h"
#include "configParserStruct/structparservars.h"

// =====================================================

namespace configParserStruct
{
  namespace structParser
  {

    class program;
    class nopCommand;
    
    // =====================================================
   
    class commandAction
    {
      public:
        virtual ~commandAction() {}
        virtual commandAction* clone() const = 0;
        virtual void execute( program *Program ) const = 0; 
    };
    
    // =====================================================
    
    class command 
    {
      private:
        clonablePointer<commandAction,nopCommand> Action;

      public:
        command() {}
        command( const commandAction &A ) : Action(A) {}
        void execute( program *Program ) const { Action->execute(Program); }
        const std::type_info& actionType() const { return typeid(*Action); }
    };
    
    // =====================================================
    
    class commandsList
    {
      private:
        std::vector< command > Commands;
        unsigned CurrentCommandIndex;

      public:
        commandsList() : CurrentCommandIndex(0) {}

        unsigned push( const command &Cmd );
        unsigned push( const commandAction &Action );
        void set( unsigned Index, const command &Cmd );

        size_t size() const { return Commands.size(); }
        const command& currentCommand() const;
        unsigned currentCommandIndex() const { return CurrentCommandIndex; }

        void resetCurrentCommandIndex() { CurrentCommandIndex = 0; }
        bool isAllCommandsDone() const { return CurrentCommandIndex >= Commands.size(); }
        unsigned executeOneCommand( program *Program );
    };
    
    // =====================================================
    
    class nopCommand : public commandAction
    {
      public:
        void execute( program* ) const {}
        commandAction* clone() const { return new nopCommand(); }
    };

    // -----------------------------------------------------
    
    class pushCommand : public commandAction
    {
      private:
        variable Variable;

      public:
        pushCommand( const variable &V ) : Variable(V) {}
        void execute( program *Program ) const; 
        commandAction* clone() const { return new pushCommand(Variable); }
    };
    
    // -----------------------------------------------------
    
    class popCommand : public commandAction
    {
      public:
        void execute( program *Program ) const; 
        commandAction* clone() const { return new popCommand(); }
    };
    
    // -----------------------------------------------------

    class addCommand : public commandAction
    {
      public:
        void execute( program *Program ) const;
        commandAction* clone() const { return new addCommand(); }
    };
    
    // =====================================================

  }
}

// =====================================================

