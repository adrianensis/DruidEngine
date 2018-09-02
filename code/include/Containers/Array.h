#ifndef DE_ARRAY_H
#define DE_ARRAY_H

#include "SequentialContainer.h"
#include <cstring>

namespace DE {

/*!
  \brief Array of elements. Fixed size.
  \tparam Elements class.
*/
template <class T>
class Array : public SequentialContainer<T>{

private:

  T* mTStart;
  void* mStart;

  T& randomAccessOperator(const u32 index) const{
  DE_ASSERT(index >= 0 && index < this->getLength(), "Index out of bounds.");
    return mTStart[index];
  };

  void checkPut(const SequentialContainer<T>& other, const u32 destinyIndex, const u32 sourceIndex, const u32 length) override {
    DE_ASSERT(sourceIndex >= 0 && sourceIndex < other.getLength(), "sourceIndex is out of bounds.");
    DE_ASSERT(destinyIndex >= 0, "destinyIndex must be greater than 0.");
    DE_ASSERT(length <= other.getLength() - sourceIndex, "Not enough space to copy.");
    DE_ASSERT(length <= this->getLength() - destinyIndex, "Not enough space to put array.");
  };

  void raw_copy(const Array& other){
    raw_init(other.mStart, other.mLength, other.mElementSize, other.mAlignment);
  };

  void raw_init(const void* rawArray, const u32 length, const u32 elementSize) {
    raw_init(length, elementSize, 1);
    raw_set(rawArray);
  };

  void raw_init(const void* rawArray, const u32 length, const u32 elementSize, const u32 alignment) {
    raw_init(length, elementSize, alignment);
    raw_set(rawArray);
  };

  void raw_init(const u32 length, const u32 elementSize) {
    raw_allocate(length, elementSize, 1);
  };

  void raw_init(const u32 length, const u32 elementSize, const u32 alignment) {
    raw_allocate(length, elementSize, alignment);
  };

  void raw_set(const void* rawArray){
    std::memcpy(mStart,rawArray,this->mLength*(this->mElementSize));
  };

  void raw_put(const void* rawArray, const u32 destinyIndex, const u32 sourceIndex, const u32 length){
    std::memcpy(mStart+destinyIndex*(this->mElementSize),rawArray+sourceIndex*(this->mElementSize),length*(this->mElementSize));
  };

  void raw_allocate(const u32 length, const u32 elementSize, const u32 alignment) {
    BaseContainer::init(length,elementSize,alignment);
    mStart = this->mAllocator->allocate(this->mLength*(this->mElementSize), this->mAlignment);
    // TODO allocate with Memory::allocate, create new function for raw allocation.
  };

  void raw_clear() {
    BaseContainer::clear();
    std::memset(mStart, 0, this->mLength*(this->mElementSize));
  };

public:

  using SequentialContainer<T>::put; // because "put" method is ambiguous.

  /*!
  \brief Default Constructor.
  */
  Array() : SequentialContainer<T>()/*, BaseArray()*/{
    mStart = nullptr;
    mTStart = nullptr;
  };

  /*!
  \brief Destructor.
  */
  ~Array() {
    if(mStart != nullptr){
      this->mAllocator->free(mStart);
      mStart = nullptr;
      mTStart = nullptr;
    }
  };

  /*!
  \brief Copy Constructor.
  \param other Other Array.
  */
  void init(const Array<T>& other){
    raw_copy(other);
    mTStart = static_cast<T*>(mStart);
  };


  void fill(const T element) override {
    for (u32 i = 0; i < SequentialContainer<T>::mLength; i++)
    mTStart[i] = element;
  };

  void init(const void* rawArray, const u32 length) override {
    raw_init(rawArray, length, sizeof(T));
    mTStart = static_cast<T*>(mStart);
  };

  void init(const void* rawArray, const u32 length, const u32 alignment) override {
    raw_init(rawArray, length, sizeof(T), alignment);
    mTStart = static_cast<T*>(mStart);
  };

  void init(const u32 length) override {
    raw_init(length, sizeof(T));
    mTStart = static_cast<T*>(mStart);
  };

  void init(const u32 length, const u32 alignment) override {
    raw_init(length, sizeof(T), alignment);
    mTStart = static_cast<T*>(mStart);
  };

  void put(const Array<T>& other, const u32 destinyIndex, const u32 sourceIndex) {
    Array::put(other, destinyIndex, sourceIndex, other.getLength());
  };

  void put(const Array<T>& other, const u32 destinyIndex, const u32 sourceIndex, const u32 length) {
    this->checkPut(other, destinyIndex, sourceIndex, length);
    raw_put(other.mStart, destinyIndex, sourceIndex, length);
  };

  T get(const u32 index) const override {
    DE_ASSERT(index >= 0 && index < SequentialContainer<T>::getLength(), "Index out of bounds.");
    return mTStart[index];
  };

  void set(const u32 index, const T element) override {
    DE_ASSERT(index >= 0 && index < SequentialContainer<T>::getLength(), "Index out of bounds.");
    mTStart[index] = element;
  };

  void clear() override {
    raw_clear();
  };

  const T* getRawData() const{
     return mTStart;
   };

};

} /* namespace DE */

#endif /* DE_ARRAY_H */
