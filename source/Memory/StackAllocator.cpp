#include "StackAllocator.h"
#include "BasicTypes.h"
// #include <iostream>

namespace DE {

// using namespace std;

const u32 StackAllocator::smHeaderSize = sizeof(u32);

void StackAllocator::storeHeader(const void* address, const u32 size){
  u32* u32Array = reinterpret_cast<u32*>(reinterpret_cast<ptr>(address));

  // header is stored in the last position of the allocated memory.
  // (the previus one to the following address).
  u32Array[-1] = size;
}

StackAllocator::StackAllocator() : LinearAllocator(){

}

StackAllocator::~StackAllocator(){
  mTop = nullptr;
}

void* StackAllocator::getTop(){
  return mTop;
}

void StackAllocator::init(const u32 size){
  LinearAllocator::init(size);
  mTop = mStart;

  // cout << "init " << reinterpret_cast<ptr>(mTop) << endl;
}

void* StackAllocator::allocate(const u32 size){

  // allocate size + header
  const ptr address = reinterpret_cast<ptr>(LinearAllocator::allocate(size+smHeaderSize));

  // save the top
  mTop = reinterpret_cast<void*>(address+size+smHeaderSize);

  // cout << "allocate mTop " << reinterpret_cast<ptr>(mTop) << endl;

  // store header
  StackAllocator::storeHeader(mTop, size);

  return reinterpret_cast<void*>(address);
}


void* StackAllocator::allocateAligned(const u32 size, const u32 alignment){

  // allocate size + header + alignment
  ptr alignedAddress = reinterpret_cast<ptr>(LinearAllocator::allocateAligned(size+smHeaderSize,alignment));
  // cout << "aligned address " << alignedAddress << endl;

  // save the top
  mTop = reinterpret_cast<void*>(alignedAddress+size+smHeaderSize);

  // cout << "allocateAligned mTop "<< reinterpret_cast<ptr>(mTop) << endl;

  // store header
  StackAllocator::storeHeader(mTop, size+alignment);

  return reinterpret_cast<void*>(alignedAddress);
}

void StackAllocator::free(const void* pointer){
  mTop = (void*) pointer;
}

void StackAllocator::freeAligned(const void* pointer){
  assert(false, "StackAllocator can't use freeAligned(void* pointer), use freeAligned().");
}

void StackAllocator::free(){

  Allocator::checkFree();

  // cout << "free " << reinterpret_cast<ptr>(mTop) << endl;

  // read header
  u32* u32Array = reinterpret_cast<u32*>(mTop);
  u32 size = u32Array[-1];

  // reduce mOffset
  mOffset -= smHeaderSize;
  mOffset -= size;

  // reduce mAllocated
  mAllocated = mOffset;

  mTop = reinterpret_cast<void*>(reinterpret_cast<ptr>(mStart) + mOffset);
  // cout << "free complete " << reinterpret_cast<ptr>(mTop) << endl;

}

void StackAllocator::freeAligned(){

  Allocator::checkFree();

  this->free();

  const u8* u8Array = reinterpret_cast<const u8*>(mTop);
  ptr alignedAddress = reinterpret_cast<ptr>(mTop);
  ptrdiff adjustment = static_cast<ptrdiff>(u8Array[-1]);
  ptr address = alignedAddress - adjustment;

  mTop = reinterpret_cast<void*>(address);

  // cout << "freeAligned " << reinterpret_cast<ptr>(mTop) << endl;

}

void StackAllocator::reset(){
  LinearAllocator::reset();
  mTop = nullptr;
}

} /* namespace DE */
