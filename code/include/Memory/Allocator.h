#ifndef DE_ALLOCATOR_H
#define DE_ALLOCATOR_H

#include "Assert.h"
#include "Basic.h"

namespace DE {

/*!
  \brief Manages memory allocation.
*/
class Allocator : public DE_Class {

protected:

  u32 mTotalSize;
  u32 mAllocated;
  void* mStart;

  void checkAllocate(u32 size) const;
  void checkAlignment(u32 alignment) const;
  void checkFree() const;
  void* calculateAlignedAddress(const void* unalignedAddress, u32 alignment) const;
  void* calculateUnalignedAddress(const void* alignedAddress) const;
  void* allocateAlignedAddress(void* unalignedAddress, u32 size, u32 alignment);
  void setAllocated(u32 size);
  void clean(void* mem, u32 size);
  void _init(void* mem);

public:

  /*!
    \brief Default Constructor.
  */
  Allocator();

  /*!
    \brief Destructor.
  */
  ~Allocator() override;

  /*!
    \return Total size.
  */
  u32 getSize() const;

  /*!
    \return Amount of memory used.
  */
  u32 getAllocated() const;

  /*!
    \return True if space is enough.
    \param size Size you want to check.
  */
  bool hasSpace(u32 size) const;

  /*!
    \brief Constructor.
    \param size Amount of memory you want to allocate.
  */
  virtual void init(u32 size);

  /*!
    \brief Constructor.
    \param size Amount of memory you want to allocate.
    \param mem Pointer to pre-allocated memory chunk.
  */
  virtual void initFromMemory(u32 size, void* mem);

  /*!
    \brief Allocates memory.
    \param size Amount of memory you want to allocate.
    \return Pointer to memory chunk.
  */
  virtual void* allocate(u32 size) = 0;

  /*!
    \brief Allocates memory.
    \param size Amount of memory you want to allocate.
    \param alignment Bytes alignment.
    \return Pointer to memory chunk.
  */
  virtual void* allocate(u32 size, u32 alignment) = 0;

  /*!
    \brief Frees memory.
    \param pointer Pointer to memory.
  */
  virtual void free(const void* pointer) = 0;

  /*!
    \brief Frees aligned memory.
  */
  virtual void reset();

};

} /* namespace DE */

#endif /* DE_ALLOCATOR_H */
