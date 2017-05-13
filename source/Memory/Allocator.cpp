#include "Allocator.h"
#include "Assert.h"
#include "BasicTypes.h"

// #include <iostream>

namespace DE {

  // using namespace std;

void Allocator::checkAllocate(const u32 size) {
  assert(mAllocated + size <= mTotalSize, "Total memory size exceeded.");
};

void Allocator::checkFree() {
  assert(mAllocated > 0, "Allocated memory is 0.");
};

void Allocator::setAllocated(u32 size){
  mAllocated = size;
}

Allocator::Allocator(){
  mStart = nullptr;
  mStartCopy = nullptr;
}

Allocator::~Allocator(){
  std::free(mStartCopy);
  mStart = nullptr;
  mStartCopy = nullptr;
}

u32 Allocator::getSize() {
  return mTotalSize;
};

u32 Allocator::getAllocated() {
  return mAllocated;
};

bool Allocator::hasSpace(const u32 size) {
  return (mTotalSize-mAllocated) >= size;
};

void Allocator::init(const u32 size) {
  mTotalSize = size;
  Allocator::reset();

  if(mStartCopy != nullptr)
    std::free(mStartCopy);

  mStart = std::malloc(size);

  mStartCopy = mStart;
};

void Allocator::reset() {
  mAllocated = 0;
};


} /* namespace DE */
