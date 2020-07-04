#include "PoolAllocator.hpp"

namespace DE {

const u32 PoolAllocator::smPtrSize = sizeof(ptr);

// ---------------------------------------------------------------------------

void PoolAllocator::storePointer(byte *address, const byte *pointer) {
	ptr *ptrArray = reinterpret_cast<ptr*>(address);

	// header is stored in the last position of the allocated memory.
	ptrArray[0] = reinterpret_cast<ptr>(pointer);
}

// ---------------------------------------------------------------------------

byte* PoolAllocator::getNextIterator(byte *it) {
	ptr ptrValue = reinterpret_cast<ptr*>(it)[0];
	return reinterpret_cast<byte*>(ptrValue);
}

// ---------------------------------------------------------------------------

byte* PoolAllocator::getBlock(const byte *it) {
	ptr ptrValue = reinterpret_cast<ptr>(it);
	return (byte*) (ptrValue - mBlockSize);
}

// ---------------------------------------------------------------------------

byte* PoolAllocator::getIteratorFromBlock(const byte *block) {
	ptr ptrValue = reinterpret_cast<ptr>(block);
	return (byte*) (ptrValue + mBlockSize);
}

// ---------------------------------------------------------------------------

void PoolAllocator::checkAllocateBlock() const {
	DE_ASSERT(mUsedBlocks + 1 <= mMaxBlocks, "Total memory size exceeded.");
}

// ---------------------------------------------------------------------------

void PoolAllocator::checkFreeBlock() const {
	DE_ASSERT(mUsedBlocks > 0, "Allocated memory is 0.");
}

// ---------------------------------------------------------------------------

PoolAllocator::PoolAllocator() :
		LinearAllocator() {
}

// ---------------------------------------------------------------------------

PoolAllocator::~PoolAllocator() {
}

// ---------------------------------------------------------------------------

u32 PoolAllocator::getFreeBlocks() const {
	return mMaxBlocks - mUsedBlocks;
}

// ---------------------------------------------------------------------------

void PoolAllocator::internalInit(u32 blockSize, u32 numBlocks, byte *mem,
		u32 alignment) {
	mUsedBlocks = 0;
	mBlockSize = blockSize;
	mAlignment = alignment;
	mFullBlockSize = smPtrSize + mBlockSize;
	mMaxBlocks = numBlocks;

	if (mem == nullptr) {
		LinearAllocator::init((mAlignment + mFullBlockSize) * mMaxBlocks);
	TRACE()
} else {
	LinearAllocator::initFromMemory((mAlignment + mFullBlockSize) * mMaxBlocks,
			mem);
TRACE()
}

mFirst = (byte*) (reinterpret_cast<ptr>(LinearAllocator::allocate(
	mFullBlockSize, mAlignment)) + mBlockSize);
byte *current = mFirst;
byte *next = nullptr;

 // iterate over all blocks
FOR_RANGE (i, 1, numBlocks)
{
// store the next pointer in the first position of the block
next = (byte*) (reinterpret_cast<ptr>(LinearAllocator::allocate(mFullBlockSize,
		mAlignment)) + mBlockSize);
storePointer(current, next);
current = next;
}

 // store last pointer to null
 // store the next pointer in the first position of the block
storePointer(current, nullptr);
mLast = current;
}

// ---------------------------------------------------------------------------

void PoolAllocator::init(u32 blockSize, u32 numBlocks, u32 alignment) {
TRACE();

PoolAllocator::internalInit(blockSize, numBlocks, nullptr, alignment);
}

// ---------------------------------------------------------------------------

void PoolAllocator::initFromMemory(u32 blockSize, byte *mem, u32 numBlocks,
u32 alignment) {
TRACE();

PoolAllocator::internalInit(blockSize, numBlocks, mem, alignment);
}

// ---------------------------------------------------------------------------

void PoolAllocator::initFromMemory(u32 blockSize, byte *mem, u32 numBlocks) {
TRACE();

PoolAllocator::internalInit(blockSize, numBlocks, mem, 1);
}

// ---------------------------------------------------------------------------

void PoolAllocator::init(u32 blockSize, u32 numBlocks) {
TRACE();

PoolAllocator::init(blockSize, numBlocks, 1);
TRACE()
}

// ---------------------------------------------------------------------------

byte* PoolAllocator::allocateBlock() {
return PoolAllocator::allocate(0);
}

// ---------------------------------------------------------------------------

byte* PoolAllocator::allocate(u32 size) {
PoolAllocator::checkAllocateBlock();

byte *address = getBlock(mFirst); // take the first free block
mFirst = getNextIterator(mFirst); // it++
mUsedBlocks++;
return address;
}

// ---------------------------------------------------------------------------

byte* PoolAllocator::allocate(u32 size, u32 alignment) {
return PoolAllocator::allocate(0);
}

// ---------------------------------------------------------------------------

void PoolAllocator::free(const byte *pointer) {
PoolAllocator::checkFreeBlock();

byte *it = getIteratorFromBlock(pointer);
storePointer(mLast, it); // we can recycle the storePointer function
mLast = it;
storePointer(mLast, nullptr);
mUsedBlocks--;
}

// ---------------------------------------------------------------------------

void PoolAllocator::reset() {
LinearAllocator::reset();
mUsedBlocks = 0;
}

// ---------------------------------------------------------------------------

} /* namespace DE */
