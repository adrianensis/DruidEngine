#include "PoolAllocator.h"
#include "Assert.h"
#include "BasicTypes.h"
#include "Debug.h"

namespace DE {

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

void PoolAllocator::checkAllocateBlock() {
    ASSERT(mUsedBlocks + 1 <= mMaxBlocks, "Total memory size exceeded.");
}

void PoolAllocator::checkFreeBlock() {
    ASSERT(mUsedBlocks > 0, "Allocated memory is 0.");
}

PoolAllocator::PoolAllocator() : LinearAllocator(){
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
    mFullBlockSize = smPtrSize + mBlockSize ;
    mMaxBlocks = numBlocks;
    LinearAllocator::init((mAlignment + mFullBlockSize) * mMaxBlocks);

    // align mStart
    // if(mAlignment >= 1){
    //     ptr unalignedAddress = reinterpret_cast<ptr>(mStart);
    //     u32 mask = alignment - 1;
    //     ptr misalignment = unalignedAddress & mask;
    //     ptrdiff adjustment = alignment - misalignment;
    //     ptr alignedAddress = unalignedAddress + adjustment;
    //     mStart = reinterpret_cast<void*>(alignedAddress);
    // }



    // mFirst = mStart + (1*mFullBlockSize) - smPtrSize;
    // mLast = mStart + (mMaxBlocks*mFullBlockSize) - smPtrSize;

    mFirst = LinearAllocator::allocate(mFullBlockSize, mAlignment) + mBlockSize;
    void* current = mFirst;
    void* next = nullptr;

    // iterate over all blocks
    for (u32 i = 1; i < numBlocks; i++) {
        // store the next pointer in the first position of the block
        next = LinearAllocator::allocate(mFullBlockSize, mAlignment) + mBlockSize;
        storePointer(current, next);
        current = next;
    }

    // store last pointer to null
    // store the next pointer in the first position of the block
    storePointer(current, nullptr); // nullptr = 0
    mLast = current;
}

void PoolAllocator::init(u32 blockSize, u32 numBlocks){
    PoolAllocator::init(blockSize, numBlocks, 1);
}

void* PoolAllocator::allocateBlock(){
    return PoolAllocator::allocate(0);
}

void* PoolAllocator::allocate(u32 size){
    PoolAllocator::checkAllocateBlock();

    void* address = getBlock(mFirst); // take the first free block
    mFirst = getNextIterator(mFirst); // it++
    mUsedBlocks++;
    return address;
}

void* PoolAllocator::allocate(u32 size, u32 alignment){
    return PoolAllocator::allocate(0);
}

void PoolAllocator::free(const void* pointer){
    PoolAllocator::checkFreeBlock();

    void* it = getIteratorFromBlock(pointer);
    storePointer(mLast, it); // we can recycle the storePointer function
    mLast = it;
    storePointer(mLast, nullptr);
    mUsedBlocks--;
}

void PoolAllocator::reset(){
    LinearAllocator::reset();
    mUsedBlocks = 0;
}

} /* namespace DE */
