#include "LinearAllocator.h"
#include "Assert.h"
#include <cstdlib> // malloc, free
#include <cstdint> // std::uintptr_t
#include <iostream>

using st = std::size_t;
using u8 = std::uint8_t;
using ptr = std::uintptr_t;
using ptrdiff = std::ptrdiff_t;

namespace DE {

LinearAllocator::LinearAllocator() : Allocator(){
  mStart = nullptr;
}

LinearAllocator::~LinearAllocator(){
  std::free(mStart);
  mStart = nullptr;
}

void LinearAllocator::init(const std::size_t size){

  if(mStart != nullptr)
    std::free(mStart);

  Allocator::init(size);
  mStart = std::malloc(size);
  this->reset();
}

void* LinearAllocator::allocate(const std::size_t size){
  Allocator::checkSpace(size);

  const ptr currentAddress = (std::size_t)mStart + mOffset;
  mOffset += size;
  mAllocated = mOffset;
  return (void*) currentAddress;
}

void* LinearAllocator::allocateAligned(const std::size_t size, const std::size_t alignment){

/*

Example:

16-byte alignment. Each '[ ]' is a byte.

-------------------------------------------

... greater addresses
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

  Allocator::checkSpace(size);

  assert(alignment >= 1);

  // 128 (decimal) = 1000 0000 (binary)
  assert(alignment <= 128);

  // IMPORTANT: 'alignment must be a power of 2 (typically 4 or 16).
  assert((alignment & (alignment - 1)) == 0, "Alignment is not power of 2."); // power of 2

  // Game Engine Architecture 2ed, page 246.

  st expandedSize = size + alignment;

  // Allocate unaligned block & convert address to uintptr_t.
  const ptr address = reinterpret_cast<ptr>(allocate(expandedSize));

  std::cout << "--------------------------------" << std::endl;
  std::cout << "alignment " << alignment << std::endl;
  std::cout << "address " << address << std::endl;

  // Calculate the adjustment by masking off the lower bits
  // of the address, to determine how "misaligned" it is.

  // This mask can be calculated in this way, because of power of 2.
  st mask = alignment - 1; // 4 = 100 -> 4 - 1 = 3 = 011

  std::cout << "st mask = alignment - 1; " << mask << std::endl;

  ptr misalignment = address & mask;
  ptrdiff adjustment = alignment - misalignment;

  ptr alignedAddress = address + adjustment;

  std::cout << "ptr misalignment = address & mask; " << misalignment << std::endl;
  std::cout << "ptrdiff adjustment = alignment - misalignment; " << adjustment << std::endl;
  std::cout << "ptr alignedAddress = address + adjustment; " << alignedAddress << std::endl;

  // Store the adjustment in the byte immediately
  // preceding the adjusted address.
  // 256 = 1 0000 0000
  assert(adjustment < 256, "Adjustment is equal or greater than 256 (uint8_t).");

  // The aligned address is accessed as a uint8_t array or byte array.
  u8* pAlignedMem = reinterpret_cast<u8*>(alignedAddress);

  std::cout << "pAlignedMem " << pAlignedMem << std::endl;

  // We always store this information in the byte immediately preceding
  // (that's why we use [-1]) the adjusted address.
  pAlignedMem[-1] = static_cast<u8>(adjustment);

  std::cout << "pAlignedMem[-1] " << pAlignedMem[-1] << std::endl;

  return static_cast<void*>(pAlignedMem);
}

void LinearAllocator::free(void* pointer){
  assert(false, "LinearAllocator can't use free(void* pointer), use reset().");
}

void LinearAllocator::freeAligned(void* pointer){
  assert(false, "LinearAllocator can't use freeAligned(void* pointer), use reset().");
}

void LinearAllocator::reset(){
  Allocator::reset();
  mOffset = 0;
}

} /* namespace DE */
