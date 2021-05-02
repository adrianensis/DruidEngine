#pragma once

#include "Core/Basic.hpp"
#include "Memory/Allocator.hpp"
#include <string>

namespace DE {

/*!
 \brief Generic container.
 */
class BaseContainer : public ObjectBase {

protected:

	 u32 mLength;
	 u32 mElementSize;
	 u32 mAlignment;
	 Allocator* mAllocator;

	void init(u32 length, u32 elementSize, u32 alignment);

public:

	GENERATE_METADATA(BaseContainer);

	BaseContainer();
	virtual ~BaseContainer() override;

	GET(Length);
	GET(ElementSize);
	GET(Alignment);
	GET_SET(Allocator);

	/*!
	 \brief Clear the container.
	 */
	virtual void clear();
};
}

