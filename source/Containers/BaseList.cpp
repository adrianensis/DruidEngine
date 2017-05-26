#include "BaseList.h"

#include "BasicTypes.h"
#include "Allocator.h"
#include "Container.h"

namespace DE {

BaseList::BaseList() {

};

BaseList::~BaseList() {

};

void BaseList::allocationLoop(const u32 length, /*const u32 elementSize,*/ const u32 alignment, Allocator* allocator) {
  for (u32 i = 0; i < length; i++) {
    // allocator->allocate(sizeof(BaseNode));
  }
};

void* BaseList::init(const u32 length, const u32 elementSize, Allocator* allocator) {

};

void* BaseList::init(const u32 length, const u32 elementSize, const u32 alignment, Allocator* allocator) {

};

void* BaseList::allocate(const u32 length, const u32 elementSize, Allocator* allocator) {
  Container::init(length,elementSize,0,allocator);
  // return allocator->allocate(length*elementSize);
};

void* BaseList::allocate(const u32 length, const u32 elementSize, const u32 alignment, Allocator* allocator) {
  Container::init(length,elementSize,alignment,allocator);
  // return allocator->allocate(length*elementSize, alignment);
};



} /* namespace DE */
