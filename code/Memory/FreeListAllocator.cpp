#include "Memory/FreeListAllocator.hpp"
#include "Maths/MathUtils.hpp"

namespace DE {

FreeListAllocator::Block::Block() {

}

FreeListAllocator::Block::~Block() {

}

void FreeListAllocator::Block::init(byte *unalignedAddress, u32 size) {
	this->unalignedAddress = unalignedAddress;
	this->size = size;
	this->blockStatus = BlockStatus::FREE;
	this->next = nullptr;
}

void FreeListAllocator::moveToUsedList(Block *block) {
	block->next = mFirstBlockUsed;
	mFirstBlockUsed = block;
}

void FreeListAllocator::moveToFreeList(Block *block) {
	block->next = mFirstBlockFree;
	mFirstBlockFree = block;
}

FreeListAllocator::Block* FreeListAllocator::allocateBlock(u32 size) {
	bool found = false;

	Block* it = mFirstBlockFree;
	Block* selectedBlock = nullptr;
	Block* previousBlock = nullptr;
	Block* previousBlockTmp = nullptr;

	u32 iterationsCounter = 0;
	while (it && !found) {

		if (it->size >= size) {
			selectedBlock = it;
			previousBlock = previousBlockTmp;
			found = true;
		}

		previousBlockTmp = it;

		it = it->next;
		iterationsCounter++;
	}

	if (selectedBlock->size == size) {

		if (previousBlock)
			previousBlock->next = selectedBlock->next; // remove block

		if (!previousBlock)
			mFirstBlockFree = selectedBlock->next;

		selectedBlock->blockStatus = BlockStatus::USED;
		moveToUsedList(selectedBlock);

	} else {

		if (previousBlock)
			previousBlock->next = selectedBlock->next; // remove block

		Block* newFreeBlock = Allocator::internalAllocate<Block>(&mLinearAllocator);
		newFreeBlock->init((byte*) (reinterpret_cast<byte*>(selectedBlock->unalignedAddress) + size),
				selectedBlock->size - size);
		newFreeBlock->blockStatus = BlockStatus::FREE;

		if (!previousBlock)
			mFirstBlockFree = selectedBlock->next;
		moveToFreeList(newFreeBlock);

		Block* newUsedBlock = selectedBlock; //Allocator::internalAllocate<Block>(&mLinearAllocator);
		newUsedBlock->init((byte*) (reinterpret_cast<byte*>(selectedBlock->unalignedAddress)), size);
		newUsedBlock->blockStatus = BlockStatus::USED;
		moveToUsedList(newUsedBlock);
	}

	return selectedBlock;
}

u32 FreeListAllocator::freeBlock(byte *unalignedAddress) {
	bool found = false;

	Block* it = mFirstBlockUsed;
	Block* previousBlock = nullptr;
	Block* previousBlockTmp = nullptr;
	Block* selectedBlock = nullptr;

	u32 iterationsCounter = 0;
	while (it && !found) {

		if (it->unalignedAddress == unalignedAddress) {
			found = true;
			selectedBlock = it;
			previousBlock = previousBlockTmp;
		}

		previousBlockTmp = it;

		it = it->next;

		iterationsCounter++;
	}

	if (previousBlock)
		previousBlock->next = selectedBlock->next; // remove block

	if (!previousBlock)
		mFirstBlockUsed = selectedBlock->next;

	selectedBlock->blockStatus = BlockStatus::FREE;

	moveToFreeList(selectedBlock);

	return selectedBlock->size;
}

FreeListAllocator::FreeListAllocator() : Allocator() {

}

FreeListAllocator::~FreeListAllocator() {

}

void FreeListAllocator::init(u32 size) {
	DE_TRACE()

	Allocator::init(size);
	DE_TRACE()
	FreeListAllocator::reset();
}

byte* FreeListAllocator::allocate(u32 size) {
	return FreeListAllocator::allocate(size, 1);
}

byte* FreeListAllocator::allocate(u32 size, u32 alignment) {
	Block* block = allocateBlock(size + alignment);
	byte* unalignedAddress = block->unalignedAddress;
	return Allocator::allocateAlignedAddress(unalignedAddress, size, alignment);
}

void FreeListAllocator::free(const byte *pointer) {
	// pointer is an aligned address
	byte* unalignedAddress = calculateUnalignedAddress(pointer);
	u32 freeSize = freeBlock(unalignedAddress);

	// reduce mAllocatedSize
	Allocator::setAllocatedSize(Allocator::getAllocatedSize() - freeSize);
}

void FreeListAllocator::flush() {

}

void FreeListAllocator::reset() {
	Allocator::reset();

	mLinearAllocator.init(std::max(1024.0f * 10.0f, mTotalSize * 0.1f));

	mFirstBlockFree = Allocator::internalAllocate<Block>(&mLinearAllocator);
	mFirstBlockFree->init(mStart, mTotalSize);

	mFirstBlockUsed = nullptr;
}

void FreeListAllocator::terminate() {

}

}
