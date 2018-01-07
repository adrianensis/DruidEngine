#include "BaseArray.h"

#include "BasicTypes.h"
#include "Allocator.h"
#include <cstring>


namespace DE {

void BaseArray::copy(const BaseArray& other){
    BaseArray::init(other.mStart, other.mLength, other.mElementSize, other.mAlignment);
}


BaseArray::BaseArray() : Container(){

}

BaseArray::~BaseArray(){
    // NOTE: user must free memory externally, by using the choosen allocator.
};

void BaseArray::init(const void* rawArray, u32 length, u32 elementSize) {
    BaseArray::init(length, elementSize, 1);
    set(rawArray);
};

void BaseArray::init(const void* rawArray, u32 length, u32 elementSize, u32 alignment) {
    BaseArray::init(length, elementSize, alignment);
    set(rawArray);
};

void BaseArray::init(u32 length, u32 elementSize) {
    BaseArray::allocate(length, elementSize, 1);
};

void BaseArray::init(u32 length, u32 elementSize, u32 alignment) {
    BaseArray::allocate(length, elementSize, alignment);
};

void BaseArray::set(const void* rawArray){
    std::memcpy(mStart,rawArray,mLength*mElementSize);
};

void BaseArray::put(const void* rawArray, u32 destinyIndex, u32 sourceIndex, u32 length){
    std::memcpy(mStart+destinyIndex*mElementSize,rawArray+sourceIndex*mElementSize,length*mElementSize);
};

void BaseArray::allocate(u32 length, u32 elementSize, u32 alignment) {
    Container::init(length,elementSize,alignment);
    mStart = mAllocator->allocate(mLength*mElementSize, mAlignment);
};

} /* namespace DE */
