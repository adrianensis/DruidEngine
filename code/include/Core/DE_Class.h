#ifndef DE_CLASS_H
#define DE_CLASS_H

#include "Hash.h"
#include <string>

namespace DE {

/*
  METADATA MACROS
*/

#define DE_GENERATE_ID_STATIC(Class) static ClassId getClassIdStatic() {static ClassId classId = Hash::hash(#Class); return classId;}
#define DE_GENERATE_NAME_STATIC(Class) static std::string getClassNameStatic() {static std::string className = #Class; return className;}
#define DE_GENERATE_ID_VIRTUAL(Class) virtual ClassId getClassId() override { return Class::getClassIdStatic(); };
#define DE_GENERATE_NAME_VIRTUAL(Class) virtual std::string getClassName() override { return Class::getClassNameStatic(); };
#define DE_GENERATE_METADATA(Class) DE_GENERATE_NAME_STATIC(Class); DE_GENERATE_ID_STATIC(Class); DE_GENERATE_ID_VIRTUAL(Class);

#define DE_CLASS(Class) Class(); ~Class() override; DE_GENERATE_METADATA(Class)

// ---------------------------------------------------------------------------

/*!
  \brief Base class for Druid Engine.
*/
class DE_Class : public Hash {
public:

  DE_Class() = default;
  virtual ~DE_Class() = default;

  virtual ClassId getClassId() { return 0; };
  virtual std::string getClassName() { return "DE_Class"; };
};

// ---------------------------------------------------------------------------

} /* namespace DE */


#endif //DE_CLASS_H
