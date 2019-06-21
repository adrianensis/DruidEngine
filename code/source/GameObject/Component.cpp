#include "Component.h"

#include "GameObject.h"

namespace DE {

Component::Component() : DE_Class(),
		mGameObject(nullptr)
{
	// TODO Auto-generated constructor stub
}

Component::~Component() {
	// TODO Auto-generated destructor stub
}

GameObject* Component::getGameObject() const{
	return mGameObject;
}

void Component::setGameObject(GameObject* gameObject){
	mGameObject = gameObject;
}


} /* namespace DE */
