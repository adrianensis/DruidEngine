#include "Allocator.h"
#include "Assert.h"
#include "BasicTypes.h"

namespace DE {

void Allocator::checkAllocate(u32 size) {
    ASSERT(mAllocated + size <= mTotalSize, "Total memory size exceeded.");
}

void Allocator::checkFree() {
    ASSERT(mAllocated > 0, "Allocated memory is 0.");
}

void Allocator::setAllocated(u32 size){
    mAllocated = size;
}

Allocator::Allocator(){
    mStart = nullptr;
    mStartCopy = nullptr;
}

Allocator::~Allocator(){
    delete mStartCopy;
    mStart = nullptr;
    mStartCopy = nullptr;
}

u32 Allocator::getSize() {
    return mTotalSize;
}

u32 Allocator::getAllocated() {
    return mAllocated;
}

bool Allocator::hasSpace(u32 size) {
    return (mTotalSize-mAllocated) >= size;
}

void Allocator::_init(void* mem) {
    if(mStartCopy != nullptr)
        delete mStartCopy;

    mStart = mem;
    mEnd = mStart + mTotalSize - 1;

    mStartCopy = mStart;
}

void Allocator::init(u32 size) {
    initFromMemory(size, ::operator new (size));
}

void Allocator::initFromMemory(u32 size, void* mem) {
    mTotalSize = size;

    Allocator::reset();

    _init(mem);

    mStartCopy = mStart;
}

void Allocator::reset() {
    mAllocated = 0;
}


} /* namespace DE */
