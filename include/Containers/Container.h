#ifndef CONTAINER_H_
#define CONTAINER_H_

#include "BasicTypes.h"
#include "Allocable.h"
#include "Allocator.h"

namespace DE {

/*!
    \brief Generic container.
*/
class Container : public Allocable {

protected:

  u32 mElementSize;
  u32 mLength;
  u32 mAlignment;

  void init(const u32 length, const u32 elementSize, const u32 alignment);

public:

  /*!
     \brief Default Constructor.
  */
  Container();

  /*!
     \brief Destructor.
  */
  virtual ~Container();

  /*!
     \return Container length.
  */
  u32 getLength() const;

};

} /* namespace DE */

#endif /* CONTAINER_H_ */
