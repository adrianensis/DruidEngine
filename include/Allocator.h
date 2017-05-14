#ifndef ALLOCATOR_H_
#define ALLOCATOR_H_

#include "Assert.h"
#include "BasicTypes.h"

namespace DE {

class Allocator {

protected:

  u32 mTotalSize;
  u32 mAllocated;
  void* mStart;
  void* mEnd;
  void* mStartCopy; // a backup of mStart for secure the free() call

  void checkAllocate(u32 size);
  void checkFree();
  void setAllocated(u32 size);
  void _init(void* mem);

public:

  Allocator();
  virtual ~Allocator();
  virtual u32 getSize();
  virtual u32 getAllocated();
  virtual bool hasSpace(const u32 size);
  virtual void init(const u32 size);
  virtual void initFromMemory(const u32 size, void* mem);
  virtual void* allocate(const u32 size) = 0;
  virtual void* allocateAligned(const u32 size, const u32 alignment) = 0;
  virtual void free(const void* pointer) = 0;
  virtual void freeAligned(const void* pointer) = 0;
  virtual void reset();

};

} /* namespace DE */

#endif /* ALLOCATOR_H_ */
