#ifndef DE_POOLALLOCATOR_H
#define DE_POOLALLOCATOR_H

#include "LinearAllocator.h"
#include "Basic.h"

namespace DE {

/*!
  \brief Allocates memory using fixed-size blocks. The blocks can be freed.
*/
class PoolAllocator : public LinearAllocator {

private:

  u32 mBlockSize;
  u32 mFullBlockSize;
  u32 mAlignment;
  u32 mMaxBlocks;
  u32 mUsedBlocks;
  void* mFirst;
  void* mLast;

  static const u32 smPtrSize;

  static void storePointer(void* address, const void* pointer);
  static void* getNextIterator(void* it);

  void* getBlock(const void* it);
  void* getIteratorFromBlock(const void* block);

  void checkAllocateBlock() const;
  void checkFreeBlock() const;

  void internalInit(u32 blockSize, u32 numBlocks, void* mem, u32 alignment);

public:

  DE_CLASS(PoolAllocator, LinearAllocator)

  /*!
  \return The count of free blocks.
  */
  u32 getFreeBlocks() const;

  /*!
  \brief Constructor.
  \param blockSize Size of a single block.
  \param numBlocks Number of blocks.
  \param alignment Bytes alignment.
  */
  void init(u32 blockSize, u32 numBlocks, u32 alignment);

  /*!
  \brief Constructor.
  \param blockSize Size of a single block.
  \param numBlocks Number of blocks.
  */
  void init(u32 blockSize, u32 numBlocks);

  void initFromMemory(u32 size, void* mem, u32 numBlocks);
  void initFromMemory(u32 size, void* mem, u32 numBlocks, u32 alignment);

  void* allocate(u32 size) override;
  void* allocate(u32 size, u32 alignment) override;

  /*!
  \brief Allocates a single block.
  \return Pointer to the new block.
  */
  void* allocateBlock();

  void free(const void* pointer) override;
  void reset() override;

};

} /* namespace DE */

#endif /* DE_POOLALLOCATOR_H */
