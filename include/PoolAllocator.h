#ifndef POOLALLOCATOR_H_
#define POOLALLOCATOR_H_

#include "Allocator.h"
#include "BasicTypes.h"

namespace DE {

class PoolAllocator : public Allocator {

private:
  u32 mBlockSize;
  u32 mFullBlockSize;
  u32 mAlignment;
  u32 mMaxBlocks;
  u32 mUsedBlocks;
  void* mFirst;
  void* mLast;

  static const u32 smPtrSize;
  static void storePointer(const void* address, const void* pointer);
  static void* getNextIterator(const void* it);
  void* getBlock(const void* it);
  void* getIteratorFromBlock(const void* block);

public:

  PoolAllocator();
  virtual ~PoolAllocator();
  u32 getFreeBlocks();
  virtual void init(const u32 blockSize, const u32 numBlocks, const u32 alignment);
  virtual void* allocate(const u32 size);
  virtual void* allocate(const u32 size, const u32 alignment);
  virtual void* allocateBlock();
  virtual void free(const void* pointer);
  virtual void freeAligned(const void* pointer);
  // virtual void reset();

};

} /* namespace DE */

#endif /* POOLALLOCATOR_H_ */
