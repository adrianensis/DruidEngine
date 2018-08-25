#ifndef STACKALLOCATOR_H_
#define STACKALLOCATOR_H_

#include "LinearAllocator.h"
#include "Basic.h"

namespace DE {

/*!
  \brief Allocates memory in FIFO strategy.
*/
class StackAllocator : public LinearAllocator {

private:

  void* mTop;

  static u32 smHeaderSize;
  void storeHeader(const void* address, const u32 size);

public:

  /*!
  \brief Default Constructor.
  */
  StackAllocator();

  /*!
  \brief Destructor.
  */
  ~StackAllocator() override;

  /*!
  \return Pointer to the top of the stack.
  */
  void* getTop();

  void init(const u32 size) override;
  void initFromMemory(const u32 size, void* mem) override;
  void* allocate(const u32 size) override;
  void* allocate(const u32 size, const u32 alignment) override;
  void free(const void* pointer) override;

  /*!
  \brief Free the top of the stack.
  */
  void free();

  void reset() override;

};

} /* namespace DE */

#endif /* STACKALLOCATOR_H_ */
