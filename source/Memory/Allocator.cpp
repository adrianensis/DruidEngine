#include "Allocator.h"
#include <cstring>

namespace DE {

/*
 * https://en.wikipedia.org/wiki/Data_structure_alignment
 *
 *  ---- pseudo-code: computing padding ----
 * padding = (align - (offset mod align)) mod align
 * new offset = offset + padding = offset + (align - (offset mod align)) mod align
 */

void Allocator::checkAllocate(const u32 size) const {
  DE_ASSERT(mAllocated + size <= mTotalSize, "Total memory size exceeded.");
}

void Allocator::checkAlignment(const u32 alignment) const {
  // Because we need at least, 1 byte for adjustment storage.
  DE_ASSERT(alignment >= 1, "Alignment must be greater than or equal to 1.");

  // 128 (decimal) = 1000 0000 (binary)
  DE_ASSERT(alignment <= 128, "Alignment must be less than or equal to 128.");

  // IMPORTANT: 'alignment must be a power of 2 (typically 4 or 16).
  DE_ASSERT((alignment & (alignment - 1)) == 0, "Alignment is not power of 2."); // power of 2
}

void Allocator::checkFree() const {
  DE_ASSERT(mAllocated > 0, "Allocated memory is 0.");
}

void* Allocator::calculateAlignedAddress(const void* unalignedAddress, const u32 alignment) const {

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
  DE_ASSERT(adjustment < 256, "Adjustment is equal or greater than 256 (uint8_t).");

  // The aligned address is accessed as a uint8_t array or byte array.
  u8* u8Array = reinterpret_cast<u8*>(alignedAddress);

  // We always store this information in the byte immediately preceding
  // (that's why we use [-1]) the adjusted address.
  u8Array[-1] = static_cast<u8>(adjustment);

  return static_cast<void*>(u8Array);
}

void* Allocator::calculateUnalignedAddress(const void* alignedAddress) const {

  const u8* u8Array = reinterpret_cast<const u8*>(alignedAddress);
  ptr _alignedAddress = reinterpret_cast<ptr>(alignedAddress);
  ptrdiff adjustment = static_cast<ptrdiff>(u8Array[-1]);
  ptr unalignedAddress = _alignedAddress - adjustment;

  return reinterpret_cast<void*>(unalignedAddress);
}

void* Allocator::allocateAlignedAddress(void* unalignedAddress, const u32 size, const u32 alignment) {
  u32 expandedSize = size + alignment;

  Allocator::checkAllocate(expandedSize);

  Allocator::checkAlignment(alignment);

  // Game Engine Architecture 2ed, page 246.

  Allocator::setAllocated(Allocator::getAllocated() + expandedSize);

  void* alignedAddress = Allocator::calculateAlignedAddress(unalignedAddress, alignment);
  
  Allocator::clean(alignedAddress, size);

  return alignedAddress;
}

void Allocator::setAllocated(const u32 size){
  mAllocated = size;
}

void Allocator::clean(void *mem, const u32 size) {
  // clean memory block
  std::memset(mem, 0, size);
}

Allocator::Allocator() : DE_Class(){
  mStart = nullptr;
}

Allocator::~Allocator(){
  delete mStart;
  mStart = nullptr;
}

u32 Allocator::getSize() const {
  return mTotalSize;
}

u32 Allocator::getAllocated() const {
  return mAllocated;
}

bool Allocator::hasSpace(const u32 size) const {
  return (mTotalSize-mAllocated) >= size;
}

void Allocator::_init(void* mem) {
  // Only must delete when Allocator is destroyed. See ~Allocator()

  mStart = mem;
}

void Allocator::init(const u32 size) {
  initFromMemory(size, ::operator new (size));
}

void Allocator::initFromMemory(const u32 size, void* mem) {
  mTotalSize = size;

  _init(mem);

  Allocator::reset();
}

void Allocator::reset() {
  mAllocated = 0;
  Allocator::clean(mStart, mTotalSize);
}

} /* namespace DE */
