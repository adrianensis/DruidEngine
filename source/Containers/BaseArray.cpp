#include "BaseArray.h"

#include "BasicTypes.h"
#include "Allocator.h"
#include <cstring>


namespace DE {

  // using namespace std;

void BaseArray::fill(void* destiny, void* source, const u32 size) {
  std::memcpy(destiny, source, size);
};

void BaseArray::copy(const BaseArray& other, Allocator* allocator){
  if(other.mAlignment == 0)
    init(other.mStart, other.mLength, other.mElementSize, allocator);
  else
    init(other.mStart, other.mLength, other.mElementSize, other.mAlignment, allocator);
}


BaseArray::BaseArray() : Container(){

}

BaseArray::~BaseArray(){

};

void BaseArray::init(void* rawArray, const u32 length, const u32 elementSize, Allocator* allocator) {
  mStart = allocate(length, elementSize, allocator);
  fill(mStart,rawArray,mLength*mElementSize);
};

void BaseArray::init(void* rawArray, const u32 length, const u32 elementSize, const u32 alignment, Allocator* allocator) {
  mStart = allocate(length, elementSize, alignment, allocator);
  fill(mStart,rawArray,mLength*mElementSize);
};

void* BaseArray::allocate(const u32 length, const u32 elementSize, Allocator* allocator) {
  Container::init(length,elementSize,0,allocator);
  return allocator->allocate(mLength*mElementSize);
};

void* BaseArray::allocate(const u32 length, const u32 elementSize, const u32 alignment, Allocator* allocator) {
  Container::init(length,elementSize,alignment,allocator);
  return allocator->allocate(mLength*mElementSize, mAlignment);
};

} /* namespace DE */
