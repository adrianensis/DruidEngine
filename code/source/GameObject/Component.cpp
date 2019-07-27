#include "Component.h"

#include "GameObject.h"

namespace DE {

// ---------------------------------------------------------------------------

Component::Component() : DE_Class(){
	mGameObject = nullptr;
}

Component::~Component() = default;

// ---------------------------------------------------------------------------

GameObject* Component::getGameObject() const{ return mGameObject; }

void Component::setGameObject(GameObject* gameObject){ mGameObject = gameObject; }

// ---------------------------------------------------------------------------

} /* namespace DE */
