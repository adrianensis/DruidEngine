#ifndef ALLOCATOR_H_
#define ALLOCATOR_H_

#include <cstdlib>
#include "Assert.h"
#include "BasicTypes.h"

namespace DE {

class Allocator {

protected:

  u32 mTotalSize;
  u32 mAllocated;

  void checkSpace(u32 size);

public:

  Allocator();
  virtual ~Allocator();
  u32 getSize();
  u32 getAllocated();
  bool hasSpace(const u32 size);
  virtual void init(const u32 size);
  virtual void* allocate(const u32 size) = 0;
  virtual void* allocateAligned(const u32 size, const u32 alignment) = 0;
  virtual void free(const void* pointer) = 0;
  virtual void freeAligned(const void* pointer) = 0;
  virtual void reset();

};

} /* namespace DE */

#endif /* ALLOCATOR_H_ */
