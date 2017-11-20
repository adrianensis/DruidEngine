#ifndef POOLALLOCATOR_H_
#define POOLALLOCATOR_H_

#include "Allocator.h"
#include "BasicTypes.h"

namespace DE {

    /*!
        \brief Allocates memory using fixed-size blocks. The blocks can be freed.
    */
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

  /*!
     \brief Default Constructor.
  */
  PoolAllocator();

  /*!
     \brief Destructor.
  */
  virtual ~PoolAllocator();

  /*!
     \return The count of free blocks.
  */
  u32 getFreeBlocks();

  /*!
     \brief Constructor.
     \param blockSize Size of a single block.
     \param numBlocks Number of blocks.
     \param alignment Bytes alignment.
  */
  virtual void init(const u32 blockSize, const u32 numBlocks, const u32 alignment);

  virtual void* allocate(const u32 size);
  virtual void* allocate(const u32 size, const u32 alignment);

  /*!
     \brief Allocates a single block.
     \return Pointer to the new block.
  */
  void* allocateBlock();

  virtual void free(const void* pointer);
  virtual void freeAligned(const void* pointer);
  // virtual void reset(); // TODO: implement

};

} /* namespace DE */

#endif /* POOLALLOCATOR_H_ */
