#include "PoolAllocator.h"
#include "Assert.h"
#include "BasicTypes.h"

// #include <iostream>

namespace DE {

  // using namespace std;

const u32 PoolAllocator::smPtrSize = sizeof(ptr);

void PoolAllocator::storePointer(const void* address, const void* pointer){
  ptr* ptrArray = reinterpret_cast<ptr*>(reinterpret_cast<ptr>(address));

  // header is stored in the last position of the allocated memory.
  // (the previus one to the following address).
  ptrArray[-1] = reinterpret_cast<ptr>(pointer);
}

void* PoolAllocator::getNextIterator(const void* it){
  ptr ptrValue = reinterpret_cast<ptr*>(reinterpret_cast<ptr>(it))[0];
  return reinterpret_cast<void*>(ptrValue);
}

void* PoolAllocator::getBlock(const void* it){
  return (void*)(it - mBlockSize);
}

void* PoolAllocator::getIteratorFromBlock(const void* block){
  return (void*)(block + mBlockSize);
}

PoolAllocator::PoolAllocator() : Allocator(){
}

PoolAllocator::~PoolAllocator(){
}

void PoolAllocator::init(const u32 blockSize, const u32 numBlocks, const u32 alignment){
  this->reset();

  mUsedBlocks = 0;
  mBlockSize = blockSize;
  mAlignment = alignment;
  // mFullBlockSize = mAlignment+mBlockSize+smPtrSize;
  mFullBlockSize = mBlockSize+smPtrSize;
  mNumBlocks = numBlocks;
  Allocator::init(mAlignment+mFullBlockSize*mNumBlocks);

  // align mStart
  if(mAlignment >= 1){
    ptr unaligned = reinterpret_cast<ptr>(mStart);
    u32 mask = alignment - 1;
    ptr misalignment = unaligned & mask;
    ptrdiff adjustment = alignment - misalignment;
    ptr alignedAddress = unaligned + adjustment;
    mStart = reinterpret_cast<void*>(alignedAddress);
  }

  mFirst = mStart + (1*mFullBlockSize) - smPtrSize;
  mLast = mStart + (mNumBlocks*mFullBlockSize) - smPtrSize;

  // cout << "smPtrSize " << smPtrSize << endl;
  // cout << "mBlockSize " << mBlockSize << endl;
  // cout << "mAlignment " << mAlignment << endl;
  // cout << "mFullBlockSize " << mFullBlockSize << endl;
  // cout << "mNumBlocks " << mNumBlocks << endl;
  // cout << "mStart init " << reinterpret_cast<ptr>(mStart) << endl;
  // cout << "mFirst " << reinterpret_cast<ptr>(mFirst) << endl;
  // cout << "mLast " << reinterpret_cast<ptr>(mLast) << endl;

  // cout << "mFirst value " << reinterpret_cast<ptr*>(mFirst)[0] << endl;
  // cout << "mLast value " << reinterpret_cast<ptr*>(mLast)[0] << endl;

  ptr* ptrArray = nullptr;

  // iterate over all blocks
  for (u32 i = 1; i < numBlocks; i++) {

    // cout << "next " << i << " " << reinterpret_cast<ptr>(mStart + (i*mFullBlockSize)) << endl;

    // store the next pointer in the first position of the block
    storePointer(mStart + (i*mFullBlockSize), mStart + ((i+1)*mFullBlockSize) - smPtrSize);


  }

  //last = null pointer.
  ptrArray = reinterpret_cast<ptr*>(mStart + (numBlocks*mFullBlockSize));

  // cout << "sizeof(nullptr) " << sizeof(nullptr) << endl;
  // cout << "reinterpret_cast<ptr>(nullptr) " << reinterpret_cast<ptr>(nullptr) << endl;

  // store the next pointer in the first position of the block
  storePointer(mStart + (numBlocks*mFullBlockSize), nullptr); // nullptr = 0

  // test
  // void* iterator = mFirst;
  // ptr ptrValue = 0;
  //
  // while ( iterator != nullptr ) {
  //
  //   ptrValue = reinterpret_cast<ptr*>(iterator)[0];
  //
  //   cout << "loop " << ptrValue << endl;
  //
  //   iterator = reinterpret_cast<void*>(ptrValue);
  //
  // }
  //
  // cout << endl;

}

void* PoolAllocator::allocate(const u32 size){
  return PoolAllocator::allocate();
}

void* PoolAllocator::allocateAligned(const u32 size, const u32 alignment){
  return PoolAllocator::allocateAligned(alignment);
}

void* PoolAllocator::allocate(){

  void* address = getBlock(mFirst); // take the first free block

  mFirst = getNextIterator(mFirst); // it++

  mUsedBlocks++;

  Allocator::setAllocated(mAlignment + mUsedBlocks*mFullBlockSize);

  return address;
}

void* PoolAllocator::allocateAligned(const u32 alignment){
  assert(false, "PoolAllocator can't use allocateAligned(const u32 alignment) use allocate().");
}

void PoolAllocator::free(const void* pointer){
  void* it = getIteratorFromBlock(pointer);

  storePointer(mLast+smPtrSize, it); // we can recycle the storePointer function

  mLast = it;

  storePointer(mLast+smPtrSize, nullptr);

  mUsedBlocks--;

  Allocator::setAllocated(mAlignment + mUsedBlocks*mFullBlockSize);
}

void PoolAllocator::freeAligned(const void* pointer){
  assert(false, "PoolAllocator can't use freeAligned(const void* pointer) use free(const void* pointer).");

}

// void PoolAllocator::reset(){
//   LinearAllocator::reset();
// }

} /* namespace DE */
