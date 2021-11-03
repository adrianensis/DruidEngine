#pragma once

#include "Core/Core.hpp"

class GameObject;

class Component: public ObjectBase
{
    GENERATE_METADATA(Component)
	PRI bool mIsActive = {};
	PRI bool mIsStatic = {};

	PRI GameObject * mGameObject = {}; GET_SET(GameObject)
	PRI bool mAlreadyAddedToEngine = {}; GET_SET(AlreadyAddedToEngine)
	PRI bool mIsPendingToBeDestroyed = {}; GET(IsPendingToBeDestroyed)
	PRI bool mIsDestroyed = {}; GET(IsDestroyed)

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