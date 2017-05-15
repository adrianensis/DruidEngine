#ifndef ARRAY_H_
#define ARRAY_H_

#include "BaseArray.h"
#include "BasicTypes.h"
#include "Allocator.h"

#include <cstring>

namespace DE {

template <class T>
class Array : public BaseArray {

private:

  T* mStart;

  void fill(T* destiny, T* source, const u32 size){
    std::memcpy(destiny, source, size);
  };
public:

  Array() : BaseArray(){

  };

  ~Array(){

  };

    // using namespace std;


  void init(const T* array, const u32 length, Allocator* allocator) {
    mStart = (T*)(allocate(length, sizeof(T), allocator));
    // fill(mStart,array);
    std::memcpy(mStart, array, length*sizeof(T));
    // for (size_t i = 0; i < length; i++) {
    //   mStart[i] = array[i];
    // }
  };

  void init(const T* array, const u32 length, const u32 alignment, Allocator* allocator) {
    // mStart = (T*)(allocate(length, sizeof(T), alignment, allocator));
    // fill(mStart,array);
  };

  // can be used for assignment
	T& operator[](const size_t i) {
		assert(i >= 0 && i < mLength, "Index out of bounds.");
		return mStart[i];
	}

};

} /* namespace DE */

#endif /* ARRAY_H_ */
