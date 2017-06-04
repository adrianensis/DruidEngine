#ifndef BASEARRAY_H_
#define BASEARRAY_H_

#include "Container.h"
#include "BasicTypes.h"
#include "Allocator.h"

namespace DE {

class BaseArray : public Container {

private:

  void fill(void* destiny, void* source, const u32 size);

protected:

  void* mStart;

  void copy(const BaseArray& other, Allocator* allocator);
  void init(void* rawArray, const u32 length, const u32 elementSize, Allocator* allocator);
  void init(void* rawArray, const u32 length, const u32 elementSize, const u32 alignment, Allocator* allocator);
  void allocate(const u32 length, const u32 elementSize, const u32 alignment, Allocator* allocator);

public:

  BaseArray();
  virtual ~BaseArray();

};

} /* namespace DE */

#endif /* BASEARRAY_H_ */
