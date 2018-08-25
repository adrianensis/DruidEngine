#include "BaseContainer.h"
#include "BasicTypes.h"
#include "Memory.h"

namespace DE {

BaseContainer::BaseContainer() : DE_Class(){
  mAllocator = nullptr;
};

BaseContainer::~BaseContainer() {

};

void BaseContainer::init(const u32 length, const u32 elementSize, const u32 alignment) {
  mLength = length;
  mElementSize = elementSize;
  mAlignment = alignment;
  mAllocator = &Memory::getGlobal();
};

u32 BaseContainer::getLength() const {
  return mLength;
};

u32 BaseContainer::getElementSize() const {
  return mElementSize;
};

u32 BaseContainer::getAlignment() const {
  return mAlignment;
};

void BaseContainer::setAllocator(Allocator* allocator){
  mAllocator = allocator;
};

void BaseContainer::clear(){
  mLength = 0;
};

} /* namespace DE */
