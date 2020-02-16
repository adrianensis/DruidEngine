#include "Component.h"

#include "GameObject.h"

namespace DE {

// ---------------------------------------------------------------------------

Component::Component() : DE_Class(){
	mGameObject = nullptr;
}

Component::~Component() = default;

// ---------------------------------------------------------------------------

bool Component::isStatic() {
	mGameObject->isStatic();
}

// ---------------------------------------------------------------------------

} /* namespace DE */
