#ifndef CONTAINER_H_
#define CONTAINER_H_

#include "BasicTypes.h"
#include "Allocable.h"
#include "Allocator.h"

namespace DE {

class Container : public Allocable {

protected:

  u32 mElementSize;
  u32 mLength;
  u32 mAlignment;

  void init(const u32 length, const u32 elementSize, const u32 alignment);

public:

  Container();
  virtual ~Container();
  u32 getLength() const;

};

} /* namespace DE */

#endif /* CONTAINER_H_ */
