#ifndef BASECONTAINER_H_
#define BASECONTAINER_H_

#include "Basic.h"
#include "Allocator.h"

namespace DE {

/*!
  \brief Generic container.
*/
class BaseContainer : public DE_Class {

protected:

  u32 mLength;
  u32 mElementSize;
  u32 mAlignment;
  Allocator* mAllocator;

  void init(const u32 length, const u32 elementSize, const u32 alignment);

public:

  /*!
    \brief Default Constructor.
  */
  BaseContainer();

  /*!
  \brief Destructor.
  */
  ~BaseContainer() = default;

  /*!
    \return Container length.
  */
  u32 getLength() const;

  /*!
    \return Container element size.
  */
  u32 getElementSize() const;

  /*!
    \return Container alignment.
  */
  u32 getAlignment() const;

  /*!
    \brief Sets the allocator.
    \param allocator Pointer to Allocator.
  */
  void setAllocator(Allocator* allocator);

  /*!
    \brief Clear the container.
  */
  virtual void clear();
};

} /* namespace DE */

#endif /* BASECONTAINER_H_ */
