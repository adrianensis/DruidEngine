#ifndef DE_STACKALLOCATOR_H
#define DE_STACKALLOCATOR_H

#include "LinearAllocator.h"
#include "Basic.h"

namespace DE {

/*!
  \brief Allocates memory in FIFO strategy.
*/
class StackAllocator : public LinearAllocator {

private:

  void* mTop;

  static const u32 smHeaderSize;
  void storeHeader(const void* address, u32 size);

public:

  DE_CLASS(StackAllocator, LinearAllocator)

  /*!
  \return Pointer to the top of the stack.
  */
  void* getTop();

  void init(u32 size) override;
  void initFromMemory(u32 size, void* mem) override;
  void* allocate(u32 size) override;
  void* allocate(u32 size, u32 alignment) override;
  void free(const void* pointer) override;

  /*!
  \brief Free the top of the stack.
  */
  void free();

  void reset() override;

};

} /* namespace DE */

#endif /* DE_STACKALLOCATOR_H */
