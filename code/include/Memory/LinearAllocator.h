#ifndef DE_LINEARALLOCATOR_H
#define DE_LINEARALLOCATOR_H

#include "Allocator.h"
#include "Basic.h"

namespace DE {

/*!
  \brief Allocates memory in a linear way. The whole memory is freed in one shot.
*/
class LinearAllocator : public Allocator {

public:

  DE_CLASS(LinearAllocator, Allocator)

  void init(u32 size) override;
  void* allocate(u32 size) override;
  void* allocate(u32 size, u32 alignment) override;
  void free(const void* pointer) override;
  void reset() override;

};

} /* namespace DE */

#endif /* DE_LINEARALLOCATOR_H */
