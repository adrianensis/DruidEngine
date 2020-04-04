#ifndef DE_COMPONENT_H
#define DE_COMPONENT_H

#include "DE_Class.h"

namespace DE {

class GameObject;

class Component : public DE_Class{

private:

	GameObject* mGameObject;
	bool mIsActive;
	bool mDestroy;

public:

	DE_CLASS(Component, DE_Class);

	virtual void init() = 0;

	GameObject* getGameObject() const { return mGameObject;};
	void setGameObject(GameObject* newGameObject ) { mGameObject = newGameObject; };

	bool isStatic();
	bool isActive() const { return mIsActive; };
	void setIsActive( bool isActive ) { mIsActive = isActive; };
	bool isDestroyed() const { return mDestroy; };
	void destroy() { mDestroy = true; mIsActive = false; };
};

} /* namespace DE */

#endif /* DE_COMPONENT_H */
