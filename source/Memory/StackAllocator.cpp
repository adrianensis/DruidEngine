#include "StackAllocator.h"
#include "AtomicTypes.h"
#include <iostream>

namespace DE {

void StackAllocator::storeHeader(const ptr address, const u32 size){
  u32* u32Array = reinterpret_cast<u32*>(address);
  u32Array[0] = size;

  std::cout << "storeHeader " << u32Array[0] << std::endl;
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

  std::cout << "allocate " << size << std::endl;

  // allocate size + header
  const ptr address = reinterpret_cast<ptr>(LinearAllocator::allocate(size+smHeaderSize));

  std::cout << ">  " << address << std::endl;

  // save the top
  mTop = reinterpret_cast<void*>(address);

  // store header
  StackAllocator::storeHeader(address, size);

  std::cout << ">  " << address+smHeaderSize << std::endl;

  return reinterpret_cast<void*>(address+smHeaderSize);
}


void* StackAllocator::allocateAligned(const u32 size, const u32 alignment){

  std::cout << "allocateAligned " << std::endl;

  // allocate size + header + alignment
  const ptr alignedAddress = reinterpret_cast<ptr>(LinearAllocator::allocateAligned(size+smHeaderSize,alignment));

  std::cout << ">  " << alignedAddress << std::endl;
  std::cout << "mOffset  " << mOffset << std::endl;

  // save the top
  mTop = reinterpret_cast<void*>(alignedAddress);

  // store header
  StackAllocator::storeHeader(alignedAddress, size+alignment);

  std::cout << ">  " << alignedAddress+smHeaderSize << std::endl;
  std::cout << "> mTop  " << reinterpret_cast<ptr>(mTop) << std::endl << std::endl;

  return reinterpret_cast<void*>(alignedAddress+smHeaderSize);
}

void StackAllocator::free(void* pointer){
  assert(false, "StackAllocator can't use free(void* pointer), use free().");
}

void StackAllocator::freeAligned(void* pointer){
  assert(false, "StackAllocator can't use freeAligned(void* pointer), use freeAligned().");
}

void StackAllocator::free(){

  std::cout << "free " << std::endl;

  std::cout << ">  " << reinterpret_cast<ptr>(mTop) << std::endl;

  // read header
  u32* u32Array = reinterpret_cast<u32*>(mTop);
  u32 size = u32Array[0];

  std::cout << "size free " << size << std::endl;
  std::cout << "mOffset 1 " << mOffset << std::endl;

  // reduce mOffset
  mOffset -= smHeaderSize;
  mOffset -= size;

  // reduce mAllocated
  mAllocated = mOffset;

  mTop = reinterpret_cast<void*>(reinterpret_cast<ptr>(mTop) + mOffset);

  std::cout << "mOffset 2 " << mOffset << std::endl;
}

void StackAllocator::freeAligned(){

  std::cout << "freeAligned " << std::endl;

  std::cout << ">  " << reinterpret_cast<ptr>(mTop) << std::endl;



  this->free();

  const u8* u8Array = reinterpret_cast<const u8*>(mTop);
  ptr alignedAddress = reinterpret_cast<ptr>(mTop);
  ptrdiff adjustment = static_cast<ptrdiff>(u8Array[-1]);
  ptr address = alignedAddress - adjustment;

  mTop = reinterpret_cast<void*>(address);

  std::cout << "> original  " << reinterpret_cast<ptr>(mTop) << std::endl;

}

void StackAllocator::reset(){
  LinearAllocator::reset();
  mTop = nullptr;
}

} /* namespace DE */
