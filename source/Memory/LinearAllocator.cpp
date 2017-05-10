#include "LinearAllocator.h"
#include "Assert.h"
#include "BasicTypes.h"
#include <cstdlib> // malloc, free

namespace DE {

LinearAllocator::LinearAllocator() : Allocator(){
  mStart = nullptr;
}

LinearAllocator::~LinearAllocator(){
  std::free(mStart);
  mStart = nullptr;
}

void LinearAllocator::init(const u32 size){

  Allocator::init(size);

  if(mStart != nullptr)
    std::free(mStart);

  mStart = std::malloc(size);
  this->reset();
}

void* LinearAllocator::allocate(const u32 size){
  Allocator::checkSpace(size);

  const ptr currentAddress = reinterpret_cast<ptr>(mStart) + mOffset; // first time -> mStart + 0 = mStart
  mOffset += size;
  mAllocated = mOffset;
  return reinterpret_cast<void*>(currentAddress);
}

void* LinearAllocator::allocateAligned(const u32 size, const u32 alignment){

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

  u32 expandedSize = size + alignment;

  Allocator::checkSpace(size + alignment);

  assert(alignment >= 1); // Because we need at least, 1 byte for adjustment storage.

  // 128 (decimal) = 1000 0000 (binary)
  assert(alignment <= 128);

  // IMPORTANT: 'alignment must be a power of 2 (typically 4 or 16).
  assert((alignment & (alignment - 1)) == 0, "Alignment is not power of 2."); // power of 2

  // Game Engine Architecture 2ed, page 246.

  // Allocate unaligned block & convert address to uintptr_t.
  const ptr address = reinterpret_cast<ptr>(LinearAllocator::allocate(expandedSize));

  // Calculate the adjustment by masking off the lower bits
  // of the address, to determine how "misaligned" it is.

  // This mask can be calculated in this way, because of power of 2.
  u32 mask = alignment - 1; // 4 = 100 -> 4 - 1 = 3 = 011

  ptr misalignment = address & mask;
  ptrdiff adjustment = alignment - misalignment;

  ptr alignedAddress = address + adjustment;

  // Store the adjustment in the byte immediately
  // preceding the adjusted address.
  // 256 = 1 0000 0000
  assert(adjustment < 256, "Adjustment is equal or greater than 256 (uint8_t).");

  // The aligned address is accessed as a uint8_t array or byte array.
  u8* u8Array = reinterpret_cast<u8*>(alignedAddress);

  // We always store this information in the byte immediately preceding
  // (that's why we use [-1]) the adjusted address.
  u8Array[-1] = static_cast<u8>(adjustment);

  return static_cast<void*>(u8Array);
}

void LinearAllocator::free(const void* pointer){
  assert(false, "LinearAllocator can't use free(void* pointer), use reset().");
}

void LinearAllocator::freeAligned(const void* pointer){
  assert(false, "LinearAllocator can't use freeAligned(void* pointer), use reset().");
}

void LinearAllocator::reset(){
  Allocator::reset();
  mOffset = 0;
}

} /* namespace DE */
