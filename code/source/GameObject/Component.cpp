#include "Component.h"

#include "GameObject.h"

namespace DE {

Component::Component() : DE_Class(),
		gameObject(nullptr)
{
	// TODO Auto-generated constructor stub
}

Component::~Component() {
	// TODO Auto-generated destructor stub
}

GameObject* Component::getGameObject() const{
	return gameObject;
}

} /* namespace DE */
