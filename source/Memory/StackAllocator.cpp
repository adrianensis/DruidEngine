#include "StackAllocator.h"

namespace DE {

u32 StackAllocator::smHeaderSize = sizeof(u32);

void StackAllocator::storeHeader(const void* address, const u32 size){
    u32* u32Array = reinterpret_cast<u32*>(reinterpret_cast<ptr>(address));

    // header is stored in the last position of the allocated memory.
    // (the previus one to the following address).

    u32Array[-1] = size;  // store header at the end of the block
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
}

void StackAllocator::initFromMemory(const u32 size, void* mem){
    LinearAllocator::initFromMemory(size, mem);
    mTop = mStart;
}

void* StackAllocator::allocate(const u32 size){
    return StackAllocator::allocate(size,1);
}

void* StackAllocator::allocate(const u32 size, const u32 alignment){
    // allocate size + header + alignment
    ptr alignedAddress = reinterpret_cast<ptr>(LinearAllocator::allocate(size+smHeaderSize,alignment));

    // save the top
    mTop = reinterpret_cast<void*>(alignedAddress+size+smHeaderSize);

    // store header
    StackAllocator::storeHeader(mTop, size+alignment); // at the end of the block

    return reinterpret_cast<void*>(alignedAddress);
}

void StackAllocator::free(const void* pointer){
    mTop = (void*) pointer;
}

void StackAllocator::free(){
    Allocator::checkFree();

    // read header
    u32* u32Array = reinterpret_cast<u32*>(mTop);

    u32 size = u32Array[-1];

    // reduce mAllocated
    Allocator::setAllocated(Allocator::getAllocated() - smHeaderSize - size);

    mTop = calculateUnalignedAddress(mStart + Allocator::getAllocated());
}

void StackAllocator::reset(){
    LinearAllocator::reset();
    mTop = nullptr;
}

} /* namespace DE */
