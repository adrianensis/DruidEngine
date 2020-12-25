#include "Component.hpp"

#include "GameObject.hpp"

namespace DE {

// ---------------------------------------------------------------------------

Component::Component() : DE_Class() {
	mGameObject = nullptr;
	mIsActive = true;
	mIsDestroyed = false;
	mIsStatic = false;
}

Component::~Component() = default;

// ---------------------------------------------------------------------------

bool Component::isStatic() {

	if(mGameObject){
		mIsStatic = mGameObject->getIsStatic();
	}

	return mIsStatic;
}

// ---------------------------------------------------------------------------

} /* namespace DE */
