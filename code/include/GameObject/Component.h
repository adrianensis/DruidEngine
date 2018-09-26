#ifndef DE_COMPONENT_H
#define DE_COMPONENT_H

#include "DE_Class.h"

namespace DE {

class GameObject;

class Component : public DE_Class{
private:
	GameObject* gameObject;
public:
	Component();
	~Component() override;

	GameObject* getGameObject() const;
};

} /* namespace DE */

#endif /* DE_COMPONENT_H */
