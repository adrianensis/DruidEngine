#include "BaseList.h"

#include "BasicTypes.h"
#include "Allocator.h"
#include "Container.h"
#include "Assert.h"

namespace DE {

BaseList::BaseNode* BaseList::newNode(){
  return static_cast<BaseNode*>(mAllocator->allocate(smNodeSize));
};

const u32 BaseList::smNodeSize = sizeof(BaseNode);

BaseList::BaseNode::BaseNode() {
  BaseNode::init();
};

BaseList::BaseNode::~BaseNode() {

};

void BaseList::BaseNode::init() {
  mNext = nullptr;
  mPrev = nullptr;
  mElement = nullptr;
};

void BaseList::BaseNode::init(void* element) {
  BaseNode::init();
  mElement = element;
};

BaseList::BaseIterator::BaseIterator() {
  mNode = nullptr;
};

BaseList::BaseIterator::~BaseIterator() {

};

void BaseList::BaseIterator::init(BaseNode* start) {
  mNode = start;
};

bool BaseList::BaseIterator::hasNext() {
  if(mReverse)
    return mNode->mPrev != nullptr;
  else
    return mNode->mNext != nullptr;
};

void BaseList::BaseIterator::next() {
  if(mReverse)
    mNode = mNode->mPrev;
  else
    mNode = mNode->mNext;
};

void* BaseList::BaseIterator::get() {
  return mNode->mElement;
};

bool BaseList::BaseIterator::isReverse(){
  return mReverse;
};

void BaseList::BaseIterator::setReverse(bool isReverse){
  mReverse = isReverse;
};

BaseList::BaseList() : Container() {
  mFirst = nullptr;
  mLast = nullptr;
};

BaseList::~BaseList() {
 // TODO
};

// void BaseList::allocationLoop(const u32 length) {
//
//   BaseNode* node = nullptr;
//
//   // Node 0
//   mFirst = newNode();
//   mFirst->init();
//   mLast = mFirst;
//
//   // The rest. Nodes 1 to length-1
//   for (u32 i = 1; i < length; i++) {
//     node = newNode();
//     node->init();
//
//     mLast->mNext = node;
//     node->mPrev = mLast;
//     mLast = node;
//   }
// };

void BaseList::allocate(const u32 length, const u32 elementSize, const u32 alignment, Allocator* allocator) {
  Container::init(length, elementSize, alignment, allocator);
  // allocationLoop(length);
};

void BaseList::init(const u32 length, const u32 elementSize, Allocator* allocator) {
  BaseList::allocate(length, elementSize, 0, allocator);
};

void BaseList::init(const u32 length, const u32 elementSize, const u32 alignment, Allocator* allocator) {
  BaseList::allocate(length, elementSize, alignment, allocator);
};

BaseList::BaseIterator BaseList::getIterator() const{
  BaseIterator it;
  it.init(this->mFirst);
  return it;
};

BaseList::BaseIterator BaseList::getRevIterator() const{
  BaseIterator it;
  it.init(this->mLast);
  it.setReverse(true);
  return it;
};

bool BaseList::isEmpty(){
  return mLength == 0;
};

void BaseList::pushFront(void* element){

  BaseNode* node = newNode();
  node->init(element);

  if( ! BaseList::isEmpty()){
    mFirst->mPrev = node;
    node->mNext = mFirst;
  }else
    mLast = node; // if is empty then also sets mLast

  mFirst = node;

  mLength++;

};

void BaseList::pushBack(void* element){

  BaseNode* node = newNode();
  node->init(element);

  if( ! BaseList::isEmpty()){
    mLast->mNext = node;
    node->mPrev = mLast;
  }else
    mFirst = node; // if is empty then also sets mFirst

  mLast = node;

  mLength++;

};

void* BaseList::popFront(){

  void* element = nullptr;

  if( ! BaseList::isEmpty()){
    mLength--;

    element = mFirst->mElement;
    void* old = mFirst;

    if(! BaseList::isEmpty()){
      mFirst = mFirst->mNext;
      mFirst->mPrev = nullptr;
    }else{ // if len = 0
      mFirst = nullptr;
      mLast = nullptr;
    }

    mAllocator->free(old);
  }

  return element;
};

void* BaseList::popBack(){

  void* element = nullptr;

  if( ! BaseList::isEmpty()){
    mLength--;

    element = mLast->mElement;
    void* old = mLast;

    if(! BaseList::isEmpty()){
      mLast = mLast->mPrev;
      mLast->mNext = nullptr;
    }else{ // if len = 0
      mFirst = nullptr;
      mLast = nullptr;
    }

    mAllocator->free(old);
  }

  return element;
};

void* BaseList::get(u32 index) const{

  assert(index >= 0 && index < mLength, "Index out of bounds.");

  u32 i = 0;
  BaseIterator it = BaseList::getIterator();

  for (; i < index && it.hasNext(); it.next())
    i++;

  return it.get();
};

void BaseList::remove(u32 index){

  assert(index >= 0 && index < mLength, "Index out of bounds.");

  if( ! BaseList::isEmpty()){
    // mLength--;

    u32 i = 0;
    BaseIterator it = BaseList::getIterator();

    for (; i < index && it.hasNext(); it.next())
      i++;

    BaseList::remove(it);
  }

};

void BaseList::remove(BaseIterator& it){

  if( ! BaseList::isEmpty()){
    mLength--;

    BaseNode* prev = it.mNode->mPrev;
    BaseNode* next = it.mNode->mNext;

    if(prev != nullptr)
      prev->mNext = next;

    if(next != nullptr)
      next->mPrev = prev;

    //remove it.mNode
    mAllocator->free(it.mNode);
  }
};

void BaseList::insert(u32 index, void* element){

  assert(index >= 0 && index < mLength, "Index out of bounds.");

  // mLength++;

  // TODO refactor loop
  u32 i = 0;
  BaseIterator it = BaseList::getIterator();

  for (; i < index && it.hasNext(); it.next())
    i++;

  BaseList::insert(it,element);
};

void BaseList::insert(BaseIterator& it, void* element){

  BaseNode* node = newNode();
  node->init(element);

  it.mNode->mPrev->mNext = node;
  node->mPrev = it.mNode->mPrev;
  node->mNext = it.mNode;

  mLength++;
};




} /* namespace DE */
