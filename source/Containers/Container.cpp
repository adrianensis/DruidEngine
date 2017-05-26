#include "Container.h"

#include "BasicTypes.h"
#include "Allocatable.h"
#include "Allocator.h"

namespace DE {

Container::Container() : Allocatable() {

};

Container::~Container() {

};

void Container::init(const u32 length, const u32 elementSize, const u32 alignment, Allocator* allocator) {
  mLength = length;
  mElementSize = elementSize;
  mAlignment = alignment;
  Allocatable::init(allocator);
};

u32 Container::getLength() {
  return mLength;
};

} /* namespace DE */
