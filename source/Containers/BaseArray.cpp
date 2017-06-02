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
  // TODO
};

void BaseArray::init(void* rawArray, const u32 length, const u32 elementSize, Allocator* allocator) {
  allocate(length, elementSize, 0, allocator);
  fill(mStart,rawArray,mLength*mElementSize);
};

void BaseArray::init(void* rawArray, const u32 length, const u32 elementSize, const u32 alignment, Allocator* allocator) {
  allocate(length, elementSize, alignment, allocator);
  fill(mStart,rawArray,mLength*mElementSize);
};

void BaseArray::allocate(const u32 length, const u32 elementSize, const u32 alignment, Allocator* allocator) {
  Container::init(length,elementSize,alignment,allocator);

  if(mAlignment > 0)
    mStart = allocator->allocate(mLength*mElementSize, mAlignment);
  else
    mStart = allocator->allocate(mLength*mElementSize);
};

} /* namespace DE */
