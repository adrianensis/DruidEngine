#include "BaseList.h"

#include "BasicTypes.h"
#include "Allocator.h"
#include "Container.h"

namespace DE {

BaseList::BaseNode::BaseNode() {

};

BaseList::BaseNode::~BaseNode() {

};

void BaseList::BaseNode::init() {
  mNext = nullptr;
  mPrev = nullptr;
  mElement = nullptr;
};

BaseList::BaseList() {

};

BaseList::~BaseList() {

};

void BaseList::allocationLoop(const u32 length) {

  u32 nodeSize = sizeof(BaseNode);
  BaseNode* node = nullptr;

  // Node 0
  mFirst = static_cast<BaseNode*>(mAllocator->allocate(nodeSize));
  mFirst->init();
  mLast = mFirst;

  // The rest. Nodes 1 to length-1
  for (u32 i = 1; i < length; i++) {
    node = static_cast<BaseNode*>(mAllocator->allocate(nodeSize));
    node->init();

    mLast->mNext = node;
    node->mPrev = mLast;
    mLast = node;
  }
};

void* BaseList::init(const u32 length, const u32 elementSize, Allocator* allocator) {
  return BaseList::allocate(length, elementSize, allocator);
};

void* BaseList::init(const u32 length, const u32 elementSize, const u32 alignment, Allocator* allocator) {
  return BaseList::allocate(length, elementSize, alignment, allocator);
};

void* BaseList::allocate(const u32 length, const u32 elementSize, Allocator* allocator) {
  Container::init(length,elementSize,0,allocator);
  allocationLoop(length);
  return mFirst;
};

void* BaseList::allocate(const u32 length, const u32 elementSize, const u32 alignment, Allocator* allocator) {
  Container::init(length,elementSize,alignment,allocator);
  allocationLoop(length);
  return mFirst;
};



} /* namespace DE */
