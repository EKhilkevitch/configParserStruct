
#pragma once

// =====================================================

#include <vector>
#include <typeinfo>
#include <cstdlib>

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
        unsigned replaceMarker( const commandAction &Action );

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
    
    class markerCommand : public commandAction
    {
      public:
        void execute( program* ) const { std::abort(); }
        commandAction* clone() const { return new markerCommand(); }
        std::string toString() const { return "<MARKER>"; }
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
    
    class popCommand : public commandAction
    {
      public:
        void execute( program *Program ) const; 
        commandAction* clone() const { return new popCommand(); }
        std::string toString() const { return "pop"; }
    };
    
    // -----------------------------------------------------
    
    class callCommand : public commandAction
    {
      private:
        std::string Name;
      public:
        callCommand( const std::string &N ) : Name(N) {}
        void execute( program *Program ) const; 
        commandAction* clone() const { return new callCommand(*this); }
        std::string toString() const { return "call " + Name; }
    };
    
    // -----------------------------------------------------
    
    class retCommand : public commandAction
    {
      public:
        void execute( program *Program ) const; 
        commandAction* clone() const { return new retCommand(); }
        std::string toString() const { return "ret"; }
    };
    
    // -----------------------------------------------------
    
    class pushVarFrameCommand : public commandAction
    {
      public:
        void execute( program *Program ) const; 
        commandAction* clone() const { return new pushVarFrameCommand(); }
        std::string toString() const { return "push variable frame"; }
    };
    
    // -----------------------------------------------------
    
    class popVarFrameCommand : public commandAction
    {
      public:
        void execute( program *Program ) const; 
        commandAction* clone() const { return new popVarFrameCommand(); }
        std::string toString() const { return "pop variable frame"; }
    };
    
    // -----------------------------------------------------
    
    class pushArgumentCommand : public commandAction
    {
      public:
        void execute( program *Program ) const; 
        commandAction* clone() const { return new pushArgumentCommand(); }
        std::string toString() const { return "push argument"; }
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
   
    class jumpIfFalseCommand : public commandAction
    {
      private:
        unsigned Index;
      public:
        jumpIfFalseCommand( unsigned I ) : Index(I) {}
        void execute( program *Program ) const; 
        commandAction* clone() const { return new jumpIfFalseCommand(*this); }
        std::string toString() const { return "jump if false to " + convertToString(Index); }
    };

    // -----------------------------------------------------
    
    class twoStackOperandsCommand : public commandAction
    {
      protected:
        virtual const variable calculateResultVariable( const variable &Op1, const variable &Op2 ) const = 0;
      public:
        void execute( program *Program ) const;
    };
    
    // -----------------------------------------------------
    
    template <class T> class templateTwoStackOperandsCommand : public twoStackOperandsCommand
    {
      protected:
        const variable calculateResultVariable( const variable &Op1, const variable &Op2 ) const
        {
          return createVariable<T>( calculateResult( Op1.valueScalar<T>(), Op2.valueScalar<T>() ) );
        };

        virtual T calculateResult( const T& Op1, const T& Op2 ) const = 0;
    };
    
    // -----------------------------------------------------

    class addCommand : public templateTwoStackOperandsCommand<double>
    {
      protected:
        double calculateResult( const double &Op1, const double &Op2 ) const { return Op2 + Op1; }
      public:
        commandAction* clone() const { return new addCommand(); }
        std::string toString() const { return "add"; }
    };

    // -----------------------------------------------------

    class subCommand : public templateTwoStackOperandsCommand<double>
    {
      protected:
        double calculateResult( const double &Op1, const double &Op2 ) const { return Op2 - Op1; }
      public:
        commandAction* clone() const { return new subCommand(); }
        std::string toString() const { return "sub"; }
    };

    // -----------------------------------------------------

    class mulCommand : public templateTwoStackOperandsCommand<double>
    {
      protected:
        double calculateResult( const double &Op1, const double &Op2 ) const { return Op2 * Op1; }
      public:
        commandAction* clone() const { return new mulCommand(); }
        std::string toString() const { return "mul"; }
    };

    // -----------------------------------------------------

    class divCommand : public templateTwoStackOperandsCommand<double>
    {
      protected:
        double calculateResult( const double &Op1, const double &Op2 ) const { return Op2 / Op1; }
      public:
        commandAction* clone() const { return new divCommand(); }
        std::string toString() const { return "div"; }
    };
    
    // -----------------------------------------------------
    
    class modCommand : public templateTwoStackOperandsCommand<int>
    {
      protected:
        int calculateResult( const int &Op1, const int &Op2 ) const { return Op2 % Op1; }
      public:
        commandAction* clone() const { return new modCommand(); }
        std::string toString() const { return "mod"; }
    };
    
    // -----------------------------------------------------
    
    class numEqCommand : public templateTwoStackOperandsCommand<double>
    {
      protected:
        double calculateResult( const double &Op1, const double &Op2 ) const { return Op2 == Op1; }
      public:
        commandAction* clone() const { return new numEqCommand(); }
        std::string toString() const { return "eq numbers"; }
    };    

    // -----------------------------------------------------
    
    class numNeCommand : public templateTwoStackOperandsCommand<double>
    {
      protected:
        double calculateResult( const double &Op1, const double &Op2 ) const { return Op2 != Op1; }
      public:
        commandAction* clone() const { return new numNeCommand(); }
        std::string toString() const { return "ne numbers"; }
    };
    
    // -----------------------------------------------------
    
    class numGtCommand : public templateTwoStackOperandsCommand<double>
    {
      protected:
        double calculateResult( const double &Op1, const double &Op2 ) const { return Op2 > Op1; }
      public:
        commandAction* clone() const { return new numGtCommand(); }
        std::string toString() const { return "gt numbers"; }
    };
    
    // -----------------------------------------------------
    
    class numLtCommand : public templateTwoStackOperandsCommand<double>
    {
      protected:
        double calculateResult( const double &Op1, const double &Op2 ) const { return Op2 < Op1; }
      public:
        commandAction* clone() const { return new numLtCommand(); }
        std::string toString() const { return "lt numbers"; }
    };    

    // -----------------------------------------------------
    
    class numGeCommand : public templateTwoStackOperandsCommand<double>
    {
      protected:
        double calculateResult( const double &Op1, const double &Op2 ) const { return Op2 >= Op1; }
      public:
        commandAction* clone() const { return new numGeCommand(); }
        std::string toString() const { return "ge numbers"; }
    };

    // -----------------------------------------------------
    
    class numLeCommand : public templateTwoStackOperandsCommand<double>
    {
      protected:
        double calculateResult( const double &Op1, const double &Op2 ) const { return Op2 <= Op1; }
      public:
        commandAction* clone() const { return new numLeCommand(); }
        std::string toString() const { return "le numbers"; }
    };
    
    // -----------------------------------------------------
    
    class boolAndCommand : public templateTwoStackOperandsCommand<bool>
    {
      protected:
        bool calculateResult( const bool &Op1, const bool &Op2 ) const { return Op2 && Op1; }
      public:
        commandAction* clone() const { return new boolAndCommand(); }
        std::string toString() const { return "and bool"; }
    };

    // -----------------------------------------------------
    
    class boolOrCommand : public templateTwoStackOperandsCommand<bool>
    {
      protected:
        bool calculateResult( const bool &Op1, const bool &Op2 ) const { return Op2 || Op1; }
      public:
        commandAction* clone() const { return new boolOrCommand(); }
        std::string toString() const { return "or bool"; }
    };

    // -----------------------------------------------------
    
    class boolNotCommand : public commandAction
    {
      public:
        void execute( program *Program ) const;
        commandAction* clone() const { return new boolNotCommand(); }
        std::string toString() const { return "bot bool"; }
    };
    
    // -----------------------------------------------------
    
    class addStrCommand : public templateTwoStackOperandsCommand<std::string>
    {
      protected:
        std::string calculateResult( const std::string &Op1, const std::string &Op2 ) const { return Op2 + Op1; }
      public:
        commandAction* clone() const { return new addStrCommand(); }
        std::string toString() const { return "add string"; }
    };

    // -----------------------------------------------------
    
    class assignVariableCommand : public commandAction
    {
      public:
        void execute( program *Program ) const;
        commandAction* clone() const { return new assignVariableCommand(); }
        std::string toString() const { return "assign"; }
    };
    
    // -----------------------------------------------------
    
    class assignLastExpressionCommand : public commandAction
    {
      public:
        void execute( program *Program ) const;
        commandAction* clone() const { return new assignLastExpressionCommand(); }
        std::string toString() const { return "assign last"; }
    };
    
    // -----------------------------------------------------
    
    class pushRefValueCommand : public commandAction
    {
      public:
        void execute( program *Program ) const;
        commandAction* clone() const { return new pushRefValueCommand(); }
        std::string toString() const { return "push ref value"; }
    };
    
    // -----------------------------------------------------

    class replaceRefToValueCommand : public commandAction
    {
      public:
        void execute( program *Program ) const;
        commandAction* clone() const { return new replaceRefToValueCommand(); }
        std::string toString() const { return "replace ref value"; }
    };

    // -----------------------------------------------------
    
    class replaceStringToRefCommand : public commandAction
    {
      public:
        void execute( program *Program ) const;
        commandAction* clone() const { return new replaceStringToRefCommand(); }
        std::string toString() const { return "replace string ref"; }
    };
    
    // -----------------------------------------------------
    
    class setRefAttributeCommand : public commandAction
    {
      public:
        void execute( program *Program ) const;
        commandAction* clone() const { return new setRefAttributeCommand(); }
        std::string toString() const { return "set ref attr"; }
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
    
    // -----------------------------------------------------

    class pushArrayElementCommand : public commandAction
    {
      public:
        void execute( program *Program ) const;
        commandAction* clone() const { return new pushArrayElementCommand(); }
        std::string toString() const { return "push to array"; }
    };

    // =====================================================

  }
}

// =====================================================

