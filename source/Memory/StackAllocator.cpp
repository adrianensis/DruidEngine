#include "StackAllocator.h"
#include "BasicTypes.h"

namespace DE {

u32 StackAllocator::smHeaderSize = sizeof(u32);

void StackAllocator::storeHeader(const void* address, u32 size){
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

void StackAllocator::init(u32 size){
    LinearAllocator::init(size);
    mTop = mStart;
}

void* StackAllocator::allocate(u32 size){
    return StackAllocator::allocate(size,1);
}

void* StackAllocator::allocate(u32 size, u32 alignment){
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

void StackAllocator::freeAligned(const void* pointer){
    // ASSERT(false, "StackAllocator can't use freeAligned(void* pointer), use freeAligned().");
    mTop = (void*) pointer;
}

void StackAllocator::free(){
    Allocator::checkFree();

    // read header
    u32* u32Array = reinterpret_cast<u32*>(mTop);

    u32 size = 0;

    size = u32Array[-1];

    // reduce mOffset
    mOffset -= smHeaderSize;
    mOffset -= size;

    // reduce mAllocated
    Allocator::setAllocated(mOffset);

    mTop = reinterpret_cast<void*>(reinterpret_cast<ptr>(mStart + mOffset));

    const u8* u8Array = reinterpret_cast<const u8*>(mTop);
    ptr alignedAddress = reinterpret_cast<ptr>(mTop);
    ptrdiff adjustment = static_cast<ptrdiff>(u8Array[-1]);
    ptr address = alignedAddress - adjustment;

    mTop = reinterpret_cast<void*>(address);
}

void StackAllocator::freeAligned(){
    // Allocator::checkFree();
    //
    // StackAllocator::free();
    //
    // const u8* u8Array = reinterpret_cast<const u8*>(mTop);
    // ptr alignedAddress = reinterpret_cast<ptr>(mTop);
    // ptrdiff adjustment = static_cast<ptrdiff>(u8Array[-1]);
    // ptr address = alignedAddress - adjustment;
    //
    // mTop = reinterpret_cast<void*>(address);
}

void StackAllocator::reset(){
    LinearAllocator::reset();
    mTop = nullptr;
}

} /* namespace DE */
