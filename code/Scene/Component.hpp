#pragma once

#include "Core/Core.hpp"

class GameObject;

CLASS(Component, ObjectBase)
{
	PRI(bool, IsActive, NONE)
	PRI(bool, IsStatic, NONE)

	PRI(GameObject *, GameObject, GET_SET)
	PRI(bool, AlreadyAddedToEngine, GET_SET)
	PRI(bool, IsPendingToBeDestroyed, GET)
	PRI(bool, IsDestroyed, GET)

public:
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