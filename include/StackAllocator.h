#ifndef STACKALLOCATOR_H_
#define STACKALLOCATOR_H_

#include "LinearAllocator.h"
#include "BasicTypes.h"

namespace DE {

class StackAllocator : public LinearAllocator {

private:
  void* mTop;

  static const u32 smHeaderSize;
  void storeHeader(const void* address, const u32 size);

public:

  StackAllocator();
  virtual ~StackAllocator();
  void* getTop();
  virtual void init(const u32 size);
  virtual void* allocate(const u32 size);
  virtual void* allocateAligned(const u32 size, const u32 alignment);
  virtual void free(const void* pointer);
  virtual void freeAligned(const void* pointer);
  virtual void free();
  virtual void freeAligned();
  virtual void reset();

};

} /* namespace DE */

#endif /* STACKALLOCATOR_H_ */
