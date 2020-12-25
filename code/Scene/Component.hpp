#ifndef DE_COMPONENT_H
#define DE_COMPONENT_H

#include "DE_Class.hpp"

namespace DE {

class GameObject;

class Component: public DE_Class {

private:

	GameObject* mGameObject;
	bool mIsActive;
	bool mIsDestroyed;
	bool mIsPendingToBeDestroyed;
	bool mAlreadyAddedToEngine;
	bool mIsStatic;

public:

	DE_CLASS(Component, DE_Class)

	DE_GET_SET(GameObject)
	DE_GET_SET(AlreadyAddedToEngine)
	DE_GET(IsPendingToBeDestroyed)
	DE_GET(IsDestroyed)

	virtual void init() = 0;

	bool isStatic();

	bool isActive() const {
		return (mIsDestroyed || mIsPendingToBeDestroyed || !mGameObject) ? false : mIsActive;
	};

	void setIsActive(bool isActive) {
		mIsActive = (mIsDestroyed || mIsPendingToBeDestroyed || !mGameObject) ? false : isActive;
	};

	void setDestroyed() {
		mIsDestroyed = true;
		mIsPendingToBeDestroyed = false;
	};

	void destroy() {
		mIsPendingToBeDestroyed = true;
		mIsActive = false;
		onDestroy();
	};

	virtual void onDestroy() { };

};

} /* namespace DE */

#endif /* DE_COMPONENT_H */
