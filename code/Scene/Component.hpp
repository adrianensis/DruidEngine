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

	VIR void init() = 0;

	VIR void onComponentAdded() { };

	bool isStatic();

	bool isActive() CNS
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

	VIR void onDestroy(){};

	SERIALIZE();
	DESERIALIZE();
};