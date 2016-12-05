
#pragma once

// =====================================================

#include "configParserStruct/exception.h"

#include <string>
#include <map>
#include <vector>
#include <list>
#include <set>

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
      struct exception : public ::configParserStruct::exception
      {
        exception( const std::string &What );
      };

      struct invalidArgumentNameException : public exception
      {
        invalidArgumentNameException( const std::string &ArgName );
      };

      struct invalidArgumentValueException : public exception
      {
        invalidArgumentValueException( const std::string &ArgName );
      };

      struct doubleInsertingOptionException : public exception
      {
        doubleInsertingOptionException( const std::string &Name );
      };

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
          char FoundUnknownOption;

        public:
          explicit parsedArguments( const std::string &Name );

          const std::string& programName() const;

          void insert( char ArgShortName, const char *Value );
          void setFoundUnknownOption( char Char );
          void pushFileArgument( const std::string &Arg );

          char foundUnknownOption() const;
          bool exist( char ArgShortName ) const;
          const std::list<std::string>& listOfFileArguments() const;
          std::set<char> existingArguments() const;
          
          std::string arg( char ArgShortName, const std::string &DefaultValue = std::string(), bool EnableException = false ) const;
          int argInt( char ArgShortName, const int DefaultValue = 0, bool EnableException = false ) const;
          double argDouble( char ArgShortName, const double DefaultValue = 0, bool EnableException = false ) const;
      };


    private:
      std::vector< getoptOption > Options;

    private:
      void clearOptions();

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
      void addOption( const std::string &FullName, const bool NeedParameter );

      bool isOptionExist( char ShortName );
      bool isOptionExist( const std::string &FullName );

      size_t numberOfOptions() const;
      std::string optionFullName( size_t Index ) const;
      bool optionHasArgument( size_t Index ) const;
      char optionShortName( size_t Index ) const;

      parsedArguments parse( int argc, char *argv[] ) const;
      parsedArguments parse( const std::list< std::string > &Arguments ) const;
      parsedArguments operator()( int argc, char *argv[] ) const;
      parsedArguments operator()(  const std::list< std::string > &Arguments ) const;
  };

// =====================================================

}


