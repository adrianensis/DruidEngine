#ifndef LINEARALLOCATOR_H_
#define LINEARALLOCATOR_H_

#include "Allocator.h"
#include "Basic.h"

namespace DE {

/*!
  \brief Allocates memory in a linear way. The whole memory is freed in one shot.
*/
class LinearAllocator : public Allocator {

public:

  /*!
    \brief Default Constructor.
  */
  LinearAllocator();

  /*!
    \brief Destructor.
  */
  ~LinearAllocator() override;

  void init(const u32 size) override;
  void* allocate(const u32 size) override;
  void* allocate(const u32 size, const u32 alignment) override;
  void free(const void* pointer) override;
  void reset() override;

};

} /* namespace DE */

#endif /* LINEARALLOCATOR_H_ */
