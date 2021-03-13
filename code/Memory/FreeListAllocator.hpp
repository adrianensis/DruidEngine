#pragma once

#include "Core/Basic.hpp"
#include "Containers/List.hpp"
#include "Memory/LinearAllocator.hpp"

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
		byte* unalignedAddress;
		u32 size;
		BlockStatus blockStatus;
		Block* next;

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

	Block* mFirstBlockUsed;
	Block* mFirstBlockFree;

	void moveToUsedList(Block *block);
	void moveToFreeList(Block *block);

	Block* allocateBlock(u32 size);
	u32 freeBlock(byte *unalignedAddress);

public:

	DE_CLASS_BODY(FreeListAllocator)

	virtual void init(u32 size) override;
	virtual byte* allocate(u32 size) override;
	virtual byte* allocate(u32 size, u32 alignment) override;
	virtual void free(const byte *pointer) override;
	virtual void flush() override;
	virtual void reset() override;
	virtual void terminate() override;

};

}

