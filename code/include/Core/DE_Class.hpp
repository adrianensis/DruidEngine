#ifndef DE_CLASS_H
#define DE_CLASS_H

#include "Hash.hpp"
#include <string>

namespace DE {

/*
  METADATA MACROS
*/

#define DE_GENERATE_ID_STATIC(Class) static ClassId getClassIdStatic(){static ClassId classId = Hash::hash(#Class); return classId;}
#define DE_GENERATE_PARENT_ID_STATIC(ParentClass) static ClassId getParentClassIdStatic(){ return ParentClass::getClassIdStatic(); }

#define DE_GENERATE_NAME_STATIC(Class) static std::string getClassNameStatic(){static std::string className = #Class; return #Class;}

#define DE_GENERATE_ID_VIRTUAL(Class) ClassId getClassId() override { return Class::getClassIdStatic(); };
#define DE_GENERATE_PARENT_ID_VIRTUAL(ParentClass) ClassId getParentClassId() override { return ParentClass::getClassIdStatic(); };

#define DE_GENERATE_NAME_VIRTUAL(Class) std::string getClassName() override { return Class::getClassNameStatic(); };

#define DE_GENERATE_METADATA(Class, ParentClass) DE_GENERATE_NAME_STATIC(Class); DE_GENERATE_NAME_VIRTUAL(Class); DE_GENERATE_ID_STATIC(Class); DE_GENERATE_PARENT_ID_STATIC(ParentClass); DE_GENERATE_ID_VIRTUAL(Class); DE_GENERATE_PARENT_ID_VIRTUAL(ParentClass);

#define DE_CLASS(Class, ParentClass) Class(); ~Class() override; DE_GENERATE_METADATA(Class, ParentClass)

#define DE_GET(Class, BaseName) Class get ## BaseName() const { return m ## BaseName; }
#define DE_SET(Class, BaseName) void set ## BaseName (Class new ## BaseName){ m ## BaseName = new ## BaseName; }
#define DE_GET_SET(Class, BaseName) DE_GET(Class, BaseName) DE_SET(Class, BaseName)

// ---------------------------------------------------------------------------

/*!
  \brief Base class for Druid Engine.
*/
class DE_Class : public Hash {

public:

  static std::string getClassNameStatic(){ return "DE_Class"; };
  static ClassId getClassIdStatic(){ return 0; }

  DE_Class() = default;
  virtual ~DE_Class() = default;

  virtual ClassId getClassId(){ return 0; };
  virtual ClassId getParentClassId(){ return 0; };
  virtual std::string getClassName(){ return "DE_Class"; };
};

// ---------------------------------------------------------------------------

} /* namespace DE */


#endif //DE_CLASS_H
