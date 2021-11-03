#pragma once

#include "Core/Core.hpp"

class GameObject;

class Component: public ObjectBase
{
    GENERATE_METADATA(Component)
	PRI_M(bool, IsActive, NONE)
	PRI_M(bool, IsStatic, NONE)

	PRI_M(GameObject *, GameObject, GET_SET)
	PRI_M(bool, AlreadyAddedToEngine, GET_SET)
	PRI_M(bool, IsPendingToBeDestroyed, GET)
	PRI_M(bool, IsDestroyed, GET)

PUB
	Component();

	virtual void init() = 0;

	virtual void onComponentAdded() { };

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

	SERIALIZE();
	DESERIALIZE();
};