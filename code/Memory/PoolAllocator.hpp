#ifndef DE_POOLALLOCATOR_H
#define DE_POOLALLOCATOR_H

#include "LinearAllocator.hpp"
#include "Basic.hpp"

namespace DE {

/*!
 \brief Allocates memory using fixed-size blocks. The blocks can be freed.
 */
class PoolAllocator: public LinearAllocator {

private:

	DE_M(BlockSize, u32)
	DE_M(FullBlockSize, u32)
	DE_M(Alignment, u32)
	DE_M(MaxBlocks, u32)
	DE_M(UsedBlocks, u32)
	DE_M(First, byte*)
	DE_M(Last, byte*)

	static const u32 smPtrSize;

	static void storePointer(byte *address, const byte *pointer);
	static byte* getNextIterator(byte *it);

	byte* getBlock(const byte *it);
	byte* getIteratorFromBlock(const byte *block);

	void checkAllocateBlock() const;
	void checkFreeBlock() const;

	void internalInit(u32 blockSize, u32 numBlocks, byte *mem, u32 alignment);

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

	void initFromMemory(u32 size, byte *mem, u32 numBlocks);
	void initFromMemory(u32 size, byte *mem, u32 numBlocks, u32 alignment);

	byte* allocate(u32 size) override;
	byte* allocate(u32 size, u32 alignment) override;

	/*!
	 \brief Allocates a single block.
	 \return Pointer to the new block.
	 */
	byte* allocateBlock();

	void free(const byte *pointer) override;
	void reset() override;

};

} /* namespace DE */

#endif /* DE_POOLALLOCATOR_H */
