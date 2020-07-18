#include "BaseContainer.hpp"
#include "BasicTypes.hpp"
#include "Memory.hpp"

namespace DE {

// ---------------------------------------------------------------------------

BaseContainer::BaseContainer() : DE_Class() {
	mLength = 0;
	mElementSize = 0;
	mAlignment = 0;
	mAllocator = nullptr;
}

BaseContainer::~BaseContainer() = default;

// ---------------------------------------------------------------------------

void BaseContainer::init(u32 length, u32 elementSize, u32 alignment) {
	mLength = length;
	mElementSize = elementSize;
	mAlignment = alignment;
	mAllocator = &Memory::getGlobal();
}

// ---------------------------------------------------------------------------

void BaseContainer::clear() {
	mLength = 0;
}

// ---------------------------------------------------------------------------

} /* namespace DE */
