#include "LinearAllocator.h"
#include "Assert.h"
#include "BasicTypes.h"
#include <cstdlib> // malloc, free

#include <iostream>

/*
 * https://en.wikipedia.org/wiki/Data_structure_alignment
 *
 *  ---- pseudo-code: computing padding ----
 * padding = (align - (offset mod align)) mod align
 * new offset = offset + padding = offset + (align - (offset mod align)) mod align
 */

namespace DE {
  using namespace std;

LinearAllocator::LinearAllocator() : Allocator(){

}

LinearAllocator::~LinearAllocator(){

}

void LinearAllocator::setReverse(bool isReverse){
  mIsReverse = isReverse;
}

void LinearAllocator::init(u32 size){
  Allocator::init(size);
  mOffset = 0;
  mIsReverse = false;
}

void* LinearAllocator::allocate(u32 size){
  Allocator::checkAllocate(size);

  ptr currentAddress = 0;

  if(mIsReverse)
    currentAddress = reinterpret_cast<ptr>(mEnd - mOffset - size);
  else
    currentAddress = reinterpret_cast<ptr>(mStart + mOffset); // first time -> mStart + 0 = mStart


  mOffset += size;
  Allocator::setAllocated(mOffset);

  return reinterpret_cast<void*>(currentAddress);
}

void* LinearAllocator::allocate(u32 size, u32 alignment){

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

  Allocator::checkAllocate(expandedSize);

  ASSERT(alignment >= 1, "Alignment must be greater than or equal to 1."); // Because we need at least, 1 byte for adjustment storage.

  // 128 (decimal) = 1000 0000 (binary)
  ASSERT(alignment <= 128, "Alignment must be less than or equal to 128.");

  // IMPORTANT: 'alignment must be a power of 2 (typically 4 or 16).
  ASSERT((alignment & (alignment - 1)) == 0, "Alignment is not power of 2."); // power of 2

  // Game Engine Architecture 2ed, page 246.

  // Allocate unaligned block & convert address to uintptr_t.
  ptr address = reinterpret_cast<ptr>(LinearAllocator::allocate(expandedSize));

  // cout << "unaligned address " << address << endl;

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
  ASSERT(adjustment < 256, "Adjustment is equal or greater than 256 (uint8_t).");

  // The aligned address is accessed as a uint8_t array or byte array.
  u8* u8Array = reinterpret_cast<u8*>(alignedAddress);

  // We always store this information in the byte immediately preceding
  // (that's why we use [-1]) the adjusted address.
  u8Array[-1] = static_cast<u8>(adjustment);

  return static_cast<void*>(u8Array);
}

void LinearAllocator::free(const void* pointer){
  // ASSERT(false, "LinearAllocator can't use free(void* pointer), use reset().");
}

void LinearAllocator::freeAligned(const void* pointer){
  // ASSERT(false, "LinearAllocator can't use freeAligned(void* pointer), use reset().");
}

void LinearAllocator::reset(){
  Allocator::reset();
  mOffset = 0;
}

} /* namespace DE */
