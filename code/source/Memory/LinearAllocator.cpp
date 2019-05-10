#include "LinearAllocator.h"

namespace DE {

LinearAllocator::LinearAllocator() : Allocator(){

}

LinearAllocator::~LinearAllocator(){

}

void LinearAllocator::init(u32 size){
  Allocator::init(size);
}

void* LinearAllocator::allocate(u32 size){
  LinearAllocator::allocate(size, 1);
}

void* LinearAllocator::allocate(u32 size, u32 alignment){
  return Allocator::allocateAlignedAddress(mStart + Allocator::getAllocatedSize(), size, alignment);
}

void LinearAllocator::free(const void* pointer){
  // DE_ASSERT(false, "LinearAllocator can't use free(void* pointer), use reset().");
}

void LinearAllocator::reset(){
  Allocator::reset();
}

} /* namespace DE */
