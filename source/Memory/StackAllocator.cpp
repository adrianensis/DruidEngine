#include "StackAllocator.h"
#include "AtomicTypes.h"
#include <iostream>

namespace DE {

StackAllocator::StackAllocator() : LinearAllocator(){

}

StackAllocator::~StackAllocator(){
  mTop = nullptr;
}

void StackAllocator::init(const u32 size){
  LinearAllocator::init(size);
  mTop = mStart;
}

void* StackAllocator::allocate(const u32 size){

  const ptr address = reinterpret_cast<ptr>(LinearAllocator::allocate(size+smHeaderSize));
  mTop = reinterpret_cast<void*>(address);

  u32* u32Array = reinterpret_cast<u32*>(address);
  u32Array[0] = size;

  // std::cout << "pAlignedMem[0] " << pAlignedMem[0] << std::endl;

  return reinterpret_cast<void*>(address+smHeaderSize);
}


void* StackAllocator::allocateAligned(const u32 size, const u32 alignment){

  const ptr alignedAddress = reinterpret_cast<ptr>(LinearAllocator::allocateAligned(size+smHeaderSize,alignment));
  mTop = reinterpret_cast<void*>(alignedAddress);

  u32* u32Array = reinterpret_cast<u32*>(alignedAddress);
  u32Array[0] = size+alignment; // expanded size

  return reinterpret_cast<void*>(alignedAddress+smHeaderSize);
}

void StackAllocator::free(void* pointer){
  assert(false, "StackAllocator can't use free(void* pointer), use free().");
}

void StackAllocator::freeAligned(void* pointer){
  assert(false, "StackAllocator can't use freeAligned(void* pointer), use freeAligned().");
}

void StackAllocator::free(){

  u32* u32Array = reinterpret_cast<u32*>(mTop);
  u32 size = u32Array[0];

  mOffset -= smHeaderSize;
  mOffset -= size; // remember: expanded size = size + alignment

  mAllocated = mOffset;

  mTop = reinterpret_cast<void*>(reinterpret_cast<ptr>(mTop) - mOffset);
}

void StackAllocator::freeAligned(){

  const u8* u8Array = reinterpret_cast<const u8*>(mTop);
  ptr alignedAddress = reinterpret_cast<ptr>(mTop);
  ptrdiff adjustment = static_cast<ptrdiff>(u8Array[-1]);
  ptr address = alignedAddress - adjustment;
  void* originalAddress = reinterpret_cast<void*>(address);

  mTop = originalAddress;

  this->free(originalAddress);
}

void StackAllocator::reset(){
  LinearAllocator::reset();
  mTop = nullptr;
}

} /* namespace DE */
