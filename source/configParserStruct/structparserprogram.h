
#pragma once

// =====================================================

#include <string>
#include <list>
#include <set>
#include <cstddef>

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

        void initBuiltInVariables();

        size_t pushCommand( const commandAction &Action );
        size_t numberOfCommands() const;
        void executeOneCommand();
        template <class jmpCommandAction> size_t replaceCommandMarkerToJmp( ptrdiff_t Shift = 0 );
        size_t replaceCommandMarkerToNop();

        void setCurrentCommandIndex( size_t Index );
        size_t currentCommandIndex() const;

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
        variable* getNamedVariablePointer( const std::string &Name ) { return Variables.getPointer(Name); }
        const variable getNamedVariableFromTopOfStack( const std::string &Name ) const { return Variables.getFromTopOfStack(Name); }
        void setNamedVariable( const std::string &Name, const variable &Value ) { Variables.set(Name,Value); }
        std::set<std::string> onExecuteVariableNames() const { return Variables.listOfNamesInAllStack(); }
        std::set<std::string> onBuildVariableNames() const;
        const variable getLastExpressionReuslt() const { return Variables.get( lastResultVariableName() ); }
        
        void setErrorLine( int Line ) { ErrorLine = Line; }

        void clear();
        bool build( const std::string &ProgramText );
        int errorLine() { return ErrorLine; }
        void execute();
        bool rebuildAndExecute( const std::string &ProgramText );
        
        static std::string lastResultVariableName();

        std::string toString() const;
    };
  
    // =====================================================
    
    template <class jmpCommandAction> size_t program::replaceCommandMarkerToJmp( ptrdiff_t Shift ) 
    { 
      jmpCommandAction JmpCommandAction( Commands.size()+Shift );
      return Commands.replaceMarker( JmpCommandAction ); 
    }
    
    // =====================================================
    
  }
}

// =====================================================

