
#pragma once

// =====================================================

#include <vector>
#include <typeinfo>

#include "configParserStruct/clonablepointer.h"
#include "configParserStruct/structparservars.h"

// =====================================================

namespace configParserStruct
{
  namespace structParserUtil
  {

    class program;
    class nopCommand;
    
    // =====================================================
   
    class commandAction
    {
      public:
        virtual ~commandAction() {};
        virtual commandAction* clone() const = 0;
        virtual void execute( program *Program ) const = 0; 
        virtual std::string toString() const = 0;
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
        const std::string toString() const { return Action->toString(); }
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

        void clear();

        size_t size() const { return Commands.size(); }
        const command& currentCommand() const;
        const command& getCommand( unsigned Index ) const { return Commands.at(Index); }
        unsigned currentCommandIndex() const { return CurrentCommandIndex; }
        void resetCurrentCommandIndex() { CurrentCommandIndex = 0; }
        void setCurrentCommandIndex( unsigned Index ) { CurrentCommandIndex = Index; }

        bool isAllCommandsDone() const { return CurrentCommandIndex >= Commands.size(); }
        unsigned executeOneCommand( program *Program );
        void execute( program *Program );
    };
    
    // =====================================================
    
    class nopCommand : public commandAction
    {
      public:
        void execute( program* ) const {}
        commandAction* clone() const { return new nopCommand(); }
        std::string toString() const { return "nop"; }
    };

    // -----------------------------------------------------
    
    class pushValueCommand : public commandAction
    {
      private:
        variable Variable;

      public:
        pushValueCommand( const variable &V ) : Variable(V) {}
        void execute( program *Program ) const; 
        commandAction* clone() const { return new pushValueCommand(Variable); }
        std::string toString() const { return "push value " + Variable.string() + ""; }
    };
    
    // -----------------------------------------------------
    
    class pushVariableCommand : public commandAction
    {
      private:
        std::string Name;
      public:
        pushVariableCommand( const std::string &N ) : Name(N) {}
        void execute( program *Program ) const; 
        commandAction* clone() const { return new pushVariableCommand(Name); }
        std::string toString() const { return "push variable " + Name + ""; }
    };
    
    // -----------------------------------------------------
    
    class popCommand : public commandAction
    {
      public:
        void execute( program *Program ) const; 
        commandAction* clone() const { return new popCommand(); }
        std::string toString() const { return "pop"; }
    };
    
    // -----------------------------------------------------
    
    class callFunction : public commandAction
    {
      private:
        std::string Name;
      public:
        callFunction( const std::string &N ) : Name(N) {}
        void execute( program *Program ) const; 
        commandAction* clone() const { return new callFunction(*this); }
        std::string toString() const { return "call " + Name; }
    };
    
    // -----------------------------------------------------
    
    class retFromFunction : public commandAction
    {
      public:
        void execute( program *Program ) const; 
        commandAction* clone() const { return new retFromFunction(); }
        std::string toString() const { return "ret"; }
    };
    
    // -----------------------------------------------------
    
    class jumpToCommand : public commandAction
    {
      private:
        unsigned Index;
      public:
        jumpToCommand( unsigned I ) : Index(I) {}
        void execute( program *Program ) const; 
        commandAction* clone() const { return new jumpToCommand(*this); }
        std::string toString() const { return "jump to " + convertToString(Index); }
    };
    
    // -----------------------------------------------------

    class addCommand : public commandAction
    {
      public:
        void execute( program *Program ) const;
        commandAction* clone() const { return new addCommand(); }
        std::string toString() const { return "add"; }
    };

    // -----------------------------------------------------

    class subCommand : public commandAction
    {
      public:
        void execute( program *Program ) const;
        commandAction* clone() const { return new subCommand(); }
        std::string toString() const { return "sub"; }
    };

    // -----------------------------------------------------

    class mulCommand : public commandAction
    {
      public:
        void execute( program *Program ) const;
        commandAction* clone() const { return new mulCommand(); }
        std::string toString() const { return "mul"; }
    };

    // -----------------------------------------------------

    class divCommand : public commandAction
    {
      public:
        void execute( program *Program ) const;
        commandAction* clone() const { return new divCommand(); }
        std::string toString() const { return "div"; }
    };
    
    // -----------------------------------------------------
    
    class assignVariableCommand : public commandAction
    {
      private:
        std::string Name;
      public:
        assignVariableCommand( const std::string &N ) : Name(N) {}
        void execute( program *Program ) const;
        commandAction* clone() const { return new assignVariableCommand(Name); }
        std::string toString() const { return "assign variable " + Name; }
    };
    
    // -----------------------------------------------------
    
    class setDictFieldCommand : public commandAction
    {
      private:
        std::string Name;
      public:
        setDictFieldCommand( const std::string &N ) : Name(N) {}
        void execute( program *Program ) const;
        commandAction* clone() const { return new setDictFieldCommand(Name); }
        std::string toString() const { return "set dict field " + Name; }
    };

    // =====================================================

  }
}

// =====================================================

