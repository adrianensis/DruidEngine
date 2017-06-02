#ifndef ARRAY_H_
#define ARRAY_H_

#include "BaseArray.h"
#include "BasicTypes.h"
#include "Allocator.h"
#include "Assert.h"


namespace DE {

template <class T>
class Array : public BaseArray {

private:

  T* mTStart;

public:

  Array() : BaseArray(){

  };

  Array(const Array& other) : BaseArray(){
    copy(other, other.mAllocator);
    mTStart = static_cast<T*>(mStart);
  };

  Array(const Array& other, Allocator* allocator) : BaseArray(){
    copy(other, allocator);
    mTStart = static_cast<T*>(mStart);
  };

  ~Array(){};

  void init(void* rawArray, const u32 length, Allocator* allocator) {
    BaseArray::init(rawArray, length, sizeof(T), allocator);
    mTStart = static_cast<T*>(mStart);
  };

  void init(void* rawArray, const u32 length, const u32 alignment, Allocator* allocator) {
    BaseArray::init(rawArray, length, sizeof(T), alignment, allocator);
    mTStart = static_cast<T*>(mStart);
  };

  // can be used for assignment
	T& operator[](const size_t i) {
		assert(i >= 0 && i < mLength, "Index out of bounds.");
		return mTStart[i];
	}

};

} /* namespace DE */

#endif /* ARRAY_H_ */
