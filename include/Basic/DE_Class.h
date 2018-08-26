#ifndef DE_CLASS_H
#define DE_CLASS_H

#include "Hash.h"

namespace DE {

/*!
  \brief Base class for Druid Engine.
*/
class DE_Class : public Hash {
public:
  DE_Class() = default;
  virtual ~DE_Class() = default;
};

} /* namespace DE */


#endif //DE_CLASS_H
