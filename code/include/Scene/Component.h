#ifndef DE_COMPONENT_H
#define DE_COMPONENT_H

#include "DE_Class.h"

namespace DE {

class GameObject;

class Component : public DE_Class{

private:

	GameObject* mGameObject;

public:

	DE_CLASS(Component, DE_Class);

	virtual void init() = 0;

	DE_GET_SET(GameObject*, GameObject);

	bool getIsStatic();
};

} /* namespace DE */

#endif /* DE_COMPONENT_H */
