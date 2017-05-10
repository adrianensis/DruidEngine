#include "Allocator.h"
#include "Assert.h"
#include "BasicTypes.h"

namespace DE {

void Allocator::checkSpace(const u32 size) {
  assert(mAllocated + size <= mTotalSize, "Total memory size exceeded.");
};

Allocator::Allocator(){
}

Allocator::~Allocator(){
}

u32 Allocator::getSize() {
  return mTotalSize;
};

u32 Allocator::getAllocated() {
  return mAllocated;
};

bool Allocator::hasSpace(const u32 size) {
  return mAllocated;
};

void Allocator::init(const u32 size) {
  mTotalSize = size;
  Allocator::reset();
};

void Allocator::reset() {
  mAllocated = 0;
};


} /* namespace DE */
