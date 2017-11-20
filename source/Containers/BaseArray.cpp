#include "BaseArray.h"

#include "BasicTypes.h"
#include "Allocator.h"
#include <cstring>


namespace DE {

void BaseArray::fill(void* destiny, const void* source, u32 size) {
  std::memcpy(destiny, source, size);
};

void BaseArray::copy(const BaseArray& other){
  if(other.mAlignment == 0)
    init(other.mStart, other.mLength, other.mElementSize);
  else
    init(other.mStart, other.mLength, other.mElementSize, other.mAlignment);
}


BaseArray::BaseArray() : Container(){

}

BaseArray::~BaseArray(){
  // NOTE: user must free memory externally, by using the choosen allocator.
};

void BaseArray::init(const void* rawArray, u32 length, u32 elementSize) {
  BaseArray::init(length, elementSize, 0);
  set(rawArray);
};

void BaseArray::init(const void* rawArray, u32 length, u32 elementSize, u32 alignment) {
  BaseArray::init(length, elementSize, alignment);
  set(rawArray);
};

void BaseArray::init(u32 length, u32 elementSize) {
  BaseArray::allocate(length, elementSize, 0);
};

void BaseArray::init(u32 length, u32 elementSize, u32 alignment) {
  BaseArray::allocate(length, elementSize, alignment);
};

void BaseArray::set(const void* rawArray){
  fill(mStart,rawArray,mLength*mElementSize);
};

void BaseArray::put(const void* rawArray, u32 index, u32 length){
  fill(mStart+index*mElementSize,rawArray,length*mElementSize);
};

void BaseArray::allocate(u32 length, u32 elementSize, u32 alignment) {
  Container::init(length,elementSize,alignment);

  if(mAlignment > 0)
    mStart = mAllocator->allocate(mLength*mElementSize, mAlignment);
  else
    mStart = mAllocator->allocate(mLength*mElementSize);
};

} /* namespace DE */
