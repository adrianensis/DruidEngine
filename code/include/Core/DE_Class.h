#ifndef DE_CLASS_H
#define DE_CLASS_H

#include "Hash.h"
#include <string>

namespace DE {

/*
  SIMPLIFY C++ MACROS
*/

#define DE_PUB public :
#define DE_PRI private :
#define DE_PRO protected :

#define DE_MEMBER_BASE(T, name) T m##name;

#define DE_MEMBER_GET_BASE(T, name) T get##name () { return m##name; }
#define DE_MEMBER_SET_BASE(T, name) void set##name (T new##name) { m##name = new##name; }

#define DE_MEMBER_GET(T, name) DE_PUB DE_MEMBER_GET_BASE(T, name)
#define DE_MEMBER_SET(T, name) DE_PUB DE_MEMBER_SET_BASE(T, name)
#define DE_MEMBER_GET_S(T, name) DE_PUB static DE_MEMBER_GET_BASE(T, name)
#define DE_MEMBER_SET_S(T, name) DE_PUB static DE_MEMBER_SET_BASE(T, name)

#define DE_MEMBER(T, name) DE_MEMBER_BASE(T, name) DE_MEMBER_GET(T, name) DE_MEMBER_SET(T, name)
#define DE_MEMBER_S(T, name) static DE_MEMBER_BASE(T, name) DE_MEMBER_GET_S(T, name) DE_MEMBER_SET_S(T, name)

#define DE_CONSTRUCTOR(class_name) DE_PUB class_name(); ~class_name() override;

#define DE_CLASS(class_name) class class_name : public DE_Class { DE_CONSTRUCTOR(class_name)
#define DE_CLASS_SINGLETON(class_name) class class_name : public DE_Class, public Singleton< class_name > { DE_CONSTRUCTOR(class_name)
#define DE_END_CLASS }

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
  DE_Class() = default;
  virtual ~DE_Class() = default;
};

// ---------------------------------------------------------------------------

} /* namespace DE */


#endif //DE_CLASS_H
