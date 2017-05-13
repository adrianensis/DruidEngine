#ifndef LINEARALLOCATOR_H_
#define LINEARALLOCATOR_H_

#include "Allocator.h"
#include "BasicTypes.h"

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
  u32 mOffset;
  bool mIsReverse;
public:

  LinearAllocator();
  virtual ~LinearAllocator();
  void setReverse(bool isReverse);
  virtual void init(const u32 size);
  virtual void* allocate(const u32 size);
  virtual void* allocateAligned(const u32 size, const u32 alignment);
  virtual void free(const void* pointer);
  virtual void freeAligned(const void* pointer);
  virtual void reset();

};

} /* namespace DE */

#endif /* LINEARALLOCATOR_H_ */
