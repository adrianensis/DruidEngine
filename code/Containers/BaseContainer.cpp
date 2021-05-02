#include "BaseContainer.hpp"
#include "Core/BasicTypes.hpp"
#include "Memory/Memory.hpp"

namespace DE {

BaseContainer::BaseContainer() : ObjectBase() {
	mLength = 0;
	mElementSize = 0;
	mAlignment = 0;
	mAllocator = nullptr;
}

BaseContainer::~BaseContainer() = default;

void BaseContainer::init(u32 length, u32 elementSize, u32 alignment) {
	mLength = length;
	mElementSize = elementSize;
	mAlignment = alignment;
	mAllocator = &Memory::getGlobal();
}

void BaseContainer::clear() {
	mLength = 0;
}

}
