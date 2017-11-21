#include "Container.h"
#include "BasicTypes.h"

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
};

u32 Container::getLength() const {
    return mLength;
};

void Container::setAllocator(Allocator* allocator){
    mAllocator = allocator;
};

} /* namespace DE */
