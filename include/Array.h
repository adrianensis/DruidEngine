#ifndef ARRAY_H_
#define ARRAY_H_

#include "BaseArray.h"
#include "BasicTypes.h"
#include "Allocator.h"

#include <cstring>
#include <iostream>
#include <typeinfo>



namespace DE {

template <class T>
class Array : public BaseArray {

private:

  T* mStart;

  void fill(T* destiny, T* source, const u32 size) {
    std::memcpy(destiny, source, size);
  };

public:

  Array() : BaseArray(){};

  Array(const Array& other) : BaseArray(){
    init(other.mStart, other.mLength, other.mAllocator);
  };

  Array(const Array& other, Allocator* allocator) : BaseArray(){
    init(other.mStart, other.mLength, allocator);
  };

  ~Array(){};

  void init(T* array, u32 length, Allocator* allocator) {
    mStart = static_cast<T*>(allocate(length, sizeof(T), allocator));
    this->fill(mStart,array,length*sizeof(T));
  };

  void init(T* array, u32 length, u32 alignment, Allocator* allocator) {
    mStart = static_cast<T*>(allocate(length, sizeof(T), alignment, allocator));
    this->fill(mStart,array,length*sizeof(T));
  };

  // can be used for assignment
	T& operator[](const size_t i) {
		assert(i >= 0 && i < mLength, "Index out of bounds.");
		return mStart[i];
	}

};

} /* namespace DE */

#endif /* ARRAY_H_ */
