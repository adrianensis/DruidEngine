#pragma once

#include "Core/DE_Class.hpp"

namespace DE {

class GameObject;
class ConfigMap;

class Component: public DE_Class {

private:

	DE_M(IsActive, bool)
	DE_M(IsStatic, bool)

	DE_M_GET_SET(GameObject, GameObject*)
	DE_M_GET_SET(AlreadyAddedToEngine, bool)
	DE_M_GET(IsPendingToBeDestroyed, bool)
	DE_M_GET(IsDestroyed, bool)

public:

	DE_CLASS(Component)

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

	virtual void save(ConfigMap* configMap, const std::string& objectName);
	virtual void load(ConfigMap* configMap, const std::string& objectName);
};

}

