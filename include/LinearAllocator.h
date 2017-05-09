#ifndef LINEARALLOCATOR_H_
#define LINEARALLOCATOR_H_

#include "Allocator.h"

/*
 * https://en.wikipedia.org/wiki/Data_structure_alignment
 *
 *  ---- pseudo-code: computing padding ----
 * padding = (align - (offset mod align)) mod align
 * new offset = offset + padding = offset + (align - (offset mod align)) mod align
 */

namespace DE {

class LinearAllocator : public Allocator {

protected:
  void* mStart;
  std::size_t mOffset;
public:

  LinearAllocator();
  virtual ~LinearAllocator();
  virtual void init(const std::size_t size);
  virtual void* allocate(const std::size_t size);
  virtual void* allocateAligned(const std::size_t size, const std::size_t alignment);
  virtual void free(void* pointer);
  virtual void freeAligned(void* pointer);
  virtual void reset();

};

} /* namespace DE */

#endif /* LINEARALLOCATOR_H_ */
