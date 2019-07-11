#ifndef DE_COMPONENT_H
#define DE_COMPONENT_H

#include "DE_Class.h"

namespace DE {

class GameObject;

class Component : public DE_Class{

private:

	GameObject* mGameObject;

public:

	DE_CLASS(Component);

	virtual void init() = 0;

	void setGameObject(GameObject* gameObject);
	GameObject* getGameObject() const;
};

} /* namespace DE */

#endif /* DE_COMPONENT_H */
