#pragma once

#include "Core/Basic.hpp"
#include "Memory/Allocator.hpp"
#include <string>

namespace DE {

/*!
 \brief Generic container.
 */
class BaseContainer: public DE_Class {

protected:

	DE_M_GET(Length, u32)
	DE_M_GET(ElementSize, u32)
	DE_M_GET(Alignment, u32)
	DE_M_GET_SET(Allocator, Allocator*)

	void init(u32 length, u32 elementSize, u32 alignment);

public:

	DE_CLASS_BODY(BaseContainer)

	/*!
	 \brief Clear the container.
	 */
	virtual void clear();
};
}

