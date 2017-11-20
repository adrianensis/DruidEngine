#ifndef LINEARALLOCATOR_H_
#define LINEARALLOCATOR_H_

#include "Allocator.h"
#include "BasicTypes.h"

namespace DE {

/*!
    \brief Allocates memory in a linear way. The whole memory is freed in one shot.
*/
class LinearAllocator : public Allocator {

protected:
  u32 mOffset;
  bool mIsReverse;
public:

  /*!
     \brief Default Constructor.
  */
  LinearAllocator();

  /*!
     \brief Destructor.
  */
  virtual ~LinearAllocator();

  /*!
     \brief Sets the direction of allocation.
     \param isReverse Boolean.
  */
  void setReverse(bool isReverse);

  virtual void init(const u32 size);
  virtual void* allocate(const u32 size);
  virtual void* allocate(const u32 size, const u32 alignment);
  virtual void free(const void* pointer);
  virtual void freeAligned(const void* pointer);
  virtual void reset();

};

} /* namespace DE */

#endif /* LINEARALLOCATOR_H_ */
