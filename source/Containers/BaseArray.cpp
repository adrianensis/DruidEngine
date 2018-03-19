#include "BaseArray.h"

#include <cstring>

namespace DE {

void BaseArray::copy(const BaseArray& other){
    BaseArray::init(other.mStart, other.mLength, other.mElementSize, other.mAlignment);
}


BaseArray::BaseArray() : BaseContainer(){

}

BaseArray::~BaseArray() = default;

void BaseArray::init(const void* rawArray, const u32 length, const u32 elementSize) {
    BaseArray::init(length, elementSize, 1);
    set(rawArray);
};

void BaseArray::init(const void* rawArray, const u32 length, const u32 elementSize, const u32 alignment) {
    BaseArray::init(length, elementSize, alignment);
    set(rawArray);
};

void BaseArray::init(const u32 length, const u32 elementSize) {
    BaseArray::allocate(length, elementSize, 1);
};

void BaseArray::init(const u32 length, const u32 elementSize, const u32 alignment) {
    BaseArray::allocate(length, elementSize, alignment);
};

void BaseArray::set(const void* rawArray){
    std::memcpy(mStart,rawArray,mLength*mElementSize);
};

void BaseArray::put(const void* rawArray, const u32 destinyIndex, const u32 sourceIndex, const u32 length){
    std::memcpy(mStart+destinyIndex*mElementSize,rawArray+sourceIndex*mElementSize,length*mElementSize);
};

void BaseArray::allocate(const u32 length, const u32 elementSize, const u32 alignment) {
    BaseContainer::init(length,elementSize,alignment);
    mStart = mAllocator->allocate(mLength*mElementSize, mAlignment);
};

void BaseArray::clear() {
    BaseContainer::clear();
    std::memset(mStart, 0, mLength*mElementSize);
};

} /* namespace DE */
