
#pragma once

// =====================================================

#include "configParserStruct/structparservars.h"
#include "configParserStruct/structparsercommand.h"

// =====================================================

namespace configParserStruct
{
  namespace structParserUtil
  {
    
    // =====================================================

    class program
    {
      private:
        commandsList Commands;
        variablesListStack Variables;
        variablesStack Stack;
        int ErrorLine;

      public:
        program();
        ~program();

        unsigned pushCommand( const commandAction &A ) { return Commands.push(A); }
        size_t numberOfCommands() const { return Commands.size(); }
        void executeOneCommand() { Commands.executeOneCommand(this); }

        void setCurrentCommandIndex( unsigned Index ) { Commands.setCurrentCommandIndex(Index); }
        unsigned currentCommandIndex()  const { return Commands.currentCommandIndex(); }

        void pushStackVariable( const variableValue &V ) { Stack.push(V); }
        void pushStackVariable( const variable &V )      { Stack.push(V); }
        const variable popStackVariable() { return Stack.pop(); }
        const variable topStackVariable() const { return Stack.top(); }
        size_t stackSize() const { return Stack.size(); }

        void pushVariableFrame() { Variables.pushNewList(); }
        void pushFunctionArgument( const variable &V );
        void popVariableFrame() { Variables.popList(); }
        size_t functionLevel() const { return Variables.size(); }

        const variable getNamedVariable( const std::string &Name ) const { return Variables.get(Name); }
        void setNamedVariable( const std::string &Name, const variable &Value ) { Variables.set(Name,Value); }
        std::list<std::string> variableNames() const { return Variables.listOfNamesInAllStack(); }
        const variable getLastExpressionReuslt() const { return Variables.get( lastResultVariableName() ); }
        
        void setErrorLine( int Line ) { ErrorLine = Line; }

        void clear();
        bool build( const std::string &ProgramText );
        int errorLine() { return ErrorLine; }
        void execute() { Commands.execute( this ); }
        bool rebuildAndExecute( const std::string &ProgramText );

        std::string toString() const;

        static std::string lastResultVariableName() { return ":LAST_EXPRESSION:"; }
    };
  
    // =====================================================
    
  }
}

// =====================================================

