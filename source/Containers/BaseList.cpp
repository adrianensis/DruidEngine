#include "BaseList.h"

#include "BasicTypes.h"
#include "Allocator.h"
#include "Container.h"
#include "Assert.h"

#include <iostream>

using namespace std;

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

BaseList::BaseIterator::BaseIterator(const BaseIterator& other) {
  mNode = other.mNode;
  mReverse = other.mReverse;
};

BaseList::BaseIterator::~BaseIterator() {

};

void BaseList::BaseIterator::init(BaseNode* start) {
  mNode = start;
  mReverse = false;
};

bool BaseList::BaseIterator::hasNext() {
  if(mReverse)
    return (mNode->mPrev != nullptr);
  else
    return (mNode->mNext != nullptr);
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
 // NOTE: user must free memory externally, by using the choosen allocator.
};


void BaseList::allocate(const u32 elementSize, const u32 alignment) {
  Container::init(0, elementSize, alignment); // mLength = 0
};

void BaseList::init(const u32 elementSize) {
  BaseList::allocate(elementSize, 0);
};

void BaseList::copy(const BaseList& other){
  this->init(other.mElementSize);
  BaseIterator it = other.getIterator();

  for (; it.hasNext(); it.next())
    this->pushBack(it.get());

  this->pushBack(it.get());
}

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

void BaseList::clear(){
  BaseIterator it = BaseList::getIterator();
  for (; it.hasNext(); BaseList::remove(it));

  BaseList::remove(it); // remove last
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
    BaseNode* old = mFirst;

    if(! BaseList::isEmpty()){
      mFirst = mFirst->mNext;
      mFirst->mPrev = nullptr;
    }else{ // if len = 0
      mFirst = nullptr;
      mLast = nullptr;
    }

    // mAllocator->free(old);
    // BaseList::freeNode(old);
  }

  return element;
};

void* BaseList::popBack(){

  void* element = nullptr;

  if( ! BaseList::isEmpty()){
    mLength--;

    element = mLast->mElement;
    BaseNode* old = mLast;

    if(! BaseList::isEmpty()){
      mLast = mLast->mPrev;
      mLast->mNext = nullptr;
    }else{ // if len = 0
      mFirst = nullptr;
      mLast = nullptr;
    }

    // mAllocator->free(old);
    // BaseList::freeNode(old);
  }

  return element;
};

void* BaseList::get(u32 index) const{

  ASSERT(index >= 0 && index < mLength, "Index out of bounds.");

  u32 i = 0;
  BaseIterator it = BaseList::getIterator();

  for (; i < index && it.hasNext(); it.next())
    i++;

  return it.get();
};

void BaseList::remove(u32 index){

  ASSERT(index >= 0 && index < mLength, "Index out of bounds.");

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

    if(next != nullptr){
      next->mPrev = prev;

      it.mNode = next; // advance iterator.
    }

    //remove it.mNode
    // mAllocator->free(it.mNode);
    // BaseList::freeNode(it.mNode);

  }

};

void BaseList::insert(u32 index, void* element){

  ASSERT(index >= 0 && index < mLength, "Index out of bounds.");

  // mLength++;

  // TODO refactor loop
  u32 i = 0;
  BaseIterator it = BaseList::getIterator();

  for (; i < index && it.hasNext(); it.next())
    i++;

  BaseList::insert(it,element);
};

void BaseList::insert(BaseIterator& it, void* element){

  if(it.mNode != mFirst){
    BaseNode* node = newNode();
    node->init(element);

    // this function inserts new node before the current node (it.mNode)
    it.mNode->mPrev->mNext = node;

    node->mPrev = it.mNode->mPrev;
    node->mNext = it.mNode;

    mLength++;
  }else
    BaseList::pushFront(element);
};




} /* namespace DE */
