#include "StackAllocator.h"
#include <cstdint> // std::uintptr_t

using st = std::size_t;
using u8 = std::uint8_t;
using ptr = std::uintptr_t;
using ptrdiff = std::ptrdiff_t;

namespace DE {

StackAllocator::StackAllocator() : LinearAllocator(){

}

StackAllocator::~StackAllocator(){
  mTop = nullptr;
}

void* StackAllocator::allocate(const std::size_t size){
  return LinearAllocator::allocate(size);
}

void StackAllocator::init(const std::size_t size){
  LinearAllocator::init(size);
  mTop = mStart;
}

void* StackAllocator::allocateAligned(const std::size_t size, const std::size_t alignment){
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
