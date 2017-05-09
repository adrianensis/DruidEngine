#include "StackAllocator.h"
#include "AtomicTypes.h"


namespace DE {

StackAllocator::StackAllocator() : LinearAllocator(){

}

StackAllocator::~StackAllocator(){
  mTop = nullptr;
}

void* StackAllocator::allocate(const u32 size){
  return LinearAllocator::allocate(size);
}

void StackAllocator::init(const u32 size){
  LinearAllocator::init(size);
  mTop = mStart;
}

void* StackAllocator::allocateAligned(const u32 size, const u32 alignment){
  return LinearAllocator::allocateAligned(size,alignment);
}

void StackAllocator::free(void* pointer){

}

void StackAllocator::freeAligned(void* pointer){

  const u8* pAlignedMem = reinterpret_cast<const u8*>(pointer);
  ptr alignedAddress = reinterpret_cast<ptr>(pointer);
  ptrdiff adjustment = static_cast<ptrdiff>(pAlignedMem[-1]);
  ptr address = alignedAddress - adjustment;
  void* pRawMem = reinterpret_cast<void*>(address);
  this->free(pRawMem);
}

void StackAllocator::reset(){
  LinearAllocator::reset();
  mTop = nullptr;
}

} /* namespace DE */
