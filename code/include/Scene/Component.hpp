#ifndef DE_COMPONENT_H
#define DE_COMPONENT_H

#include "DE_Class.hpp"

namespace DE {

class GameObject;

class Component: public DE_Class {

private:

	GameObject *mGameObject;
	bool mIsActive;
	bool mIsDestroyed;
	bool mIsPendingToBeDestroyed;
	bool mAlreadyAddedToEngine;

public:

	DE_CLASS(Component, DE_Class)
	;

	virtual void init() = 0;

	GameObject* getGameObject() const {
		return mGameObject;
	}
	;
	void setGameObject(GameObject *newGameObject) {
		mGameObject = newGameObject;
	}
	;

	bool isAlreadyAddedToEngine() const {
		return mAlreadyAddedToEngine;
	}
	;
	void setAlreadyAddedToEngine(bool alreadyAddedToEngine) {
		mAlreadyAddedToEngine = alreadyAddedToEngine;
	}
	;

	bool isStatic();
	bool isActive() const {
		return mIsDestroyed || mIsPendingToBeDestroyed ? false : mIsActive;
	}
	;
	void setIsActive(bool isActive) {
		mIsActive = mIsDestroyed || mIsPendingToBeDestroyed ? false : isActive;
	}
	;
	bool isPendingToBeDestroyed() const {
		return mIsPendingToBeDestroyed;
	}
	;
	bool isDestroyed() const {
		return mIsDestroyed;
	}
	;
	void setDestroyed() {
		mIsDestroyed = true;
		mIsPendingToBeDestroyed = false;
	}
	; // NOTE : only the systems must use this method!
	void destroy() {
		mIsPendingToBeDestroyed = true;
		mIsActive = false;
	}
	;
};

} /* namespace DE */

#endif /* DE_COMPONENT_H */
