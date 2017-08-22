#ifndef ARRAY_H_
#define ARRAY_H_

#include "BaseArray.h"
#include "BasicTypes.h"
#include "Allocator.h"
#include "Allocable.h"
#include "Assert.h"


namespace DE {

template <class T>
class Array : public BaseArray {

private:

  T* mTStart;

public:

  Array() : BaseArray(){

  };

  virtual ~Array(){};

  void init(const Array<T>& other){
    BaseArray::copy(other);
    mTStart = static_cast<T*>(mStart);
  };

  void fill(T element, const u32 length){
    Array<T>::init(length);

    mTStart = static_cast<T*>(mStart);

    for (u32 i = 0; i < mLength; i++)
      mTStart[i] = element;
  };

  void fill(T element, const u32 length, const u32 alignment){
    Array<T>::init(length, alignment);

    mTStart = static_cast<T*>(mStart);

    for (u32 i = 0; i < mLength; i++)
      mTStart[i] = element;
  };

  void init(const void* rawArray, const u32 length) {
    BaseArray::init(rawArray, length, sizeof(T));
    mTStart = static_cast<T*>(mStart);
  };

  void init(const void* rawArray, const u32 length, const u32 alignment) {
    BaseArray::init(rawArray, length, sizeof(T), alignment);
    mTStart = static_cast<T*>(mStart);
  };

  void init(const u32 length) {
    BaseArray::init(length, sizeof(T));
    mTStart = static_cast<T*>(mStart);
  };

  void init(const u32 length, const u32 alignment) {
    BaseArray::init(length, sizeof(T), alignment);
    mTStart = static_cast<T*>(mStart);
  };

  void put(const Array<T>& other, u32 index){
    BaseArray::put(other.mStart, index, other.getLength()*mElementSize);
  };

  void put(const void* rawArray, u32 index, const u32 length){
    BaseArray::put(rawArray, index, length*mElementSize);
  };

  // can be used for assignment
	T& operator[](const size_t i) {
		ASSERT(i >= 0 && i < mLength, "Index out of bounds.");
		return mTStart[i];
	}

};

} /* namespace DE */

#endif /* ARRAY_H_ */
