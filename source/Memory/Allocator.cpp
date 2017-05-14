#include "Allocator.h"
#include "Assert.h"
#include "BasicTypes.h"
#include <cstdlib>

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

void Allocator::_init(void* mem) {
  if(mStartCopy != nullptr)
    std::free(mStartCopy);

  mStart = mem;
  mEnd = mStart + mTotalSize;

  mStartCopy = mStart;
};

void Allocator::init(const u32 size) {
  initFromMemory(size, std::malloc(size));
};

void Allocator::initFromMemory(const u32 size, void* mem) {
  mTotalSize = size;

  Allocator::reset();

  _init(mem);

  mStartCopy = mStart;
};

void Allocator::reset() {
  mAllocated = 0;
};


} /* namespace DE */
