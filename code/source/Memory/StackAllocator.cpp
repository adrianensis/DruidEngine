#include "StackAllocator.hpp"

#include <cstring>

namespace DE {

const u32 StackAllocator::smHeaderSize = sizeof(u32);

// ---------------------------------------------------------------------------

void StackAllocator::storeHeader(const byte *address, u32 size) {
	u32 *u32Array = reinterpret_cast<u32*>(reinterpret_cast<ptr>(address));

	// header is stored in the last position of the allocated memory.
	// (the previus one to the following address).

	u32Array[-1] = size;  // store header at the end of the block
}

// ---------------------------------------------------------------------------

StackAllocator::StackAllocator() :
		LinearAllocator() {

}

// ---------------------------------------------------------------------------

StackAllocator::~StackAllocator() {
	mTop = nullptr;
}

// ---------------------------------------------------------------------------

byte* StackAllocator::getTop() {
	return mTop;
}

// ---------------------------------------------------------------------------

void StackAllocator::init(u32 size) {
	TRACE();

	LinearAllocator::init(size);
	TRACE()
	mTop = mStart;
}

// ---------------------------------------------------------------------------

void StackAllocator::initFromMemory(u32 size, byte *mem) {
	TRACE();

	LinearAllocator::initFromMemory(size, mem);
	TRACE()
	mTop = mStart;
}

// ---------------------------------------------------------------------------

byte* StackAllocator::allocate(u32 size) {
	return StackAllocator::allocate(size, 1);
}

// ---------------------------------------------------------------------------

byte* StackAllocator::allocate(u32 size, u32 alignment) {
	// allocate size + header + alignment
	ptr alignedAddress = reinterpret_cast<ptr>(LinearAllocator::allocate(
			size + smHeaderSize, alignment));

	// save the top
	mTop = reinterpret_cast<byte*>(alignedAddress + size + smHeaderSize);

	// store header
	StackAllocator::storeHeader(mTop, size + alignment); // at the end of the block

	return reinterpret_cast<byte*>(alignedAddress);
}

// ---------------------------------------------------------------------------

void StackAllocator::free(const byte *pointer) {
	mTop = (byte*) pointer;
}

// ---------------------------------------------------------------------------

void StackAllocator::free() {
	Allocator::checkFree();

	// read header
	u32 *u32Array = reinterpret_cast<u32*>(mTop);

	u32 size = u32Array[-1];

	// reduce mAllocatedSize
	Allocator::setAllocatedSize(
			Allocator::getAllocatedSize() - smHeaderSize - size);

	// clean memory block
	std::memset(mTop, 0, size);

	mTop = calculateUnalignedAddress(
			(byte*) (reinterpret_cast<ptr>(mStart)
					+ Allocator::getAllocatedSize()));
}

// ---------------------------------------------------------------------------

void StackAllocator::reset() {
	LinearAllocator::reset();
	mTop = nullptr;
}

// ---------------------------------------------------------------------------

} /* namespace DE */
