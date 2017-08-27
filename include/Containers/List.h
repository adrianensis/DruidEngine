#ifndef LIST_H_
#define LIST_H_

#include "BaseList.h"
#include "BasicTypes.h"
#include "Allocator.h"
#include "Assert.h"
#include "Debug.h"

namespace DE {

template <class T>
class List : public BaseList {

public:

  class Iterator : public BaseIterator {
  public:

    Iterator () : BaseIterator(){ };

    Iterator(const BaseIterator& other) : BaseIterator(other){ };

    virtual ~Iterator (){ };

    virtual T& get(){
      return *(static_cast<T*>(BaseIterator::get()));
    };

    virtual void set(T& element){
      BaseIterator::set(static_cast<void*>(&element));
    };

    Iterator getNext(){
      return BaseIterator::getNext();
    };

    Iterator getPrev(){
      return BaseIterator::getPrev();
    };

  };

private:

  static u8 defaultComparator(const T& a, const T& b){
    return (a < b) ? 1 : (b < a) ? 2 : 0;
  };

  void qsort(u8 (*comparator)(const T& a, const T& b), i32 left, i32 right){

    i32 length = right - left + 1;

    if(length > 1){

      T pivot = this->get((left + length)/2);

      while(left <= right){

        while(comparator(this->get(left), pivot) == 1)
          left++;

        while(comparator(this->get(right), pivot) == 2)
          right--;

        if(left <= right){
          this->swap(left,right);
          left++;
          right--;
        }
      }

      qsort(comparator, 0, right);
      qsort(comparator, left, length-1);

    }
  };


public:

  List() : BaseList(){

  };

  virtual ~List(){};

  void init(const List& other){
    BaseList::copy(other);
  };

  void init() {
    BaseList::init(sizeof(T));
  };

  Iterator getIterator() const {
    return Iterator(BaseList::getIterator());
  };

  Iterator getRevIterator() const {
    return Iterator(BaseList::getRevIterator());
  };

  void pushFront(T element){
    BaseList::pushFront(&element);
  };

  void pushBack(T element){
    BaseList::pushBack(&element);
  };

  T popFront(){
    return *(static_cast<T*>(BaseList::popFront()));
  };

  T popBack(){
    return *(static_cast<T*>(BaseList::popBack()));
  };

  T get(u32 index) const{
    return *(static_cast<T*>(BaseList::get(index)));
  };

  void set(u32 index, T element){
    BaseList::set(index, static_cast<void*>(&element));
  };

  void swap(u32 index1, u32 index2){
    BaseList::swap(index1, index2);
  };

  void remove(u32 index){
    BaseList::remove(index);
  };

  void remove(Iterator& it){
    BaseList::remove(it);
  };

  void insert(u32 index, T element){
    BaseList::insert(index, &element);
  };

  void insert(Iterator& it, T element){
    BaseList::insert(it, &element);
  };

  void sort(){
    sort(defaultComparator);
  };

  void sort(u8 (*comparator)(const T& a, const T& b)){
    qsort(comparator, 0, this->getLength()-1);
  };


};

} /* namespace DE */

#endif /* LIST_H_ */
