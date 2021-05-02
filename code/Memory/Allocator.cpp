#include "Memory/Allocator.hpp"
#include <cstring>

namespace DE {

/*
 * https://en.wikipedia.org/wiki/Data_structure_alignment
 *
 *  ---- pseudo-code: computing padding ----
 * padding = (align - (offset mod align)) mod align
 * new offset = offset + padding = offset + (align - (offset mod align)) mod align
 */

void Allocator::checkAllocate(u32 size) const {
	/*VAR(u32, mTotalSize)
	VAR(u32, mAllocatedSize)
	VAR(u32, size)
	VAR(u32, mAllocatedSize + size)*/
	ASSERT(mAllocatedSize + size <= mTotalSize, "Total memory size exceeded.");
}

void Allocator::checkAlignment(u32 alignment) const {
	// Because we need at least, 1 byte for adjustment storage.
	ASSERT(alignment >= 1, "Alignment must be greater than or equal to 1.");

	// 128 (decimal) = 1000 0000 (binary)
	ASSERT(alignment <= 128, "Alignment must be less than or equal to 128.");

	// IMPORTANT: 'alignment must be a power of 2 (typically 4 or 16).
	ASSERT((alignment & (alignment - 1)) == 0, "Alignment is not power of 2."); // power of 2
}

void Allocator::checkFree() const {
	ASSERT(mAllocatedSize > 0, "Allocated memory is 0.");
}

byte* Allocator::calculateAlignedAddress(const byte *unalignedAddress, u32 alignment) const {

	/*

	 Example:

	 16-byte alignment. Each '[ ]' is a byte.

	 -------------------------------------------

	 ... higher addresses
	 ↑

	 0x21AC [ ] [ ] [ ] [ ]
	 0x21A8 [ ] [ ] [ ] [ ]
	 0x21A4 [ ] [ ] [ ] [ ]
	 0x21A0 [b] [ ] [ ] [ ]
	 0x219C [a] [x] [x] [4]

	 ↓
	 ... lower addresses

	 -------------------------------------------

	 a -> unaligned address
	 b -> 16-aligned address
	 4 -> this is the preceding byte to the aligned address (b)
	 x -> just data

	 -------------------------------------------

	 Other version:

	 [a  4bxxxxxxxxx|  ]
	 size of the chunk of memory

	 */

	ptr _unalignedAddress = reinterpret_cast<ptr>(unalignedAddress);

	// Calculate the adjustment by masking off the lower bits
	// of the address, to determine how "misaligned" it is.

	// This mask can be calculated in this way, because of power of 2.
	u32 mask = alignment - 1; // 4 = 100 -> 4 - 1 = 3 = 011

	ptr misalignment = _unalignedAddress & mask;
	ptrdiff adjustment = alignment - misalignment;

	ptr alignedAddress = _unalignedAddress + adjustment;

	// Store the adjustment in the byte immediately
	// preceding the adjusted address.
	// 256 = 1 0000 0000
	ASSERT(adjustment < 256, "Adjustment is equal or greater than 256 (uint8_t).");

	// The aligned address is accessed as a uint8_t array or byte array.
	byte* byteArray = reinterpret_cast<byte*>(alignedAddress);

	// We always store this information in the byte immediately preceding
	// (that's why we use [-1]) the adjusted address.
	byteArray[-1] = static_cast<byte>(adjustment);

	return static_cast<byte*>(byteArray);
}

byte* Allocator::calculateUnalignedAddress(const byte *alignedAddress) const {

	const byte* byteArray = reinterpret_cast<const byte*>(alignedAddress);
	ptr _alignedAddress = reinterpret_cast<ptr>(alignedAddress);
	ptrdiff adjustment = static_cast<ptrdiff>(byteArray[-1]);
	ptr unalignedAddress = _alignedAddress - adjustment;

	return reinterpret_cast<byte*>(unalignedAddress);
}

byte* Allocator::allocateAlignedAddress(byte *unalignedAddress, u32 size, u32 alignment) {
	u32 expandedSize = size + alignment;

	Allocator::checkAllocate(expandedSize);

	Allocator::checkAlignment(alignment);

	// Game Engine Architecture 2ed, page 246.

	Allocator::setAllocatedSize(Allocator::mAllocatedSize + expandedSize);

	byte* alignedAddress = Allocator::calculateAlignedAddress(unalignedAddress, alignment);

	Allocator::clean(alignedAddress, size);

	return alignedAddress;
}

void Allocator::setAllocatedSize(u32 size) {
	mAllocatedSize = size;

	mDebugMaxAllocatedSize = mDebugMaxAllocatedSize < mAllocatedSize ? mAllocatedSize : mDebugMaxAllocatedSize;
}

void Allocator::clean(byte *mem, u32 size) {
	// clean memory block
	std::memset(mem, 0, size);
}

Allocator::Allocator() : ObjectBase() {
	mStart = nullptr;
}

Allocator::~Allocator() {
	delete mStart;
	mStart = nullptr;
}

bool Allocator::hasSpace(u32 size) const {
	return (mTotalSize - mAllocatedSize) >= size;
}

void Allocator::setMemoryChunk(byte *mem) {
	// Only must delete when Allocator is destroyed. See ~Allocator()

	mStart = mem;
}

void Allocator::init(u32 size) {
	TRACE()

	initFromMemory(size, new byte[size]);
}

void Allocator::initFromMemory(u32 size, byte *mem) {
	TRACE()

	mTotalSize = size;

	setMemoryChunk(mem);

	Allocator::reset();
}

void Allocator::flush() {

}

void Allocator::terminate() {

}

void Allocator::reset() {
	mAllocatedSize = 0;
	mDebugMaxAllocatedSize = 0;
	Allocator::clean(mStart, mTotalSize);
}

}
