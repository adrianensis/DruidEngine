#include "PoolAllocator.h"
#include "Assert.h"
#include "BasicTypes.h"
#include "Debug.h"

// #include <iostream>

namespace DE {

  // using namespace std;

u32 PoolAllocator::smPtrSize = sizeof(ptr);

void PoolAllocator::storePointer(const void* address, const void* pointer){
    ptr* ptrArray = reinterpret_cast<ptr*>(reinterpret_cast<ptr>(address));

    // header is stored in the last position of the allocated memory.
    ptrArray[0] = reinterpret_cast<ptr>(pointer);
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

u32 PoolAllocator::getFreeBlocks(){
    return mMaxBlocks - mUsedBlocks;
}

void PoolAllocator::init(u32 blockSize, u32 numBlocks, u32 alignment){
    Allocator::reset();

    mUsedBlocks = 0;
    mBlockSize = blockSize;
    mAlignment = alignment;
    mFullBlockSize = mBlockSize+smPtrSize;
    mMaxBlocks = numBlocks;
    Allocator::init(mAlignment+mFullBlockSize*mMaxBlocks);

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
    mLast = mStart + (mMaxBlocks*mFullBlockSize) - smPtrSize;

    ptr* ptrArray = nullptr;

    // iterate over all blocks
    for (u32 i = 1; i < numBlocks; i++) {
        // store the next pointer in the first position of the block
        storePointer(mStart + (i*mFullBlockSize) - smPtrSize, mStart + ((i+1)*mFullBlockSize) - smPtrSize);
    }

    //last = null pointer.
    ptrArray = reinterpret_cast<ptr*>(mStart + (numBlocks*mFullBlockSize));

    // store the next pointer in the first position of the block
    storePointer(mStart + (numBlocks*mFullBlockSize) - smPtrSize, nullptr); // nullptr = 0
}

void* PoolAllocator::allocateBlock(){
    return PoolAllocator::allocate(0);
}

// void* PoolAllocator::allocate(u32 alignment){
//   return PoolAllocator::allocate(alignment);
// }

void* PoolAllocator::allocate(u32 size){
    Allocator::checkAllocate(mFullBlockSize);

    void* address = getBlock(mFirst); // take the first free block
    mFirst = getNextIterator(mFirst); // it++
    mUsedBlocks++;
    Allocator::setAllocated(mAlignment + mUsedBlocks*mFullBlockSize);
    return address;
}

void* PoolAllocator::allocate(u32 size, u32 alignment){
    return PoolAllocator::allocate(0);
}

void PoolAllocator::free(const void* pointer){
    Allocator::checkFree();

    void* it = getIteratorFromBlock(pointer);
    storePointer(mLast, it); // we can recycle the storePointer function
    mLast = it;
    storePointer(mLast, nullptr);
    mUsedBlocks--;
    Allocator::setAllocated(mAlignment + mUsedBlocks*mFullBlockSize);
}

void PoolAllocator::freeAligned(const void* pointer){
    PoolAllocator::free(pointer);
}

void PoolAllocator::reset(){
    Allocator::reset();
    mUsedBlocks = 0;
}

} /* namespace DE */
