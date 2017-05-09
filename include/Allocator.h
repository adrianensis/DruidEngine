#ifndef ALLOCATOR_H_
#define ALLOCATOR_H_

#include <cstdlib>
#include "Assert.h"
#include "AtomicTypes.h"

namespace DE {

class Allocator {

protected:

  u32 mTotalSize;
  u32 mAllocated;

  void checkSpace(u32 size) { assert(mAllocated + size <= mTotalSize, "Total memory size exceeded."); };

public:

  Allocator() {};
  virtual ~Allocator() {};
  u32 getSize() { return mTotalSize; };
  u32 getAllocated() { return mAllocated; };
  bool hasSpace(u32) { return mAllocated; };
  virtual void init(const u32 size) { mTotalSize = size; mAllocated = 0; };
  virtual void* allocate(const u32 size) = 0;
  virtual void* allocateAligned(const u32 size, const u32 alignment) = 0;
  virtual void free(void* pointer) = 0;
  virtual void freeAligned(void* pointer) = 0;
  virtual void reset() { mAllocated = 0; };

};

} /* namespace DE */

#endif /* ALLOCATOR_H_ */
