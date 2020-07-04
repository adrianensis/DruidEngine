#ifndef DE_FREELISTALLOCATOR_H
#define DE_FREELISTALLOCATOR_H

#include "Basic.hpp"
#include "List.hpp"
#include "LinearAllocator.hpp"

namespace DE {

/*!
 \brief List of memory blocks.
 */
class FreeListAllocator: public Allocator {

private:

	enum class BlockStatus {
		INVALID, FREE, USED
	};

	class Block {
	public:
		byte *unalignedAddress;
		u32 size;
		BlockStatus blockStatus;
		Block *next;

		Block();
		~Block();
		void init(byte *unalignedAddress, u32 size);

		Block& operator=(const Block &rhs) {
			if (this == &rhs)
				return *this; // handle self assignment
			this->unalignedAddress = rhs.unalignedAddress;
			this->size = rhs.size;
			this->blockStatus = rhs.blockStatus;
			return (*this);
		}
	};

	LinearAllocator mLinearAllocator; // TODO: change for Memory::allocate()
	// List<Block>* mFreeBlocks; // TODO: change to Block*
	// List<Block>* mUsedBlocks;

	Block *firstBlockUsed;
	Block *lastBlockUsed;
	Block *firstBlockFree;
	Block *lastBlockFree;

	void moveToUsedList(Block *block);
	void moveToFreeList(Block *block);

	Block* allocateBlock(u32 size);
	u32 freeBlock(byte *unalignedAddress);

public:

	DE_CLASS(FreeListAllocator, Allocator)

	void init(u32 size) override;
	byte* allocate(u32 size) override;
	byte* allocate(u32 size, u32 alignment) override;
	void free(const byte *pointer) override;
	virtual void flush() override;
	void reset() override;
	void terminate() override;

};

} /* namespace DE */

#endif /* DE_FREELISTALLOCATOR_H */
