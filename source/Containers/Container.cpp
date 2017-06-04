#include "Container.h"

#include "BasicTypes.h"
#include "Allocable.h"
#include "Allocator.h"

namespace DE {

Container::Container() : Allocable() {

};

Container::~Container() {

};

void Container::init(const u32 length, const u32 elementSize, const u32 alignment) {
  mLength = length;
  mElementSize = elementSize;
  mAlignment = alignment;
};

u32 Container::getLength() const {
  return mLength;
};

} /* namespace DE */
