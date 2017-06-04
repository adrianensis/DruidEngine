#ifndef CONTAINER_H_
#define CONTAINER_H_

#include "BasicTypes.h"
#include "Allocatable.h"
#include "Allocator.h"

namespace DE {

class Container : public Allocatable {

protected:

  u32 mElementSize;
  u32 mLength;
  u32 mAlignment;

  void init(const u32 length, const u32 elementSize, const u32 alignment, Allocator* allocator);

public:

  Container();
  virtual ~Container();
  u32 getLength();

};

} /* namespace DE */

#endif /* CONTAINER_H_ */
