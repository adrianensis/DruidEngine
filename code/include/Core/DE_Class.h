#ifndef DE_CLASS_H
#define DE_CLASS_H

#include "Hash.h"
#include <string>

namespace DE {

/*
  METADATA MACROS
*/

#define DE_GENERATE_ID(Class) static ClassId getClassId() {static ClassId classId = Hash::hash(#Class); return classId;}
#define DE_GENERATE_NAME(Class) static std::string getClassName() {static std::string className = #Class; return className;}
#define DE_GENERATE_METADATA(Class) DE_GENERATE_NAME(Class); DE_GENERATE_ID(Class);

// ---------------------------------------------------------------------------

/*!
  \brief Base class for Druid Engine.
*/
class DE_Class : public Hash {
public:

  static ClassId rootClassId;

  DE_Class() = default;
  virtual ~DE_Class() = default;
};

// ---------------------------------------------------------------------------

} /* namespace DE */


#endif //DE_CLASS_H
