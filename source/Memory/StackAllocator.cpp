#include "StackAllocator.h"
#include "BasicTypes.h"
#include "Debug.h"
#include <iostream>

namespace DE {

using namespace Debug;

void StackAllocator::storeHeader(const ptr address, const u32 size){
  u32* u32Array = reinterpret_cast<u32*>(address);
  u32Array[0] = size;
}

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

  // allocate size + header
  const ptr address = reinterpret_cast<ptr>(LinearAllocator::allocate(size+smHeaderSize));

  // save the top
  mTop = reinterpret_cast<void*>(address);

  // store header
  StackAllocator::storeHeader(address, size);

  return reinterpret_cast<void*>(address+smHeaderSize);
}


void* StackAllocator::allocateAligned(const u32 size, const u32 alignment){

  // allocate size + header + alignment
  const ptr alignedAddress = reinterpret_cast<ptr>(LinearAllocator::allocateAligned(size+smHeaderSize,alignment));

  // save the top
  mTop = reinterpret_cast<void*>(alignedAddress);

  // store header
  StackAllocator::storeHeader(alignedAddress, size+alignment);

  return reinterpret_cast<void*>(alignedAddress+smHeaderSize);
}

void StackAllocator::free(const void* pointer){
  assert(false, "StackAllocator can't use free(void* pointer), use free().");
}

void StackAllocator::freeAligned(const void* pointer){
  assert(false, "StackAllocator can't use freeAligned(void* pointer), use freeAligned().");
}

void StackAllocator::free(){

  // read header
  u32* u32Array = reinterpret_cast<u32*>(mTop);
  u32 size = u32Array[0];

  // reduce mOffset
  mOffset -= smHeaderSize;
  mOffset -= size;

  // reduce mAllocated
  mAllocated = mOffset;

  mTop = reinterpret_cast<void*>(reinterpret_cast<ptr>(mTop) + mOffset);

}

void StackAllocator::freeAligned(){

  this->free();

  const u8* u8Array = reinterpret_cast<const u8*>(mTop);
  ptr alignedAddress = reinterpret_cast<ptr>(mTop);
  ptrdiff adjustment = static_cast<ptrdiff>(u8Array[-1]);
  ptr address = alignedAddress - adjustment;

  mTop = reinterpret_cast<void*>(address);

}

void StackAllocator::reset(){
  LinearAllocator::reset();
  mTop = nullptr;
}

} /* namespace DE */
