#ifndef DE_FREELISTALLOCATOR_H
#define DE_FREELISTALLOCATOR_H

#include "Basic.h"
#include "List.h"
#include "LinearAllocator.h"

namespace DE {

/*!
  \brief List of memory blocks.
*/
class FreeListAllocator : public Allocator {

private:

  class Block{
  public:
    void* unalignedAddress;
    u32 size;

    Block();
    ~Block();
    void init(void* unalignedAddress, const u32 size);

    Block& operator=(const Block& rhs) {
      if (this == &rhs) return *this; // handle self assignment
      this->unalignedAddress = rhs.unalignedAddress;
      this->size = rhs.size;
      return (*this);
    }
  };

  LinearAllocator mLinearAllocator; // TODO: change for Memory::allocate()
  List<Block>* mFreeBlocks; // TODO: change to Block*
  List<Block>* mUsedBlocks;

  Block allocateBlock(const u32 size);
  u32 freeBlock(void* unalignedAddress);

public:

  /*!
  \brief Default Constructor.
  */
  FreeListAllocator();

  /*!
  \brief Destructor.
  */
  ~FreeListAllocator() override;

  void init(const u32 size) override;
  void* allocate(const u32 size) override;
  void* allocate(const u32 size, const u32 alignment) override;
  void free(const void* pointer) override;
  void reset() override;

};

} /* namespace DE */

#endif /* DE_FREELISTALLOCATOR_H */
