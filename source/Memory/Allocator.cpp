#include "Allocator.h"
#include "Assert.h"
#include "BasicTypes.h"

namespace DE {

/*
 * https://en.wikipedia.org/wiki/Data_structure_alignment
 *
 *  ---- pseudo-code: computing padding ----
 * padding = (align - (offset mod align)) mod align
 * new offset = offset + padding = offset + (align - (offset mod align)) mod align
 */

void Allocator::checkAllocate(u32 size) {
    ASSERT(mAllocated + size <= mTotalSize, "Total memory size exceeded.");
}

void Allocator::checkAlignment(u32 alignment) {
    // Because we need at least, 1 byte for adjustment storage.
    ASSERT(alignment >= 1, "Alignment must be greater than or equal to 1.");

    // 128 (decimal) = 1000 0000 (binary)
    ASSERT(alignment <= 128, "Alignment must be less than or equal to 128.");

    // IMPORTANT: 'alignment must be a power of 2 (typically 4 or 16).
    ASSERT((alignment & (alignment - 1)) == 0, "Alignment is not power of 2."); // power of 2
}

void Allocator::checkFree() {
    ASSERT(mAllocated > 0, "Allocated memory is 0.");
}

void* Allocator::calculateAlignedAddress(const void* unalignedAddress, u32 alignment) {

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

      [a  4bxxxxxxxxx|                  ]
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
    u8* u8Array = reinterpret_cast<u8*>(alignedAddress);

    // We always store this information in the byte immediately preceding
    // (that's why we use [-1]) the adjusted address.
    u8Array[-1] = static_cast<u8>(adjustment);

    return static_cast<void*>(u8Array);
}

void* Allocator::calculateUnalignedAddress(const void* alignedAddress) {

    const u8* u8Array = reinterpret_cast<const u8*>(alignedAddress);
    ptr _alignedAddress = reinterpret_cast<ptr>(alignedAddress);
    ptrdiff adjustment = static_cast<ptrdiff>(u8Array[-1]);
    ptr unalignedAddress = _alignedAddress - adjustment;

    return reinterpret_cast<void*>(unalignedAddress);
}

void* Allocator::allocateAlignedAddress(const void* unalignedAddress, u32 size, u32 alignment) {
    u32 expandedSize = size + alignment;

    Allocator::checkAllocate(expandedSize);

    Allocator::checkAlignment(alignment);

    // Game Engine Architecture 2ed, page 246.

    Allocator::setAllocated(Allocator::getAllocated() + expandedSize);

    // Allocate unaligned block & convert address to uintptr_t.
    return Allocator::calculateAlignedAddress(unalignedAddress, alignment);
}

void Allocator::setAllocated(u32 size){
    mAllocated = size;
}

Allocator::Allocator(){
    mStart = nullptr;
}

Allocator::~Allocator(){
    delete mStart;
    mStart = nullptr;
}

u32 Allocator::getSize() {
    return mTotalSize;
}

u32 Allocator::getAllocated() {
    return mAllocated;
}

bool Allocator::hasSpace(u32 size) {
    return (mTotalSize-mAllocated) >= size;
}

void Allocator::_init(void* mem) {
    // Only must delete when Allocator is destroyed. See ~Allocator()
    // if(mStartCopy != nullptr)
    //     delete mStartCopy;

    mStart = mem;
    mEnd = mStart + mTotalSize - 1;
}

void Allocator::init(u32 size) {
    initFromMemory(size, ::operator new (size));
}

void Allocator::initFromMemory(u32 size, void* mem) {
    mTotalSize = size;

    Allocator::reset();

    _init(mem);
}

void Allocator::reset() {
    mAllocated = 0;
}


} /* namespace DE */
