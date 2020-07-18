#include "LinearAllocator.hpp"

namespace DE {

// ---------------------------------------------------------------------------

LinearAllocator::LinearAllocator() : Allocator() {

}

LinearAllocator::~LinearAllocator() {

}

// ---------------------------------------------------------------------------

void LinearAllocator::init(u32 size) {
	TRACE();

	Allocator::init(size);
TRACE()
}

// ---------------------------------------------------------------------------

byte* LinearAllocator::allocate(u32 size) {
return LinearAllocator::allocate(size, 1);
}

// ---------------------------------------------------------------------------

byte* LinearAllocator::allocate(u32 size, u32 alignment) {
return Allocator::allocateAlignedAddress((byte*) (reinterpret_cast<byte*>(mStart) + Allocator::getAllocatedSize()),
		size, alignment);
}

// ---------------------------------------------------------------------------

void LinearAllocator::free(const byte *pointer) {
// DE_ASSERT(false, "LinearAllocator can't use free(byte* pointer), use reset().");
}

// ---------------------------------------------------------------------------

void LinearAllocator::reset() {
Allocator::reset();
}

// ---------------------------------------------------------------------------

} /* namespace DE */
