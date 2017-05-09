#ifndef ALLOCATOR_H_
#define ALLOCATOR_H_

#include <cstddef> // std::size_t
#include <cstdlib>
#include "Assert.h"

namespace DE {

class Allocator {

protected:

  std::size_t mTotalSize;
  std::size_t mAllocated;

  void checkSpace(std::size_t size) { assert(mAllocated + size <= mTotalSize, "Total memory size exceeded."); };

public:

  Allocator() {};
  virtual ~Allocator() {};
  std::size_t getSize() { return mTotalSize; };
  std::size_t getAllocated() { return mAllocated; };
  bool hasSpace(std::size_t) { return mAllocated; };
  virtual void init(const std::size_t size) { mTotalSize = size; mAllocated = 0; };
  virtual void* allocate(const std::size_t size) = 0;
  virtual void* allocateAligned(const std::size_t size, const std::size_t alignment) = 0;
  virtual void free(void* pointer) = 0;
  virtual void freeAligned(void* pointer) = 0;
  virtual void reset() { mAllocated = 0; };

};

} /* namespace DE */

#endif /* ALLOCATOR_H_ */
