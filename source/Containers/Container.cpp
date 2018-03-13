#include "Container.h"
#include "BasicTypes.h"
#include "Memory.h"

namespace DE {

Container::Container(){
    mAllocator = nullptr;
};

Container::~Container() {

};

void Container::init(u32 length, u32 elementSize, u32 alignment) {
    mLength = length;
    mElementSize = elementSize;
    mAlignment = alignment;
    mAllocator = &Memory::getGlobal();
};

u32 Container::getLength() const {
    return mLength;
};

u32 Container::getElementSize() const {
    return mElementSize;
};

u32 Container::getAlignment() const {
    return mAlignment;
};

void Container::setAllocator(Allocator* allocator){
    mAllocator = allocator;
};

} /* namespace DE */
