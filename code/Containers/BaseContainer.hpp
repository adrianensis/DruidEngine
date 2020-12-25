#ifndef DE_BASECONTAINER_H
#define DE_BASECONTAINER_H

#include "Basic.hpp"
#include "Allocator.hpp"
#include <string>

namespace DE {

/*!
 \brief Generic container.
 */
class BaseContainer: public DE_Class {

protected:

	u32 mLength;
	u32 mElementSize;
	u32 mAlignment;
	Allocator* mAllocator;

	void init(u32 length, u32 elementSize, u32 alignment);

public:

	DE_CLASS(BaseContainer, DE_Class)

	DE_GET(Length)
	DE_GET(ElementSize)
	DE_GET(Alignment)
	DE_GET_SET(Allocator)

	/*!
	 \brief Clear the container.
	 */
	virtual void clear();
};

} /* namespace DE */

#endif /* DE_BASECONTAINER_H */
