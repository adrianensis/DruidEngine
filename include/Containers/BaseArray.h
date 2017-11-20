#ifndef BASEARRAY_H_
#define BASEARRAY_H_

#include "Container.h"
#include "BasicTypes.h"
#include "Allocator.h"

namespace DE {
    
class BaseArray : public Container {

protected:

  void* mStart;

  void fill(void* destiny, const void* source, const u32 size);

  void copy(const BaseArray& other);
  void init(const void* rawArray, const u32 length, const u32 elementSize);
  void init(const void* rawArray, const u32 length, const u32 elementSize, const u32 alignment);
  void init(const u32 length, const u32 elementSize);
  void init(const u32 length, const u32 elementSize, const u32 alignment);
  void allocate(const u32 length, const u32 elementSize, const u32 alignment);

public:

  BaseArray();
  virtual ~BaseArray();

  void set(const void* rawArray);
  void put(const void* rawArray, u32 index, const u32 length);

};

} /* namespace DE */

#endif /* BASEARRAY_H_ */
