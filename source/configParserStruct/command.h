
#pragma once

// =====================================================

#include "configParserStruct/variable.h"

#include <string>
#include <iosfwd>

// =====================================================

namespace configParserStruct
{

  // =====================================================
  
  class memory;

  // =====================================================
 
  class command
  {
    private:
      variable Argument;

    public:
      command();
      explicit command( const variable &Variable );
      virtual ~command() = 0;

      virtual void exec( memory *Memory ) const = 0;
      virtual std::string toString() const = 0;
      virtual command* clone( void *Memory = NULL ) const = 0;
      virtual command* cloneWithArgument( const variable &Variable, void *Memory = NULL ) const;
      
      const variable& argument() const;
  };

  // =====================================================
  
  std::ostream& operator<<( std::ostream &Stream, const command &Command );

  // =====================================================

}

