#ifndef BASEARRAY_H_
#define BASEARRAY_H_

#include "BasicTypes.h"
#include "Allocatable.h"
#include "Allocator.h"

namespace DE {

class BaseArray : public Allocatable {

private:

  void _init(const u32 length, const u32 elementSize, Allocator* allocator);

protected:

  u32 mElementSize;
  u32 mLength;

  void* allocate(const u32 length, const u32 elementSize, Allocator* allocator);
  void* allocate(const u32 length, const u32 elementSize, const u32 alignment, Allocator* allocator);

public:

  BaseArray();
  virtual ~BaseArray();
  u32 getLength();

};

} /* namespace DE */

#endif /* BASEARRAY_H_ */
