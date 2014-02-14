
#pragma once

// =====================================================

#include <string>
#include <map>
#include <vector>
#include <list>
#include <set>
#include <stdexcept>
#include <functional>

// =====================================================

/**
 * @file
 * @brief Парсер командной строки.
 */

// =====================================================

namespace configParserStruct
{

// =====================================================

  /**
   * @brief 
   * Парсер аргументов командной строки
   *
   * Парсинг осуществляется функцией getopt_long. Данный класс является только оболочкой 
   * для парсера.
   */
  class commandLineArgumentsParser
  {
    public:
      struct getoptOption
      {
	std::string Name;
	bool HasArg;
	char ShortName;
      };
      
    public:

      /**
       * @brief
       * Распарсеные аргументы командной строки.
       */
      class parsedArguments
      {
        private:
          std::string ProgramName;
          std::map< char, std::string > Arguments;
          std::list< std::string > ListOfFileArguments;
          bool FoundUnknownOption;

        public:
          parsedArguments( const std::string &Name ) : ProgramName(Name), FoundUnknownOption(false) {}

          const std::string& programName() const { return ProgramName; }

          void insert( char ArgShortName, const char *Value );
          void setFoundUnknownOption() { FoundUnknownOption = true; }
          void pushFileArgument( const std::string &Arg ) { ListOfFileArguments.push_back(Arg); }

          bool foundUnknownOption() const { return FoundUnknownOption; }
          bool exist( char ArgShortName ) const { return Arguments.find(ArgShortName) != Arguments.end(); }
          const std::list<std::string>& listOfFileArguments() const { return ListOfFileArguments; }
          std::set<char> existingArguments() const;
          
          std::string arg( char ArgShortName, const std::string &DefaultValue = std::string(), bool EnableException = false ) const;
          int argInt( char ArgShortName, const int DefaultValue = 0, bool EnableException = false ) const;
          double argDouble( char ArgShortName, const double DefaultValue = 0, bool EnableException = false ) const;
      };


    private:
      std::vector< getoptOption > Options;

    private:
      void clearOptions() { Options.clear(); }

    protected:
      std::string shortOptions() const;
      std::vector<getoptOption> longOptions() const;

      void clearGetoptInternalIndexes() const;
      void setArgumentsFromParameters( parsedArguments *ParsingResults, int argc, char *argv[] ) const;
      void setArgumentsFromFileList( parsedArguments *ParsingResults, int argc, char *argv[] ) const;

      static char optionShortName( const getoptOption &Option ) { return Option.ShortName; }
      static std::string optionFullName( const getoptOption &Option ) { return Option.Name; }
      static bool optionHasArgument( const getoptOption &Option ) { return Option.HasArg; }

    public:
      commandLineArgumentsParser();

      void addOption( const std::string &FullName, const bool NeedParameter, const char ShortName );
      void addOption( const std::string &FullName, const bool NeedParameter ) 
        { if ( ! FullName.empty() ) addOption( FullName, NeedParameter, FullName[0] ); }

      bool isOptionExist( char ShortName );
      bool isOptionExist( const std::string &FullName );

      size_t numberOfOptions() const { return Options.size(); }
      std::string optionFullName( unsigned i ) const { return i < numberOfOptions() ? optionFullName(Options[i]) : ""; }
      bool optionHasArgument( unsigned i ) const { return i < numberOfOptions() ? optionHasArgument(Options[i]) : false; } 
      char optionShortName( unsigned i ) const { return i < numberOfOptions() ? optionShortName(Options[i]) : '\0'; }

      parsedArguments parse( int argc, char *argv[] ) const;
      parsedArguments parse( const std::list< std::string > &Arguments ) const;
      parsedArguments operator()( int argc, char *argv[] ) const { return parse(argc,argv); }
      parsedArguments operator()(  const std::list< std::string > &Arguments ) const { return parse(Arguments); }
  };

// =====================================================

}


