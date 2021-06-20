#pragma once

#include "Core/ObjectBase.hpp"

class GameObject;
class ConfigObject;

CLASS(Component, ObjectBase)
{
	PRI(IsActive, NONE, bool)
	PRI(IsStatic, NONE, bool)

	PRI(GameObject, GET_SET, GameObject *)
	PRI(AlreadyAddedToEngine, GET_SET, bool)
	PRI(IsPendingToBeDestroyed, GET, bool)
	PRI(IsDestroyed, GET, bool)

public:
	Component();

	virtual void init() = 0;

	bool isStatic();

	bool isActive() const
	{
		return (mIsDestroyed || mIsPendingToBeDestroyed || !mGameObject) ? false : mIsActive;
	};

	void setIsActive(bool isActive)
	{
		mIsActive = (mIsDestroyed || mIsPendingToBeDestroyed || !mGameObject) ? false : isActive;
	};

	void finallyDestroy()
	{
		mIsDestroyed = true;
		mIsPendingToBeDestroyed = false;
	};

	void destroy();

	virtual void onDestroy(){};
};