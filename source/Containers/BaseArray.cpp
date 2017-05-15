#include "BaseArray.h"

#include "BasicTypes.h"
// #include "Allocatable.h"
#include "Allocator.h"

namespace DE {

  // using namespace std;

BaseArray::BaseArray() : Allocatable(){

}

BaseArray::~BaseArray(){

}

void BaseArray::_init(const u32 length, const u32 elementSize, Allocator* allocator) {
  mAllocator = allocator;
  mLength = length;
  mElementSize = elementSize;
};

void* BaseArray::allocate(const u32 length, const u32 elementSize, Allocator* allocator) {
  _init(length, elementSize, allocator);
  return allocator->allocate(length*elementSize);
};

void* BaseArray::allocate(const u32 length, const u32 elementSize, const u32 alignment, Allocator* allocator) {
  _init(length, elementSize, allocator);
  return allocator->allocate(length*elementSize, alignment);
};

u32 BaseArray::getLength() {
  return mLength;
};

} /* namespace DE */
