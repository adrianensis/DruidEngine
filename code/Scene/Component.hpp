#pragma once

#include "Core/ObjectBase.hpp"

namespace DE {

class GameObject;
class ConfigMap;

class Component : public ObjectBase {

private:

	 bool mIsActive;
	 bool mIsStatic;

	 GameObject* mGameObject;
	 bool mAlreadyAddedToEngine;
	 bool mIsPendingToBeDestroyed;
	 bool mIsDestroyed;

public:

	GENERATE_METADATA(Component);

	Component();
	virtual ~Component() override;;

	GET_SET(GameObject);
	GET_SET(AlreadyAddedToEngine);
	GET(IsPendingToBeDestroyed);
	GET(IsDestroyed);

	virtual void init() = 0;

	bool isStatic();

	bool isActive() const {
		return (mIsDestroyed || mIsPendingToBeDestroyed || !mGameObject) ? false : mIsActive;
	};

	void setIsActive(bool isActive) {
		mIsActive = (mIsDestroyed || mIsPendingToBeDestroyed || !mGameObject) ? false : isActive;
	};

	void finallyDestroy() {
		mIsDestroyed = true;
		mIsPendingToBeDestroyed = false;
	};

	void destroy() {
		mIsPendingToBeDestroyed = true;
		mIsActive = false;
		onDestroy();
	};

	virtual void onDestroy() { };

	virtual void save(ConfigMap* configMap, StringRef objectName);
	virtual void load(ConfigMap* configMap, StringRef objectName);
};
}

