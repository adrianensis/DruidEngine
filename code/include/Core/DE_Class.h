#ifndef DE_CLASS_H
#define DE_CLASS_H

#include "Hash.h"

namespace DE {

#define DE_GENERATE_CLASS_ID static ClassId getClassId() {static ClassId classId = ++DE_Class::rootClassId; return classId;}

/*!
  \brief Base class for Druid Engine.
*/
class DE_Class : public Hash {
public:

  static ClassId rootClassId;

  DE_Class() = default;
  virtual ~DE_Class() = default;
};

} /* namespace DE */


#endif //DE_CLASS_H
