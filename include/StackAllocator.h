#ifndef STACKALLOCATOR_H_
#define STACKALLOCATOR_H_

#include "LinearAllocator.h"

namespace DE {

class StackAllocator : public LinearAllocator {

private:
  void* mTop;
public:

  StackAllocator();
  virtual ~StackAllocator();
  virtual void init(const std::size_t size);
  virtual void* allocate(const std::size_t size);
  virtual void* allocateAligned(const std::size_t size, const std::size_t alignment);
  virtual void free(void* pointer);
  virtual void freeAligned(void* pointer);
  virtual void reset();

};

} /* namespace DE */

#endif /* STACKALLOCATOR_H_ */
