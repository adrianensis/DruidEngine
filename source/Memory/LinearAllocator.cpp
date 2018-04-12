#include "LinearAllocator.h"

namespace DE {

LinearAllocator::LinearAllocator() : IAllocator(){

}

LinearAllocator::~LinearAllocator(){

}

void LinearAllocator::init(const u32 size){
    IAllocator::init(size);
}

void* LinearAllocator::allocate(const u32 size){
    LinearAllocator::allocate(size, 1);
}

void* LinearAllocator::allocate(const u32 size, const u32 alignment){
    return IAllocator::allocateAlignedAddress(mStart + IAllocator::getAllocated(), size, alignment);
}

void LinearAllocator::free(const void* pointer){
    // DE_ASSERT(false, "LinearAllocator can't use free(void* pointer), use reset().");
}

void LinearAllocator::reset(){
    IAllocator::reset();
}

} /* namespace DE */
