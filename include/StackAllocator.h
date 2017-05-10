#ifndef STACKALLOCATOR_H_
#define STACKALLOCATOR_H_

#include "LinearAllocator.h"
#include "AtomicTypes.h"

namespace DE {

class StackAllocator : public LinearAllocator {

private:
  void* mTop;
  static const u32 smHeaderSize = sizeof(u32);
public:

  StackAllocator();
  virtual ~StackAllocator();
  virtual void init(const u32 size);
  virtual void* allocate(const u32 size);
  virtual void* allocateAligned(const u32 size, const u32 alignment);
  virtual void free(void* pointer);
  virtual void freeAligned(void* pointer);
  virtual void free();
  virtual void freeAligned();
  virtual void reset();

};

} /* namespace DE */

#endif /* STACKALLOCATOR_H_ */
